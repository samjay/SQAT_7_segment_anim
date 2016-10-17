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
#include "display.h"
#include "i2c.h"

#define USE_MOCKS 1

#if USE_MOCKS
#include "Mock_I2C.h"
#include "Mock_delay.h"
#endif

using ::testing::InSequence;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::_;

namespace DISP_test_namespace2 {
	//
	// I2C_write dependency is put into the namespace to FORCE the stub code
	//   in use (and not the real)
	//
	int I2C_write(int address,const char* buffer,int length);
	//
	// delay dependency
	//
	void delay();
	//
	// target code is included instead of (direct) compilation:
	//   This allows to cut the dependencies (such as I2C_write above)
	//
	#include "../src/display.cpp"
}

using ::testing::Test;

//
// this action will be called as part of GoogleTest/GoogleMock
// when the i2c_write call is recorded
//
ACTION_P( SaveDispBuffer, store ) { memcpy(store,arg1,arg2); }

static char mDispBuffer[10];

class unittest_DISP2 : public Test
{
protected:
	virtual void SetUp()
	{
		mRc = -1;
		memset( mDispBuffer, 0x00, sizeof(mDispBuffer) );
		mMockI2C = new Mock_I2C();
		mMockDelay = new Mock_Delay();
	}
	//
	virtual void TearDown()
	{
		delete mMockI2C;
		mMockI2C = 0;
		delete mMockDelay;
		mMockDelay = 0;
	}
	//
	// compare buffer contents
	//   - true if full match
	//   - false otherwise
	//
	bool compare_display_data( const char* correctData,
			const int correctLength,
			const char* mDisplayedData )
	{
		bool status = true;
		for( int i=0; i<correctLength; i++ ){
			if( correctData[i] != mDisplayedData[i] ){
				status = false;
			}
		}
		return status;
	}
	void verify_frame_equals( const char* correct,
			const int length,
			const char* recorded )
	{
		bool equals = compare_display_data( correct,
				length,
				recorded );
		EXPECT_TRUE( equals );
	}
	//
	//
	//
	void add_expectation_with_delay( char* recordFrame1,
			const char*& tempPointer,
			const int frameDataLength )
	{
		EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
				WillOnce( DoAll(
						SaveArg<1>( &tempPointer ),
						SaveDispBuffer( recordFrame1 ),
						Return(frameDataLength) ) );
		EXPECT_CALL(*mMockDelay,delay());
	}
protected:
	int mRc;
//public:
	//
	// this "friend" access lets the "DISP_test_namespace::I2C_write" code below
	//   to access the variables used to record the parameters
	//
	friend int DISP_test_namespace2::I2C_write(int address,const char* buffer,int length);
	friend void DISP_test_namespace2::delay();
	static Mock_I2C* mMockI2C;
	static Mock_Delay* mMockDelay;
};

//
// initialized here as these are static members
//
Mock_I2C* unittest_DISP2::mMockI2C=0;
Mock_Delay* unittest_DISP2::mMockDelay=0;

//
//
//
int DISP_test_namespace2::I2C_write(int address,const char* buffer,int length)
{
	//
	// this WILL CRASH if mMockI2C has not been created
	//
	return unittest_DISP2::mMockI2C->i2c_write(address,buffer,length);
}

void DISP_test_namespace2::delay()
{
	//
	// this WILL CRASH if mMockDelay has not been created
	//
	unittest_DISP2::mMockDelay->delay();
}

//
//
//
TEST_F( unittest_DISP2, fixture )
{
  EXPECT_EQ( -1, mRc );
}

//
// this fails if the test class has not
//   constructed the mocks properly
//
TEST_F( unittest_DISP2, constructor )
{
	ASSERT_NE( (long)0, (long)mMockI2C );
	ASSERT_NE( (long)0, (long)mMockDelay );
}

/**
 *
 * test for display control
 *
 */

