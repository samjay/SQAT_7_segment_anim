/*
 * i2c_test.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: timppa
 */


#include "gtest_inc.h"

#include "chip.h"

#include "display.h"

namespace DISP_test_namespace {

//
// I2C_write dependency is put into the namespace to FORCE the stub code
//   in use (and not the real)
//
int I2C_write(int address,const char* buffer,int length);
//
// target code is included instead of (direct) compilation:
//   This allows to cut the dependencies (such as I2C_write above)
//
#include "../src/display.cpp"

}

using ::testing::Test;

/**********************************************************************
 *
 * GoogleTest test-class for 7-segment display testing
 *
 **********************************************************************/
class unittest_DISP : public Test
{
public:
	//
	// constructor just resets attributes to some default values
	//
	unittest_DISP()
	{
		mRc = -1;
	}
	virtual ~unittest_DISP(){}
protected:
	//
	// SetUp performs the per-test initialization/setup of test instance
	//
	virtual void SetUp()
	{
		mRc = -1;
		memset( mI2C_write_buffer,0,sizeof(mI2C_write_buffer));
	}
	//
	// TearDown cleans-up test instance
	//
	virtual void TearDown()
	{
	}
	static char get_i2c_buffer_char(int index)
	{
		if ( index>=0 && index<sizeof(unittest_DISP::mI2C_write_buffer) ){
			return unittest_DISP::mI2C_write_buffer[index];
		} else {
			return -1;
		}
	}
protected:
	int mRc;
	friend int DISP_test_namespace::I2C_write(int address,const char* buffer,int length);
	static char mI2C_write_buffer[10];
	static int mI2C_write_address;
	static int mI2C_write_length;
};

char unittest_DISP::mI2C_write_buffer[10]={0,0,0,0,0,0,0,0,0,0};
int unittest_DISP::mI2C_write_address;
int unittest_DISP::mI2C_write_length;

int DISP_test_namespace::I2C_write(int address,const char* buffer,int length)
{
	if ( length>0 ){
		if ( length>sizeof(unittest_DISP::mI2C_write_buffer) )length=sizeof(unittest_DISP::mI2C_write_buffer);
		memcpy(unittest_DISP::mI2C_write_buffer,buffer,length);
	} else {
		memset(unittest_DISP::mI2C_write_buffer,0xff,sizeof(unittest_DISP::mI2C_write_buffer));
	}
	unittest_DISP::mI2C_write_address = address;
	unittest_DISP::mI2C_write_length = length;
	return length;
}

/**********************************************************************
 *
 * quick verification of test fixture
 * - this helps detect problems in test setup
 *
 **********************************************************************/
TEST_F( unittest_DISP, test_fixture )
{
	EXPECT_EQ( -1, mRc );
}

/**********************************************************************
 *
 * quick verification of test setup/construction
 * - this helps detect problems in test setup
 *
 **********************************************************************/

TEST_F( unittest_DISP, test_constructor )
{
}

/**********************************************************************
 *
 * test cases
 *
 **********************************************************************/

//
// test that the DISP_show_frame forms correct data to I2C
//
TEST_F( unittest_DISP, display_empty_frame )
{
	mRc = DISP_test_namespace::DISP_show_frame( FRAME_EMPTY );
	EXPECT_EQ( 10, mRc );

	EXPECT_EQ( 0x00, get_i2c_buffer_char(1) );
	EXPECT_EQ( 0x00, get_i2c_buffer_char(3) );
	EXPECT_EQ( 0x00, get_i2c_buffer_char(5) );
	EXPECT_EQ( 0x00, get_i2c_buffer_char(7) );
	EXPECT_EQ( 0x00, get_i2c_buffer_char(9) );
}

TEST_F( unittest_DISP, display_all_frame )
{
	mRc = DISP_test_namespace::DISP_show_frame( FRAME_ALL );
	EXPECT_EQ( 10, mRc );

	EXPECT_EQ( (char)0xFF, get_i2c_buffer_char(1) );
	EXPECT_EQ( (char)0xFF, get_i2c_buffer_char(3) );
	EXPECT_EQ( (char)0xFF, get_i2c_buffer_char(7) );
	EXPECT_EQ( (char)0xFF, get_i2c_buffer_char(9) );
}

TEST_F( unittest_DISP, colon_is_not_part_of_frame_data )
{
	mRc = DISP_test_namespace::DISP_show_frame( FRAME_ALL );
	EXPECT_EQ( 10, mRc );
	EXPECT_EQ( (char)0x00, get_i2c_buffer_char(5) );	//

	mRc = DISP_test_namespace::DISP_show_frame( FRAME_EMPTY );
	EXPECT_EQ( 10, mRc );
	EXPECT_EQ( (char)0x00, get_i2c_buffer_char(5) );	//
}
