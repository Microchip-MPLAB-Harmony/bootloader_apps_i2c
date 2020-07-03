[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

To clone or download this application from Github,go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/bootloader_apps_i2c) and then click Clone button to clone this repo or download as zip file. This content can also be download using content manager by following [these instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki)

# I2C WLCSP Bootloader

This example application shows how to use the Bootloader Library to bootload an application using I2C protocol.

### Bootloader Application

- This is a bootloader application which resides from starting location of the device flash memory
- Bootloader runs from RAM to allow self-upgradation of the bootloader code itself.
- Trigger Methods:
    * By driving I2C SDA and SCL low on external reset. This assumes that the bootloader host application has control over the reset pin to ensure that execution of the bootloader starts after the values of SCL and SDA pins is settled to a desired level.
    * Pattern - 0x5048434D in each of the first 4 words (total 16 bytes), starting from RAM address - 0x20000000
    * No valid application. Application is considered invalid if the first 4 bytes of the application (which contains the starting address for the main stack pointer) are 0xFFFFFFFF
- Port pins used for I2C communication:
    * PA08 (SERCOM2_PAD0)
    * PA09 (SERCOM2_PAD1)
- Bootloader programs fuse settings to default values. Any custom fuse bit settings must be programmed by the application
- Bootloader uses I2C peripheral library in non-interrupt mode and implements two tasks:
    1. To process the I2C events
    2. To perform flash read/write/verify operations
- It is implemented in non-blocking mode thereby allowing other tasks to co-exist (if any)

### SDCARD Host Application
- This is a embedded I2C host application which sends the application image stored in the SD card to the target board over the I2C bus
- The user application binary is copied into an SD card and inserted in the SD card slot on the host board

    ![i2c_bootloader_host_sdcard](../docs/images/i2c_bootloader_host_sdcard.png)

## Targets
The following table provides links to documentation on how to build and run I2C WLCSP bootloader on SAMD20E15BU and SAMD20E16BU targets

| Development Kit |
|:---------|
|[SAMD20E15BU](docs/readme_sam_d20_e15.md) |
|[SAMD20E16BU](docs/readme_sam_d20_e16.md) |
