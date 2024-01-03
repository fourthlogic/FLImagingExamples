
// ImageViewDlg.h : header file
//

#pragma once


// CImageViewDlg dialog
class CImageViewDlg : public CDialogEx
{
// Construction
public:
	CImageViewDlg(CWnd* pParent = NULL);	// standard constructor

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
	void LockControls(bool bLock);

	EFigureDeclType SelectedDeclType();

protected:
	CGUIViewImageWrap m_viewImage;
	bool m_bLockControls;

public:
	afx_msg void OnBnClickedButtonOpenImageView();
	afx_msg void OnBnClickedButtonTerminateImageView();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonFigureObjectCreate();
	afx_msg void OnBnClickedButtonFigureObjectPopFront();
	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedButtonSaveImage();
};
