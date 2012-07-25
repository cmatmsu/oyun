/*
 * This file has been released into the public domain.
 */

#ifndef TESTREGISTRY_H
#define TESTREGISTRY_H

#include <map>

class Test;
class TestResult;

class TestRegistry
{
public:
	static void addTest (Test *test);
	static bool runAllTests (TestResult& result);

private:
	static TestRegistry& instance ();
	void add (Test *test);
	bool run (TestResult& result);
	
	std::map<wxString, Test *> tests;
};

#endif

