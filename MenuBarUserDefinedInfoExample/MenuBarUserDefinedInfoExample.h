
// MenuBarUserDefinedInfoExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMenuBarUserDefinedInfoExampleApp:
// �� Ŭ������ ������ ���ؼ��� MenuBarUserDefinedInfoExample.cpp�� �����Ͻʽÿ�.
//

class CMenuBarUserDefinedInfoExampleApp : public CWinAppEx
{
public:
	CMenuBarUserDefinedInfoExampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMenuBarUserDefinedInfoExampleApp theApp;