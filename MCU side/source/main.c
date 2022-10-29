#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
               // enterLPM(lpm_mode);
        __bis_SR_register(LPM0_bits + GIE);
		break;
		 
	  case state1:
		TIMER_A_config();
		motor_control();
	    break;
		 
	  case state2:
	      painter();
	    break;
		
	  case state3:
	      calibrate();
	      state = state0;
	    break;

	  case state4:
            script_mode();
            state = state0;
        break;
	}
  }
}
  
  
  
  
  
