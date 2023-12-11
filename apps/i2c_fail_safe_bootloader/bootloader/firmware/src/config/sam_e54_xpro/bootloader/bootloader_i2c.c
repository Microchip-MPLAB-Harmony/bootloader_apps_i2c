/*******************************************************************************
  I2C Bootloader Source File

  File Name:
    bootloader_i2c.c

  Summary:
    This file contains source code necessary to execute I2C bootloader.

  Description:
    This file contains source code necessary to execute I2C bootloader.
    It implements bootloader protocol which uses I2C peripheral to download
    application firmware into internal flash.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "bootloader_common.h"
#include <device.h>

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define SET_BIT(reg, bits)                      ((reg) |= (bits))
#define CLR_BIT(reg, bits)                      ((reg) &= ~(bits))
#define IS_BIT_SET(reg, bit)                    (((reg) & (bit))? (true):(false))

#define BL_BUFFER_SIZE                          ERASE_BLOCK_SIZE

#define BL_STATUS_BIT_BUSY                      ((uint8_t)0x01U << 0)
#define BL_STATUS_BIT_INVALID_COMMAND           ((uint8_t)0x01U << 1)
#define BL_STATUS_BIT_INVALID_MEM_ADDR          ((uint8_t)0x01U << 2)
#define BL_STATUS_BIT_COMMAND_EXECUTION_ERROR   ((uint8_t)0x01U << 3)      //Valid only when BL_STATUS_BIT_BUSY is 0
#define BL_STATUS_BIT_CRC_ERROR                 ((uint8_t)0x01U << 4)
#define BL_STATUS_BIT_COMM_ERROR                ((uint8_t)0x01U << 5)
#define BL_STATUS_BIT_ALL                       (BL_STATUS_BIT_BUSY | BL_STATUS_BIT_INVALID_COMMAND | BL_STATUS_BIT_INVALID_MEM_ADDR | \
                                                 BL_STATUS_BIT_COMMAND_EXECUTION_ERROR | BL_STATUS_BIT_CRC_ERROR | BL_STATUS_BIT_COMM_ERROR)

#define     BL_COMMAND_UNLOCK         0xA0U
#define     BL_COMMAND_ERASE          0xA1U
#define     BL_COMMAND_PROGRAM        0xA2U
#define     BL_COMMAND_VERIFY         0xA3U
#define     BL_COMMAND_RESET          0xA4U
#define     BL_COMMAND_READ_STATUS    0xA5U
#define     BL_COMMAND_BKSWAP_RESET   0xA6U
#define     BL_COMMAND_READ_VERSION    0xA8U
#define     BL_COMMAND_MAX             0xA9U

typedef uint8_t BL_COMMAND;

typedef enum
{
    BL_I2C_READ_COMMAND = 0,
    BL_I2C_READ_COMMAND_ARGUMENTS,
    BL_I2C_READ_PROGRAM_DATA,
}BL_I2C_READ_STATE;

typedef enum
{
    BL_FLASH_STATE_IDLE = 0,
    BL_FLASH_STATE_ERASE,
    BL_FLASH_STATE_WRITE,
    BL_FLASH_STATE_VERIFY,
    BL_FLASH_STATE_RESET,
    BL_FLASH_STATE_ERASE_BUSY_POLL,
    BL_FLASH_STATE_WRITE_BUSY_POLL,
    BL_FLASH_STATE_BKSWAP_RESET,
}BL_FLASH_STATE;

typedef struct
{
    uint32_t                                appImageStartAddr;
    uint32_t                                appImageSize;
}BL_COMMAND_PROTOCOL_UNLOCK;

typedef struct
{
    uint32_t                                nBytes;
    uint32_t                                memAddr;
    uint8_t                                 data[BL_BUFFER_SIZE];
}BL_COMMAND_PROTOCOL_PROGRAM;

typedef struct
{
    uint32_t                                memAddr;
}BL_COMMAND_PROTOCOL_ERASE;

typedef struct
{
    uint32_t                                crc;
}BL_COMMAND_PROTOCOL_VERIFY;

typedef union __ALIGNED(4)
{
    uint32_t                                cmdArg[2];
    BL_COMMAND_PROTOCOL_UNLOCK              unlockCommand;
    BL_COMMAND_PROTOCOL_ERASE               eraseCommand;
    BL_COMMAND_PROTOCOL_PROGRAM             programCommand;
    BL_COMMAND_PROTOCOL_VERIFY              verifyCommand;
}BL_COMMAND_PROTOCOL;

typedef struct
{
    int32_t                                 index;
    uint32_t                                nCmdArgWords;
    BL_I2C_READ_STATE                       rdState;
    BL_FLASH_STATE                          flashState;
    BL_COMMAND                              command;
    uint8_t                                 status;
    uint32_t                                appImageStartAddr;
    uint32_t                                appImageEndAddr;
    uint32_t                                nFlashBytesWritten;
    BL_COMMAND_PROTOCOL                     cmdProtocol;
}BL_PROTOCOL;



// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************

static BL_PROTOCOL blProtocol;

static bool i2cBLActive = false;

// *****************************************************************************
// *****************************************************************************
// Section: Bootloader Local Functions
// *****************************************************************************
// *****************************************************************************

static void BL_I2C_SendResponse(uint8_t command)
{
    static uint8_t numVersionBytesSent = 0;
    uint16_t btlVersion = 0;

    switch(command)
    {
        case BL_COMMAND_READ_STATUS:
            SERCOM3_I2C_WriteByte(blProtocol.status);

            /* Clear all status bits except the busy bit */
            CLR_BIT(blProtocol.status, (BL_STATUS_BIT_ALL & ~(BL_STATUS_BIT_BUSY)));

            break;

        case BL_COMMAND_READ_VERSION:
            btlVersion = bootloader_GetVersion();

            if (numVersionBytesSent == 0U)
            {
                SERCOM3_I2C_WriteByte((uint8_t)((btlVersion >> 8) & 0xFFU));

                numVersionBytesSent = 1;
            }
            else
            {
                SERCOM3_I2C_WriteByte((uint8_t)(btlVersion & 0xFFU));

                numVersionBytesSent = 0;
            }

            break;

        default:
            /* Do Nothing */
            break;
    }
}

