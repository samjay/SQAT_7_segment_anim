/*
 * display.c
 *
 *  Created on: Oct 7, 2016
 *      Author: odieste
 */

#include "delay.h"

void delay() {
	// Avoid code optimization
	volatile static int i;
	for(i = 0; i <= 700000; i++);
}
