// ******** PID.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/2018
// Description: File for tachometer interface
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/2018
// Hardware Configuration: N/A

#include <stdint.h>
#include "Motor.h"
#include "PID.h"
#include "PWM.h"
#include "Tachometer.h"

float k_Prop = 0.00125;
float k_Int = 0.00125;
float totalIntegralError = 0;
float motorSpeed = 0;
float speedInput = 0;
float currentError = 0;
float propError = 0;
float totalError = 0;

/* 	NAME: PILoop
	DESCRIPTION: Implaments the PI Loop in the code
	INPUT: none
	OUTPUT: none
*/
void PID_Update (void)
{
	speedInput = TargetRPS;						
	motorSpeed = Tach_GetSpeed();								


	if(motorSpeed > MAXRPS) motorSpeed = MAXRPS;			// Saturate the motor speed if too high or too low
	if(motorSpeed < MINRPS) motorSpeed = MINRPS;

	currentError = speedInput - motorSpeed;						// Get the error from the input speed and motor speed
	
	propError = k_Prop * currentError;								// Proporional part of the PI loop
	totalIntegralError += (k_Int * currentError);					// Integral part of the PI loop

	if(totalIntegralError > MAXRPS) totalIntegralError = MAXRPS;		//Saturate the two errors to make sure there are no values too extreme
	if(totalIntegralError < 0) totalIntegralError = 0;
	if(propError > MAXRPS) propError = MAXRPS;
	if(propError < MINRPS) propError = MINRPS;
	
	totalError = propError + totalIntegralError;

	if( totalError < 0 ) totalError = 0;					//Saturate the total errors to make sure there are no bad values
	if( totalError > MAXRPS) totalError = MAXRPS;
	
	PWM0A_Duty(8000 * (((float)(totalError)) / (float)(MAXRPS)));
}