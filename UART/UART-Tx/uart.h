#ifndef UART_H
#define UART_H

#include "Common_Macros.h"
#include "Std_Types.h"
#include "tm4c123gh6pm_registers.h"


//UART Clock Calculations
/*
BDR = UART Clock/(divisor*Baud rate)
BDR= UARTIBRD(integer part) + UARTFBRD(float part*64+0.5)
*/
#define UART_CLOCK (16000000.0)
#define UART_CLOCK_DIVISOR (16.0)
#define UART_BAUD_RATE (9600.0)
#define BDR(UART_CLOCK,UART_CLOCK_DIVISOR,UART_BAUD_RATE)  ((UART_CLOCK)/((UART_CLOCK_DIVISOR)*(UART_BAUD_RATE)))
#define UARTIBRD_VALUE ((uint16)BDR(UART_CLOCK,UART_CLOCK_DIVISOR,UART_BAUD_RATE))
#define UARTFBRD_VALUE  ((uint16)((((BDR(UART_CLOCK,UART_CLOCK_DIVISOR,UART_BAUD_RATE))-(UARTIBRD_VALUE) )*(64))+(0.5)))

//Enable UART & PORT Clock Registers
#define RCGCUART  (*((volatile uint32 *)0x400FE618))  //Enable UART n Clock with corresponding bit n in the Register
#define RCGCGPIO  (*((volatile uint32 *)0x400FE608))  //Enable PORT Clock

//Base Registers
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

#define UART0_BASE_ADDRESS                0x4000C000
#define UART1_BASE_ADDRESS                0x4000D000
#define UART2_BASE_ADDRESS                0x4000E000
#define UART3_BASE_ADDRESS                0x4000F000
#define UART4_BASE_ADDRESS                0x40010000
#define UART5_BASE_ADDRESS                0x40011000
#define UART6_BASE_ADDRESS                0x40012000
#define UART7_BASE_ADDRESS                0x40013000

//Offest Registers
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_CTL_REG_OFFSET               0x52C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C

#define UARTDR_OFFSET                     0x000
#define UARTCTL_OFFSET                    0x030
#define UARTIBRD_OFFSET                   0x024
#define UARTFBRD_OFFSET                   0x028
#define UARTLCRH_OFFSET                   0x02C
#define UARTCC_OFFSET                     0xFC8
#define UARTFR_OFFSET                     0x018

//UART Registers Bits
#define UARTEN   (0U)
#define TXE      (8U)
#define RXE      (9U)
#define TXFF     (5U)
#define RXFE     (4U)
#define FEN      (4U)

//User Configuration
#define UART_0_RX            0xE0A0   //E:Recieve ,0:UART0, A0: Pin A0
#define UART_0_TX            0xC0A1   //C:Send ,0:UART0, A1: Pin A1
#define UART_1_RX_PB0        0xE1B0
#define UART_1_TX_PB1        0xC1B1 
#define UART_1_RX_PC4        0xE1C4
#define UART_1_TX_PC5        0xC1C5
#define UART_2_RX            0xE2D6
#define UART_2_TX            0xC2D7
#define UART_3_RX            0xE3C6 
#define UART_3_TX            0xC3C7
#define UART_4_RX            0xE4C4
#define UART_4_TX            0xC4C5
#define UART_5_RX            0xE5E4
#define UART_5_TX            0xC5E5
#define UART_6_RX            0xE6D4
#define UART_6_TX            0xC6D5
#define UART_7_RX            0xE7E0
#define UART_7_TX            0xC7E1

#define DATA_LENGTH_5_BITS    (0U)
#define DATA_LENGTH_6_BITS    (1U)
#define DATA_LENGTH_7_BITS    (2U)
#define DATA_LENGTH_8_BITS    (3U)

#define ONE_STOP_BIT          (0U)
#define TWO_STOP_BITS         (1U)

//Data Types

typedef struct{
uint16 UART_Mode;
uint8 Data_length_bits;
uint8 Stop_Bits_No;

}UART_Config;

//Functions Prototype
void UART_Init( const UART_Config * ConfigPtr);
void UART_Send_Byte(uint8 data , uint16 UART_Mode);
uint8 UART_Recieve_Byte(uint16 UART_Mode);
void UART_sendString(const uint8 *Str, uint16 UART_Mode);
void UART_receiveString(uint8 *Str , uint16 UART_Mode);




#endif