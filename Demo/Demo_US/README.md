#Roseline Audio + Firefly + ALPS for Ultarsonic People Counting

## Use
This section describes the use of the hardware + software system for people counting applications.

### Hardware Setup
**Corrections to the roseline audio board**

Cut off the 3.3V power connection to the Firefly ALPS board (FF-ALPS: Hirose 31 pin Connector, pin 1)

### Software Setup
Simply clone (or download) this repository on your **Beaglebone Black running Debian** and use the included Makefile in the min folder for compiling applications.

*The Makefiles in the individual application folders are not yet functional.*

[Finish this documentation later]

## Debug

### Hardware system

#### Chirp Transmission

As seen in the [Roseline audio board schematic](docs/Audio-Radio-Cape-v2_1.pdf), the connections between the Firefly ALPS & Beaglebone are Microcontroller reset (#3), I2C (#6,7), Serial (#8,9), GPIO Interrupt (#14), Audio Bit Clock (#20). The GPIO Interrupt is the primary (& currently implemented) method of trigger an audio chirp transmission.

Some interference has been known to occur due to interference on some of these channels. We suggest testing these thru the use of jumper cables between the Beaglebone & Audio board.

All pin configurations are described in the [Beaglebone Black System Reference Manual](https://github.com/CircuitCo/BeagleBone-Black/blob/master/BBB_SRM.pdf?raw=true)


1. Complete the all the relevant communications connections between 
the Beaglebone & audio board using jumper cables: I2C (9.19, 9.20), GPIO Interrupt (9.23), ALPS BCLK (8.10), Microphone input (9.37)
2. Connect the firefly ALPS with speaker setup to the Audio board Hirose connector.
3. Connect the +5V & GND lines on the BBB & the Audio board. (Use the SYS_5V (9.7, 9.8) for +5 V supply. All GND are internally connected). This should power on the audio board & ALPS Firefly.
4. If the ALPS firefly is programmed with the [audible pulse + 100ms chirp] application, it should give out a small audible tone once powered up. If this fails to happen, it is a power or communication problem.
5. To debug a power problem, ensure the ALPS receives both +5, GND using a probe. The ALPS Firefly may also heat up, in this case check the voltage regulator on the ALPS board.
6. To debug a communication problem, disconnect communication lines one at a time in the following order: BCLK, I2C, GPIO Interupt. Any of the other connections should not interfere with the chirp transmission.

You may also face a different set of problems while booting the Beaglebone.
7. Repeat the procedure in step (6) but while booting the Beaglebone at each turn.