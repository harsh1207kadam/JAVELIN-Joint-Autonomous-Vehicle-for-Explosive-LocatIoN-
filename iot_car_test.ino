
// Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

 int RXPin = 5;
 int TXPin = 4;   
int GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object

SoftwareSerial sim800l(D5,D8); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted

 WidgetLED led1(V12);
SoftwareSerial gpsSerial(D1, D2);  // The serial connection to the GPS device

BlynkTimer timer;

float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

// Define the motor pins
#define ENA 1
#define IN1 D6
#define IN2 D7
#define IN3 D3
#define IN4 D4
#define ENB 3

// Variables for the Blynk widget values (motor)
bool forward=0;
bool backward=0;
bool left=0;
bool right=0;
int Speed;

char auth[] = "Jmjmrpv7Soata85FbIx2W1IMvKbRZKBL"; //Enter your Blynk auth token
char ssid[] = "Aryan_hotspot"; //Enter your WIFI name
char pass[] = "qwerty1234"; //Enter your WIFI passowrd

//GPS
//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now


void setup() {
  sim800l.begin(19200);
  delay(2000);
  Serial.begin(19200);
  delay(1000);
  pinMode(D0, INPUT);
    
  //Set the motor pins as output pin
  pinMode(ENA, FUNCTION_3);
  pinMode(ENB, FUNCTION_3);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  //gps
   Serial.println();
  gpsSerial.begin(GPSBaud);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

    //timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
}


//void SendSMS(int i)
//{
//  if(i==1){
//  Serial.println("Sending SMS...");               //Show this message on serial monitor
//  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
//  delay(100);
//  sim800l.print("AT+CMGS=\"+917588669178\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
//  delay(500);  sim800l.print("Mine Detected");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
//  delay(500); sim800l.print((char)26);// (required according to the datasheet)
//  delay(500);
//  sim800l.println();
//  Serial.println("Text Sent.");
//  delay(5000);
//  }
//  else{
//    Serial.print("Mine not detected..... message not sent");
//  }
//  delay(1000);
//}

int blynkLedWidget(){
  int a = digitalRead(D0);
   if (a == HIGH) {
      Serial.println("mine detected");
      led1.on();
      float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng());
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(300);
  sim800l.print("AT+CMGS=\"+917588669178\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(300);  sim800l.print("Mine Detected  "); 
  sim800l.print("http://maps.google.com/maps?q=loc:");   
   sim800l.print(latitude,6);
   sim800l.print(","); 
   sim800l.print(longitude,6);
  delay(300); sim800l.print((char)26);// (required according to the datasheet)
  delay(100);
  sim800l.println();
  Serial.println("Text Sent.");
  
  
      return(1);
   } else {
      Serial.println("The pin is at a LOW Level");
      led1.off();
      return(0);
   }
    
}

//void checkGPS(){
//  if (gps.charsProcessed() < 10)
//  {
//    Serial.println(F("No GPS detected: check wiring."));
//      Blynk.virtualWrite(V7, "GPS ERROR");  // Value Display widget  on V7 if GPS not detected
//  }
//}


// Get the button values
BLYNK_WRITE(V0) {
  forward = param.asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  backward = param.asInt();
}
BLYNK_WRITE(V2) {
  left = param.asInt();
}
BLYNK_WRITE(V3) {
  right = param.asInt();
}
//Get the slider values
BLYNK_WRITE(V4) {
  Speed = param.asInt();
}

// Check these values using the IF condition
void smartcar() {
  if (forward == 1) {
    carForward();
    
  } else if (backward==1) {
    carBackward();
    
  } else if (left==1) {
    carLeft();
    
  } else if (right==1) {
    carRight();
   
  } else if (forward == 0 && backward==0 && left==0 && right==0) {
    carStop();
    
  }
}
void loop() {

 while (gpsSerial.available() > 0) 
    if (gps.encode(gpsSerial.read()))
      displayInfo();
  
  
  Blynk.run();// Run the blynk function
  timer.run();
  smartcar();// Call the main function

//  while((millis() > 5000 && gps.charsProcessed() < 10))
//  {
//    Serial.println("No GPS detected");
//    Serial.println(gps.satellites.value());
//     displayInfo();
//   delay(5000);
//  }
                          //Small delay to avoid detecting the button press many times
    //int xyz = mineDetect();
//                             //And this function is called

int mine= blynkLedWidget();
  //delay(1000);
//   SendSMS(mine);
//   delay(1000);
}

/*************Motor movement functions****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//gps
void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
   // Serial.print("LAT:  ");
   // Serial.println(latitude, 6);  // float to x decimal places
   // Serial.print("LONG: ");
    //Serial.println(longitude, 6);
    Blynk.virtualWrite(V5, String(latitude, 6));   
    Blynk.virtualWrite(V6, String(longitude, 6));  
    
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V8, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V7, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V9, bearing);                   
  }
  
 //Serial.println();
}