static bool BL_I2C_MasterWriteHandler(uint8_t rdByte)
{
    switch(blProtocol.rdState)
    {
        case BL_I2C_READ_COMMAND:
            blProtocol.command = rdByte;
            /* Set default value of index to 3. Over-write below if needed */
            blProtocol.index = 3;

            blProtocol.nCmdArgWords = 0;

            if ((blProtocol.command < BL_COMMAND_UNLOCK) || (blProtocol.command >= BL_COMMAND_MAX))
            {
                SET_BIT(blProtocol.status, BL_STATUS_BIT_INVALID_COMMAND);
                return false;
            }
            else if (blProtocol.command == BL_COMMAND_RESET)
            {
                blProtocol.flashState = BL_FLASH_STATE_RESET;
            }
            else if (blProtocol.command == BL_COMMAND_BKSWAP_RESET)
            {
                blProtocol.flashState = BL_FLASH_STATE_BKSWAP_RESET;
            }
            else if ((blProtocol.command == BL_COMMAND_READ_STATUS) || (blProtocol.command == BL_COMMAND_READ_VERSION))
            {
                /* Do Nothing */
            }
            else
            {
                /* All commands transition to the BL_I2C_READ_COMMAND_ARGUMENTS state */
                blProtocol.rdState = BL_I2C_READ_COMMAND_ARGUMENTS;
            }
            break;
        case BL_I2C_READ_COMMAND_ARGUMENTS:
            ((uint8_t*)&blProtocol.cmdProtocol.cmdArg[blProtocol.nCmdArgWords])[blProtocol.index] = rdByte;
            blProtocol.index--;

            if (blProtocol.index < 0)
            {
                /* Program enters here after receiving each word of the command argument */
                blProtocol.nCmdArgWords++;

                if ((blProtocol.command == BL_COMMAND_UNLOCK) || (blProtocol.command == BL_COMMAND_PROGRAM))
                {
                    if (blProtocol.nCmdArgWords < 2U)
                    {
                        blProtocol.index = 3;
                    }
                }
            }

            if (blProtocol.index < 0)
            {
                /* Set default state to BL_I2C_READ_COMMAND. Over-write below if needed */
                blProtocol.rdState = BL_I2C_READ_COMMAND;

                if (blProtocol.command == BL_COMMAND_UNLOCK)
                {
                    /* Since this is the first command, clear any previously set status bits (host may be retrying and status may be set from previous communication) */
                    CLR_BIT(blProtocol.status, BL_STATUS_BIT_ALL);

                    /* Save application start address and size for future reference */
                    if ((blProtocol.cmdProtocol.unlockCommand.appImageStartAddr + blProtocol.cmdProtocol.unlockCommand.appImageSize) > (FLASH_START + FLASH_LENGTH))
                    {
                        SET_BIT(blProtocol.status, BL_STATUS_BIT_INVALID_MEM_ADDR);
                        return false;
                    }
                    else
                    {
                        blProtocol.appImageStartAddr = blProtocol.cmdProtocol.unlockCommand.appImageStartAddr;
                        blProtocol.appImageEndAddr = blProtocol.cmdProtocol.unlockCommand.appImageStartAddr + blProtocol.cmdProtocol.unlockCommand.appImageSize;
                    }
                }
                else if (blProtocol.command == BL_COMMAND_PROGRAM)
                {
                    if ((blProtocol.cmdProtocol.programCommand.memAddr < blProtocol.appImageStartAddr) || (blProtocol.cmdProtocol.programCommand.nBytes > BL_BUFFER_SIZE)
                     || ((blProtocol.cmdProtocol.programCommand.memAddr + blProtocol.cmdProtocol.programCommand.nBytes) > blProtocol.appImageEndAddr))
                    {
                        SET_BIT(blProtocol.status, BL_STATUS_BIT_INVALID_MEM_ADDR);
                        return false;
                    }
                    else
                    {
                        blProtocol.index = 0;
                        blProtocol.rdState = BL_I2C_READ_PROGRAM_DATA;
                    }
                }
                else if (blProtocol.command == BL_COMMAND_ERASE)
                {
                    if ((blProtocol.cmdProtocol.eraseCommand.memAddr >= blProtocol.appImageStartAddr) && ((blProtocol.cmdProtocol.eraseCommand.memAddr + ERASE_BLOCK_SIZE) <= blProtocol.appImageEndAddr))
                    {
                        SET_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
                        blProtocol.flashState = BL_FLASH_STATE_ERASE;
                    }
                    else
                    {
                        SET_BIT(blProtocol.status, BL_STATUS_BIT_INVALID_MEM_ADDR);
                        return false;
                    }
                }
                else if (blProtocol.command == BL_COMMAND_VERIFY)
                {
                   SET_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
                   blProtocol.flashState = BL_FLASH_STATE_VERIFY;
                }
                else
                {
                   /* Do Nothing */
                }
            }
            break;
        case BL_I2C_READ_PROGRAM_DATA:
            blProtocol.cmdProtocol.programCommand.data[blProtocol.index] = rdByte;
            blProtocol.index++;
            if (blProtocol.index >= (int32_t)blProtocol.cmdProtocol.programCommand.nBytes)
            {
                SET_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
                blProtocol.nFlashBytesWritten = 0;
                blProtocol.rdState = BL_I2C_READ_COMMAND;
                blProtocol.flashState = BL_FLASH_STATE_WRITE;
            }
            break;
        default:
            /* Do Nothing */
            break;
    }
    return true;
}

