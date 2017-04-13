#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)

RF24 radio(6,7);

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2,3,4	 };
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 5,9,10 };
const uint8_t num_led_pins = sizeof(led_pins);

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];
int led_values[num_led_pins];

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
	printf("\n\rRF24/examples/led_remote/\n\r");
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

	radio.openReadingPipe(1,pipe);

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
	int i = num_led_pins;
	while(i--)
	{
		pinMode(led_pins[i],OUTPUT);
		led_states[i] = HIGH;
		digitalWrite(led_pins[i],led_states[i]);
		led_states[i] = 255;
	}


	

}

//
// Loop
//

void loop(void)
{

	//
	// LED role.  Receive the state of all buttons, and reflect that in the LEDs
	//

	// if there is data ready
	if ( radio.available() )
	{
		// Dump the payloads until we've gotten everything
		while (radio.available())
		{
			// Fetch the payload, and see if this was the last one.
			radio.read( button_states, num_button_pins );

			// Spew it
			printf("Got buttons\n\r");
			int e =num_button_pins;


			while	(e--){

				Serial.println("Button "+e);
			Serial.println(button_states[e]);
			}

			// Increase R brightness
			if (button_states[0] && led_values[0] < 245) {
				led_values[0] +=10;
			Serial.println("R+ pushed, R=");
				Serial.println(led_values[0]);

			}
			// Decrease R Brightness
			if (button_states[1] && led_values[0] > 11) {
				led_values[0] -=10;
				Serial.println("R- pushed, R=");
				Serial.println(led_values[0]);
			}
			// Increase G brightness
			if (button_states[2] && led_values[1] < 245) {
				led_values[1] +=10;
				Serial.println("G+ pushed, G=");
				Serial.println(led_values[0]);
			}
			// Decrease G Brightness
			if (button_states[3] && led_values[1] > 11) {
				led_values[1] =-10;
				Serial.println("G- pushed, G=");
				Serial.println(led_values[0]);
			}
			// Increase B brightness
			if (button_states[4] && led_values[2] < 245) {
				led_values[2] +=10;
				Serial.println("B+ pushed, B=");
				Serial.println(led_values[0]);
			}
			// Decrease B Brightness
			if (button_states[5]  && led_values[2] > 11) {
				led_values[2] -=10;
				Serial.println("B- pushed, B=");
				Serial.println(led_values[0]);
			}

			// For each button, if the button now on, then toggle the LED
			int i = num_led_pins;
			while(i--)
			{
					led_states[i] ^= HIGH;
					//digitalWrite(led_pins[i],led_states[i]);
					analogWrite(led_pins[i], led_values[i]);
			}
		}
	}
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
