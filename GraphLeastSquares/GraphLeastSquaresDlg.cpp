
// GraphLeastSquaresDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphLeastSquares.h"
#include "GraphLeastSquaresDlg.h"
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


// CGraphLeastSquaresDlg dialog




CGraphLeastSquaresDlg::CGraphLeastSquaresDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphLeastSquaresDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraphLeastSquaresDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGraphLeastSquaresDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GRAPH_ADD, &CGraphLeastSquaresDlg::OnBnClickedButtonGraphAdd)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH_CLEAR, &CGraphLeastSquaresDlg::OnBnClickedButtonGraphClear)
END_MESSAGE_MAP()


// CGraphLeastSquaresDlg message handlers

BOOL CGraphLeastSquaresDlg::OnInitDialog()
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

	// �׷��� �� ����
	CResult res = m_viewGraph.CreateAndFitParent((size_t)GetDlgItem(IDC_STATIC_GRAPH_VIEW)->GetSafeHwnd());

	if(res.IsFail())
		ErrorMessageBox(res);

	SetDlgItemText(IDC_EDIT_GRAPH_PLOT_NAME, L"Example");
	SetDlgItemText(IDC_EDIT_GRAPH_LEAST_SQUARES_DEGREE, L"1");

	OnBnClickedButtonGraphAdd();

	UpdateControls();

	SetTimer(1024, 100, nullptr);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CGraphLeastSquaresDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if(pMsg->wParam == VK_RETURN)
				OnBnClickedButtonGraphAdd();

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGraphLeastSquaresDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGraphLeastSquaresDlg::OnPaint()
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
HCURSOR CGraphLeastSquaresDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGraphLeastSquaresDlg::UpdateControls()
{
	// �׷��� �� ��ȿ�� üũ
	GetDlgItem(IDC_BUTTON_GRAPH_ADD)->EnableWindow(m_viewGraph.IsAvailable());

	// �׷��� �� ��ȿ�� üũ, �׷��� �� ������ ���� ���� üũ
	GetDlgItem(IDC_BUTTON_GRAPH_CLEAR)->EnableWindow(m_viewGraph.IsAvailable() && m_viewGraph.DoesGraphExist());
}

void CGraphLeastSquaresDlg::OnTimer(UINT_PTR nIDEvent)
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


BOOL CGraphLeastSquaresDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// �׷��� �並 �����Ѵ�.
	m_viewGraph.Destroy();

	return CDialogEx::DestroyWindow();
}


void CGraphLeastSquaresDlg::OnBnClickedButtonGraphAdd()
{
	// TODO: Add your control notification handler code here
	CFLString<wchar_t> flstrInfo = L"";

	srand((unsigned int)time(nullptr));

	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		uint32_t ui32Color = COLOR(rand()%256, rand()%256, rand()%256);

		CEdit* pEditName = (CEdit*)GetDlgItem(IDC_EDIT_GRAPH_PLOT_NAME);
		CEdit* pEditDegree = (CEdit*)GetDlgItem(IDC_EDIT_GRAPH_LEAST_SQUARES_DEGREE);

		if(!pEditName || !pEditDegree)
			break;

		CString strName = _T("");
		pEditName->GetWindowText(strName);

		CString strDegree = _T("");
		pEditDegree->GetWindowText(strDegree);

		CFLString<wchar_t> flstrName(strName);
		int32_t i32Degree = _wtoi(strDegree);

		// �������� 100���� �����͸� �����Ѵ�.
		const size_t stDataCount = 100;
		double arrF64DataX[stDataCount] = { 0., };
		double arrF64DataY[stDataCount] = { 0., };

		double f64PrevX = 0.;
		double f64PrevY = 0.;

		for(size_t i = 0; i < stDataCount; ++i)
		{
			arrF64DataX[i] = f64PrevX + ((rand() % 100) / 10.);
			if(rand() % 2)
				arrF64DataY[i] = f64PrevY + ((rand() % 100) / 10.);
			else
				arrF64DataY[i] = f64PrevY - ((rand() % 100) / 10.);

			f64PrevX = arrF64DataX[i];
			f64PrevY = arrF64DataY[i];
		}

		// �׷����� ������ �����͸� �߰��Ѵ�.
		m_viewGraph.Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Scatter, ui32Color, flstrName, nullptr);

		if(i32Degree == 0)
		{
			flstrInfo = L"Please check the degree.";
			break;
		}

		// LeastSquaresD ��ü ���� // Create LeastSquaresD object
		CLeastSquaresD ls;
		// �����͸� �Ҵ�
		ls.Assign(arrF64DataX, arrF64DataY, stDataCount);

		// ��� ���� �ޱ� ���� FLArray ����
		CFLArray<double> vctOutput;
		// R square ���� �ޱ� ����double ����
		double f64TRSqr = 0.;

		// ���׽� ����� ��´�.
		ls.GetPoly(i32Degree, &vctOutput, &f64TRSqr);

		CFLString<wchar_t> flstrEquation = L"";

		int64_t i64Count = vctOutput.GetCount();
		if(!i64Count)
		{
			flstrInfo = L"Empty result";
			break;
		}

		// ������ ���������� ����� ������ ���δ�.
		// ���� ��� 4������ ���, 4 + 12 = 16, �� �Ҽ��� 16° �ڸ����� ����� ǥ���Ѵ�.
		int32_t i32Precision = 12 + i32Degree;

		// ���� ����� ���׽��� �����.
		for(int32_t i = 0; i < i64Count; ++i)
		{
			double f64Coef = vctOutput.GetAt(i);
			if(f64Coef == 0.)
				continue;

			if(!flstrEquation.IsEmpty() && f64Coef > 0.)
				flstrEquation += L" + ";

			CFLString<wchar_t> flstrFormat = L"";
			if(i == (i64Count - 2))
			{
				flstrFormat.Format(L"%.*f * x", i32Precision, f64Coef);
			}
			else if(i == (i64Count - 1))
			{
				flstrFormat.Format(L"%.*f", i32Precision, f64Coef);
			}
			else
			{
				flstrFormat.Format(L"%.*f*x^%d", i32Precision, f64Coef, i64Count - 1 - i);
			}

			flstrEquation += flstrFormat;
		}

		if(flstrEquation.IsEmpty())
			break;

		flstrInfo.Format(L"R square value: %lf", f64TRSqr);

		// ���� ��ü ���� // Create ���� object
		CExpression exp;
		// ���� ���ڿ��� �����Ѵ�
		exp.SetExpression(flstrEquation);

		// �׷��� �信 ���� �����͸� �߰��Ѵ�
		m_viewGraph.Plot(&exp, ui32Color);

		// �׷��� �並 �����Ѵ�
		m_viewGraph.Invalidate();
	}
	while(false);

	GetDlgItem(IDC_EDIT_EQUATION_INFO)->SetWindowText(flstrInfo);
}


void CGraphLeastSquaresDlg::OnBnClickedButtonGraphClear()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �׷��� �� ��ȿ�� üũ
		if(!m_viewGraph.IsAvailable())
			break;

		// �׷��� ���� �����͸� �ʱ�ȭ�Ѵ�
		m_viewGraph.Clear();

		// �׷��� �並 �����Ѵ�
		m_viewGraph.Invalidate();
	}
	while(false);
}
