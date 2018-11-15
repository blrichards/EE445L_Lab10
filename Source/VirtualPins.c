#include "Blynk.h"
#include "Motor.h"
#include "Timer.h"
#include "UART.h"
#include "VirtualPins.h"

#define ANALOG_CLOCK_MODE 0
#define DIGITAL_CLOCK_MODE 1
#define BUTTON_PRESSED 1
#define BUTTON_RELEASED 0

FromBlynkHandler FromBlynkHandlers[NUM_VIRTUAL_PINS_FROM_BLYNK] = {0};
ToBlynkHandler ToBlynkHandlers[NUM_VIRTUAL_PINS_TO_BLYNK] = {0};

////////////////////////////
//  From Blynk Handlers   //
////////////////////////////

static void buttonZeroPressed(int32_t pressedReleased)
{
	if (pressedReleased == BUTTON_PRESSED)
		Motor_SpeedShouldUpdate(ButtonZeroPressed);
}

static void buttonOnePressed(int32_t pressedReleased)
{
	if (pressedReleased == BUTTON_PRESSED)
		Motor_SpeedShouldUpdate(ButtonOnePressed);
}


////////////////////////////
//   To Blynk Handlers    //
////////////////////////////
static void sendCurrentHours(void) 
{
	TM4C_to_Blynk(VP_CURRENT_HOURS, CurrentHours);
}

static void sendCurrentMinutes(void) 
{
	TM4C_to_Blynk(VP_CURRENT_MINUTES, CurrentMinutes);
}

static void sendCurrentSeconds(void) 
{
	TM4C_to_Blynk(VP_CURRENT_SECONDS, CurrentSeconds);
}

static void fromBlynkInit(void)
{
	// FromBlynkHandlers[VP_DISPLAY_MODE_SELECTOR] = setDisplayMode;
	FromBlynkHandlers[VP_FAR_LEFT_SWITCH] = buttonZeroPressed;
	FromBlynkHandlers[VP_FAR_RIGHT_SWITCH] = buttonOnePressed;
}

static void toBlynkInit(void)
{
	ToBlynkHandlers[VP_CURRENT_HOURS - TO_BLYNK_VIRTUAL_PIN_OFFSET] = sendCurrentHours;
	ToBlynkHandlers[VP_CURRENT_MINUTES - TO_BLYNK_VIRTUAL_PIN_OFFSET] = sendCurrentMinutes;
	ToBlynkHandlers[VP_CURRENT_SECONDS - TO_BLYNK_VIRTUAL_PIN_OFFSET] = sendCurrentSeconds;
}

void VirtualPins_Init(void)
{
	fromBlynkInit();
	toBlynkInit();
}
