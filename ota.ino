#include <ArduinoOTA.h>
#include <FS.h>
#include <ArduinoJson.h> 

void readfile(String file)
{
    if (SPIFFS.exists(file))
    {
      //file exists, reading and loading
      Serial.println("Reading config file");
      
      //Serial.println(file);
      File configFile = SPIFFS.open(file, "r");
      if (configFile)
      {
        Serial.println("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        
        DynamicJsonDocument jsonDoc(1024);
        DeserializationError error = deserializeJson(jsonDoc, buf.get());
        
  
        if (!error)
        {
          Serial.println("parsed json");
          serializeJson(jsonDoc, Serial);
          
          strcpy(ssid, jsonDoc["ssid"]);
          
          strcpy(password, jsonDoc["password"]);
          
          strcpy(devicename, jsonDoc["devicename"]);
          strcpy(mqtt_server, jsonDoc["mqtt_server"]);
          strcpy(mqtt_username, jsonDoc["mqtt_user"]);
          strcpy(mqtt_password, jsonDoc["mqtt_pwd"]);
          strcpy(WSPixel, jsonDoc["WSPixel"]);
          
          Serial.println(ssid);
          //Serial.println(password);
          Serial.println(devicename);
          
        String sName;

        sName=mqtt_topictemp;
        sName.replace("NAME",devicename);
        strcpy(mqtt_topictemp ,sName.c_str());
        
        
        sName=mqtt_topicled;
        sName.replace("NAME",devicename);
        strcpy(mqtt_topicled ,sName.c_str());

        sName=mqtt_topicstate;
        sName.replace("NAME",devicename);
        strcpy(mqtt_topicstate ,sName.c_str());
        
        sName=mqtt_subtopicled;
        sName.replace("NAME",devicename);
        strcpy(mqtt_subtopicled ,sName.c_str());

        sName=mqtt_subtopicstate;
        sName.replace("NAME",devicename);
        strcpy(mqtt_subtopicstate ,sName.c_str());
        
        sName=mqtt_subtopictext;
        sName.replace("NAME",devicename);
        strcpy(mqtt_subtopictext ,sName.c_str());
        
        sName=mqtt_subtopiccolor;
        sName.replace("NAME",devicename);
        strcpy(mqtt_subtopiccolor ,sName.c_str());      
              
        } 
      }
    }
    else
    {
       
    }
}
