#include <Arduino.h>

void setup();
void loop();
bool getValues(int values[]);
#line 1 "src/sketch.ino"

/*Base unit connected to the LED-strip
	This arduino receives signals send by the remote and sets the control outputs accordingly*/

const int rControl = 2;
const int gControl = 4;
const int bControl = 8;

void setup()
{
				Serial.begin(9600);
}



void loop()
{
				int values[3] = {2,2,2};
				Serial.println(values[0]);
				getValues(values);
				Serial.println(values[0]);
}


bool getValues(int values[]) {

				values[0] = 0;
				values[1] = 0;
				values[2] = 0;
}





