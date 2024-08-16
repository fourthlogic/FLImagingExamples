
// FigureOperationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FigureOperation.h"
#include "FigureOperationDlg.h"
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


// CFigureOperationDlg dialog




CFigureOperationDlg::CFigureOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFigureOperationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFigureOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFigureOperationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_CREATE, &CFigureOperationDlg::OnBnClickedButtonFigureObjectCreate)
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_CLEAR, &CFigureOperationDlg::OnBnClickedButtonFigureObjectClear)
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_EXECUTE, &CFigureOperationDlg::OnBnClickedButtonFigureObjectExecute)
	ON_CBN_SELCHANGE(IDC_COMBO_FIGURE_OBJECT_SELECT1, &CFigureOperationDlg::OnCbnSelchangeComboFigureObjectSelect1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIGURE_OBJECT_SELECT2, &CFigureOperationDlg::OnCbnSelchangeComboFigureObjectSelect2)
END_MESSAGE_MAP()


// CFigureOperationDlg message handlers

BOOL CFigureOperationDlg::OnInitDialog()
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
	
	// �̹��� �� ���� // Create image view
 	CResult res = m_viewImage.CreateAndFitParent((size_t)GetDlgItem(IDC_STATIC_IMAGE_VIEW)->GetSafeHwnd());
 
 	if(res.IsFail())
 		ErrorMessageBox(res);

	// �޺� �ڽ��� ���� �׸��� �����Ѵ�.
	CComboBox* pComboBoxDeclType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE);
	pComboBoxDeclType->ResetContent();

	pComboBoxDeclType->AddString(_T("Point"));
	pComboBoxDeclType->AddString(_T("Line"));
	pComboBoxDeclType->AddString(_T("Rect"));
	pComboBoxDeclType->AddString(_T("Quad"));
	pComboBoxDeclType->AddString(_T("Circle"));
	pComboBoxDeclType->AddString(_T("Ellipse"));
	pComboBoxDeclType->AddString(_T("Cubic Spline"));
	pComboBoxDeclType->AddString(_T("Region"));
	pComboBoxDeclType->AddString(_T("Complex Region"));
	pComboBoxDeclType->AddString(_T("Doughnut"));

	pComboBoxDeclType->SetCurSel(0);

	CComboBox* pComboBoxTemplateType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE);
	pComboBoxTemplateType->ResetContent();

	pComboBoxTemplateType->AddString(_T("Int32"));
	pComboBoxTemplateType->AddString(_T("Int64"));
	pComboBoxTemplateType->AddString(_T("Float"));
	pComboBoxTemplateType->AddString(_T("Double"));

	pComboBoxTemplateType->SetCurSel(3);

	CComboBox* pComboOperation = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_OPERATION);
	pComboOperation->ResetContent();

	pComboOperation->AddString(_T("Intersection"));
	pComboOperation->AddString(_T("Union"));
	pComboOperation->AddString(_T("Subtraction"));
	pComboOperation->AddString(_T("Exclusive Or"));

	pComboOperation->SetCurSel(0);

	UpdateControls();

	SetTimer(1024, 30, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFigureOperationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFigureOperationDlg::OnPaint()
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
HCURSOR CFigureOperationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFigureOperationDlg::UpdateControls()
{
	CWnd* pWndDeclType = GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE);
	CWnd* pWndTempType = GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE);
	CWnd* pWndCreate = GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_CREATE);
	CWnd* pWndClear = GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_CLEAR);
	CWnd* pWndSelect1 = GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT1);
	CWnd* pWndSelect2 = GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT2);
	CWnd* pWndOperation = GetDlgItem(IDC_COMBO_FIGURE_OBJECT_OPERATION);
	CWnd* pWndExecute = GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_EXECUTE);

	// �̹��� �� ��ȿ�� üũ
	if(!m_viewImage.IsAvailable())
	{
		pWndDeclType->EnableWindow(false);
		pWndTempType->EnableWindow(false);
		pWndCreate->EnableWindow(false);
		pWndClear->EnableWindow(false);
		pWndSelect1->EnableWindow(false);
		pWndSelect2->EnableWindow(false);
		pWndOperation->EnableWindow(false);
		pWndExecute->EnableWindow(false);
	}
	else
	{
		pWndDeclType->EnableWindow(true);
		pWndTempType->EnableWindow(true);
		pWndCreate->EnableWindow(true);

		// �̹��� ���� Figure object ������ ���´�.
		pWndClear->EnableWindow(m_viewImage.GetFigureObjectCount() ? true : false);

		pWndSelect1->EnableWindow(true);
		pWndSelect2->EnableWindow(true);
		pWndOperation->EnableWindow(true);
		pWndExecute->EnableWindow(true);
	}

	do 
	{
		CComboBox* pCombo1 = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT1);
		CComboBox* pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT2);

		if(!pCombo1 || !pCombo2)
			break;

		// �̹��� ���� Figure object ������ ���´�.
		size_t stCount = m_viewImage.GetFigureObjectCount();

		if(pCombo1->GetCount() == stCount && pCombo2->GetCount() == stCount)
			break;

		// �޺� �ڽ��� Figure Object �׸��� �����Ѵ�.
		UpdateFigureObjectList();
	}
	while (false);
}

