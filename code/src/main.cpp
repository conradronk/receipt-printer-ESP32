#include "Arduino.h"

#include "string.h" // unsure if this is even needed
#include "WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "time.h"


#include "EPPrinter.h"
#include "Ticket.h"


// ---- defines etc. ---- //
#define control_loop_interval 100
uint32_t controlLoopScheduled;

#define wifi_manager_loop_interval 500
uint32_t wifiManagerLoopScheduled;

#define rs232_tx_pin 32
#define rs232_rx_pin 33

#define top_debug 25
#define bottom_debug 26
uint8_t topButton;
uint8_t topButtonLast;
uint8_t bottomButton;
uint8_t bottomButtonLast;

#include "credentials.h"
// ----  Wifi Config ---- //
// const char *ssid = "ssid";
// const char *password = "password";

// ---- MQTT Config ---- //
// const char *mqtt_broker = "mqtt broker";
// const char *topic = "typically in format user/f/channel";
// const char *mqtt_username = "username";
// const char *mqtt_key = "mqtt key phrase";
// const char *clientId = "ESP-Receipt-Client";
// const int mqtt_port = 1883;


// ---- Time Confit ---- //
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800; // For PST
const int   daylightOffset_sec = 3600;



// ---- Globals & Objects ---- //
EPPrinter RS232(&Serial2);
WiFiClient NetworkClient;
PubSubClient MqttClient(NetworkClient);


// ---- Function Prototypes ---- //
void callback(char *topic, byte *payload, unsigned int length);


/* ---- ---- ---- ---- ---- ---- */
/*             Setup             */
/* ---- ---- ---- ---- ---- ---- */
void setup() {
  // ---- General init ---- //
  Serial.begin(115200);


  // ---- RS232 init ---- //
  Serial2.begin(baud_rate, SERIAL_8N1, rs232_rx_pin, rs232_tx_pin);
  RS232.initialize();
  

  // ---- GPIO Init ---- //
  pinMode(top_debug, INPUT_PULLUP);
  pinMode(bottom_debug, INPUT_PULLUP);


  // ---- Wifi Init ---- //
  //move this to a function and just call it? In case needed to re-establish
  WiFi.begin(ssid, password);
  // So I'd like to move this to somewhere in loop, so that the whole thing
  // doesn't just hang on the wifi situation. Be ideal to still be able to
  // interact with the device in the meantime
  while (WiFi.status() != WL_CONNECTED) { //unlimited attempts, sooo
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("connected to wifi!");

  // ---- Time Init ---- //
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


  // ---- Mqtt Init ---- //
  // ibid: move to a function
  MqttClient.setServer(mqtt_broker, mqtt_port);
  MqttClient.setCallback(callback);
  while (!MqttClient.connected()) { //also a required loop atm
    if (MqttClient.connect(clientId, mqtt_username, mqtt_key)) {
      Serial.println("Successfully connected to mqtt broker");
    } else {
      Serial.print("Failed to connect to broker. State: ");
      Serial.println(MqttClient.state());
    }
  }
  MqttClient.setBufferSize(1024);
  MqttClient.subscribe(topic);
}

void loop() {
  MqttClient.loop();
  /* ---- ---- ---- ---- ---- ---- */
  /*         Control  Loop         */
  /* ---- ---- ---- ---- ---- ---- */
  if (millis() >= controlLoopScheduled) {
    controlLoopScheduled = millis() + control_loop_interval;
    topButton = !digitalRead(top_debug);
    bottomButton = !digitalRead(bottom_debug);
  }
  
  /* ---- ---- ---- ---- ---- ---- */
  /*        Wifi Manager Loop      */
  /* ---- ---- ---- ---- ---- ---- */
  if (millis() >= wifiManagerLoopScheduled) {
    wifiManagerLoopScheduled = millis() + wifi_manager_loop_interval;
    //this is wifi code
  }
  
  /* ---- ---- ---- ---- ---- ---- */
  /*      Debug button Crap        */
  /* ---- ---- ---- ---- ---- ---- */
  if (topButton && !topButtonLast) {
    //other test code
  }
  topButtonLast = topButton;

  if (bottomButton && !bottomButtonLast) {
    // TEST CODE UAU!
  }
  bottomButtonLast = bottomButton;
}

/* ---- ---- ---- ---- ---- ---- */
/*          Functions            */
/* ---- ---- ---- ---- ---- ---- */
void callback(char *topic, byte *payload, unsigned int length) {

  //Some decent code to see what comes in an when
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
  }
  Serial.println();
  
  Ticket Stub; //So this will be created on the stack, discarded after callback is ran
  
  Stub.ingestJSON(payload, length);
  
  struct tm timeInfo; // = {0};
  getLocalTime(&timeInfo);

  Stub.sendToPrinter(RS232, timeInfo); // also pass in a time struct
  

  // Sample Code for json processing
  // StaticJsonDocument<256> jsonPayload;  //Using static, which will put it on the stack
  //                                       //If we going big with it (they saying 1KB but I think we
  //                                       // have some overhead with the esp32)
  // DeserializationError err = deserializeJson(jsonPayload, payload, length);
  // Serial.println(err.f_str());
  // const char* sensor1 = jsonPayload["sensor-1"];
  // Serial.println(sensor1);
  // float sensor2 = jsonPayload["sensor-2"];
  // Serial.println(sensor2);
}