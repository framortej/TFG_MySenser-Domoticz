/*****************************************************************
 Project        : P4 Weather Station
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : Standalone solar weather station
 InitialRelease : 04.08.2019 
 LastUpdate     : 05.09.2019 
*****************************************************************/
// Enable debug prints
//#define MY_DEBUG

// Define the node ID
#define MY_NODE_ID 40

// Enable and select radio type attached 
#define MY_RADIO_RF24

// Board Info
#define N_MODULE "P4_WeatherStation"
#define V_MODULE "1.3"

// Child ID for gateway
#define CHILD_ID_BATTERY 41
#define CHILD_ID_HUM 42
#define CHILD_ID_TEMP 43
#define CHILD_ID_PRS 44

// Define ports
#define BATTERY_SENSE_PIN A0
#define LEDLOW_PIN 8
#define LEDMID_PIN 7
#define LEDHIGH_PIN 6

// Include libraries
#include <MySensors.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Initialize variables
static const uint32_t UPDATE_INTERVAL = 600000;
float lastHum;
float lastTemp;
float lastPrs;

// Initialize device
Adafruit_BME280 bme;

// Initialize MyMessage
MyMessage batteryPercentMsg(CHILD_ID_BATTERY, V_TEXT);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgPrs(CHILD_ID_PRS, V_PRESSURE);

// Custom funcions
float readVcc() {
  //Local values
  long result;
  
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  
  // Wait for Vref to settle
  delay(2); 

  // Convert
  ADCSRA |= _BV(ADSC); 
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  
  // Back-calculate AVcc in mV
  result = 1126400L / result;

  // Return Vcc voltage
  return (float)result / 1000.0;
}

void sendBatteryStatus(){
    // Local values
    char Output[40];
    char cTmp[5];
    String strOut;

    // Send notification to gateway
    strOut = "Getting the battery %";
    strOut.toCharArray(Output,40);
    send(batteryPercentMsg.set(Output));
    
    // Get the battery stats
    int sensorValue = analogRead(BATTERY_SENSE_PIN);
    
    // Wait to get real value
    delay(20);
    
    // Duplicate voltage divider value 
    float voltage = (sensorValue * (readVcc() / 1023.0)) * 2.0;

    // Offset del adc
    //voltage += 0.19; 

    // Calculate percentage and contrain result
    int batteryPcnt = (int)(100.0 * (voltage - 3.2)); 
    batteryPcnt = constrain(batteryPcnt, 0, 100);
    
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

    // Data processing
    dtostrf(voltage,4,2,cTmp);
    
    strOut = ((String)cTmp) + "V / " + ((String)batteryPcnt) + "%";
    strOut.toCharArray(Output,13);

    // Send info to the gateway
    send(batteryPercentMsg.set(Output));
    sendBatteryLevel(batteryPcnt);


    // Update leds info
    if (batteryPcnt >= 85){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDMID_PIN, HIGH);
      digitalWrite(LEDHIGH_PIN, HIGH);
      sleep(10000);
     }else if(batteryPcnt >= 70 && batteryPcnt < 85){
      digitalWrite(LEDMID_PIN, HIGH);
      digitalWrite(LEDLOW_PIN, HIGH);
      int count = 10;
      while (count > 0) {
        digitalWrite(LEDHIGH_PIN, !digitalRead(LEDHIGH_PIN));
        count--; 
        delay(1000);
      }
     }else if(batteryPcnt >= 55 && batteryPcnt < 70){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDMID_PIN, HIGH);
      digitalWrite(LEDHIGH_PIN, LOW);
      sleep(10000);
     }else if(batteryPcnt >= 40 && batteryPcnt < 55){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDHIGH_PIN, LOW);
      int count = 10;
      while (count > 0) {
        digitalWrite(LEDMID_PIN, !digitalRead(LEDMID_PIN));
        count--; 
        delay(1000);
      }
     }else if(batteryPcnt >= 10 && batteryPcnt < 25){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDMID_PIN, LOW);
      digitalWrite(LEDHIGH_PIN, LOW);
      sleep(10000);
     }else if(batteryPcnt < 10){
      digitalWrite(LEDMID_PIN, LOW);
      digitalWrite(LEDHIGH_PIN, LOW);
      int count = 10;
      while (count > 0) {
        digitalWrite(LEDLOW_PIN, !digitalRead(LEDLOW_PIN));
        count--; 
        delay(1000);
      }
    }

    // Turning off LEDs
    digitalWrite(LEDLOW_PIN, LOW);
    digitalWrite(LEDMID_PIN, LOW);
    digitalWrite(LEDHIGH_PIN, LOW);
}

void presentation()  
{ 
  // Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_BATTERY, S_INFO, "WS_BATTERY");
  present(CHILD_ID_HUM, S_HUM , "WS_HUM");
  present(CHILD_ID_TEMP, S_TEMP , "WS_TEMP");
  present(CHILD_ID_PRS, S_BARO , "WS_BARO");
}


void setup()
{ 
  //Setting up pins
  pinMode(LEDLOW_PIN, OUTPUT);
  pinMode(LEDMID_PIN, OUTPUT);
  pinMode(LEDHIGH_PIN, OUTPUT);

  digitalWrite(LEDLOW_PIN, LOW);
  digitalWrite(LEDMID_PIN, LOW);
  digitalWrite(LEDHIGH_PIN, LOW);
  
  if (!bme.begin(0x76)) {
#ifdef MY_DEBUG  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
#endif
    while (1);
  }
}

void loop()      
{  
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
    
    // Send battery info to gateway
    sendBatteryStatus();

    // Sleep for a while to save energy
    sleep(UPDATE_INTERVAL);
  
}
