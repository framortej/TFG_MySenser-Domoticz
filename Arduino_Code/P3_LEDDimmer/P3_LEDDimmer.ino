/*****************************************************************
 Project        : P3 LEDDimmer
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : Management circuit of a 5V LED strip
 InitialRelease : 19.07.2019 
 LastUpdate     : 03.09.2019 
*****************************************************************/
// Enable debug prints
//#define MY_DEBUG

// Define the node ID
#define MY_NODE_ID 30

// Define configuration
#define MY_RADIO_RF24
#define CHILD_ID_BATTERY 31
#define CHILD_ID_DIMMER 32
#define CHILD_ID_MODE_DIMMER 33
#define CHILD_ID_BATTERY_MODE 34
#define FADE_DELAY 10

// Define module INFO
#define N_MODULE "P3_LEDDimmer"
#define V_MODULE "1.2"

// Define ports
#define PTM_PIN A0
#define BATTERY_SENSE_PIN A1
#define BTN_PIN 3
#define LEDLOW_PIN 4
#define LEDMODE_PIN 5
#define GATE_PIN 6
#define LEDMID_PIN 7
#define LEDHIGH_PIN 8

// Include libraries
#include <MySensors.h>

// Initialize variables
static int16_t currentLevel = 0;
volatile bool manual = false;
static bool state = false;
int16_t batteryCount = 0;
int16_t oldBatteryPcnt = 0;

// Initialize Message Object
MyMessage dimmerMsg(CHILD_ID_DIMMER, V_PERCENTAGE);
MyMessage modeDimmerMsg(CHILD_ID_MODE_DIMMER, V_STATUS);
MyMessage batteryPercentMsg(CHILD_ID_BATTERY, V_TEXT);
MyMessage getBatteryInfo(CHILD_ID_BATTERY_MODE, V_STATUS);


void setup()
{
  // Get the last status from GW
  request( CHILD_ID_MODE_DIMMER, V_STATUS);
  request( CHILD_ID_DIMMER, V_PERCENTAGE );
  request( CHILD_ID_DIMMER, V_STATUS );

  //Setting up pins
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LEDLOW_PIN, OUTPUT);
  pinMode(LEDMODE_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);
  pinMode(LEDMID_PIN, OUTPUT);
  pinMode(LEDHIGH_PIN, OUTPUT);
  
  digitalWrite(LEDLOW_PIN, LOW);
  digitalWrite(LEDMODE_PIN, HIGH);
  digitalWrite(GATE_PIN, LOW);
  digitalWrite(LEDMID_PIN, LOW);
  digitalWrite(LEDHIGH_PIN, LOW);

  //wait 3 seconds to gw messages
  wait(1000);
  digitalWrite(LEDMODE_PIN, LOW);
  wait(1000);
  digitalWrite(LEDMODE_PIN, HIGH);
  wait(1000);
  digitalWrite(LEDMODE_PIN, LOW);
  
  //Active interrupt port
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), blink, RISING);
}

