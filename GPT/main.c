#include "GPT.h"
#include "tm4c123gh6pm_registers.h"


/* Enable PF1 (RED LED) */
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

void Toggle_led(void)
{
   TOGGLE_BIT(GPIO_PORTF_DATA_REG,1);
}


int main(void)
{
  /*GPT User Configuration*/
  Enable_Interrupts();
  GPT_ConfigType Configurations = {Timer_0, Timer_Periodic_Mode};
  Led_Red_Init();
  Timer_Init(&Configurations);
  /*
   *Limitations: 0< time out <= 4ms              in 16 bits Timer
                 0< time out <= 268 sec          in 32 bits Timer  
                 0< time out <= 444799 months    in 64 bits Timer ^_^ 
   */
  Timer_Start(Timer_0, 2 * SECOND);
  Timer0A_setCallBack(Toggle_led);
  while(1)
  {
    
     Wait_For_Interrupt();
     
  }
}
