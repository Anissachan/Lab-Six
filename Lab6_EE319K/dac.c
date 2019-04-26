
    // dac.c
    // This software configures DAC output
    // Lab 6 requires a minimum of 4 bits for the DAC, but you could have 5 or 6 bits
    // Runs on LM4F120 or TM4C123
    // Program written by: put your names here
    // Date Created: 3/6/17 
    // Last Modified: 3/5/18 
    // Lab number: 6
    // Hardware connections
    // TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
      // PB0-3 4 bit input pins
    
    #include <stdint.h>
    #include "../inc/tm4c123gh6pm.h"
    // Code files contain the actual implemenation for public functions
    // this file also contains an private functions and private data
    
    // **************DAC_Init*********************
    // Initialize 4-bit DAC, called once 
    // Input: none
    // Output: none
    void DAC_Init(void){
			int delay;
            
      SYSCTL_RCGC2_R |= 0x02;  //turning on clock B
      delay =SYSCTL_RCGC2_R; //allow time to finish activating
      GPIO_PORTB_AMSEL_R &=~0x0F; //no analog
      GPIO_PORTB_PCTL_R &=~0x00000FFF; //regular GPIO function
      GPIO_PORTB_DIR_R |= 0x0F; //setting PB0-3 as outputs
      GPIO_PORTB_AFSEL_R &= ~0x0F;  //disabling the alt function on PB2-5
      GPIO_PORTB_DEN_R |= 0x0F; //enabing PB0-3
    
    
    }
    
 // **************DAC_Out*********************
 // output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
    void DAC_Out(uint32_t data){
        GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & ~0x0F) | (data & 0x0F);
				
    }
		
void HeartBeat_Init(void){
			int delay;
			SYSCTL_RCGC2_R |= 0x20;
			delay = SYSCTL_RCGC2_R;
			GPIO_PORTF_DIR_R |= 0x02; //output for the hearbeat LED
	    GPIO_PORTF_DIR_R &= ~0x04; //input for the song button
			GPIO_PORTF_DEN_R |= 0x06;
}
//void SysTick_Handler(void){
//    Index=(Index+1)&0x0F; //activates the data structure
//   DAC_Out (SineWave[Index]); //outputs one value every interrupt
//}
		
        
