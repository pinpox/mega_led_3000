# mega_led_3000

![Alt text](images/title.png?raw=true "Title")

Remote + Controller for a RGB LED light strip.

The project consists of two parts: a base unit that is connected to the LED-strip and a remote control unit. Both are build using Arduinos and some other parts (see below) and communnicate over 2.4 GHz signals.

The sourcecode is separted in the respective folders so that it can be build and uploaded using the inotool for arduino (http://inotool.org/quickstart)

For the 2.4GHz communication the RF24 library is used (https://github.com/nRF24/RF24).

## Schematic
![Alt text](images/schematic_base.png?raw=true "Schemaric Base")
![Alt text](images/schemaric_remote.png?raw=true "Schemaric Remote")


## Partlist

#### Base unit
- Arduino Uno
- nRF24L01 2.4 GHz transciever
- 3x Resistor 220 Ohm
- 3x LED (red, green, blue)
- 3x Power Transistor

#### Remote
- Arduino Micro
- nRF24L01 2.4 GHz transciever
- 3x RGB-illuminated rotatory encoders

