# PIC32MK MCJ Curiosity Pro: Building and Running the I2C Bootloader applications

Path of the application within the repository is **apps/i2c\_bootloader/**

To build the application, refer to the following table and open the project using its IDE.

**Bootloader Application**

|Project Name|Description|
|------------|-----------|
|bootloader/firmware/pic32mk\_mcj\_curiosity\_pro.X|MPLABX Project for [PIC32MK MCJ Curiosity Pro](https://www.microchip.com/en-us/development-tool/dt100113)|

**Test Application**

|Project Name|Description|
|------------|-----------|
|test\_app/firmware/pic32mk\_mcj\_curiosity\_pro.X|MPLABX Project for [PIC32MK MCJ Curiosity Pro](https://www.microchip.com/en-us/development-tool/dt100113)|

**Setting up [PIC32MK MCJ Curiosity Pro](https://www.microchip.com/en-us/development-tool/dt100113)**

-   [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/en-us/development-tool/atsamd21-xpro) is used as the **Host development kit** and [PIC32MK MCJ Curiosity Pro](https://www.microchip.com/en-us/development-tool/dt100113) is used as the **Target Development Kit**

    ![i2c_bootloader_host_target_connection](GUID-F8F581BB-09A3-46F9-AFF4-062DD2811E65-low.png)

-   Connect the I2C SDA line Pin 11 \(SDA2/RA8\) on EXT1 of the **Target Development Kit** to the I2C SDA line Pin 11 \(SERCOM2 PAD/PA08\) on EXT1 of the **Host development kit**

-   Connect the I2C SCL line Pin 12 \(SCL2/RB4\) on EXT1 of the **Host development kit** to the I2C SCL line Pin 12 \(SERCOM2 PAD/PA09\) on EXT1 of the **Host development kit**

-   Connect a ground wire between the Host development kit and Target development kit

-   Connect the Debug USB port on the Host development kit to the computer using a micro USB cable

-   Connect the Debug USB port on the Target development kit to the computer using a micro USB cable


**Building and Configuring I2C Host Applications**

**Using I2C SDCARD Host application to send the application binary to Target development kit**

![host_app_sdcard_setup](GUID-D175F964-8EE5-4362-9F37-A4DC77454196-low.png)

If the SDCARD Host Development Kit being used is other than [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/en-us/development-tool/atsamd21-xpro) then follow the steps mentioned in [Configuring SDCARD Host application project](GUID-8D59C55C-A3C0-4F4F-A391-F30292F6FC9F.md)

**configuring-the-sdcard-host-application**

1.  Open the SDCARD host application project *host\_app\_sdcard/firmware/sam\_d21\_xpro.X* in the IDE

    -   If a SDCARD host application project of different development kit is used then open that project in the IDE

2.  Make sure that the APP\_I2C\_BOOTLOADER\_TARGET\_DEVICE macro is set to the target board being programmed

3.  Build and program the SDCARD host application using the IDE on to the I2C host dvelopment kit

4.  Open the test application project *test\_app/firmware/pic32mk\_mcj\_curiosity\_pro.X* in the IDE

5.  Build the project to generate the binary **\(Do not program the binary\)**

6.  Connect the [I/O1 Xplained Pro Extension Kit](https://www.microchip.com/en-us/development-tool/atio1-xpro) on EXT2 header of [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/en-us/development-tool/atsamd21-xpro).

7.  Copy the application binary into the SD card and insert the SD card in the SD card slot available on the [I/O1 Xplained Pro Extension Kit](https://www.microchip.com/en-us/development-tool/atio1-xproCopy)

8.  Open the Terminal application \(Ex.:Tera Term\) on the computer

9.  Configure the serial port settings for **Host Development kit** as follows:

    -   Baud : 115200

    -   Data : 8 Bits

    -   Parity : None

    -   Stop : 1 Bit

    -   Flow Control : None

10. Jump to [Running The Application](#running-the-application)


**Running the Application**

1.  Open the bootloader project *bootloader/firmware/pic32mk\_mcj\_curiosity\_pro.X* in the IDE

2.  Build and program the application using the IDE on to the **Target development kit**

    -   **LED2** will be turned-on to indicate that bootloader code is running on the target

    -   **LED2** will also turn on when the bootloader does not find a valid application; i.e. the first word of the application \(stack pointer\), contains 0xFFFFFFFF

3.  **If the test application is being programmed**, Open the Terminal application \(Ex.:Tera Term\) on the computer and configure the serial port settings for **Target Development kit** as follows:

    -   Baud : 115200

    -   Data : 8 Bits

    -   Parity : None

    -   Stop : 1 Bit

    -   Flow Control : None

4.  Press the Switch **SW0** on the Host development kit to trigger programming of the application binary

5.  Once the programming is complete,

    -   **LED0** on the Host development kit will be turned on indicating success

    -   If **SDCARD Host application** was used to program the application binary following message will be displayed on the terminal of **Host development kit**

        ![i2c_bootloader_host_app_sdcard_output](GUID-64A63848-053C-4E65-8C2C-15E4A911DB65-low.png)

    -   The target development kit will be reset. Upon re-start, the boot-loader will jump to the user application

    -   If the test application is programmed then **LED2** should start blinking and you should see below output on the **Target development kit** console

        ![output](GUID-304634AD-F02D-4BFA-A530-C651923A9146-low.png)

6.  Press and hold the Switch **SW200** to trigger Bootloader from test application and you should see below output

    ![output](GUID-0DB8F538-D7FC-48CB-B30D-CB9F983C7047-low.png)

7.  Press Reset button on the Host development kit to reprogram the application binary

8.  Repeat Steps 4-5 once

    -   This step is to verify that bootloader is running after triggering bootloader from test application in Step 6


**Additional Steps \(Optional\)**

**Using SDCARD Host application**

-   If multiple Target development kit are to be programmed using the same Host development kit then refer to [Configuring SDCARD Host application project](GUID-8D59C55C-A3C0-4F4F-A391-F30292F6FC9F.md)

-   Once done repeat the applicable steps mentioned in [Running The Application](#running-the-application)


**Parent topic:**[I2C Bootloader](GUID-C4C2DFDC-C41B-4AB4-A500-170B6B69DF51.md)

