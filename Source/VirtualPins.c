#include "Blynk.h"
#include "Motor.h"
#include "PID.h"
#include "Timer.h"
#include "UART.h"
#include "VirtualPins.h"

#define ANALOG_CLOCK_MODE 0
#define DIGITAL_CLOCK_MODE 1
#define BUTTON_PRESSED 1
#define BUTTON_RELEASED 0

FromBlynkHandler FromBlynkHandlers[NUM_VIRTUAL_PINS_FROM_BLYNK] = {0};

////////////////////////////
//  From Blynk Handlers   //
////////////////////////////

static void updateDesiredMotorSpeed(float speed)
{
	if (speed >= MINRPS && speed <= MAXRPS)
		TargetRPS = (uint16_t)speed;
}

static void updateKI(float ki)
{
	ki = ki / 1000;
	if (ki >= K_INT_MIN && ki <= K_INT_MAX)
		k_Int = ki;
}

static void updateKP(float kp)
{
	kp = kp / 1000;
	if (kp >= K_PROP_MIN && kp <= K_PROP_MAX)
		k_Prop = kp;
}

static void fromBlynkInit(void)
{
	FromBlynkHandlers[VP_DESIRED_MOTOR_SPEED] = updateDesiredMotorSpeed;
	FromBlynkHandlers[VP_KP] = updateKP;
	FromBlynkHandlers[VP_KI] = updateKI;
}

void VirtualPins_Init(void)
{
	fromBlynkInit();
}
