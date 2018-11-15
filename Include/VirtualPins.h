// ******** VirtualPins.h **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 9/18/18
// Description: Defines virtual pin numbers and their uses.
// Lab Number: Lab04
// TA: Zee Lv
// Date of last revision: 9/19/2018
// Hardware Configuration: N/A

#include <stdint.h>

#ifndef VIRTUAL_PINS_H_
#define VIRTUAL_PINS_H_

// From Blynk
#define NUM_VIRTUAL_PINS_FROM_BLYNK 3
#define VP_DESIRED_MOTOR_SPEED	 	0
#define VP_KP			 			1
#define VP_KI				 		2

typedef void(*FromBlynkHandler)(float);

void VirtualPins_Init(void);

extern FromBlynkHandler FromBlynkHandlers[NUM_VIRTUAL_PINS_FROM_BLYNK];

#endif // VIRTUAL_PINS_H_
