/*
 * TestAll.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: timppa
 */

#include <string>

#include "gtest/gtest.h"

//#define ARG_INCL_PERFORMANCE "--performance"
//
//namespace testargs {
//	bool isPerfIncluded;
//}
//
int main(int argc,char** argv)
{
	int rc=-1;

//	testargs::isPerfIncluded = false;
//	for ( int i=1; i<argc; i++ ){
//		if ( 0==strcmp(argv[i],ARG_INCL_PERFORMANCE) ){
//			testargs::isPerfIncluded = true;
//		}
//	}
	::testing::InitGoogleTest(&argc,argv);
	rc=RUN_ALL_TESTS();
	return rc;
}
