#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#define USER_SSID "Vasiljevic-2.4G"
#define USER_PASSWORD "RadenkoSalapura"
#define LED_OUTPUT 4
#define SWITCH_INPUT 5


WiFiServer server(8080); 

void setup() {
  
    Serial.begin(115200);
    WiFi.begin(USER_SSID, USER_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
        Serial.println("Connecting to WiFi..."); //touch the antenna...
    }

    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP());
    server.begin();

    pinMode(LED_OUTPUT, OUTPUT);
    pinMode(SWITCH_INPUT, INPUT_PULLUP);

    digitalWrite(LED_OUTPUT, HIGH);
    delay(500);
    digitalWrite(LED_OUTPUT, LOW);
}

void loop() {
    WiFiClient client = server.available(); //check for an incomming connection from client

    if (client) //check if the client is connected
    {
        while (client.connected()) 
        {
            // Send switch input once every second
            client.println(digitalRead(SWITCH_INPUT));
            delay(10);

            // Check for incoming message from client
            while (client.available()) 
            {
                String cmd = client.readStringUntil('\n');
                cmd.trim(); // Remove \r or whitespace

                if (cmd == "LED_ON") 
                {
                    digitalWrite(LED_OUTPUT, HIGH);
                } 
                else if (cmd == "LED_OFF") 
                {
                    digitalWrite(LED_OUTPUT, LOW);
                }
            }
        }

        client.stop();
    }
    else
    {
      if (digitalRead(SWITCH_INPUT) != HIGH) 
      {
          digitalWrite(LED_OUTPUT, LOW); //if the client disconnected with the light off turn the LED off to save power
      } 
      else
      {
          //nothing to do LED is OFF
      }      
    }
}
