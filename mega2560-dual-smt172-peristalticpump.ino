// Peristalticpump control,2 smt172 Temperature Sensors and a MPXV7002DP Differential Pressure Sensor (5V)
//by nrbrt
//GPLv3 applies

#include <arduino.h>
#include <AccelStepper.h>
#include <SMT172_T4.h>
#include <SMT172_T5.h>

//Define stepper motor connections
#define dirPin 12
#define stepPin 11

// prepare microstepping modes
#define m0Pin 28
#define m1Pin 27
#define m2Pin 25

int fullStep[4] = {0,0,0};
int halfStep[4] = {1,0,0};
int quarterStep[4] = {0,1,0};
int eighthStep[4] = {1,1,0};
int sixteenthStep[4] = {0,0,1};
int thirtysecondthStep[4] = {1,1,1};

// enable/disable motor
#define enablePin 31

// select the input pin for the Pressure Sensor
int sensorPin = A0;    

//Create stepper object
AccelStepper stepper(1,stepPin,dirPin); //motor interface type must be set to 1 when using a driver.

long last_update = 0;
long time_now;
float temp1 = 0;
float temp2 = 0;
String str; //string received by serial port
int count; //amount of elements after splitting the received string
String sParams[10];
bool rotating = false; 
bool positioning = false;
float motorspeed = 300;
float motoraccel = 3000;
float voltage=0; //to store the converted analog reading
float pressure_kPa=0; //to store the calculated differential pressure


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  SMT172_T4::startTemperature(0.001);
  SMT172_T5::startTemperature(0.001);
  
  //default enable motor and set full step mode
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin,LOW);
  pinMode(m0Pin, OUTPUT);
  digitalWrite(m0Pin, LOW);
  pinMode(m1Pin, OUTPUT);
  digitalWrite(m1Pin, LOW);
  pinMode(m2Pin, OUTPUT);
  digitalWrite(m2Pin, LOW);
  
  pinMode(sensorPin, INPUT);  // Pressure sensor is on Analogue pin 0
  delay(30);
}


void loop() {
  time_now = millis();
  
  if(rotating){stepper.runSpeed();}
  if(positioning){stepper.runSpeedToPosition();}
  
  if((time_now - last_update) >= 1000){
        getTemp();
        voltage = analogRead(sensorPin)*(5.0 / 1023.0);
        pressure_kPa = voltage - 2.5;
        Serial.print("t1:");
        Serial.print(temp1);
        Serial.print(",t2:");
        Serial.print(temp2);
        Serial.print(",p:");
        Serial.println(pressure_kPa);
        temp1 = 0;
        temp2 = 0;
        if(rotating){stepper.runSpeed();}
        if(positioning){stepper.runSpeedToPosition();}
        last_update = millis();        
  }


  if(Serial.available()){
     str = Serial.readStringUntil('\n');
     count = StringSplit(str,':',sParams,6);

     //rotate at a defined speed with a defined acceleration.
     if(sParams[0] == "rot" && count == 2){
          rotating = true;
          positioning = false;
          stepper.setMaxSpeed(sParams[2].toFloat());
          stepper.setAcceleration(sParams[1].toFloat());
          stepper.setSpeed(sParams[2].toFloat());
     }

     // go to specific position and halt there
     if(sParams[0] == "pos" && count == 2){
        positioning = true;
        rotating = false;
        stepper.setMaxSpeed(sParams[1].toFloat());       
        long pos = long(sParams[2].toInt());
        stepper.move(pos);
        stepper.setSpeed(sParams[1].toFloat());      
     }

     // define speed settings
     if(sParams[0] == "spd" && count == 2){
      motorspeed = sParams[1].toFloat();
      stepper.setMaxSpeed(motorspeed);
      stepper.setSpeed(motorspeed);
     }
     
     // define acceleration settings
     if(sParams[0] == "acc" && count == 2){
      motoraccel = sParams[1].toFloat();
      if(motoraccel == 0){
        motoraccel = 1;
      }
      stepper.setAcceleration(motoraccel);
     }

     // enable or disable the motor
     if(sParams[0] == "motor" && count == 2){
      if(sParams[1] == "enable"){
        digitalWrite(enablePin, LOW);
      }else{
        digitalWrite(enablePin, HIGH);
      }
     }

     // selection of microstepping mode
     if(sParams[0] == "mode" && count == 2){
      if(sParams[1] == "1"){
        digitalWrite(m0Pin,fullStep[0]);
        digitalWrite(m1Pin,fullStep[1]);
        digitalWrite(m2Pin,fullStep[2]);
      }
      if(sParams[1] == "2"){
        digitalWrite(m0Pin,halfStep[0]);
        digitalWrite(m1Pin,halfStep[1]);
        digitalWrite(m2Pin,halfStep[2]);
      }
      if(sParams[1] == "4"){
        digitalWrite(m0Pin,quarterStep[0]);
        digitalWrite(m1Pin,quarterStep[1]);
        digitalWrite(m2Pin,quarterStep[2]);
      }
      if(sParams[1] == "8"){
        digitalWrite(m0Pin,eighthStep[0]);
        digitalWrite(m1Pin,eighthStep[1]);
        digitalWrite(m2Pin,eighthStep[2]);
      }
      if(sParams[1] == "16"){
        digitalWrite(m0Pin,sixteenthStep[0]);
        digitalWrite(m1Pin,sixteenthStep[1]);
        digitalWrite(m2Pin,sixteenthStep[2]);
      }
      if(sParams[1] == "32"){
        digitalWrite(m0Pin,thirtysecondthStep[0]);
        digitalWrite(m1Pin,thirtysecondthStep[1]);
        digitalWrite(m2Pin,thirtysecondthStep[2]);
      }
     }
  }
  if(rotating){stepper.runSpeed();}
  if(positioning){stepper.runSpeedToPosition();}
}


