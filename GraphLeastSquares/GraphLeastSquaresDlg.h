
// GraphLeastSquaresDlg.h : header file
//

#pragma once


// CGraphLeastSquaresDlg dialog
class CGraphLeastSquaresDlg : public CDialogEx
{
// Construction
public:
	CGraphLeastSquaresDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GRAPHVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateControls();

protected:
	CGUIViewGraphWrap m_viewGraph;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonGraphAdd();
	afx_msg void OnBnClickedButtonGraphClear();
};
