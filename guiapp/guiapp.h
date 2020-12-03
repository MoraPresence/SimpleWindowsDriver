
// guiapp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CguiappApp:
// See guiapp.cpp for the implementation of this class
//

class CguiappApp : public CWinAppEx
{
public:
	CguiappApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CguiappApp theApp;