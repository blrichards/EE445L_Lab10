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
#include "VirtualPins.h"
#include "tm4c123gh6pm.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t displayCursor = 0;
int16_t oldTargetY[ST7735_TFTWIDTH];
int16_t oldCurrentY[ST7735_TFTWIDTH];

#define TEXT_DISPLAY_OFFSET 40
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define PF2 (*((volatile uint32_t*)0x40025010))
#define PF1 (*((volatile uint32_t*)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
long StartCritical(void); // previous I bit, disable interrupts
void EndCritical(long sr); // restore I bit to previous value
void WaitForInterrupt(void); // low power mode

static int16_t interpolateRPS(int16_t value)
{
	static const int16_t newMin = ST7735_TFTHEIGHT;
	static const int16_t newMax = TEXT_DISPLAY_OFFSET;
	static const int16_t newRange = newMax - newMin;
	static const int16_t oldMin = MINRPS;
	static const int16_t oldMax = MAXRPS;
	static const int16_t oldRange = oldMax - oldMin;
	
	value = MIN(MAX(value, oldMin), oldMax);
	return (((value - oldMin) * newRange) / oldRange) + newMin;
}

static void updatePID()
{
	PID_Update();
	PF1 ^= 0x02; // toggles when running in main
}

static void collectFromBlynkAndUpdateDisplay()
{
	uint16_t current = Tach_GetSpeed();
	
	Blynk_to_TM4C();
	ST7735_SetCursor(0,1); 
	ST7735_OutString("Target RPS: ");
	ST7735_OutUDec(TargetRPS / 10);			//0.1 resolution
	ST7735_OutChar('.');
	ST7735_OutUDec(TargetRPS % 10);
	ST7735_SetCursor(0,2); 
	ST7735_OutString("Current RPS: ");
	ST7735_OutUDec(current / 10);			//0.1 resolution
	ST7735_OutChar('.');
	ST7735_OutUDec(current % 10);
	
	// Clear old points.
	ST7735_DrawPixel(displayCursor, oldTargetY[displayCursor], ST7735_BLACK);
	ST7735_DrawPixel(displayCursor, oldCurrentY[displayCursor], ST7735_BLACK);
	
	// Set new.
	oldTargetY[displayCursor] = interpolateRPS(TargetRPS);
	oldCurrentY[displayCursor] = interpolateRPS(current);
	
	// Draw new.
	ST7735_DrawPixel(displayCursor, oldTargetY[displayCursor], ST7735_BURNT_ORANGE);
	ST7735_DrawPixel(displayCursor, oldCurrentY[displayCursor], ST7735_WHITE);
	
	displayCursor = (displayCursor + 1) % ST7735_TFTWIDTH;
	
	updatePID();
}

static void staticsInit(void)
{
	const uint16_t bottomPixel = interpolateRPS(0);
	for (size_t i = 0; i < ST7735_TFTWIDTH; ++i) {
		oldCurrentY[i] = bottomPixel;
		oldTargetY[i] = bottomPixel;
	}
}

int main(void)
{ 
	staticsInit();
	
 	PLL_Init(Bus80MHz);             // Setup PLL for 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x33; 		// activate port F, E, B, and A
	while ((SYSCTL_PRGPIO_R & 0x33) != 0x33) {}
	DisableInterrupts();			// Disable Interrupts during INIT
  	Output_Init();					// Initialize the ST7735
	ST7735_SetCursor(0,0);
	ST7735_OutString("EE445L Lab10");
  	Debug_Init();                 	// Initialize the LEDs
	Buttons_Init();
	PWM0B_Init(40000, 17000);		// 10kHz 50% duty cycle on PB6
	Tachometer_Init();
	VirtualPins_Init();
  	//UART2_Init();               	// Enable Debug Serial Port
  	//UART5_Init();               	// Enable ESP8266 Serial Port
	ESP8266_Init();
  	ESP8266_Reset();				// Reset the WiFi module
	ESP8266_SetupWiFi();

	// check for receive data from Blynk App every 10ms
	Timer2_Init(&collectFromBlynkAndUpdateDisplay, 799999); 
	
	// update PID every 10ms
	// Timer3_Init(&updatePID, 799999);
	
	
  	EnableInterrupts();				// Enable Interrupts 
	
	while(!PeriodOneCaptured) {}

    while (true) {}
}

