// ******** Motor.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/2018
// Description: File for tachometer interface
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/2018
// Hardware Configuration: N/A

#include <stdint.h>
#include "Motor.h"
#include "Tachometer.h"

uint8_t TargetRPS = MINRPS;

void Motor_SpeedShouldUpdate(SpeedChange change)
{
	uint8_t CurrentRPS = Tach_GetSpeed();
	if(ButtonZeroPressed){
		CurrentRPS -= SPEEDCHANGE;
		TargetRPS = (CurrentRPS <= MINRPS) ? MINRPS : CurrentRPS;
	}
	else if (ButtonOnePressed){
		CurrentRPS += SPEEDCHANGE;
		TargetRPS = (CurrentRPS >= MAXRPS) ? MAXRPS : CurrentRPS;
	}
}
