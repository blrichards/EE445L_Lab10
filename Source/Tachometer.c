// ******** Tachometer.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/2018
// Description: File for tachometer interface
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/2018
// Hardware Configuration: N/A

#include <stdbool.h>
#include <stdint.h>
#include "Motor.h"
#include "Tachometer.h"
#include "tm4c123gh6pm.h"

uint32_t CurrentPeriod = 0;
bool PeriodOneCaptured = false;
int32_t Overflow;
uint32_t static First;        // Timer0A first edge, 12.5 ns units

//COULD BE A BUG HERE
/*
void Tachometer_Init(void)
{
	SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
	SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B
	CurrentPeriod = 0xFFFFFFFF;      // first will be wrong
	PeriodOneCaptured = false;       // set on subsequent
	GPIO_PORTB_DIR_R &= ~0x80;       // make PB7 input
	GPIO_PORTB_AFSEL_R |= 0x80;      // enable alt funct on PB7
	GPIO_PORTB_DEN_R |= 0x80;        // configure PB7 as T0CCP0
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0x0FFFFFFF) + 0x70000000;
	TIMER0_CTL_R &= ~0x00000001;     // disable timer0A during setup
	TIMER0_CFG_R = 0x00000004;       // configure for 16-bit capture mode
	TIMER0_TAMR_R = 0x00000007;      // configure for rising edge event
	TIMER0_CTL_R &= ~0x0000000C;     // rising edge
	TIMER0_TAILR_R = 0x0000FFFF;     // start value
	TIMER0_TAPR_R = 0xFF;            // activate prescale, creating 24-bit 
	TIMER0_IMR_R |= 0x00000004;      // enable capture match interrupt
	TIMER0_ICR_R = 0x00000004;       // clear timer0A capture match flag
	TIMER0_CTL_R |= 0x00000001;      // timer0A 24-b, +edge, interrupts
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; //Timer0A=priority 2
	NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}
*/

void Tachometer_Init(void)
{

	SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
	SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B
	First = 0;                       // first will be wrong
	PeriodOneCaptured = false;       // set on subsequent
	GPIO_PORTB_DIR_R &= ~0x40;       // make PB6 input
	GPIO_PORTB_AFSEL_R |= 0x40;      // enable alt funct on PB6
	GPIO_PORTB_DEN_R |= 0x40;        // configure PB6 as T0CCP0
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xF0FFFFFF)+0x07000000;
	TIMER0_CTL_R &= ~0x00000001;     // disable timer0A during setup
	TIMER0_CFG_R = 0x00000004;       // configure for 16-bit capture mode
	TIMER0_TAMR_R = 0x00000007;      // configure for rising edge event
	TIMER0_CTL_R &= ~0x0000000C;     // rising edge
	TIMER0_TAILR_R = 0x0000FFFF;     // start value
	TIMER0_TAPR_R = 0xFF;            // activate prescale, creating 24-bit 
	TIMER0_IMR_R |= 0x00000004;      // enable capture match interrupt
	TIMER0_ICR_R = 0x00000004;       // clear timer0A capture match flag
	TIMER0_CTL_R |= 0x00000001;      // timer0A 24-b, +edge, interrupts
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; //Timer0A=priority 2
	NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}


void Timer0A_Handler(void)
{
	
	TIMER0_ICR_R = 0x00000004;       // acknowledge timer0A
	GPIO_PORTF_DATA_R ^= 0x08;			// heartbeat
	CurrentPeriod = (First - TIMER0_TAR_R) & 0x00FFFFFF;
	if(CurrentPeriod > 1400000) 
		CurrentPeriod = 1400000;
	// 24-bit, 12.5ns resolution
	First = TIMER0_TAR_R;            // setup for next
	PeriodOneCaptured = true;                        // set semaphore
};

uint16_t Tach_GetSpeed(void) {
	uint16_t currentSpeed =  200000000/CurrentPeriod;
	if(currentSpeed > MAXRPS) return MAXRPS;
	if(currentSpeed < MINRPS) return MINRPS;
	return currentSpeed; 				// 0.1RPS resolution
}
