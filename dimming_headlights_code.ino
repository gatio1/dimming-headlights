/*============================================================================
 | 
 | Project:     Self-dimming headlights
 | 
 +----------------------------------------------------------------------------
 | 
 | Description: When there is a vehicle ahead, the headlights point 
 |		down and become less bright. It also helps to save energy during the day. 
 |		There is an indicator for the brightness that can help the driver 
 |		make a rough estimate of how far away the upcoming vehicle is. 
 | 
 | Input:       A single photo resistor that detects thebrightness ahead.
 | Output:      -A lamp that changes its brightness.
 |				-A servo that tilts
 |				-A display that changes its reading from 0 to 8.
 |				-Serial output that shows the binary number given to the decoder,
 |				the reading of the photoresistor and the number given to the decoder
 |				in decimal.
 | 
 | The Circuit: 
 |				(1) An NPN transistor amplifies the signal from the Arduino(pin 3) 
 |				connected in scheme common emitter. A lamp powered by a 9V battery.
 |				
 |				(2) A photoresistor that is connected to 5V and analog pin A5.
 |				Analog pin A5 reads the input from the photoresistor.
 |
 |				(3)A servo connected to 5V, GND and pin5 of the arduino. pin5
 |				is used to control the servo.
 |				
 |				(4) A 7 segment decoder CD4511 connected to 5V and GND of the 
 |				Arduino. The input pins used to control it are(4,9,10,11). 
 |				The input is a 4 digit binary number. The decoder controls a 
 |				7 segment common cathode display. The display is connected to 
 |				5V and GND. Its Input consists of the according output of the decoder.
 |				
 |				(5) An arduino UNO on which the code is loaded.
 | 
 | Algorithm:   The code is written in the language used for Arduino(C++).
 |				The level of light is detected by the photoresistor and stored 
 |				in a variable. Then if the level is below certan number nothing 
 |				about the lamp and the servo changes. If the level is higher, 
 |				The signal at the transistor's base is lowered and the servo 
 |				is tilted. For the display, the reading of the photoresistor
 |				is devided by a number so that at maximum the new number is 8.
 |				That then is turned into a binary number and passed to the decoder
 |				as each digit is the value of signal of a different input PIN of the
 |				decoder. 
 | 
 | Known Bugs:  The input from the photoresistor is not linear and some numbers 
 |				on the display apear for a very short range of light values.
 | 
 +----------------------------------------------------------------------------
 | 
 | Author:      Vasil Gatev
 | Group:       47
 | Faculty #:   121220117
 | 
 | Discipline:  TPLab2021 (Technological Practicum - Laboratory Exercises)
 | Institution: Technical University of Sofia
 | Instructor:  A.Redzheb (adnan@tu-sofia.bg)
 | Platform:    TinkerCAD Circuits
 | Hyperlink:   https://www.tinkercad.com/things/1eLaKnfMH2L-dimming-headlights/editel?sharecode=7URF3ReT3z1NsUEhHAVr5RhxKH9fDii_pZSmipYtrhk
 | Due Date:    04 June 2021 
 |
 *==========================================================================*/
#include<Servo.h>
int servopin=5;
int basepin=3;
int pos=0;
int dmm;
int a=0;
Servo servo;

void setup()
{
  pinMode(basepin,OUTPUT);
  pinMode(A5,INPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  servo.attach(servopin,500,2500);
  Serial.begin(9600);
}
void loop()
{
  int reading=analogRead(A5);
  int result;
  if(reading>=711)
  {
  	result=(reading-711)/((965-711)/8);
    Serial.print("result:");
    Serial.println(result);
  }
  else {result =0;}
  bool resbin[4];
  for(int i=3; i>=0; i--)
  {
    resbin[i]=0;
  }
  for(int i=0;i<4;i++)//turn result into binary number in resbin[] 
  {
    resbin[i]=result%2;
    result=result/2;
  }
  Serial.print("resbin:");
  for(int i=0; i<4; i++)
  {
    Serial.print(resbin[i]);
  }
  Serial.println();
  if(resbin[0]==1)//0 razrqd
  {
   digitalWrite(6,HIGH);
  }
  else digitalWrite(6,LOW);
  if(resbin[1]==1)//1 razrqd
  {
    digitalWrite(9,HIGH);
  }
  else digitalWrite(9,LOW);
  if(resbin[02]==1)//2 razrqd
  {
    digitalWrite(10,HIGH);
  }
  else digitalWrite(10,LOW);
  if(resbin[03]==1)//3 razrqd
  {
    digitalWrite(11,HIGH);
  }
  else digitalWrite(11,LOW);
  //Serial.println(reading);
  if(reading>940)//намаляне на яркостта и насочване надолу чрез серво
  {
    tilt(30);
    percentdim(50);
  }
  else
  {
    tilt(0);
    percentdim(0);
  }
}
void percentdim(int percent)//
{
  dmm=100-percent;
  dmm = map(dmm,0,100,0,255);
  analogWrite(basepin, dmm);
}
void tilt(int deg)
{
  servo.write(90-deg);
    delay(1000);
}
