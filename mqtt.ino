#include <stdio.h>
#include "Streaming.h"
#include <String.h>


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    Serial.println();
      
    if (client.connect(clientName, mqtt_username, mqtt_password)) 
    {
      Serial.println("connected");
    }
    else
    {
      
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      Serial.println(clientName);
      Serial.println(mqtt_username);
      Serial.println(mqtt_password);
   
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message arrived [" + String(topic)+"] ");
  Serial.println("Length: " + String(length,DEC));
  char message_buff[100];
  int i;
    for (i = 0; i < length; i++){
      message_buff[i] = payload[i];    
  }
  message_buff[i] = '\0';
  Serial.println();
 String msgString = String(message_buff);
 Serial.println("Payload: " + msgString);
  
     if(String(topic)==String(mqtt_subtopicstate))
     {
        Serial.println("state:" +msgString);
        state=msgString.toInt();
     }

     if(String(topic)==String(mqtt_subtopicled))
     {
        Serial.println("led:" +msgString);
    
     } 
     if(String(topic)==String(mqtt_subtopiccolor))
     {
        Serial.println("color:" +msgString);
     } 
     if(String(topic)==String(mqtt_subtopictext))
     {
        Serial.println("Text:" +msgString);
        cText=msgString;
        state=8;
     } 
}
