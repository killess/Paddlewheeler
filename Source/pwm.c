//------------------------------------------------------------------------------
// Description:
//
//   Pulse width modulation (PWM)
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
#include "pwm.h"




//------------------------------------------------------------------------------
//  External
//------------------------------------------------------------------------------
pwmtim_t pwmtim;


//------------------------------------------------------------------------------
//  Private
//------------------------------------------------------------------------------
static __IO pwm_t pwm;

static void pwm_gpio_init(void);
static void pwm_timOC_init(void);
static void pwm_timIC14_init(void);
static void pwm_timIC58_init(void);


//------------------------------------------------------------------------------
// Task Name      : pwm_init
// Description    : Initialize PWMs
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwm_init(void)
{
	pwm_gpio_init();
	pwm_timOC_init();
	pwm_timIC14_init();
	pwm_timIC58_init();
}


//------------------------------------------------------------------------------
// Task Name      : pwm_gpio_init
// Description    : Initialize PWM gpio pins
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwm_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// Clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// PWM Output
	GPIO_InitStruct.Pin = PWM_CH1_Pin|PWM_CH2_Pin|PWM_CH3_Pin|PWM_CH4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// PWM Input
	GPIO_InitStruct.Pin = PWMI_CH1_Pin|PWMI_CH2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = PWMI_CH3_Pin|PWMI_CH4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//------------------------------------------------------------------------------
// Task Name      : pwm_init
// Description    : Initialize PWMs
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwm_timOC_init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};


	// Clocks
	__HAL_RCC_TIM1_CLK_ENABLE();

	// TIM
	pwmtim.htimOC.Instance = PWM_TIM_OC;
	pwmtim.htimOC.Init.Prescaler = PWM_PRESCALAR-1;
	pwmtim.htimOC.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwmtim.htimOC.Init.Period = PWM_PERIOD-1;
	pwmtim.htimOC.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwmtim.htimOC.Init.RepetitionCounter = 0;
	pwmtim.htimOC.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&pwmtim.htimOC);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&pwmtim.htimOC, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&pwmtim.htimOC);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&pwmtim.htimOC, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	sConfigOC.Pulse = PWM_CTR;
	HAL_TIM_PWM_ConfigChannel(&pwmtim.htimOC, &sConfigOC, TIM_CHANNEL_1);

	sConfigOC.Pulse = PWM_CTR;
	HAL_TIM_PWM_ConfigChannel(&pwmtim.htimOC, &sConfigOC, TIM_CHANNEL_2);

	sConfigOC.Pulse = PWM_CTR;
	HAL_TIM_PWM_ConfigChannel(&pwmtim.htimOC, &sConfigOC, TIM_CHANNEL_3);

	sConfigOC.Pulse = PWM_CTR;
	HAL_TIM_PWM_ConfigChannel(&pwmtim.htimOC, &sConfigOC, TIM_CHANNEL_4);

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&pwmtim.htimOC, &sBreakDeadTimeConfig);

	HAL_TIM_PWM_Start(&pwmtim.htimOC,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwmtim.htimOC,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwmtim.htimOC,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&pwmtim.htimOC,TIM_CHANNEL_4);
}


//------------------------------------------------------------------------------
// Task Name      : pwm_timIC14_init
// Description    : Initialize PWMs
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwm_timIC14_init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	// Clocks
	__HAL_RCC_TIM3_CLK_ENABLE();

	// TIM
	pwmtim.htimIC14.Instance = PWM_TIM_IC14;
	pwmtim.htimIC14.Init.Prescaler = 90-1;
	pwmtim.htimIC14.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwmtim.htimIC14.Init.Period = 0xFFFF;
	pwmtim.htimIC14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwmtim.htimIC14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&pwmtim.htimIC14);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&pwmtim.htimIC14, &sClockSourceConfig);

	HAL_TIM_IC_Init(&pwmtim.htimIC14);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&pwmtim.htimIC14, &sMasterConfig);

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC14, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC14, &sConfigIC, TIM_CHANNEL_2);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC14, &sConfigIC, TIM_CHANNEL_3);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC14, &sConfigIC, TIM_CHANNEL_4);

	// NVIC
    HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_TIM_IC_Start_IT(&pwmtim.htimIC14,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC14,TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC14,TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC14,TIM_CHANNEL_4);
}


//------------------------------------------------------------------------------
// Task Name      : pwm_timIC58_init
// Description    : Initialize PWMs
// Input          : None
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwm_timIC58_init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	// Clocks
	__HAL_RCC_TIM3_CLK_ENABLE();

	// TIM
	pwmtim.htimIC58.Instance = PWM_TIM_IC58;
	pwmtim.htimIC58.Init.Prescaler = 90-1;
	pwmtim.htimIC58.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwmtim.htimIC58.Init.Period = 0xFFFF;
	pwmtim.htimIC58.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwmtim.htimIC58.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&pwmtim.htimIC58);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&pwmtim.htimIC58, &sClockSourceConfig);

	HAL_TIM_IC_Init(&pwmtim.htimIC58);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&pwmtim.htimIC58, &sMasterConfig);

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC58, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC58, &sConfigIC, TIM_CHANNEL_2);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC58, &sConfigIC, TIM_CHANNEL_3);
	HAL_TIM_IC_ConfigChannel(&pwmtim.htimIC58, &sConfigIC, TIM_CHANNEL_4);

	// NVIC
    HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    HAL_TIM_IC_Start_IT(&pwmtim.htimIC58,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC58,TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC58,TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(&pwmtim.htimIC58,TIM_CHANNEL_4);
}


