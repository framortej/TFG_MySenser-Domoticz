#define MY_DEBUG
#define MY_RADIO_RF24

#define MY_NODE_ID      2
#define CHILD_ID  1

#include <SPI.h>
#include <MySensors.h>
#include <IRremote.h>

IRsend irsend;

MyMessage msgIrReceive(CHILD_ID, V_TEXT);

void presentation ()
{
  sendSketchInfo("IR sender", "1.0");

  present(CHILD_ID, S_INFO);
}

void receive(const MyMessage &message) {

  if (message.type == V_TEXT) {

    unsigned long hex = strtoul(message.getString(), 0, 16);
    Serial.println(hex);
    irsend.sendSAMSUNG(hex, 32);
  }
}
