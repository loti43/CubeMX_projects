#include "motor.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"


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
void motor_run(uint16_t left_speed_pwm,int a,uint16_t right_speed_pwm,int b)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET); //LED on the Nucleo board
    //Motor control
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    if(a > 0){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
        TIM3-> CCR4 = left_speed_pwm;  // 寄存器
    }
    if(a <= 0){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
        //  htim3.Init.Period = 1999; 2000 为设置的周期+1
        TIM3-> CCR4 = (2000 - left_speed_pwm)/2;  // 寄存器直接写
    }

    if(b > 0){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
        TIM4-> CCR4 = right_speed_pwm;  // 寄存器直接写
    }
    if(b <= 0){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
        //  htim3.Init.Period = 1999; 2000 为设置的周期+1
        TIM4-> CCR4 = (2000 - right_speed_pwm)/2;  // 寄存器直接写
    }

}

void motor_bluetooth_ctrl()
{
    int i;
    uint16_t x=0;
    uint16_t y=0;
    uint8_t x_dir;
    uint8_t y_dir;
    uint16_t left;
    uint16_t right;
    for(i=0;i<20;i++){
        if((rx3_buffer[i] == 0x61) && (rx3_buffer[i+1] == 0x63) && (rx3_buffer[i+2] == 0x63))
        {
            if(rx3_buffer[i+3]==0x2d){
                x_dir = 0;
                x = rx3_buffer[i+4]&0x0f;
                if(rx3_buffer[i+5]!= 0x2e){
                    x = x *10 + (rx3_buffer[i+5]&0x0f);
                    if(rx3_buffer[i+6]!= 0x2e){
                        x = x *10 + (rx3_buffer[i+6]&0x0f);
                    }
                }
            }
            else{
                x_dir = 1;
                x = rx3_buffer[i+3]&0x0f;
                if(rx3_buffer[i+4]!= 0x2e){
                    x = x *10 + (rx3_buffer[i+4]&0x0f);
                    if(rx3_buffer[i+5]!= 0x2e){
                        x = x *10 + (rx3_buffer[i+5]&0x0f);
                    }
                }
             }
        }
        if(rx3_buffer[i] == 0x2c){
            if(rx3_buffer[i+1]==0x2d){
                y_dir = 0;
                y = (rx3_buffer[i+2]&0x0f);
                if(rx3_buffer[i+3]!= 0x2e){
                    y = y *10 + (rx3_buffer[i+3]&0x0f);
                    if(rx3_buffer[i+4]!= 0x2e){
                        y = y *10 + rx3_buffer[i+4]&0x0f;
                    }
                }
            }
            else{
                y_dir = 1;
                y = rx3_buffer[i+1]&0x0f;
                if(rx3_buffer[i+2]!= 0x2e){
                    y = y *10 + (rx3_buffer[i+2]&0x0f);
                    if(rx3_buffer[i+3]!= 0x2e){
                        y = y *10 + (rx3_buffer[i+3]&0x0f);
                    }
                }
            }
        }
    }
    printf("x is %d,y is %d,x_dir is %d,y_dir is %d\n",x,y,x_dir,y_dir);
    left = right = y*20; //2000/100
    if(x_dir<0 && x<100)
    {
        left = 2000-x*20;//2000/100
    }
    if(x_dir>0 && x<100)
    {
        right = 2000-x*20; //2000/100
    }
    if(y>100)
    {
        left = right = 0;
    }
    motor_run(left,y_dir,right,y_dir);
}

void motor_balance()
{
    
}