TEST_F( unittest_DISP2, display_off )
{
	const int cmdLen = 1;
	const char* data=0;
	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,cmdLen)).
			WillOnce( DoAll(
					SaveArg<1>( &data ),
					SaveDispBuffer( mDispBuffer ),
					Return(cmdLen) ) );
	mRc = DISP_test_namespace2::DISP_off();
	EXPECT_EQ( cmdLen, mRc );
	EXPECT_EQ( HT16K33_CMD_OSCILLATOR_OFF, mDispBuffer[0] );
}

//
// the power on sequence has several calls to i2c_write
//   - these need to be recorded for verification
//     => mock with recording skills is needed
//
TEST_F( unittest_DISP2, power_on_display )
{
	const int cmdLen = 1;
	const char* data=0;
	char command1[2];
	char command2[2];
	char command3[2];

	// GoogleMock uses this:
	InSequence callSequenceRecording;
	//
	// the "_" (underscore) is a wild char here, matching any
	//   - ADDR_HT16K133 must be matched
	//   - cmdLen must be matched
	//
	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,cmdLen)).
			WillOnce( DoAll(
					SaveArg<1>( &data ),
					SaveDispBuffer( command1 ),
					Return(cmdLen) ) );
	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,cmdLen)).
			WillOnce( DoAll(
					SaveArg<1>( &data ),
					SaveDispBuffer( command2 ),
					Return(cmdLen) ) );
	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,cmdLen)).
			WillOnce( DoAll(
					SaveArg<1>( &data ),
					SaveDispBuffer( command3 ),
					Return(cmdLen) ) );

	mRc = DISP_test_namespace2::DISP_on();
	EXPECT_EQ( cmdLen, mRc );
	EXPECT_EQ( HT16K33_CMD_OSCILLATOR_ON, command1[0] );
	EXPECT_EQ( (char)(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON), command2[0] );
	EXPECT_EQ( (char)(HT16K33_CMD_BRIGHTNESS | HT16K33_8_16_DUTY), command3[0] );
}

/**
 *
 * test animation
 *
 */

const int frameDataLength = 10;
const char data_ShowAllSegments[frameDataLength]={
		0,(char)SEGMENT_ALL,
		1,(char)SEGMENT_ALL,
		2,0,
		3,(char)SEGMENT_ALL,
		4,(char)SEGMENT_ALL
};

TEST_F( unittest_DISP2, show_single_frame )
{
	const char* data=0;

	//
	// IF expectation is matched THEN it records the data
	//   - "data" contains pointer value (should be non zero)
	//   - "mDispBuffer" contains the copy of the data
	//
	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
			WillOnce( DoAll(
					SaveArg<1>( &data ),
					SaveDispBuffer( mDispBuffer ),
					Return(frameDataLength) ) );
	//
	// since framedelay is "1" we expect a call to delay too
	//
	EXPECT_CALL(*mMockDelay,delay());
	//
	// call the target code
	//
	mRc = DISP_test_namespace2::DISP_show_frame( FRAME_ALL, FRAME_DELAY_ON );
	//
	// verify that data size is good
	ASSERT_EQ( frameDataLength, mRc );
	//
	// verify the actual frame data: displayed segments
	//   - mDispBuffer is where the mock saved it in EXPECT_CALL
	bool equals = compare_display_data( data_ShowAllSegments,
			frameDataLength,
			mDispBuffer );
	EXPECT_TRUE( equals );
}

//
// correct data for 4 frame animation moving the dot
//   from left to right
//
const char dot_anim_frame_1_data[frameDataLength]=
	{0,CSEGMENT_DOT,	1,0x00,				2,0,3,0x00,	4,0x00};
const char dot_anim_frame_2_data[frameDataLength]=
	{0,0x00,		1,CSEGMENT_DOT,		2,0,3,0x00,	4,0x00};
const char dot_anim_frame_3_data[frameDataLength]=
	{0,0x00,		1,0x00,		2,0,3,	CSEGMENT_DOT,4,0x00};
const char dot_anim_frame_4_data[frameDataLength]=
	{0,0x00,		1,0x00,		2,0,3,	0x00,4,		CSEGMENT_DOT};

