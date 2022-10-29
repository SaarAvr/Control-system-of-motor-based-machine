#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/api.h"

unsigned int stick_vec[2];
unsigned int seperate_stick_vec[6];
unsigned int seperate_paint[4];
unsigned int seperate_4FF[4] = {0xFF,0xFF,0xFF,0xFF};
int start = 0;
int stop  = 0;

const char string1[] = { "Hello World\r\n" };
unsigned int i;
unsigned int m, z;
int get_flag = 0;
int motor_flag = 0;
int motor_state = 0;
float num = 36.8;
int num_int;
int TX_flag;
int track_flag = 0;
int direc = 0;
int num_of_steps;
int rgb_color[8] = {0x00,0x20,0x40,0x60,0x80,0xA0,0xC0,0xE0};
int delay_srt = delay_fac * 50;
int operand1 = 0;
int operand2 = 0;
int shift[5] = {0x10,0x20,0x40,0x80};
int led_state = 0;
int first_opc = 1;
int num_of_opc[3];
int rec_seg[64];
int seg_inx = 0;
char two_bytes[2];
int temp_inx = 0;
int flash_flag = 0;
char *Flash_ptr;                          // Flash pointer
char *Flash_ptr_read;
int num_of_files = 0;
char state4_command;
int opc = 0;
int which_file_read;
int dynamic_disp;
int dynamic_deg, state2_flag = 0;
char test;


//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
    volatile unsigned int i;

    for(i=t; i>0; i--);
}

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
}

//--------------------------------------------------------------------
//             joystick
//--------------------------------------------------------------------
void get_value(){
    unsigned int low1, low2, high1, high2;
    //int i;
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);
    ADC10CTL0 |= ENC + ADC10SC;
    ADC10SA = (unsigned int)stick_vec;                     // Set Breakpoint here to read ADC
    //__no_operation();
    //__no_operation();
    //delay(1000);
    seperate_stick_vec[1] = stick_vec[0] & 0xFF;
    seperate_stick_vec[3] = stick_vec[1] & 0xFF;
    seperate_stick_vec[0] = stick_vec[0] >> 8;
    seperate_stick_vec[2] = stick_vec[1] >> 8;
    //num = pointer_degree;
    num_int = pointer_degree;
    seperate_stick_vec[4] = num_int >> 8;
    seperate_stick_vec[5] = num_int & 0xFF;

    i = 0;
    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
    //UCA0TXBUF = stick_vec[i++];

    //  IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
}

//---------------------------------------------------------------------
//               state 4
//              functions
//---------------------------------------------------------------------

//--------------------------------------------------------------------
//          write to flash
//--------------------------------------------------------------------
void flash_write(){
    int i = 0;
    FCTL2 = FWKEY + FSSEL_1 + FN0;       // Clk = SMCLK/4
    FCTL3 = FWKEY;                       // Clear Lock bit
    FCTL1 = FWKEY + WRT;                 // Set WRT bit for write operation
    Flash_ptr = (char *) (0x1000 + num_of_files*64);             // Initialize Flash segment C pointer

    for (i=0; i<64; i++)
      *Flash_ptr++ = rec_seg[i];         // copy value to flash

    FCTL1 = FWKEY;                        // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                 // Set LOCK bit

    seg_inx = 0;
    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
}
//---------------------------------------------------------------------
//            Erase Flash func
//---------------------------------------------------------------------
void flash_erase()
{
  for (i=0; i<3; i++){
      while(BUSY & FCTL3);                 // Check if Flash being used
      FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
      FCTL1 = FWKEY + ERASE;               // Set Erase bit
      FCTL3 = FWKEY;                       // Clear Lock bit

      Flash_ptr = (char *) (0x1000 + i*64);
      *Flash_ptr = 0;                           // Dummy write to erase Flash segment

      while(BUSY & FCTL3);                 // Check if Flash being used
      FCTL1 = FWKEY;                       // Clear WRT bit
      FCTL3 = FWKEY + LOCK;                // Set LOCK bit
  }
}

