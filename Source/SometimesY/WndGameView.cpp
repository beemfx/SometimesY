// WndGameView.cpp : implementation file
//

#include "stdafx.h"
#include "SometimesY.h"
#include "WndGameView.h"


// CWndGameView

IMPLEMENT_DYNAMIC(CWndGameView, CWnd)

CWndGameView::CWndGameView(CSYGame* pGame)
: m_pGame(pGame)
{
	m_Font.CreateFont(36, 0, 0, 0, 
		FW_BOLD, 0,0,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,
		_T("Courier New"));
		
	m_mnuInsert.LoadMenu(IDR_MENU_INSERT);
}

CWndGameView::~CWndGameView()
{
}


BEGIN_MESSAGE_MAP(CWndGameView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CWndGameView message handlers



void CWndGameView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CFont* fntOld;
	CBrush* brshOld;
	
	dc.SetBkMode(TRANSPARENT);
	
	CBrush brshWhite(RGB(255, 255, 255));
	CBrush brshBlack(RGB(0, 0, 0));
	CBrush brshPlaced(RGB(200, 0, 0));
	CBrush brshPlacedValid(RGB(0, 200, 0));
	CBrush brshFixed(RGB(150, 150, 0));
	CBrush brshFixedValid(RGB(255, 255, 0));
	
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rc;
	GetClientRect(&rc);
	
	brshOld=dc.SelectObject(&brshWhite);
	dc.Rectangle(&rc);
	
	fntOld=dc.SelectObject(&m_Font);
	//dc.TextOut(1, 2, CString(_T("Hi")));
	DWORD sx, sy;
	m_pGame->GetSize(sx, sy);
	for(DWORD x=0; x<sx; x++)
	{
		for(DWORD y=0; y<sy; y++)
		{
			const CSYGame::Block* pBlk=m_pGame->GetBlock(x, y);
			TCHAR cBlock=0;
			switch(pBlk->nType)
			{
			case CSYGame::BLOCK_UNUSABLE:
				cBlock='X'; break;
			case CSYGame::BLOCK_A:
				cBlock='A'; break;
			case CSYGame::BLOCK_E:
				cBlock='E'; break;
			case CSYGame::BLOCK_I:
				cBlock='I'; break;
			case CSYGame::BLOCK_O:
				cBlock='O'; break;
			case CSYGame::BLOCK_U:
				cBlock='U'; break;
			case CSYGame::BLOCK_Y:
				cBlock='Y'; break;
			case CSYGame::BLOCK_EMPTY:
				cBlock=' '; break;
			}
	
			if(pBlk->nStatus==CSYGame::STATUS_PLACED)
			{
				dc.SelectObject((pBlk->bValid)?&brshPlacedValid:&brshPlaced);
			}
			else if(pBlk->nStatus==CSYGame::STATUS_FIXED)
			{
				dc.SelectObject(pBlk->bValid?&brshFixedValid:&brshFixed);
			}
			else
				dc.SelectObject(&brshWhite);
			
			
			rc.SetRect(x*40, y*40, x*40+40, y*40+40);
			
			if(cBlock=='X')
			{
				dc.SelectObject(&brshBlack);
				dc.Rectangle(rc);
			}
			else
			{	
				dc.Rectangle(rc);
				dc.TextOutA(1+x*40+9, 1+y*40, CString(cBlock));
			}
		}
	}
	
	//Draw the letters available:
	DWORD nCounts[6];
	m_pGame->GetAvailLetters(nCounts);
	
	CString strOut;
	strOut.Format(_T("A - %d"), nCounts[0]);
	dc.TextOut(420, 50, strOut);
	strOut.Format(_T("E - %d"), nCounts[1]);
	dc.TextOut(420, 90, strOut);
	strOut.Format(_T("I - %d"), nCounts[2]);
	dc.TextOut(420, 130, strOut);
	strOut.Format(_T("O - %d"), nCounts[3]);
	dc.TextOut(420, 170, strOut);
	strOut.Format(_T("U - %d"), nCounts[4]);
	dc.TextOut(420, 210, strOut);
	strOut.Format(_T("Y - %d"), nCounts[5]);
	dc.TextOut(420, 250, strOut);
	
	dc.SelectObject(fntOld);
	dc.SelectObject(brshOld);
}


void CWndGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	DWORD x=m_pntLastBlock.x = point.x/40;
	DWORD y=m_pntLastBlock.y = point.y/40;
	
	
	CString str;
	
	DWORD nCounts[6];
	m_pGame->GetAvailLetters(nCounts);
	
	//str.Format(_T("You cliked box (%i, %i)"), x, y);
	//MessageBox(str);
	
	if(m_pGame->GetBlock(x, y)->nStatus!=m_pGame->STATUS_EMPTY)
		return;
	
	CMenu* pMenuLetters=m_mnuInsert.GetSubMenu(0);
	
	for(DWORD i=0; i<6; i++)
	{
		CString str;
		
		TCHAR c;
		switch(i){
		case 0:c='A';break;
		case 1:c='E';break;
		case 2:c='I';break;
		case 3:c='O';break;
		case 4:c='U';break;
		case 5:c='Y';break;
		default:c='X';break;
		}
		
		str.Format(_T("%c: %i"), c, nCounts[i]);
		pMenuLetters->ModifyMenu(i, MF_BYPOSITION, ID_INSERT_A+i, str.GetBuffer());
		pMenuLetters->EnableMenuItem(i, MF_BYPOSITION|(nCounts[i]>0?MF_ENABLED:MF_DISABLED));
	}
	
	this->ClientToScreen(&point);
	pMenuLetters->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);

	CWnd::OnLButtonDown(nFlags, point);
}

void CWndGameView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DWORD x = point.x/40;
	DWORD y = point.y/40;
	
	this->m_pGame->RemoveLetter(x, y);
	this->Invalidate();
	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CWndGameView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_INSERT_A:
		m_pGame->PlaceLetter(CSYGame::BLOCK_A, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	case ID_INSERT_E:
		m_pGame->PlaceLetter(CSYGame::BLOCK_E, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	case ID_INSERT_I:
		m_pGame->PlaceLetter(CSYGame::BLOCK_I, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	case ID_INSERT_O:
		m_pGame->PlaceLetter(CSYGame::BLOCK_O, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	case ID_INSERT_U:
		m_pGame->PlaceLetter(CSYGame::BLOCK_U, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	case ID_INSERT_Y:
		m_pGame->PlaceLetter(CSYGame::BLOCK_Y, this->m_pntLastBlock.x, m_pntLastBlock.y, NULL);
		Invalidate();
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}
