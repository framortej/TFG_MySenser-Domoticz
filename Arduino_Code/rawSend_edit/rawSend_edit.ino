/* rawSend.ino Example sketch for IRLib2
    Illustrates how to send a code Using raw timings which were captured
    from the "rawRecv.ino" sample sketch.  Load that sketch and
    capture the values. They will print in the serial monitor. Then you
    cut and paste that output into the appropriate section below.
*/
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender

IRsendRaw mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Every time you press a key is a serial monitor we will send."));
}
/* Cut and paste the output from "rawRecv.ino" below here. It will
   consist of a #define RAW_DATA_LEN statement and an array definition
   beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
   with "…,1000};"
*/
#define RAW_DATA_LEN 100
uint16_t rawData[RAW_DATA_LEN] = {
  8998, 4498, 598, 1690, 574, 1690, 574, 558,
  598, 578, 598, 558, 594, 554, 598, 554,
  602, 1714, 574, 554, 598, 1690, 578, 1686,
  578, 602, 574, 554, 602, 550, 602, 554,
  598, 578, 598, 1690, 578, 550, 602, 1686,
  578, 578, 598, 554, 602, 554, 598, 550,
  602, 578, 598, 554, 598, 554, 598, 582,
  574, 578, 598, 1690, 578, 550, 602, 550,
  602, 1714, 574, 582, 570, 554, 602, 550,
  602, 578, 598, 554, 598, 578, 574, 554,
  598, 578, 602, 554, 598, 554, 598, 554,
  598, 582, 598, 550, 602, 578, 574, 578,
  574, 570, 574, 1000
};






/*
   Cut-and-paste into the area above.
*/

void loop() {

  mySender.send(rawData, RAW_DATA_LEN, 4); //Pass the buffer,length, optionally frequency
  delay(5000);
  

}
