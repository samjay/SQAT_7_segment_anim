/*
 * led_test.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: timppa
 */


#include "gtest_inc.h"

#include "chip.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#include "swm.h"
//#include "led.h"

//using ::testing::Return;

namespace SWM_ut {
#include "../src/swm.cpp"
}

using ::testing::Test;

/**********************************************************************
 *
 * GoogleTest test-class for SWM testing
 *
 **********************************************************************/
class ut_SWM : public Test
{
public:
	//
	// constructor just resets attributes to some default values
	//
	ut_SWM()
	{
		mRc = -1;
		pLPC_SWM = 0;
		pLPC_SYSCTL = 0;
	}
	virtual ~ut_SWM(){}
protected:
	//
	// SetUp performs the per-test initialization/setup of test instance
	//
	virtual void SetUp()
	{
		mRc = -1;
		pLPC_SWM = new LPC_SWM_T();
		pLPC_SYSCTL = new LPC_SYSCTL_T();
	}
	//
	// TearDown cleans-up test instance
	//
	virtual void TearDown()
	{
		delete pLPC_SWM;
		delete pLPC_SYSCTL;
	}
	void reset_to_all_ones()
	{
		memset( pLPC_SWM, 0xFF, sizeof(*pLPC_SWM) );
		memset( pLPC_SYSCTL, 0xFF, sizeof(*pLPC_SYSCTL) );
	}
	void reset_to_all_zeroes()
	{
		memset( pLPC_SWM, 0x0, sizeof(*pLPC_SWM) );
		memset( pLPC_SYSCTL, 0x0, sizeof(*pLPC_SYSCTL) );
	}
	int get_I2C_SDA_pin()
	{
		int pin = (pLPC_SWM->PINASSIGN[7] >> 24);

		return 0x00FF & pin;
	}
	int get_I2C_SCL_pin()
	{
		int pin = pLPC_SWM->PINASSIGN[8];

		return 0x00FF & pin;
	}
protected:
	int mRc;
	LPC_SWM_T* pLPC_SWM;
	LPC_SYSCTL_T* pLPC_SYSCTL;
};

/**********************************************************************
 *
 * quick verification of test fixture
 * - this helps detect problems in test setup
 *
 **********************************************************************/
TEST_F( ut_SWM, test_fixture )
{
	EXPECT_EQ( -1, mRc );
}

/**********************************************************************
 *
 * quick verification of test setup/construction
 * - this helps detect problems in test setup
 *
 **********************************************************************/

TEST_F( ut_SWM, test_constructor )
{
	ASSERT_NE( (void*)0, pLPC_SWM );
	ASSERT_NE( (void*)0, pLPC_SYSCTL );
}

/**********************************************************************
 *
 * test cases
 *
 **********************************************************************/

//
// test that the SWM_init works
// - obviously cannot verify everything here...
//
TEST_F( ut_SWM, correct_SWM_initialization_for_I2C_pins )
{
	reset_to_all_zeroes();

	mRc = SWM_ut::SWM_init( pLPC_SYSCTL, pLPC_SWM );
	EXPECT_EQ( SWM_RC_OK, mRc );

	int i2c_sda_pin = get_I2C_SDA_pin();
	int i2c_scl_pin = get_I2C_SCL_pin();

	EXPECT_EQ( 10, i2c_sda_pin );
	EXPECT_EQ( 11, i2c_scl_pin );
	EXPECT_EQ( 0xffffffb3UL, pLPC_SWM->PINENABLE0);
}

TEST_F( ut_SWM, swm_is_not_initialized_if_SYSCON_param_is_null )
{
	mRc = SWM_ut::SWM_init(0,pLPC_SWM);
	EXPECT_EQ( SWM_RC_PARAM_ERROR, mRc );

	int i2c_sda_pin = get_I2C_SDA_pin();
	int i2c_scl_pin = get_I2C_SCL_pin();

	EXPECT_EQ( 0, i2c_sda_pin );
	EXPECT_EQ( 0, i2c_scl_pin );
}

TEST_F( ut_SWM, swm_is_not_initialized_if_SWM_param_is_null )
{
	mRc = SWM_ut::SWM_init(pLPC_SYSCTL,0);
	EXPECT_EQ( SWM_RC_PARAM_ERROR, mRc );

	int i2c_sda_pin = get_I2C_SDA_pin();
	int i2c_scl_pin = get_I2C_SCL_pin();

	EXPECT_EQ( 0, i2c_sda_pin );
	EXPECT_EQ( 0, i2c_scl_pin );
}
