#include <Wire.h>
#include <Ethernet.h>
#include <SPI.h>
#include <math.h>
#include "yl_data_point.h"
#include "yl_device.h"
#include "yl_w5100_client.h"
#include "yl_messenger.h"
#include "yl_sensor.h"
#include "yl_value_data_point.h"
#include "yl_sensor.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define dht_dpin 43

#define DEVICEID 17799
#define SENSOR_T 31035
#define SENSOR_H 31034
#define DSBTemp 388088

// Data wire is plugged into pin 40 on the Arduino
#define ONE_WIRE_BUS 40

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void InitDHT();
void ReadDHT();
byte read_dht_dat();


//replace 2633 3539 with ur device id and sensor id
yl_device ardu(DEVICEID);
yl_sensor therm(SENSOR_H, &ardu);
yl_sensor therm2(SENSOR_T, &ardu);
yl_sensor therm3(DSBTemp, &ardu);
//replace first param value with ur u-apikey
yl_w5100_client client;
yl_messenger messenger(&client, "372ff6f73d10b192449d1c95dbaecff0", "api.yeelink.net");

const int THERM_PIN  = 0;

byte bGlobalErr;
byte dht_dat[4];

void setup()
{
  Serial.begin(9600);
 Serial.println("PROGRAM Start");
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAE};
  Ethernet.begin(mac); //
  Serial.println("MAC config done");
  Wire.begin();
  sensors.begin();
  InitDHT();
}

void loop()
{
  Serial.println("Read sensor: ");

  ReadDHT();
  float humid = int(dht_dat[0]) + (double)(dht_dat[1]) / 100.0;
  Serial.print("Humid = ");
  Serial.println(humid);

  yl_value_data_point dp(humid);
  if(therm.single_post(messenger, dp))
    Serial.println("Update Humid Done");
  else
    Serial.println("Update Humid False");

  delay(500);

  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  float result_temp = (sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1) + sensors.getTempCByIndex(2)) / 3.0;
  Serial.print("Average temp : ");
  Serial.println(result_temp);

  yl_value_data_point dp3(result_temp);
  if(therm3.single_post(messenger, dp3))
   Serial.println("Update AverageTemp Done");
  else
    Serial.println("Update AverageTemp False");

  delay(500);

  ReadDHT();
  float temp = int(dht_dat[2]) + (double)(dht_dat[3]) / 100.0;
  Serial.print("Temperature = ");
  Serial.println(temp);

  yl_value_data_point dp2(temp);
  if(therm2.single_post(messenger, dp2))
    Serial.println("Update Temp Done");
  else
    Serial.println("Update Temp False");
  delay(500);
}

void InitDHT()
{
  pinMode(dht_dpin, OUTPUT);
  digitalWrite(dht_dpin, HIGH);
}

void ReadDHT()
{
  bGlobalErr=0;
  byte dht_in;
  byte i;
  
  digitalWrite(dht_dpin, LOW);
  delay(18);
  delay(5);
  
  digitalWrite(dht_dpin, HIGH);
  delayMicroseconds(40);
  
  pinMode(dht_dpin, INPUT);
  
  dht_in = digitalRead(dht_dpin);
  if (dht_in)
  {
    bGlobalErr = 1;
    return;
  }
  
  delayMicroseconds(80);
  
  dht_in = digitalRead(dht_dpin);
  if(!dht_in)
  {
    bGlobalErr = 2;
    return;
  }
  
  delayMicroseconds(80);
  
  for (i=0; i<5; i++)
  {
    dht_dat[i] = read_dht_dat();
  }
  
  pinMode(dht_dpin, OUTPUT);
  
  digitalWrite(dht_dpin, HIGH);
  
  byte dht_check_sum = dht_dat[0] + dht_dat[1] + dht_dat[2] + dht_dat[3];
  if (dht_dat[4] != dht_check_sum)
  {
    bGlobalErr=3;
    return;
  }
}


byte read_dht_dat()
{
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
  {
    while(digitalRead(dht_dpin) == LOW);
    delayMicroseconds(30);

    if (digitalRead(dht_dpin) == HIGH)
    {
      result |= (1<<(7-i));
    }

    while (digitalRead(dht_dpin) == HIGH);
  }
  return result;
}

