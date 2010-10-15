/*
 * This file has been released into the public domain.
 */

#ifndef FAILURE_H
#define FAILURE_H


class Failure
{
public:
	Failure (const wxString& theTestName, const wxString& theFileName, long theLineNumber, const wxString& theCondition);
	Failure (const wxString& theTestName, const wxString& theFileName, long theLineNumber, const wxString& expected, const wxString& actual);
	
	wxString message;
	wxString testName;
	wxString fileName;
	long lineNumber;
};


#endif