static void BL_I2C_EventsProcess(void)
{

    static bool isFirstRxByte;
    static bool transferDir;
    SERCOM_I2C_SLAVE_ERROR error;
    SERCOM_I2C_SLAVE_INTFLAG intFlags = SERCOM3_I2C_InterruptFlagsGet();

    if (((uint8_t)intFlags & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_ERROR) != 0U)
    {
        error = SERCOM3_I2C_ErrorGet();
        if (error != 0U)
        {
            /* Do nothing */
        }

        SERCOM3_I2C_InterruptFlagsClear(SERCOM_I2C_SLAVE_INTFLAG_ERROR);

        SET_BIT(blProtocol.status, BL_STATUS_BIT_COMM_ERROR);
    }
    else if (((uint8_t)intFlags & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_AMATCH) != 0U)
    {
        isFirstRxByte = true;
        i2cBLActive   = true;

        transferDir = (bool)SERCOM3_I2C_TransferDirGet();

        /* Reset the I2C read state machine */
        blProtocol.rdState = BL_I2C_READ_COMMAND;

        if (IS_BIT_SET(blProtocol.status, BL_STATUS_BIT_BUSY) != 0U)
        {
            SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_SEND_NAK);
        }
        else
        {
            SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_SEND_ACK);
        }
    }
    else if (((uint8_t)intFlags & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_DRDY) != 0U)
    {
        if (transferDir == (bool)SERCOM_I2C_SLAVE_TRANSFER_DIR_WRITE)
        {
            if (BL_I2C_MasterWriteHandler(SERCOM3_I2C_ReadByte()) == true)
            {
                SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_SEND_ACK);
            }
            else
            {
                SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_SEND_NAK);
            }
        }
        else
        {
            if ((isFirstRxByte == true) || (SERCOM3_I2C_LastByteAckStatusGet() == SERCOM_I2C_SLAVE_ACK_STATUS_RECEIVED_ACK))
            {
                BL_I2C_SendResponse(blProtocol.command);

                isFirstRxByte = false;

                SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_RECEIVE_ACK_NAK);
            }
            else
            {
                SERCOM3_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND_WAIT_FOR_START);
            }
        }

    }
    else if (((uint8_t)intFlags & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_PREC) != 0U)
    {
        SERCOM3_I2C_InterruptFlagsClear(SERCOM_I2C_SLAVE_INTFLAG_PREC);
    }
    else
    {
        /* Do nothing */
    }
}

