# SAM E70 Xplained Ultra Evaluation Kit: Building and Running the TWI Bootloader applications

Path of the application within the repository is **apps/i2c\_bootloader/**

To build the application, refer to the following table and open the project using its IDE.

**Bootloader Application**

|Project Name|Description|
|------------|-----------|
|bootloader/firmware/sam\_e70\_xult.X|MPLABX Project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113)|

**Test Application**

|Project Name|Description|
|------------|-----------|
|test\_app/firmware/sam\_e70\_xult.X|MPLABX Project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113)|

**Setting up [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113)**

-   [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113) is used for both **Host Development kit** and **Target Development kit**

    ![i2c_bootloader_host_target_connection](GUID-F8F581BB-09A3-46F9-AFF4-062DD2811E65-low.png)

-   Connect the TWI SDA line Pin 06 \(PA03\) on MikroBUS Header of the **Host development kit** to the TWI SDA line Pin 06 \(PA03\) on MikroBUS Header of the **Target development kit**

-   Connect the TWI SCL line Pin 05 \(PA04\) on MikroBUS Header of the **Host development kit** to the TWI SCL line Pin 05 \(PA04\) on MikroBUS Header of the **Target development kit**

-   Connect a ground wire from **Host development kit** to **Target development kit**

-   Connect the Jumper between pin1 and pin2 of J203 header.

-   Connect the Debug USB port on the Host development kit to the computer using a micro USB cable

-   Connect the Debug USB port on the Target development kit to the computer using a micro USB cable


**Building and Configuring TWI Host Applications**

**Using TWI NVM Host application to send the application binary to Target development kit**

![host_app_nvm_setup](GUID-9B48E66A-435C-4B28-969E-E8559987721C-low.png)

If the NVM Host Development Kit being used is other than [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113) then follow the steps mentioned in [Configuring NVM Host application project](GUID-E614E914-48BF-43EC-87B8-BAE0F81B83AE.md#)

1.  Open the NVM host application project *host\_app\_nvm/firmware/sam\_e70\_xult.X* in the IDE

    -   If a NVM host application project of different development kit is used then open that project in the IDE

2.  Build and program the NVM host application using the IDE on to the Host development kit

    -   The prebuilt test application image available under **host\_app\_nvm/firmware/src/test\_app\_images/image\_pattern\_hex\_sam\_e70\_xult.h** will be programmed on to the Target development kit with default **host\_app\_nvm** project configuration

3.  Jump to [Running The Application](#running-the-application)


**Using I2C SDCARD Host application to send the application binary to Target development kit**

![host_app_sdcard_setup](GUID-D175F964-8EE5-4362-9F37-A4DC77454196-low.png)

If the SDCARD Host Development Kit being used is other than [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113) then follow the steps mentioned in [Configuring SDCARD Host application project](GUID-8D59C55C-A3C0-4F4F-A391-F30292F6FC9F.md#)

1.  Open the SDCARD host application project *host\_app\_sdcard/firmware/sam\_e70\_xult.X* in the IDE

    -   If a SDCARD host application project of different development kit is used then open that project in the IDE

2.  Build and program the SDCARD host application using the IDE on to the I2C host dvelopment kit

3.  Open the test application project *test\_app/firmware/sam\_e70\_xult.X* in the IDE

4.  Build the project to generate the binary **\(Do not program the binary\)**

5.  Copy the application binary into the SD card and insert the SD card in the SD card slot available on the [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113)

6.  Open the Terminal application \(Ex.:Tera Term\) on the computer

7.  Configure the serial port settings for **Host Development kit** as follows:

    -   Baud : 115200

    -   Data : 8 Bits

    -   Parity : None

    -   Stop : 1 Bit

    -   Flow Control : None

8.  Jump to [Running The Application](#running-the-application)


**Running the Application**

1.  Open the bootloader project *bootloader/firmware/sam\_e70\_xult.X* in the IDE

2.  Build and program the application using the IDE on to the **Target development kit**

    -   **LED1** will be turned-on to indicate that bootloader code is running on the target

    -   **LED1** will also turn on when the bootloader does not find a valid application; i.e. the first word of the application \(stack pointer\), contains 0xFFFFFFFF

3.  **If the test application is being programmed**, Open the Terminal application \(Ex.:Tera Term\) on the computer and configure the serial port settings for **Target Development kit** as follows:

    -   Baud : 115200

    -   Data : 8 Bits

    -   Parity : None

    -   Stop : 1 Bit

    -   Flow Control : None

4.  Press the Switch **SW0** on the Host development kit to trigger programming of the application binary

5.  Once the programming is complete,

    -   **LED1** of Host development kit will turn on indicating success

    -   The target development kit will be reset. Upon re-start, the boot-loader will jump to the user application

    -   If the test application is programmed then **LED1** is off and **LED2** should start blinking and you should see below output on the **Target development kit** console

        ![output](GUID-304634AD-F02D-4BFA-A530-C651923A9146-low.png)

6.  Press and hold the Switch **SW0** to trigger Bootloader from test application and you should see below output

    ![output](GUID-0DB8F538-D7FC-48CB-B30D-CB9F983C7047-low.png)

7.  Press Reset button on the Host development kit to reprogram the application binary

8.  Repeat Steps 4-5 once

    -   This step is to verify that bootloader is running after triggering bootloader from test application in Step 6


**Additional Steps \(Optional\)**

**Using I2C NVM Host application**

-   To bootload any application other than **host\_app\_nvm/firmware/src/test\_app\_images/image\_pattern\_hex\_sam\_e70\_xult.h** refer to [Application Configurations](GUID-DBC21340-BFFA-466C-9909-E696C180A54E.md)

-   Once the application is configured, Refer to [Configuring NVM Host application project](GUID-E614E914-48BF-43EC-87B8-BAE0F81B83AE.md) for setting up the **host\_app\_nvm** project

-   Once done repeat the applicable steps mentioned in [Running The Application](#running-the-application)


**Using SDCARD Host application**

-   If multiple Target development kit are to be programmed using the same Host development kit then refer to [Configuring SDCARD Host application project](GUID-8D59C55C-A3C0-4F4F-A391-F30292F6FC9F.md)

-   Once done repeat the applicable steps mentioned in [Running The Application](#running-the-application)


**Parent topic:**[I2C Bootloader](GUID-C4C2DFDC-C41B-4AB4-A500-170B6B69DF51.md)

