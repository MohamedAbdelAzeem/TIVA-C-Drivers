



#include "GPT.h"
#include "tm4c123gh6pm_registers.h"


/*Global variable*/
static  void (*g_Timer0A_callBackPtr)(void) = NULL_PTR;
static  void (*g_Timer0B_callBackPtr)(void) = NULL_PTR;

void Timer0A_ISR(void) //ISR for Timer0A 16bits or Timer0 32bits
{
  	if(g_Timer0A_callBackPtr != NULL_PTR)
	{
           /* Call the Call Back function in the application after the edge is detected */
	(*g_Timer0A_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
  
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Timer_0_BaseAddress + GPTM_Interrupt_Clear_Offset),TATOCINT); //Clear Timeout Flag
}

void Timer0B_ISR(void)//ISR for Timer0B 16bits
{
    	if(g_Timer0B_callBackPtr != NULL_PTR)
	{
           /* Call the Call Back function in the application after the edge is detected */
	(*g_Timer0B_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Timer_0_BaseAddress + GPTM_Interrupt_Clear_Offset),TBTOCINT); //Clear Timeout Flag
}

void Wide_Timer0A_ISR(void)//ISR for Wide_Timer0A 32bits or Wide_Timer0 64bits
{
  
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Wide_Timer_0_BaseAddress + GPTM_Interrupt_Clear_Offset),TATOCINT); //Clear Timeout Flag
}

void Wide_Timer0B_ISR(void) //ISR for Wide_Timer0B 32bits
{
 
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Wide_Timer_0_BaseAddress + GPTM_Interrupt_Clear_Offset),TBTOCINT); //Clear Timeout Flag
}

 

void Timer_Init(GPT_ConfigType * PtrConfig)
{
  
  volatile uint32 * TimerBaseAddress_Ptr = NULL_PTR; /* point to the required Timer Registers base address */
  
    switch( (PtrConfig->Timer_ID)& (0x00FF) )
    {
        case  0x00: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_0_BaseAddress;
                    SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P0); //Enable Timer 0 Clock
		 break;
	case  0x01: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_1_BaseAddress; 
                    SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P1); //Enable Timer 1 Clock
		 break;
	case  0x02: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_2_BaseAddress; 
                     SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P2); //Enable Timer 2 Clock
		 break;
	case  0x03: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_3_BaseAddress; 
                     SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P3); //Enable Timer 3 Clock
		 break;
        case  0x04: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_4_BaseAddress;
                      SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P4); //Enable Timer 4 Clock
		 break;
        case  0x05: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_5_BaseAddress; 
                      SET_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P5); //Enable Timer 5 Clock
		 break;
        case  0xE0: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_0_BaseAddress;
                     SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P0); //Enable Wide Timer 0 Clock
		 break;
	case  0xE1: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_1_BaseAddress;
                     SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P1); //Enable Wide Timer 1 Clock
		 break;
	case  0xE2: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_2_BaseAddress; 
                    SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P2); //Enable Wide Timer 2 Clock
		 break;
	case  0xE3: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_3_BaseAddress; 
                     SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P3); //Enable Wide Timer 3 Clock
		 break;
        case  0xE4: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_4_BaseAddress; 
                     SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P4); //Enable Wide Timer 4 Clock
		 break;
        case  0xE5: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_5_BaseAddress; 
                     SET_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P5); //Enable Wide Timer 5 Clock
		 break;
    }
    
       
     
     switch ((PtrConfig->Timer_ID)>>12)
     {
     case 0x00: //In case we use Timer_N_32bits or Wide_Timer_N_64bits
     case 0x0A: //In case we use Timer A 
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Control_Offset) , TAEN); //Disable Timer A
               *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Config_Offset) |= ((PtrConfig->Timer_ID) & (0X0F00))>>8; //GPT Configuration
              /* Select Timer Mode 
               * 0x01 One Shot mode
               * 0x02 Periodic mode
               * 0x03 Capture mode  
               */
              *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_A_Mode_Offset) &= ~(0x00000003);
              *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_A_Mode_Offset) |= (PtrConfig->Timer_Mode)&(0x03);
               
       break;
     case 0x0B:  //In case we use Timer B
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Control_Offset) , TBEN); //Disable Timer B
               *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Config_Offset) |= ((PtrConfig->Timer_ID) & (0X0F00))>>8; //GPT Configuration
              /* Select Timer Mode 
               * 0x01 One Shot mode
               * 0x02 Periodic mode
               * 0x03 Capture mode  
               */
              *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_B_Mode_Offset) &= ~(0x00000003);
              *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_B_Mode_Offset) |= (PtrConfig->Timer_Mode)&(0x03);
       break;
     }
      
}



