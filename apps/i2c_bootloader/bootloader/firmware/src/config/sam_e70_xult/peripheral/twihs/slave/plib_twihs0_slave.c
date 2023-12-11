/*******************************************************************************
  TWIHS Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_twihs0_slave.c

  Summary
    TWIHS Slave peripheral library interface.

  Description

  Remarks:

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_twihs0_slave.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// TWIHS0 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

void TWIHS0_Initialize( void )
{
    /* Reset the TWIHS Module */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    /* Disable the TWIHS Master/Slave Mode */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    /* Configure slave address */
    TWIHS0_REGS->TWIHS_SMR = TWIHS_SMR_SADR(0x54);

    /* Clear the Transmit Holding Register and set TXRDY, TXCOMP flags */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_THRCLR_Msk;

    /* Initialize the TWIHS PLIB Object */

    /* Enable Slave Mode */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_SVEN_Msk;
}

uint8_t TWIHS0_ReadByte(void)
{
    return (uint8_t)(TWIHS0_REGS->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);
}

void TWIHS0_WriteByte(uint8_t wrByte)
{
    TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(wrByte);
}

TWIHS_SLAVE_TRANSFER_DIR TWIHS0_TransferDirGet(void)
{
    return ((TWIHS0_REGS->TWIHS_SR & TWIHS_SR_SVREAD_Msk) != 0U)? TWIHS_SLAVE_TRANSFER_DIR_READ: TWIHS_SLAVE_TRANSFER_DIR_WRITE;
}

TWIHS_SLAVE_ACK_STATUS TWIHS0_LastByteAckStatusGet(void)
{
    return ((TWIHS0_REGS->TWIHS_SR & TWIHS_SR_NACK_Msk) != 0U)? TWIHS_SLAVE_ACK_STATUS_RECEIVED_NAK : TWIHS_SLAVE_ACK_STATUS_RECEIVED_ACK;
}

void TWIHS0_NACKDataPhase(bool isNACKEnable)
{
    if (isNACKEnable == true)
    {
        TWIHS0_REGS->TWIHS_SMR = (TWIHS0_REGS->TWIHS_SMR & ~TWIHS_SMR_NACKEN_Msk) | TWIHS_SMR_NACKEN_Msk;
    }
    else
    {
        TWIHS0_REGS->TWIHS_SMR = (TWIHS0_REGS->TWIHS_SMR & ~TWIHS_SMR_NACKEN_Msk);
    }
}

TWIHS_SLAVE_STATUS_FLAG TWIHS0_StatusGet(void)
{
    return (TWIHS0_REGS->TWIHS_SR & (TWIHS_SR_SVACC_Msk | TWIHS_SR_EOSACC_Msk | TWIHS_SR_SVREAD_Msk | TWIHS_SR_TXRDY_Msk | TWIHS_SR_RXRDY_Msk | TWIHS_SR_NACK_Msk | TWIHS_SR_TXCOMP_Msk));
}
