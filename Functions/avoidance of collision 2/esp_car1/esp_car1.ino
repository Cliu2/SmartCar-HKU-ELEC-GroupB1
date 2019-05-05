/*
 * Use ESP8266 as MQTT adapter
 *
 * Serial COMMAND:
 * PUB,(topic name),(payload)\n  : publish as the MQTT topic
 * 
 * Serial RESPONSE:
 * CONNECTED                      : WiFi connected
 * MSG,(topic name),(payload)\r\n : response of subscribed topics
 * 
 * 
 * Subscribed topics are put under reconnect()
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void mqtt_callback(char* topic, byte* payload, unsigned int len);
bool setup_wifi();
void reconnect();

////// wifi network information ///////////////
String ssid = "IDP-wifi";
String password = "Admin1234";
///////////////////////////////////////////////

/////// MQTT topic information ////////////////
// MAKE SURE YOU USE THE RIGHT GROUP NAME AND CAR ID
//char* my_topic = "T1_car_1/distance";
//char* message = "This is car_1";
char* sub_topic = "car2_coordinate";
///////////////////////////////////////////////

/////// declare MQTT client ////////////////////
const IPAddress mqtt_server(192, 168, 0, 2);
WiFiClient esp_client;
PubSubClient client(mqtt_server, 1883, mqtt_callback, esp_client);
////////////////////////////////////////////////


void setup() {
  
  Serial.begin(115200);
  Serial.println(); // clear some mojibake from Serial

  setup_wifi();

  Serial.println(F("Listening..."));
}

void loop() {

  /* MQTT connection */
  if (!client.connected()) {
    reconnect();
  }
    
  /* Skip if there is no serial input */
  if (!Serial.available()){
    /* receive subscribed MQTT topics if any */
    client.loop();
    return;
  }
  
  /* parse the serial input */
  String serial_str;
  serial_str = Serial.readStringUntil('\n');

  char* serial_c_str = new char[serial_str.length() + 1];
  strcpy(serial_c_str, serial_str.c_str());
  char* sub_str = strtok(serial_c_str, ",");

  /* first sub-string is the command */
  if (strcmp(sub_str, "PUB") == 0) {

    /* parse topic and payload */
    sub_str = strtok(NULL, ",");
    if (sub_str == NULL) // the whole command is incomplete
      return;
    String topic(sub_str);
    
    sub_str = strtok(NULL, ",");
    if (sub_str == NULL) // the whole command is incomplete
      return;
    char* payload = sub_str;

    
    /* publish to MQTT broker */
    client.publish(topic.c_str(), payload);
  }
  
  /* receive subscribed MQTT topics if any */
  client.loop();

  delete [] serial_c_str;  // avoid memory leak
  delay(50);
}



bool setup_wifi() {
  
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(F("."));
    
  }

  randomSeed(micros());
  
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  
  Serial.println();
  Serial.println(F("CONNECTED"));
  return true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print(F("Attempting MQTT connection..."));
     
    // Create a random client ID
    String clientId = "Smart_Car-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println(F("connected"));

      // subscribe topic of interest
      client.subscribe(sub_topic);

    } 
    else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      Serial.print(F("ERROR,Connection failed"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int len) {  
  Serial.print(F("MSG,"));
  Serial.print(topic);
  Serial.print(',');
  for (unsigned int i = 0; i < len; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}
