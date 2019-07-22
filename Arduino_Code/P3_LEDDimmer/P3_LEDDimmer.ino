/******************************************************************
 Project     : P3 LEDDimmer
 Libraries   : MySensors
 Author      : Francisco Moreno Tejeda
 Date        : 02.07.2019 16:16:38
 Description : 
******************************************************************/
//Include libraries
#include <MySensors.h>

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
#define FADE_DELAY 50

//Initialize device and variables
static int16_t currentLevel = 0;
static bool mode = false

MyMessage dimmerMsg(CHILD_ID_DIMMER, V_DIMMER);
MyMessage lightMsg(CHILD_ID_DIMMER, V_LIGHT);
MyMessage modeDimmerMsg(CHILD_ID_MODE_DIMMER, V_STATUS);


void setup()
{
    request( CHILD_ID_RELAY, V_DIMMER );
    pinMode(BOTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW)
    attachInterrupt(digitalPinToInterrupt(BOTON_PIN), blink, CHANGE);
}

void presentation()
{
    sendSketchInfo(N_MODULE, V_MODULE);
    present( CHILD_ID_RELAY, S_DIMMER );
}

void blink() {
 state = !state;
}

void loop()
{
  if (mode == true){
    int valorpote=analogRead(PTM_PIN);
    int pwm1 = map(valorpote, 0, 1023, 0, 255);
    analogWrite(GATE_PIN,pwm1);
    digitalWrite(LED_PIN, HIGH)
  }else{
    digitalWrite(LED_PIN, LOW)
  }
}



void receive(const MyMessage &message)
{
    if (message.type == V_LIGHT || message.type == V_DIMMER) {

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

        // Inform the gateway of the current DimmableLED's SwitchPower1 and LoadLevelStatus value...
        send(lightMsg.set(currentLevel > 0));

        // hek comment: Is this really nessesary?
        send( dimmerMsg.set(currentLevel) );


    }
}

/***
 *  This method provides a graceful fade up/down effect
 */
void fadeToLevel( int toLevel )
{

    int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;

    while ( currentLevel != toLevel ) {
        currentLevel += delta;
        analogWrite( LED_PIN, (int)(currentLevel / 100. * 255) );
        delay( FADE_DELAY );
    }
}
