#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "delay.h"

void delay_us(uint16_t us)
{
    uint16_t differ=0xffff-us-5;
    /*Ϊ��ֹ���жϴ����ʱ����ɼ�������.
     ���0xfffE��ʼ��ʱ1us,�������жϴ��
    ����ʱ���������ڼ����������������0xffff��
    ��ֹͣ�����������ڴ������ֵ��������arrֵ��
    ����ʵ����ʱ(0xffff+1)us
    */

    HAL_TIM_Base_Start(&htim14);

    __HAL_TIM_SetCounter(&htim14,differ);

    while(differ<0xffff-5)
    {
        differ=__HAL_TIM_GetCounter(&htim14);
    }

    HAL_TIM_Base_Stop(&htim14);
}
