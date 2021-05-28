#include"uart.h"

void Led_Red_Init(void)
{
     volatile uint32 delay = 0;
     SYSCTL_REGCGC2_REG |= (1<<5);
    delay = SYSCTL_REGCGC2_REG;
    GPIO_PORTF_AMSEL_REG &= ~(1<<1);      /* Disable Analog on PF1 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFFFF0F;   /* Clear PMCx bits for PF1 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   |= (1<<1);       /* Configure PF1 as output pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<1);      /* Disable alternative function on PF1 */
    GPIO_PORTF_DEN_REG   |= (1<<1);       /* Enable Digital I/O on PF1 */
    GPIO_PORTF_DATA_REG  &= ~(1<<1);      /* Clear bit 1 in Data regsiter to turn off the led */
  
}

void Led_Blue_Init(void)
{
    volatile uint32 delay = 0;
    SYSCTL_REGCGC2_REG |= (1<<5);
    delay = SYSCTL_REGCGC2_REG;
    GPIO_PORTF_AMSEL_REG &= ~(1<<2);      /* Disable Analog on PF2 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFFF0FF;   /* Clear PMCx bits for PF2 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   |= (1<<2);       /* Configure PF2 as output pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<2);      /* Disable alternative function on PF2 */
    GPIO_PORTF_DEN_REG   |= (1<<2);       /* Enable Digital I/O on PF2 */
    GPIO_PORTF_DATA_REG  &= ~(1<<2);      /* Clear bit 2 in Data regsiter to turn off the led */
  
}

int main()
{
  Led_Red_Init();
  Led_Blue_Init();
  UART_Config Configuration2 ={UART_2_RX,DATA_LENGTH_8_BITS,ONE_STOP_BIT};
  
  uint8 arr[] = {'U','A','R','T','\0'};
  UART_Init(&Configuration2);
  uint8 data_received ;
  uint8 data_received2[10] ;
  uint8 i=0,flag=1;
  
  while(1){
  // data_received = UART_Recieve_Byte(UART_2_RX);
    UART_receiveString(data_received2 ,UART_2_RX);
  
 /*if(data_received == 0xA7)
  {
   GPIO_PORTF_DATA_REG  |= (1<<2);    
  }
 for(int i=0;i<200000000;++i);
  data_received = UART_Recieve_Byte(UART_2_RX);
 
   if(data_received == 0xEE)
  {
   GPIO_PORTF_DATA_REG  |= (1<<1);    
  }
*/
 
 while( data_received2[i] != '\0')
  {
    if(data_received2[i] != arr[i])
    {
      flag=0;
    }
 
    ++i;
  }
  if(flag == 1)
  {
    GPIO_PORTF_DATA_REG  |= (1<<1);
  }
  
  
  }

  
  return 0;
}
