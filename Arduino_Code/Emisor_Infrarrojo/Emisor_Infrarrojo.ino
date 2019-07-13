#include <IRremote.h>
IRsend irsend;

void setup()
{
}

void loop() {
  irsend.sendSAMSUNG(0xE0E040BF, 32);
  delay(10000);
}
