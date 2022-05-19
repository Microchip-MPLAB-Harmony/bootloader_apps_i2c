// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
/*******************************************************************************
  Target Configuration Header

  File Name:
    i2c_target_config.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef I2C_TARGET_CONFIG_H
#define I2C_TARGET_CONFIG_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************

/* Include the Hex header file of the application to be programmed by the
 * I2C host bootloader application.
 * Specify the I2C slave address, Erase page size and Program Page size.
 * and the start address of the application being programmed.
 * For SAM D20, SAM D21, SAM C21N, SAM D11, SAM DA1, SAM L10, SAM L11, SAM L21, SAM L22,
 * PIC32CM, the Erase Page Size is 256 bytes and Program Page Size can be specified as
 * 64 bytes or equal to the erase page size (256)
 * #define APP_ERASE_PAGE_SIZE         (256L)
 * In case the embedded host has limited RAM, the APP_PROGRAM_PAGE_SIZE macro
 * can be set to the actual program page size (64 or 512) to reduce the RAM used
 * to hold the program data.
 * #define APP_PROGRAM_PAGE_SIZE       (64L)
 * Specify the user application start address. The application start address must
 * be aligned to erase page unit. If the bootloader itself is being upgraded then
 * the APP_IMAGE_START_ADDR must be set to 0x00 (start of bootloader). Ensure the
 * bootloader and application is also configured with the same value of application
 * start address.
 * #define APP_IMAGE_START_ADDR        0x800UL
 * For SAM E54, the Erase Page Size is 8192 bytes and Program Page Size is 512 bytes.
 * #define APP_ERASE_PAGE_SIZE         (8192L)
 * #define APP_PROGRAM_PAGE_SIZE       (512L)
 * #define APP_IMAGE_START_ADDR        0x2000UL
 * 
 * Specify if Fuse bits have to be programmed. If yes provide the devcgf.txt
 * file having all the necessary Fuse settings.
 * 
 * Expected Format for Device Configurations in text file.
 * Device Configurations for each row has to start with ROW_START followed by address
 * Device configurations should end with ROW_END
 * Each 32-Bit Fuse bit value has to be newline separated
 *
 * ROW_START 0x00804000
 * 0x78563412
 * 0x00EFCDAB
 * ROW_END
 *
 * ROW_START 0x0080C000
 * 0xF00FFFFF
 * 0xFFE81001
 * 0xFFFFFFFF
 * 0xFFFFFFFF
 * 0xFFFFFFFF
 * ...
 * ROW_END
 *
 * #define APP_PROGRAM_DEV_CONFIG      false
 * #define APP_DEVCFG_FILE             "devcfg.txt"
 */

/* Refer to user.h of the configuration to see the selected target device being upgraded
 * by the I2C bootloader host.
*/

#if APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_C21N_XPRO
#define APP_BINARY_FILE             "sam_c21n_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_D11_XPRO
#define APP_BINARY_FILE             "sam_d11_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_D20_XPRO
#define APP_BINARY_FILE             "sam_d20_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_D21_XPRO
#define APP_BINARY_FILE             "sam_d21_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_DA1_XPRO
#define APP_BINARY_FILE             "sam_da1_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_E54_XPRO
#define APP_BINARY_FILE             "sam_e54_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (8192L)
#define APP_PROGRAM_PAGE_SIZE       (8192L)
#define APP_IMAGE_START_ADDR        0x2000UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_HA1_XPRO
#define APP_BINARY_FILE             "sam_ha1_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_L10_XPRO
#define APP_BINARY_FILE             "sam_l10_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_L21_XPRO
#define APP_BINARY_FILE             "sam_l21_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == SAM_L22_XPRO
#define APP_BINARY_FILE             "sam_l22_xpro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#elif APP_I2C_BOOTLOADER_TARGET_DEVICE == PIC32CM_MC00_CURIOSITY_PRO
#define APP_BINARY_FILE             "pic32cm_mc00_curiosity_pro.X.production.bin"
#define APP_I2C_SLAVE_ADDR          0x0054
#define APP_ERASE_PAGE_SIZE         (256L)
#define APP_PROGRAM_PAGE_SIZE       (256L)
#define APP_IMAGE_START_ADDR        0x800UL
#define APP_PROGRAM_DEV_CONFIG      false
#define APP_DEVCFG_FILE             "devcfg.txt"

#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // I2C_TARGET_CONFIG_H
/*******************************************************************************
 End of File
*/
