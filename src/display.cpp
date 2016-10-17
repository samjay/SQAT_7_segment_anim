/*
 * display.c
 *
 *  Created on: Oct 7, 2016
 *      Author: odieste
 */

#include "i2c.h"
#include "delay.h"
#include "display.h"

const int addr = ADDR_HT16K33;

/**********************************************************************
 *
 * display setup
 *
 **********************************************************************/

/*
 * Disable the display
 * Further explanations unnecessary. See HT16K33 user manual if interested (summary in p. 30-31)
 *
 */
int DISP_off()
{
	char data[10];
	int rc;

	data[0] = HT16K33_CMD_OSCILLATOR_OFF;
	data[1]=0;

	rc = I2C_write(addr, data, 1);
	return rc;
}

/*
 * Enable and setup the display
 * Further explanations unnecessary. See HT16K33 user manual if interested (summary in p. 30-31)
 *
 */
int DISP_on()
{
	char data[10];
	int rc;

	data[0] = HT16K33_CMD_OSCILLATOR_ON;
	data[1]=0;

	rc = I2C_write(addr, data, 1);
	if ( 0 <= rc ){
		data[0] = HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON;
		rc = I2C_write(addr, data, 1);
	} else {
		return rc;
	}
	if ( 0 <= rc ){
		data[0] = HT16K33_CMD_BRIGHTNESS | HT16K33_8_16_DUTY;
		rc = I2C_write(addr, data, 1);
	} else {
		return rc;
	}
	return rc;
}

/**********************************************************************
 *
 * display animation
 *
 **********************************************************************/

// Data frame to be sent to the display
char frame [] = {0, 0, 1, 0, 2, 0, 3, 0, 4, 0};

// Logical index to HW mapping
int mapping[] = {9, 7, 3, 1};

// Format the data frame and write to the display
int DISP_show_frame(uint32_t value,int framedelay)
{
	int rc;

	for(int i = 0; i <= 3; i++) {
		frame[mapping[i]] = value & 0x000000FF;
		value = value >> 8;
	}
	rc = I2C_write(addr, frame, 10);
	if ( framedelay ){
		delay();
	}
	return rc;
}

int DISP_show_anim( const uint32_t* frames, int length, int framedelay )
{
	if ( 0!=frames && length>0 ){
		for(int i=0; i<length; i++){
			DISP_show_frame( frames[i],framedelay );
		}
		return length;
	}
	return 0;
}
