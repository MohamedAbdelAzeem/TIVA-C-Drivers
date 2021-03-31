
#ifndef GPT_H
#define GPT_H

/*Include Files*/
#include "Std_Types.h"
#include "Common_Macros.h"

/*GPT Base Addresses*/
#define Timer_0_BaseAddress   0x40030000
#define Timer_1_BaseAddress   0x40031000
#define Timer_2_BaseAddress   0x40032000
#define Timer_3_BaseAddress   0x40033000
#define Timer_4_BaseAddress   0x40034000
#define Timer_5_BaseAddress   0x40035000

#define Wide_Timer_0_BaseAddress   0x40036000
#define Wide_Timer_1_BaseAddress   0x40037000
#define Wide_Timer_2_BaseAddress   0x4004C00
#define Wide_Timer_3_BaseAddress   0x4004D000
#define Wide_Timer_4_BaseAddress   0x4004E000
#define Wide_Timer_5_BaseAddress   0x4004F000

/*Offest GPT Registers*/
#define GPTM_Config_Offset 0x00
#define GPTM_Timer_A_Mode_Offset 0x04
#define GPTM_Timer_B_Mode_Offset 0x08
#define GPTM_Control_Offset 0x0C
#define GPTM_Interrupt_Mask_Offset 0x18
#define GPTM_Raw_Interrupt_Status_Offset 0x1C
#define GPTM_Interrupt_Clear_Offset 0x24
#define GPTM_Timer_A_Interval_Load_Offset 0x28
#define GPTM_Timer_B_Interval_Load_Offset 0x2C
#define GPTM_Timer_A_Match_Offset 0x30
#define GPTM_Timer_B_Match_Offset 0x34
#define GPTM_Timer_A_Prescaler_Offset 0x38
#define GPTM_Timer_B_Prescaler_Offset 0x3C
#define GPTM_Timer_A_Offset 0x48
#define GPTM_Timer_B_Offset 0x4C

/*TimerN Clock enable Register*/
#define GPTM_Timer_Run_Mode_Clock_Gating_Reg        (*((volatile uint32 *)0x400FE604))

/*Wide_Timer_N Clock enable Register*/
#define Wide_GPTM_Timer_Run_Mode_Clock_Gating_Reg   (*((volatile uint32 *)0x400FE35C))

/*Enable / disable Interrupts*/
#define Enable_Interrupts()    __asm("CPSIE I")
#define Disable_Interrupts() __asm("CPSID I")

/* Go to low power mode while waiting for the next interrupt */
#define Wait_For_Interrupt()   __asm("WFI")

/*Timer Interrupt bit enable*/
#define Timer_0A_Interrupt_EN  (19U)
#define Timer_0B_Interrupt_EN  (20U)
#define Timer_1A_Interrupt_EN  (21U)
#define Timer_1B_Interrupt_EN  (22U)
#define Timer_2A_Interrupt_EN  (23U)
#define Timer_2B_Interrupt_EN  (24U)
#define Timer_3A_Interrupt_EN  (35U)
#define Timer_3B_Interrupt_EN  (36U)
#define Timer_4A_Interrupt_EN  (70U)
#define Timer_4B_Interrupt_EN  (71U)
#define Timer_5A_Interrupt_EN  (92U)
#define Timer_5B_Interrupt_EN  (93U)

/*Wide Timer Interrupt bit enable*/
#define Wide_Timer_0A_Interrupt_EN  (94U)
#define Wide_Timer_0B_Interrupt_EN  (95U)
#define Wide_Timer_1A_Interrupt_EN  (96U)
#define Wide_Timer_1B_Interrupt_EN  (97U)
#define Wide_Timer_2A_Interrupt_EN  (98U)
#define Wide_Timer_2B_Interrupt_EN  (99U)
#define Wide_Timer_3A_Interrupt_EN  (100U)
#define Wide_Timer_3B_Interrupt_EN  (101U)
#define Wide_Timer_4A_Interrupt_EN  (102U)
#define Wide_Timer_4B_Interrupt_EN  (103U)
#define Wide_Timer_5A_Interrupt_EN  (104U)
#define Wide_Timer_5B_Interrupt_EN  (105U)

