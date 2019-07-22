#define GATE_PIN 6
#define PTM_PIN A0
#define BTN_PIN 3
#define LED_PIN 5
#define FADE_DELAY 50

//Initialize device and variables
volatile byte state = LOW;

void setup()
{
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), blink, RISING);
}

void blink() {
 state = !state;
}

void loop()
{
  if (state == HIGH){
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