void getTemp(){
  if(rotating){stepper.runSpeed();}
  if(positioning){stepper.runSpeedToPosition();}
  
  SMT172_T4::startTemperature(0.001);
  repeat_T4:
    switch (SMT172_T4::getStatus()) {
    case 0: goto repeat_T4; // O Dijkstra, be merciful onto me, for I have sinned against you :)
    case 1: 
      temp1 = SMT172_T4::getTemperature();
      if(rotating){stepper.runSpeed();}
      if(positioning){stepper.runSpeedToPosition();}
      break;
    case 2:
      temp1 = -1;
      if(rotating){stepper.runSpeed();}
      if(positioning){stepper.runSpeedToPosition();}
  }
  
  SMT172_T5::startTemperature(0.001);
  repeat_T5:
    switch (SMT172_T5::getStatus()) {
    case 0: goto repeat_T5; // O Dijkstra, be merciful onto me, for I have sinned against you :)
    case 1:
      temp2 = SMT172_T5::getTemperature();
      if(rotating){stepper.runSpeed();}
      if(positioning){stepper.runSpeedToPosition();}
      break;
    case 2:
      temp2 = -1;
      if(rotating){stepper.runSpeed();}
      if(positioning){stepper.runSpeedToPosition();}
  }
}

int StringSplit(String sInput, char cDelim, String sParams[], int iMaxParams)
{
    int iParamCount = 0;
    int iPosDelim, iPosStart = 0;

    do {
        // Searching the delimiter using indexOf()
        iPosDelim = sInput.indexOf(cDelim,iPosStart);
        if (iPosDelim > (iPosStart+1)) {
            // Adding a new parameter using substring() 
            sParams[iParamCount] = sInput.substring(iPosStart,iPosDelim);
            iParamCount++;
            // Checking the number of parameters
            if (iParamCount >= iMaxParams) {
                return (iParamCount);
            }
            iPosStart = iPosDelim + 1;
        }
    } while (iPosDelim >= 0);
    if (iParamCount < iMaxParams) {
        // Adding the last parameter as the end of the line
        sParams[iParamCount] = sInput.substring(iPosStart);
        iParamCount++;
    }

    return (iParamCount);
}
