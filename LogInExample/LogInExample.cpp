
// LogInExample.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "LogInExample.h"

#include "PropertyLogInExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogInExampleApp

BEGIN_MESSAGE_MAP(CLogInExampleApp, CWinApp)
END_MESSAGE_MAP()


// CLogInExampleApp ����

CLogInExampleApp::CLogInExampleApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CLogInExampleApp ��ü�Դϴ�.

CLogInExampleApp theApp;


// CLogInExampleApp �ʱ�ȭ

BOOL CLogInExampleApp::InitInstance()
{
	CWinAppEx::InitInstance();
	SetRegistryKey(_T("LogInExample examples"));

	// GUI Manager �ʱ�ȭ ��ó���� �����մϴ�.
	CGUIManager::PreInitialize();

	// �α��� �޴��� �߰��մϴ�.
	CGUIManager::AddMenuItem(new CGUIMenuItemLogIn);

	// �߰��� �α��� �޴��� ���� ������ �̸��� ��� �ɴϴ�.
	Base::CFLString<wchar_t> strMenuItemParentFullPath;
	int64_t i64MenuItemCount = GUI::CGUIManager::GetMenuItemCount();

	for(int64_t i = i64MenuItemCount - 1; i >= 0; --i)
	{
		CGUIMenuItemLogIn* pMenuItemLogIn = dynamic_cast<CGUIMenuItemLogIn*>(GUI::CGUIManager::GetMenuItem(i));

		if(pMenuItemLogIn)
		{
			Base::CFLString<wchar_t> strMenuItemName = pMenuItemLogIn->GetName();
			strMenuItemParentFullPath = L"FLImaging@" + pMenuItemLogIn->GetFullPath();
			strMenuItemParentFullPath.Replace(L"@" + strMenuItemName, L"");
			break;
		}
	}

	// �α��� �޴��� Ʈ������ ��Ĩ�ϴ�.
	if(!strMenuItemParentFullPath.IsEmpty())
	{
		bool bExpand = true;
		bool bRecursive = true;
		bool bAllowToLoad = false;
		CGUIManager::AddPredefinedMenuItemExpandSetting(strMenuItemParentFullPath, bExpand, bRecursive, bAllowToLoad);
	}

	CGUIManager::AddPredefinedMenuItemExpandSetting(L"FLImaging@Menu", true, true, false);
	CGUIManager::AddPredefinedMenuItemExpandSetting(L"FLImaging@View Manager", false, true, false);
	CGUIManager::AddPredefinedMenuItemExpandSetting(L"FLImaging@Model Manager", false, true, false);

	// ���� ������Ƽ �޴��� ����մϴ�.
	CGUIManager::RegisterMenu(CPropertyLogInExample(), L"Log In Property Example", L"Menu", false);

	// GUI �󿡼� ���� �並 �����մϴ�.
	CGUIFixedViewDeclaration* pDeclarationCam = new CGUIFixedViewDeclaration;
	pDeclarationCam->SetMultiDocTemplateName("CGUIDocImageCGUIChildFrameImageCGUIViewImage");
	pDeclarationCam->SetViewName(L"Image View");
	CGUIManagerView::AddFixedViewDeclaration(pDeclarationCam);

	// GUI �󿡼� ���� �迭�� �����մϴ�.
	CGUIFixedViewPlacement fvp;
	fvp.SetFixedViewDeclaration(pDeclarationCam);
	fvp.SetPlacement(CFLRect<double>(0., 0., 1., 1.));

	CGUIFixedViewPlacementSet fvpSet1(false);
	fvpSet1.SetName(L"Prompt Image View Set 1");
	fvpSet1.AddFixedViewPlacement(fvp);

	CGUIManagerView::AddFixedViewPlacementSet(fvpSet1);

	// �ε����� �ش��ϴ� ���� �������� �����մϴ�.
	CGUIManagerView::SelectFixedViewPlacementSet(0);

	CGUIManager::PreInitializePaneVisibility(true, false, false, false, false);

	// �� �����ڸ� �ʱ�ȭ �մϴ�.
	CGUIManagerModel::Initialize();

	// GUI �����ڸ� �ʱ�ȭ �մϴ�.
	CGUIManager::Initialize();

	// Menu tree �� expand ������ �ҷ� �ɴϴ�.
	CGUIManager::LoadMenuItemExpandSetting();

	// Main Frame ��ġ�� �����մϴ�.
	CGUIMainFrame* pMF = nullptr;

	if(AfxGetApp() && AfxGetApp()->m_pMainWnd)
		pMF = dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd);

	if(pMF)
	{
 		pMF->ShowWindow(SW_SHOWMAXIMIZED);
		pMF->ModifyStyle(WS_MAXIMIZEBOX, 0, 0);
		pMF->SetWindowPos(NULL, 0, 0, 1280, 1024, NULL);
	}

	// "Log In Property Example" �̶�� �̸��� �޴� ������ ã��
	CPropertyLogInExample* pPropertyMenu = dynamic_cast<CPropertyLogInExample*>(CGUIManager::GetMenuItem(L"Log In Property Example", L"Menu"));

	// "Log In Property Example" �̶�� �̸��� �޴� �������� ���� ���, ������Ƽ â ����
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
