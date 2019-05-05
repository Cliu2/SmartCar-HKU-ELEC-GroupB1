#include "ESP_WiFi.h"

WiFi_Car::WiFi_Car(String myTopic){
	my_topic=myTopic;
	Serial1.begin(115200);
	wait_esp8266_connected();
}

void WiFi_Car::wait_esp8266_connected(){
	String serial_str("");
	Serial1.println("CON,none,none");
	do {
		while (Serial1.available()) {
			serial_str = Serial1.readStringUntil('\n');
			// Serial.println(serial_str);
			// Serial.println(serial_str != "CONNECTED\r");
			if (serial_str == "CONNECTED\r"){
				// Serial.println("Found connected !!!");
				break;
			}
		}
		// if (serial_str != "CONNECTED\r"){	//a typo here?
	}while (serial_str != "CONNECTED\r");
}

MqttMsg WiFi_Car::parse_mqtt_msg(String str, bool &success){
	MqttMsg mqtt_msg;

	// create char* that keeps str for subsequent operations
	char* c_str = new char[str.length() + 1];
	strcpy(c_str, str.c_str());

	// The C library function char *strtok(char *str, const char *delim) 
	// breaks string str into a series of tokens using the delimiter delim.
	char* sub_str = strtok(c_str, ",");
	
	if (strcmp(sub_str, "MSG") != 0) {
		success=false;
		return mqtt_msg; // str does not contain MQTT message
	}
	
	sub_str = strtok(NULL, ",");
	if (sub_str == NULL) {
		success=false;
		return mqtt_msg; // str not complete
	}
	mqtt_msg.topic = String(sub_str);
	
	sub_str = strtok(NULL, ",");
	if (sub_str == NULL) {
		success=false;
		return mqtt_msg; // str not complete
	}
	mqtt_msg.payload = String(sub_str);

	success=true;
	return mqtt_msg;
}

void WiFi_Car::send(String msg){
	Serial1.print("PUB,"+my_topic+",");
	Serial1.print(msg);
	Serial1.print("\n");
}

MqttMsg WiFi_Car::receive(bool &success){
	MqttMsg mqtt_msg;
	while(Serial1.available()){
		String serial_str;
		serial_str = Serial1.readStringUntil('\n');

		bool res=false;
		Serial.println(serial_str);
		mqtt_msg = parse_mqtt_msg(serial_str,res);
		if(res){
			success=true;
			return mqtt_msg;
		}
	}
	success=false;
	return mqtt_msg;
}