
// ImageViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageView.h"
#include "ImageViewDlg.h"
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


// CImageViewDlg dialog




CImageViewDlg::CImageViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageViewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLockControls = false;
}

void CImageViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_IMAGE_VIEW, &CImageViewDlg::OnBnClickedButtonOpenImageView)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE_IMAGE_VIEW, &CImageViewDlg::OnBnClickedButtonTerminateImageView)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_CREATE, &CImageViewDlg::OnBnClickedButtonFigureObjectCreate)
	ON_BN_CLICKED(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT, &CImageViewDlg::OnBnClickedButtonFigureObjectPopFront)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CImageViewDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGE, &CImageViewDlg::OnBnClickedButtonSaveImage)
END_MESSAGE_MAP()


// CImageViewDlg message handlers

BOOL CImageViewDlg::OnInitDialog()
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
	CComboBox* pComboBoxDeclType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE);
	pComboBoxDeclType->Clear();

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
	pComboBoxTemplateType->Clear();

	pComboBoxTemplateType->AddString(_T("Int32"));
	pComboBoxTemplateType->AddString(_T("Int64"));
	pComboBoxTemplateType->AddString(_T("Float"));
	pComboBoxTemplateType->AddString(_T("Double"));

	pComboBoxTemplateType->SetCurSel(3);

	UpdateControls();

	SetTimer(1024, 100, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageViewDlg::OnPaint()
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
HCURSOR CImageViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageViewDlg::UpdateControls()
{
	if(m_bLockControls)
	{
		GetDlgItem(IDC_BUTTON_OPEN_IMAGE_VIEW)->EnableWindow(false);

		GetDlgItem(IDC_BUTTON_TERMINATE_IMAGE_VIEW)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_CREATE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT)->EnableWindow(false);
	}
	// �̹��� �� ��ȿ�� üũ
	else if(!m_viewImage.IsAvailable())
	{
		GetDlgItem(IDC_BUTTON_OPEN_IMAGE_VIEW)->EnableWindow(true);

		GetDlgItem(IDC_BUTTON_TERMINATE_IMAGE_VIEW)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_CREATE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPEN_IMAGE_VIEW)->EnableWindow(false);

		GetDlgItem(IDC_BUTTON_TERMINATE_IMAGE_VIEW)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_CREATE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT)->EnableWindow(true);

		// �̹��� ���� �̹��� ���۰� �����ϴ��� üũ
		if(!m_viewImage.DoesFLImageBufferExist())
		{
			GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(false);
		}

		CComboBox* pComboBoxDeclType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_DECL_TYPE);
		CComboBox* pComboBoxTemplateType = (CComboBox*)GetDlgItem(IDC_COMBO_FIGURE_OBJECT_TEMPLATE_TYPE);

		if(SelectedDeclType() == EFigureDeclType_CubicSpline || SelectedDeclType() == EFigureDeclType_Region || SelectedDeclType() == EFigureDeclType_ComplexRegion)
		{
			pComboBoxTemplateType->SetCurSel(EFigureTemplateType_Double - 1);
			pComboBoxTemplateType->EnableWindow(false);
		}

		// �̹��� ���� Figure object ������ ���´�.
		if(!m_viewImage.GetFigureObjectCount())
		{
			GetDlgItem(IDC_BUTTON_FIGURE_OBJECT_POP_FRONT)->EnableWindow(false);
		}
	}
}

void CImageViewDlg::LockControls(bool bLock)
{
	m_bLockControls = bLock;
	UpdateControls();
}

EFigureDeclType CImageViewDlg::SelectedDeclType()
{
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

void CImageViewDlg::OnBnClickedButtonOpenImageView()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(m_viewImage.IsAvailable())
			break;

		// �̹��� �� ���� // Create image view
		CResult eResult = m_viewImage.Create(0, 0, 500, 500);

		if(eResult.IsFail())
			ErrorMessageBox(eResult);
	}
	while (false);
}


void CImageViewDlg::OnBnClickedButtonTerminateImageView()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// �̹��� �並 �����Ѵ�.
		CResult eResult = m_viewImage.Destroy();

		if(eResult.IsFail())
			ErrorMessageBox(eResult);
	}
	while (false);
}


void CImageViewDlg::OnTimer(UINT_PTR nIDEvent)
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


BOOL CImageViewDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1024);

	// �̹��� �並 �����Ѵ�.
	m_viewImage.Destroy();

	return CDialogEx::DestroyWindow();
}

