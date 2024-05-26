
// ImageViewIntoDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageViewIntoDialog.h"
#include "ImageViewIntoDialogDlg.h"
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


// CImageViewIntoDialogDlg dialog




CImageViewIntoDialogDlg::CImageViewIntoDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageViewIntoDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageViewIntoDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageViewIntoDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_CREATE, &CImageViewIntoDialogDlg::OnBnClickedButtonFigureObjectCreate)
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT, &CImageViewIntoDialogDlg::OnBnClickedButtonFigureObjectPopFront)
END_MESSAGE_MAP()


// CImageViewIntoDialogDlg message handlers

BOOL CImageViewIntoDialogDlg::OnInitDialog()
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
	// �̹��� �� ���� // Create image view

	CRect crImageView;

	// IDC_STATIC_IMAGE_VIEW �� Ŭ���̾�Ʈ �簢���� ���´�.
	GetDlgItem(IDC_STATIC_IMAGE_VIEW)->GetClientRect(crImageView);

	// �̹��� �� ���� // Create image view
	CResult res = m_viewImage.Create(0, 0, 1024, 1024);

	if(res.IsFail())
		ErrorMessageBox(res);

	// �̹��� ���� �������� ���´�.
	HWND hWndImageView = (HWND)m_viewImage.GetWindowHandle();

	if(hWndImageView)
	{
		// �̹��� ���� �θ� IDC_STATIC_IMAGE_VIEW �� �����Ѵ�.
		::SetParent(hWndImageView, GetDlgItem(IDC_STATIC_IMAGE_VIEW)->GetSafeHwnd());

		// �̹��� ���� ������ ��ġ�� ũ�⸦ IDC_STATIC_IMAGE_VIEW ũ�⿡ �°� �����Ѵ�.
		::MoveWindow(hWndImageView, 0, 0, crImageView.Width(), crImageView.Height(), true);
	}

	UpdateControls();

	SetTimer(1024, 100, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageViewIntoDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageViewIntoDialogDlg::OnPaint()
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
HCURSOR CImageViewIntoDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageViewIntoDialogDlg::UpdateControls()
{
	bool bEnable = false;

	do
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// �̹��� ���� Figure object ������ ���´�.
		if(!m_viewImage.GetFigureObjectCount())
			break;

		bEnable = true;
	}
	while(false);

	GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT)->EnableWindow(bEnable);
}

void CImageViewIntoDialogDlg::OnTimer(UINT_PTR nIDEvent)
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


BOOL CImageViewIntoDialogDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// �̹��� �並 �����Ѵ�.
	m_viewImage.Destroy();

	return CDialogEx::DestroyWindow();
}

void CImageViewIntoDialogDlg::OnBnClickedButtonFigureObjectCreate()
{
	// TODO: Add your control notification handler code here

	do
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// �̹��� ���� ĵ���� ������ ���´�.
		CFLRect<int32_t> flrlCanvas = m_viewImage.GetClientRectCanvasRegion();

		// ĵ���� ������ ��ǥ�踦 �̹��� ������ ��ǥ��� ��ȯ�Ѵ�.
		CFLRect<double> flrdImage = m_viewImage.ConvertCanvasCoordToImageCoord(flrlCanvas);

		// �̹��� ������ �������� ������ Figure �� ũ��� ����� �簢������ �����Ѵ�.
		double f64Width = flrdImage.GetWidth() / 10.;
		double f64Height = flrdImage.GetHeight() / 10.;
		double f64Size = __min(f64Width, f64Height);

		CFLPoint<double> flpdCenter(0, 0);
		flrdImage.GetCenter(flpdCenter);

		CFLRect<double> flrdFigure(flpdCenter.x - f64Size, flpdCenter.y - f64Size, flpdCenter.x + f64Size, flpdCenter.y + f64Size);

		// �̹��� �信 Figure object �� �����Ѵ�.
		// ���� ������ �Ķ���ʹ� Ȱ��ȭ �Ǵ� �޴��� �����̸�, EAvailableFigureContextMenu_All �� �⺻ �޴��� Ȱ��ȭ �Ѵ�.
		// Ȱ��ȭ �ϰ��� �ϴ� �޴��� �߰� Ȥ�� ���� �ϱ� ���ؼ��� enum ���� ��Ʈ �������� �־��ָ� �ȴ�.
		// ex) EAvailableFigureContextMenu_None -> Ȱ��ȭ �Ǵ� �޴� ����
		//     EAvailableFigureContextMenu_All -> ��ü �޴� Ȱ��ȭ
		//     EAvailableFigureContextMenu_DeclType | EAvailableFigureContextMenu_TemplateType -> Decl Type, Template Type ��ȯ �޴� Ȱ��ȭ
		m_viewImage.PushBackFigureObject(&flrdFigure, EAvailableFigureContextMenu_All);
	}
	while(false);
}


void CImageViewIntoDialogDlg::OnBnClickedButtonFigureObjectPopFront()
{
	// TODO: Add your control notification handler code here
	CFLFigure* pFlFigure = nullptr;
	CString strFigureInfo = _T("Error");

	do
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// �̹��� ���� �� ���� Figure �� �����ϸ鼭 ���´�.
		pFlFigure = m_viewImage.PopFrontFigureObject();
		if(!pFlFigure)
			break;

		// Figure �� ���ڿ��� ���´�.
		CFLString<wchar_t> flStrFigure = CFigureUtilities::ConvertFigureObjectToString(pFlFigure);

		strFigureInfo = flStrFigure;
	}
	while(false);

	GetDlgItem(IDC_EDIT_FIGURE_OBEJCT_INFO)->SetWindowText(strFigureInfo);

	// ���� Figure ��ü ����
	if(pFlFigure)
	{
		delete pFlFigure;
		pFlFigure = nullptr;
	}
}