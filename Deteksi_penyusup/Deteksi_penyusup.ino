
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

  
#define BLYNK_PRINT Serial

const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
char auth[] ="LW_wAtPx13yh7wFO28WCR8stdSPTEPe5";
// Set password to "" for open networks.
char ssid[] = "ORLANDO";
char pass[] = "HAARITSKHAIZAFIR";
const int trigPin =5;
const int echoPin =16;
int id =0;
long duration;
int distance;
int pinValue;
String currentTime;

int previous_millis = millis();

BlynkTimer timer;


void setup() {
  Serial.begin(115200);
 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  //calling the connectToWifi() function
 connectToWifi();
 timeClient.begin();
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com",8080);
    timer.setInterval(300L,sendSensor);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectToWifi();        //reconnect in case of connection loss
  }
  Blynk.run();
  timer.run();
}


void connectToWifi(){
  // funtion to connect to WiFi
  WiFi.begin(ssid, pass);
  Serial.print("\n\nconnecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nconnected: ");
  Serial.println(WiFi.localIP());
}

void sendSensor(){

  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
  Blynk.virtualWrite(V6, distance);
//delay(100);

  
if((distance<40) && (pinValue == 1))
{
    timeClient.update();
   
// Length (with one extra character for the null terminator)

    
  currentTime = String("Hari "+String(daysOfTheWeek[timeClient.getDay()])+" Jam "+String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()));
    String currentTime2 = String(String(daysOfTheWeek[timeClient.getDay()])+" "+String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()));

   Serial.println(currentTime);
    Serial.println(String("Broo!! ")+currentTime+String(" ada Penyusup!! "));
long uptime = millis() / 60000L;

  // Actually send the message.
  // Note:
  //   We allow 1 notification per 5 seconds for now.
  //Blynk.notify(String("Running for ") + uptime + " minutes.");
  String emailisi= "Ada Penyusup Dikamar bro!! "+ currentTime;
  Blynk.email("monuilfaku6@gmail.com","Penyusup allert!", emailisi);
  Blynk.notify(String("Broo!! ")+currentTime+String(" Ada Penyusup!! "));
   Blynk.virtualWrite(V1,"add",id++,"Penyusup",currentTime2);
}

}

BLYNK_WRITE(V2) // V5 is the number of Virtual Pin  
{
  pinValue = param.asInt();
}
