
/*
//Rotary Encoder:
int clkPin_r = [pin];
int dtPin_r = [pin];

int clkPin_g = [pin];
int dtPin_g = [pin];

int clkPin_b = [pin];
int dtPin_b = [pin];

void setup()
{
//Rotary Encoder: Red
pinMode(clkPin_r, INPUT_PULLUP);
pinMode(dtPin_r, INPUT_PULLUP);

//Rotary Encoder: Green
pinMode(clkPin_g, INPUT_PULLUP);
pinMode(dtPin_g, INPUT_PULLUP);

//Rotary Encoder: Blue
pinMode(clkPin_b, INPUT_PULLUP);
pinMode(dtPin_b, INPUT_PULLUP);
}

void loop()
{
}

// returns 0, unless Encoder is turned; returns 1 for right turn, -1 for left turn
int getEncoderTurn(clkPin, dtPin)
{
static int oldA = HIGH;
static int oldB = HIGH;
int result = 0;
int newA = digitalRead(clkPin);
int newB = digitalRead(dtPin);
if (newA != oldA || newB != oldB)
{
// something has changed
if (oldA == HIGH && newA == LOW)
{
result = (oldB * 2 - 1);
}
}
oldA = newA;
oldB = newB;
return result;
}

 */

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
const uint8_t button_pins[] = { 2, 3, 4};
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 5, 9, 10 };
const uint8_t num_led_pins = sizeof(led_pins);

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

//
// Payload
//

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];

uint8_t led_abs_vals[] = {200,200,200};
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
	printf("ROLE REMOTE");

	//
	// Setup and configure rf radio
	//

	radio.begin();

	//
	// Open pipes to other nodes for communication
	//

	// This simple sketch opens a single pipes for these two nodes to communicate
	// back and forth.  One listens on it, the other talks to it.

	radio.openWritingPipe(pipe);

	//
	// Start listening
	//

	//
	// Dump the configuration of the rf unit for debugging
//c//

	radio.printDetails();

	//
	// Set up buttons / LED's
	//

	// Set pull-up resistors for all buttons
	int i = num_button_pins;
	while (i--) {
		pinMode(button_pins[i], INPUT);
		digitalWrite(button_pins[i], LOW);
	}
}

//
// Loop
//

void loop(void)
{
	//
	// Remote role.  If the state of any button has changed, send the whole state of
	// all buttons.
	//

	// Get the current state of buttons, and
	// Test if the current state is different from the last state we sent
	int i = num_button_pins;
	bool different = false;
	while (i--) {
		uint8_t state = !digitalRead(button_pins[i]);
		if (state != button_states[i]) {
			different = true;
			button_states[i] = state;
		}
	}

	// Send the state of the buttons to the LED board
	if (different) {
		printf("Now sending...");
		bool ok = radio.write(led_abs_vals, num_button_pins);
		//bool ok = radio.write(button_states, num_button_pins);
		if (ok)
			printf("ok\n\r");
		else
			printf("failed\n\r");
	}

	// Try again in a short while
	delay(20);
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