EFigureDeclType CFigureOperationDlg::SelectedDeclType()
{
	// ���� ���õ� Decl Type �� ���´�.
	EFigureDeclType eReturn = EFigureDeclType_Unknown;

	do 
	{
		CComboBox* pComboBoxDeclType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE);
		if(!pComboBoxDeclType)
			break;

		int32_t i32CurSel = pComboBoxDeclType->GetCurSel();

		switch(i32CurSel)
		{
		case 0:
			eReturn = EFigureDeclType_Point;
			break;

		case 1:
			eReturn = EFigureDeclType_Line;
			break;

		case 2:
			eReturn = EFigureDeclType_Rect;
			break;

		case 3:
			eReturn = EFigureDeclType_Quad;
			break;

		case 4:
			eReturn = EFigureDeclType_Circle;
			break;

		case 5:
			eReturn = EFigureDeclType_Ellipse;
			break;

		case 6:
			eReturn = EFigureDeclType_CubicSpline;
			break;

		case 7:
			eReturn = EFigureDeclType_Region;
			break;

		case 8:
			eReturn = EFigureDeclType_ComplexRegion;
			break;

		case 9:
			eReturn = EFigureDeclType_Doughnut;
			break;

		default:
			break;
		}
	}
	while (false);

	return eReturn;
}

void CFigureOperationDlg::UpdateFigureObjectList()
{
	do 
	{
		CComboBox* pCombo1 = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT1);
		CComboBox* pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT2);

		if(!pCombo1 || !pCombo2)
			break;

		int32_t i32Selected1 = __max(0, pCombo1->GetCurSel());
		int32_t i32Selected2 = __max(0, pCombo2->GetCurSel());

		pCombo1->ResetContent();
		pCombo2->ResetContent();

		// �̹��� ���� Figure object ������ ���´�.
		size_t stCount = m_viewImage.GetFigureObjectCount();
		if(!stCount)
			break;

		for(size_t i = 0; i < stCount; ++i)
		{
			// �ش� �ε����� Figure Object �� ���´�.
			CFLFigure* pFigure = m_viewImage.GetFigureObject((int32_t)i);
			if(!pFigure)
				continue;

			// Figure Object �� �̸��� �����Ѵ�.
			CFLString<wchar_t> strFigureName = MakeFigureObjectName(i, pFigure);
			if(strFigureName.IsEmpty())
				break;

			// �޺� �ڽ��� �׸��� �߰��Ѵ�.
			pCombo1->AddString(strFigureName);
			pCombo2->AddString(strFigureName);

			// ���� Figure Object ����
			delete pFigure;
			pFigure = nullptr;
		}

		pCombo1->SetCurSel(i32Selected1);
		pCombo2->SetCurSel(i32Selected2);
	}
	while (false);
}

