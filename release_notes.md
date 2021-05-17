---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## I2C Bootloader Applications Release v3.1.0

### New Features

- This release includes support of
    - I2C Bootloader Applications for PIC32CM MC family of 32-bit microcontrollers.

### Development kit and demo application support
- The following table provides bootloader demo applications available for different development kits.

    | Product Family                 | Development Kits                                    | I2C              | I2C Fail Safe             |
    | ------------------------------ | --------------------------------------------------- | ---------------- | ------------------------- |
    | SAM D09/D10/D11                | [SAM D11 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamd11-xpro)               | Yes              | NA                        |
    | SAM D20                        | [SAM D20 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD20-XPRO)   | Yes              | NA                        |
    | SAM D21/DA1                    | [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO)   | Yes              | NA                        |
    | SAM D21/DA1                    | [SAM DA1 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/ATSAMDA1-XPRO)        | Yes              | NA                        |
    | SAM HA1                        | [SAM HA1G16A Xplained Pro](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/ATSAMHA1G16A-XPRO)    | Yes              | NA                        |
    | SAM C20/C21                    | [SAM C21N Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMC21-XPRO)   | Yes              | NA                        |
    | SAM L21                        | [SAM L21 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML21-XPRO-B)             | Yes              | NA                        |
    | SAM L22                        | [SAM L22 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML22-XPRO-B)             | Yes              | NA                        |
    | SAM L10/L11                    | [SAM L10 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320204)                    | Yes              | NA                        |
    | SAM D5x/E5x                    | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO)               | Yes              | Yes                       |
    | PIC32CM MC                     | [PIC32CM MC00 Curiosity Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/EV15N46A)                    | Yes              | NA                        |

- **NA:** Lack of product capability

- The following WLCSP devices are shipped with preprogrammed bootloader

    | Part Number                    | I2C              |
    | ------------------------------ | ---------------- |
    | SAMD20 (ATSAMD20E15BU)         | Yes              |
    | SAMD20 (ATSAMD20E16BU)         | Yes              |


### Known Issues

- No changes from v3.0.0

### Development Tools

* [MPLAB® X IDE v5.50](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v3.00](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * MPLAB® Harmony 3 Launcher v3.6.4 and above.

## I2C Bootloader Applications Release v3.0.2

- Added discover.microchip.com metadata

### Known Issues
- No changes from v3.0.0

### Development Tools
- No changes from v3.0.0

## I2C Bootloader Applications Release v3.0.1

- Updated package.xml

### Known Issues
- No changes from v3.0.0

### Development Tools
- No changes from v3.0.0

## I2C Bootloader Applications Release v3.0.0

### New Features

- This release includes support of
    - I2C Bootloader Applications for Cortex-M0+ and Cortex-M4 based SAM family of 32-bit microcontrollers.
    - I2C Fail Safe Bootloader for devices with dual flash bank support.
    - Preprogrammed I2C Bootloader applications for WLCSP devices

### Development kit and demo application support
- The following table provides bootloader demo applications available for different development kits.

    | Product Family                 | Development Kits                                    | I2C              | I2C Fail Safe             |
    | ------------------------------ | --------------------------------------------------- | ---------------- | ------------------------- |
    | SAM D09/D10/D11                | [SAM D11 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamd11-xpro)               | Yes              | NA                        |
    | SAM D20                        | [SAM D20 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD20-XPRO)   | Yes              | NA                        |
    | SAM D21/DA1                    | [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO)   | Yes              | NA                        |
    | SAM D21/DA1                    | [SAM DA1 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/ATSAMDA1-XPRO)        | Yes              | NA                        |
    | SAM HA1                        | [SAM HA1G16A Xplained Pro](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/ATSAMHA1G16A-XPRO)    | Yes              | NA                        |
    | SAM C20/C21                    | [SAM C21N Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMC21-XPRO)   | Yes              | NA                        |
    | SAM L21                        | [SAM L21 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML21-XPRO-B)             | Yes              | NA                        |
    | SAM L22                        | [SAM L22 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML22-XPRO-B)             | Yes              | NA                        |
    | SAM L10/L11                    | [SAM L10 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320204)                    | Yes              | NA                        |
    | SAM D5x/E5x                    | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO)               | Yes              | Yes                       |

- **NA:** Lack of product capability

- The following WLCSP devices are shipped with preprogrammed bootloader

    | Part Number                    | I2C              |
    | ------------------------------ | ---------------- |
    | SAMD20 (ATSAMD20E15BU)         | Yes              |
    | SAMD20 (ATSAMD20E16BU)         | Yes              |


### Known Issues

The current known issues are as follows:

* Initialized global variables will not be initialized at startup for I2C bootloaders.


### Development Tools

* [MPLAB® X IDE v5.40](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.41](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * MPLAB® Harmony Configurator (MHC) v3.5.0 and above.
