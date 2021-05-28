#include"uart.h"


/*

 (((ConfigPtr->UART_Mode)&(0x0F00))>>8)          --> UART Number(0 or 1 or 2  or 4 or 5 or 6 or 7)
    (((ConfigPtr->UART_Mode)&(0x00F0))>>4)       --> PORT Symbol (A or B or C or D or E or F) 
       ((ConfigPtr->UART_Mode)&(0x000F))         -->PIN(0 or 1 or 2 or 3 or 4 or 5 or 6 or 7)
        (((ConfigPtr->UART_Mode)&(0xF000))>>12)  -->E:Recieve or -->C:Send
         ((ConfigPtr->UART_Mode)&(0x00FF))       -->PORT PIN(A0 or A1 or B0 or B1 or C4 or C5 or D6 or D7 or C6 or C7 or C4 or C5 or E4 or E5 or D4 or D5 or E0 or E1)
*/


void UART_Init( const UART_Config * ConfigPtr)
{
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 * UART_Ptr = NULL_PTR;     /* point to the required UART Registers base address */
    volatile uint32 delay = 0;
             uint8 Port_Num ;
  
  //Enable UART n Clock
  SET_BIT(RCGCUART, (((ConfigPtr->UART_Mode)&(0x0F00))>>8) );
  delay = RCGCUART;
    
   switch( (((ConfigPtr->UART_Mode)&(0x00F0))>>4) )
   {
   case 0x0A:  Port_Num=0;
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
     break;
   case 0x0B:  Port_Num=1;
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
     break;
   case 0x0C:  Port_Num=2;
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
     break;
   case 0x0D:  Port_Num=3; 
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
     break;
   case 0x0E:  Port_Num=4;
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
     break;
   case 0x0F:  Port_Num=5;
              PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
     break;
   }
   
     /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_Num);
    delay = SYSCTL_REGCGC2_REG;
    
    if(  (((ConfigPtr->UART_Mode)&(0x00FF)) == 0xD7) ) //Unlock D7
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ((ConfigPtr->UART_Mode)&(0x000F)));  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    
    /*Enable Alternate Function*/
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), ((ConfigPtr->UART_Mode)&(0x000F)) );
      
   switch( (((ConfigPtr->UART_Mode)&(0x0F00))>>8) )
   {
   case 0:   UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
     break;
   case 1:   UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
     break;
   case 2:   UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
     break;
   case 3:   UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
     break;
   case 4:   UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
     break;
   case 5:   UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
     break;
   case 6:   UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
     break;
   case 7:   UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
     break;
     
   }
    
   //Puting the Mode Number in PMCn fields of Register GPIOPCTL 
   if(    ((ConfigPtr->UART_Mode) == UART_1_RX_PC4)  || ((ConfigPtr->UART_Mode) == UART_1_TX_PC5)  )
   {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~( 0x0000000F << ( ((ConfigPtr->UART_Mode)&(0x000F)) * 4)); /* Clear the PMCx bits for this pin*/
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  ( (2) << ( ((ConfigPtr->UART_Mode)&(0x000F))  * 4)); /* Set the PMCx bits for this pin with its mode number*/
   }
   else
   {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~( 0x0000000F << ( ((ConfigPtr->UART_Mode)&(0x000F)) * 4)); /* Clear the PMCx bits for this pin*/
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  ( (1) << ( ((ConfigPtr->UART_Mode)&(0x000F))  * 4)); /* Set the PMCx bits for this pin with its mode number*/
   }
    
   SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , ((ConfigPtr->UART_Mode)&(0x000F)) );/* Set the corresponding bit in the GPIODEN register to enable digital function*/
   
   //Disable UART
   CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),UARTEN );
   
   //Wait for Previous Transmission or receive
    switch( (((ConfigPtr->UART_Mode)&(0xF000))>>12) )
    {
    case 0x0C: //TX 
               while( BIT_IS_SET( *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTFR_OFFSET) ,TXFF) );
    break;
    case 0x0E: //RX
                while( BIT_IS_CLEAR( *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTFR_OFFSET) ,RXFE) );
    break;
    }
   
   //Flush Transmit FIFO
   CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTLCRH_OFFSET),FEN);
   
   //Write integer portion of BRD to the UARTIBRD register
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTIBRD_OFFSET) = UARTIBRD_VALUE;
   
   //Write fraction portion of BRD to the UARTFBRD register
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTFBRD_OFFSET) = UARTFBRD_VALUE;
   
   //Clearing UARTDR Data Register (Initial Values)
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTDR_OFFSET)  = 0x00000000; 

   
   //Clearing UARTLCRH (Initial Value)
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTLCRH_OFFSET) = 0x00000000;
   //Adjusting Data length as User wanted   
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTLCRH_OFFSET) |= ((ConfigPtr->Data_length_bits) << 5);   
   //Adjusting Number of Stop bits as User Wanted (1 or 2)
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTLCRH_OFFSET) |= ((ConfigPtr->Stop_Bits_No) << 3);
   
   //Choosing System Clock Source to be The UART Clock Source 
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCC_OFFSET) = 0;
   
   //Clearing UARTCTL (Initial Value)
    *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET)=0x00000000;
    
   //Enable Transmission or Receive
    switch( (((ConfigPtr->UART_Mode)&(0xF000))>>12) )
    {
    case 0x0C: SET_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),TXE );
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),RXE );
    break;
    case 0x0E: SET_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),RXE );
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),TXE );
    break;
    }

    //Enable UART n
   SET_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTCTL_OFFSET),UARTEN );
    
}

