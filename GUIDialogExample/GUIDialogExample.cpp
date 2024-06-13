
// GUIDialogExample.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "GUIDialogExample.h"

#include "PropertyMenuGUIDialogExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIDialogExampleApp

BEGIN_MESSAGE_MAP(CGUIDialogExampleApp, CWinApp)
END_MESSAGE_MAP()


// CGUIDialogExampleApp ����

CGUIDialogExampleApp::CGUIDialogExampleApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CGUIDialogExampleApp ��ü�Դϴ�.

CGUIDialogExampleApp theApp;


// CGUIDialogExampleApp �ʱ�ȭ

BOOL CGUIDialogExampleApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(_T("GUIDialogExample examples"));

	// GUI Manager �ʱ�ȭ ��ó���� �����մϴ�.
	CGUIManager::PreInitialize();

	// �̹��� �� ���� �޴��� �߰��մϴ�.
	CGUIManager::AddMenuItem(new CGUIMenuItemCreateViewImage);

	// ���� ������Ƽ �޴��� ����մϴ�.
	CGUIManager::RegisterMenu(CPropertyMenuGUIDialogExample(), L"GUI Dialog Example", L"Menu", false);

	// GUI �󿡼� ���� �並 �����մϴ�.
	std::vector<CGUIFixedViewDeclaration*> vctFixedViewDecl;

	for(int32_t i = 0; i < 4; ++i)
	{
		CGUIFixedViewDeclaration* pDeclarationCam = new CGUIFixedViewDeclaration;
		pDeclarationCam->SetMultiDocTemplateName("CGUIDocImageCGUIChildFrameImageCGUIViewImage");
		pDeclarationCam->SetViewName(L"Image View");
		CGUIManagerView::AddFixedViewDeclaration(pDeclarationCam);
		vctFixedViewDecl.push_back(pDeclarationCam);
	}

	// GUI �󿡼� ���� �迭�� �����մϴ�.
	CGUIFixedViewPlacement fvp;
	CGUIFixedViewPlacementSet fvpSet1(false);
	fvpSet1.SetName(L"Prompt Image View Set 1");

	CGUIFixedViewDeclaration* pDeclarationCam = vctFixedViewDecl[0];
	fvp.SetFixedViewDeclaration(pDeclarationCam);
	fvp.SetPlacement(CFLRect<double>(0.0, 0.0, 1.0, 1.0));
	fvpSet1.AddFixedViewPlacement(fvp);

	CGUIManagerView::AddFixedViewPlacementSet(fvpSet1);

	// �ε����� �ش��ϴ� ���� �������� �����մϴ�.
	CGUIManagerView::SelectFixedViewPlacementSet(0);

	CGUIManager::PreInitializePaneVisibility(true, false, false, false, false);

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
		pMF->SetWindowPos(NULL, 0, 0, 1280, 1024, NULL);
	}

	CPropertyMenuGUIDialogExample* pPropertyMenu = dynamic_cast<CPropertyMenuGUIDialogExample*>(CGUIManager::GetMenuItem(L"GUI Dialog Example", L"Menu"));

	if(pPropertyMenu)
	{
		if(!pPropertyMenu->OnLButtonDoubleClick())
			pPropertyMenu->OnLButtonDoubleClick();

		pPropertyMenu->GetPaneProperties()->UndockPane();
		pPropertyMenu->GetPaneProperties()->SetWindowPos(nullptr, 0, 0, 400, 1000, SWP_NOMOVE | SWP_NOZORDER);
		pPropertyMenu->GetPaneProperties()->DockToFrameWindow(CBRS_ALIGN_RIGHT, nullptr, DT_DOCK_FIRST, nullptr, -1, true);
		pPropertyMenu->GetPaneProperties()->GetPropertyGridCtrl()->Invalidate();
		pPropertyMenu->ShowPaneWindow();
	}

	return TRUE;
}
