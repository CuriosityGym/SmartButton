#include <ESP8266WiFi.h>
#define EVENTS 8

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CuriosityGym-BCK";
char password[] = "#3Twinkle3#";

String IFTTT_KEY = "iYiYhj3KyPFEwyVRuJzEb"; 
// my api key  iYiYhj3KyPFEwyVRuJzEb(for testing)
const char* eventName  = "IOT_BUTTON";
const char* IFTTT_URL = "maker.ifttt.com";

const int BUTTON_PIN = 0;

//////////////////////
// Button Variables //
//////////////////////
int BUTTON_STATE;
int LAST_BUTTON_STATE = HIGH;
long LAST_DEBOUNCE_TIME = 0;
long DEBOUNCE_DELAY = 100;
int BUTTON_COUNTER = 0;
int IDLE_DELAY=5000;
int lastPressedMillis=0;


// Debounce Button Presses
boolean debounce() {
  boolean retVal = false;
  int reading = digitalRead(BUTTON_PIN);
  if (reading != LAST_BUTTON_STATE) {
    LAST_DEBOUNCE_TIME = millis();
  }
  if ((millis() - LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY) {
    if (reading != BUTTON_STATE) {
      BUTTON_STATE = reading;
      if (BUTTON_STATE == LOW) {
        retVal = true;
      }
    }
  }
  LAST_BUTTON_STATE = reading;
  return retVal;
}

void setup()
{
   Serial.begin(9600);
 // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
    
}

void loop()
{
    // Define the WiFi Client
       WiFiClient client;
       // Set the http Port
       const int httpPort = 80;
 // Wait for button Presses
  boolean pressed = debounce();
  if (pressed == true)
  {
      if(BUTTON_COUNTER==EVENTS)// This will make the Button Counter roll back to 1
      {
        BUTTON_COUNTER=0;
      }
      BUTTON_COUNTER++;
      Serial.print(BUTTON_COUNTER);
      
      lastPressedMillis=millis();  

  }
  if((millis()-lastPressedMillis)>IDLE_DELAY && lastPressedMillis>0)  //Wait for " IDLE_DELAY" Time to ensure person has clicked switch the right number of times
  {
    lastPressedMillis=0;
    Serial.print("Trigger" + String(eventName) + " Event Pressed ");
    Serial.print(BUTTON_COUNTER);
    Serial.println(" times");
       // Make sure we can connect
       if (!client.connect(IFTTT_URL, httpPort))
          {
            return;
          }

       // We now create a URI for the request
       // String url = "/trigger/" + String(eventName) + "-"+String(eventCount)+ "/with/key/" + String(IFTTT_KEY);
       String url = "/trigger/" + String(eventName) + "/with/key/" + String(IFTTT_KEY);

       // Set some values for the JSON data depending on which event has been triggered
       IPAddress ip = WiFi.localIP();
       String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
       String value_1 = String(BUTTON_COUNTER);
       //String value_2 = String(h);
       //String value_3 = "";
       


        // Build JSON data string
        String data = "";
        data = data + "\n" + "{\"value1\":\"" + value_1 + "\"}";

        // Post the button press to IFTTT
        if (client.connect(IFTTT_URL, httpPort))
           {

             // Sent HTTP POST Request with JSON data
             client.println("POST " + url + " HTTP/1.1");
             Serial.println("POST " + url + " HTTP/1.1");
             client.println("Host: " + String(IFTTT_URL));
             Serial.println("Host: " + String(IFTTT_URL));
             client.println("User-Agent: Arduino/1.0");
             Serial.println("User-Agent: Arduino/1.0");
             client.print("Accept: *");
             Serial.print("Accept: *");
             client.print("/");
             Serial.print("/");
             client.println("*");
             Serial.println("*");
             client.print("Content-Length: ");
             Serial.print("Content-Length: ");
             client.println(data.length());
             Serial.println(data.length());
             client.println("Content-Type: application/json");
             Serial.println("Content-Type: application/json");
             client.println("Connection: close");
             Serial.println("Connection: close");
             client.println();
             Serial.println();
             client.println(data);
             Serial.println(data);

            BUTTON_COUNTER=0;
           }
      //delay(30000);
   }

}
