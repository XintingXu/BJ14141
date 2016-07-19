#include <Ethernet.h>
#include <WiFi.h>
#include <SPI.h>
#include "yl_device.h"
#include "yl_w5100_client.h"
#include "yl_messenger.h"
#include "yl_value_data_point.h"
#include "yl_sensor.h"

#define dht_dpin 43

#define DEVICEID 17799
#define DHT11_H 31034
#define DHT11_T 31035
#define AUTO_CONTROL 35367
#define BE_HOT_CONTROL 35368
#define BE_COLD_CONTROL 35369
#define BE_VET_CONTROL 388072
#define DS18B20_T 388088

//define pin number
#define AUTO_CONTROL_PIN 22
#define AUTO_BE_COLD_PIN 24
#define AUTO_BE_HOT_PIN 26
#define HAND_BE_COLD_PIN 28
#define HAND_BE_HOT_PIN 30
#define HAND_BE_VET_PIN 32

//define value board
#define AUTO_TEMP_LOW 20
#define AUTO_TEMP_HIGH 27
#define HAND_TEMP_LOW 5
#define HAND_TEMP_HIGH 40
#define HAND_DEVICE_HIGH 35

byte bGlobalErr;
byte dht_dat[4];

yl_device ardu(DEVICEID);
yl_sensor therm0(DHT11_H, &ardu);
yl_sensor therm1(DHT11_T, &ardu);
yl_sensor therm2(AUTO_CONTROL, &ardu);
yl_sensor therm3(BE_HOT_CONTROL, &ardu);
yl_sensor therm4(BE_COLD_CONTROL, &ardu);
yl_sensor therm5(BE_VET_CONTROL, &ardu);
yl_sensor therm6(DS18B20_T, &ardu);

yl_w5100_client client;
yl_messenger messenger(&client, "372ff6f73d10b192449d1c95dbaecff0", "api.yeelink.net");

void InitDHT();
void ReadDHT();
byte read_dht_dat();
void setPin();

bool auto_control = true;
bool auto_cold = false;
bool auto_hot = false;
bool hand_cold = false;
bool hand_hot = false;
bool hand_vet = false;
bool hand_cold_changed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Get Sensor Value Start");
  byte mac[] = {0xDE, 0xED, 0xBE, 0xEF, 0xFE, 0xAA};
  Ethernet.begin(mac);
  Serial.println("MAC config done");
  InitDHT();
  pinMode(AUTO_CONTROL_PIN, OUTPUT); digitalWrite(AUTO_CONTROL_PIN, HIGH);
  pinMode(AUTO_BE_COLD_PIN, OUTPUT); digitalWrite(AUTO_BE_COLD_PIN, HIGH);
  pinMode(AUTO_BE_HOT_PIN, OUTPUT); digitalWrite(AUTO_BE_HOT_PIN, HIGH);
  pinMode(HAND_BE_COLD_PIN, OUTPUT); digitalWrite(HAND_BE_COLD_PIN, HIGH);
  pinMode(HAND_BE_HOT_PIN, OUTPUT); digitalWrite(HAND_BE_HOT_PIN, HIGH);
  pinMode(HAND_BE_VET_PIN, OUTPUT); digitalWrite(HAND_BE_VET_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  yl_value_data_point dp;
  float dht11_h;
  if (therm0.single_get(messenger, dp))
    dht11_h = dp.get_value();
  Serial.print("DHT11_H = "); Serial.println(dht11_h);
  
  delay(200);

  yl_value_data_point dp1;
  float dht11_t;
  if (therm1.single_get(messenger, dp1))
    dht11_t = dp1.get_value();
  Serial.print("DHT11_T = "); Serial.println(dht11_t);

  delay(200);
  
  yl_value_data_point dp2;
  if (therm2.single_get(messenger, dp2))
    auto_control = dp2.get_value();
  Serial.print("AUTO_CONTROL = "); Serial.println(auto_control);

  delay(200);

  yl_value_data_point dp3;
  if (therm3.single_get(messenger, dp3))
    hand_hot = dp3.get_value();
  Serial.print("BE_HOT_CONTROL = "); Serial.println(hand_hot);

  delay(200);

  yl_value_data_point dp4;
  if (therm4.single_get(messenger, dp4))
    hand_cold = dp4.get_value();
  Serial.print("BE_COLD_CONTROL = "); Serial.println(hand_cold);

  delay(200);

  yl_value_data_point dp5;
  if (therm5.single_get(messenger, dp5))
    hand_vet = dp5.get_value();
  Serial.print("BE_VET_CONTROL = "); Serial.println(hand_vet);

  delay(200);

  yl_value_data_point dp6;
  float ds18b20_t = 23.00;
  if (therm6.single_get(messenger, dp6))
    ds18b20_t = dp6.get_value();
  if ((ds18b20_t - AUTO_TEMP_LOW > 0.0) && (AUTO_TEMP_HIGH - ds18b20_t > 0.0)) {
    auto_cold = false;
    auto_hot = false;
  }
  else {
    if (ds18b20_t - AUTO_TEMP_LOW < 0.0) {
      auto_hot = true;
    }
    else {
      auto_cold = true;
    }
  }
  Serial.print("DS18B20_T = "); Serial.println(ds18b20_t);
/*
  ReadDHT();
  float temp = int(dht_dat[2]) + (double)(dht_dat[3]) / 100.0;

  if (temp - HAND_DEVICE_HIGH > 0.0) {
    hand_cold = false;
    auto_cold = false;
    hand_cold_changed = true;
  } else {
    hand_cold_changed = false;
  }*/
  setPin();
}