//---------------------------------------------------------------------
//            read the script
//---------------------------------------------------------------------
void read_file()
{
    int temp;
    //temp = *Flash_ptr_read++;
    opc = *Flash_ptr_read++;
    //opc = opc + (16 * temp);

    if((opc != 5) & (opc != 8)){
        operand1 = *Flash_ptr_read++;
        //temp = Flash_ptr_read++;
        //operand1 = operand1 + (16 * temp);
    }
    if(opc == 7){
        operand2 = *Flash_ptr_read++;
        //temp = Flash_ptr_read++;
        //operand2 = operand2 + (16 * temp);
    }
}

//--------------------------------------------------------------------
//          blink rgb
//--------------------------------------------------------------------
void blink_rgb(){
    int cnt = 0;
    int i;
    TACCR0 = delay_srt;
    while(cnt < operand1){
        for(i=0 ; i<8 ; i++){
            CCTL0 = CCIE;
            TACTL = TASSEL_2 + MC_1 + ID_3;
            __bis_SR_register(LPM0_bits + GIE);
            P1_OUT = rgb_color[i];
        }
        cnt = cnt + 1;
    }
}

//--------------------------------------------------------------------
//         Rotate left circularly a single lighted LEDv
//--------------------------------------------------------------------
void rlc_leds(){
    int cnt = 0;
    int i;
    TACCR0 = delay_srt;
    for(i=0 ; i<operand1 ; i++){
        CCTL0 = CCIE;
        TACTL = TASSEL_2 + MC_1 + ID_3;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = shift[led_state];
        led_state = led_state + 1;
        if(led_state == 4){
            led_state = 0;
        }
    }
}

//--------------------------------------------------------------------
//          Rotate left circularly a single lighted LED
//--------------------------------------------------------------------
void rrc_leds(){
    int cnt = 0;
    int i;
    TACCR0 = delay_srt;
    for(i=0 ; i<operand1 ; i++){
        CCTL0 = CCIE;
        TACTL = TASSEL_2 + MC_1 + ID_3;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = shift[led_state];
        led_state = led_state - 1;
        if(led_state < 0){
            led_state = 3;
        }
    }
}

//--------------------------------------------------------------------
//          Set the delay
//--------------------------------------------------------------------
void set_delay(){
    delay_srt = delay_fac * operand1;
}

//--------------------------------------------------------------------
//          Clear all LEDs
//--------------------------------------------------------------------
void clear_all_leds(){
    P1_OUT = 0;
    P2_OUT = 0;
}

//--------------------------------------------------------------------
//          stepper deg
//--------------------------------------------------------------------
void stepper_deg(){
    int num_of_delta;
    int u;
    int diff;

    diff = operand1 - pointer_degree;
    if(diff > 180){
        diff = diff - 360;
    }
    if (diff < -180){
        diff = 360 + diff;
    }
    if (diff > 0){
        num_of_delta = (int)(diff / delta_phi);
        for(u=0 ; u<num_of_delta ; u++){
            motor_clockwise();
        }
    }
    else{
        num_of_delta = (int)(-diff / delta_phi);
        for(u=0 ; u<num_of_delta ; u++){
            motor_counterwise();
        }
    }
    dynamic_deg = pointer_degree;
    if (dynamic_deg == 255){
        dynamic_deg = 254;
    }
    dynamic_disp = 1;
    IE2 |= UCA0TXIE;
}

