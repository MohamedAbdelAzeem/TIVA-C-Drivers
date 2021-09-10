

#include "can.h"

int main()
{
  /* Configure CAN0 : loopback mode, bitrate 1Kbps */
  CANCongfig_t CAN0 = {CAN_0, LOOPBACK_MODE, 1000};
  CAN_Init(&CAN0);
  
  /* Configure CAN1 : loopback mode, bitrate 1Kbps */
  CANCongfig_t CAN1 = {CAN_1, LOOPBACK_MODE, 1000};
  CAN_Init(&CAN1);
  
   /* Transmit data1 */
  uint8 data1[] = "Mohamed1"; /* 8-byte data  */
  /* Configure message 1 : 
  			MsgID = 0x01, 
  			Message number = 0x01;
 		        mask = 0x00, 
  			11_bit id,
 		        8-byte datal length, */
  CANMsgObject_t message1 = {0x01, 0x01, 0x00, STANDARD_ID,8};
  
  /* Configure message object for CAN_0, 0x01 object number, */
  CAN_Transmit_SetObject(CAN_0, &message1);
  
  /* trasnmit message number 1 with data message1*/
  CAN_Transmit_Object(CAN_0 , 0x01, data1);
  
  
  /* Receive data2 */
  volatile uint8* data2 = NULL_PTR;
  /* Any received message with the following masked ID will be received and stored in message number 2 */
  CANMsgObject_t message2 = {0x01, 0x02, 0x00, STANDARD_ID,8};
  CAN_Transmit_SetObject(CAN_0, &message2);
  
  /* receive messsage number 2 */
  data2 =  CAN_Receive_Object(CAN_0, 0x02);
  

  return 0;
}
