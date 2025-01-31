#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(5); // CS pin is GPIO 5
//#define CAN_ACK_ID 0x037  // CAN ID for acknowledgment


void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  /*
 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();*/
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    //    if (canMsg.can_id == 0x036)  // Check if the message is from the sender

    if (canMsg.data[0] == 1) {
      Serial.println("PULSADO");
    }
    
    else {
      Serial.println("NO PULSADO");
    }
  }
}
