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
#define CHILD_ID_DIMMER 3
#define CHILD_ID_MODE_DIMMER 4
#define VOLTAGE_CHILD_ID 54
#define N_MODULE "P3_LEDDimmer"
#define V_MODULE "1.0"
#define GATE_PIN 6
#define PTM_PIN A0
#define BTN_PIN 3
#define LED_PIN 5
#define FADE_DELAY 10

#define MY_RF24_CE_PIN 49
#define MY_RF24_CS_PIN 53

//Include libraries
#include <MySensors.h>

//Initialize device and variables
static int16_t currentLevel = 0;
volatile bool manual = false;
static bool state = false;

int BATTERY_SENSE_PIN = A3;  // select the input pin for the battery sense point

uint32_t SLEEP_TIME = 10000;  // sleep time between reads (seconds * 1000 milliseconds)

MyMessage dimmerMsg(CHILD_ID_DIMMER, V_PERCENTAGE);
MyMessage modeDimmerMsg(CHILD_ID_MODE_DIMMER, V_STATUS);
MyMessage voltageMsg(VOLTAGE_CHILD_ID, V_VOLTAGE);


void setup()
{
  //Get the last status from web
  request( CHILD_ID_MODE_DIMMER, V_STATUS);
  //request( CHILD_ID_DIMMER, V_STATUS );
  //request( CHILD_ID_DIMMER, V_PERCENTAGE );

  //Setting up pins
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), blink, RISING);
}

void presentation()
{
  //Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);
  present( CHILD_ID_DIMMER, S_DIMMER );
  present( CHILD_ID_MODE_DIMMER, S_BINARY);
  present( VOLTAGE_CHILD_ID, S_MULTIMETER, "Battery level" );
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

void loop()
{
  // get the battery Voltage
    int sensorValue = analogRead(BATTERY_SENSE_PIN);
    float voltage = sensorValue * (5.0 / 1023.0);

#ifdef MY_DEBUG
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);
    Serial.print("Battery Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
#endif
    send(voltageMsg.set(voltage,2));  //send battery in Volt 2 decimal places
    sleep(SLEEP_TIME);
    
  if (manual){
    int valorpote = analogRead(PTM_PIN);
    int pwm1 = map(valorpote, 0, 1023, 255, 0);//invert positions to go from left to right (- to +)
    pwm1 = pwm1 > 250 ? 250 : pwm1;//Optimice the result to have better performances
    pwm1 = pwm1 < 5   ? 0   : pwm1;
    
    analogWrite(GATE_PIN,pwm1);
    digitalWrite(LED_PIN, HIGH);
    
  }else{
    digitalWrite(LED_PIN, LOW);
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
