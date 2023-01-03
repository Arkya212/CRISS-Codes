#define sensitivity 33
#define loops 2
#define B 512
#define sensornum 3
 //quiescent 0.5 --- MULTIPLY 5*0.25
 //voltage reference 5V
float value = 0;
const float cutoffCurrent1 = 1;
const float cutoffCurrent2 = 1;
const float cutoffCurrent3 = 1;

//long int v1;
long int v0[3];
long int buffer[B] = {0};

void start(int cutoffCurrent, int vin, long int buffer[], int relaypin);
void sensor(int sensorpin, int relaypin);
void sensorsetup(int analoginput, int vin);

void setup() {
  pinMode(6,OUTPUT); //Connected to S pin
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
Serial.begin(9600);
//Setting up the Setup
int analoginput = A0;
for(int k=0; k<sensornum; k++){
  sensorsetup(analoginput, k);

  switch (k){
case 1:
analoginput = A1;
break;
case 2:
analoginput = A2;
// Add this the last sensor for the loop to go on for infinite time
break;
  }

}
}


void loop(){
//  buffer[B]={0};
//Checking for initial value
int sensorpin = A0;
int relaypin = 6;
float cutoffCurrent = cutoffCurrent1;
for(int y=2; y<sensornum; y++){
//buffer[B]={0};
//for(int i = 0 ; i < B ; ++i )
//  {
//    buffer[i] = analogRead(A0);
//    delayMicroseconds(1000);
//  }
//  for( int i = 0 ; i < B ; ++ i )
//  {
//  v1 += buffer[i];
//  }
//  v1 = v1/B;//Setting the initial Condition
//   delay(990);

//Keep Adding info about Pins as you increase the number of input lines
switch (y){
case 1:
sensorpin = A1;
relaypin = 7;
cutoffCurrent = cutoffCurrent2;
break;
case 2:
sensorpin = A2;
relaypin = 8;
cutoffCurrent = cutoffCurrent3;
// Add this the last sensor for the loop to go on for infinite time
break;
//default:
//Serial.println("Input Unavailable");
//break;
}
 sensor(sensorpin, relaypin, cutoffCurrent, y);
}
Serial.println(" ");
delay(990);
}

void sensor(int sensorpin, int relaypin, float cutoffCurrent, int vin) {
  // put your main code here, to run repeatedly:
  for( int i = 0 ; i < B ; ++i )
  {
    buffer[i] = analogRead(sensorpin);
    delayMicroseconds(1000);
  }
 
  start(cutoffCurrent, vin, buffer, relaypin);
//  Serial.println(" ");
}

void start(int cutoffCurrent, int vin, long int buffer[], int relaypin){

  long int voltage = 0;
  for( int i = 0 ; i < B ; ++ i ) voltage = voltage + buffer[i]; //Taking Root Mean Square voltage
  voltage = voltage/B;
  value = (4.89/sensitivity)*(voltage-v0[vin])/(loops);
          if(abs(value)<cutoffCurrent){
        digitalWrite(relaypin, HIGH); //keep Load on
      }

      else if(abs(value)>cutoffCurrent){
        digitalWrite(relaypin, LOW);
      }
        Serial.print(value);
  Serial.print("\t");
}

void sensorsetup(int analoginput, int vin){
    for( int i = 0 ; i < B ; ++i )
  {
    buffer[i] = analogRead(analoginput);
    delayMicroseconds(1000);
  }
  for( int i = 0 ; i < B ; ++ i ) v0[vin] += buffer[i];
  v0[vin] = v0[vin]/B;//Setting the initial Condition
  }
