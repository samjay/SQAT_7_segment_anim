/*
 * Mock_I2C.h
 *
 *  Created on: Jun 19, 2016
 *      Author: timppa
 */

#ifndef TEST_MOCK_MOCK_I2C_H_
#define TEST_MOCK_MOCK_I2C_H_

#include "gmock_inc.h"

class Mock_I2C
{
public:
	Mock_I2C() {}
	virtual ~Mock_I2C() {}
	//
	MOCK_METHOD3(i2c_write,int(int address,const char* buffer,int length));
	MOCK_METHOD5(i2c_read,int(int,char*,int,char*,int));
};

#endif /* TEST_MOCK_MOCK_I2C_H_ */
