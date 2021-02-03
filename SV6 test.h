
// SV6 test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "stdafx.h"
#include "resource.h"		// main symbols


// CSV6testApp:
// See SV6 test.cpp for the implementation of this class
//

class CSV6testApp : public CWinApp
{
public:
	CSV6testApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSV6testApp theApp;