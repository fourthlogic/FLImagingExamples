
// GUIDialogExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CGUIDialogExampleApp:
// �� Ŭ������ ������ ���ؼ��� GUIDialogExample.cpp�� �����Ͻʽÿ�.
//

class CGUIDialogExampleApp : public CWinAppEx
{
public:
	CGUIDialogExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGUIDialogExampleApp theApp;