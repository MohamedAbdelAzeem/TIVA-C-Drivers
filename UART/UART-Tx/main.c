#include"uart.h"

int main()
{
  uint8 string []= {'U','A','R','T','#','\0'};
  UART_Config Configuration1 ={UART_1_TX_PB1,DATA_LENGTH_8_BITS,ONE_STOP_BIT};
  
  UART_Init(&Configuration1);
  
  while(1)

   //UART_Send_Byte(0xA7, UART_1_TX_PB1);
   UART_sendString(string, UART_1_TX_PB1);

   //UART_Send_Byte(0xEE, UART_1_TX_PB1);
   
  return 0;
}
