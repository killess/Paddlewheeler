#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_


#define PWM_MAX          2000 // 2.0ms
#define PWM_CTR          1500 // 1.5ms
#define PWM_MIN          1000 // 1.0ms

#define PWM_PRESCALAR    90
#define PWM_PERIOD       16000 // us

#define PWM_TIM_OC       TIM1
#define PWM_TIM_IC14     TIM3
#define PWM_TIM_IC58     TIM4

#define PWM_CH1_Pin GPIO_PIN_8
#define PWM_CH1_GPIO_Port GPIOA
#define PWM_CH2_Pin GPIO_PIN_9
#define PWM_CH2_GPIO_Port GPIOA
#define PWM_CH3_Pin GPIO_PIN_10
#define PWM_CH3_GPIO_Port GPIOA
#define PWM_CH4_Pin GPIO_PIN_11
#define PWM_CH4_GPIO_Port GPIOA
#define PWM_CH5_Pin
#define PWM_CH5_GPIO_Port
#define PWM_CH6_Pin
#define PWM_CH6_GPIO_Port
#define PWM_CH7_Pin
#define PWM_CH7_GPIO_Port
#define PWM_CH8_Pin
#define PWM_CH8_GPIO_Port

#define PWMI_CH1_Pin GPIO_PIN_6
#define PWMI_CH1_GPIO_Port GPIOA
#define PWMI_CH2_Pin GPIO_PIN_7
#define PWMI_CH2_GPIO_Port GPIOA
#define PWMI_CH3_Pin GPIO_PIN_0
#define PWMI_CH3_GPIO_Port GPIOB
#define PWMI_CH4_Pin GPIO_PIN_1
#define PWMI_CH4_GPIO_Port GPIOB
#define PWMI_CH5_Pin GPIO_PIN_6
#define PWMI_CH5_GPIO_Port GPIOB
#define PWMI_CH6_Pin GPIO_PIN_7
#define PWMI_CH6_GPIO_Port GPIOB
#define PWMI_CH7_Pin GPIO_PIN_8
#define PWMI_CH7_GPIO_Port GPIOB
#define PWMI_CH8_Pin GPIO_PIN_9
#define PWMI_CH8_GPIO_Port GPIOB

#define PWMI_CH1 PWMI_CH1_GPIO_Port,PWMI_CH1_Pin
#define PWMI_CH2 PWMI_CH2_GPIO_Port,PWMI_CH2_Pin
#define PWMI_CH3 PWMI_CH3_GPIO_Port,PWMI_CH3_Pin
#define PWMI_CH4 PWMI_CH4_GPIO_Port,PWMI_CH4_Pin
#define PWMI_CH5 PWMI_CH5_GPIO_Port,PWMI_CH5_Pin
#define PWMI_CH6 PWMI_CH6_GPIO_Port,PWMI_CH6_Pin
#define PWMI_CH7 PWMI_CH7_GPIO_Port,PWMI_CH7_Pin
#define PWMI_CH8 PWMI_CH8_GPIO_Port,PWMI_CH8_Pin


typedef struct pwmChannel
{
	uint16_t risingEdge;
	uint16_t risingEdge_prev;
	uint16_t fallingEdge;
	uint16_t period;
	uint16_t dutycycle;
}pwmch_t;

typedef struct pwmTim
{
	TIM_HandleTypeDef htimOC;
	TIM_HandleTypeDef htimIC14;
	TIM_HandleTypeDef htimIC58;
}pwmtim_t;

typedef struct pwmData
{
	pwmch_t ch1;
	pwmch_t ch2;
	pwmch_t ch3;
	pwmch_t ch4;
	pwmch_t ch5;
	pwmch_t ch6;
	pwmch_t ch7;
	pwmch_t ch8;

}pwm_t;






//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
void pwm_init(void);
void pwmi_14_callback(TIM_HandleTypeDef *htim);
void pwmi_58_callback(TIM_HandleTypeDef *htim);


#endif /* INCLUDE_PWM_H_ */
