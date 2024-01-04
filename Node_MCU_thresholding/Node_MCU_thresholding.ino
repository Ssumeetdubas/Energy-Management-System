// this code does the current thresholding using the threshold variable 
#define BLYNK_TEMPLATE_ID "TMPL3r4IuPZRZ"
#define BLYNK_TEMPLATE_NAME "PowerManager"
#define BLYNK_AUTH_TOKEN "YDMca7B8f-JQesrUPzC3MgN2SqSEE3wq"

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define led1 D2
float threshold=0.25; // give the threshold value 
char auth[] = BLYNK_AUTH_TOKEN;
const int sensorIn = A0;
int mVperAmp = 185;
int Watt = 0;
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double offset = 0.0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, "SGD", "12345sgd");
  pinMode(led1,OUTPUT); 
}

void loop() {
  Blynk.run();

  Voltage = getVPP();
  VRMS = (Voltage / 2.0) * 0.707;
  AmpsRMS = ((VRMS * 1000) / mVperAmp);

  Serial.print("Voltage: ");
  Serial.print(VRMS);
  Serial.print("V, Current: ");
  Serial.print(AmpsRMS, 2);
  Serial.print("A RMS  ---  ");
  
  Watt = int(AmpsRMS * 240); // Assuming 240V as line voltage
  Serial.print("Power: ");
  Serial.print(Watt);
  Serial.println(" Watts");

  Blynk.virtualWrite(V0, AmpsRMS);
  if(AmpsRMS>threshold)
  {
    Blynk.virtualWrite(V1, 255);
    digitalWrite(led1,HIGH);
  }
  else
  {
    Blynk.virtualWrite(V1, 0);
    digitalWrite(led1,LOW);
  }
  delay(500);
}

float getVPP() {
  int readValue;
  int maxValue = 0;
  int minValue = 1023;

  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) {
    readValue = analogRead(sensorIn);
    if (readValue > maxValue) {
      maxValue = readValue;
    }
    if (readValue < minValue) {
      minValue = readValue;
    }
  }

  float result = ((maxValue - minValue) * 5.0) / 1024.0;
  return result;
}
