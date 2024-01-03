#pragma once
#include "afxdialogex.h"


// CFLImagingGUIExampleDlg 대화 상자

class CFLImagingGUIExampleDlg : public FLImaging::GUI::CGUIDialogCustomEmbedded
{
	DECLARE_DYNAMIC(CFLImagingGUIExampleDlg)

public:
	CFLImagingGUIExampleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFLImagingGUIExampleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFLImagingGUIExampleDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedGUIExampleButtonPreset1();
	afx_msg void OnBnClickedGUIExampleButtonPreset2();
	afx_msg void OnBnClickedGUIExampleButtonPreset3();
	afx_msg void OnBnClickedGUIExampleButtonPreset4();
	afx_msg void OnBnClickedGUIExampleButtonGetValue();

protected:

	CString m_strPropertyValues;
public:
	afx_msg void OnBnClickedGUIExampleButtonClearEditBox();
};
