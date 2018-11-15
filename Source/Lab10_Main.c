// ******** Lab10_Main.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 9/18/18
// Description: Main file for Lab 3
// Lab Number: Lab03
// TA: Zee Lv
// Date of last revision: 9/19/2018
// Hardware Configuration: N/A

#include "Blynk.h"
#include "Buttons.h"
#include "Debug.h"
#include "esp8266.h"
#include "Graphics.h"
#include "Motor.h"
#include "PID.h"
#include "PLL.h"
#include "PWM.h"
#include "ST7735.h"
#include "Tachometer.h"
#include "Timer.h"
#include "tm4c123gh6pm.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PF2 (*((volatile uint32_t*)0x40025010))
#define PF1 (*((volatile uint32_t*)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
long StartCritical(void); // previous I bit, disable interrupts
void EndCritical(long sr); // restore I bit to previous value
void WaitForInterrupt(void); // low power mode

int main(void)
{ 
 	PLL_Init(Bus80MHz);             // Setup PLL for 80 MHz
	DisableInterrupts();			// Disable Interrupts during INIT
  	Debug_Init();                 	// Initialize the LEDs
	Buttons_Init();
	PWM0A_Init(8000, 4000);			// 10kHz 50% duty cycle on PB6
	Tachometer_Init();
  	//UART2_Init();               	// Enable Debug Serial Port
  	//UART5_Init();               	// Enable ESP8266 Serial Port
  	ESP8266_Reset();				// Reset the WiFi module
  	Output_Init();		// Initialize the ST7735
  	//SetupWiFi();                	// Setup communications to Blynk Server
  	//Timer5_Init(&TM4C_to_Blynk, 80000000);  // TODO - Timer for Blynk Server Comm
  	EnableInterrupts();		// Enable Interrupts 
	ST7735_SetCursor(0,0);
	ST7735_OutString("EE445L Lab10");

    while (1) { 
		while(!PeriodOneCaptured){}
		Blynk_to_TM4C();              // Get serial data from ESP8266
		PID_Update();
		PF1 ^= 0x02; // toggles when running in main
		
		ST7735_SetCursor(0,1); 
		ST7735_OutString("Current RPS: ");
		ST7735_OutUDec(Tach_GetSpeed());
		ST7735_SetCursor(0,2); 
		ST7735_OutString("Target RPS: ");
		ST7735_OutUDec(TargetRPS);
		//TODO - NEED Graphics that will go through and output a line
		//of the TargetRPS and an output of the current RPS like Lab09
	}
}

