
// GraphViewContextMenuDlg.h : header file
//

#pragma once


// CGraphViewContextMenuDlg dialog
class CGraphViewContextMenuDlg : public CDialogEx
{
// Construction
public:
	CGraphViewContextMenuDlg(CWnd* pParent = NULL);	// standard constructor

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
	virtual void CreateContextMenuCheckbox();
	virtual void UpdateContentPosition(int32_t i32PosDiff);

protected:
	CGUIViewGraphWrap m_viewGraph;
	CScrollBar m_VScrollBar;
	int32_t m_i32VScrollPosPrev;
	int32_t m_i32VScrollMax;

	CFLArray<uint64_t> m_flaAvailableContextMenu;
	CFLArray<uint32_t> m_flaCheckBoxId;

public:
	afx_msg void OnRangeRadioGroupHide(UINT uID);
	afx_msg void OnBnClickedButtonApply();

protected:
	afx_msg void OnRangeRadioGroupSelectAll(UINT uID);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL DestroyWindow();
};