void InitDHT()
{
  pinMode(dht_dpin, OUTPUT);
  digitalWrite(dht_dpin, HIGH);
}

void ReadDHT()
{
  bGlobalErr = 0;
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
  if (!dht_in)
  {
    bGlobalErr = 2;
    return;
  }

  delayMicroseconds(80);

  for (i = 0; i < 5; i++)
  {
    dht_dat[i] = read_dht_dat();
  }

  pinMode(dht_dpin, OUTPUT);

  digitalWrite(dht_dpin, HIGH);

  byte dht_check_sum = dht_dat[0] + dht_dat[1] + dht_dat[2] + dht_dat[3];
  if (dht_dat[4] != dht_check_sum)
  {
    bGlobalErr = 3;
    return;
  }
}


byte read_dht_dat()
{
  byte i = 0;
  byte result = 0;
  for (i = 0; i < 8; i++)
  {
    while (digitalRead(dht_dpin) == LOW);
    delayMicroseconds(30);

    if (digitalRead(dht_dpin) == HIGH)
    {
      result |= (1 << (7 - i));
    }

    while (digitalRead(dht_dpin) == HIGH);
  }
  return result;
}

void setPin() {
  Serial.println("Setting Pin");
  if (auto_control) {
    digitalWrite(AUTO_CONTROL_PIN, LOW);
  } else {
    digitalWrite(AUTO_CONTROL_PIN, HIGH);
  }
  if (auto_cold)
  {
    digitalWrite(AUTO_BE_COLD_PIN, LOW);
  } else {
    digitalWrite(AUTO_BE_COLD_PIN, HIGH);
  }
  if (auto_hot) {
    digitalWrite(AUTO_BE_HOT_PIN, LOW);
  } else {
    digitalWrite(AUTO_BE_HOT_PIN, HIGH);
  }
  if (hand_cold) {
    digitalWrite(HAND_BE_COLD_PIN, LOW);
  } else {
    digitalWrite(HAND_BE_COLD_PIN, HIGH);
  }
  if (hand_hot) {
    digitalWrite(HAND_BE_HOT_PIN, LOW);
  } else {
    digitalWrite(HAND_BE_HOT_PIN, HIGH);
  }
  if (hand_vet) {
    digitalWrite(HAND_BE_VET_PIN, LOW);
  } else {
    digitalWrite(HAND_BE_VET_PIN, HIGH);
  }
  if (hand_cold_changed) {
    yl_value_data_point dp(0.0);
    therm4.single_post(messenger, dp);
  }
}