CFLString<wchar_t> CFigureOperationDlg::MakeFigureObjectName(size_t stIndex, CFLFigure* pFigure)
{
	CFLString<wchar_t> strReturn = L"";
	bool bError = false;

	do 
	{
		if(!pFigure)
			break;

		strReturn.Format(L"[%d] ", stIndex);

		// Figure �� Decl Type �� ���� �̸� ����
		switch(pFigure->GetDeclType())
		{
		case EFigureDeclType_Point:
			strReturn += L"Point";
			break;
			
		case EFigureDeclType_Line:
			strReturn += L"Line";
			break;

		case EFigureDeclType_Rect:
			strReturn += L"Rect";
			break;
			
		case EFigureDeclType_Quad:
			strReturn += L"Quad";
			break;
			
		case EFigureDeclType_Circle:
			strReturn += L"Circle";
			break;

		case EFigureDeclType_Ellipse:
			strReturn += L"Ellipse";
			break;

		case EFigureDeclType_CubicSpline:
			strReturn += L"Cubic Spline";
			break;

		case EFigureDeclType_Region:
			strReturn += L"Region";
			break;

		case EFigureDeclType_ComplexRegion:
			strReturn += L"Complex Region";
			break;

		case EFigureDeclType_Array:
			strReturn += L"Array";
			break;

		case EFigureDeclType_Doughnut:
			strReturn += L"Doughnut";
			break;

		default:
			bError = true;
			break;
		}

		if(bError)
			break;

		// Figure �� Template Type �� ���� �̸� ����
		switch(pFigure->GetTemplateType())
		{
		case EFigureTemplateType_Int32:
			strReturn += L"(Int32)";
			break;

		case EFigureTemplateType_Int64:
			strReturn += L"(Int64)";
			break;

		case EFigureTemplateType_Float:
			strReturn += L"(Float)";
			break;

		case EFigureTemplateType_Double:
			strReturn += L"(Double)";
			break;

		default:
			bError = true;
			break;
		}
	}
	while (false);

	if(bError)
		strReturn = L"";

	return strReturn;
}

void CFigureOperationDlg::DrawSelectedFigure()
{
	// ���õ� Figure Object �� ���´�.
	CFLFigure* pFigure1 = GetSelectedFigure1();
	CFLFigure* pFigure2 = GetSelectedFigure2();

	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = m_viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		if(!pFigure1 && !pFigure2)
			break;

		if(pFigure1)
		{
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// ���⼭ 0.5, 0.3�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			layer.DrawFigureImage(pFigure1, RED, 5, RED, EGUIViewImagePenStyle_Solid, 0.5f, 0.3f);
		}

		if(pFigure2)
		{
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// ���⼭ 0.5, 0.3�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			layer.DrawFigureImage(pFigure2, BLUE, 5, BLUE, EGUIViewImagePenStyle_Solid, 0.5f, 0.3f);
		}
	}
	while (false);

	// ���� Figure Object ����
	if(pFigure1)
	{
		delete pFigure1;
		pFigure1 = nullptr;
	}

	// ���� Figure Object ����
	if(pFigure2)
	{
		delete pFigure2;
		pFigure2 = nullptr;
	}

	// �̹��� �並 �����Ѵ�.
	m_viewImage.Invalidate(true);
}

CFLFigure* CFigureOperationDlg::GetSelectedFigure1()
{
	CFLFigure* pReturn = nullptr;

	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT1);
		if(!pCombo)
			break;

		int32_t i32Selected = pCombo->GetCurSel();
		if(i32Selected < 0)
			break;

		// �ش� �ε����� Figure Object �� ���´�.
		pReturn = m_viewImage.GetFigureObject(i32Selected);
	}
	while (false);

	return pReturn;
}

CFLFigure* CFigureOperationDlg::GetSelectedFigure2()
{
	CFLFigure* pReturn = nullptr;

	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_SELECT2);
		if(!pCombo)
			break;

		int32_t i32Selected = pCombo->GetCurSel();
		if(i32Selected < 0)
			break;

		// �ش� �ε����� Figure Object �� ���´�.
		pReturn = m_viewImage.GetFigureObject(i32Selected);
	}
	while (false);

	return pReturn;
}

void CFigureOperationDlg::DisplayMessage(CFLString<wchar_t> flstrMessage)
{
	do 
	{
		CEdit* pEditMessage = (CEdit*)GetDlgItem(IDC_EDIT_FIGURE_OPERATION_MESSAGE);
		if(!pEditMessage)
			break;

		pEditMessage->SetWindowText(flstrMessage);
	}
	while (false);
}

void CFigureOperationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1024:
		{
			UpdateControls();
			DrawSelectedFigure();
		}
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CFigureOperationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_SOURCE_FIGURE)
		pDC->SetBkColor(RGB(255, 128, 128));
	else if(pWnd->GetDlgCtrlID() == IDC_STATIC_DESTINATION_FIGURE)
		pDC->SetBkColor(RGB(128, 128, 255));

	return hbr;
}


BOOL CFigureOperationDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// �̹��� �並 �����Ѵ�.
	m_viewImage.Destroy();

	return CDialogEx::DestroyWindow();
}

void CFigureOperationDlg::OnBnClickedButtonFigureObjectCreate()
{
	// TODO: Add your control notification handler code here
	CFLFigure* pFlFigure = nullptr;

	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		CComboBox* pComboBoxTemplateType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE);

		if(!pComboBoxTemplateType)
			break;

		int32_t i32TemplateType = pComboBoxTemplateType->GetCurSel() + 1;

		// �̹��� ���� ĵ���� ������ ���´�.
		CFLRect<int32_t> flrlCanvas = m_viewImage.GetClientRectCanvasRegion();

		// ĵ���� ������ ��ǥ�踦 �̹��� ������ ��ǥ��� ��ȯ�Ѵ�.
		CFLRect<double> flrdImage =  m_viewImage.ConvertCanvasCoordToImageCoord(flrlCanvas);

		// �̹��� ������ �������� ������ Figure �� ũ��� ����� �簢������ �����Ѵ�.
		double f64Width = flrdImage.GetWidth() / 10.;
		double f64Height = flrdImage.GetHeight() / 10.;
		double f64Size = __min(f64Width, f64Height);

		CFLPoint<double> flpdCenter(0, 0);
		flrdImage.GetCenter(flpdCenter);

		CFLRect<double> flrdFigureShape(flpdCenter.x - f64Size, flpdCenter.y - f64Size, flpdCenter.x + f64Size, flpdCenter.y + f64Size);

		// ������ Decl Type, Template Type ���� Figure �� �����Ѵ�.
		// CubicSpline, ComplexRegion ���� ��쿡�� Template Type �� double ������ �����̴�.
		switch(SelectedDeclType())
		{
		case EFigureDeclType_Point:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLPoint<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLPoint<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLPoint<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLPoint<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_Line:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLLine<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLLine<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLLine<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLLine<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_Rect:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLRect<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLRect<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLRect<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLRect<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_Quad:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLQuad<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLQuad<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLQuad<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLQuad<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_Circle:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLCircle<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLCircle<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLCircle<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLCircle<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_Ellipse:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLEllipse<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLEllipse<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLEllipse<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLEllipse<double>();
					break;

				default:
					break;
				}
			}
			break;

		case EFigureDeclType_CubicSpline:
			{
				pFlFigure = new CFLCubicSpline();
			}
			break;

		case EFigureDeclType_Region:
			{
				pFlFigure = new CFLRegion();
			}
			break;

		case EFigureDeclType_ComplexRegion:
			{
				pFlFigure = new CFLComplexRegion();
			}
			break;

		case EFigureDeclType_Doughnut:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLDoughnut<int32_t>();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLDoughnut<int64_t>();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLDoughnut<float>();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLDoughnut<double>();
					break;

				default:
					break;
				}
			}

		default:
			break;
		}

		if(!pFlFigure)
			break;

		// ������ Figure �� �簢���� ���������ν� �� ���� �°� �����Ѵ�.
		pFlFigure->Set(flrdFigureShape);

		// �̹��� �信 Figure object �� �����Ѵ�.
		// ���� ������ �Ķ���ʹ� Ȱ��ȭ �Ǵ� �޴��� �����̸�, EAvailableFigureContextMenu_All �� �⺻ �޴��� Ȱ��ȭ �Ѵ�.
		// Ȱ��ȭ �ϰ��� �ϴ� �޴��� �߰� Ȥ�� ���� �ϱ� ���ؼ��� enum ���� ��Ʈ �������� �־��ָ� �ȴ�.
		// ex) EAvailableFigureContextMenu_None -> Ȱ��ȭ �Ǵ� �޴� ����
		//     EAvailableFigureContextMenu_All -> ��ü �޴� Ȱ��ȭ
		//     EAvailableFigureContextMenu_DeclType | EAvailableFigureContextMenu_TemplateType -> Decl Type, Template Type ��ȯ �޴� Ȱ��ȭ

		m_viewImage.PushBackFigureObject(pFlFigure, EAvailableFigureContextMenu_All);

		// �̹��� �並 �����Ѵ�.
		m_viewImage.Invalidate(true);

		// �޺� �ڽ��� Figure Object �׸��� �����Ѵ�.
		UpdateFigureObjectList();
	}
	while (false);

	// ������ Figure ��ü ����
	// PushBackFigureObject() �Լ� ���ο��� Figure ���簡 �Ǳ� ������ �����ߴ� ��ü�� ������ ����Ѵ�.
	if(pFlFigure)
	{
		delete pFlFigure;
		pFlFigure = nullptr;
	}
}

