
// View3DIntoDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "View3DIntoDialog.h"
#include "View3DIntoDialogDlg.h"
#include "afxdialogex.h"

#include "../CommomHeader/ErrorMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum
	{
		IDD = IDD_ABOUTBOX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CView3DIntoDialogDlg dialog


CView3DIntoDialogDlg::CView3DIntoDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CView3DIntoDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CView3DIntoDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CView3DIntoDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GET_HEIGHT_PROFILE, &CView3DIntoDialogDlg::OnBnClickedButtonGetHeightProfile)
END_MESSAGE_MAP()


// CView3DIntoDialogDlg message handlers

BOOL CView3DIntoDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 3D 뷰 생성

	CRect crView3D;

	// IDC_STATIC_VIEW3D 의 클라이언트 사각형을 얻어온다.
	GetDlgItem(IDC_STATIC_VIEW3D)->GetClientRect(crView3D);

	// 3D 뷰 생성
	CResult res = m_view3D.Create(0, 0, 1024, 1024);

	if(res.IsFail())
		ErrorMessageBox(res);

	// 3D 뷰의 윈도우를 얻어온다.
	HWND hWndView3D = (HWND)m_view3D.GetWindowHandle();

	if(hWndView3D)
	{
		// 3D 뷰의 부모를 IDC_STATIC_VIEW3D 로 지정한다.
		::SetParent(hWndView3D, GetDlgItem(IDC_STATIC_VIEW3D)->GetSafeHwnd());

		// 3D 뷰의 윈도우 위치와 크기를 IDC_STATIC_VIEW3D 크기에 맞게 설정한다.
		::MoveWindow(hWndView3D, 0, 0, crView3D.Width(), crView3D.Height(), true);
	}

	// 높이 맵 이미지와 텍스쳐 이미지 로드 // Load height map image and texture
	m_view3D.Load(L"../../ExampleImages/View3D/mountain.flif", L"../../ExampleImages/View3D/mountain_texture.flif");

	UpdateControls();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HP_SX);
	pEdit->SetWindowTextW(L"0");
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HP_SY);
	pEdit->SetWindowTextW(L"0");
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HP_EX);
	pEdit->SetWindowTextW(L"104");
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HP_EY);
	pEdit->SetWindowTextW(L"120");
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT_PROFILE);
	pEdit->SetWindowTextW(L"");

	SetTimer(1024, 100, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CView3DIntoDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CView3DIntoDialogDlg::OnPaint()
{
	if(IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CView3DIntoDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CView3DIntoDialogDlg::UpdateControls()
{
	bool bEnable = false;

	do
	{
		// 3D 뷰 유효성 체크
		if(!m_view3D.IsAvailable())
			break;

		bEnable = true;
	}
	while(false);
}

void CView3DIntoDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1024:
		{
			UpdateControls();
		}
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CView3DIntoDialogDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// 3D 뷰를 종료한다.
	m_view3D.Destroy();

	return CDialogEx::DestroyWindow();
}

void CView3DIntoDialogDlg::OnBnClickedButtonGetHeightProfile()
{
	do
	{
		// 3D 뷰 유효성 체크
		if(!m_view3D.IsAvailable())
			break;

		long i64StartX, i64StartY, i64EndX, i64EndY;
		CString strHeightValue;

		// Edit Box에 있는 숫자값을 long으로 변환하여 시작좌표와 끝좌표 얻어 오기
		GetDlgItem(IDC_EDIT_HP_SX)->GetWindowText(strHeightValue);
		i64StartX = _wtoi(strHeightValue);
		GetDlgItem(IDC_EDIT_HP_SY)->GetWindowText(strHeightValue);
		i64StartY = _wtoi(strHeightValue);
		GetDlgItem(IDC_EDIT_HP_EX)->GetWindowText(strHeightValue);
		i64EndX = _wtoi(strHeightValue);
		GetDlgItem(IDC_EDIT_HP_EY)->GetWindowText(strHeightValue);
		i64EndY = _wtoi(strHeightValue);

		// 시작좌표와 끝좌표 설정
		CFLPoint<int64_t> flp1(i64StartX, i64StartY);
		CFLPoint<int64_t> flp2(i64EndX, i64EndY);
		CFLArrayD flaD;
		
		// 시작좌표와 끝좌표를 잇는 선분에 대한 Height Profile 얻기
		m_view3D.GetHeightProfile(&flp1, &flp2, &flaD);

		// 얻은 Height Profile 을 화면에 디스플레이
		CFLString<wchar_t> flstr, flstrIndex, flstrVal;

		for(int64_t i = 0; i < flaD.GetCount(); ++i)
		{
			flstr.AppendFormat(L"[%d] ", i);
			flstr.AppendFormat(L"%.3lf \r\n", flaD.GetAt(i));
		}

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT_PROFILE);
		pEdit->SetWindowText(flstr.GetString());

		m_view3D.Invalidate();
	}
	while(false);
}