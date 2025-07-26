
// FigureOperationDlg.h : header file
//

#pragma once


// CFigureOperationDlg dialog
class CFigureOperationDlg : public CDialogEx
{
// Construction
public:
	CFigureOperationDlg(CWnd* pParent = NULL);	// standard constructor

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateControls();

	EFigureDeclType SelectedDeclType();

	void UpdateFigureObjectList();

	CFLString<wchar_t> MakeFigureObjectName(size_t stIndex, CFLFigure* pFigure);

	void DrawSelectedFigure();

	CFLFigure* GetSelectedFigure1();
	CFLFigure* GetSelectedFigure2();

	void DisplayMessage(CFLString<wchar_t> flstrMessage);

protected:
	CGUIViewImageWrap m_viewImage;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonFigureObjectCreate();
	afx_msg void OnBnClickedButtonFigureObjectClear();
	afx_msg void OnBnClickedButtonFigureObjectExecute();
	afx_msg void OnCbnSelchangeComboFigureObjectSelect1();
	afx_msg void OnCbnSelchangeComboFigureObjectSelect2();
};
