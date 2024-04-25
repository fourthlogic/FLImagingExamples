
// GraphViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphView.h"
#include "GraphViewDlg.h"
#include "afxdialogex.h"
#include <time.h>

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
	enum { IDD = IDD_ABOUTBOX };

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


// CGraphViewDlg dialog




CGraphViewDlg::CGraphViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphViewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLockControls = false;
}

void CGraphViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGraphViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_GRAPH_VIEW, &CGraphViewDlg::OnBnClickedButtonOpenGraphView)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE_GRAPH_VIEW, &CGraphViewDlg::OnBnClickedButtonTerminateGraphView)
	ON_BN_CLICKED(IDC_BUTTON_CHART_ADD, &CGraphViewDlg::OnBnClickedButtonChartAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHART_CLEAR, &CGraphViewDlg::OnBnClickedButtonChartClear)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_GRAPH, &CGraphViewDlg::OnBnClickedButtonLoadGraph)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_GRAPH, &CGraphViewDlg::OnBnClickedButtonSaveGraph)
END_MESSAGE_MAP()


// CGraphViewDlg message handlers

BOOL CGraphViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
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
	CComboBox* pComboBoxChartType = (CComboBox*)GetDlgItem(IDC_COMBO_CHART_TYPE);
	pComboBoxChartType->Clear();

	pComboBoxChartType->AddString(_T("Bar"));
	pComboBoxChartType->AddString(_T("Line"));
	pComboBoxChartType->AddString(_T("Scatter"));

	pComboBoxChartType->SetCurSel(0);

	UpdateControls();

	SetTimer(1024, 100, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGraphViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void CGraphViewDlg::OnPaint()
{
	if (IsIconic())
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
HCURSOR CGraphViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGraphViewDlg::UpdateControls()
{
	if(m_bLockControls)
	{
		GetDlgItem(IDC_BUTTON_OPEN_GRAPH_VIEW)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_TERMINATE_GRAPH_VIEW)->EnableWindow(false);

		GetDlgItem(IDC_BUTTON_LOAD_GRAPH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SAVE_GRAPH)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_CHART_NAME)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_CHART_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CHART_ADD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CHART_CLEAR)->EnableWindow(false);
	}
	// �׷��� �� ��ȿ�� üũ
	else if(!m_viewGraph.IsAvailable())
	{
		GetDlgItem(IDC_BUTTON_OPEN_GRAPH_VIEW)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_TERMINATE_GRAPH_VIEW)->EnableWindow(false);

		GetDlgItem(IDC_BUTTON_LOAD_GRAPH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SAVE_GRAPH)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_CHART_NAME)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_CHART_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CHART_ADD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CHART_CLEAR)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPEN_GRAPH_VIEW)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_TERMINATE_GRAPH_VIEW)->EnableWindow(true);

		GetDlgItem(IDC_BUTTON_LOAD_GRAPH)->EnableWindow(true);

		// �׷��� ��Ʈ ������/���� �������� ���� ������ ��� �´�.
		if(m_viewGraph.DoesGraphExist())
		{
			GetDlgItem(IDC_BUTTON_SAVE_GRAPH)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_CHART_CLEAR)->EnableWindow(true);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_SAVE_GRAPH)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_CHART_CLEAR)->EnableWindow(false);
		}

		GetDlgItem(IDC_EDIT_CHART_NAME)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_CHART_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CHART_ADD)->EnableWindow(true);
	}
}

void CGraphViewDlg::LockControls(bool bLock)
{
	m_bLockControls = bLock;
	UpdateControls();
}

void CGraphViewDlg::OnTimer(UINT_PTR nIDEvent)
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


BOOL CGraphViewDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// �׷��� �並 �����Ѵ�.
	m_viewGraph.Destroy();

	return CDialogEx::DestroyWindow();
}

void CGraphViewDlg::OnBnClickedButtonOpenGraphView()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(m_viewGraph.IsAvailable())
			break;

		// �׷��� �� ����
		CResult res = m_viewGraph.Create(0, 0, 500, 500);

		if(res.IsFail())
			ErrorMessageBox(res);
	}
	while (false);
}


void CGraphViewDlg::OnBnClickedButtonTerminateGraphView()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		// �׷��� �並 �����Ѵ�.
		CResult res = m_viewGraph.Destroy();

		if(res.IsFail())
			ErrorMessageBox(res);
	}
	while (false);
}


void CGraphViewDlg::OnBnClickedButtonChartAdd()
{
	// TODO: Add your control notification handler code here
	srand((unsigned int)time(nullptr));

	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		CEdit* pEditChartName = (CEdit*)GetDlgItem(IDC_EDIT_CHART_NAME);
		if(!pEditChartName)
			break;

		// �Է��� ��Ʈ�̸��� ���´�.
		CString strChartName = _T("");
		pEditChartName->GetWindowText(strChartName);

		CFLString<wchar_t> flstrChartName(strChartName);
		if(flstrChartName.IsEmpty())
			flstrChartName = L"Chart";

		CComboBox* pComboBoxChartType = (CComboBox*)GetDlgItem(IDC_COMBO_CHART_TYPE);
		if(!pComboBoxChartType)
			break;

		// ������ ��ƮŸ���� ���´�.
		EChartType eChartType = (EChartType)(pComboBoxChartType->GetCurSel() + 1);

		// �������� 10���� �����͸� �����Ѵ�.
		const size_t stDataCount = 10;
		double arrF64DataX1[stDataCount] = { 0., };
		double arrF64DataY1[stDataCount] = { 0., };

		for(size_t i = 0; i < stDataCount; ++i)
		{
			arrF64DataX1[i] = (double)(rand() % 100);
			arrF64DataY1[i] = (double)(rand() % 100);
		}

		// �׷����� ������ �����͸� �߰��Ѵ�.
		m_viewGraph.Plot(arrF64DataX1, arrF64DataY1, stDataCount, eChartType, COLOR(rand()%255, rand()%255, rand()%255), flstrChartName, nullptr);

		// �׷��� �並 ���� �Ѵ�.
		m_viewGraph.Invalidate();
	}
	while (false);
}


void CGraphViewDlg::OnBnClickedButtonChartClear()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		// �׷����� �����͸� Ŭ�����Ѵ�.
		m_viewGraph.Clear();

		// �׷��� �並 ���� �Ѵ�.
		m_viewGraph.Invalidate();
	}
	while (false);
}


void CGraphViewDlg::OnBnClickedButtonLoadGraph()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		LockControls(true);

		// �׷��� ���� �ε� ���̾�α׸� Ȱ��ȭ ��Ų��.
		// ���� ������ �Ķ���ͷ� �ε� �ɼ��� �����Ѵ�.
		// ex) EViewGraphLoadOption_Load -> �׷��� ����
		//     EViewGraphLoadOption_Load | EViewGraphLoadOption_OpenDialog �׷��� ���� �ε� ���̾�α� Ȱ��ȭ
		m_viewGraph.Load(nullptr, EViewGraphLoadOption_Load | EViewGraphLoadOption_OpenDialog);

		// �׷��� �並 ���� �Ѵ�.
		m_viewGraph.Invalidate();

		LockControls(false);
	}
	while (false);
}


void CGraphViewDlg::OnBnClickedButtonSaveGraph()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		LockControls(true);

		// �׷��� ���� ���̾�α׸� Ȱ��ȭ ��Ų��.
		m_viewGraph.Save();

		LockControls(false);
	}
	while (false);
}