void Timer_Start(uint8 Timer_ID, uint32 Bound_Value)
{
           
volatile uint32 * TimerBaseAddress_Ptr = NULL_PTR; /* point to the required Timer Registers base address */
  
 switch( (Timer_ID)& (0x00FF) )
    {
        case  0x00: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_0_BaseAddress;
		 break;
	case  0x01: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_1_BaseAddress; 
		 break;
	case  0x02: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_2_BaseAddress; 
		 break;
	case  0x03: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_3_BaseAddress; 
		 break;
        case  0x04: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_4_BaseAddress;
		 break;
        case  0x05: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_5_BaseAddress; 
		 break;
        case  0xE0: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_0_BaseAddress;
		 break;
	case  0xE1: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_1_BaseAddress;
		 break;
	case  0xE2: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_2_BaseAddress; 
		 break;
	case  0xE3: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_3_BaseAddress; 
		 break;
        case  0xE4: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_4_BaseAddress; 
		 break;
        case  0xE5: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_5_BaseAddress; 
		 break;
    }
 
 
  switch ((Timer_ID)>>12)
     {
     case 0x00: //In case we use Timer_N_32bits or Wide_Timer_N_64bits
     case 0x0A: //In case we use Timer A 
          if( (Timer_ID & (0x0FFF))== Wide_Timer_0 || (Timer_ID & (0x0FFF))== Wide_Timer_1 || (Timer_ID & (0x0FFF))== Wide_Timer_2 || (Timer_ID & (0x0FFF))== Wide_Timer_3 || (Timer_ID & (0x0FFF))== Wide_Timer_4 || (Timer_ID & (0x0FFF))== Wide_Timer_5) 
          {
            //64 BITS Not Supported yet
            
            //32 bits of GPTM_Timer_A_Interval_Load_Offset will be The Least Significant 32 bits of Bound_Value -1
            //32 bits of GPTM_Timer_B_Interval_Load_Offset will be The Most Significant 32 bits of Bound_Value -1
           // *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_A_Interval_Load_Offset) = (Bound_Value - 1)&(0X00000000FFFFFFFF);
           // *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_B_Interval_Load_Offset) = (Bound_Value - 1)>>32;
          
          }    
          else
          {
         *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_A_Interval_Load_Offset) = Bound_Value - 1;
          }
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Interrupt_Mask_Offset) , TATOIM); //Enable Timer A Interrupt               
       break;
     case 0x0B:  //In case we use Timer B
         *(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Timer_B_Interval_Load_Offset) = Bound_Value - 1;
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Interrupt_Mask_Offset) , TBTOIM); //Enable Timer B Interrupt
       break;
     }
 
    switch( (Timer_ID)& (0xF0FF) ) //Enable Timer in NVIC Register
    {
        case  0x0000: 
        case  0xA000:  SET_BIT(NVIC_EN0_REG , Timer_0A_Interrupt_EN);
		 break;
	case  0xB000:  SET_BIT(NVIC_EN0_REG , Timer_0B_Interrupt_EN);
		 break;
                 
        case  0x0001:
        case  0xA001:  SET_BIT(NVIC_EN0_REG , Timer_1A_Interrupt_EN);
		 break;
	case  0xB001:  SET_BIT(NVIC_EN0_REG , Timer_1B_Interrupt_EN);
		 break;
                 
        case  0x0002:
        case  0xA002:  SET_BIT(NVIC_EN0_REG , Timer_2A_Interrupt_EN);
		 break;
	case  0xB002:  SET_BIT(NVIC_EN0_REG , Timer_2B_Interrupt_EN);
		 break;
                 
        case  0x0003:
        case  0xA003:  SET_BIT(NVIC_EN1_REG , Timer_3A_Interrupt_EN);
		 break;
	case  0xB003:  SET_BIT(NVIC_EN1_REG , Timer_3B_Interrupt_EN);
		 break;
                 
        case  0x0004:
        case  0xA004:  SET_BIT(NVIC_EN2_REG , Timer_4A_Interrupt_EN);
		 break;
	case  0xB004:  SET_BIT(NVIC_EN2_REG , Timer_4B_Interrupt_EN);
		 break;
                 
        case  0x0005:
        case  0xA005:  SET_BIT(NVIC_EN2_REG , Timer_5A_Interrupt_EN);
		 break;
	case  0xB005:  SET_BIT(NVIC_EN2_REG , Timer_5B_Interrupt_EN);
		 break;
                 
        case  0x00E0:
        case  0xA0E0:  SET_BIT(NVIC_EN2_REG , Wide_Timer_0A_Interrupt_EN);
		 break;
	case  0xB0E0:  SET_BIT(NVIC_EN2_REG , Wide_Timer_0B_Interrupt_EN);
		 break;
                 
        case  0x00E1:
        case  0xA0E1:  SET_BIT(NVIC_EN3_REG , Wide_Timer_1A_Interrupt_EN);
		 break;
	case  0xB0E1:  SET_BIT(NVIC_EN3_REG , Wide_Timer_1B_Interrupt_EN);
		 break;
                 
        case  0x00E2:
        case  0xA0E2:  SET_BIT(NVIC_EN3_REG , Wide_Timer_2A_Interrupt_EN);
		 break;
	case  0xB0E2:  SET_BIT(NVIC_EN3_REG , Wide_Timer_2B_Interrupt_EN);
		 break;
                 
        case  0x00E3:
        case  0xA0E3:  SET_BIT(NVIC_EN3_REG , Wide_Timer_3A_Interrupt_EN);
		 break;
	case  0xB0E3:  SET_BIT(NVIC_EN3_REG , Wide_Timer_3B_Interrupt_EN);
		 break;
                 
        case  0x00E4:
        case  0xA0E4:  SET_BIT(NVIC_EN3_REG , Wide_Timer_4A_Interrupt_EN);
		 break;
	case  0xB0E4:  SET_BIT(NVIC_EN3_REG , Wide_Timer_4B_Interrupt_EN);
		 break;
                 
        case  0x00E5:
        case  0xA0E5:  SET_BIT(NVIC_EN3_REG , Wide_Timer_5A_Interrupt_EN);
		 break;
	case  0xB0E5:  SET_BIT(NVIC_EN3_REG , Wide_Timer_5B_Interrupt_EN);
		 break;
    }
   
     
     // Default Priority is used ^_^
           
     /*Enable Timer A*/
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)TimerBaseAddress_Ptr + GPTM_Control_Offset) , TAEN);
   
}


