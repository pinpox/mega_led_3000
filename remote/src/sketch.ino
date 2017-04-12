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
