// ******** PID.h **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/14/18
// Description: Header file for PID.c
// Lab Number: Lab10
// TA: Zee Lv
// Date of last revision: 11/14/18
// Hardware Configuration: N/A

#include <stdint.h>

#ifndef PID_H_
#define PID_H_

#define K_PROP_MAX 0.0025
#define K_PROP_MIN 0.0015
#define K_INT_MAX 0.0025
#define K_INT_MIN 0.0015

extern float k_Prop;
extern float k_Int;

void PID_Update(void);

#endif // PID_H_
