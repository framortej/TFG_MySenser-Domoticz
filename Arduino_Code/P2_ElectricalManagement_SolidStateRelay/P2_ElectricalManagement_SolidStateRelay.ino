/*****************************************************************
 Project        : P2 ElectricalManagement Solid State Relay
 Libraries      : MySensors
 Author         : Francisco Moreno Tejeda
 Description    : Electrical management circuit for lamps
 InitialRelease : 02.06.2019 
 LastUpdate     : 30.08.2019 
*****************************************************************/
// Enable debug prints to serial monitor
//#define MY_DEBUG 

//Define the node ID
#define MY_NODE_ID 20

// Enable and select radio type attached
#define MY_RADIO_RF24

// Board Info
#define N_MODULE "P2_SolidStateRelay"
#define V_MODULE "1.2"

// Include libraries
#include <MySensors.h>

// Child ID for gateway
#define CHILD_ID_RELAY 21

// Define ports
#define RELAY_PIN  6

// Define states
#define RELAY_ON 1
#define RELAY_OFF 0

// Create global variables
bool state;

// Initialize MyMessage
MyMessage msgRELAY(CHILD_ID_RELAY,V_STATUS);

void setup()  
{  
  // Setting up pins
  pinMode(RELAY_PIN, OUTPUT); 

  // Get online state from gateway
  request(CHILD_ID_RELAY, V_STATUS);

  //wait 3 seconds to gw message, them charge local state
  wait(3000);
  
  // Get local state
  state = loadState(CHILD_ID_RELAY);

  // Change to real state
  digitalWrite(RELAY_PIN, state?RELAY_ON:RELAY_OFF);
}

void presentation()  {

  // Identification of the modules and the information of this device
  sendSketchInfo(N_MODULE, V_MODULE);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_RELAY, S_BINARY, "Garage light");
}


void receive(const MyMessage &message) {
  
  if (message.type == V_STATUS){
    // Get the value of the payload
    state = message.getBool();
    
#ifdef MY_DEBUG      
    Serial.print("New state receive: ");
    Serial.println(state);
#endif
    
    // Assign the new state and save in local memory
    digitalWrite(RELAY_PIN, state?RELAY_ON:RELAY_OFF);
    saveState(CHILD_ID_RELAY, state);
  }
  
}
