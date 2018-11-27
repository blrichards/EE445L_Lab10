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

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

uint16_t TargetRPS = MAXRPS;

void Motor_SpeedShouldUpdate(SpeedChange change)
{
	switch (change) {
	case ButtonZeroPressed:
		TargetRPS = MIN(MAX(TargetRPS - SPEEDCHANGE, MINRPS), MAXRPS);
		break;
	case ButtonOnePressed:
		TargetRPS = MAX(MIN(TargetRPS + SPEEDCHANGE, MAXRPS), MINRPS);
		break;
	}
	if (TargetRPS > MAXRPS)
		TargetRPS = MAXRPS;
	if (TargetRPS < MINRPS)
		TargetRPS = MINRPS;
}
