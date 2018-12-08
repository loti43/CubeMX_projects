#include "menu.h"
#include "iap.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "common.h"
#include "flash_if.h"
#include "iap.h"

uint32_t *data;
uint32_t base_address,offset,line_address;
uint8_t rx2_buffer[BUFFER_SIZE];
uint8_t verify_num,length,i,key;
void python_usart_upload(void);
void program_one_line_hex(uint8_t[]);
void program_one_line_hex(uint8_t line_data[])
{
    uint32_t sum = 0;
    length = line_data[0];
    verify_num = line_data[4+length];
    for(i=0;i<length+4;i++)
    {
        sum+=line_data[i];
    }
    if(0x100-sum != verify_num)
    {
        Serial_PutString("\n\n\r verify error\n\r");
    }
    else
    {
        if(line_data[3]==0x04)
        {
            base_address = (line_data[5]<<24)+(line_data[6]<<16);
        }
        if(line_data[3]==0x00)
        {
            for(i=0;i<length/4;i++)
            {
                data[i]=(line_data[i*4]<<12)+(line_data[i*4]<<8)+(line_data[i*4]<<4)+line_data[i*4];
            }
            offset = (line_data[1]<<4)+line_data[2];
            line_address = base_address + offset;
            FLASH_If_Write(line_address,data,(uint32_t)length/4);

        }
        if(line_data[3] == 0x01)
        {
            Serial_PutString("\n\n\r program done!\n\r");
        }
    }

}


void python_usart_upload(void)
{
  Serial_PutString("\n\n\rSend data via usart\n\r");
  HAL_UART_Receive_DMA(&huart2,rx2_buffer,BUFFER_SIZE);
  program_one_line_hex(rx2_buffer);

}

void main_menu(void)
{
 while (1)
  {

    Serial_PutString("\r\n=================== Main Menu ============================\r\n\n");
    Serial_PutString("  Upload image from the internal Flash ----------------- 1\r\n\n");
    Serial_PutString("  Execute the loaded application ----------------------- 2\r\n\n");

    HAL_UART_Receive(&huart2, &key, 1, RX_TIMEOUT);

    switch (key)
    {
    case '1' :
      /* Upload user application from the Flash */
      python_usart_upload();
      break;
    case '2' :
      Serial_PutString("Start program execution......\r\n\n");

      jump();
	default:
	Serial_PutString("Invalid Number ! ==> The number should be either 1, 2, 3 \r");
	break;
    }
  }
}
