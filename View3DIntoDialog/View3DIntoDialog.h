
// ImageViewIntoDialog.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CView3DIntoDialogApp:
// See ImageViewIntoDialog.cpp for the implementation of this class
//

class CView3DIntoDialogApp : public CWinApp
{
public:
	CView3DIntoDialogApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CView3DIntoDialogApp theApp;