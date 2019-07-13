/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 19.12.2017 16:16:38
 Project     :
 Libraries   :
 Author      : Francisco Moreno Tejeda
 Description :
******************************************************************/

#define MY_NODE_ID 1

// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24

#include <MySensors.h>
#include <Bounce2.h>

#define RELAY_PIN  6  // Pin Relay
#define CHILD_ID_RELAY 0   // Id of the sensor child

#define RELAY_ON 1
#define RELAY_OFF 0

bool state;

MyMessage msgRELAY(CHILD_ID_RELAY,V_STATUS);

void setup()  
{  
  pinMode(RELAY_PIN, OUTPUT); 

  
  //Cargamos el estado anterior 
  state = loadState(CHILD_ID_RELAY);
  
  Serial.print("Estado cargado: ");
  Serial.println(state);
  
  digitalWrite(RELAY_PIN, state?RELAY_ON:RELAY_OFF);
}

void presentation()  {

  sendSketchInfo("Rele_PCB_LDH", "1.0");

  // Registro de todos los dispositivos usados
  present(CHILD_ID_RELAY, S_BINARY);
}


void receive(const MyMessage &message) {
  
  if (message.isAck()) {
     Serial.println("This is an ack from gateway");
  }
  
  if (message.type == V_STATUS){
  
    state = message.getBool();
    
    Serial.print("Nuevo estado recibido y almacenado: ");
    Serial.println(state);
    
    // Acualizamos el valor del rele
    digitalWrite(RELAY_PIN, state?RELAY_ON:RELAY_OFF);
    saveState(CHILD_ID_RELAY, state);
  }
  
}
