# Arduino small Bluetooth receiver module for PS3 joystick controller

<img src="img\bt_module_full_v2.png" style="zoom:60%;" />

This repository will explain how to setup a small Bluetooth module to control basically anything with a PS3 controller (or any controller). The module uses a small Arduino Pro Mini, paired with a USB Host Shield Mini. The ATMEGA328P will load a complete USB software, to talk with a Bluetooth Dongle, and convert the PS3 controller commands into simple serial (UART) messages for the rest of the system. The bluetooth and USB protocols are quite heavy, that why we use an arduino board to ease the rest of the system (e.g. a robot).



<img src="img\module_diagram.svg" style="zoom:120%;" />



## Materials

To create a module, you will need some materials :

- An Arduino Pro Mini **5V/16MHz** with an **ATMEGA328** (the ATMEGA128 doesn't have enough memory to carry the USB library). Like [this](https://www.aliexpress.com/item/1005001621723982.html) or [this](https://www.digikey.com/en/products/detail/sparkfun-electronics/DEV-11113/5140820).
- A **USB Host Shield mini** 2.0, which can be plugged directly under an Arduino Pro Mini. Like [this](https://www.aliexpress.com/item/1005004123059904.html) or [this](https://www.amazon.com/1pcs-Mini-Host-Shield-Development/dp/B085X1MG5J/).
- Some 2.54mm female pin header, to solder on the USB Host Shield (you will need x24 at most).
- Some wire to bypass VBUS.
- One **3.3V LDO**, preferably in TO-92 package, like a LE33CZ or L78L33.
- One small Bluetooth dongle, like [this one](https://www.aliexpress.com/item/33002451753.html) (or any CSR 4.0 dongle). A non-exhaustive list can be found [here](https://github.com/felis/USB_Host_Shield_2.0/wiki/Bluetooth-dongles).
- One FT232RL USB to TTL board, to program the Arduino Pro Mini, with some jumper wires.
- A PS3 joystick controller. You can use a chinese one, like [this one](https://www.aliexpress.com/item/4001296810227.html) or [this one](https://www.amazon.fr/Molyhood-Playstation3-Bluetooth-Vibration-T%C3%A9l%C3%A9commande/dp/B07YKYZVMB/).



## I] Hardware Setup

To begin with, we have to modify the USB Host Shield to power the VBUS of the USB to 5V, instead of 3.3V. Most USB dongle could work in 3.3V, but the communication with the joystick controller will be very unstable. That why we use a 5V arduino pro mini : the 5V will be used to power the VBUS of the USB Host Shield, and an additional 3.3V LDO will be add to power the MAX3421E controller (which doesn't support 5V power input voltage).

For easier implementation, be sure to get a LDO with the right pinout, from left to right VOUT, GND, VIN :

<img src="img\LE33CZ_pinout.png" style="zoom:50%;" />



0. First, get the USB Host shield upside down.
    <img src="img\HW_00.png" style="zoom:30%;" />

1. Then, cut the VIN trace with a cutter.
   <img src="img\HW_01.png" style="zoom:20%;" />

2. In the other side, cut the VBUS trace from coming to the MAX3421E.
   <img src="img\HW_02.png" style="zoom:50%;" />

3. Solder one part of the bypass wire in VBUS hole, the other part will be soldered at the end. Try to keep a lot of wire for the other side, for easier LDO soldering. 
   <p float="left">
     <img src="img\HW_03.png" style="zoom:40%;" />
     <img src="img\HW_04.png" style="zoom:55%;" />
   </p>

4. Reduce the pin of the LDO, except VIN, that you can keep a bit long, at 90°.
   
   <p float="left">
     <img src="img\HW_05.png" style="zoom:50%;" />
     <img src="img\HW_05b.png" style="zoom:55%;" />
   </p>
   
5. Now for the tricky part : place the LDO over the capacitor above the MAX3421E, and solder VOUT and GND directly in the capacitor.

   <p float="left">
     <img src="img\HW_06.png" style="zoom:54%;" />
     <img src="img\HW_07.png" style="zoom:60%;" />
   </p>
   > Be sure the LDO you use has the right PINOUT ! From left to right : VOUT / GND / VIN

6. Then you can solder VIN directly into the bypass wire.
    <img src="img\HW_08.png" style="zoom:40%;" />

    > Please note, this is not an ideal modification, because the input capacitor is missing. But it works fine for small currents like this module.

7. You can now place and solder the two 2.54mm headers rows.
    <img src="img\HW_09.png" style="zoom:60%;" />

8. Last but not least, you can solder the other part of the bypass wire, directly into the VIN pin of the header (4th pin from bottom right).
    <img src="img\HW_10.png" style="zoom:40%;" />



Now you can use the usb host shield with a 5V Arduino Pro Mini.



## II] Software Setup

todo

## III] Bluetooth Setup

The next step is to setup the dongle bluetooth. This step is necessary IF you want to use a PS3 controller, because you have to set the Bluetooth address of the dongle internally to your PS3 Controller. If you want to use a different controller, feel free to check the [USB library readme](https://github.com/felis/USB_Host_Shield_2.0) to get the necessary setup.

**This should be done only one time.** The PS3 controller will always remember his associated dongle. You can't use multiple dongle for the same controller.



1. First, power up the module at VCC=5V. Depending of the dongle, it should blink. If not, reset the module (reset button on the arduino pro mini), and wait for the dongle to blink.
   <img src="img\BT_01.png" style="zoom:40%;" />

2. After 10 sec, unplug the Bluetooth dongle, and plug the PS3 controller instead (don't reset the module).
   <img src="img\BT_02.png" style="zoom:50%;" />

   Wait 10sec again : the module will automatically write the Bluetooth Dongle Mac Adress to the PS3 controller.

3. Unplug the PS3 controller, then plug the dongle again. On the controller, push the "PS3" button (or "P3" button for the chinese one). The fours leds of the controller should blink rapidly.
   <img src="img\BT_03.png" style="zoom:50%;" />
   
4. Wait a few seconds, the controller should now connect automatically to the module. If not, try to reset the module. If it still doesn't work, go to step one again.
   <img src="img\BT_04.png" style="zoom:50%;" />

