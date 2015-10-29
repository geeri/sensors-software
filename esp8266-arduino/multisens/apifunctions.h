#ifndef APIFUNCTIONS_H
#define APIFUNCTIONS_H
// #include <ESP8266WiFi.h>
/**********************************************/
/* WiFi connecting script
/**********************************************/
void connectWifi() {
  WiFi.begin(ssid, password); // Start WiFI
  
  Serial.print("Connecting ");
  while (WiFi.status() != WL_CONNECTED) 
  {
#ifdef PIN_LED_STATUS
digitalWrite(PIN_LED_STATUS, LOW);
#endif
    delay(500);
    Serial.print(".");
#ifdef PIN_LED_STATUS
digitalWrite(PIN_LED_STATUS, HIGH);
#endif
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  uint8_t mac[6];
    for (int i = 0; i < 6; ++i) {
     Serial.print(String(mac[i], 16));
    if (i < 5)
      Serial.print(":");
    }
    Serial.println();
  WiFi.printDiag(Serial);
}
/**********************************************/
/* send data to rest api
/**********************************************/
void sendData(const String& data) {
  //  delay(60000);
  ++value;

#ifdef PIN_LED_STATUS
digitalWrite(PIN_LED_STATUS, LOW);
#endif

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // create an URI for the request
  String url = "/v1/push-sensor-data/";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  Serial.println(ESP.getChipId());
  Serial.println(data);
  
  // send request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Sensor: esp8266-");
  client.println(ESP.getChipId());
  client.print("Content-Length: ");
  client.println(data.length(), DEC);
  client.println("Connection: close\r\n");
  client.println(data);
  delay(1);
  
  // Read reply from server and print them
  while(client.available()){
    char c = client.read();
    Serial.print(c);
  }
  
  Serial.println();
  Serial.println("closing connection");
#ifdef PIN_LED_STATUS
digitalWrite(PIN_LED_STATUS, HIGH);
#endif
}

String Float2String(float value)
{
  // Convert a float to String with two decimals.
  String s;
  s = String(int(value));
  s += '.';
  s += int((value - int(value)) * 100);

  return s;
}

#endif
