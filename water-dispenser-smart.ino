#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "UPBGuest";
const char* password = "";

// MQTT Broker
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_topic = "water-dispenser";

// Sensor pins
#define sensorPower 13
#define sensorPin A0
const int trigPin = 12;
const int echoPin = 14;
const int pumpPin = 15;

// Variables for storing sensor readings
int waterLevel = 0;
float distance = 0;

// Counter for pump activations
int pumpActivationCount = 0;

// MQTT client and WiFi client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Callback function
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe to MQTT topic
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Initialize sensor pins
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW); // Turn the sensor OFF
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check for pet presence every 5 seconds
  static unsigned long lastPetCheckTime = 0;
  if (millis() - lastPetCheckTime >= 5000) {
    lastPetCheckTime = millis();
    checkPetPresence();
  }
}

// Function to check if there's a pet in front of the ultrasonic sensor
void checkPetPresence() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;

  if (distance < 20) { // Assuming a pet is present if distance is less than 20 cm
    // Pet detected, do nothing
    Serial.println("Pet detected");
  } else {
    // No pet detected, check water level
    int waterLevel = readSensor();
    Serial.print("Water level: ");
    Serial.println(waterLevel);
    Serial.print("Activasion count: ");
    Serial.println(pumpActivationCount);

    // If water level is too low, turn on pump for 3 seconds
    if (waterLevel < 100) {
      Serial.println("Water level too low, turning on pump");
      digitalWrite(pumpPin, HIGH);
      delay(3000); // Run pump for 3 seconds
      digitalWrite(pumpPin, LOW);

      // Increment pump activation count
      pumpActivationCount++;

      // If pump has been activated 5 times, send MQTT message
      if (pumpActivationCount >= 5) {
        client.publish(mqtt_topic, "You need to refill the water dispenser");
        pumpActivationCount = 0; // Reset counter after sending message
      }
    }
  }
}

// Function to read water level sensor
int readSensor() {
  digitalWrite(sensorPower, HIGH); // Turn the sensor ON
  delay(10);                        // Wait for sensor to stabilize
  int level = analogRead(sensorPin); // Read the analog value from sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return level;                     // Return current reading
}
