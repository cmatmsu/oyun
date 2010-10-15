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

#include "Test.h"
#include "TestRegistry.h"
#include "TestResult.h"
#include "Failure.h"


Test::Test (const wxString& testName, const wxString& testGroup) 
	: name_ (testName), group_ (testGroup) 
{
	TestRegistry::addTest (this);
}


Test *Test::getNext() const
{
	return next_;
}


void Test::setNext(Test *test)
{	
	next_ = test;
}

bool Test::check(long expected, long actual, TestResult& result, 
                 const wxString& fileName, long lineNumber)
{
	if (expected == actual)
	{
		result.addSuccess();
		return true;
	}

	wxString strExpected = wxString::Format(wxT("%d"), expected);
	wxString strActual = wxString::Format(wxT("%d"), actual);

	result.addFailure (Failure (name_, fileName, lineNumber, strExpected, strActual));

	return false;
}

bool Test::check(const wxString& expected, const wxString& actual, 
		 TestResult& result, const wxString& fileName, long lineNumber)
{
	if (expected == actual)
	{
		result.addSuccess();
		return true;
	}

	result.addFailure (Failure (name_, fileName, lineNumber, expected, actual));
	return false;
}

