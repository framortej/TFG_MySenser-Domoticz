// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24

#define CHILD_ID_BATTERY 18

#define BATTERY_SENSE_PIN A3 // select the input pin for the battery sense point

#define MY_RF24_CE_PIN 49
#define MY_RF24_CS_PIN 53

#include <MySensors.h>

uint32_t SLEEP_TIME = 10000;  // sleep time between reads (seconds * 1000 milliseconds)
int oldBatteryPcnt = 0;

MyMessage batteryPercentMsg(CHILD_ID_BATTERY, V_TEXT);

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


void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Battery Meter", "1.3");
    present( CHILD_ID_BATTERY, S_INFO );
}

void loop()
{
    // get the battery stats
    int sensorValue = analogRead(BATTERY_SENSE_PIN);
    float voltage = (sensorValue * (5.0 / 1023.0)) * 2.0; //duplicate voltage divider value
    int batteryPcnt = (int)(((voltage - 6.4) * 100) / 2.0);
    //int batteryPcnt = (int)((sensorValue * 100.0) / 1023.0);

#ifdef MY_DEBUG
    Serial.print("Battery Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
    Serial.print("Battery percent: ");
    Serial.print(batteryPcnt);
    Serial.println(" %");
#endif

    if (oldBatteryPcnt != batteryPcnt) {
        // Power up radio after sleep
        char Output[12];
        char cTmp[5];
        String strOut;

        dtostrf(voltage,4,2,cTmp);
        
        strOut = ((String)cTmp) + "V / " + ((String)batteryPcnt) + "%";
        
        strOut.toCharArray(Output,12);
        
        send(batteryPercentMsg.set(Output));
        sendBatteryLevel(batteryPcnt);
        
        oldBatteryPcnt = batteryPcnt;
    }
    sleep(SLEEP_TIME);
}