void CFigureOperationDlg::OnBnClickedButtonFigureObjectClear()
{
	// TODO: Add your control notification handler code here
	do 
	{
		if(!m_viewImage.IsAvailable())
			break;

		// ���� �̹��� �信 �ִ� Figure Objects �� �����Ѵ�.
		m_viewImage.ClearFigureObject();

		// �̹��� �並 �����Ѵ�.
		m_viewImage.Invalidate(true);

		// �޺� �ڽ��� Figure Object �׸��� �����Ѵ�.
		UpdateFigureObjectList();
	}
	while (false);
}


void CFigureOperationDlg::OnBnClickedButtonFigureObjectExecute()
{
	// TODO: Add your control notification handler code here
	CFLFigure* pFigure1 = nullptr;
	CFLFigure* pFigure2 = nullptr;
	CResult res = EResult_UnknownError;

	do 
	{
		CComboBox* pComboOperation = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_OPERATION);
		if(!pComboOperation)
			break;

		// ���õ� Figure Object �� ���´�.
		pFigure1 = GetSelectedFigure1();
		pFigure2 = GetSelectedFigure2();

		if(!pFigure1 || !pFigure2)
		{
			res = EResult_InvalidObject;
			break;
		}

		// Operation ����� ��� ���� FLFigureArray �� �����Ѵ�.
		CFLFigureArray flfa;

		switch(pComboOperation->GetCurSel())
		{
		case 0:
			// Intersection Operation ����
			res = pFigure1->GetRegionOfIntersection(pFigure2, &flfa);
			break;

		case 1:
			// Union Operation ����
			res = pFigure1->GetRegionOfUnion(pFigure2, &flfa);
			break;

		case 2:
			// Subtraction Operation ����
			res = pFigure1->GetRegionOfSubtraction(pFigure2, &flfa);
			break;

		case 3:
			// Exclusive Or Operation ����
			res = pFigure1->GetRegionOfExclusiveOr(pFigure2, &flfa);
			break;
		}

		// ���� ����� Ȯ���Ѵ�.
		if(IsFail(res))
			break;

		if(!flfa.GetCount())
			break;

		// �̹��� �信 Figure object �� �����Ѵ�.
		if(flfa.GetCount() == 1)
			m_viewImage.PushBackFigureObject(flfa.GetAt(0), EAvailableFigureContextMenu_All);
		else
			m_viewImage.PushBackFigureObject(&flfa, EAvailableFigureContextMenu_All);

		// �̹��� �並 �����Ѵ�.
		m_viewImage.Invalidate(true);

		// �޺� �ڽ��� Figure Object �׸��� �����Ѵ�.
		UpdateFigureObjectList();
	}
	while (false);

	// ���� Figure Object ����
	if(pFigure1)
	{
		delete pFigure1;
		pFigure1 = nullptr;
	}

	// ���� Figure Object ����
	if(pFigure2)
	{
		delete pFigure2;
		pFigure2 = nullptr;
	}

	// ���� ��� �޼����� ǥ���Ѵ�.
	DisplayMessage(res.GetString());
}

void CFigureOperationDlg::OnCbnSelchangeComboFigureObjectSelect1()
{
	// TODO: Add your control notification handler code here
	// ���õ� Figure �� �׸���.
	DrawSelectedFigure();
}


void CFigureOperationDlg::OnCbnSelchangeComboFigureObjectSelect2()
{
	// TODO: Add your control notification handler code here
	// ���õ� Figure �� �׸���.
	DrawSelectedFigure();
}
