
// SNAPViewIntoDialogDlg.h : header file
//

#pragma once


// CSNAPViewIntoDialogDlg dialog
class CSNAPViewIntoDialogDlg : public CDialogEx
{
// Construction
public:
	CSNAPViewIntoDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SNAPVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	CGUIViewSNAPWrap m_viewSNAP;
};
