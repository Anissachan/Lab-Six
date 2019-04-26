
// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/5/18 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "../inc/tm4c123gh6pm.h"
#define quarter 500
#define half 1000
#define eighth 250
#define sixteenth 125
#define a			2841
#define fsharp 3378
#define csharp 4510
#define d      4257
#define b			2531





// **************Sound_Init*********************
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none

uint32_t sineWave[] = {8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};

int wave_index;
void Sound_Init(void){
       DAC_Init();                                //setting PortB as DAC
       wave_index=0;

          //SYSCTL_RCGC2_R |= 0x000002; //activate port B
          //delay = SYSCTL_RCGC2_R; 
          //GPIO_PORTB_AMSEL_R &=~0x0F; //no analog
          //GPIO_PORTB_PCTL_R &=~0x00F00000; //regular function
          //GPIO_PORTB_DIR_R |= 0x0F; //make PB0-3 make output
          //GPIO_PORTB_DR8R_R|=0x0F; //drive up to 8mA out
          //GPIO_PORTB_AFSEL_R &=~0x0F; //disables alt function
          //GPIO_PORTB_DEN_R |=0x0F; //enable digital I/O

          NVIC_ST_CTRL_R=0; //disable systick for set up
          NVIC_ST_RELOAD_R=0x00000000; //unsure about this value
          NVIC_ST_CURRENT_R=0; //any write to current clears it
          NVIC_SYS_PRI3_R= NVIC_SYS_PRI3_R&0x00FFFFFF; //priority zero
          NVIC_ST_CTRL_R =0x0007; //enables core clock with interrupts
    }

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Play called again
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
//           Units of period to be determined by YOU
//           Maximum period to be determined by YOU
//           Minimum period to be determined by YOU
//         if period equals zero, disable sound output
// Output: none
		
void Sound_Play(uint32_t period){     
//interrupts systic at rate specified by period
  if (period){
		NVIC_ST_RELOAD_R = period - 1;
    //NVIC_ST_CURRENT_R= 0;
    NVIC_ST_CTRL_R|=0x02; //enables the systick with the core clock 
  }else{
    NVIC_ST_CTRL_R &=~0x02; //if the period is zero disable the systick 
		DAC_Out(0);
  }
}


void SysTick_Handler(void)
{
  GPIO_PORTF_DATA_R ^= 0x02;                 //toggling LED for heartbeat
  DAC_Out(sineWave[wave_index]);
  wave_index++;
  wave_index = wave_index%0x1F; //masking index (5 bits to account for 32 elements)
}


void (*PeriodicTask0)(void);
void Timer0A_Init(void(*task)(void),uint32_t period){
	SYSCTL_RCGCTIMER_R |=0x01; // activate the timer
	PeriodicTask0 =task; //user function
	TIMER0_CTL_R = 0x00000000;
	TIMER0_CFG_R = 0x00000000;
	TIMER0_TAMR_R = 0x00000002;
	TIMER0_TAILR_R = period - 1;
	TIMER0_TAPR_R = 0;
	TIMER0_ICR_R = 0x00000001;
	TIMER0_IMR_R = 0x00000001;
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; //8) priority 4
	NVIC_EN0_R = 1<<19;	
	TIMER0_CTL_R = 0x00000001; 
}

void Timer0A_Handler(void){
		TIMER0_ICR_R = TIMER_ICR_TATOCINT; //
	(*PeriodicTask0)(); 
}

void Music_Song(void){
	struct State{
        uint16_t note;
        uint32_t duration;
};

typedef struct State stype_t;

stype_t song[22] = {{fsharp,quarter},{a,eighth},{b,eighth},{a,quarter},{fsharp,quarter},{a, half},
									{b,quarter},{a,eighth},{b,eighth},{d,quarter},{csharp,eighth},{d,eighth},{csharp,half},{csharp,eighth},{d,eighth},{csharp,half},
										{fsharp,half},{csharp, quarter},{d,quarter},{fsharp,half},{fsharp,quarter},{d,quarter}};
} 