#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern unsigned int stick_vec[2];
extern int rec_seg[64];
extern int start;
extern int stop;
extern int get_flag;
extern int motor_flag;
extern int TX_flag;
extern unsigned int seperate_stick_vec[6];
extern int motor_state;
extern int direc;
extern int num_of_steps;
extern int delay_srt;
extern int flash_flag;
extern int num_of_files;
extern char state4_command;
extern int opc;
extern int which_file_read;
extern char *Flash_ptr_read;
extern int num_of_opc[3];
extern int temp_inx;
extern int dynamic_disp;
//extern const char string1[];

extern void delay(unsigned int);
extern void sysConfig(void);
extern void enterLPM(unsigned char);
extern void get_value();
extern void blink_rgb();
extern void rlc_leds();
extern void rrc_leds();
extern void set_delay();
extern void clear_all_leds();
extern void flash_write();
extern void flash_write();
extern void read_file();
extern void stepper_deg();
extern void stepper_scan();
extern void paint_values();

extern __interrupt void Timer_A (void);
extern __interrupt void ADC10_ISR (void);
extern __interrupt void PBs_handler(void);
extern __interrupt void USCI0TX_ISR(void);
extern __interrupt void USCI0RX_ISR(void);

#endif







