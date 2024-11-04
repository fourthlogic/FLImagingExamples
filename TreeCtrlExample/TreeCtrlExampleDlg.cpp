
// InspUnderfillExDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TreeCtrlExampleDlg.h"
#include "PropertyMenuLight.h"
#include "PropertyMenuCamera.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTreeCtrlExampleDlg, CDialogEx)

CTreeCtrlExampleDlg::CTreeCtrlExampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TREECTRLEXAMPLE_DIALOG, pParent)
	, m_vctItems(*new std::vector<CGUIMenuItemBase*>)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTreeCtrlExampleDlg::~CTreeCtrlExampleDlg()
{
	for(size_t i = 0; i < m_vctItems.size(); i++)
	{
		if(m_vctItems[i])
		{
			delete m_vctItems[i];
			m_vctItems[i] = nullptr;
		}
	}

	m_vctItems.clear();
	delete& m_vctItems;
}

BEGIN_MESSAGE_MAP(CTreeCtrlExampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, ID_MENUTREE, &CTreeCtrlExampleDlg::OnNMDblclk)
	ON_NOTIFY(NM_RCLICK, ID_MENUTREE, &CTreeCtrlExampleDlg::OnNMRClick)
	ON_MESSAGE(ETreeCtrlExampleMessage_DeleteMenuItem, &CTreeCtrlExampleDlg::OnDeleteMenuItem)
END_MESSAGE_MAP()

void CTreeCtrlExampleDlg::AddMenuItem(FLImaging::GUI::CGUIMenuItemBase* pMenuItem)
{
	m_vctItems.push_back(pMenuItem);
}

