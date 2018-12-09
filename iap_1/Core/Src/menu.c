#include "menu.h"
#include "iap.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "common.h"
#include "flash_if.h"
#include "iap.h"
#include "dma.h"

uint32_t *data;
uint32_t base_address=0x8000000,offset,line_address;
volatile uint8_t rx2_buffer[BUFFER_SIZE];
uint8_t verify_num,length,i,key;
void python_usart_upload(void);
void program_one_line_hex(uint8_t[]);

void program_one_line_hex(uint8_t line_data[])
{
    uint8_t hex_data[22];
    uint32_t sum = 0;
    int i;
    Serial_PutString("\n\n\r ready to verify\n\r");
    Serial_PutString(line_data);
    for(i=0;i<22;i++)
    {
        hex_data[i]=(CONVERTHEX(line_data[2*i+1]))<<4 +(CONVERTHEX(line_data[2*i+2]));
    }
    HAL_UART_Transmit_IT(&huart2, (uint8_t *)&hex_data, 22);
    length = hex_data[0];

    verify_num = hex_data[4+length];
    for(i=0;i<length+4;i++)
    {
        sum+=hex_data[i];
    }
    if(0x100-sum != verify_num)
    {
        Serial_PutString("\n\n\r verify error\n\r");
    }
    else
    {
        if(hex_data[3]==0x04)
        {
            base_address = (line_data[5]<<24)+(line_data[6]<<16);
        }
        if(hex_data[3]==0x00)
        {
            for(i=0;i<length/4;i++)
            {
                data[i]=(hex_data[i*4]<<12)+(hex_data[i*4]<<8)+(hex_data[i*4]<<4)+hex_data[i*4];
            }
            offset = (hex_data[1]<<4) + hex_data[2];
            line_address = base_address + offset;
            FLASH_If_Write(line_address,data,(uint32_t)length/4);

        }
        if(hex_data[3] == 0x01)
        {
            Serial_PutString("\n\n\r program done!\n\r");
        }
    }

}


void python_usart_upload(void)
{
  // Serial_PutString("\n\n\rSend data via usart\n\r");
  while(1)
  {

      HAL_UART_Receive_IT(&huart2,rx2_buffer,BUFFER_SIZE);

      // Serial_PutString("\n\n\rdata recieved\n\r");
      // Serial_PutString((uint8_t *)&rx2_buffer);

      program_one_line_hex(rx2_buffer);

  }

}

// void main_menu(void)
// {
//     Serial_PutString("\r\n=================== Main Menu ============================\r\n\n");
//     Serial_PutString("  Upload image from the internal Flash ----------------- 1\r\n\n");
//     Serial_PutString("  Execute the loaded application ----------------------- 2\r\n\n");
//
//     HAL_UART_Receive(&huart2, &key, 1, RX_TIMEOUT);
//
//     switch (key)
//     {
//     case '1' :
//       /* Upload user application from the Flash */
//       python_usart_upload();
//       break;
//     case '2' :
//       Serial_PutString("Start program execution......\r\n\n");
//
//       jump();
// 	default:
// 	Serial_PutString("Invalid Number ! ==> The number should be either 1, 2, 3 \r");
// 	break;
//     }
//   }

void main_menu(void)
{
    while(1)
    {
        HAL_UART_Receive_IT(&huart2,rx2_buffer,BUFFER_SIZE);
        HAL_UART_Transmit_IT(&huart2, rx2_buffer, 1);

        if (rx2_buffer[0] == 0x53) //"S"
        {
          Serial_PutString("s");
          // python_usart_upload();

        }
        // Serial_PutString("1");

        for(i=0;i<BUFFER_SIZE;i++)
        {
            rx2_buffer[i]=0;
        }
    }


}
