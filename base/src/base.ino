#include "RF24.h"
#include "nRF24L01.h"
#include "printf.h"
#include <SPI.h>

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)

RF24 radio( 7,8);

// Pins on the remote for buttons

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 3, 5, 6 };

uint8_t led_values[sizeof(led_pins)];
//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;



//
// Setup
//

void setup(void)
{

	//
	// Print preamble
	//

	Serial.begin(115200);
	printf_begin();
	printf("ROLE: BASE");

	//
	// Setup and configure rf radio
	//

	radio.begin();

	//
	// Open pipes to other nodes for communication
	//

	// This simple sketch opens a single pipes for these two nodes to communicate
	// back and forth.  One listens on it, the other talks to it.

	radio.openReadingPipe(1, pipe);

	//
	// Start listening
	//

	radio.startListening();

	//
	// Dump the configuration of the rf unit for debugging
	//

	radio.printDetails();

	//
	// Set up buttons / LED's
	//

	// Set pull-up resistors for all buttons

	// Turn LED's ON until we start getting keys
	int i = sizeof(led_values);
	while (i--) {
		pinMode(led_pins[i], OUTPUT);
		digitalWrite(led_pins[i], HIGH);
		delay(600);
		digitalWrite(led_pins[i], LOW);
	}


}

void loop(void)
{
	// if there is data ready
	if (radio.available()) {
		// Dump the payloads until we've gotten everything
		while (radio.available()) {
			// Fetch the payload, and see if this was the last one.
			radio.read(led_values, sizeof(led_values));

			//Set the color of the strip
			setStripColor();
		}
	}
}

// Set the color of the LED strip according to the current led_values
void setStripColor(){
	int i = sizeof(led_pins);
	while (i--) {
		analogWrite(led_pins[i], led_values[i]);
	}
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
