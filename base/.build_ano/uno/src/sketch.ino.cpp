#include <Arduino.h>

void setup();
void loop();
void setValues(int values[]);
bool getValues(int values[]);
#line 1 "src/sketch.ino"

//Base unit connected to the LED-strip
//This arduino receives signals send by the remote
//and sets the control outputs accordingly

const int rControl = 2;
const int gControl = 4;
const int bControl = 8;

int values[3] = {2,2,2};
void setup()
{
				Serial.begin(9600);

				pinMode(rControl, OUTPUT);
				pinMode(gControl, OUTPUT);
				pinMode(bControl, OUTPUT);
}



void loop()
{
				if ( getValues(values)) {
								setValues(values);
				}
}



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

				int old_values = {};
				old_values = values;

				//TODO get values
				values[0] = 0;
				values[1] = 0;
				values[2] = 0;

				if ( old_values[0] == values[0] && old_values[1] == values[1] && old_values[2] == values[2]) {
								return false;
				}

				return true;
}
