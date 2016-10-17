/*
 * TestAll.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: timppa
 */

//#include <string>

#include "gtest/gtest.h"

int main(int argc,char** argv)
{
	int rc=-1;

	::testing::InitGoogleTest(&argc,argv);
	rc=RUN_ALL_TESTS();
	return rc;
}