const CResult CTreeCtrlExampleDlg::BuildTree()
{
	CResult cr;

	do
	{
		// 트리 뷰 사이즈
		CRect rectDummy(0, 0, 400, 300);

		// 트리 뷰를 만듭니다.
		const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_FULLROWSELECT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | WS_HSCROLL | WS_VSCROLL;

		if(!m_treeCtrl.Create(dwViewStyle, rectDummy, this, ID_MENUTREE))
		{
			TRACE0("트리 뷰를 만들지 못했습니다.\n");
			cr = EResult_FailedToCreate;
			break;
		}

		m_treeCtrl.ModifyStyleEx(0, WS_EX_TRANSPARENT);

		// 다크 테마를 사용하려면 true,
		// 다크 테마를 사용하지 않으려면 false 를 설정합니다.
		bool bUseDarkMode = true;

		// 다크 테마
		if(bUseDarkMode)
		{
			SetWindowTheme(m_treeCtrl.GetSafeHwnd(), L"DarkMode_Explorer", NULL);
			SetWindowTheme(GetSafeHwnd(), L"DarkMode_Explorer", NULL);

			// 트리 아이콘 로드
			m_treeCtrl.SetDefaultImageList();
		}
		else
		{
			// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
			SetWindowTheme(m_treeCtrl.GetSafeHwnd(), L"Explorer", NULL);
			SetWindowTheme(GetSafeHwnd(), L"Explorer", NULL);
		}

		// root item 등록
		HTREEITEM hRoot = m_treeCtrl.InsertItem(L"Root Menu", (int32_t)ETreeItemIcon_CloseFolderHorz, (int32_t)ETreeItemIcon_CloseFolderHorz_Selected);

		// Example Path 1@Example Path A
		HTREEITEM hNewParent = m_treeCtrl.InsertItem(L"Example Path 1", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hRoot, 0);
		hNewParent = m_treeCtrl.InsertItem(L"Example Path A", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hNewParent, 0);

		// Example Path 2@Example Path B
		hNewParent = m_treeCtrl.InsertItem(L"Example Path 2", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hRoot, 0);
		hNewParent = m_treeCtrl.InsertItem(L"Example Path B", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hNewParent, 0);

		// Example Path 3@Example Path C
		HTREEITEM hExamplePath3 = m_treeCtrl.InsertItem(L"Example Path 3", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hRoot, 0);
		HTREEITEM hExamplePathC = m_treeCtrl.InsertItem(L"Example Path C", (int32_t)ETreeItemIcon_OpenFolderVert, (int32_t)ETreeItemIcon_OpenFolderVert_Selected, hExamplePath3, 0);

		// Light Item 추가
		{
			CPropertyMenuLight* pItem = new CPropertyMenuLight;
			pItem->SetParentTree(&m_treeCtrl);
			pItem->SetParentDialog(this);
			pItem->SetPath(L"Example Path 3@Example Path C");
			pItem->SetName(L"Light Item");
			m_vctItems.push_back(pItem);

			HTREEITEM hCurItem = m_treeCtrl.InsertItem(pItem->GetName(), (int32_t)ETreeItemIcon_Light, (int32_t)ETreeItemIcon_Light_Selected, hExamplePathC, 0);

			if(hCurItem)
				m_treeCtrl.SetItemData(hCurItem, (DWORD_PTR)pItem);
		}

		// Camera Item 추가
		{
			CPropertyMenuCamera* pItem = new CPropertyMenuCamera;
			pItem->SetParentTree(&m_treeCtrl);
			pItem->SetParentDialog(this);
			pItem->SetPath(L"Example Path 3@Example Path C");
			pItem->SetName(L"Camera Item");
			m_vctItems.push_back(pItem);

			HTREEITEM hCurItem = m_treeCtrl.InsertItem(pItem->GetName(), (int32_t)ETreeItemIcon_Camera, (int32_t)ETreeItemIcon_Camera_Selected, hExamplePathC, 0);

			if(hCurItem)
				m_treeCtrl.SetItemData(hCurItem, (DWORD_PTR)pItem);
		}

		// 트리의 root 아이템을 expand
		m_treeCtrl.Expand(hRoot, TVE_EXPAND);

		// 트리의 Example Path 3 아이템을 하위 아이템까지 재귀적으로 expand
		m_treeCtrl.RecursiveExpand(hExamplePath3, TVE_EXPAND, false);

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

// CTreeCtrlExampleDlg 메시지 처리기

BOOL CTreeCtrlExampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if(!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ModifyStyleEx(0, WS_EX_COMPOSITED);
	BuildTree();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTreeCtrlExampleDlg::OnPaint()
{
	if(IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTreeCtrlExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTreeCtrlExampleDlg::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	do
	{
		// 현재 마우스 스크린좌표 얻기
		POINT point;
		::GetCursorPos(&point);

		// 트리 컨트롤의 좌표로 변환
		m_treeCtrl.ScreenToClient(&point);

		// 트리 컨트롤에 대해 마우스 포인트 Hit Test
		HTREEITEM hItem = m_treeCtrl.HitTest(point);

		// 마우스 좌표에 의해 선택된 아이템이 없다면 함수 종료
		if(!hItem)
			break;

		// 선택한 아이템의 텍스트 영역 얻기
		CRect rcItemTextOnly;
		m_treeCtrl.GetItemRect(hItem, rcItemTextOnly, true);

		// 텍스트 영역을 클릭하지 않았다면 함수 종료
		if(!PtInRect(rcItemTextOnly, point))
			break;

		// 트리에서 아이템 선택
		m_treeCtrl.SelectItem(hItem);

		// 선택한 트리아이템의 ItemData 를 얻기
		CGUIMenuItemBase* pItem = dynamic_cast<CGUIMenuItemBase*>((CFLBase*)m_treeCtrl.GetItemData(hItem));

		// pItem 이 있다면, 별도로 정의한 OnLButtonDoubleClick() 함수 수행
		if(pItem)
		{
			// CPropertyMenuLight::OnLButtonDoubleClick() 또는
			// CPropertyMenuCamera::OnLButtonDoubleClick() 가 수행됩니다.
			pItem->OnLButtonDoubleClick();
		}
		// pItem 이 없다면, 기본 동작 수행
		else
		{
			CString str = m_treeCtrl.GetItemText(hItem);
			AfxMessageBox(L"[" + CFLString<wchar_t>(str) + L"] LButton Double Click", MB_OK);
		}
	}
	while(false);
}

void CTreeCtrlExampleDlg::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	do 
	{
		// 현재 마우스 스크린좌표 얻기
		POINT point;
		::GetCursorPos(&point);

		// 트리 컨트롤의 좌표로 변환
		m_treeCtrl.ScreenToClient(&point);

		// 트리 컨트롤에 대해 마우스 포인트 Hit Test
		HTREEITEM hItem = m_treeCtrl.HitTest(point);

		// 마우스 좌표에 의해 선택된 아이템이 없는 경우
		if(!hItem)
		{
			// 팝업 메뉴 생성
			CMenu popupMenu;
			popupMenu.CreatePopupMenu();

			// 메뉴 아이템 enum 정의
			enum ETreeMenuItem
			{
				ETreeMenuItem_BackgroundMenu1 = 1,
				ETreeMenuItem_BackgroundMenu2,
			};

			// 팝업 메뉴에 메뉴 아이템 추가
			popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_BackgroundMenu1, L"Background menu 1");
			popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_BackgroundMenu2, L"Background menu 2");

			CPoint ptPoint;
			GetCursorPos(&ptPoint);

			ETreeMenuItem eSelectedMenu = (ETreeMenuItem)popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, ptPoint.x, ptPoint.y, this);

			// 사용자가 선택한 메뉴에 따라 특정 코드 수행
			switch(eSelectedMenu)
			{
			case ETreeMenuItem_BackgroundMenu1:
				AfxMessageBox(L"Background menu 1", MB_OK);
				break;
			case ETreeMenuItem_BackgroundMenu2:
				AfxMessageBox(L"Background menu 2", MB_OK);
				break;
			}

			// if(!hItem) 분기 종료
			break;
		}

		// 선택한 아이템의 텍스트 영역 얻기
		CRect rcItemTextOnly;
		m_treeCtrl.GetItemRect(hItem, rcItemTextOnly, true);

		// 텍스트 영역을 클릭하지 않았다면 함수 종료
		if(!PtInRect(rcItemTextOnly, point))
			break;

		// 트리에서 아이템 선택
		m_treeCtrl.SelectItem(hItem);

		// 선택한 트리아이템의 ItemData 를 얻기
		CGUIMenuItemBase* pItem = dynamic_cast<CGUIMenuItemBase*>((CFLBase*)m_treeCtrl.GetItemData(hItem));

		// pItem 이 있다면, 별도로 정의한 OnRButtonClick() 함수 수행
		if(pItem)
		{
			// CPropertyMenuLight::OnRButtonClick() 또는
			// CPropertyMenuCamera::OnRButtonClick() 가 수행됩니다.
			pItem->OnRButtonClick();
		}
		// pItem 이 없다면, 기본 동작 수행
		else
		{
			// 팝업 메뉴 생성
			CMenu popupMenu;
			popupMenu.CreatePopupMenu();

			// 메뉴 아이템 enum 정의
			enum ETreeMenuItem
			{
				ETreeMenuItem_Remove = 1,
			};

			popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Remove, L"Remove");

			CPoint ptPoint;
			GetCursorPos(&ptPoint);

			ETreeMenuItem eSelectedMenu = (ETreeMenuItem)popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, ptPoint.x, ptPoint.y, this);

			switch(eSelectedMenu)
			{
			case ETreeMenuItem_Remove:
				{
					UINT u32Answer = AfxMessageBox(L"[" + CFLString<wchar_t>(m_treeCtrl.GetItemText(hItem)) + L"]\nDo you really want to remove this item?", MB_YESNO);

					if(u32Answer == IDYES)
					{
						// 트리에서 트리아이템 제거
						m_treeCtrl.DeleteItem(m_treeCtrl.GetSelectedItem());
					}
				}
				break;
			}
		}
	}
	while(false);
}

LRESULT CTreeCtrlExampleDlg::OnDeleteMenuItem(WPARAM wParam, LPARAM lParam)
{
	if(lParam != NULL)
	{
		CGUIMenuItemBase* pMenuItemBase = (CGUIMenuItemProperty*)lParam;

		if(pMenuItemBase)
		{
			auto it = std::find(m_vctItems.begin(), m_vctItems.end(), pMenuItemBase);

			if(it != m_vctItems.end())
			{
				delete pMenuItemBase;
				m_vctItems.erase(it);
			}
		}
	}

	return 0;
}