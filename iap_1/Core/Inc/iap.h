#ifndef _IAP_H
#define _IAP_H


#define APPLICATION_ADDRESS     (uint32_t)0x08008000      /* Start user code address: ADDR_FLASH_PAGE_8 */

typedef  void (*pFunction)(void);
extern void jump(void);

#endif
