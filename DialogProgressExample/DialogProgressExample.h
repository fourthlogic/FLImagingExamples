
// DialogProgressExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDialogProgressExampleApp:
// �� Ŭ������ ������ ���ؼ��� DialogProgressExample.cpp�� �����Ͻʽÿ�.
//

class CDialogProgressExampleApp : public CWinAppEx
{
public:
	CDialogProgressExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDialogProgressExampleApp theApp;