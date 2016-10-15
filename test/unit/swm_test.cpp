/*
 * led_test.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: timppa
 */


#include "gtest_inc.h"

//#include <LPC8xx.h>
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#include "swm.h"
//#include "led.h"

//using ::testing::Return;

namespace SWM_ut {
#include "../src/swm.cpp"
}

using ::testing::Test;

class ut_SWM : public Test
{
protected:
	virtual void SetUp()
	{
		mRc = -1;
		pLPC_SWM = new SWM_ut::LPC_SWM_T();
	}
	//
	virtual void TearDown()
	{
		delete pLPC_SWM;
	}
	void reset_to_all_ones()
	{
		memset( pLPC_SWM, 0xFF, sizeof(*pLPC_SWM) );
	}
	void reset_to_all_zeroes()
	{
		memset( pLPC_SWM, 0x0, sizeof(*pLPC_SWM) );
	}
	//  int data_sum_of_gpio_structure()
	//  {
	//	  unsigned char *p=(unsigned char*)(pLPC_GPIO_PORT);
	//	  int i;
	//	  int sum = 0;
	//	  for(i=0; i<sizeof(*pLPC_GPIO_PORT); i++){
	//		  sum += p[i];
	//	  }
	//	  return sum;
	//  }
protected:
	int mRc;
	SWM_ut::LPC_SWM_T* pLPC_SWM;
};

//
//
//
TEST_F( ut_SWM, fixture )
{
	EXPECT_EQ( -1, mRc );
}

TEST_F( ut_SWM, constructor )
{
	//  EXPECT_NE( (long)0,(long)mInstance );
}

#if 1
TEST_F( ut_SWM, correct_initialization_of_switch_matrix )
{
	reset_to_all_zeroes();

	SWM_ut::SWM_init();	// Segmentation fault (core dumped)

	EXPECT_EQ( 0xffff0004UL, pLPC_SWM->PINASSIGN[0] );
	EXPECT_EQ( 0x0affffffUL, pLPC_SWM->PINASSIGN[7]);
	EXPECT_EQ( 0xffffff0bUL, pLPC_SWM->PINASSIGN[8]);
	EXPECT_EQ( 0xffffffb3UL, pLPC_SWM->PINENABLE0);
	//	led_ut::led_on( pLPC_GPIO_PORT,LED_RED );
	//	EXPECT_EQ( 0, pLPC_SWM->B[0][LED_RED] );
}
#endif
