// Basic Bluetooth test sketch 5b for the Arduino Mega. 
// simple communication with an Android device
//
// Uses serial with the host computer and Serial3 for communication with the Bluetooth module
//
//  Pins
//  BT VCC to Arduino 5V out. Disconnect before running the sketch
//  BT GND to Arduino GND
//  BT RX (through a voltage divider) to Arduino TX1 (pin 18)
//  BT TX  to Arduino RX1 (no need voltage divider)   (pin 19)
//
// When text is entered in to the serial monitor on the computer the
// Arduino will relay it to the Bluetooth module.
// When text is entered in to a Bluetooth terminal on the Android device 
// it will be sent to the Arduino and be displayed in the serial monitor
//
#include <Car.h> 
int spd=0;
int turnspd=0;
int count=0;


Car* car;

 
void setup() 
{
  car=new Car("car1");
   car->moveForward(0);
    // communication with the host computer
    Serial.begin(9600);  
 
    Serial.println("Do not power the BT module");
 
    // communication with the BT module on Serial3
    Serial3.begin(38400);
 
 
 
    Serial.println(" ");
    Serial.println("HC-05 started");
//    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
//    Serial.println("Do not enter AT mode");
//    Serial.println(""); 
   
}
 
 
void loop() 
{
    char command='x';
    // check commands
    if ( Serial3.available() )   
    {        
      char check=Serial3.read();
//      Serial.println(check);
      if (check=='S'){
        spd=0;
        for(int i=0;i<4;i++){
          while(!Serial3.available());
          int c=Serial3.read()-'0';
          spd=spd*10+(c);          
        }
        spd-=128;
        command=Serial3.read();
      }else if(check=='D'){
        turnspd=0;
        for(int i=0;i<4;i++){
          while(!Serial3.available());
          int c=Serial3.read()-'0';
          turnspd=turnspd*10+(c);  
        }
        turnspd-=128;
      }
    }
      car->moveWithTurn(spd,turnspd);
}
 