//--------------------------------------------------------------------
//          stepper scan
//--------------------------------------------------------------------
void stepper_scan(){
    int num_of_delta;
    int u;
    int diff;

    diff = operand1 - pointer_degree;
    if(diff > 180){
            diff = diff - 360;
    }
    if (diff < -180){
        diff = 360 + diff;
    }
    if (diff > 0){
        num_of_delta = (int)(diff / delta_phi);
        for(u=0 ; u<num_of_delta ; u++){
            motor_clockwise();
        }
    }
    else{
        num_of_delta = (int)(-diff / delta_phi);
        for(u=0 ; u<num_of_delta ; u++){
            motor_counterwise();
        }
    }
    dynamic_deg = pointer_degree;
    if (dynamic_deg == 255){
        dynamic_deg = 254;
    }
    dynamic_disp = 1;
    IE2 |= UCA0TXIE;

    slow_flag = 1;
    num_of_delta = (int)(abs(operand2 - operand1) / delta_phi);
    for(u=0 ; u<num_of_delta ; u++){
        motor_clockwise();
    }
    slow_flag = 0;
    dynamic_deg = pointer_degree;
    if (dynamic_deg == 255){
        dynamic_deg = 254;
    }
    dynamic_disp = 1;
    IE2 |= UCA0TXIE;



    //num_of_delta = (int)(360 - operand1 / delta_phi);
    //for(u=0 ; u<num_of_delta ; u++){
    //    motor_counterwise();
    //}
    //num_of_delta = (int)(operand2 / delta_phi);
    //for(u=0 ; u<num_of_delta ; u++){
    //    motor_clockwise();
    //}
}





//--------------------------------------------------------------------
//             paint values func
//--------------------------------------------------------------------
void paint_values(){
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);
    ADC10CTL0 |= ENC + ADC10SC;
    ADC10SA = (unsigned int)stick_vec;                     // Set Breakpoint here to read ADC
    if(state2_flag == 0){
        seperate_paint[1] = stick_vec[0] & 0xFF;
        seperate_paint[3] = stick_vec[1] & 0xFF;
        seperate_paint[0] = stick_vec[0] >> 8;
        seperate_paint[2] = stick_vec[1] >> 8;
    }
    else{
        state2_flag = 0;
        seperate_paint[1] = 0xFF;
        seperate_paint[3] = 0xFF;
        seperate_paint[0] = 0xFF;
        seperate_paint[2] = 0xFF;
    }


    //state2_flag = 0;
    m = 0;
    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
    //UCA0TXBUF = stick_vec[i++];

    //  IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
}














//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}


// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL1 &= ~CCIFG;
	__bic_SR_register_on_exit(LPM0_bits + GIE);
}

// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){

    delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(P1_IFG & PB0){
        P1_IFG &= ~PB0;
        state2_flag = 1;

      //state = state1;
        //P1_IFG &= ~PB0;
        }
    else if(P1_IFG & PB1){
      //state = state2;
        P1_IFG &= ~PB1;
        }
    else if(P1_IFG & PB2){
      //state = state3;
        P1_IFG &= ~PB2;
        }
    else if(P1_IFG & PB3){
        P1_IFG &= ~PB3;
        if (start + stop == 0){
            start = 1;
        }
        else{
            start = 0;
            stop  = 1;
        }
        //state = state4;
    }