/*Pre-Compile Configurations*/
#define Timer_0  0x0000   //32 BITS
#define Timer_0A 0xA400   //16 BITS
#define Timer_0B 0xB400  //16 BITS

#define Timer_1  0x0001
#define Timer_1A 0xA401
#define Timer_1B 0xB401

#define Timer_1  0x0001
#define Timer_1A 0xA401
#define Timer_1B 0xB401

#define Timer_2  0x0002
#define Timer_2A 0xA402
#define Timer_2B 0xB402

#define Timer_3  0x0003
#define Timer_3A 0xA403
#define Timer_3B 0xB403

#define Timer_4  0x0004
#define Timer_4A 0xA404
#define Timer_4B 0xB404

#define Timer_5  0x0005
#define Timer_5A 0xA405
#define Timer_5B 0xB405

/*Wide Timers*/
#define Wide_Timer_0  0x00E0     //64 BITS
#define Wide_Timer_0A 0xA4E0    //32 BITS
#define Wide_Timer_0B 0xB4E0   //32 BITS

#define Wide_Timer_1  0x00E1
#define Wide_Timer_1A 0xA4E1
#define Wide_Timer_1B 0xB4E1

#define Wide_Timer_1  0x00E1
#define Wide_Timer_1A 0xA4E1
#define Wide_Timer_1B 0xB4E1

#define Wide_Timer_2  0x00E2
#define Wide_Timer_2A 0xA4E2
#define Wide_Timer_2B 0xB4E2

#define Wide_Timer_3  0x00E3
#define Wide_Timer_3A 0xA4E3
#define Wide_Timer_3B 0xB4E3

#define Wide_Timer_4  0x00E4
#define Wide_Timer_4A 0xA4E4
#define Wide_Timer_4B 0xB4E4

#define Wide_Timer_5  0x00E5
#define Wide_Timer_5A 0xA4E5
#define Wide_Timer_5B 0xB4E5


/*Timer Modes*/
#define Timer_Periodic_Mode 0x02
#define Timer_One_Shot_Mode  0x01
#define Timer_Capture_Mode  0x03  //Not supported yet 6_6

/*Timer interrupt seconds*/
#define SECOND (16000000-1U)
#define MILLISECOND (16000-1U)


/*Some Timer Registers Pins*/
#define TAEN     (0U)   //Enable Timer A bit
#define TBEN     (8U)   //Enable Timer B bit
#define TATOIM   (0U)  //Enable interrupt Timer A Timeout bit
#define TBTOIM   (8U)  //Enable interrupt Timer B Timeout bit
#define TATORIS  (0U) //Status Falg bit of Timer A timeout
#define TATOCINT (0U) //Clear Status Flag bit of Timer A timeout by writing in it 1
#define TBTOCINT (8U) //Clear Status Flag bit of Timer B timeout by writing in it 1

/*Enable (Wide)Timer_N Clock bits*/
#define P0    (0U) //(Wide)Timer_0 Clock enable bit
#define P1    (1U) //(Wide)Timer_1 Clock enable bit
#define P2    (2U) //(Wide)Timer_2 Clock enable bit
#define P3    (3U) //(Wide)Timer_3 Clock enable bit
#define P4    (4U) //(Wide)Timer_4 Clock enable bit
#define P5    (5U) //(Wide)Timer_5 Clock enable bit

/*Type definitions*/
typedef struct{
  
  uint8 Timer_ID;
  uint8 Timer_Mode;
 
}GPT_ConfigType;

/*Functions Definitions*/
void Timer_Init(GPT_ConfigType * PtrConfig);
void Timer_Start(uint8 Timer_ID, uint32 Bound_Value);
void Timer_Stop(uint8 Timer_ID);
void Timer0A_setCallBack(void(*a_ptr)(void));
void Timer0B_setCallBack(void(*a_ptr)(void));


#endif 
