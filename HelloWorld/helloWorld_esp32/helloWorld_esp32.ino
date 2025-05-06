#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "Vasiljevic-2.4G";
const char* password = "RadenkoSalapura";

WiFiServer server(8080);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..."); //touch the antenna...
        Serial.println(WiFi.status());
    }

    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                String message = client.readStringUntil('\n');
                Serial.println("Received: " + message);
                client.write("Hello from ESP32 server!");
            }
        }
        client.stop();
        Serial.println("Client disconnected");
    }
}
