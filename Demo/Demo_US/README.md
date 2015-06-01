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
