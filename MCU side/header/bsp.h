#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
// #include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      10000
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms
#define   delay_fac          0x051A


#define   TXLED            BIT0
#define   RXLED            BIT6
#define   TXD              BIT2
#define   RXD              BIT1


#define SWmask             0x0F

//---------------------------------------------------------------------
//            port1 config
//---------------------------------------------------------------------

#define P1_IN              P1IN
#define P1_OUT             P1OUT
#define P1_DIR             P1DIR
#define P1_SEL             P1SEL
#define P1_IFG             P1IFG
#define P1_IE              P1IE
#define P1_IES             P1IES

//---------------------------------------------------------------------
//            port2 config
//---------------------------------------------------------------------

#define P2_IN			   P2IN
#define P2_OUT      	   P2OUT
#define P2_DIR		       P2DIR
#define P2_SEL		       P2SEL
#define P2_IFG      	   P2IFG
#define P2_IE  	       	   P2IE
#define P2_IES     	 	   P2IES

//---------------------------------------------------------------------
//            push buttons config
//---------------------------------------------------------------------
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08





extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
extern void TIMER_A_config(void);

#endif



