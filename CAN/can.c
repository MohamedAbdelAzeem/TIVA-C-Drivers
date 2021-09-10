 /******************************************************************************
 *
 * Module: CAN module
 *
 * File Name: CAN.c
 *
 * Author: Mohamed AbdelAzeem Ahmedy
 * 
 * Date : Sep,2021
 *
 *******************************************************************************/

#include "can.h"

/* Assuming CAN interface 1 is for transmitting */
/* Assuming CAN interface 2 is for transmitting */
   
volatile uint32 * CAN_BASE_ADDRESS_ARRAY[] = {(volatile uint32 *)CAN0_BASE_ADDRESS , (volatile uint32 *)CAN1_BASE_ADDRESS};




uint8 Bitrate_set( volatile uint32 *CAN_ptr ,uint32 bitrate);



/************************************************************************************
* Service Name: CAN_Init
* Parameters (in): ConfigPtr
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Init CAN module 
************************************************************************************/
void CAN_Init( const CANCongfig_t * ConfigPtr )
{
    volatile uint32 * CAN_ptr = NULL_PTR;     /* point to the required CAN Registers base address */
    /* Pointing to the corresponding CAN base address */
    CAN_ptr = CAN_BASE_ADDRESS_ARRAY[ConfigPtr->CanID];
  
    /* Enable CAN clock  */ 
    SET_BIT(RCGCCAN, ConfigPtr->CanID );
  
    /* Set the corresponding bit in CANCTL register to start initialization */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANCTL_OFFSET) , INIT_BIT); 
   
   
   switch( ConfigPtr->mode   )
   {
   case LOOPBACK_MODE: 	
     	/* Setting test bit to enable test mode  */
   	SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANCTL_OFFSET) , TEST_BIT);
	/* Enabling Loopback mode in control test register */
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANTST_OFFSET) , LBACK_BIT); 
     break;
   case SILENT_MODE: 
     
     break;
   }
    
   
   /* Bit timing calculations */   
   uint8 BitRateCheck =  Bitrate_set(CAN_ptr,ConfigPtr->bitrate);
   assert(BitRateCheck); /* Bitrate is not calculated properly */
   
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANCTL_OFFSET) , INIT_BIT); 
   
}


/************************************************************************************
* Service Name: Bitrate_set
* Parameters (in): bitrate 
* Parameters (inout): None
* Parameters (out): None
* Return value: return 1 if Bitrate calculated susccessfully else return 0
* Description: Function used by CAN_Init to Calculate bitrate parameters and set them to the CANBIT register
************************************************************************************/
uint8 Bitrate_set( volatile   uint32 *CAN_ptr ,uint32 bitrate)
{
     /* Write accesses to the CANBIT register are allowed  */
   SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANCTL_OFFSET) , CCE_BIT);
   
   uint32 desiredRatio = SYSTEM_CLOCK / bitrate ;  /* DesiredRatio = n * prescaler */
   
   /* calculated bit rate should be smaller required bit rate if the ratio is not an integer  */
  /*
   * Example : System Clock = 16Mhz  required bit rate = 9600 
   * desiredRatio = 16Mhz / 9600 = 1666.667 => 1666
   * calculated bitrate  = 16Mhz / 1666 = 9603 > 9600
   * if desiredRatio = 1666 + 1 = 1667 
   * calculated bitrate = 16Mhz / 1667 = 9598 < 9600 acceptable 
   */
   if(SYSTEM_CLOCK / desiredRatio >  bitrate)
   {
     desiredRatio += 1;
   }
   
   while(desiredRatio <= (CAN_MAX_PRESCALER  * CAN_MAX_BIT_DIVISOR))
   {
     for(uint32 n = CAN_MAX_BIT_DIVISOR ; n >= CAN_MIN_BIT_DIVISOR ; n--)
     {
     	uint32 prescaler = desiredRatio / n ;
	if( (prescaler * n) == desiredRatio  )
	{
	  /* If condition enusres that prescaler is an integer value and it is accepted */
	  /* assuming SYNC = 1 tq  && tprop = 2 tq */
	  /* tphase1 + tphase2 = (n-3) tq */
	  if((n - 3) & 1)
	  {
	    /* Odd value -> tphase2 = tphase1 + 1  = (n-3) tq */
	    
	        /* if prescaler > 63 (5 bits in CANBIT register ) we put other bits in CANBRPE register */
	    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANBIT_OFFSET) =
	      (( (tseg2(n)) & 0x7) << 12) | ( (tseg1(n) - 1 ) &0x0F << 8) | ( (tsjw) & 0x03 << 6)  | prescaler & 0x3F;
	    
	    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANBRPE_OFFSET) = (prescaler & 0x3C0) >> 6;
	    
	  }
	  else
	  {
	    /* even value -> tphase2 = tphase1 */
	    
	    /* if prescaler > 63 (5 bits in CANBIT register ) we put other bits in CANBRPE register */
	    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANBIT_OFFSET) =
	      (((tseg2(n)) & 0x7) << 12) | ( (tseg1(n)) &0x0F << 8) | ( (tsjw) & 0x03 << 6)  | prescaler & 0x3F;
	    
	    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANBRPE_OFFSET) = (prescaler & 0x3C0) >> 6;
	  }
	  return 1;
	}
     }
     /* if for loop ends means that calculated bitrate can not exist with integer values for prescaler */
     /* we add 1 to the desiredRatio so that calculated bit rate decreases by small amount and we loop again */
      desiredRatio += 1; 
   }
   
   /*A valid combination could not be found*/
  return 0;
}




