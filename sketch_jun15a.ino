#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

const char* ssid = "AndroidAP";
const char* password = "daim9608";

const char* mqttServer = "m10.cloudmqtt.com";
const int mqttPort = 15026;
const char* mqttUser = "qxuufkxb";
const char* mqttPassword = "iz1cz4OG_42S";
const char* mqttClientId = "Roland";

const int analogInPin = A0;
int sensorValue = 0;
int outputValue = 0;

char sensor[40];

WiFiClient wclient;
PubSubClient client(mqttServer, mqttPort, wclient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
    
  }

  if (client.connect(mqttClientId, mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT...");
  }
  
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
  
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
}

void loop() {
  client.loop();
  // put your main code here, to run repeatedly:
  digitalWrite(D0, HIGH);
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 1, 255);
  //Serial.print("sensor1 = ");
  //Serial.println(sensorValue);
  snprintf(sensor, 40, "sensor1 = %d", outputValue);
  if (client.publish("esp/test", sensor)) {
    Serial.println("sensor1 = " + sensorValue);
  } else {
    client.connect(mqttClientId, mqttUser, mqttPassword);
    delay(10); 
    client.publish("esp/test", sensor);
  }
  digitalWrite(D0, LOW);
  digitalWrite(D1, HIGH);
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  //Serial.print("sensor2 = ");
  //Serial.println(sensorValue);
  snprintf(sensor, 40, "sensor2 = %d", sensorValue);
  if (client.publish("esp/test", sensor)) {
    Serial.println("sensor2 = " + sensorValue);
  } else {
    client.connect(mqttClientId, mqttUser, mqttPassword);
    delay(10);
    
    client.publish("esp/test", sensor);
  }
  digitalWrite(D1, LOW);
  delay(500);
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