void UART_Send_Byte(uint8 data , uint16 UART_Mode)
{
  volatile uint32 * UART_Ptr = NULL_PTR;     /* point to the required UART Registers base address */
  
   switch( (((UART_Mode)&(0x0F00))>>8) )
   {
   case 0:   UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
     break;
   case 1:   UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
     break;
   case 2:   UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
     break;
   case 3:   UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
     break;
   case 4:   UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
     break;
   case 5:   UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
     break;
   case 6:   UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
     break;
   case 7:   UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
     break;
     
   }
   
   while( BIT_IS_SET( *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTFR_OFFSET) ,TXFF) ); /*The Transmitter is Full ,so do nth till it becomes empty*/
   /*Transmitter is empty and ready to transmit new data*/
  //Transmit data Byte
   *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTDR_OFFSET) |= data;
  
}

uint8 UART_Recieve_Byte(uint16 UART_Mode)
{
    volatile uint32 * UART_Ptr = NULL_PTR;     /* point to the required UART Registers base address */
  
   switch( (((UART_Mode)&(0x0F00))>>8) )
   {
   case 0:   UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
     break;
   case 1:   UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
     break;
   case 2:   UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
     break;
   case 3:   UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
     break;
   case 4:   UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
     break;
   case 5:   UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
     break;
   case 6:   UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
     break;
   case 7:   UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
     break;
     
   }
   
  while( BIT_IS_SET( *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTFR_OFFSET) ,RXFE) ); /*The Receiver is empty*/ 
  /*The Receiver has data to return */
   return ((uint8)((*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UARTDR_OFFSET))&(0x000000FF)));
  
  
}

void UART_sendString(const uint8 *Str, uint16 UART_Mode) //The characters in Str is stored in The Transmitter UARTDR Register in its ASCII number
{	
	while(*Str != '\0')
	{
                UART_Send_Byte(*Str , UART_Mode);
		Str++;
	}		
}


void UART_receiveString(uint8 *Str , uint16 UART_Mode) //The Global variable value will be changed as it passed by reference carrying the received data
{
	uint8 i = 0;
	Str[i] = UART_Recieve_Byte(UART_Mode);
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_Recieve_Byte(UART_Mode);
	}
	Str[i] = '\0';
}

