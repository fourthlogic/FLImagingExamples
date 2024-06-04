
// View3DExamples.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "View3DExamples.h"

#include "PropertyView3DExamples.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CView3DExamplesApp

BEGIN_MESSAGE_MAP(CView3DExamplesApp, CWinApp)
END_MESSAGE_MAP()


// CView3DExamplesApp ����

CView3DExamplesApp::CView3DExamplesApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CView3DExamplesApp ��ü�Դϴ�.

CView3DExamplesApp theApp;


// CView3DExamplesApp �ʱ�ȭ

BOOL CView3DExamplesApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(_T("View3DExamples examples"));

	// GUI Manager �ʱ�ȭ ��ó���� �����մϴ�.
	CGUIManager::PreInitialize();

	// ���� ������Ƽ �޴��� ����մϴ�.
	CGUIManager::RegisterMenu(CPropertyView3DExamples(), L"View3D Example", L"Menu", false);


	// GUI �󿡼� ���� �並 �����մϴ�.
	std::vector<CGUIFixedViewDeclaration*> vctFixedViewDecl;

	for(int32_t i = 0; i < 4; ++i)
	{
		CGUIFixedViewDeclaration* pDeclarationCam = new CGUIFixedViewDeclaration;
		pDeclarationCam->SetMultiDocTemplateName("CGUIDoc3DCGUIChildFrame3DCGUIView3D");
		pDeclarationCam->SetViewName(L"3D View");
		CGUIManagerView::AddFixedViewDeclaration(pDeclarationCam);
		vctFixedViewDecl.push_back(pDeclarationCam);
	}

	// GUI �󿡼� ���� �迭�� �����մϴ�.
	CGUIFixedViewPlacement fvp;

	CGUIFixedViewPlacementSet fvpSet1(false);
	CGUIFixedViewPlacementSet fvpSet2(false);
	CGUIFixedViewPlacementSet fvpSet3(false);
	CGUIFixedViewPlacementSet fvpSet4(false);

	fvpSet1.SetName(L"Prompt 3D View Set 1");
	fvpSet2.SetName(L"Prompt 3D View Set 2");
	fvpSet3.SetName(L"Prompt 3D View Set 3");
	fvpSet4.SetName(L"Prompt 3D View Set 4");

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

	CGUIManager::PreInitializePaneVisibility(true, true, false, false, false);

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

	// "View3D Example" �̶�� �̸��� �޴� ������ ã��
	CPropertyView3DExamples* pPropertyMenu = dynamic_cast<CPropertyView3DExamples*>(CGUIManager::GetMenuItem(L"View3D Example", L"Menu"));

	// "View3D Example" �̶�� �̸��� �޴� �������� ���� ���, ������Ƽ â ����
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
