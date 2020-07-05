//------------------------------------------------------------------------------
// Description:
//
//   Callback functions
//
//
//   Subroutines:
//
//
//   Functions:
//     None
//
//   Interrupts:
//     None
//
//
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
//  Includes Files
//------------------------------------------------------------------------------
#include "stm32f4xx.h"
#include "callback.h"
#include "pwm.h"



//------------------------------------------------------------------------------
// Task Name      : HAL_TIM_IC_CaptureCallback
// Description    : Initialize PWMs
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		pwmi_14_callback(htim);
	}

	if(htim->Instance == TIM4)
	{
		pwmi_58_callback(htim);
	}

}


