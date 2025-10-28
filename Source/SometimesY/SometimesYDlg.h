
// SometimesYDlg.h : header file
//

#pragma once
#include "sygame.h"
#include "wndgameview.h"


// CSometimesYDlg dialog
class CSometimesYDlg : public CDialog
{
// Construction
public:
	CSometimesYDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOMETIMESY_DIALOG };

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
private:
	CSYGame m_SYGame;
	CWndGameView m_wndGameView;
};
