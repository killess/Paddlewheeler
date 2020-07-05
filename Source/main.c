//------------------------------------------------------------------------------
// Program Name:    HughesBoat
// Program Number:  H003
// Program Date:    8/11/19
// Project:
// Board:
// Author:          Greg Hughes
// IDE:             TrueStudio v9.3
// Microcontroller: STMicroelectronics
//                  STM32F446RE
//
// Description:
//
//
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
#include "stm32f4xx.h"
#include "system.h"
#include "rtos.h"

#include "pwm.h"



int main(void)
{
	system_init();


	pwm_init();

	rtos_start();

  while(1)
  {}
}


