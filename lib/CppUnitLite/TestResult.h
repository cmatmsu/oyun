/*
 * This file has been released into the public domain.
 */

#ifndef TESTRESULT_H
#define TESTRESULT_H

class Failure;

class TestResult
{
public:
	TestResult ();
	virtual ~TestResult() { }

	virtual void testsStarted ();
	virtual void runTest ();
	virtual void addSuccess ();
	virtual void addFailure (const Failure& failure);
	virtual void testsEnded ();

private:
	long testCount;
	long checkCount;
	long failureCount;
};

#endif

