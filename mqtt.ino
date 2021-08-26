
#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.


WiFiClient espClient;
PubSubClient MQTTclient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int MQTTconnectionTimer = 1;
int MQTTPortInUse = 0;

void setupMQTT() {
  if(MqttPort==0) return;                     // do not try to connect if port is zero
  MQTTclient.setServer(MqttServer, MqttPort);
  MQTTclient.setCallback(callback);
  MQTTconnectionTimer = millis()-5000;
  MQTTPortInUse = MqttPort;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic, R1Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly1, HIGH);    // Switch on the Relay if an 1 was received as first character
    else digitalWrite(Rly1, LOW); 
  }
  if(strcmp(topic, R2Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly2, HIGH);
    else digitalWrite(Rly2, LOW); 
  }
  if(strcmp(topic, R3Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly3, HIGH);
    else digitalWrite(Rly3, LOW); 
  }
  if(strcmp(topic, R4Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly4, HIGH);
    else digitalWrite(Rly4, LOW); 
  }
}

void reconnect() {
  if(WiFi.status() != WL_CONNECTED) return; //WIFI disconnected so don't attempt reconnect
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (MQTTclient.connect(MqttID,MqttUser,MqttPasswd)) {
      Serial.println("connected");
      
// subscribe
      MQTTclient.subscribe(R1Topic);
      MQTTclient.loop();
      MQTTclient.subscribe(R2Topic);
      MQTTclient.loop();
      MQTTclient.subscribe(R3Topic);
      MQTTclient.loop();
      MQTTclient.subscribe(R4Topic);
      MQTTclient.loop();
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 10 seconds");
    }
}



void modeMQTT() {
char Inp;
static int x=0;

if(MQTTPortInUse != MqttPort){
  setupMQTT();
  return;
}
if (MQTTclient.connected()) {
    switch(x) {
      case 0:
        Inp = digitalRead(Inp1);                          // read the input
        if(Inp != Inputs[0]) {                            // if its changed ..
          Inputs[0] = Inp;                                // store new state
          msg[0] = Inp+0x30;    // convert to ASCII       // generate MQTT message
          MQTTclient.publish(N1Topic, msg, true);             // and publish it
        }
        break;      
      case 1:
        Inp = digitalRead(Inp2);
        if(Inp != Inputs[1]) {
          Inputs[1] = Inp;
          msg[0] = Inp+0x30;    // convert to ASCII
          MQTTclient.publish(N2Topic, msg, true);
        }
        break;      
    }
    if(++x > 1) x=0;      // bump to next input to process
    MQTTclient.loop();
  }
  else {
    if(MqttPort==0) return;                     // do not try to connect if port is zero  
    if(millis()-MQTTconnectionTimer<10000) return;
    reconnect();
    MQTTconnectionTimer = millis();
  }
}
