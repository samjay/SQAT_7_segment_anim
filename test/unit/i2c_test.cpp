/*
 * i2c_test.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: timppa
 */


#include "gtest_inc.h"

#include "chip.h"

#include "i2c.h"

namespace I2C_test_namespace {
#include "../src/i2c.cpp"
}

using ::testing::Test;

/**********************************************************************
 *
 * GoogleTest test-class for I2C testing
 *
 **********************************************************************/
class unittest_I2C : public Test
{
public:
	//
	// constructor just resets attributes to some default values
	//
	unittest_I2C()
	{
		mRc = -1;
		pLPC_SYSCTL = 0;
		pLPC_I2C = 0;
	}
	virtual ~unittest_I2C(){}
protected:
	//
	// SetUp performs the per-test initialization/setup of test instance
	//
	virtual void SetUp()
	{
		mRc = -1;
		pLPC_SYSCTL = new LPC_SYSCTL_T();
		pLPC_I2C = new LPC_I2C_T();
	}
	//
	// TearDown cleans-up test instance
	//
	virtual void TearDown()
	{
		delete pLPC_I2C;
		delete pLPC_SYSCTL;
	}
	void reset_to_all_ones()
	{
		memset( pLPC_I2C, 0xFF, sizeof(*pLPC_I2C) );
		memset( pLPC_SYSCTL, 0xFF, sizeof(*pLPC_SYSCTL) );
	}
	void reset_to_all_zeroes()
	{
		memset( pLPC_I2C, 0x0, sizeof(*pLPC_I2C) );
		memset( pLPC_SYSCTL, 0x0, sizeof(*pLPC_SYSCTL) );
	}
protected:
	int mRc;
	LPC_I2C_T* pLPC_I2C;
	LPC_SYSCTL_T* pLPC_SYSCTL;
};

/**********************************************************************
 *
 * quick verification of test fixture
 * - this helps detect problems in test setup
 *
 **********************************************************************/
TEST_F( unittest_I2C, test_fixture )
{
	EXPECT_EQ( -1, mRc );
}

/**********************************************************************
 *
 * quick verification of test setup/construction
 * - this helps detect problems in test setup
 *
 **********************************************************************/

TEST_F( unittest_I2C, test_constructor )
{
	ASSERT_NE( (void*)0, pLPC_I2C );
	ASSERT_NE( (void*)0, pLPC_SYSCTL );
}

/**********************************************************************
 *
 * test cases
 *
 **********************************************************************/

//
// test that the I2C_init works
// - obviously cannot verify everything here...
//
TEST_F( unittest_I2C, correct_initialization_of_I2C_clocking )
{
	reset_to_all_zeroes();

	I2C_test_namespace::I2C_init(pLPC_SYSCTL,pLPC_I2C);
//	EXPECT_EQ( I2C_RC_OK, mRc );

	EXPECT_EQ( (1<<5), pLPC_SYSCTL->SYSAHBCLKCTRL);
	EXPECT_EQ( 15, pLPC_I2C->CLKDIV);
	EXPECT_EQ( I2C_CFG_MSTEN, pLPC_I2C->CFG);
}

TEST_F( unittest_I2C, correct_initialization_of_I2C_master )
{
	reset_to_all_zeroes();

	I2C_test_namespace::I2C_init(pLPC_SYSCTL,pLPC_I2C);
//	EXPECT_EQ( I2C_RC_OK, mRc );

	EXPECT_EQ( I2C_CFG_MSTEN, pLPC_I2C->CFG);
}
//
//
//
