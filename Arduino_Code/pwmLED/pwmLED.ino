#define LED_PIN 6      // Arduino pin attached to MOSFET Gate pin
#define FADE_DELAY 10  // Delay in ms for each percentage fade up/down (10ms = 1s full-range dim)
#define BOTON_PIN 2      // Arduino pin attached to MOSFET Gate pin

static int16_t currentLevel = 0; 
static int16_t aumento = 0;
static int16_t requestedLevel = 0;
volatile byte state = LOW;

void setup()
{
    Serial.begin(9600);
    pinMode(BOTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BOTON_PIN), blink, CHANGE);
}

void fadeToLevel( int toLevel )
{
    int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;

    while ( currentLevel != toLevel ) {
        currentLevel += delta;
        analogWrite( LED_PIN, (int)(currentLevel / 100. * 255) );
        Serial.println((int)(currentLevel / 100. * 255));
        delay( FADE_DELAY );
    }
}

void blink() {
 state = !state;
}


void loop()
{
  if (state == LOW){
    requestedLevel = aumento;
      
    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
    
    Serial.print( "Changing level to " );
    Serial.print( requestedLevel );
    Serial.print( ", from " );
    Serial.println( currentLevel );
    
    fadeToLevel( requestedLevel );
    aumento += 25;
    state = HIGH;
  }
}
