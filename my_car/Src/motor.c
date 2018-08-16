#include "motor.h"

uint8_t encoder_state_L = 0;
uint8_t encoder_state_R = 0;
uint32_t encoder_count ;

uint8_t motor_get_encoder_state_L()
{
    return encoder_state_L;
}

void motor_toggle_encoder_state_L()
{
    if (encoder_state_L == 0)   encoder_state_L = 1;
    else encoder_state_L = 0;
}
uint8_t motor_get_encoder_state_R()
{
    return encoder_state_R;
}
void motor_toggle_encoder_state_R()
{
    if (encoder_state_R == 0)   encoder_state_R = 1;
    else encoder_state_R = 0;
}
void motor_run(uint16_t left_speed_pwm,uint16_t right_speed_pwm)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET); //LED on the Nucleo board
    //Motor control
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    TIM3-> CCR4 = 500;  // ¼Ä´æÆ÷Ö±½ÓÐ´
    TIM4-> CCR4 = 500;  // ¼Ä´æÆ÷Ö±½ÓÐ´


}

void motor_stop()
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
}
