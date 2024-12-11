#include <AWS_IOT.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <stdlib.h>

AWS_IOT hornbill;

char WIFI_SSID[]="<wifi_ssid>"; 
char WIFI_PASSWORD[]="<pswd>"; 
char HOST_ADDRESS[]="xxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com"; // REST API endpoint 
char CLIENT_ID[]= "192.168.0.14";   //IP address of ESP32 when connected to wifi
char TOPIC_NAME[]= "LEDbulbs/topic001"; //Subscription to the thing topic 

const int output02 = 2; //RED LED 
const int output05 = 5; //green LED 

int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
 
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void connectToWifi()
{
    while (status != WL_CONNECTED)
    {
        Serial.print("Connecting to SSID: ");
        Serial.println(WIFI_SSID);
        
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);        
        delay(5000);
    }
    Serial.println("Connected to wifi ");
    Serial.print("ESP32 Client IP address: ");
    Serial.println(WiFi.localIP()); //add a new line after this is printed
}


void connectToAWS()
{
  if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        delay(2000);
        Serial.println("Connected to AWS ");        

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe to topic Successful");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }
}

void setup() {
    Serial.begin(115200);     
    connectToWifi();    
    connectToAWS();
    pinMode(output02, OUTPUT);    
    pinMode(output05, OUTPUT);    
    digitalWrite(output02, LOW);    
    digitalWrite(output05, LOW);    
}

/*******  Loop method ********/
void loop()
{
   /* Message Payload
    {
      "sensor": "ESP32",
      "location": "UK",
      "node1": {
        "type": "LED",
        "color": "red",
        "value": "1"    //1 to turn ON, 0 to turn off    
      },
      "node2": {
        "type": "LED",
        "color": "green",
        "value": "0"    //1 to turn ON, 0 to turn off    
      }
    } 
   */
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
      
        StaticJsonBuffer<512> jsonBuffer;  
        JsonObject& root = jsonBuffer.parseObject(rcvdPayload);
      
        // Test if parsing succeeds.
        if (!root.success()) {
          Serial.println("parseObject() failed");
          return;
        }

        // Fetch values.
        const char* sensor = root["sensor"];
        const char* led2Value = root["node1"]["value"];
        const char* led5Value = root["node2"]["value"];
      
        // Print values.
        Serial.println(sensor);
        Serial.print("Red = ");
        Serial.println(led2Value);
        Serial.print("Green = ");
        Serial.println(led5Value);

        //parse the JSON message to extract LED status values
        int redLED = atoi(led2Value);
        int greenLED = atoi(led5Value);
        digitalWrite(output02, redLED);//set the red LED ON  
        digitalWrite(output05, greenLED);//set the red LED ON  
        
    }
}
