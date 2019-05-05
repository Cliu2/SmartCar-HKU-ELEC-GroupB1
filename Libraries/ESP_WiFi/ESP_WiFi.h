#include "Arduino.h"

#ifndef IDP_ESP_h
#define IDP_ESP_h

struct MqttMsg {
  String topic;
  String payload;
};

class WiFi_Car{
	public:
		WiFi_Car(String myTopic);
		void send(String msg);
		MqttMsg receive(bool &success);

	private:
		MqttMsg parse_mqtt_msg(String str, bool &success);
		void wait_esp8266_connected();
		String my_topic;
};


#endif