/******************************************************************
 Project        : P4 Weather Station
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : 
 InitialRelease : 02.08.2019 
 LastUpdate     : 23.08.2019 
******************************************************************/
// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached 
#define MY_RADIO_RF24

// Board Info
#define N_MODULE "P4_WeatherStation"
#define V_MODULE "1.1"

// Pressure at sea level
#define SEALEVELPRESSURE_HPA (1013.25)

// Child ID for gateway
#define CHILD_ID_BATTERY 40
#define CHILD_ID_HUM 41
#define CHILD_ID_TEMP 42
#define CHILD_ID_PRS 43
//#define CHILD_ID_ALT 44

//Define ports
#define BATTERY_SENSE_PIN A0
#define LEDLOW_PIN 8
#define LEDMID_PIN 7
#define LEDHIGH_PIN 6

// Include libraries
#include <MySensors.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Initialize variables
int16_t oldBatteryPcnt = 999;
static const uint32_t UPDATE_INTERVAL = 10000;
float lastHum;
float lastTemp;
float lastPrs;
//float lastAlt;

//Initialize device
Adafruit_BME280 bme;

//Initialize device
MyMessage batteryPercentMsg(CHILD_ID_BATTERY, V_TEXT);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgPrs(CHILD_ID_PRS, V_PRESSURE);
//MyMessage msgAlt(CHILD_ID_ALT, V_DISTANCE);

void presentation()  
{ 
  //Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_BATTERY, S_INFO, "WS_BATTERY");
  present(CHILD_ID_HUM, S_HUM , "WS_HUM");
  present(CHILD_ID_TEMP, S_TEMP , "WS_TEMP");
  present(CHILD_ID_PRS, S_BARO , "WS_BARO");
  //present(CHILD_ID_ALT, S_DISTANCE);
}


void setup()
{ 
  //Setting up pins
  pinMode(LEDLOW_PIN, OUTPUT);
  pinMode(LEDMID_PIN, OUTPUT);
  pinMode(LEDHIGH_PIN, OUTPUT);

  digitalWrite(LEDLOW_PIN, HIGH);
  digitalWrite(LEDMID_PIN, HIGH);
  digitalWrite(LEDHIGH_PIN, HIGH);
  
  if (!bme.begin(0x76)) {
  #ifdef MY_DEBUG  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  #endif
    while (1);
  }
}

float readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return (float)result / 1000.0;
}


void loop()      
{  
  // get the battery stats
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  float voltage = (sensorValue * (readVcc() / 1023.0)) * 2.0; //duplicate voltage divider value 
  voltage += 0.19; //Offset del adc
  int batteryPcnt = (int)(100.0 * (voltage - 3.2)); //100.0 * (Read_Volts()-range_min) / (range_max-range_min);
  batteryPcnt = constrain(batteryPcnt, 0, 100);
  
  
  //Update leds info
  if (batteryPcnt >= 75){
    digitalWrite(LEDLOW_PIN, HIGH);
    digitalWrite(LEDMID_PIN, HIGH);
    digitalWrite(LEDHIGH_PIN, HIGH);
  }else if(batteryPcnt >= 50 && batteryPcnt < 75){
    digitalWrite(LEDLOW_PIN, HIGH);
    digitalWrite(LEDMID_PIN, HIGH);
    digitalWrite(LEDHIGH_PIN, LOW);      
  }else if(batteryPcnt >= 25 && batteryPcnt < 50){
    digitalWrite(LEDLOW_PIN, HIGH);
    digitalWrite(LEDMID_PIN, LOW);
    digitalWrite(LEDHIGH_PIN, LOW);     
  }else if(batteryPcnt < 25){
    digitalWrite(LEDLOW_PIN, LOW);
    digitalWrite(LEDMID_PIN, LOW);
    digitalWrite(LEDHIGH_PIN, LOW);      
  }
  
#ifdef MY_DEBUG
  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  Serial.print("VCC Voltage: ");
  Serial.print(readVcc());
  Serial.println(" V");
  Serial.print("Battery percent: ");
  Serial.print(batteryPcnt);
  Serial.println(" %");
#endif
  
  if (oldBatteryPcnt != batteryPcnt) {
      char Output[13];
      char cTmp[5];
      String strOut;
  
      dtostrf(voltage,4,2,cTmp);
      
      strOut = ((String)cTmp) + "V / " + ((String)batteryPcnt) + "%";
      strOut.toCharArray(Output,13);
      
      send(batteryPercentMsg.set(Output));
      sendBatteryLevel(batteryPcnt);
      
      oldBatteryPcnt = batteryPcnt;
  }

  // Get temperature from sensor
  float temperature = bme.readTemperature();
  if (isnan(temperature)) {
#ifdef MY_DEBUG    
    Serial.println("Failed reading temperature from BME280!");
#endif
  } else if (temperature != lastTemp) {
    // Only send temperature if it changed since the last measurement
    lastTemp = temperature;

    send(msgTemp.set(temperature, 1));

#ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
#endif
  }

  // Get humidity from sensor
  float humidity = bme.readHumidity();
  if (isnan(humidity)) {
#ifdef MY_DEBUG
    Serial.println("Failed reading humidity from BME280!");
#endif
  } else if (humidity != lastHum) {
    
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
  
    send(msgHum.set(humidity, 1));

#ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
#endif
  }
  
  // Get pressure from sensor (hPa)
  float pressure = bme.readPressure() / 100.0F;
  if (isnan(pressure)) {
#ifdef MY_DEBUG
    Serial.println("Failed reading pressure from BME280!");
#endif
  } else if (pressure != lastPrs) {
    
    // Only send pressure if it changed since the last measurement
    lastPrs = pressure;
  
    send(msgPrs.set(pressure, 1));

#ifdef MY_DEBUG
    Serial.print("Pre: ");
    Serial.println(pressure);
#endif
  }
/******************  
  // Get altitude from sensor (m)
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  if (isnan(altitude)) {
#ifdef MY_DEBUG
    Serial.println("Failed reading altitude from BME280!");
#endif
  } else if (altitude != lastAlt) {
    
    // Only send altitude if it changed since the last measurement
    lastAlt = altitude;
  
    send(msgAlt.set(altitude, 1));

#ifdef MY_DEBUG
    Serial.print("Alt: ");
    Serial.println(altitude);
#endif
  }
****************/

  // Sleep for a while to save energy
  sleep(UPDATE_INTERVAL); 
}
