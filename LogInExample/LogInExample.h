
// LogInExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CLogInExampleApp:
// �� Ŭ������ ������ ���ؼ��� LogInExample.cpp�� �����Ͻʽÿ�.
//

class CLogInExampleApp : public CWinAppEx
{
public:
	CLogInExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CLogInExampleApp theApp;