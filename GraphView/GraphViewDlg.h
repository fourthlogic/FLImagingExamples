
// GraphViewDlg.h : header file
//

#pragma once


// CGraphViewDlg dialog
class CGraphViewDlg : public CDialogEx
{
// Construction
public:
	CGraphViewDlg(CWnd* pParent = NULL);	// standard constructor

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
	void UpdateControls();
	void LockControls(bool bLock);

protected:
	CGUIViewGraphWrap m_viewGraph;
	bool m_bLockControls;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonOpenGraphView();
	afx_msg void OnBnClickedButtonTerminateGraphView();
	afx_msg void OnBnClickedButtonChartAdd();
	afx_msg void OnBnClickedButtonChartClear();
	afx_msg void OnBnClickedButtonLoadGraph();
	afx_msg void OnBnClickedButtonSaveGraph();
};
