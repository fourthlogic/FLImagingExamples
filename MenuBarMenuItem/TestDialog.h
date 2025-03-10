#pragma once
#include "afxdialogex.h"


// CTestDialog 대화 상자

class CTestDialog : public FLImaging::GUI::CGUIDialogCustomEmbedded
{
	DECLARE_DYNAMIC(CTestDialog)

public:
	CTestDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTestDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEST };
#endif

	SupportToDuplicateObject(CTestDialog);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
