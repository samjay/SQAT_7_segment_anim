/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "swm.h"
#include "i2c.h"
#include "delay.h"
#include "display.h"

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    uint32_t frame [] = {
    		0x80000000,
			0x00800000,
			0x00008000,
			0x00000080
    };

    SWM_init();
    I2C_init();
    DISP_on();

    int i = 0;

    // Enter an infinite loop
    while(true) {
    	show_dot(frame[i]);
    	delay();
    	i++;
    	if (i > 3) i = 0;
    }

    // Unreachable code
    return 0 ;
}
