// ******** Buttons.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 9/18/18
// Description: File for button functionality
// Lab Number: Lab03
// TA: Zee Lv
// Date of last revision: 9/19/2018
// Hardware Configuration: N/A

#include "Buttons.h"
#include "Motor.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

#define FAR_LEFT_SWITCH 0x2
#define FAR_RIGHT_SWITCH 0x1
#define PF2 (*((volatile uint32_t*)0x40025010))

void Buttons_Init(void)
{
    volatile uint32_t delay;
    SYSCTL_RCGCGPIO_R |= 0x00000002; // 1) activate clock for Port B
    delay = SYSCTL_RCGCGPIO_R; // allow time for clock to start
    GPIO_PORTB_DIR_R &= ~0x03; // Set dir to input 0-1
    GPIO_PORTB_AFSEL_R &= ~0x03; // disable alt funct
    GPIO_PORTB_DEN_R |= 0x03; //     enable digital I/O
    GPIO_PORTB_PCTL_R &= ~0x00030000; // configure PF4 as GPIO
    GPIO_PORTB_AMSEL_R = 0; //     disable analog functionality on PF
    GPIO_PORTB_IS_R &= ~0x03; // is edge-sensitive
    GPIO_PORTB_IBE_R &= ~0x03; // PF4 is not both edges
    GPIO_PORTB_IEV_R |= ~0x03; // PF4 rising edge event
    GPIO_PORTB_ICR_R = 0x03; // clear flag
    GPIO_PORTB_IM_R |= 0x03; // arm interrupt on PF4 *** No IME bit as mentioned in Book ***
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF00) | 0x000000E0; // (g) priority 5
    NVIC_EN0_R = 0x00000002; // (h) enable interrupt 30 in NVIC
}

void GPIOPortB_Handler(void)
{
    uint8_t flags = (GPIO_PORTB_MIS_R & 0x03);
    GPIO_PORTB_ICR_R = 0x03; // acknowledge flag

	switch(flags){
		case FAR_LEFT_SWITCH:
			Motor_SpeedShouldUpdate(ButtonZeroPressed);
			break;
		case FAR_RIGHT_SWITCH:	
			Motor_SpeedShouldUpdate(ButtonOnePressed);
			break;
		default:	//multiple buttons pressed, dont do anything
			break;
	}
    PF2 ^= 0x04;
}
