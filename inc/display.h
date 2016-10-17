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

#define SEGMENT_ALL  0xFF
#define SEGMENT_NONE 0x00
#define SEGMENT_DOT  0x80
#define CSEGMENT_DOT (char)0x80

#define ANIM_DOT_1 0x80000000
#define ANIM_DOT_2 0x00800000
#define ANIM_DOT_3 0x00008000
#define ANIM_DOT_4 0x00000080

#define FRAME_DELAY_ON  1
#define FRAME_DELAY_OFF 0

int DISP_off();
int DISP_on();
int DISP_show_frame(uint32_t position,int delayon);
int DISP_show_anim( const uint32_t* frames, int length, int framedelay );

#endif /* DISPLAY_H_ */