static void BL_I2C_FlashTask(void)
{
    switch(blProtocol.flashState)
    {
        case BL_FLASH_STATE_ERASE:
            // Lock region size is always bigger than the row size
            NVMCTRL_RegionUnlock(blProtocol.cmdProtocol.eraseCommand.memAddr);

            while(NVMCTRL_IsBusy() == true)
            {
            }



            /* Erase the Current row */
            (void) NVMCTRL_BlockErase(blProtocol.cmdProtocol.eraseCommand.memAddr);

            blProtocol.flashState = BL_FLASH_STATE_ERASE_BUSY_POLL;
            break;

        case BL_FLASH_STATE_WRITE:
            (void) NVMCTRL_PageWrite((void *)&blProtocol.cmdProtocol.programCommand.data[blProtocol.nFlashBytesWritten], (blProtocol.cmdProtocol.programCommand.memAddr + blProtocol.nFlashBytesWritten));
            blProtocol.flashState = BL_FLASH_STATE_WRITE_BUSY_POLL;
            break;

        case BL_FLASH_STATE_VERIFY:
            if (bootloader_CRCGenerate(blProtocol.appImageStartAddr, (blProtocol.appImageEndAddr - blProtocol.appImageStartAddr)) != blProtocol.cmdProtocol.verifyCommand.crc)
            {
                SET_BIT(blProtocol.status, BL_STATUS_BIT_CRC_ERROR);
            }
            CLR_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
            blProtocol.flashState = BL_FLASH_STATE_IDLE;
            break;

        case BL_FLASH_STATE_ERASE_BUSY_POLL:
            if(NVMCTRL_IsBusy() == false)
            {
                CLR_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
                blProtocol.flashState = BL_FLASH_STATE_IDLE;
            }
            break;

        case BL_FLASH_STATE_WRITE_BUSY_POLL:
            if(NVMCTRL_IsBusy() == false)
            {
                blProtocol.nFlashBytesWritten += PAGE_SIZE;

                if (blProtocol.nFlashBytesWritten < blProtocol.cmdProtocol.programCommand.nBytes)
                {
                    blProtocol.flashState = BL_FLASH_STATE_WRITE;
                }
                else
                {
                    CLR_BIT(blProtocol.status, BL_STATUS_BIT_BUSY);
                    blProtocol.flashState = BL_FLASH_STATE_IDLE;
                }
            }
            break;

        case BL_FLASH_STATE_RESET:
            /* Wait for the I2C transfer to complete */
            while (((uint8_t)SERCOM3_I2C_InterruptFlagsGet() & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_PREC) == 0U)
            {
                /* Do Nothing */
            }

            bootloader_TriggerReset();
            break;

        case BL_FLASH_STATE_BKSWAP_RESET:
            /* Wait for the I2C transfer to complete */
            while (((uint8_t)SERCOM3_I2C_InterruptFlagsGet() & (uint8_t)SERCOM_I2C_SLAVE_INTFLAG_PREC) == 0U)
            {
                /* Do Nothing */
            }
            NVMCTRL_BankSwap();
            break;

        case BL_FLASH_STATE_IDLE:
            /* Do nothing */
            break;

        default:
            /* Do Nothing */
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Bootloader Global Functions
// *****************************************************************************
// *****************************************************************************

void bootloader_I2C_Tasks(void)
{
    do
    {
        if (IS_BIT_SET(blProtocol.status, BL_STATUS_BIT_BUSY) == false)
        {
            BL_I2C_EventsProcess();
        }

        BL_I2C_FlashTask();
    } while (i2cBLActive);
}