void Timer_Stop(uint8 Timer_ID)
{
  
   volatile uint32 * TimerBaseAddress_Ptr = NULL_PTR; /* point to the required Timer Registers base address */
  
    switch( (Timer_ID)& (0x00FF) )
    {
        case  0x00: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_0_BaseAddress;
                    CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P0); //Disable Timer 0 Clock
		 break;
	case  0x01: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_1_BaseAddress; 
                    CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P1); //Disable Timer 1 Clock
		 break;
	case  0x02: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_2_BaseAddress; 
                     CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P2); //Disable Timer 2 Clock
		 break;
	case  0x03: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_3_BaseAddress; 
                     CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P3); //Disable Timer 3 Clock
		 break;
        case  0x04: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_4_BaseAddress;
                      CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P4); //Disable Timer 4 Clock
		 break;
        case  0x05: TimerBaseAddress_Ptr = (volatile uint32 *)Timer_5_BaseAddress; 
                      CLEAR_BIT(GPTM_Timer_Run_Mode_Clock_Gating_Reg,P5); //Disable Timer 5 Clock
		 break;
        case  0xE0: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_0_BaseAddress;
                     CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P0); //Disable Wide Timer 0 Clock
		 break;
	case  0xE1: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_1_BaseAddress;
                     CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P1); //Disable Wide Timer 1 Clock
		 break;
	case  0xE2: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_2_BaseAddress; 
                    CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P2); //Disable Wide Timer 2 Clock
		 break;
	case  0xE3: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_3_BaseAddress; 
                     CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P3); //Disable Wide Timer 3 Clock
		 break;
        case  0xE4: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_4_BaseAddress; 
                     CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P4); //Disable Wide Timer 4 Clock
		 break;
        case  0xE5: TimerBaseAddress_Ptr = (volatile uint32 *)Wide_Timer_5_BaseAddress; 
                     CLEAR_BIT(Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg,P5); //Disable Wide Timer 5 Clock
		 break;
    }
  
}

void Timer0A_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer0A_callBackPtr = a_ptr;
}

void Timer0B_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer0B_callBackPtr = a_ptr;
}