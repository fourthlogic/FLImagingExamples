
// MenuBarUserDefinedInfoExample.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "MenuBarUserDefinedInfoExample.h"

#include "PropertyMenuBarUserDefinedInfoExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMenuBarUserDefinedInfoExampleApp

BEGIN_MESSAGE_MAP(CMenuBarUserDefinedInfoExampleApp, CWinApp)
END_MESSAGE_MAP()


// CMenuBarUserDefinedInfoExampleApp ����

CMenuBarUserDefinedInfoExampleApp::CMenuBarUserDefinedInfoExampleApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CMenuBarUserDefinedInfoExampleApp ��ü�Դϴ�.

CMenuBarUserDefinedInfoExampleApp theApp;


// CMenuBarUserDefinedInfoExampleApp �ʱ�ȭ

BOOL CMenuBarUserDefinedInfoExampleApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(_T("MenuBarUserDefinedInfoExample examples"));

	// GUI Manager �ʱ�ȭ ��ó���� �����մϴ�.
	CGUIManager::PreInitialize();

	// ���� ������Ƽ �޴��� ����մϴ�.
	CGUIManager::RegisterMenu(CPropertyMenuBarUserDefinedInfoExample(), L"MenuBar User Defined Info Example", L"Menu", false);


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
	CGUIFixedViewPlacementSet fvpSet2(false);
	CGUIFixedViewPlacementSet fvpSet3(false);
	CGUIFixedViewPlacementSet fvpSet4(false);

	fvpSet1.SetName(L"Prompt Image View Set 1");
	fvpSet2.SetName(L"Prompt Image View Set 2");
	fvpSet3.SetName(L"Prompt Image View Set 3");
	fvpSet4.SetName(L"Prompt Image View Set 4");

	for(int32_t x = 0; x < 2; ++x)
	{
		for(int32_t y = 0; y < 2; ++y)
		{
			int32_t i32Index = (y * 2) + x;

			CGUIFixedViewDeclaration* pDeclarationCam = vctFixedViewDecl[i32Index];
			fvp.SetFixedViewDeclaration(pDeclarationCam);

			switch (i32Index)
			{
			case 0:
				{
					fvp.SetPlacement(CFLRect<double>(1.0 * x, 1.0 * y, 1.0 * (x + 1), 1.0 * (y + 1)));
					fvpSet1.AddFixedViewPlacement(fvp);

					fvp.SetPlacement(CFLRect<double>(0.5 * x, 1.0 * y, 0.5 * (x + 1), 1.0 * (y + 1)));
					fvpSet2.AddFixedViewPlacement(fvp);

					fvp.SetPlacement(CFLRect<double>(0.5 * x, 0.5 * y, 0.5 * (x + 1), 0.5 * (y + 1)));
					fvpSet3.AddFixedViewPlacement(fvp);
					fvpSet4.AddFixedViewPlacement(fvp);
				}
				break;

			case 1:
				{
					fvp.SetPlacement(CFLRect<double>(0.5 * x, 1.0 * y, 0.5 * (x + 1), 1.0 * (y + 1)));
					fvpSet2.AddFixedViewPlacement(fvp);

					fvp.SetPlacement(CFLRect<double>(0.5 * x, 0.5 * y, 0.5 * (x + 1), 0.5 * (y + 1)));
					fvpSet3.AddFixedViewPlacement(fvp);
					fvpSet4.AddFixedViewPlacement(fvp);
				}
				break;

			case 2:
				{
					fvp.SetPlacement(CFLRect<double>(1.0 * x, 0.5 * y, 1.0 * (x + 1), 0.5 * (y + 1)));
					fvpSet3.AddFixedViewPlacement(fvp);

					fvp.SetPlacement(CFLRect<double>(0.5 * x, 0.5 * y, 0.5 * (x + 1), 0.5 * (y + 1)));
					fvpSet4.AddFixedViewPlacement(fvp);
				}
				break;

			case 3:
				{
					fvp.SetPlacement(CFLRect<double>(0.5 * x, 0.5 * y, 0.5 * (x + 1), 0.5 * (y + 1)));
					fvpSet4.AddFixedViewPlacement(fvp);
				}
				break;
			}
		}
	}

	CGUIManagerView::AddFixedViewPlacementSet(fvpSet1);
	CGUIManagerView::AddFixedViewPlacementSet(fvpSet2);
	CGUIManagerView::AddFixedViewPlacementSet(fvpSet3);
	CGUIManagerView::AddFixedViewPlacementSet(fvpSet4);

	// �ε����� �ش��ϴ� ���� �������� �����մϴ�.
	CGUIManagerView::SelectFixedViewPlacementSet(3);

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

		// MainFrame ���� CGUIMenuBar ��ü ��� ����
		CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

		if(pMenuBar)
		{
			// ��ü ���� �� ���ڿ�, ���� ����
			CGUIMenuBarUserDefinedInfo udi(L"Stopped", RGB(128, 0, 0));

			// Current model name �簢���� ���� �������� �ϳ��� �߰��˴ϴ�.
			// ������ ��ü�� �� �ڿ� �߰�(PushBack)
			pMenuBar->PushBackUserDefinedInfo(udi);

			pMenuBar->Invalidate();
		}
	}

	// "MenuBar User Defined Info Example" �̶�� �̸��� �޴� ������ ã��
	CPropertyMenuBarUserDefinedInfoExample* pPropertyMenu = dynamic_cast<CPropertyMenuBarUserDefinedInfoExample*>(CGUIManager::GetMenuItem(L"MenuBar User Defined Info Example", L"Menu"));

	// "MenuBar User Defined Info Example" �̶�� �̸��� �޴� �������� ���� ���, ������Ƽ â ����
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
