/******************************************************************
 Project     : P3 LEDDimmer
 Libraries   : MySensors
 Author      : Francisco Moreno Tejeda
 Date        : 02.07.2019 16:16:38
 Description : 
******************************************************************/
//Enable debug
#define MY_DEBUG

//Define configuration
#define MY_RADIO_RF24
#define CHILD_ID_DIMMER 3
#define CHILD_ID_MODE_DIMMER 4 
#define N_MODULE "P3_LEDDimmer"
#define V_MODULE "1.0"
#define GATE_PIN 6
#define PTM_PIN A0
#define BTN_PIN 3
#define LED_PIN 5
#define FADE_DELAY 10

//Include libraries
#include <MySensors.h>

//Initialize device and variables
static int16_t currentLevel = 0;
volatile bool manual = false;

MyMessage dimmerMsg(CHILD_ID_DIMMER, V_PERCENTAGE);
MyMessage modeDimmerMsg(CHILD_ID_MODE_DIMMER, V_STATUS);


void setup()
{
  request( CHILD_ID_MODE_DIMMER, V_STATUS);
  request( CHILD_ID_DIMMER, V_PERCENTAGE );
  
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), blink, RISING);
}

void presentation()
{
  sendSketchInfo(N_MODULE, V_MODULE);
  present( CHILD_ID_DIMMER, S_DIMMER );
  present( CHILD_ID_MODE_DIMMER, S_BINARY);
}

void updateState(){
  if (!manual){
    Serial.println( "Obteniendo valores del dimmer de la web" );
    currentLevel = 0;
    request( CHILD_ID_DIMMER, V_LIGHT );
    request( CHILD_ID_DIMMER, V_PERCENTAGE );
 }
}

void blink() {
 manual = !manual;
 send(modeDimmerMsg.set(manual));
 updateState();
}

void fadeToLevel( int toLevel )
{
    int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;

    while ( currentLevel != toLevel ) {
        currentLevel += delta;
        analogWrite( GATE_PIN, (int)(currentLevel / 100. * 255) );
        delay( FADE_DELAY );
    }
}

void loop()
{
  if (manual == true){
    int valorpote=analogRead(PTM_PIN);
    int pwm1 = map(valorpote, 0, 1023, 255, 0);//invertimos para ir de izq a derecha (- a +)
    pwm1 = pwm1 > 250 ? 250 : pwm1;//Optimizamos el resultado para eliminar ruidos
    pwm1 = pwm1 < 5   ? 0   : pwm1;
    
    analogWrite(GATE_PIN,pwm1);
    digitalWrite(LED_PIN, HIGH);
    
  }else{
    digitalWrite(LED_PIN, LOW);
  }
}



void receive(const MyMessage &message)
{
  Serial.print( "SensorID: ");
  Serial.print( message.sensor);
  Serial.print( " Type: ");
  Serial.println( message.type);
  
  if (manual == false && message.sensor ==  CHILD_ID_DIMMER){
    if (message.type == V_LIGHT || message.type == V_PERCENTAGE) {

      //  Retrieve the power or dim level from the incoming request message
      int requestedLevel = atoi( message.data );
      
      // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
      requestedLevel *= ( message.type == V_LIGHT ? 100 : 1 );
      
      // Clip incoming level to valid range of 0 to 100
      requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
      requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
      
      Serial.print( "Changing level to " );
      Serial.print( requestedLevel );
      Serial.print( ", from " );
      Serial.println( currentLevel );
      
      fadeToLevel( requestedLevel );
      
      //send(lightMsg.set(currentLevel > 0));
      send( dimmerMsg.set(currentLevel) );
    }
  }
  if (message.type == V_STATUS && message.sensor ==  CHILD_ID_MODE_DIMMER ) {
    manual = message.getBool();
    Serial.print( "Nuevo estado manual ?: " );
    Serial.println( manual );
    updateState();
  }
}
