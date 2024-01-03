// FLImagingGUIExampleDlg.cpp: 구현 파일
//

//#include "pch.h"
#include "stdafx.h"
#include "FLImagingGUIExample.h"
#include "afxdialogex.h"
#include "FLImagingGUIExampleDlg.h"


// CFLImagingGUIExampleDlg 대화 상자

IMPLEMENT_DYNAMIC(CFLImagingGUIExampleDlg, CGUIDialogCustomEmbedded)

CFLImagingGUIExampleDlg::CFLImagingGUIExampleDlg(CWnd* pParent /*=nullptr*/)
	: CGUIDialogCustomEmbedded(IDD_DIALOG_GUI_EXAMPLE, pParent)
{

}

CFLImagingGUIExampleDlg::~CFLImagingGUIExampleDlg()
{
}

void CFLImagingGUIExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CGUIDialogCustomEmbedded::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUI_EXAMPLE_EDIT_GETVALUES, m_strPropertyValues);
}


BEGIN_MESSAGE_MAP(CFLImagingGUIExampleDlg, CGUIDialogCustomEmbedded)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_PRESET_1, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset1)
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_PRESET_2, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset2)
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_PRESET_3, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset3)
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_PRESET_4, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset4)
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_GETVALUE, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonGetValue)
	ON_BN_CLICKED(IDC_GUI_EXAMPLE_BUTTON_CLEAR, &CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonClearEditBox)
END_MESSAGE_MAP()


// CFLImagingGUIExampleDlg 메시지 처리기


BOOL CFLImagingGUIExampleDlg::OnInitDialog()
{
	BOOL bReturn = CGUIDialogCustomEmbedded::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetDlgItemTextW(IDC_GUI_EXAMPLE_EDIT_GETVALUES, L"");

	UpdateData(false);

	return bReturn;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFLImagingGUIExampleDlg::OnDestroy()
{
	CGUIDialogCustomEmbedded::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGUIManagerView::SelectFixedViewPlacementSet(0);
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGUIManagerView::SelectFixedViewPlacementSet(1);
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGUIManagerView::SelectFixedViewPlacementSet(2);
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonPreset4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGUIManagerView::SelectFixedViewPlacementSet(3);
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonGetValue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	do 
	{
		CModelUtilities mu(CGUIManagerModel::GetCurrentModelFullPath(), L"Menu@Example");

		if((mu.Load()).IsFail())
			break;

		m_strPropertyValues.Empty();

		m_strPropertyValues.AppendFormat(L"Current Model : %s\r\n", mu.GetModelName().GetString());
		m_strPropertyValues.AppendFormat(L"Current Property : %s\r\n\n", mu.GetPropertyName().GetString());

		m_strPropertyValues.AppendFormat(L"Image View : %s\r\n", mu.GetValue(L"Example@Image", L"Image View").GetString());
		m_strPropertyValues.AppendFormat(L"File : %s\r\n", mu.GetValue(L"Example@Image", L"File").GetString());
		m_strPropertyValues.AppendFormat(L"ROI : %s\r\n", mu.GetValue(L"Example@Image", L"ROI").GetString());
		m_strPropertyValues.AppendFormat(L"Text : %s\r\n", mu.GetValue(L"Example@Parameters", L"Text",L"ABC").GetString());
		m_strPropertyValues.AppendFormat(L"Only Number : %s\r\n", mu.GetValue(L"Example@Parameters", L"Only Number", L"123").GetString());
		m_strPropertyValues.AppendFormat(L"Dropdown List : %s\r\n", mu.GetValue(L"Example@Parameters", L"Dropdown List", L"OK").GetString());
		m_strPropertyValues.AppendFormat(L"Hide Image Category : %s\r\n", mu.GetValue(L"Example@Parameters", L"Hide Image Category", L"Unchecked").GetString());
		m_strPropertyValues.AppendFormat(L"Slider : %s\r\n", mu.GetValue(L"Example@Parameters", L"Slider", L"0").GetString());
		m_strPropertyValues.AppendFormat(L"Spin : %s\r\n", mu.GetValue(L"Example@Parameters", L"Spin", L"0").GetString());
		m_strPropertyValues.AppendFormat(L"IPAddress : %s", mu.GetValue(L"Example@Parameters", L"IPAddress", L"000.000.000.000").GetString());
	
		UpdateData(false);
	}
	while(false);
	
}


void CFLImagingGUIExampleDlg::OnBnClickedGUIExampleButtonClearEditBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strPropertyValues.Empty();

	UpdateData(false);
}
