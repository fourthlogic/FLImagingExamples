
// DialogProgressExample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFullyConnectedXORApp:
// �� Ŭ������ ������ ���ؼ��� DialogProgressExample.cpp�� �����Ͻʽÿ�.
//

class CFullyConnectedXORApp : public CWinAppEx
{
public:
	CFullyConnectedXORApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFullyConnectedXORApp theApp;