
#include "SPI.h"
#include "RF24.h"


const int rControl = 2;
const int gControl = 4;
const int bControl = 8;

int values[3] = {2,2,2};
byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(6,7);
/**********************************************************/

// Used to control whether this node is sending or receiving
bool role = 0;

/**
 * Create a data structure for transmitting and receiving data
 * This allows many variables to be easily sent and received in a single transmission
 * See http://www.cplusplus.com/doc/tutorial/structures/
 */
struct dataStruct{
				unsigned long _micros;
				float value;
}myData;

void setup() {

				pinMode(rControl, OUTPUT);
				pinMode(gControl, OUTPUT);
				pinMode(bControl, OUTPUT);

				Serial.begin(115200);
				Serial.println(F("BASE RF24/examples/GettingStarted_HandlingData"));
				Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

				radio.begin();

				// Set the PA Level low to prevent power supply related issues since this is a
				// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
				radio.setPALevel(RF24_PA_LOW);

				// Open a writing and reading pipe on each radio, with opposite addresses
				if(radioNumber){
								radio.openWritingPipe(addresses[1]);
								radio.openReadingPipe(1,addresses[0]);
				}else{
								radio.openWritingPipe(addresses[0]);
								radio.openReadingPipe(1,addresses[1]);
				}

				myData.value = 1.22;
				// Start the radio listening for data
				radio.startListening();
}




void loop() {


				/****************** Ping Out Role ***************************/
				if (role == 1)  {

								radio.stopListening();                                    // First, stop listening so we can talk.


								Serial.println(F("Now sending"));

								myData._micros = micros();
								if (!radio.write( &myData, sizeof(myData) )){
												Serial.println(F("failed"));
								}

								radio.startListening();                                    // Now, continue listening

								unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
								boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

								while ( ! radio.available() ){                             // While nothing is received
												if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
																timeout = true;
																break;
												}
								}

								if ( timeout ){                                             // Describe the results
												Serial.println(F("Failed, response timed out."));
								}else{
												// Grab the response, compare, and send to debugging spew
												radio.read( &myData, sizeof(myData) );
												unsigned long time = micros();

												// Spew it
												Serial.print(F("Sent "));
												Serial.print(time);
												Serial.print(F(", Got response "));
												Serial.print(myData._micros);
												Serial.print(F(", Round-trip delay "));
												Serial.print(time-myData._micros);
												Serial.print(F(" microseconds Value "));
												Serial.println(myData.value);
								}

								// Try again 1s later
								delay(1000);
								if ( getValues(values)) {
												setValues(values);
								}
				}



				/****************** Pong Back Role ***************************/

				if ( role == 0 )
				{

								if( radio.available()){
												// Variable for the received timestamp
												while (radio.available()) {                          // While there is data ready
																radio.read( &myData, sizeof(myData) );             // Get the payload
												}

												radio.stopListening();                               // First, stop listening so we can talk
												myData.value += 0.01;                                // Increment the float value
												radio.write( &myData, sizeof(myData) );              // Send the final one back.
												radio.startListening();                              // Now, resume listening so we catch the next packets.
												Serial.print(F("Sent response "));
												Serial.print(myData._micros);
												Serial.print(F(" : "));
												Serial.println(myData.value);
								}
				}




				/****************** Change Roles via Serial Commands ***************************/

				if ( Serial.available() )
				{
								char c = toupper(Serial.read());
								if ( c == 'T' && role == 0 ){
												Serial.print(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
												role = 1;                  // Become the primary transmitter (ping out)

								}else
												if ( c == 'R' && role == 1 ){
																Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
																role = 0;                // Become the primary receiver (pong back)
																radio.startListening();

												}
				}


} // Loop





// Set to output pins
void setValues(int values[]) {
				analogWrite(rControl, values[0]);
				analogWrite(gControl, values[1]);
				analogWrite(bControl, values[2]);
}


//Get Values from receiver. If the control pins have to be set
//to new values (values changed) true is returned.
//False otherwise to  skip the refereshing.

bool getValues(int values[]) {

				int old_values[3] = {values[0], values[1], values[2]};

				//TODO get values

				values[0] = 0;
				values[1] = 0;
				values[2] = 0;

				if ( old_values[0] == values[0] && old_values[1] == values[1] && old_values[2] == values[2]) {
								return false;
				}

				return true;
}
