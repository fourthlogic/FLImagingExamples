
// PropertyPaneTimerExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPropertyPaneTimerExampleApp:
// �� Ŭ������ ������ ���ؼ��� PropertyPaneTimerExample.cpp�� �����Ͻʽÿ�.
//

class CPropertyPaneTimerExampleApp : public CWinAppEx
{
public:
	CPropertyPaneTimerExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPropertyPaneTimerExampleApp theApp;