// ******** Tachometer.h **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/18
// Description: Header file for Tachometer.c
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/18
// Hardware Configuration: N/A

#ifndef TACHOMETER_H_
#define TACHOMETER_H_

#include <stdint.h>
#include <stdbool.h>

extern uint32_t CurrentPeriod;
extern bool PeriodOneCaptured;

void Tachometer_Init(void);
uint16_t Tach_GetSpeed(void);

#endif // TACHOMETER_H_
