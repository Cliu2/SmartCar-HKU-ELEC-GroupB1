/*****************************************************
IDP Lab 3 Feedback and control of servo motor
Usage of servo motor control library and ADC for tracking the light source 

*****************************************************/
#include <Servo.h> 
#include "Car.h"

#define left_PIN 9        //servo connected at pin 9
#define right_PIN 10        //servo connected at pin 9
#define TOL   10           //tolerance for adc different, avoid oscillation
#define K   1              //Step size
#define thrd 100
Car* car;

Servo left,right;            //decare servo object

int int_position=90;      //Servo initial position

//variables for light intensity to ADC reading equations 
int int_adc0, int_adc0_m, int_adc0_c;
int int_adc1, int_adc1_m, int_adc1_c;     
int int_left, int_right;
int obstacleDistance;
void setup() 
{
  car=new Car("car1");
  car->moveForward(0);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
//  left.attach(left_PIN);
//  left.write(int_position);
//  right.attach(right_PIN);
//  right.write(int_position);
  Serial.println();

// measure the sensors reading at ambient light intensity  
  int_adc0=analogRead(A9);   // Left sensor at ambient light intensity
  int_adc1=analogRead(A8);   // Right sensor at ambient light intensity
  
  Serial.print("Calibration in progress, cover the sensors with your hand ......");
  delay(10000);        // delay 10000 ms

// measure the sensors reading at zero light intensity  
  int_adc0_c=analogRead(A9);   // Left sensor at zero light intensity
  int_adc1_c=analogRead(A8);   // Right sensor at zero light intensity

// calculate the slope of light intensity to ADC reading equations  
  int_adc0_m=(int_adc0-int_adc0_c)/100;
  int_adc1_m=(int_adc1-int_adc1_c)/100; 
    
  Serial.println("...... Completed! Remove your hand");
  delay(5000);        // delay 5000 ms

  bool light = false;
  while(!light){
    int_left=(analogRead(A9)-int_adc0_c)/int_adc0_m;
    int_right=(analogRead(A8)-int_adc1_c)/int_adc1_m;
    if (int_left > thrd || int_right > thrd)
      light = true;
  }
  car->moveForward(80);
}

void loop() 
{
  int_left=(analogRead(A9)-int_adc0_c)/int_adc0_m;
  int_right=(analogRead(A8)-int_adc1_c)/int_adc1_m;
  if (abs(int_left-int_right) > TOL)
    tracker();

  Serial.print("Left sensor intensity=");
  Serial.print(int_left);
  Serial.print(";  Right sensor intensity=");
  Serial.print(int_right);
  Serial.print(";  Servo position=");
  Serial.println(int_position);
  
  obstacleDistance=car->getUltraSonicReading();
  if (obstacleDistance > 20 && obstacleDistance < 30)
    car->moveForward(80);
  else if (obstacleDistance > 30 && obstacleDistance < 50)
    car->moveForward(100);
  else if (obstacleDistance > 10 && obstacleDistance < 20)
    car->moveForward(50);
  else if(obstacleDistance < 10)
    car->moveForward(20);
  else
    car->moveForward(80);

  delay(50);        // delay 100 ms
  
}

void tracker()
{
 // If left sensor is brighter than right sensor, decrease servo angle and turn LEFT
 int angle=0;
  if (int_left>(int_right+TOL))
  {
    if (int_position>0)
    {
//      int_position = int_position-K;
angle=int_left-int_right;
      car->turn(-angle);    
    }    
  }

 // if right sensor is bright than left sensor, increase servo angle and turn RIGHT
  if (int_right>(int_left+TOL))
  {
    if (int_position>0)
    {
//      int_position = int_position+K;
angle=int_right-int_left;

      car->turn(angle); 
    }    
  }
}
