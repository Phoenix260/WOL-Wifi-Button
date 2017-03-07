#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <WakeOnLan.h>

#define buttonPin 0 // Button Pin
#define buttonPin_gnd 13 // Button Pin
#define LED_PIN 5   // (LED) attaches the LED (may be other pins on other boards

const char* ssid     = "ASUS-BOB-BOB-Jelly-Jube";
const char* password = "fuck off get your own";

boolean wifiConnected = false;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

WiFiUDP UDP;
/**
 * The target IP address to send the magic packet to.
 */
IPAddress computer_ip(192, 168, 1, 255); 

/**
 * The targets MAC address to send the packet to
 */
byte mac[] = { 0x54, 0x04, 0xA6, 0x13, 0xDD, 0x96 };


boolean connectWifi(); //empty methods defined here, for declaration see lower.
void sendWOL();

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  UDP.begin(9); //start UDP client, not sure if really necessary.

  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP); //Set Wake monitor pin as INPUT ... also make it pullup to HIGH by default
  pinMode(LED_PIN, OUTPUT);
  pinMode(buttonPin_gnd, OUTPUT);

  buttonState = digitalRead(buttonPin);
  lastButtonState = buttonState;
  
  digitalWrite(buttonPin_gnd, LOW); //Now that all initialization is done, set the other side of the Button to GND
}

void loop()
{
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter & send WOL

    Serial.println("Sending WOL Packet...");

    WakeOnLan::sendWOL(computer_ip, UDP, mac, sizeof mac);
    delay(4000); //sending WOL packets every 4th second.

    Serial.println("... Sent!");

     // Blink the LED pin during setup (for fun)
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;

  if( WiFi.status() != WL_CONNECTED){
    wifiConnected = connectWifi(); //reconnect wifi if disconnected for some reason
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
