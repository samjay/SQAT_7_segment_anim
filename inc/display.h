/*
 * display.h
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define ADDR_HT16K33 				0x70

#define HT16K33_BLINK_CMD       	0x80
#define HT16K33_BLINK_DISPLAYON 	0x01

#define HT16K33_BLINK_OFF       	0x00
#define HT16K33_BLINK_2HZ       	0x01
#define HT16K33_BLINK_1HZ       	0x02
#define HT16K33_BLINK_HALFHZ    	0x03

#define HT16K33_8_16_DUTY    		0x08

#define HT16K33_CMD_BRIGHTNESS  	0xE0
#define HT16K33_CMD_OSCILLATOR_ON 	0x21
#define HT16K33_CMD_OSCILLATOR_OFF 	0x20

#include "chip.h"

#define FRAME_EMPTY 0x00000000
#define FRAME_ALL   0xFFFFFFFF

int DISP_off();
int DISP_on();
int DISP_show_frame(uint32_t position);

#endif /* DISPLAY_H_ */
