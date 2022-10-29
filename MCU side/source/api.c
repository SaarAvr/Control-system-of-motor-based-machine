#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
//#include     <math.h>
int norm_stick_vec[2];
int step_counter;
unsigned int amplitude;
float delta_phi = 0.707;
float alpha_phi;
float stick_degree;
float pointer_degree;
float deg_result;
int wait_command;
int slow_flag = 0;
int step_counter_send[2];
int l;


//--------------------------------------------------------------------
//             motor control
//--------------------------------------------------------------------
void motor_control(){
    volatile unsigned int j;

    direc = 0;
    stop = 0;
    start = 0;
    TACTL = TASSEL_2 + MC_0 + ID_3;
    CCTL0 &= ~CCIE;
    TACCTL0 &= ~CCIE;

    __bis_SR_register(LPM0_bits + GIE);
    clockwise_turn();
    pointer_degree = 0;
    while(state == state1){
        get_value();
        //delay(1000);
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM3 w/ int until Byte RXed
        if(direc == 1){
            for(j=0; j < num_of_steps; j++){
                motor_clockwise();
            }

        }
        else if(direc == 2){
            for(j=0; j < num_of_steps; j++){
                motor_counterwise();
            }
        }
    }
    TACTL = TASSEL_2 + MC_0 + ID_3;
}




//--------------------------------------------------------------------
//             motor clockwise turn
//--------------------------------------------------------------------
void motor_clockwise(void){

    if (slow_flag == 1){
        TACCR0 = 0x03EA;
    }
    else{
        TACCR0 = 0x00EA; //EA-> Max Speeed
    }

    CCTL0 = CCIE;
    TACTL = TASSEL_2 + MC_1 + ID_3;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x08;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x01;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x02;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x04;


    TACTL = TASSEL_2 + MC_0 + ID_3;
    pointer_degree = pointer_degree + delta_phi;
    if (pointer_degree > 360){
        pointer_degree = pointer_degree - 360;
    }
}

//--------------------------------------------------------------------
//             motor counterwise turn
//--------------------------------------------------------------------
void motor_counterwise(void){

    if (slow_flag == 1){
        TACCR0 = 0x03EA;
    }
    else{
        TACCR0 = 0x00EA; //EA-> Max Speeed
    }

    CCTL0 = CCIE;
    TACTL = TASSEL_2 + MC_1 + ID_3;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x01;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x08;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x04;
    __bis_SR_register(LPM0_bits + GIE);
    P2_OUT = 0x02;


    TACTL = TASSEL_2 + MC_0 + ID_3;
    pointer_degree = pointer_degree - delta_phi;
    if (pointer_degree < 0){
        pointer_degree = pointer_degree + 360;
    }
}




//--------------------------------------------------------------------
//             clockwise turn
//--------------------------------------------------------------------
void clockwise_turn(void){

    while((start == 1) & (stop == 0)){
        TACCR0 = 0x03EA; //EA-> Max Speeed
        CCTL0 = CCIE;
        TACTL = TASSEL_2 + MC_1 + ID_3;

        P2_OUT = 0x01;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x08;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x04;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x02;
        __bis_SR_register(LPM0_bits + GIE);

    }
    TACTL = TASSEL_2 + MC_0 + ID_3;
    start = 0;
    stop  = 0;
}

//--------------------------------------------------------------------
//             amplitude calculator
//--------------------------------------------------------------------
unsigned int amp_calc(int vec[]){
    double result;
    double x;
    double y;
    //x = pow(vec[0],2);
    //y = pow(vec[1],2);
    //result = sqrt(x + y);
    result = 300;
    return (unsigned int)result;
}

//--------------------------------------------------------------------
//             amplitude calculator
//--------------------------------------------------------------------
void deg_calc(){

    float temp_deg;
    float atan_temp;
    double x;
    double y;

    x = norm_stick_vec[1];
    y = norm_stick_vec[0];
    //atan_temp = atan(y/x);
    atan_temp = 1;
    //temp_deg = atan_temp*180.0/3.14;
    temp_deg = 180;
    if (x>0){
        if (y>0){
            deg_result = temp_deg;
        }
        else{
            deg_result = temp_deg + 360;
        }
    }
    else{
        deg_result = temp_deg + 180;
    }
    stick_degree = deg_result;
}


