// CTestDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "MenuBarMenuItem.h"
#include "afxdialogex.h"
#include "TestDialog.h"


// CTestDialog 대화 상자

IMPLEMENT_DYNAMIC(CTestDialog, CGUIDialogCustomEmbedded)

CTestDialog::CTestDialog(CWnd* pParent /*=nullptr*/)
	: CGUIDialogCustomEmbedded(IDD_DIALOG_TEST, pParent)
{

}

CTestDialog::~CTestDialog()
{
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CGUIDialogCustomEmbedded::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDialog, CGUIDialogCustomEmbedded)
END_MESSAGE_MAP()


// CTestDialog 메시지 처리기
