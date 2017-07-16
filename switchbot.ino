//-----------------------------------------------------------------
//  Switch bot: Alexa controlled ESP8266 with solenoid actuator
//-----------------------------------------------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

// fauxmoESP library emulates a WeMo device on the ESP8266

#define WIFI_SSID "XXXXXXXXX"
#define WIFI_PASS "xxxxxx"
#define SERIAL_BAUDRATE 115200

// solenoid control pin (via a power transistor, such as TIP120)
#define SOL_PIN 14

fauxmoESP wemoem; //initialise WeMo emulator

volatile boolean solenoid_state = false; // off by default

// Setup wifi connection to access point
void wifi_setup() {

    // Set esp8266 to station mode
    WiFi.mode(WIFI_STA);

    // Connect to AP
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait until connected
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
}

void callback(uint8_t device_id, const char * device_name, bool state) {

      Serial.print(" kettle on "); //alexa command action 
      solenoid_state = true;
}

void setup() {
    // initialize digital pin as an output.
    pinMode(SOL_PIN, OUTPUT);
    digitalWrite(SOL_PIN, LOW);
    
    // Wifi
    wifi_setup();

    // Fauxmo
    wemoem.addDevice("kettle"); //alexa recognised device
    wemoem.onMessage(callback);
}

void loop() {
  fauxmo.handle();
  
  if(solenoid_state) {
    digitalWrite(SOL_PIN, HIGH); //actuate solenoid for 200 ms
    delay(200);
    digitalWrite(SOL_PIN, LOW);
    solenoid_state = false;
  }
}
