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

#include "Failure.h"


Failure::Failure (const wxString& theTestName,
                  const wxString& theFileName, 
		  long theLineNumber,
		  const wxString& theCondition) 
 : message (theCondition), testName (theTestName),
   fileName (theFileName), lineNumber (theLineNumber)
{
}

Failure::Failure (const wxString& theTestName, 
                  const wxString& theFileName,
                  long theLineNumber,
                  const wxString& expected,
                  const wxString& actual) 
 : testName (theTestName), fileName (theFileName), 
   lineNumber (theLineNumber)
{
	message.Printf(wxT("expected %1$s but got %2$s"), expected.c_str(), actual.c_str());
}


