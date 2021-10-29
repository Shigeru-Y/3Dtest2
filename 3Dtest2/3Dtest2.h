
// 3Dtest2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy3Dtest2App:
// See 3Dtest2.cpp for the implementation of this class
//

class CMy3Dtest2App : public CWinApp
{
public:
	CMy3Dtest2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy3Dtest2App theApp;