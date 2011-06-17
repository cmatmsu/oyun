/*
 * This file has been released into the public domain.
 */

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include "TestResult.h"
#include "Failure.h"

#include <stdio.h>


TestResult::TestResult()
	: testCount (0), checkCount (0), failureCount (0)
{
}


void TestResult::testsStarted() 
{
	wxPrintf (wxT("Beginning unit tests for Oyun...\n"));
}

void TestResult::runTest()
{
	testCount++;
}

void TestResult::addSuccess()
{
	checkCount++;
}

void TestResult::addFailure(const Failure& failure) 
{
	wxPrintf (wxT("\nFailure: \"%1$ls\" line %2$ld in %3$ls"),
	          failure.message.wc_str(),
	          failure.lineNumber,
	          failure.fileName.wc_str());
	
	failureCount++;
	checkCount++;
}


void TestResult::testsEnded() 
{
	if (failureCount > 0)
		wxPrintf (wxT("\n%1$ld tests run [%2$ld checks]: there were %3$ld failures!\n"), testCount, checkCount, failureCount);
	else
		wxPrintf (wxT("\n%1$ld tests run [%2$ld checks]: all tests passed!\n"), testCount, checkCount);
}
