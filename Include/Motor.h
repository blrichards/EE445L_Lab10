// ******** Motor.h **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/18
// Description: Header file for Motor.c
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/18
// Hardware Configuration: N/A

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

#define MAXRPS 1500		//0.1 resolution
#define MINRPS 500		//0.1 resolution
#define SPEEDCHANGE 50

extern uint16_t TargetRPS;

typedef enum {
	ButtonZeroPressed,
	ButtonOnePressed,
} SpeedChange;

/**
 * Handler for when external state such as button push or time change occurs
 * @param: reason: Reason for why the state changed.
 */
void Motor_SpeedShouldUpdate(SpeedChange change);

#endif // MOTOR_H_