void presentation()
{
  //Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);

  // Register all sensors to gw (they will be created as child devices)
  present( CHILD_ID_DIMMER, S_DIMMER , "DIMMER");
  present( CHILD_ID_MODE_DIMMER, S_BINARY, "DIM_MODE");
  present( CHILD_ID_BATTERY, S_INFO, "DIM_BATTERY" );
  present( CHILD_ID_BATTERY_MODE, S_BINARY, "DIM_BATTERY_MODE" );
}

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
    int batteryPcnt = (int)(100.0 * (voltage - 6.4)); 
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
      sleep(3000);
     }else if(batteryPcnt >= 70 && batteryPcnt < 85){
      digitalWrite(LEDMID_PIN, HIGH);
      digitalWrite(LEDLOW_PIN, HIGH);
      int count = 3;
      while (count > 0) {
        digitalWrite(LEDHIGH_PIN, !digitalRead(LEDHIGH_PIN));
        count--; 
        delay(1000);
      }
     }else if(batteryPcnt >= 55 && batteryPcnt < 70){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDMID_PIN, HIGH);
      digitalWrite(LEDHIGH_PIN, LOW);
      sleep(3000);
     }else if(batteryPcnt >= 40 && batteryPcnt < 55){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDHIGH_PIN, LOW);
      int count = 3;
      while (count > 0) {
        digitalWrite(LEDMID_PIN, !digitalRead(LEDMID_PIN));
        count--; 
        delay(1000);
      }
     }else if(batteryPcnt >= 10 && batteryPcnt < 25){
      digitalWrite(LEDLOW_PIN, HIGH);
      digitalWrite(LEDMID_PIN, LOW);
      digitalWrite(LEDHIGH_PIN, LOW);
      sleep(3000);
     }else if(batteryPcnt < 10){
      digitalWrite(LEDMID_PIN, LOW);
      digitalWrite(LEDHIGH_PIN, LOW);
      int count = 3;
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

void fadeToLevel( int toLevel )
{
  //Simple algorithm that scales the increase or decrease in light intensity
  int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;
  
  while ( currentLevel != toLevel ) {
      currentLevel += delta;
      analogWrite( GATE_PIN, (int)(currentLevel / 100. * 255) );
      delay( FADE_DELAY );
  }
}

void blink() {
  // Alternate state
  manual = !manual;
#ifdef MY_DEBUG
  Serial.println( "Sending the \"manual\" status to the state..." );
  Serial.print( "Manual = " );
  Serial.println(manual);
#endif
  // Send information of devices to GW
  send(modeDimmerMsg.set(manual));

  if (!manual){
    send(dimmerMsg.set(currentLevel));
  }
}

void loop()
{
  //Check mode
  if (manual){
    // Read data
    int valorpote = analogRead(PTM_PIN);

    // Map for new interval
    int pwm1 = map(valorpote, 0, 1023, 0, 255);

    // Optimice the result to have better performances 
    pwm1 = pwm1 > 250 ? 250 : pwm1;
    pwm1 = pwm1 < 5   ? 0   : pwm1;

#ifdef MY_DEBUG
    Serial.print( "pwm1: ");
    Serial.println( pwm1);
#endif

    // Update status
    analogWrite(GATE_PIN,pwm1);
    digitalWrite(LEDMODE_PIN, HIGH);
    currentLevel = map(pwm1, 0, 250, 0, 100);
    
  }else{
    digitalWrite(LEDMODE_PIN, LOW);
  }
}

void receive(const MyMessage &message)
{
#ifdef MY_DEBUG
  Serial.print( "SensorID: ");
  Serial.print( message.sensor);
  Serial.print( " Type: ");
  Serial.println( message.type);
#endif

  // Send battery info
  if (message.sensor ==  CHILD_ID_BATTERY_MODE){
    sendBatteryStatus();
  }

  // Update info from GW
  if (!manual && message.sensor ==  CHILD_ID_DIMMER){
    if (message.type == V_STATUS){
      state = message.getBool();
#ifdef MY_DEBUG     
      Serial.print( "New state: " );
      Serial.println( state );
#endif    
      if (state){
        request( CHILD_ID_DIMMER, V_PERCENTAGE );
      }else {
#ifdef MY_DEBUG     
        Serial.println( "Turning OFF dimmer.. " );
#endif          
        fadeToLevel( atoi( message.data ) );
      }
    }
    
    if (message.type == V_PERCENTAGE) {

      int requestedLevel = atoi( message.data );
      // Optimice the result to have % of power
      requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
      requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
      
#ifdef MY_DEBUG     
      Serial.print( "Changing level to " );
      Serial.print( requestedLevel );
      Serial.print( ", from " );
      Serial.println( currentLevel );
#endif      
      fadeToLevel( requestedLevel );
      
    }
  }
  
  if (message.type == V_STATUS && message.sensor ==  CHILD_ID_MODE_DIMMER ) {
    
    manual = message.getBool();
#ifdef MY_DEBUG 
    Serial.print( "Mode [1:MANUAL // 0:ONLINE]: " );
    Serial.println( manual );
#endif 
    if (!manual){
      send(dimmerMsg.set(currentLevel));
    }
  }
}
