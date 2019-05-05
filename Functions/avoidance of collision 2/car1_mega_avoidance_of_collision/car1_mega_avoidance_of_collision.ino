#include "Car.h"

String serial_str;

struct Coor
{
  int x;
  int y;
};


/*
 * Mega Connects Ultrasonic sensor
 * Use the ESP8266 adapter to publish MQTT topics
 */

Car* car;
int x, y;
Coor car1;

// MQTT message struture
struct MqttMsg {
  String topic;
  String payload;
};

// method declaration
// methods are defined in the end of the file
void wait_esp8266_connected();

MqttMsg parse_mqtt_msg(String str);
 
////// MQTT topic information ////////////////
// MAKE SURE YOU USE THE RIGHT GROUP NAME AND CAR ID
String my_topic = "car1_coordinate";
///////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println(F("Waiting for ESP8266..."));
  wait_esp8266_connected();
  Serial.println(F("ESP8266 connected"));
  stay();
  car = new Car(car1)
  car1.x = 0; 
  pinMode (7, INPUT); 
}

void loop() {
  
  car->goOneBlock();
  car1.x++;
  Serial.print("car_1_coordinate");
  Serial.println(car1.x);

  /* Publish distance got from ultrasonic sensor with ESP8266 */
  Serial1.print("PUB," + my_topic + ",");
  Serial1.print(car1.x);
  Serial1.print("\n");

  /* Get MQTT messages from subscribed topic */
  while (Serial1.available()) {
    
    serial_str = Serial1.readStringUntil('\n');
    //Serial.println(serial_str);

    MqttMsg mqtt_msg = parse_mqtt_msg(serial_str);
    Serial.print(F("Topic: "));
    Serial.print(mqtt_msg.topic);
    Serial.print(F("\tPayload: "));
    Serial.println(mqtt_msg.payload);
  }
    Sameline = digitalRead(7);
    if (serial_str.toInt()  - car1.x <= 1 && Sameline ==LOW)
    {
      stay();
      while(true)
      {}
    }

}

void wait_esp8266_connected(){
  String serial_str("");
  do {
    while (Serial1.available()) {
      serial_str = Serial1.readStringUntil('\n');
      Serial.println(serial_str);
      Serial.println(serial_str != "CONNECTED\r");
    }
    if (serial_str != "CONNECTED\r")
      break;
  }while (serial_str != "CONNECTED\r");
}

// This method constructs the MQTT message from the serial
// input (a string) received from ESP8266
MqttMsg parse_mqtt_msg(String str){
  
  MqttMsg mqtt_msg;

  // create char* that keeps str for subsequent operations
  char* c_str = new char[str.length() + 1];
  strcpy(c_str, str.c_str());

  // The C library function char *strtok(char *str, const char *delim) 
  // breaks string str into a series of tokens using the delimiter delim.
  char* sub_str = strtok(c_str, ",");
  
  if (strcmp(sub_str, "MSG") != 0) {
    return mqtt_msg; // str does not contain MQTT message
  }
  
  sub_str = strtok(NULL, ",");
  if (sub_str == NULL) {
    return mqtt_msg; // str not complete
  }
  mqtt_msg.topic = String(sub_str);
  
  sub_str = strtok(NULL, ",");
  if (sub_str == NULL) {
    return mqtt_msg; // str not complete
  }
  mqtt_msg.payload = String(sub_str);

  return mqtt_msg;
}
