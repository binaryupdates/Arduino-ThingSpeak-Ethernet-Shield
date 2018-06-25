#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1}; // Must be unique on local network

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "PX3O6YJA1ZJELH0N";

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  client.stop();
  Serial.println("Connecting Arduino to Network");
  delay(1000);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Failed, Arduino Failed to Connect");
  } else {
    Serial.println("Arduino Connected to Network");
  }
  delay(1000);
}

void loop()
{
  int lux;
  lux = analogRead(A0);
  String analogValue0 = String(lux, DEC);

  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected())
  {
    client.stop();
    updateThingSpeak("field1=" + analogValue0);
  }
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    delay(16000);
  }
  else
  {
    Serial.println("Connection to ThingSpeak Failed.");
  }
}