//---------------------------------------------------------------------
//            Exit from a given LPM
//---------------------------------------------------------------------
        switch(lpm_mode){
        case mode0:
         LPM0_EXIT; // must be called from ISR only
         break;

        case mode1:
         LPM1_EXIT; // must be called from ISR only
         break;

        case mode2:
         LPM2_EXIT; // must be called from ISR only
         break;

                case mode3:
         LPM3_EXIT; // must be called from ISR only
         break;

                case mode4:
         LPM4_EXIT; // must be called from ISR only
         break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if(state == state1){
      UCA0TXBUF = seperate_stick_vec[i];                 // TX next character
      i = i + 1;
      if (i > 5)                   //(i == sizeof stick_vec - 1)              // TX over?
        IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
  }
  else if(state == state2){
          UCA0TXBUF = seperate_paint[m];
          m = m + 1;
          if (m > 3){
              IE2 &= ~UCA0TXIE;
              __bic_SR_register_on_exit(CPUOFF);
          }
  }
  else if(state == state3){
      UCA0TXBUF = step_counter_send[l];
      l = l + 1;
      if (l > 1){
          IE2 &= ~UCA0TXIE;
          __bic_SR_register_on_exit(CPUOFF);
      }
    }
  else if(state == state4){
      if (dynamic_disp == 1){
          UCA0TXBUF = dynamic_deg;
          IE2 &= ~UCA0TXIE;
          dynamic_disp = 0;
      }
      else{
          UCA0TXBUF = 255;
          IE2 &= ~UCA0TXIE;
      }
  }
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    int bytes2byte;
    int int1;
    int int2;
    if(state == state1){
        if (track_flag == 0){
            if(UCA0RXBUF == 's'){
                start = 1;
                stop = 0;
            }
            else if(UCA0RXBUF == 't'){
                start = 0;
                stop = 1;
                track_flag = 1;
            }
        }
        else{
            if((UCA0RXBUF >= 1) & (UCA0RXBUF <= 120)){
                direc = 1;
                num_of_steps = UCA0RXBUF;
            }
            else if((UCA0RXBUF >= 121) & (UCA0RXBUF <= 240)){
                direc = 2;
                num_of_steps = UCA0RXBUF -120;
            }
            else if(UCA0RXBUF == 255){    //255 to exit
                state = state0;
                direc = 0;
                track_flag = 0;
            }
            else{
                direc = 0;
            }
        }
    }


    if(state == state2){
        if(UCA0RXBUF == 'x'){
            state = state0;
        }
    }


    if(state == state3){
        if(UCA0RXBUF == 's'){
            start = 1;
            stop = 0;
        }
        else if(UCA0RXBUF == 't'){
            start = 0;
            stop = 1;
        }
    }


    if(state == state4){
        test = UCA0RXBUF;
        if(UCA0RXBUF == 'x'){
            state = state0;
        }
        else{
            if(wait_command == 1){
                state4_command = UCA0RXBUF;
                wait_command = 0;
                first_opc = 1;
            }
            else{
                if(state4_command == 'a'){
                   if(UCA0RXBUF == 's'){
                       flash_flag = 1;
                   }
                   else{
                       if(first_opc){
                           num_of_opc[num_of_files] = UCA0RXBUF;
                           first_opc = 0;
                       }
                       else{
                           if(temp_inx == 0){
                               two_bytes[0] = UCA0RXBUF;
                               temp_inx = 1;
                           }
                           else{
                               two_bytes[1] = UCA0RXBUF;
                               if((int)two_bytes[1] > 58){
                                   int1 = ((int)two_bytes[1]) - 55;
                               }
                               else{
                                   int1 = ((int)two_bytes[1]) - 48;
                               }
                               if((int)two_bytes[0] > 58){
                                   int2 = ((int)two_bytes[0]) - 55;
                               }
                               else{
                                   int2 = ((int)two_bytes[0]) - 48;
                               }
                               bytes2byte = (int1 + (int2 * 16));
                               rec_seg[seg_inx++] = bytes2byte;
                               temp_inx = 0;
                           }
                            //temp_inx = 0;
                       }
                   }
               }
               else{
                   if(UCA0RXBUF == 's'){
                       start = 1;
                       stop = 0;
                   }
                   else if(UCA0RXBUF == 't'){
                       start = 0;
                       stop = 1;
                   }
               }
                //else{
                //    if(state4_command == 'b'){
                //        which_file_read = 1;
                //    }
                //    else if(state4_command == 'c'){
                //        which_file_read = 2;
                //    }
                //    else if(state4_command == 'd'){
                //      which_file_read = 3;
                //    }
                //}
           }
       }
   }







    else{
        if(UCA0RXBUF == '1'){
            state = state1;
        }
        else if(UCA0RXBUF == '2'){
            state = state2;
        }
        else if(UCA0RXBUF == '3'){
            state = state3;
        }
        else if(UCA0RXBUF == '4'){
            state = state4;
        }
    }
    __bic_SR_register_on_exit(CPUOFF);
}

