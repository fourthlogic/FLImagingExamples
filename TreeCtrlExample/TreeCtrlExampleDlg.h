#pragma once
#include "afxdialogex.h"


// CTreeCtrlExampleDlg 대화 상자

enum ETreeCtrlExampleMessage
{
	ETreeCtrlExampleMessage_DeleteMenuItem = WM_USER + 0x5000,
};

class CTreeCtrlExampleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeCtrlExampleDlg)
// 생성입니다.
public:
	CTreeCtrlExampleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CTreeCtrlExampleDlg();

// 대화 상자 데이터입니다.
	#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_TREECTRLEXAMPLE_DIALOG
	};
	#endif

public:
	// 메뉴 아이템 추가
	virtual void AddMenuItem(FLImaging::GUI::CGUIMenuItemBase* pMenuItem);

protected:
	// Tree 구성
	virtual const CResult BuildTree();

// 구현입니다.
protected:
	HICON m_hIcon;
	CGUITreeCtrl m_treeCtrl;
	CImageList m_GUIPaneMenuImages;
	std::vector<FLImaging::GUI::CGUIMenuItemBase*>& m_vctItems;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnDeleteMenuItem(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
