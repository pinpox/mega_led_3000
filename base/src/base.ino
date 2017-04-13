#include "RF24.h"
#include "nRF24L01.h"
#include "printf.h"
#include <SPI.h>

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)

RF24 radio(6, 7);

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2, 3, 4, 5, 8, 9 };
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 5, 9, 10 };
const uint8_t num_led_pins = sizeof(led_pins);

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t button_states[num_button_pins];
int led_values[num_led_pins];

//Colors of the encoders: [encoder number][r,g or b]
int encoder_colors[num_led_pins][num_led_pins];

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
	int i = num_led_pins;
	while (i--) {
		pinMode(led_pins[i], OUTPUT);
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
			radio.read(button_states, num_button_pins);

			// Read the state of the buttons and adapt the led values
			readButtonStates();

			//Set the color of the strip
			setStripColor();

			//Set the color of the encoders
			setEncoderColor();
		}
	}
}


// Reads he led_values and sets the color of the encoders accordingly
void setEncoderColor(){

}

// Set the color of the LED strip according to the current led_values
void setStripColor(){
	int i = num_led_pins;
	while (i--) {
		analogWrite(led_pins[i], led_values[i]);
	}
}

// Read the received button state and set the strips color values accordingly
void readButtonStates(){

	// Increase R brightness
	if (button_states[0] && led_values[0] <= 250) {
		led_values[0] += 5;
		Serial.println("R+ pushed");
	}
	// Decrease R Brightness
	if (button_states[1] && led_values[0] >= 5) {
		led_values[0] -= 5;
		Serial.println("R- pushed");
	}
	// Increase G brightness
	if (button_states[2] && led_values[1] <= 250) {
		led_values[1] += 5;
		Serial.println("G+ pushed");
	}
	// Decrease G Brightness
	if (button_states[3] && led_values[1] >= 5) {
		led_values[1] -=5;
		Serial.println("G- pushed");
	}
	// Increase B brightness
	if (button_states[4] && led_values[2] <= 250) {
		led_values[2] += 5;
		Serial.println("B+ pushed");
	}
	// Decrease B Brightness
	if (button_states[5] && led_values[2] >= 5) {
		led_values[2] -= 5;
		Serial.println("B- pushed");
	}

	Serial.print("RGB ");
	Serial.print(led_values[0]);
	Serial.print(" ");
	Serial.print(led_values[1]);
	Serial.print(" ");
	Serial.println(led_values[2]);
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