void CImageViewDlg::OnBnClickedButtonFigureObjectCreate()
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
		CFLRectL flrlCanvas = m_viewImage.GetClientRectCanvasRegion();

		// ĵ���� ������ ��ǥ�踦 �̹��� ������ ��ǥ��� ��ȯ�Ѵ�.
		CFLRectD flrdImage = m_viewImage.ConvertCanvasCoordToImageCoord(flrlCanvas);

		// �̹��� ������ �������� ������ Figure �� ũ��� ����� �簢������ �����Ѵ�.
		double f64Width = flrdImage.GetWidth() / 10.;
		double f64Height = flrdImage.GetHeight() / 10.;
		double f64Size = __min(f64Width, f64Height);

		CFLPointD flpdCenter(0, 0);
		flrdImage.GetCenter(flpdCenter);

		CFLRectD flrdFigureShape(flpdCenter.x - f64Size, flpdCenter.y - f64Size, flpdCenter.x + f64Size, flpdCenter.y + f64Size);

		// ������ Decl Type, Template Type ���� Figure �� �����Ѵ�.
		// CubicSpline, ComplexRegion ���� ��쿡�� Template Type �� double ������ �����̴�.
		switch(SelectedDeclType())
		{
		case EFigureDeclType_Point:
			{
				switch(i32TemplateType)
				{
				case EFigureTemplateType_Int32:
					pFlFigure = new CFLPointL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLPointLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLPointF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLPointD();
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
					pFlFigure = new CFLLineL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLLineLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLLineF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLLineD();
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
					pFlFigure = new CFLRectL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLRectLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLRectF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLRectD();
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
					pFlFigure = new CFLQuadL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLQuadLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLQuadF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLQuadD();
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
					pFlFigure = new CFLCircleL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLCircleLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLCircleF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLCircleD();
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
					pFlFigure = new CFLEllipseL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLEllipseLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLEllipseF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLEllipseD();
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
					pFlFigure = new CFLDoughnutL();
					break;

				case EFigureTemplateType_Int64:
					pFlFigure = new CFLDoughnutLL();
					break;

				case EFigureTemplateType_Float:
					pFlFigure = new CFLDoughnutF();
					break;

				case EFigureTemplateType_Double:
					pFlFigure = new CFLDoughnutD();
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
	}
	while (false);

	// ������ Figure ��ü ����
	// C_GUI_CGUIViewImage_PushBackFigureObject() �Լ� ���ο��� Figure ���簡 �Ǳ� ������ �����ߴ� ��ü�� ������ ����Ѵ�.
	if(pFlFigure)
	{
		delete pFlFigure;
		pFlFigure = nullptr;
	}
}


void CImageViewDlg::OnBnClickedButtonFigureObjectPopFront()
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
		CFLString<wchar_t> flStrFigure = CROIUtilities::ConvertFigureObjectToString(pFlFigure);

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


void CImageViewDlg::OnBnClickedButtonLoadImage()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		LockControls(true);

		// �̹��� ���� �ε� ���̾�α׸� Ȱ��ȭ ��Ų��.
		// ���� ������ �Ķ���ͷ� �ε� �ɼ��� �����Ѵ�.
		// ex) EViewImageLoadOption_Load -> �̹��� ����/���� �ε�
		//     EViewImageLoadOption_OpenDialog | EViewImageLoadOption_DialogTypeFile �̹��� ���� �ε� ���̾�α� Ȱ��ȭ
		//     EViewImageLoadOption_OpenDialog | EViewImageLoadOption_DialogTypeFolder ���� �ε� ���̾�α� Ȱ��ȭ(���� ������ �̹��� ���ϵ��� �ε�)
		m_viewImage.Load(L"", EViewImageLoadOption_OpenDialog | EViewImageLoadOption_DialogTypeFile);

		LockControls(false);
	}
	while(false);
}


void CImageViewDlg::OnBnClickedButtonSaveImage()
{
	// TODO: Add your control notification handler code here
	do 
	{
		// �̹��� �� ��ȿ�� üũ
		if(!m_viewImage.IsAvailable())
			break;

		// �̹��� ���� �̹��� ���۰� �����ϴ��� üũ
		if(!m_viewImage.DoesFLImageBufferExist())
			break;

		LockControls(true);

		// �̹��� ���� ���̾�α׸� Ȱ��ȭ ��Ų��.
		m_viewImage.Save(L"", false);

		LockControls(false);
	}
	while(false);
}
