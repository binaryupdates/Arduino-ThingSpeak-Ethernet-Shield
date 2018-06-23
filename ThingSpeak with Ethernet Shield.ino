#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1}; // Must be unique on local network

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "PX3O6YJA1ZJELH0N";
const int updateThingSpeakInterval = 16 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

long lastConnectionTime = 0;
boolean lastConnected = false;

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  startEthernet();
}

void loop()
{
  int temp;
  temp = analogRead(A0);
  String analogValue0 = String(temp, DEC);

  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected() && lastConnected)
  {
    Serial.println("...disconnected");
    client.stop();
  }

  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval))
  {
    updateThingSpeak("field1=" + analogValue0);
  }

  lastConnected = client.connected();
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

    lastConnectionTime = millis();

  }
  else
  {
    Serial.println("Connection to ThingSpeak Failed.");
    lastConnectionTime = millis();
  }
}

void startEthernet()
{
  client.stop();
  Serial.println("Connecting Arduino to network...");
  delay(1000);

  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
  }
  else
  {
    Serial.println("Arduino connected to network using DHCP");
  }
  delay(1000);

}
