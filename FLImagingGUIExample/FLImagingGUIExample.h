
// FLImagingGUIExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFLImagingGUIExampleApp:
// �� Ŭ������ ������ ���ؼ��� FLImagingGUIExample.cpp�� �����Ͻʽÿ�.
//

class CFLImagingGUIExampleApp : public CWinAppEx
{
public:
	CFLImagingGUIExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFLImagingGUIExampleApp theApp;