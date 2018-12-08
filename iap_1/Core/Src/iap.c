

#include "iap.h"
#include "stm32f1xx_hal.h"

volatile pFunction JumpToApplication;
uint32_t JumpAddress;

void jump()
{
    /* execute the new program */
    // __disable_irq();
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    /* Jump to user application */
    JumpToApplication = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    JumpToApplication();
    // __enable_irq();

}
