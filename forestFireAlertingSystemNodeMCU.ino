
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

char ssid[] = "Technofist";     // your network SSID (name)
char pass[] = "technofist_1602"; // your network password
bool WiFiAP = false;      // Do yo want the ESP as AP?

int counter=0;
class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }
    
    virtual bool onAuth(String username, String password) {
      Serial.println("Username/Password: "+username+"/"+password);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      
      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
    }
};

myMQTTBroker myBroker;

/*
 * WiFi init stuff
 */
void startWiFiClient()
{
  Serial.println("Connecting to "+(String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println();

  // Start WiFi
  if (WiFiAP)
    startWiFiAP();
  else
    startWiFiClient();

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

/*
 * Subscribe to anything
 */
  myBroker.subscribe("#");
}



void loop()
{
/*
 * Publish the counter value as String
 */
  myBroker.publish("forest/test", (String)counter);
  if(Serial.available()){
    String data=Serial.readStringUntil('$');
    Serial.println(data);
    getSubstring(data);
  }
  // wait a second
  delay(100);
//  Serial.clear();
}


void getSubstring(String data){
  String data1=data;
  int l=data1.indexOf(';');
  int count=0;
  for(int i=0;i<3;i++){
    String subData=data1.substring(0,l);
      sp(subData);
    data1.remove(0,l+1);
    data1.trim();
    l=data1.indexOf(';');
  }
}

void sp(String subData){
  int l=subData.indexOf(':');
  String topic=subData.substring(0,l);
  subData.remove(0,l+1);
  subData.trim();
  Serial.println("Topic: "+topic+" Value: "+subData+" ");
  myBroker.publish("forest/"+topic, subData);
  delay(1000*2);
}
