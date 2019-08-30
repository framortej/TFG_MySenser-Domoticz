/******************************************************************
 Project        : P3 LEDDimmer
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : 
 InitialRelease : 02.07.2019 
 LastUpdate     : 23.07.2019 
******************************************************************/
//Enable debug
#define MY_DEBUG

//Define configuration
#define MY_RADIO_RF24
#define CHILD_ID_BATTERY 30
#define CHILD_ID_DIMMER 31
#define CHILD_ID_MODE_DIMMER 32 
#define FADE_DELAY 10

//Define module INFO
#define N_MODULE "P3_LEDDimmer"
#define V_MODULE "1.1"

//Define ports
#define PTM_PIN A0
#define BATTERY_SENSE_PIN A1
#define BTN_PIN 3
#define LEDLOW_PIN 4
#define LEDMODE_PIN 5
#define GATE_PIN 6
#define LEDMID_PIN 7
#define LEDHIGH_PIN 8

//Include libraries
#include <MySensors.h>

//Initialize variables
static int16_t currentLevel = 0;
volatile bool manual = false;
static bool state = false;
int16_t batteryCount = 0;
int16_t oldBatteryPcnt = 0;

//Initialize Message Object
MyMessage dimmerMsg(CHILD_ID_DIMMER, V_PERCENTAGE);
MyMessage modeDimmerMsg(CHILD_ID_MODE_DIMMER, V_STATUS);
MyMessage batteryPercentMsg(CHILD_ID_BATTERY, V_TEXT);


void setup()
{
  //Get the last status from web
  request( CHILD_ID_MODE_DIMMER, V_STATUS);
  //request( CHILD_ID_DIMMER, V_STATUS );
  //request( CHILD_ID_DIMMER, V_PERCENTAGE );

  //Setting up pins
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LEDLOW_PIN, OUTPUT);
  pinMode(LEDMODE_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);
  pinMode(LEDMID_PIN, OUTPUT);
  pinMode(LEDHIGH_PIN, OUTPUT);
  
  digitalWrite(LEDLOW_PIN, HIGH);
  digitalWrite(LEDMODE_PIN, LOW);
  digitalWrite(GATE_PIN, LOW);
  digitalWrite(LEDMID_PIN, HIGH);
  digitalWrite(LEDHIGH_PIN, HIGH);

  //Active interrupt port
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), blink, RISING);
}

void presentation()
{
  //Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);
  present( CHILD_ID_DIMMER, S_DIMMER , "DIMMER");
  present( CHILD_ID_MODE_DIMMER, S_BINARY, "DIM_MODE");
  present( CHILD_ID_BATTERY, S_INFO, "DIM_BATTERY" );
}

void updateState(){
  if (!manual){
    currentLevel = 0;
#ifdef MY_DEBUG
    Serial.println( "Getting the dimmer status from the web..." );
#endif
    request( CHILD_ID_DIMMER, V_STATUS );
 }
}

void blink() {
  manual = !manual;
#ifdef MY_DEBUG
  Serial.println( "Sending the \"manual\" status to the state..." );
  Serial.print( "Manual = " );
  Serial.println(manual);
#endif
  send(modeDimmerMsg.set(manual));
  updateState();
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
  // Update batteryCount
  batteryCount += 1;

  if (batteryCount == 20000){
    batteryCount = 0;
    // get the battery stats
    int sensorValue = analogRead(BATTERY_SENSE_PIN);
    float voltage = (sensorValue * (readVcc() / 1023.0)) * 2.0; //duplicate voltage divider value
    int batteryPcnt = (int)(((voltage - 6.4) * 100) / 2.0);

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
      digitalWrite(LEDLOW_PIN, !digitalRead(LEDLOW_PIN));
      digitalWrite(LEDMID_PIN, LOW);
      digitalWrite(LEDHIGH_PIN, LOW);      
    }

#ifdef MY_DEBUG
    Serial.print("Battery Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
    Serial.print("Battery percent: ");
    Serial.print(batteryPcnt);
    Serial.println(" %");
#endif

    if (oldBatteryPcnt != batteryPcnt) {
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
  }

  //Check mode
  if (manual){
    int valorpote = analogRead(PTM_PIN);
    int pwm1 = map(valorpote, 0, 1023, 255, 0);//invert positions to go from left to right (- to +)
    pwm1 = pwm1 > 250 ? 250 : pwm1;//Optimice the result to have better performances
    pwm1 = pwm1 < 5   ? 0   : pwm1;
    
    analogWrite(GATE_PIN,pwm1);
    digitalWrite(LEDMODE_PIN, HIGH);
    
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
  
  if (manual == false && message.sensor ==  CHILD_ID_DIMMER){

    //Web status/value of dimmer
    int requestedLevel = atoi( message.data );
    
    if (message.type == V_STATUS){
      state = message.getBool();
#ifdef MY_DEBUG     
      Serial.print( "New state: " );
      Serial.println( state );
#endif    
      if (state){
        request( CHILD_ID_DIMMER, V_PERCENTAGE );
      }else if (!state){
#ifdef MY_DEBUG     
        Serial.println( "Turning OFF dimmer.. " );
#endif          
        currentLevel = 50;
        fadeToLevel( requestedLevel );
      }
    }
    
    if (message.type == V_PERCENTAGE && state) {
      
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
      
      send( dimmerMsg.set(currentLevel) );
    }
  }
  if (message.type == V_STATUS && message.sensor ==  CHILD_ID_MODE_DIMMER ) {
    
    manual = message.getBool();
#ifdef MY_DEBUG 
    Serial.print( "Mode [1:MANUAL // 0:ONLINE]: " );
    Serial.println( manual );
#endif 
    updateState();
  }
}