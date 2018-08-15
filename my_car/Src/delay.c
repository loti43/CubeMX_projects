#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "delay.h"

void delay_us(uint16_t us)
{
    uint16_t differ=0xffff-us-5;
    /*为防止因中断打断延时，造成计数错误.
     如从0xfffE开始延时1us,但由于中断打断
    （此时计数器仍在计数），本因计数至0xffff）
    便停止计数，但由于错过计数值，并重载arr值，
    导致实际延时(0xffff+1)us
    */

    HAL_TIM_Base_Start(&htim14);

    __HAL_TIM_SetCounter(&htim14,differ);

    while(differ<0xffff-5)
    {
        differ=__HAL_TIM_GetCounter(&htim14);
    }

    HAL_TIM_Base_Stop(&htim14);
}
