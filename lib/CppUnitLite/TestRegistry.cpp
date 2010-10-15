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
#include "TestResult.h"
#include "TestRegistry.h"


void TestRegistry::addTest(Test *test) 
{
	instance().add (test);
}


int TestRegistry::runAllTests(TestResult& result) 
{
	return instance().run (result);
}


TestRegistry& TestRegistry::instance() 
{
	static TestRegistry registry;
	return registry;
}


void TestRegistry::add(Test *test) 
{
	const wxString &group = test->getGroup();
	
	if (!tests[group]) 
	{
		tests[group] = test;
		return;
	}
	
	test->setNext (tests[group]);
	tests[group] = test;
}


int TestRegistry::run(TestResult& result) 
{
	int num = 0;

	result.testsStarted ();

	for (std::map<wxString, Test *>::iterator iter = tests.begin() ; iter != tests.end() ; ++iter)
	{
		wxPrintf(wxT("%ls: "), (*iter).first.wc_str());
		
		for (Test *test = (*iter).second; test != 0; test = test->getNext ())
		{
			wxPrintf(wxT("."));
			result.runTest ();
			test->run (result);
			num++;
		}
		
		wxPrintf(wxT("\n"));
	}
	result.testsEnded ();

	return num;
}



