﻿
// DialogProgressExample.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "DialogProgressExample.h"

#include "PropertyMenuDialogProgressExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialogProgressExampleApp

BEGIN_MESSAGE_MAP(CDialogProgressExampleApp, CWinApp)
END_MESSAGE_MAP()


// CDialogProgressExampleApp 생성

CDialogProgressExampleApp::CDialogProgressExampleApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CDialogProgressExampleApp 개체입니다.

CDialogProgressExampleApp theApp;


// CDialogProgressExampleApp 초기화

BOOL CDialogProgressExampleApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(L"Fourth Logic Incorporated");

	// GUI Manager 초기화 전처리를 수행합니다.
	CGUIManager::PreInitialize();

	// 이미지 뷰 생성 메뉴를 추가합니다.
	CGUIManager::AddMenuItem(new CGUIMenuItemCreateViewImage);

	// 예제 프로퍼티 메뉴를 등록합니다.
	CGUIManager::RegisterMenu(CPropertyMenuDialogProgressExample(), L"Progress Dialog Example", L"Menu", false);

	// GUI 상에서 사용될 뷰를 생성합니다.
	std::vector<CGUIFixedViewDeclaration*> vctFixedViewDecl;

	for(int32_t i = 0; i < 4; ++i)
	{
		CGUIFixedViewDeclaration* pDeclarationCam = new CGUIFixedViewDeclaration;
		pDeclarationCam->SetMultiDocTemplateName("CGUIDocImageCGUIChildFrameImageCGUIViewImage");
		pDeclarationCam->SetViewName(L"Image View");
		CGUIManagerView::AddFixedViewDeclaration(pDeclarationCam);
		vctFixedViewDecl.push_back(pDeclarationCam);
	}

	// GUI 상에서 뷰의 배열을 설정합니다.
	CGUIFixedViewPlacement fvp;
	CGUIFixedViewPlacementSet fvpSet1(false);
	fvpSet1.SetName(L"Prompt Image View Set 1");

	CGUIFixedViewDeclaration* pDeclarationCam = vctFixedViewDecl[0];
	fvp.SetFixedViewDeclaration(pDeclarationCam);
	fvp.SetPlacement(CFLRect<double>(0.0, 0.0, 1.0, 1.0));
	fvpSet1.AddFixedViewPlacement(fvp);

	CGUIManagerView::AddFixedViewPlacementSet(fvpSet1);

	// 인덱스에 해당하는 뷰의 설정으로 선택합니다.
	CGUIManagerView::SelectFixedViewPlacementSet(0);

	CGUIManager::PreInitializePaneVisibility(true, false, false, false, false);

	// 모델 관리자를 초기화 합니다.
	CGUIManagerModel::Initialize();

	// GUI 관리자를 초기화 합니다.
	CGUIManager::Initialize();

	// Main Frame 위치를 설정합니다.
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

	CPropertyMenuDialogProgressExample* pPropertyMenu = dynamic_cast<CPropertyMenuDialogProgressExample*>(CGUIManager::GetMenuItem(L"Progress Dialog Example", L"Menu"));

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