//------------------------------------------------------------------------------
// Task Name      : pwmi_14_callback
// Description    : Interrupt callback to calculate period and duty cycle for channels 1-4
// Input          : TIM_HandleTypeDef
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwmi_14_callback(TIM_HandleTypeDef *htim)
{
	GPIO_PinState pinstate;

	switch(htim->Channel)
	{
	case HAL_TIM_ACTIVE_CHANNEL_1:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH1);
		if(pinstate)
		{// High
			pwm.ch1.risingEdge = htim->Instance->CCR1;
			if(pwm.ch1.risingEdge > pwm.ch1.risingEdge_prev)
				pwm.ch1.period = pwm.ch1.risingEdge - pwm.ch1.risingEdge_prev;
			else
				pwm.ch1.period = 0xFFFF - (pwm.ch1.risingEdge_prev - pwm.ch1.risingEdge);
			pwm.ch1.risingEdge_prev = pwm.ch1.risingEdge;
		}
		else
		{// Low
			pwm.ch1.fallingEdge = htim->Instance->CCR1;
			if(pwm.ch1.fallingEdge > pwm.ch1.risingEdge_prev)
				pwm.ch1.dutycycle = pwm.ch1.fallingEdge - pwm.ch1.risingEdge_prev;
			else
				pwm.ch1.dutycycle = 0xFFFF - (pwm.ch1.risingEdge_prev - pwm.ch1.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_2:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH2);
		if(pinstate)
		{// High
			pwm.ch2.risingEdge = htim->Instance->CCR3;
			if(pwm.ch2.risingEdge > pwm.ch2.risingEdge_prev)
				pwm.ch2.period = pwm.ch2.risingEdge - pwm.ch2.risingEdge_prev;
			else
				pwm.ch2.period = 0xFFFF - (pwm.ch2.risingEdge_prev - pwm.ch2.risingEdge);
			pwm.ch2.risingEdge_prev = pwm.ch2.risingEdge;
		}
		else
		{// Low
			pwm.ch2.fallingEdge = htim->Instance->CCR3;
			if(pwm.ch2.fallingEdge > pwm.ch2.risingEdge_prev)
				pwm.ch2.dutycycle = pwm.ch2.fallingEdge - pwm.ch2.risingEdge_prev;
			else
				pwm.ch2.dutycycle = 0xFFFF - (pwm.ch2.risingEdge_prev - pwm.ch2.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_3:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH3);
		if(pinstate)
		{// High
			pwm.ch3.risingEdge = htim->Instance->CCR3;
			if(pwm.ch3.risingEdge > pwm.ch3.risingEdge_prev)
				pwm.ch3.period = pwm.ch3.risingEdge - pwm.ch3.risingEdge_prev;
			else
				pwm.ch3.period = 0xFFFF - (pwm.ch3.risingEdge_prev - pwm.ch3.risingEdge);
			pwm.ch3.risingEdge_prev = pwm.ch3.risingEdge;
		}
		else
		{// Low
			pwm.ch3.fallingEdge = htim->Instance->CCR3;
			if(pwm.ch3.fallingEdge > pwm.ch3.risingEdge_prev)
				pwm.ch3.dutycycle = pwm.ch3.fallingEdge - pwm.ch3.risingEdge_prev;
			else
				pwm.ch3.dutycycle = 0xFFFF - (pwm.ch3.risingEdge_prev - pwm.ch3.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_4:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH4);
		if(pinstate)
		{// High
			pwm.ch4.risingEdge = htim->Instance->CCR4;
			if(pwm.ch4.risingEdge > pwm.ch4.risingEdge_prev)
				pwm.ch4.period = pwm.ch4.risingEdge - pwm.ch4.risingEdge_prev;
			else
				pwm.ch4.period = 0xFFFF - (pwm.ch4.risingEdge_prev - pwm.ch4.risingEdge);
			pwm.ch4.risingEdge_prev = pwm.ch4.risingEdge;
		}
		else
		{// Low
			pwm.ch4.fallingEdge = htim->Instance->CCR4;
			if(pwm.ch4.fallingEdge > pwm.ch4.risingEdge_prev)
				pwm.ch4.dutycycle = pwm.ch4.fallingEdge - pwm.ch4.risingEdge_prev;
			else
				pwm.ch4.dutycycle = 0xFFFF - (pwm.ch4.risingEdge_prev - pwm.ch4.fallingEdge);
		}
		break;
	case HAL_TIM_ACTIVE_CHANNEL_CLEARED:
		break;
	}


}


//------------------------------------------------------------------------------
// Task Name      : pwmi_58_callback
// Description    : Interrupt callback to calculate period and duty cycle for channels 5-8
// Input          : TIM_HandleTypeDef
// Output         : None
// Return         : None
//------------------------------------------------------------------------------
void pwmi_58_callback(TIM_HandleTypeDef *htim)
{
	GPIO_PinState pinstate;

	switch(htim->Channel)
	{
	case HAL_TIM_ACTIVE_CHANNEL_1:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH5);
		if(pinstate)
		{// High
			pwm.ch5.risingEdge = htim->Instance->CCR1;
			if(pwm.ch5.risingEdge > pwm.ch5.risingEdge_prev)
				pwm.ch5.period = pwm.ch5.risingEdge - pwm.ch5.risingEdge_prev;
			else
				pwm.ch5.period = 0xFFFF - (pwm.ch5.risingEdge_prev - pwm.ch5.risingEdge);
			pwm.ch5.risingEdge_prev = pwm.ch5.risingEdge;
		}
		else
		{// Low
			pwm.ch5.fallingEdge = htim->Instance->CCR1;
			if(pwm.ch5.fallingEdge > pwm.ch5.risingEdge_prev)
				pwm.ch5.dutycycle = pwm.ch5.fallingEdge - pwm.ch5.risingEdge_prev;
			else
				pwm.ch5.dutycycle = 0xFFFF - (pwm.ch5.risingEdge_prev - pwm.ch5.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_2:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH6);
		if(pinstate)
		{// High
			pwm.ch6.risingEdge = htim->Instance->CCR2;
			if(pwm.ch6.risingEdge > pwm.ch6.risingEdge_prev)
				pwm.ch6.period = pwm.ch6.risingEdge - pwm.ch6.risingEdge_prev;
			else
				pwm.ch6.period = 0xFFFF - (pwm.ch6.risingEdge_prev - pwm.ch6.risingEdge);
			pwm.ch6.risingEdge_prev = pwm.ch6.risingEdge;
		}
		else
		{// Low
			pwm.ch6.fallingEdge = htim->Instance->CCR2;
			if(pwm.ch6.fallingEdge > pwm.ch6.risingEdge_prev)
				pwm.ch6.dutycycle = pwm.ch6.fallingEdge - pwm.ch6.risingEdge_prev;
			else
				pwm.ch6.dutycycle = 0xFFFF - (pwm.ch6.risingEdge_prev - pwm.ch6.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_3:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH7);
		if(pinstate)
		{// High
			pwm.ch7.risingEdge = htim->Instance->CCR3;
			if(pwm.ch7.risingEdge > pwm.ch7.risingEdge_prev)
				pwm.ch7.period = pwm.ch7.risingEdge - pwm.ch7.risingEdge_prev;
			else
				pwm.ch7.period = 0xFFFF - (pwm.ch7.risingEdge_prev - pwm.ch7.risingEdge);
			pwm.ch7.risingEdge_prev = pwm.ch7.risingEdge;
		}
		else
		{// Low
			pwm.ch7.fallingEdge = htim->Instance->CCR3;
			if(pwm.ch7.fallingEdge > pwm.ch7.risingEdge_prev)
				pwm.ch7.dutycycle = pwm.ch7.fallingEdge - pwm.ch7.risingEdge_prev;
			else
				pwm.ch7.dutycycle = 0xFFFF - (pwm.ch7.risingEdge_prev - pwm.ch7.fallingEdge);
		}
		break;

	case HAL_TIM_ACTIVE_CHANNEL_4:
		pinstate = HAL_GPIO_ReadPin(PWMI_CH8);
		if(pinstate)
		{// High
			pwm.ch8.risingEdge = htim->Instance->CCR4;
			if(pwm.ch8.risingEdge > pwm.ch8.risingEdge_prev)
				pwm.ch8.period = pwm.ch8.risingEdge - pwm.ch8.risingEdge_prev;
			else
				pwm.ch8.period = 0xFFFF - (pwm.ch8.risingEdge_prev - pwm.ch8.risingEdge);
			pwm.ch8.risingEdge_prev = pwm.ch8.risingEdge;
		}
		else
		{// Low
			pwm.ch8.fallingEdge = htim->Instance->CCR4;
			if(pwm.ch8.fallingEdge > pwm.ch8.risingEdge_prev)
				pwm.ch8.dutycycle = pwm.ch8.fallingEdge - pwm.ch8.risingEdge_prev;
			else
				pwm.ch8.dutycycle = 0xFFFF - (pwm.ch8.risingEdge_prev - pwm.ch8.fallingEdge);
		}
		break;
	case HAL_TIM_ACTIVE_CHANNEL_CLEARED:
		break;
	}


}




