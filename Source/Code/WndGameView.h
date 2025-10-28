#pragma once


// CWndGameView

class CWndGameView : public CWnd
{
	DECLARE_DYNAMIC(CWndGameView)

public:
	CWndGameView(CSYGame* pGame);
	virtual ~CWndGameView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CFont m_Font;
	CSYGame* m_pGame;
	CMenu m_mnuInsert;
	CPoint m_pntLastBlock;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


