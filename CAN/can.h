#ifndef CAN_H
#define CAN_H

#include "Common_Macros.h"
#include "Std_Types.h"
#include "tm4c123gh6pm_registers.h"
#include <assert.h>


#define SYSTEM_CLOCK (16000000.0)
#define CAN_MAX_BIT_DIVISOR (25)   /* BIT_Divisor = n */
#define CAN_MIN_BIT_DIVISOR (4)
#define CAN_MAX_PRESCALER   (1023)  /* 10 BITS = 6 Bits in CANBIT + 4 bits in CANBIT */
#define CAN_MIN_PRESCALER   (1)

#define tseg1(n)   ((2 + ((n-3)/2) ) - 1)
#define tseg2(n)   (((n-3)/2) -1)
#define tsjw 	    (0)

//Enable CAN & PORT Clock Registers
#define RCGCCAN   (*((volatile uint32 *)0x400FE634))  //Enable UART n Clock with corresponding bit n in the Register
#define RCGCGPIO  (*((volatile uint32 *)0x400FE608))  //Enable PORT Clock

/*
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000
*/

/* Base Registers  */
#define CAN0_BASE_ADDRESS 		  0x40040000
#define CAN1_BASE_ADDRESS 		  0x40041000

/* Registers Offset */
#define CANCTL_OFFSET 			  0x000  /* CAN Control register */
#define CANSTS_OFFSET			  0x004  /* CAN Status register  */
#define CANBIT_OFFSET			  0x008  /* CAN Bit Timing register */
#define CANTST_OFFSET			  0x014  /* CAN TEST register */
#define CANINT_OFFSET 			  0x010  /* CAN Interrupt register  */
#define CANBRPE_OFFSET			  0x018  /* CAN Baud Rate Prescaler Extension register */
#define CANIF1CRQ_OFFSET		  0x020  /* CAN IF1 Command Request register  */
#define CANIF1CMSK_OFFSET 		  0x024  /* CAN IF1 Command Mask register  */
#define CANIF1MSK1_OFFSET 	          0x028  /* CAN IF1 Mask 1 register  */
#define CANIF1MSK2_OFFSET 		  0x02C  /* CAN IF1 Mask 2 register */
#define CANIF1ARB1_OFFSET 		  0x030  /* CAN IF1 Arbitration 1 register  */
#define CANIF1ARB2_OFFSET     	          0x034  /* CAN IF1 Arbitration 2 register */
#define CANIF1MCTL_OFFSET 		  0x038  /* CAN IF1 Message Control register */
#define CANIF1DA1_OFFSET 		  0x03C  /* CAN IF1 Data A1 register */
#define CANIF1DA2_OFFSET 		  0x040  /* CAN IF1 Data A2 register  */
#define CANIF1DB1_OFFSET      		  0x044  /* CAN IF1 Data B1 register  */
#define CANIF1DB2_OFFSET 		  0x048  /* CAN IF1 Data B2 register  */

#define CANIF2CRQ_OFFSET 		  0x080  /* CAN IF2 Command Request register */
#define CANIF2CMSK_OFFSET 		  0x084  /* CAN IF2 Command Mask register   */
#define CANIF2MSK1_OFFSET 		  0x088  /* CAN IF2 Mask 1 register */
#define CANIF2MSK2_OFFSET 		  0x08C  /* CAN IF2 Mask 2 register */
#define CANIF2ARB2_OFFSET 		  0x094  /* CAN IF2 Arbitration 2 register */
#define CANIF2ARB1_OFFSET 		  0x090  /* CAN IF2 Arbitration 1 register  */
#define CANIF2MCTL_OFFSET 	          0x098  /* CAN IF2 Message Control register */
#define CANIF2DA1_OFFSET 		  0x09C  /* CAN IF2 Data A1 register  */
#define CANIF2DA2_OFFSET                  0x0A0  /* CAN IF2 Data A2 register  */
#define CANIF2DB1_OFFSET 		  0x0A4  /* CAN IF2 Data B1 register  */
#define CANIF2DB2_OFFSET 		  0x0A8  /* CAN IF2 Data B2 register  */
#define CANTXRQ1_OFFSET 		  0x100  /* CAN Transmission Request 1 register   */
#define CANTXRQ2_OFFSET 		  0x104  /* CAN Transmission Request 2 register */
#define CANNWDA1_OFFSET 		  0x120  /* CAN New Data 1 register  */
#define CANNWDA2_OFFSET  		  0x124  /* CAN New Data 2 register  */
#define CANMSG1INT_OFFSET     		  0x140  /* CAN Message 1 Interrupt Pending  register*/
#define CANMSG2INT_OFFSET     		  0x144  /* CAN Message 2 Interrupt Pending  register*/
#define CANMSG1VAL_OFFSET 		  0x160  /* CAN Message 1 Valid register  */
#define CANMSG2VAL_OFFSET 		  0x164  /* CAN Message 2 Valid register */


#define CAN_0  		 (0U)
#define CAN_1 		 (1U)

/* Bits */
#define INIT_BIT	(0U)
#define TEST_BIT        (7U)
#define LBACK_BIT	(4U)
/* Configuration Change Enable bit in CANCTL register */
#define CCE_BIT		(6U)	
#define BUSY_BIT	(15U)
#define WRNRD_BIT	(7U)
#define ARB_BIT         (6U)
#define CONTROL_BIT     (4U)
#define DATAA_BIT	(1U)
#define DATAB_BIT       (0U)
#define DIR_BIT         (13U)
#define XTD_BIT		(14U)
#define MSGVAL_BIT	(15U)
#define TXRQST_BIT      (2U)
#define EOB_BIT		(7U)
#define UMASK_BIT 	(12U)
#define MASK_BIT	(6U)
#define NEWDAT_BIT	(15U)
#define MSGLST_BIT	(14U)


/* CAN Modes */ 
#define SILENT_MODE     0x03
#define LOOPBACK_MODE   0x04
#define BASIC_MODE      0x05

/* Standard/Extended identifier */
#define STANDARD_ID     (0U)
#define EXTENDED_ID     (1U)



/* CAN structure */  
typedef struct{
  uint8 CanID; 
  uint8  mode;
  uint32 bitrate;
}CANCongfig_t;

 /* Message object structure*/
typedef struct{
  uint32 ui32MsgID;
  uint8  ui8MsgObjectNumber;
  uint32 ui32MsgIDMask;
  uint8  ui8MsgIDExtension ;
  uint32 ui32MsgLen;
}CANMsgObject_t;

/* Functions Prototype  */ 
void CAN_Init( const CANCongfig_t * ConfigPtr );
void CAN_Transmit_SetObject(uint8 CANId,  CANMsgObject_t*  MsgObject);
void CAN_Transmit_Object( uint8 CANId , uint32  ObjNum, uint8* pdata);
void CAN_Receive_SetObject(uint8 CANId, CANMsgObject_t*  MsgObject);
uint8* CAN_Receive_Object( uint8 CANId , uint32 ObjNum);


#endif