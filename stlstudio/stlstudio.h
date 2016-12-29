
// stlstudio.h : main header file for the stlstudio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CstlstudioApp:
// See stlstudio.cpp for the implementation of this class
//

class CstlstudioApp : public CWinApp
{
public:
	CstlstudioApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CstlstudioApp theApp;
