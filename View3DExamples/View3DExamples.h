
// View3DExamples.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CView3DExamplesApp:
// �� Ŭ������ ������ ���ؼ��� View3DExamples.cpp�� �����Ͻʽÿ�.
//

class CView3DExamplesApp : public CWinAppEx
{
public:
	CView3DExamplesApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CView3DExamplesApp theApp;