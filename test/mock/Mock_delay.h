/*
 * Mock_delay.h
 *
 *  Created on: Oct 17, 2016
 *      Author: timppa
 */

#ifndef MOCK_MOCK_DELAY_H_
#define MOCK_MOCK_DELAY_H_

#include "gmock_inc.h"

class Mock_Delay
{
public:
	Mock_Delay() {}
	virtual ~Mock_Delay() {}
	//
	MOCK_METHOD0(delay,void() );
};



#endif /* MOCK_MOCK_DELAY_H_ */
