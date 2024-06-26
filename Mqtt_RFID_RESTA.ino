#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>   // biblioteca Servo

#include "config.h"  // Sustituir con datos de vuestra red
#include "MQTT.hpp"
#include "ESP8266_Utils.hpp"
#include "ESP8266_Utils_MQTT.hpp"

// Definición de pines utilizados para el lector RFID
#define SS_PIN 2  // Pin D4
#define RST_PIN 0 // Pin D3

#include <SPI.h>     // biblioteca SPI
#include <MFRC522.h> // biblioteca MFRC522

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear objeto MFRC522
Servo myservo;        // Crear objeto Servo
int contador = 0;
int contador_restado = 0;
const int ledPIN = 15;


void setup(void)
{
	Serial.begin(115200);
	SPIFFS.begin();

  SPI.begin();        // Iniciar bus SPI
  mfrc522.PCD_Init(); // Iniciar MFRC522

  myservo.attach(5);  // Adjuntar el servo al pin 5
  Serial.println("Iniciado");
  myservo.write(0);   // Posicionar el servo en 0 grados

  Serial.println("Iniciado");

	ConnectWiFi_STA(true);

	InitMqtt();
  
  pinMode(ledPIN , OUTPUT);  //definir pin como salida

}

//boolean conect = false;

void loop()
{
  myservo.write(0);   // Posicionar el servo en 0 grados
  HandleMqtt();
  contador = obtener_contador();
  //conect = conectado_cliente();
  //Serial.println(conect);

  digitalWrite(ledPIN , LOW);   // poner el Pin en LOW

  String userid = "";


 	// Revisamos si hay nuevas tarjetas  presentes
	if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
  		//Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  Serial.print("Card UID:");
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX); 
                          userid += String(mfrc522.uid.uidByte[i], HEX);
                  } 
                  digitalWrite(ledPIN , HIGH);   // poner el Pin en HIGH

                  Serial.println();
                  Serial.println(userid);
                  PublisMqttStringRFID(userid);
                  contador_restado = contador-1;
                  Serial.print("Numero entradas: ");
                  Serial.println(contador_restado);
                  PublisMqttRFID(contador_restado);
                  Serial.println();
                  myservo.write(180);
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA(); 
                  delay(1000);
            }      
	}	
  
}