//
// verify a sequence of frames
//
TEST_F( unittest_DISP2, show_sequence_of_frames_with_delay )
{
	uint32_t dot_anim[4] = {
			ANIM_DOT_1,ANIM_DOT_2,ANIM_DOT_3,ANIM_DOT_4
	};
	char recordFrame1[frameDataLength];
	char recordFrame2[frameDataLength];
	char recordFrame3[frameDataLength];
	char recordFrame4[frameDataLength];
	const char* tempPointer=0;

	InSequence callSequenceRecording;

	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
			WillOnce( DoAll(
					SaveArg<1>( &tempPointer ),
					SaveDispBuffer( recordFrame1 ),
					Return(frameDataLength) ) );
	EXPECT_CALL(*mMockDelay,delay());

	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
			WillOnce( DoAll(
					SaveArg<1>( &tempPointer ),
					SaveDispBuffer( recordFrame2 ),
					Return(frameDataLength) ) );
	EXPECT_CALL(*mMockDelay,delay());

	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
			WillOnce( DoAll(
					SaveArg<1>( &tempPointer ),
					SaveDispBuffer( recordFrame3 ),
					Return(frameDataLength) ) );
	EXPECT_CALL(*mMockDelay,delay());

	EXPECT_CALL(*mMockI2C,i2c_write(ADDR_HT16K33,_,frameDataLength)).
			WillOnce( DoAll(
					SaveArg<1>( &tempPointer ),
					SaveDispBuffer( recordFrame4 ),
					Return(frameDataLength) ) );
	EXPECT_CALL(*mMockDelay,delay());

	mRc = DISP_test_namespace2::DISP_show_anim(dot_anim,4,FRAME_DELAY_ON );
	EXPECT_EQ( 4, mRc );

	bool equals=false;

	//
	// verify first frame
	//
	equals = compare_display_data( dot_anim_frame_1_data,
			frameDataLength,
			recordFrame1 );
	EXPECT_TRUE( equals );
	equals = compare_display_data( dot_anim_frame_2_data,
			frameDataLength,
			recordFrame2 );
	EXPECT_TRUE( equals );
	equals = compare_display_data( dot_anim_frame_3_data,
			frameDataLength,
			recordFrame3 );
	EXPECT_TRUE( equals );
	equals = compare_display_data( dot_anim_frame_4_data,
			frameDataLength,
			recordFrame4 );
	EXPECT_TRUE( equals );
}

//
// verify a sequence of frames, cleaned up a bit...
//   TODO: spot the failure and fix the problem
//   TODO: see how HAVING expectations in reusable subroutine
//         does complicate the bug finding a little bit
//         (can no longer jump to the real expect failure)
//
TEST_F( unittest_DISP2, show_sequence_of_frames_with_delay_2 )
{
	const uint32_t dot_anim[4] = {
			ANIM_DOT_1,ANIM_DOT_2,ANIM_DOT_3,ANIM_DOT_3
	};
	char recordFrame1[frameDataLength];
	char recordFrame2[frameDataLength];
	char recordFrame3[frameDataLength];
	char recordFrame4[frameDataLength];
	const char* tempPointer=0;

	InSequence callSequenceRecording;

	add_expectation_with_delay( recordFrame1, tempPointer, frameDataLength );
	add_expectation_with_delay( recordFrame2, tempPointer, frameDataLength );
	add_expectation_with_delay( recordFrame3, tempPointer, frameDataLength );
	add_expectation_with_delay( recordFrame4, tempPointer, frameDataLength );

	mRc = DISP_test_namespace2::DISP_show_anim(dot_anim,4,FRAME_DELAY_ON );
	EXPECT_EQ( 4, mRc );

	bool equals=false;

	//
	// verify frames
	//
	verify_frame_equals( dot_anim_frame_1_data,
			frameDataLength, recordFrame1 );
	verify_frame_equals( dot_anim_frame_2_data,
			frameDataLength, recordFrame2 );
	verify_frame_equals( dot_anim_frame_3_data,
			frameDataLength, recordFrame3 );
	verify_frame_equals( dot_anim_frame_4_data,
			frameDataLength, recordFrame4 );
}
