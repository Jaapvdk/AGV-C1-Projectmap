#include <Arduino.h>
#include <VL6180X.h>
#include <Wire.h>
#include "Functions.h"

#define RESET 0
#define AUTOMATIC 1
#define MANUAL 2
#define FORWARD 3
#define CORNER 4
#define REVERSE 5

int state = 0;

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

void initialise (){
    
    pinMode(trigPin1, OUTPUT); 
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);                  
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
      break;

      case AUTOMATIC:
      break;

      case MANUAL:
      break;

      case FORWARD:
      break;

      case CORNER:
      break;

      case REVERSE:
      break;

  }


  digitalWrite(waarschuwing, HIGH);
  difference = ToF();
  Serial.print("Verschil ToF: ");
  Serial.print(difference);
  Serial.print("\n");

  distance = US();
  Serial.print("Kortste Ultrasoon: ");
  Serial.print(distance);
  Serial.print("\n");

  detectie = IR();
  if (detectie){
    digitalWrite(detectiepin, HIGH);
  }
  Serial.print("IR: ");
  Serial.print(detectie);
  Serial.print("\n");

  delay (500);
  digitalWrite(detectiepin, LOW);
  digitalWrite(waarschuwing, LOW);
}