//--------------------------------------------------------------------
//             stepper motor calipration
//--------------------------------------------------------------------
void calibrate(){

    __bis_SR_register(LPM0_bits + GIE);

    while((start == 1) & (stop == 0)){
        TACCR0 = 0x03EA; //EA-> Max Speeed
        CCTL0 = CCIE;
        TACTL = TASSEL_2 + MC_1 + ID_3;

        P2_OUT = 0x01;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x08;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x04;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x02;
        __bis_SR_register(LPM0_bits + GIE);

    }
    TACTL = TASSEL_2 + MC_0 + ID_3;
    start = 0;
    stop  = 0;
    step_counter = 0;

    __bis_SR_register(LPM0_bits + GIE);
    while((start == 1) & (stop == 0)){
        TACCR0 = 0x03EA; //EA-> Max Speeed
        CCTL0 = CCIE;
        TACTL = TASSEL_2 + MC_1 + ID_3;
        P2_OUT = 0x01;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x08;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x04;
        __bis_SR_register(LPM0_bits + GIE);
        P2_OUT = 0x02;
        __bis_SR_register(LPM0_bits + GIE);
        step_counter++;
    }
    TACTL = TASSEL_2 + MC_0 + ID_3;
    start = 0;
    stop  = 0;
    delta_phi = 360.0/step_counter;

    step_counter_send[1] = step_counter & 0xFF;
    step_counter_send[0] = step_counter >> 8;

    l = 0;
    IE2 |= UCA0TXIE;
    __bis_SR_register(LPM0_bits + GIE);
}

//--------------------------------------------------------------------
//             state 4
//--------------------------------------------------------------------
void script_mode(){
    int p;

    wait_command = 1;
    while(state == state4){
        __bis_SR_register(LPM0_bits + GIE);
        __no_operation();
        if(state4_command == 'a'){
            flash_flag = 0;
            temp_inx = 0;
            while(flash_flag == 0){
               __bis_SR_register(LPM0_bits + GIE);
            }
            if(num_of_files == 0){
                flash_erase();
            }
            flash_write();
            for (p=0; p<64; p++){
                rec_seg[p] = 0;
            }
            num_of_files = num_of_files + 1;
            if (num_of_files == 3){
                num_of_files = 0;
            }
            state4_command = 'f';
            wait_command = 1;
        }
        else if(state4_command == 'b'){
            Flash_ptr_read = (char *)0x1000;
            which_file_read = 1;
            run_script();
            wait_command = 1;
        }
        else if(state4_command == 'c'){
            which_file_read = 2;
            run_script();
            wait_command = 1;
        }
        else if(state4_command == 'd'){
            which_file_read = 3;
            run_script();
            wait_command = 1;
        }

    }
}

//--------------------------------------------------------------------
//            run script
//--------------------------------------------------------------------
void run_script(){
    int k;

    Flash_ptr_read = (char *) (0x1000 + (which_file_read - 1)*64);
    __bis_SR_register(LPM0_bits + GIE);
    clockwise_turn();
    pointer_degree = 0;
    for(k=0 ; k<num_of_opc[which_file_read - 1] ; k++){
        if(k==1){
            __no_operation();
        }
        read_file();

        if(opc == 1){
            blink_rgb();
        }
        else if(opc == 2){
            rlc_leds();
        }
        else if(opc == 3){
            rrc_leds();
        }
        else if(opc == 4){
            set_delay();
        }
        else if(opc == 5){
            clear_all_leds();
        }
        else if(opc == 6){
            stepper_deg();
        }
        else if(opc == 7){
            stepper_scan();
        }
        else if(opc == 8){
            __no_operation();
        }

    }

    TACTL = TASSEL_2 + MC_0 + ID_3;
    dynamic_disp = 0;
    IE2 |= UCA0TXIE;
    //__bis_SR_register(LPM0_bits + GIE);
    __no_operation();
}

//--------------------------------------------------------------------
//            run script
//--------------------------------------------------------------------
void painter(){
    while(state == state2){
        paint_values();
        __bis_SR_register(LPM0_bits + GIE);
    }
}





























