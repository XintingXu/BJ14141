#include "OneWire.h"
#include "DallasTemperature.h"

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
}


void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(sensors.getTempCByIndex(0));

  Serial.print("Temperature for Device 2 is: ");
  Serial.println(sensors.getTempCByIndex(1));

  Serial.print("Temperature for Device 3 is: ");
  Serial.println(sensors.getTempCByIndex(2));

  float result_temp = (sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1) + sensors.getTempCByIndex(2)) / 3.0;
  Serial.print("Average temp : ");
  Serial.println(result_temp);
}
