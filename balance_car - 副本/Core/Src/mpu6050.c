#include "stm32f1xx_hal.h"
#include "usart.h"


uint8_t buffer[33];
uint8_t data[11];
double a[3],w[3],Angle[3],T;

void mpu6050_get_data()
{
    int i;
    int j;
    int k;
    uint8_t sum = 0;
    HAL_UART_Receive(&huart1,buffer,33,0xff);
    //从21个值选择通过校验的11个值
    for(i=22;i>0;i--)
    {
        if (buffer[i] == 0x55)
        {
            for(j=0;j<10;j++)
            {
                sum += buffer[i+j];
            }
            if(sum == buffer[i+10])
            {
                for(k=0;k<33-i;k++)
                {
                    data[k] = buffer[i+k];
                }
            }
        }
    }

}

void mpu6050_decode_data(uint8_t chrTemp[])
{
    switch(chrTemp[1])
    {
        case (0x51):
        a[0] = (short)((chrTemp[3]<<8|chrTemp[2]))/32768.0*16;
        a[1] = (short)((chrTemp[5]<<8|chrTemp[4]))/32768.0*16;
        a[2] = (short)((chrTemp[7]<<8|chrTemp[6]))/32768.0*16;
        T = (short)((chrTemp[9]<<8|chrTemp[8]))/340.0+36.25;
        break;
        case (0x52):
        w[0] = (short)((chrTemp[3]<<8|chrTemp[2]))/32768.0*2000;
        w[1] = (short)((chrTemp[5]<<8|chrTemp[4]))/32768.0*2000;
        w[2] = (short)((chrTemp[7]<<8|chrTemp[6]))/32768.0*2000;
        T = (short)((chrTemp[9]<<8|chrTemp[8]))/340.0+36.25;
        break;
        case (0x53):
        Angle[0] = (short)((chrTemp[3]<<8|chrTemp[2]))/32768.0*180;
        Angle[1] = (short)((chrTemp[5]<<8|chrTemp[4]))/32768.0*180;
        Angle[2] = (short)((chrTemp[7]<<8|chrTemp[6]))/32768.0*180;
        T = (short)((chrTemp[9]<<8|chrTemp[8]))/340.0+36.25;
        break;
    }
    switch(chrTemp[12])
    {
        case (0x51):
        a[0] = (short)((chrTemp[14]<<8|chrTemp[13]))/32768.0*16;
        a[1] = (short)((chrTemp[16]<<8|chrTemp[15]))/32768.0*16;
        a[2] = (short)((chrTemp[18]<<8|chrTemp[17]))/32768.0*16;
        T = (short)((chrTemp[20]<<8|chrTemp[19]))/340.0+36.25;
        break;
        case (0x52):
        w[0] = (short)((chrTemp[14]<<8|chrTemp[13]))/32768.0*2000;
        w[1] = (short)((chrTemp[16]<<8|chrTemp[15]))/32768.0*2000;
        w[2] = (short)((chrTemp[18]<<8|chrTemp[17]))/32768.0*2000;
        T = (short)((chrTemp[20]<<8|chrTemp[19]))/340.0+36.25;
        break;
        case (0x53):
        Angle[0] = (short)((chrTemp[14]<<8|chrTemp[13]))/32768.0*180;
        Angle[1] = (short)((chrTemp[16]<<8|chrTemp[15]))/32768.0*180;
        Angle[2] = (short)((chrTemp[18]<<8|chrTemp[17]))/32768.0*180;
        T = (short)((chrTemp[20]<<8|chrTemp[19]))/340.0+36.25;
        break;
    }
    switch(chrTemp[23])
    {
        case (0x51):
        a[0] = (short)((chrTemp[25]<<8|chrTemp[24]))/32768.0*16;
        a[1] = (short)((chrTemp[27]<<8|chrTemp[26]))/32768.0*16;
        a[2] = (short)((chrTemp[29]<<8|chrTemp[28]))/32768.0*16;
        T = (short)((chrTemp[31]<<8|chrTemp[30]))/340.0+36.25;
        break;
        case (0x52):
        w[0] = (short)((chrTemp[25]<<8|chrTemp[24]))/32768.0*2000;
        w[1] = (short)((chrTemp[27]<<8|chrTemp[26]))/32768.0*2000;
        w[2] = (short)((chrTemp[29]<<8|chrTemp[28]))/32768.0*2000;
        T = (short)((chrTemp[31]<<8|chrTemp[30]))/340.0+36.25;
        break;
        case (0x53):
        Angle[0] = (short)((chrTemp[25]<<8|chrTemp[24]))/32768.0*180;
        Angle[1] = (short)((chrTemp[27]<<8|chrTemp[26]))/32768.0*180;
        Angle[2] = (short)((chrTemp[29]<<8|chrTemp[28]))/32768.0*180;
        T = (short)((chrTemp[31]<<8|chrTemp[30]))/340.0+36.25;
        break;
    }
}

void mpu6050_prt()
{
    // mpu6050_get_data();
    mpu6050_decode_data(rx1_buffer);
    printf("a = %4.3f\t%4.3f\t%4.3f\t\r\n",a[0],a[1],a[2]);
    printf("w = %4.3f\t%4.3f\t%4.3f\t\r\n",w[0],w[1],w[2]);
    printf("Angle = %4.2f\t%4.2f\t%4.2f\tT=%4.2f\r\n",Angle[0],Angle[1],Angle[2],T);
}
