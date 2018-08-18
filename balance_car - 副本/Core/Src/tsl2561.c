#include "tsl2561.h"
#include "gpio.h"
#include "i2c.h"
#include "math.h"

uint8_t send_buffer[8]= {0};
uint8_t receive_buffer[4] = {0x99};
uint16_t data0,data1;

void tsl2561_init()
{
    if(HAL_I2C_IsDeviceReady(&hi2c2,0x39<<1,0xff,0xff)== HAL_OK)
    printf("device is ok\n");
    else printf("divice is not ready\n");

    send_buffer[0] = 0x80;
    send_buffer[1] = 0x03;
    if(HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,2,0xff)== HAL_OK)
    printf("cmd power send successful\n");
    else printf("cmd 1 send error\n");
    HAL_Delay(100);

    send_buffer[0] = 0x81;
    send_buffer[1] = 0x10;
    if(HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,2,0xff)== HAL_OK)
    printf("cmd time successful\n");
    else printf("cmd 2 send error\n");
    HAL_Delay(10);
}

void tsl2561_get_data()
{

    send_buffer[0] = 0x8c;
    //data0low
    HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,1,0xff);
    HAL_I2C_Master_Receive(&hi2c2,0x39<<1,receive_buffer,1,0xff);
    //data0high
    HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,1,0xff);
    HAL_I2C_Master_Receive(&hi2c2,0x39<<1,receive_buffer+1,1,0xff);
    //data1low
    HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,1,0xff);
    HAL_I2C_Master_Receive(&hi2c2,0x39<<1,receive_buffer+2,1,0xff);
    //data1high
    HAL_I2C_Master_Transmit(&hi2c2,0x39<<1,send_buffer,1,0xff);
    HAL_I2C_Master_Receive(&hi2c2,0x39<<1,receive_buffer+3,1,0xff);

    data0 = receive_buffer[1] *256 + receive_buffer[0];
    data1 = receive_buffer[3] *256 + receive_buffer[2];
}

extern unsigned int CalculateLux(unsigned int iGain, unsigned int tInt, unsigned int ch0, unsigned int ch1, int iType)
{
    //−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
    // first, scale the channel values depending on the gain and integration time
    // 16X, 402mS is nominal.
    // scale if integration time is NOT 402 msec
    unsigned long chScale;
    unsigned long channel1;
    unsigned long channel0;
    switch (tInt)
    {
    case 0: // 13.7 msec
    chScale = CHSCALE_TINT0;
    break;
    case 1: // 101 msec
    chScale = CHSCALE_TINT1;
    break;
    default: // assume no scaling
    chScale = (1 << CH_SCALE);
    break;
    }
    // scale if gain is NOT 16X
    if (!iGain) chScale = chScale << 4; // scale 1X to 16X
    // scale the channel values
    channel0 = (ch0 * chScale) >> CH_SCALE;
    channel1 = (ch1 * chScale) >> CH_SCALE;
    //−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
    // find the ratio of the channel values (channel1/channel0)
    // protect against divide by zero
    unsigned long ratio1 = 0;
    if (channel0 != 0) ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
    // round the ratio value
    unsigned long ratio = (ratio1 + 1) >> 1;
    // is ratio <= eachBreak ?
    unsigned int b, m;
    switch (iType)
    {
        case 0: // T, FN and CL package
        if (ratio <= K1T)
        {b=B1T; m=M1T;}
        else if (ratio <= K2T)
        {b=B2T; m=M2T;}
        else if (ratio <= K3T)
        {b=B3T; m=M3T;}
        else if (ratio <= K4T)
        {b=B4T; m=M4T;}
        else if (ratio <= K5T)
        {b=B5T; m=M5T;}
        else if (ratio <= K6T)
        {b=B6T; m=M6T;}
        else if (ratio <= K7T)
        {b=B7T; m=M7T;}
        else if (ratio > K8T)
        {b=B8T; m=M8T;}
        break;
        case 1:// CS package
        if (ratio <= K1C)
        {b=B1C; m=M1C;}
        else if (ratio <= K2C)
        {b=B2C; m=M2C;}
        else if (ratio <= K3C)
        {b=B3C; m=M3C;}
        else if (ratio <= K4C)
        {b=B4C; m=M4C;}
        else if (ratio <= K5C)
        {b=B5C; m=M5C;}
        else if (ratio <= K6C)
        {b=B6C; m=M6C;}
        else if (ratio <= K7C)
        {b=B7C; m=M7C;}
        else if (ratio > K8C)
        {b=B8C; m=M8C;}
        break;
    }
    unsigned long temp;
    temp = ((channel0 * b) - (channel1 * m));
    // do not allow negative lux value
    // if (temp < 0) temp = 0;
    // round lsb (2^(LUX_SCALE−1))
    temp += (1 << (LUX_SCALE-1));
    // strip off fractional portion
    unsigned long lux = temp >> LUX_SCALE;
    return(lux);
}

extern float cal_lux(uint16_t channel0,uint16_t channel1)
{
    float E;
    if(0.0<channel1/channel0&&channel1/channel0<=0.50)E=(0.0304*channel0-0.062*channel0*pow(channel1/channel0, 1.4));
    if(0.50<channel1/channel0&&channel1/channel0<=0.61)E=(0.0224*channel0-0.031*channel1);
    if(0.61<channel1/channel0&&channel1/channel0<=0.80)E=(0.0128*channel0-0.0153*channel1);
    if(0.80<channel1/channel0&&channel1/channel0<=1.30)E=(0.00146*channel0-0.00112*channel1);
    if(channel1/channel0>1.30)E=0;
    return E;
}


float tsl2561_get_lux()
{
    float lux;
    tsl2561_get_data();
    // lux = CalculateLux(1, 0,data0, data1, 0);
    lux = cal_lux(data0,data1);

    return lux;
}
