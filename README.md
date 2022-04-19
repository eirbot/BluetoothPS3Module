# Arduino small Bluetooth receiver module for PS3 joystick controller

<p float="center">
  <img src="img\bt_module_full_v2.png" width="70%" />
</p>

This repository will explain how to setup a small Bluetooth module to control basically anything with a PS3 controller (or any controller). The module uses a small Arduino Pro Mini, paired with a USB Host Shield Mini. The ATMEGA328P will load a complete USB software, to talk with a Bluetooth Dongle, and convert the PS3 controller commands into simple serial (UART) messages for the rest of the system. The bluetooth and USB protocols are quite heavy, that why we use an arduino board to ease the rest of the system (e.g. a robot).

<p float="center">
  <img src="img\module_diagram.svg" width="100%" />
</p>



## Materials

To create a module, you will need some materials :

- An Arduino Pro Mini **5V, 16MHz** with an **ATMEGA328** (the ATMEGA128 doesn't have enough memory to carry the USB library). Like [this](https://www.aliexpress.com/item/1005001621723982.html) or [this](https://www.digikey.com/en/products/detail/sparkfun-electronics/DEV-11113/5140820).
- A **USB Host Shield mini** 2.0, which can be plugged directly under an Arduino Pro Mini. Like [this](https://www.aliexpress.com/item/1005004123059904.html) or [this](https://www.amazon.com/1pcs-Mini-Host-Shield-Development/dp/B085X1MG5J/).
- Some 2.54mm female pin header, to solder on the USB Host Shield (you will need x24 at most).
- Some wire to bypass VBUS.
- One **3.3V LDO**, preferably in TO-92 package, like a LE33CZ or L78L33.
- One small Bluetooth dongle, like [this one](https://www.aliexpress.com/item/33002451753.html) (or any CSR 4.0 dongle). A non-exhaustive list can be found [here](https://github.com/felis/USB_Host_Shield_2.0/wiki/Bluetooth-dongles).
- One FT232RL USB to TTL board, to program the Arduino Pro Mini, with some jumper wires.
- A USB mini cable to connect to the FT232RL USB to TTL board.
- A PS3 joystick controller. You can use a chinese one, like [this one](https://www.aliexpress.com/item/4001296810227.html) or [this one](https://www.amazon.fr/Molyhood-Playstation3-Bluetooth-Vibration-T%C3%A9l%C3%A9commande/dp/B07YKYZVMB/).



## I] Hardware Setup

***Disclaimer (kind of) :** EIRBOT and I are not responsible for any damage you can do to your system with theses modifications. Keep in mind it's only for small DIY project with small budgets.*



To begin with, we have to modify the USB Host Shield to power the VBUS of the USB to 5V, instead of 3.3V. Most USB dongle could work in 3.3V, but the communication with the joystick controller will be very unstable. That why we use a 5V arduino pro mini : the 5V will be used to power the VBUS of the USB Host Shield, and an additional 3.3V LDO will be add to power the MAX3421E controller (which doesn't support 5V power input voltage).

>"Why use arduino pro mini 5V instead of using a 3.3V version and play with VRAW and internal Arduino LDO instead ?" :
>Because we intend to be able to plug the entire module directly in the system, using the six pins from the back of the arduino. In this configuration, we can "plug and play" the module, power it, and discuss with it using serial. That mean we only have access to VCC from the arduino pro mini programmable bus. We can't use VRAW. So, we can't use a 3.3V arduino pro mini to power the USB power rail in 5V (unless you are comfortable with using a buck boost, but that need a lot more components).

For easier implementation, be sure to get a LDO with the right pinout, from left to right VOUT, GND, VIN :

<p float="center">
  <img src="img\LDO_pinout.png" width="30%" />
</p>



0. First, get the USB Host shield upside down.
   
    <p float="center">
      <img src="img\HW_00.png" width="30%" />
    </p>
    
1. Then, cut the VIN trace with a cutter.
   
   <p float="center">
     <img src="img\HW_01.png" width="30%" />
   </p>
   
2. In the other side, cut the VBUS trace from coming to the MAX3421E.
   
   <p float="center">
     <img src="img\HW_02.png" width="30%" />
   </p>
   
3. Solder one part of the bypass wire in VBUS hole, the other part will be soldered at the end. Try to keep a lot of wire for the other side, for easier LDO soldering. 
   <p float="center">
     <img src="img\HW_03.png" width="20%" />
     <img src="img\HW_04.png" width="52%" />
   </p>

4. Reduce the pin of the LDO, except VIN, that you can keep a bit long, at 90°.
   
   <p float="left">
     <img src="img\HW_05.png" width="37%" />
     <img src="img\HW_05b.png" width="30%" />
   </p>
   
5. Now for the tricky part : place the LDO over the capacitor above the MAX3421E, and solder VOUT and GND directly in the capacitor.

   <p float="left">
     <img src="img\HW_06.png" width="40%" />
     <img src="img\HW_07.png" width="33%" />
   </p>

   > Be sure the LDO you use has the right PINOUT ! From left to right : VOUT / GND / VIN

6. Then you can solder VIN directly into the bypass wire.
   
    <p float="center">
      <img src="img\HW_08.png" width="60%" />
    </p>
    
    > Please note, this is not an ideal modification, because the input capacitor is missing. But it works fine for small currents like this module.
    >
    > Soldering directly in a capacitor is also not recommended, it is absolutely not IPC compliant. Keep in mind this is only for small DIY project.
    
7. You can now place and solder the two 2.54mm headers rows.
   
    <p float="center">
      <img src="img\HW_09.png" width="50%" />
    </p>
    
8. Last but not least, you can solder the other part of the bypass wire, directly into the VIN pin of the header (4th pin from bottom right).
   
    <p float="center">
      <img src="img\HW_10.png" width="20%" />
    </p>



Now you can use the usb host shield with a 5V Arduino Pro Mini.



## II] Software Setup

The software project use [PlatformIO](https://platformio.org/) to download all necessary librairies, build the program, and upload to the arduino pro mini.

0. First, install Platformio if not already done (you will need python 3) :

   ```shell
   $ pip install platformio
   ```

   Alternatively, you can use [VSCode](https://platformio.org/install/ide?install=vscode), which is officially supported by PlatformIO.

1. Download this git repository and open a command prompt at the root folder, where the `platformio.ini` file is located.

2. First, init the project :

   ```shell
   $ pio init
   ```

3. Then, run the project, to build all the project files :

   ```shell
   $ pip run
   ```

4. Connect the Arduino Pro Mini to the FT232RL USB to TTL board.

    <p float="center">
      <img src="img\SW_01.png" width="70%" />
    </p>

5. Be sure to set the FT232RL USB to TTL board jumper to 5V before connecting to your computer.

   <p float="center">
     <img src="img\SW_03.png" width="70%" />
   </p>

5. Now, upload the program to the board :

   ```shell
   $ pip run -t upload
   ```

6. Wait for the confirmation message :

    <details>
      <summary>Upload logs</summary>
    
    
    ```shell
    $ pio run -t upload
    Processing pro16MHzatmega328 (platform: atmelavr; board: pro16MHzatmega328; framework: arduino)
    -------------------------------------------------------------------------------------------------------------------------------------------------------
    Tool Manager: Installing platformio/tool-avrdude @ ~1.60300.0
    Downloading  [####################################]  100%
    Unpacking  [####################################]  100%
    Tool Manager: tool-avrdude @ 1.60300.200527 has been installed!
    Verbose mode can be enabled via `-v, --verbose` option
    CONFIGURATION: https://docs.platformio.org/page/boards/atmelavr/pro16MHzatmega328.html
    PLATFORM: Atmel AVR (3.4.0) > Arduino Pro or Pro Mini ATmega328 (5V, 16 MHz)
    HARDWARE: ATMEGA328P 16MHz, 2KB RAM, 30KB Flash
    DEBUG: Current (avr-stub) On-board (avr-stub, simavr)
    PACKAGES:
     - framework-arduino-avr 5.1.0
     - tool-avrdude 1.60300.200527 (6.3.0)
     - toolchain-atmelavr 1.70300.191015 (7.3.0)
    LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
    LDF Modes: Finder ~ chain, Compatibility ~ soft
    Found 6 compatible libraries
    Scanning dependencies...
    Dependency Graph
    |-- <PS3BT>
    |   |-- <SPI> 1.0
    Building in release mode
    Checking size .pio\build\pro16MHzatmega328\firmware.elf
    Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
    RAM:   [=====     ]  51.0% (used 1044 bytes from 2048 bytes)
    Flash: [======    ]  63.2% (used 19416 bytes from 30720 bytes)
    Configuring upload protocol...
    AVAILABLE: arduino
    CURRENT: upload_protocol = arduino
    Looking for upload port...
    Auto-detected: COM4
    Uploading .pio\build\pro16MHzatmega328\firmware.hex
    
    avrdude: AVR device initialized and ready to accept instructions
    
    Reading | ################################################## | 100% 0.01s
    
    avrdude: Device signature = 0x1e950f (probably m328p)
    avrdude: reading input file ".pio\build\pro16MHzatmega328\firmware.hex"
    avrdude: writing flash (19416 bytes):
    
    Writing | ################################################## | 100% 9.72s
    
    avrdude: 19416 bytes of flash written
    avrdude: verifying flash memory against .pio\build\pro16MHzatmega328\firmware.hex:
    avrdude: load data flash data from input file .pio\build\pro16MHzatmega328\firmware.hex:
    avrdude: input file .pio\build\pro16MHzatmega328\firmware.hex contains 19416 bytes
    avrdude: reading on-chip flash data:
    
    Reading | ################################################## | 100% 8.19s
    
    avrdude: verifying ...
    avrdude: 19416 bytes of flash verified
    
    avrdude: safemode: Fuses OK (E:00, H:00, L:00)
    
    avrdude done.  Thank you.
    
    ============================================================ [SUCCESS] Took 29.57 seconds ============================================================
    ```
    
    </details>

You can now disconnect the Ardunio Pro Mini from your computer, and plug it to the modified USB Host Shield.




## III] Bluetooth Setup

The next step is to setup the dongle bluetooth. This step is necessary IF you want to use a PS3 controller, because you have to set the Bluetooth address of the dongle internally to your PS3 Controller. If you want to use a different controller, feel free to check the [USB library readme](https://github.com/felis/USB_Host_Shield_2.0) to get the necessary setup.

**This should be done only one time.** The PS3 controller will always remember his associated dongle. You can't use multiple dongle for the same controller.



1. First, power up the module at VCC=5V. Depending of the dongle, it should blink. If not, reset the module (reset button on the arduino pro mini), and wait for the dongle to blink.
   
   <p float="center">
     <img src="img\BT_01.png" width="50%" />
   </p>
   
2. After 10 sec, unplug the Bluetooth dongle, and plug the PS3 controller instead using a USB mini cable (don't reset the module).
   
   <p float="center">
     <img src="img\BT_02.png" width="50%" />
   </p>
   
2. Wait 10sec again : the module will automatically write the Bluetooth Dongle Mac Adress to the PS3 controller.
   
3. Unplug the PS3 controller, then plug the dongle again. On the controller, push the "PS3" button (or "P3" button for the chinese one). The fours leds of the controller should blink rapidly.
   
   <p float="center">
     <img src="img\BT_03.png" width="50%" />
   </p>
   
4. Wait a few seconds, the controller should now connect automatically to the module. If not, try to reset the module. If it still doesn't work, go to step one again.
   
   <p float="center">
     <img src="img\BT_04.png" width="50%" />
   </p>



## IV] Integration Setup

Todo.

For more information about the serial output of the Arduino Pro Mini, check `src/main.cpp` file.



## TODO

* Finish integration setup.
* Fix bluetooth problem when sometime the Arduino Pro Mini has to be reset to "unlock" the Bluetooth dongle.
* Update USB Host Shield library, and ideally use Platformio to download from the official repository instead of this current copy.



## Sources

* USB Host shield library :
  * https://chome.nerpa.tech/mcu/usb-host-shield-library-version-2-0-released/#more-5449
  * https://github.com/felis/USB_Host_Shield_2.0
* USB Host shield hardware manual :
  * https://chome.nerpa.tech/usb-host-shield-hardware-manual/
  * https://www.pjrc.com/teensy/td_libs_USBHostShield.html
* MAX3421E Datasheet :
  https://datasheets.maximintegrated.com/en/ds/MAX3421E.pdf
* Mini USB Host Shield Schematic (not the v2, but close enough) :
  https://chome.nerpa.tech/chome_downloads/UHS_mini_10.pdf
* Arduino pro mini schematic :
  https://www.arduino.cc/en/uploads/Main/Arduino-Pro-Mini-schematic.pdf
* Eirbot Musics Playlists (especially [Eirboom](https://www.youtube.com/playlist?list=PLCQolDsR1jjFRk9cEoq9tnNmJ_i1eqMqh)) :
  https://www.youtube.com/user/Eirbot/playlists
