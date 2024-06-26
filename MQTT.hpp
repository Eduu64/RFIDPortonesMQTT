//const char* MQTT_BROKER_ADRESS = "192.168.1.114";
const char* MQTT_BROKER_ADRESS = "10.34.181.200";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_CLIENT_NAME = "ESP8266Client_2";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void SuscribeMqtt()
{
	//mqttClient.subscribe("PT/RFID");
  mqttClient.subscribe("PT/RFID_CONTADOR",1);

}

String payload;
int contador_actual = 6;

void PublisMqttRFID(int data)
{
	payload = "";
	payload = String(data);
	mqttClient.publish("PT/RFID_CONTADOR", (char*)payload.c_str());
}

void PublisMqttStringRFID(String data)
{
	payload = "";
	payload = String(data);
	mqttClient.publish("PT/RFID", (char*)payload.c_str());
}

String content = "";
void OnMqttReceived(char* topic, byte* payload, unsigned int length) 
{
	Serial.print("Received on ");
	Serial.print(topic);
	Serial.print(": ");

	content = "";	
	for (size_t i = 0; i < length; i++) {
		content.concat((char)payload[i]);
	}

  contador_actual = content.toInt();
	Serial.print(content);
	Serial.println();
}

  int obtener_contador(){
    return contador_actual;
}


