/*****************************************************************
 Project        : P99 MySensors Tutorial
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : Simple example of sketch using MySensors librarie
 InitialRelease : 24.08.2019 
 LastUpdate     : 25.08.2019 
*****************************************************************/
// Enable debug prints
#define MY_DEBUG

// Define the node ID
#define MY_NODE_ID 10

// Enable and select radio type attached 
#define MY_RADIO_RF24

// Board Info
#define N_MODULE "P99_TutoMySensors"
#define V_MODULE "1.49"

// Child ID for gateway
#define CHILD_ID_LED 11

// Include libraries
#include <MySensors.h>

/*We initialize the MyMessage variable in charge of sending and 
 * receiving the data between the board and the gateway. We 
 * will indicate the ID of the device and that it is a device 
 * that alternates between 2 states.
 */
MyMessage msgLED(CHILD_ID_LED,V_STATUS);

void presentation()  
{ 
  // Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_LED, S_BINARY);
}

void setup()
{ 
  // Setting up pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

// This function will be executed each time we receive a message.
void receive(const MyMessage &message) 
{
  /*We checked that he's the kind we use. If there is more than 
   * one device that uses this type of messages, we must also 
   * filter by the id of the device it belongs to through the 
   * use of the property message.sensor. 
   * For example: if (message.sensor == CHILD_ID_LED)...
   */
  if (message.type == V_STATUS){

    // Get the value of the payload
    bool estado = message.getBool();
    
    // Assign the new state
    digitalWrite(LED_BUILTIN, estado?1:0);

    // Send state to gateway
    send(msgLED.set(digitalRead(LED_BUILTIN)));
  }
}
