/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_i2c2_slave.c

  Summary:
    I2C PLIB Slave Implementation file

  Description:
    This file defines the interface to the I2C peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "device.h"
#include "plib_i2c2_slave.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
#define I2C2_SLAVE_DATA_SETUP_TIME_CORE_TIMER_CNTS          6
#define I2C2_SLAVE_RISE_TIME_CORE_TIMER_CNTS                18

static I2C_SLAVE_OBJ i2c2Obj;

void I2C2_Initialize(void)
{
    /* Turn off the I2C module */
    I2C2CONCLR = _I2C2CON_ON_MASK;

    I2C2CON = (_I2C2CON_STREN_MASK | _I2C2CON_AHEN_MASK | _I2C2CON_DHEN_MASK | _I2C2CON_PCIE_MASK );

    I2C2ADD = 0x54;

    I2C2MSK = 0x00;

    /* Clear slave interrupt flag */
    IFS1CLR = _IFS1_I2C2SIF_MASK;

    /* Clear fault interrupt flag */
    IFS1CLR = _IFS1_I2C2BIF_MASK;

    /* Enable the I2C Slave interrupt */
    IEC1SET = _IEC1_I2C2SIE_MASK;

    /* Enable the I2C Bus collision interrupt */
    IEC1SET = _IEC1_I2C2BIE_MASK;

    i2c2Obj.callback = NULL;

    /* Turn on the I2C module */
    I2C2CONSET = _I2C2CON_ON_MASK;
}

static void I2C2_RiseAndSetupTime(uint8_t sdaState)
{
    uint32_t startCount, endCount;

    if (sdaState == 0)
    {
        endCount = I2C2_SLAVE_DATA_SETUP_TIME_CORE_TIMER_CNTS;
    }
    else
    {
        endCount = I2C2_SLAVE_DATA_SETUP_TIME_CORE_TIMER_CNTS + I2C2_SLAVE_RISE_TIME_CORE_TIMER_CNTS;
    }

    startCount =_CP0_GET_COUNT();

    while((_CP0_GET_COUNT()- startCount) < endCount);
}

/* I2C slave state machine */
static void I2C2_TransferSM(void)
{
    uint32_t i2c_addr;
    uint8_t sdaValue = 0;

    /* ACK the slave interrupt */
    IFS1CLR = _IFS1_I2C2SIF_MASK;

    if (I2C2STAT & _I2C2STAT_P_MASK)
    {
        if (i2c2Obj.callback != NULL)
        {
            (void)i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_STOP_BIT_RECEIVED, i2c2Obj.context);
        }
    }
    else if ((I2C2STAT & _I2C2STAT_D_A_MASK) == 0)
    {
        if (I2C2STAT & _I2C2STAT_RBF_MASK)
        {
            /* Received I2C address must be read out */
            i2c_addr = I2C2RCV;
            (void)i2c_addr;

            if (i2c2Obj.callback != NULL)
            {
                /* Notify that a address match event has occurred */
                if (i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_ADDR_MATCH, i2c2Obj.context) == true)
                {
                    if (I2C2STAT & _I2C2STAT_R_W_MASK)
                    {
                        /* I2C master wants to read */
                        if (!(I2C2STAT & _I2C2STAT_TBF_MASK))
                        {
                            /* In the callback, slave must write to transmit register by calling I2Cx_WriteByte() */
                            (void)i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_TX_READY, i2c2Obj.context);
                        }
                    }
                    /* Send ACK */
                    I2C2CONCLR = _I2C2CON_ACKDT_MASK;
                }
                else
                {
                    /* Send NAK */
                    I2C2CONSET = _I2C2CON_ACKDT_MASK;
                    sdaValue = 1;
                }
                I2C2_RiseAndSetupTime(sdaValue);
            }
        /* Data written by the application; release the clock stretch */
        I2C2CONSET = _I2C2CON_SCLREL_MASK;
        }
    }
    else
    {
        /* Master reads from slave, slave transmits */
        if (I2C2STAT & _I2C2STAT_R_W_MASK)
        {
            if ((!(I2C2STAT & _I2C2STAT_TBF_MASK)) && (!(I2C2STAT & _I2C2STAT_ACKSTAT_MASK)))
            {
                if (i2c2Obj.callback != NULL)
                {
                    /* I2C master wants to read. In the callback, slave must write to transmit register */
                    (void)i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_TX_READY, i2c2Obj.context);

                    sdaValue = (i2c2Obj.lastByteWritten & 0x80);
                }

                I2C2_RiseAndSetupTime(sdaValue);

                /* Data written by the application; release the clock stretch */
                I2C2CONSET = _I2C2CON_SCLREL_MASK;
            }
        }
        /* Master writes to slave, slave receives */
        else
        {
            if (I2C2STAT & _I2C2STAT_RBF_MASK)
            {
                if (i2c2Obj.callback != NULL)
                {
                    /* I2C master wants to write. In the callback, slave must read data by calling I2Cx_ReadByte()  */
                    if (i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_RX_READY, i2c2Obj.context) == true)
                    {
                        /* Send ACK */
                        I2C2CONCLR = _I2C2CON_ACKDT_MASK;
                    }
                    else
                    {
                        /* Send NAK */
                        I2C2CONSET = _I2C2CON_ACKDT_MASK;
                        sdaValue = 1;
                    }

                    I2C2_RiseAndSetupTime(sdaValue);
                }
                /* Data read by the application; release the clock stretch */
                I2C2CONSET = _I2C2CON_SCLREL_MASK;
            }
        }
    }
}

void I2C2_CallbackRegister(I2C_SLAVE_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    i2c2Obj.callback = callback;
    i2c2Obj.context = contextHandle;
}

bool I2C2_IsBusy(void)
{
    if(I2C2STAT & _I2C2STAT_S_MASK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t I2C2_ReadByte(void)
{
    return I2C2RCV;
}

void I2C2_WriteByte(uint8_t wrByte)
{
    if (!(I2C2STAT & _I2C2STAT_TBF_MASK))
    {
        I2C2TRN = wrByte;
        i2c2Obj.lastByteWritten = wrByte;
    }
}

I2C_SLAVE_TRANSFER_DIR I2C2_TransferDirGet(void)
{
    return (I2C2STAT & _I2C2STAT_R_W_MASK) ? I2C_SLAVE_TRANSFER_DIR_READ : I2C_SLAVE_TRANSFER_DIR_WRITE;
}

I2C_SLAVE_ACK_STATUS I2C2_LastByteAckStatusGet(void)
{
    return (I2C2STAT & _I2C2STAT_ACKSTAT_MASK) ? I2C_SLAVE_ACK_STATUS_RECEIVED_NAK : I2C_SLAVE_ACK_STATUS_RECEIVED_ACK;
}

I2C_SLAVE_ERROR I2C2_ErrorGet(void)
{
    I2C_SLAVE_ERROR error;

    error = i2c2Obj.error;
    i2c2Obj.error = I2C_SLAVE_ERROR_NONE;

    return error;
}

void I2C2_BUS_InterruptHandler(void)
{
    /* Clear the bus collision error status bit */
    I2C2STATCLR = _I2C2STAT_BCL_MASK;

    /* ACK the bus interrupt */
    IFS1CLR = _IFS1_I2C2BIF_MASK;

    i2c2Obj.error = I2C_SLAVE_ERROR_BUS_COLLISION;

    if (i2c2Obj.callback != NULL)
    {
        i2c2Obj.callback(I2C_SLAVE_TRANSFER_EVENT_ERROR, i2c2Obj.context);
    }
}

void I2C2_SLAVE_InterruptHandler(void)
{
    I2C2_TransferSM();
}