/************************************************************************************
* Service Name: CAN_Transmit_SetObject
* Parameters (in): object number, Pointer to MsgObject
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Configure trasnmit message object 
************************************************************************************/
void CAN_Transmit_SetObject(uint8 CANId, CANMsgObject_t*  MsgObject)
{
     volatile uint32 * CAN_ptr = NULL_PTR;     /* point to the required CAN Registers base address */
    /* Pointing to the corresponding CAN base address */
    CAN_ptr = CAN_BASE_ADDRESS_ARRAY[CANId];
    
  /* Set WRNRD_BIT to  specify a write to the message RAM */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),WRNRD_BIT);
    
    /* Set ARB (Access arbitration bits) */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),ARB_BIT);
    
    /* Set the CONTROL bit in the CANIF1CMSK register  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),CONTROL_BIT);
    
    /* Access Data Byte 0 to 3 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),DATAA_BIT);
   
    /* Access Data Byte 4 to 7 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),DATAB_BIT); 
   
  
    /* Set direction to transmit  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB2_OFFSET),DIR_BIT);
    
    /* Check length of MsgID  */
    if(MsgObject->ui8MsgIDExtension)
    {
      /* 29-bit identifer  */
      /* Set  XTD_BIT to indicate Extended identifier */
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB2_OFFSET),XTD_BIT);
      
      /* adding first 16-bit  MsgID TO CANIF1ARB1 register   */
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB1_OFFSET) |= ((MsgObject->ui32MsgID & 0xFFFF));
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB1_OFFSET) |= (MsgObject->ui32MsgID >>  16 ) ;
    }
    else
    {
      /* 11-bit identifier  */
      
      /* Clear  XTD_BIT to indicate Extended identifier */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB2_OFFSET),XTD_BIT);
      
      /* adding MsgID TO CANIF1ARB2 register   */
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB2_OFFSET) |= (MsgObject->ui32MsgID  << 2);
    }
    
    /* set message length */
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1MCTL_OFFSET) |= MsgObject->ui32MsgLen ; 
   
    /* Set  MSGVAL to indicate that The message object is configured  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1ARB2_OFFSET),MSGVAL_BIT);  
    
    
    /* set message number */  
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CRQ_OFFSET) |= MsgObject->ui8MsgObjectNumber;
    
    /* Transmit configurations now is moved to specified object by message object number */
}




/************************************************************************************
* Service Name: CAN_Transmit_Object
* Parameters (in): CANID, object number, Pointer to data to be sent
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to send data in the corresponding object number (ObjNum) 
************************************************************************************/
void CAN_Transmit_Object( uint8 CANId , uint32  ObjNum, uint8* pdata)
{
    volatile uint32 * CAN_ptr = NULL_PTR;     /* point to the required CAN Registers base address */
    /* Pointing to the corresponding CAN base address */
    CAN_ptr = CAN_BASE_ADDRESS_ARRAY[CANId];
    
   /* Set WRNRD_BIT to  specify a write to the message RAM */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),WRNRD_BIT);
      
    /* Access Data Byte 0 to 3 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),DATAA_BIT);
    
    /* Access Data Byte 4 to 7 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),DATAB_BIT); 
    
    /* Entering data */
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1DA1_OFFSET) |=  pdata[0] | (pdata[1] << 8);
    
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1DA2_OFFSET) |=  pdata[2] | (pdata[3] << 8);
     
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1DB1_OFFSET) |=  pdata[4] | (pdata[5] << 8);
    
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1DB2_OFFSET) |=  pdata[6] | (pdata[7] << 8);
    
    /* set message number */  
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CRQ_OFFSET) |= ObjNum;
    
  
     /* Set TXRQST Bit to request a transmission */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CMSK_OFFSET),TXRQST_BIT);
    
    /* wait till last trasmition is done  */
    while(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF1CRQ_OFFSET), BUSY_BIT));
    
}

