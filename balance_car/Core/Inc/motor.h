#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f1xx_hal.h"


extern uint16_t motor_left_step;
extern uint16_t motor_right_step;
extern uint8_t motor_get_encoder_state_L(void);
extern uint8_t motor_get_encoder_state_R(void);
extern void motor_toggle_encoder_state_L(void);
extern void motor_toggle_encoder_state_R(void);


#endif
