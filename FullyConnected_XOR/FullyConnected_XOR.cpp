
// DialogProgressExample.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "FullyConnected_XOR.h"

#include "PropertyMenuFullyConnected_XOR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFullyConnectedXORApp

BEGIN_MESSAGE_MAP(CFullyConnectedXORApp, CWinApp)
END_MESSAGE_MAP()


// CFullyConnectedXORApp ����

CFullyConnectedXORApp::CFullyConnectedXORApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CFullyConnectedXORApp ��ü�Դϴ�.

CFullyConnectedXORApp theApp;


// CFullyConnectedXORApp �ʱ�ȭ

BOOL CFullyConnectedXORApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(_T("FullyConnectedXOR examples"));

	// GUI Manager �ʱ�ȭ ��ó���� �����մϴ�.
	CGUIManager::PreInitialize();

	// ���� ������Ƽ �޴��� ����մϴ�.
	CGUIManager::RegisterMenu(CPropertyMenuFullyConnectedXOR(), L"Fully-Connected XOR Example", L"Menu", false);

	// �̹��� �� ���� �޴��� �߰��մϴ�.
	CGUIManager::ModifyPaneCreationOnInitialize(EPaneType_None, EPaneType_ViewGraph);

	// �ε����� �ش��ϴ� ���� �������� �����մϴ�.
	CGUIManager::PreInitializePaneVisibility(true, false, false, false, false, true);

	// �� �����ڸ� �ʱ�ȭ �մϴ�.
	CGUIManagerModel::Initialize();

	// GUI �����ڸ� �ʱ�ȭ �մϴ�.
	CGUIManager::Initialize();

	// Main Frame ��ġ�� �����մϴ�.
	CGUIMainFrame* pMF = nullptr;

	if(AfxGetApp() && AfxGetApp()->m_pMainWnd)
		pMF = dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd);

	if(pMF)
	{
		CGUIPaneMenu* pPaneMenu = pMF->GetPaneMenu();

		if(pPaneMenu)
		{
			if(pPaneMenu->CanAutoHide())
				pPaneMenu->SetAutoHideMode(TRUE, CBRS_ALIGN_LEFT);
		}

		pMF->ShowWindow(SW_SHOWMAXIMIZED);
		pMF->ModifyStyle(WS_MAXIMIZEBOX, 0, 0);
		pMF->SetWindowPos(NULL, 0, 0, 1200, 600, NULL);
	}

	CGUIPaneViewGraph* pPaneGraph = CGUIManagerPane::GetPaneViewGraph();
	CGUIViewGraph* pViewGraph = nullptr;

	if(pPaneGraph)
	{
		pViewGraph = pPaneGraph->GetViewGraph();
		pViewGraph->SetWindowPos(nullptr, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
		pPaneGraph->DockToFrameWindow(CBRS_ALIGN_TOP, nullptr, DT_DOCK_FIRST, nullptr, -1, true);
	}

	CPropertyMenuFullyConnectedXOR* pPropertyMenu = dynamic_cast<CPropertyMenuFullyConnectedXOR*>(CGUIManager::GetMenuItem(L"Fully-Connected XOR Example", L"Menu"));

	if(pPropertyMenu)
	{
		if(!pPropertyMenu->OnLButtonDoubleClick())
			pPropertyMenu->OnLButtonDoubleClick();

		pPropertyMenu->GetPaneProperties()->UndockPane();
		pPropertyMenu->GetPaneProperties()->SetWindowPos(nullptr, 0, 0, 400, 600, SWP_NOMOVE | SWP_NOZORDER);
		pPropertyMenu->GetPaneProperties()->DockToFrameWindow(CBRS_ALIGN_RIGHT, nullptr, DT_DOCK_FIRST, nullptr, -1, true);
		pPropertyMenu->GetPaneProperties()->GetPropertyGridCtrl()->Invalidate();
		pPropertyMenu->ShowPaneWindow();
	}

	return TRUE;
}
