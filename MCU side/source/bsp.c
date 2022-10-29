#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  

  P2_DIR		|= 0x0F;   //2.0-2.3
  //P2_SEL		&= ~0x00;
  P2_OUT	 	=  0x00;
  
  //leds p2.4-p2.7
  P2_DIR        |= 0xF0;
  P2_SEL        &= ~0xF0;


  //joy PB - p1.0
  P1_DIR        &= ~0x01;
  P1_SEL        &= ~0x01;
  P1_IE         =  0x01;
  P1_IFG        =  0x00;
  P1_IES        =  0x00;

  //digital joy p1.3-p1.4
  P1_DIR        &= ~0x18;
  P1_SEL        |= 0x18; // p1.2
  P1_OUT        =  0x00;

  //Tx, Rx - p1.1-p1.2
    P1_SEL        |= 0x06; // p1.2
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD

 //RGB  p1.5-p1.7
    P1_DIR     |= 0xE0;
    P1_SEL     &= ~0xE0;



  if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
    {
      while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;


    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;                           //
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS0;               //
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM3 w/ int until Byte RXed











  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	
	//write here timers congiguration code
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADC10CTL1 = INCH_5 + ADC10DIV_0 + CONSEQ_1 + SHS_0;
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON; //ADC10IE
    ADC10AE0  = 0x18;
    ADC10DTC1 = 0x02;
}              

void TIMER_A_config(void){

	TACTL = TASSEL_2 + MC_3 + ID_3;                  // SMCLK, upmode
	TACCTL0 = CCIE;
	TA0R = 0;
}           
             

 
             
             
            
  