/************************************************************************************
* Service Name: CAN_Receive_SetObject
* Parameters (in): CANID, object number, Pointer to data to be sent
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Configure Receive message object 
************************************************************************************/
void CAN_Receive_SetObject(uint8 CANId, CANMsgObject_t*  MsgObject)
{
   volatile uint32 * CAN_ptr = NULL_PTR;     /* point to the required CAN Registers base address */
    /* Pointing to the corresponding CAN base address */
    CAN_ptr = CAN_BASE_ADDRESS_ARRAY[CANId];
  
    /* Set WRNRD_BIT to  specify a write to the message RAM */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),WRNRD_BIT);
    
    /* Set ARB (Access arbitration bits) */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),ARB_BIT);
    
    /* Set the CONTROL bit in the CANIF1CMSK register  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),CONTROL_BIT);
    
    /* Access Data Byte 0 to 3 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),DATAA_BIT);
   
    /* Access Data Byte 4 to 7 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),DATAB_BIT); 
   
    /* Set UMASK bit for MASK bits to be used for acceptance filtering */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET),UMASK_BIT); 
    
    /* Setting MASK  */
    if(MsgObject->ui8MsgIDExtension)
    {
      /* 29-bit identifer  */
 	*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MSK1_OFFSET) |= MsgObject->ui32MsgIDMask & 0xFFFF ;
	
	*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MSK2_OFFSET) |= (MsgObject->ui32MsgIDMask >> 16) & 0x1FFF ;
    }
    else
    {
      /* 11-bit identifier  */
	*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MSK2_OFFSET) |= (MsgObject->ui32MsgIDMask << 2);
    }
    
     /* Setting MSG id  */
    if(MsgObject->ui8MsgIDExtension)
    {
      /* 29-bit identifer  */
      /* Set  XTD_BIT to indicate Extended identifier */
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB2_OFFSET),XTD_BIT);
      
      /* adding first 16-bit  MsgID TO CANIF2ARB1 register   */
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB1_OFFSET) |= ((MsgObject->ui32MsgID & 0xFFFF));
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB1_OFFSET) |= (MsgObject->ui32MsgID >>  16 ) ;
    }
    else
    {
      /* 11-bit identifier  */
      /* Clear  XTD_BIT to indicate Extended identifier */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB2_OFFSET),XTD_BIT);
      
      /* adding MsgID TO CANIF1ARB2 register   */
      *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB2_OFFSET) |= (MsgObject->ui32MsgID  << 2);
    }
   
    /* Set  MSGVAL to indicate that The message object is configured  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB2_OFFSET),MSGVAL_BIT); 
    
   
    /* Set direction to Receive  */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2ARB2_OFFSET),DIR_BIT);
        
   /* Set EOB for a single message object */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET),EOB_BIT); 
    
    /* Set message length */
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET) |= MsgObject->ui32MsgLen ; 
    
     /* set message number */  
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CRQ_OFFSET) |= MsgObject->ui8MsgObjectNumber;
    /* Reception of the message object begins as soon as a matching frame is available on the CAN bus. */
}

 

/************************************************************************************
* Service Name: CAN_Receive_Object
* Parameters (in): CANID, object number, Pointer to data to be sent
* Parameters (inout): None
* Parameters (out): Pointer to received data
* Return value: None
* Description: Function to Receive data 
************************************************************************************/
uint8* CAN_Receive_Object( uint8 CANId , uint32 ObjNum)
{
    volatile uint32 * CAN_ptr = NULL_PTR;     /* point to the required CAN Registers base address */
    /* Pointing to the corresponding CAN base address */
    CAN_ptr = CAN_BASE_ADDRESS_ARRAY[CANId];
    
    /* wait till last reception is done  */
    while(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CRQ_OFFSET),BUSY_BIT));
    
    /* Set ARB (Access arbitration bits) */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),ARB_BIT);
    
    /* Set  CONTROL bit in the CANIF1CMSK register  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),CONTROL_BIT);
    
    /* Access Data Byte 0 to 3 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),DATAA_BIT);
   
    /* Access Data Byte 4 to 7 */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),DATAB_BIT); 
   
    /* Access Mask bits  */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CMSK_OFFSET),MASK_BIT); 
    
    /* set message number to transfer data to interface registers */  
    *(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2CRQ_OFFSET) |= ObjNum;
    
    /* static variables are stored in the data segment of the memory. */
    /* array to hold 8 bytes data */
    static uint8 receivedData[8] = {0};
    
    /* Check if NEWDAT bit is set  */
    if(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET),NEWDAT_BIT))
    {
      receivedData[0] = (*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DA1_OFFSET) & 0x00FF);
      receivedData[1] = ((*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DA1_OFFSET) & 0xFF00) >> 8);
      receivedData[2] = (*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DA2_OFFSET) & 0x00FF);
      receivedData[3] =((*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DA2_OFFSET) & 0xFF00) >> 8);
      receivedData[4] = (*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DB1_OFFSET) & 0x00FF);
      receivedData[5] =((*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DB1_OFFSET) & 0xFF00) >> 8);
      receivedData[6] = (*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DB2_OFFSET) & 0x00FF);
      receivedData[7] =((*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2DB2_OFFSET) & 0xFF00) >> 8);
    }
  
    if(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET), MSGLST_BIT))
    {
    	/* Call handler to handle if more than one message has been received since the last time this message object was read  */
      
       /* Clear MSGLST BIT*/
  	  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)CAN_ptr + CANIF2MCTL_OFFSET),MSGLST_BIT);
    }
    
  return receivedData;
}











