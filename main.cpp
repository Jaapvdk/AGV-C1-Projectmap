//17-06 AGV C1

#include <Arduino.h>
#include <VL6180X.h>
#include <Wire.h>

#define RESET 0
#define AUTOMATIC 1
#define MANUAL 2
#define CORNER 3
#define CORNERMANUAL 4
#define REVERSE 5

int state = 0;

const int dirPin = 13;
const int stepPin = 10;

const int dirPin2 = 12;
const int stepPin2 = 11;
const int stepsPerRevolution = 200;

const int manualswitch = 9;
const int reverse = 8;

int distance = 0;
int difference = 0;
int sensor1_pin = A0;  
int sensor2_pin = A1;                                     //SHDN Pins, worden gebruikt bij het afwisselen
VL6180X sensor1;                                         //Define allbei de tof sensoren "sensor1" en "sensor2"
VL6180X sensor2; 

const int echoPin1 = 4;
const int echoPin2 = 5;
const int trigPin1 = 6;
const int trigPin2 = 7;

int IR1 = 2;
int IR2 = 3;
int detectie = 0;

int waarschuwing = A3;
int detectiepin = A2;

int corner = 0;
int straight = 0;

int ToF1 = 0;
int ToF2 = 0;
int FlagDetect = 0;
int direction = 0;

void initialise (){
    
    // Declare pins as Outputs
    pinMode(trigPin1, OUTPUT); 
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);     
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(manualswitch, INPUT_PULLUP);
    pinMode(stepPin2, OUTPUT);
    pinMode(dirPin2, OUTPUT);
     
}

int US (){
    long duration;
    int distance;  
    int distance1; 
    int distance2; 

    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration = pulseIn(echoPin1, HIGH, 10000);
    distance1= duration*0.034/2;

    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration = pulseIn(echoPin2, HIGH, 10000);
    distance2= duration*0.034/2;

    if (distance1 < distance2){
        distance = distance1;
    }
    else
    {
        distance = distance2;
    }
    

    return (distance);
}

int ToF (){
    int difference = 0;
    
    int dist1 = sensor1.readRangeSingleMillimeters(); 
    delay(20);               
    int dist2 = sensor2.readRangeSingleMillimeters();

    difference = dist1-dist2;

    return (difference);
}

int IR (){
    int detectie = 0;

    if (!(digitalRead (IR1)) || !(digitalRead(IR2))){
      detectie = 1;
    }
    return (detectie);
}

void setup() {
  Serial.begin(9600);
  Serial.print("Initialising");
  delay (1000);       

  pinMode(sensor1_pin, OUTPUT);
  pinMode(sensor2_pin, OUTPUT);
  pinMode(waarschuwing, OUTPUT);
  pinMode(detectiepin, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  digitalWrite(waarschuwing, HIGH);
  digitalWrite(detectiepin, LOW);
  digitalWrite(sensor1_pin, LOW);                         //LOW == UIT  & HIGH == AAN
  digitalWrite(sensor2_pin, LOW);                         //Beide TOF sensoren moeten uit beginnen       

  Wire.begin();          

  digitalWrite(sensor1_pin, HIGH);                        //Sensor1 aan, calibreren, hoeft daarna niet meer uit 
  delay(50);
  sensor1.init();
  sensor1.configureDefault();
  sensor1.setTimeout(500);
  sensor1.setAddress(0x54);
  

  digitalWrite(sensor2_pin, HIGH);                        //Sensor1 aan, calibreren, hoeft daarna ook niet meer uit 
  delay(50);                                              //SetAdderss geeft elke sensor een uniek adress op de I2C bus,
  sensor2.init();                                         //de arduino kan nu beide sensoren tegelijk uitlezen.
  sensor2.configureDefault();
  sensor2.setTimeout(500);
  sensor2.setAddress(0x56);
  delay(500);    
  initialise();            
}

void loop() {

  switch (state){

      case RESET:
        Serial.println(state);
        digitalWrite(stepPin, LOW);
   		  digitalWrite(stepPin2, LOW);
        
        if (straight <2){
          if (digitalRead(manualswitch)){
            state = AUTOMATIC;
          }
          else{
            state = MANUAL;
          }
        }
        else{
          state = RESET;
        }
      break;

      case AUTOMATIC:
        Serial.println(state);

        difference = ToF();
        //Function call bewegen met evt verschil

        if (IR()){  //Boom gedecteerd
          digitalWrite(detectiepin, HIGH);
          delay (2000);
        }
        digitalWrite(detectiepin, LOW);

        distance = US(); //Persoon te dichtbij
        while (distance < 10){
          digitalWrite (waarschuwing, HIGH);
        }
        digitalWrite (waarschuwing, LOW);

        if (abs(difference)>180){ //Einde pad bereikt
          state = CORNER;
          straight++;
        }
      break;

      case CORNER:
        Serial.println(state);
        //functie call met int corner voor links of rechts
        state = RESET;
        corner++;
      break;

      case MANUAL:
        Serial.println(state);

        difference = ToF();
        //Function call bewegen met evt verschil

        distance = US(); //Persoon te dichtbij
        while (distance < 5){
          digitalWrite (waarschuwing, HIGH);
        }
        digitalWrite (waarschuwing, LOW);

        if (abs(difference)>180){ //Einde pad bereikt
          state = CORNERMANUAL;
        }
        else if (reverse){
          state = REVERSE;
        }
      break;

      case CORNERMANUAL:
        Serial.println(state);
        FlagDetect = 0;
        ToF1 = sensor1.readRangeSingleMillimeters();
        ToF2 = sensor2.readRangeSingleMillimeters();
        if (ToF1 < 25){
          direction = 1;
          FlagDetect = 1;
        }
        else if (ToF2 < 25){
          direction = 0;
          FlagDetect = 1;
        }

        if (FlagDetect){
          //Function call voor bocht met direction
          state = MANUAL;
        }
      break;

      case REVERSE:
        Serial.println(state);
        //rij achteruit langzaam

        distance = US(); //Persoon te dichtbij
        while (distance < 5){
          digitalWrite (waarschuwing, HIGH);
        }
        digitalWrite (waarschuwing, LOW);

        if (!reverse){
          state = MANUAL;
        }
      break;

  }
}