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

float k_Prop = 0.0015;
float k_Int = 0.0015;
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

	if( totalError <= MINRPS ) totalError = MAXRPS;					//Saturate the total errors to make sure there are no bad values
	if( totalError > MAXRPS) totalError = MAXRPS;
	
	PWM0B_Duty(40000 * (((float)(totalError)) / (float)(MAXRPS)));
}

//Valvano's code
/*
void Timer2A_Handler(void){
  TIMER2_ICR_R = 0x01;      // acknowledge timer2A
  Speed = 200000000/Period; // 0.1 rps
  E = Xstar-Speed;          // 0.1 rps
  U = U+(3*E)/64;           // discrete integral
  if(U < 100) U=100;        // Constrain output
  if(U>39900) U=39900;      // 100 to 39900
  PWM0B_Duty(U);            // output 
}
*/
