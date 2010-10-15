/*
 * This file has been released into the public domain.
 */

#ifndef TEST_H
#define TEST_H

#include <cmath>
#include <typeinfo>

class TestResult;


class Test
{
public:
	Test (const wxString& testName, const wxString& testGroup);
	virtual ~Test() { }

	virtual void run (TestResult& result) = 0;

	void setNext(Test *test);
	Test *getNext () const;
	
	const wxString& getName () const { return name_; }
	const wxString& getGroup () const { return group_; }

protected:
	bool check (long expected, long actual, TestResult& result, 
				const wxString& fileName, long lineNumber);
	bool check (const wxString& expected, const wxString& actual, 
				TestResult& result, const wxString& fileName, long lineNumber);

	wxString name_;
	wxString group_;
	Test *next_;
};

template <typename T>
inline wxString TestToString (const T &x)
{ wxString str; str << x; return str; }

template<>
inline wxString TestToString<const char *> (const char * const &x)
{ return wxString::FromUTF8(x); }

template<>
inline wxString TestToString<char *> (char * const &x)
{ return wxString::FromUTF8(x); }

template<>
inline wxString TestToString<void *> (void * const &x)
{ return wxString::Format(wxT("%p"), x); }

#define TEST(testGroup, testName)							\
  class testGroup##testName##Test : public Test 					\
	{ public: testGroup##testName##Test () : Test (wxT(#testName), wxT(#testGroup)) {} \
            void run (TestResult& result_); } 						\
    testGroup##testName##Instance; 							\
	void testGroup##testName##Test::run (TestResult& result_) 

#define FAIL(text) 				do { { result_.addFailure (Failure (name_, __TFILE__, __LINE__,(text))); return; } } while (0)

#define CHECK(condition)			do { { if (!(condition)) { result_.addFailure (Failure (name_, __TFILE__, __LINE__, wxString::FromUTF8( #condition ))); return; } else { result_.addSuccess(); } } } while (0)
#define CHECK_EQUAL(expected,actual) 		do { { if ((expected) != (actual)) { result_.addFailure(Failure(name_, __TFILE__, __LINE__, TestToString(expected), TestToString(actual))); } else { result_.addSuccess(); } } } while (0)
#define CHECK_TYPES_EQUAL(expected,actual)	do { { if (typeid(expected) != typeid(actual)) { result_.addFailure(Failure(name_, __TFILE__, __LINE__, TestToString(typeid(expected).name()), TestToString(typeid(actual).name()))); } else { result_.addSuccess(); } } } while (0)
#define CHECK_NOT_EQUAL(expected,actual)	do { { if ((expected) == (actual)) { result_.addFailure(Failure(name_, __TFILE__, __LINE__, TestToString(expected), wxString::Format(wxT("not %s"), TestToString(actual).c_str()))); } else { result_.addSuccess(); } } } while (0)
#define POINTERS_EQUAL(expected,actual)		do { { if ((expected) != (actual)) { result_.addFailure(Failure(name_, __TFILE__, __LINE__, TestToString((void *)expected), TestToString((void *)actual))); } else {result_.addSuccess();} } } while (0)

#define DOUBLES_EQUAL(expected,actual,threshold)					\
	do { { 										\
	  double actualTemp = actual; 							\
	  double expectedTemp = expected; 						\
	  if (fabs ((expectedTemp)-(actualTemp)) > threshold) {				\
	      result_.addFailure (Failure (name_, __TFILE__, __LINE__, TestToString(expectedTemp), TestToString(actualTemp))); \
	      return; 									\
	  }										\
	  else { result_.addSuccess(); } 						\
	} } while (0)


#endif
