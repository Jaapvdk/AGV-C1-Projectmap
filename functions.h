#include <Arduino.h>
#include <VL6180X.h>
#include <Wire.h>

void vooruit (int tofverschil)
{   
    const int dirPin = 12;
    const int stepPin = 11;
    const int dirPin2 = 13;
    const int stepPin2 = 10;
    const int achteruit = 8;
    int sensor1_pin = A0;  
    int sensor2_pin = A1;                                     //SHDN Pins, worden gebruikt bij het afwisselen
    VL6180X sensor1;                                         //Define allbei de tof sensoren "sensor1" en "sensor2"
    VL6180X sensor2;

    pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(stepPin2, OUTPUT);
	pinMode(dirPin2, OUTPUT);
    pinMode(achteruit, INPUT_PULLUP);
    pinMode(sensor1_pin, OUTPUT);
    pinMode(sensor2_pin, OUTPUT);
    digitalWrite(sensor1_pin, LOW);                         //LOW == UIT  & HIGH == AAN
    digitalWrite(sensor2_pin, LOW); 

    if (digitalRead(achteruit) == HIGH)
    {
        Serial.println(tofverschil);
        digitalWrite(dirPin2, LOW);
        digitalWrite(dirPin,  HIGH);
        for (int x=0; x<60; x++)
        {
        digitalWrite(stepPin, HIGH);
	    digitalWrite(stepPin2, HIGH);
        delayMicroseconds(5000);
	    digitalWrite(stepPin, LOW);
        digitalWrite(stepPin2, LOW);
        delayMicroseconds(500);
        if (tofverschil < -2)
        {
            for (int x=0; x <= 5; x++)
            {
	        digitalWrite(stepPin, HIGH);
	        delayMicroseconds(5000);
            digitalWrite(stepPin, LOW);
            }
        }
        else if (tofverschil > 5)
        {
	        for (int x=0; x <= 2; x++)
            {
	        digitalWrite(stepPin, HIGH);
	        delayMicroseconds(5000);
            digitalWrite(stepPin, LOW);
            }
        }
        else if (tofverschil > 10)
        {
	        for (int x=0; x <= 10; x++)
            {
	        digitalWrite(stepPin, HIGH);
	        delayMicroseconds(5000);
            digitalWrite(stepPin, LOW);
            }
        }
        else if (tofverschil < -10)
        {
	        for (int x=0; x <= 10; x++)
            {
	        digitalWrite(stepPin, HIGH);
	        delayMicroseconds(5000);
            digitalWrite(stepPin, LOW);
            }
        }
		    delayMicroseconds(5000);
        }
  }      
  else if (digitalRead(achteruit) == LOW)
  {
    digitalWrite(dirPin2, HIGH);
    digitalWrite(dirPin,  LOW);
    for(int x = 0; x < 40; x++)     //Minimaal 30 rotaties, Max 50
	{
		digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
	}
  }
}

void draaien (int corner)
{ 
    Serial.println("TURNING");
    const int dirPin = 13;
    const int stepPin = 10;
    const int dirPin2 = 12;
    const int stepPin2 = 11;
    const int stepsPerturn = 420;
    // Declare pins as Outputs
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(stepPin2, OUTPUT);
	pinMode(dirPin2, OUTPUT);

    digitalWrite(dirPin2, HIGH);
    digitalWrite(dirPin,  LOW);
    for (int x = 0; x < 200; x++)
    {
        digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
    }                                            //Forward

    digitalWrite(dirPin, HIGH);                 // Set motor direction clockwise, rechts
	digitalWrite(dirPin2, HIGH);
    if (corner == 1)
    {
        digitalWrite(dirPin, LOW);                 
	    digitalWrite(dirPin2, LOW);
    }
	for(int x = 0; x < stepsPerturn; x++)           //TURN 1
	{
		digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
	}

    digitalWrite(dirPin2, HIGH);
    digitalWrite(dirPin,  LOW);
    for (int x = 0; x < 200; x++)
    {
        digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
    }
                    
                                                 //STRAIGHT AHEADTILL NEXT JUNCTION

    digitalWrite(dirPin, HIGH);                 
	digitalWrite(dirPin2, HIGH);
    if (corner == 1)
    {
        digitalWrite(dirPin, LOW);                  //TURN 2
	    digitalWrite(dirPin2, LOW);
    }
	for(int x = 0; x < stepsPerturn; x++)           
	{
		digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
	}                                               

    digitalWrite(dirPin2, HIGH);
    digitalWrite(dirPin,  LOW);
    for (int x = 0; x < 200; x++)
    {
        digitalWrite(stepPin, HIGH);        //STRAIGHT AHEAD
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
    }                                               


}

void achteruit ()
{
    const int dirPin = 13;
    const int stepPin = 10;
    const int dirPin2 = 12;
    const int stepPin2 = 11;
    // Declare pins as Outputs
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(stepPin2, OUTPUT);
	pinMode(dirPin2, OUTPUT);
    
    digitalWrite(dirPin2, LOW);
    digitalWrite(dirPin,  HIGH);

    for (int x = 0; x < 10; x++)
    {
        digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin2, HIGH);
		delayMicroseconds(5000);
		digitalWrite(stepPin, LOW);
   		digitalWrite(stepPin2, LOW);
		delayMicroseconds(5000);
    }
}