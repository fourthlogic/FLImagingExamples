
// ImageViewIntoDialogDlg.h : header file
//

#pragma once


// CImageViewIntoDialogDlg dialog
class CImageViewIntoDialogDlg : public CDialogEx
{
// Construction
public:
	CImageViewIntoDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IMAGEVIEW_DIALOG };

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

protected:
	CGUIViewImageWrap m_viewImage;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonFigureObjectCreate();
	afx_msg void OnBnClickedButtonFigureObjectPopFront();
};
