#include "StdAfx.h"
#include "SYGame.h"

CSYGame::CSYGame(void)
: m_bGameRunning(FALSE)
, m_nGameSizeX(0), m_nGameSizeY(0)
, m_GameMatrix(NULL)
{
	EndGame();
}

CSYGame::~CSYGame(void)
{
	EndGame();
}

BOOL CSYGame::StartGame(LPCTSTR szGameFile)
{
	EndGame();
	//For now this is just a dummy game creator
	m_nGameSizeX=10;
	m_nGameSizeY=10;
	
	m_GameMatrix = new Block[m_nGameSizeX*m_nGameSizeY];
	
	for(DWORD i=0; i<m_nGameSizeX*m_nGameSizeY; i++)
	{
		m_GameMatrix[i].nType=BLOCK_UNUSABLE;
		m_GameMatrix[i].nStatus=STATUS_FIXED;
		m_GameMatrix[i].bValid=FALSE;
	}
	
	SetBlock(2, 2, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(3, 2, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(4, 2, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(2, 3, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(3, 3, BLOCK_O, STATUS_FIXED);
	SetBlock(4, 3, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(2, 4, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(3, 4, BLOCK_EMPTY, STATUS_EMPTY);
	SetBlock(4, 4, BLOCK_EMPTY, STATUS_EMPTY);
	//SetBlock(9, 9, BLOCK_EMPTY, STATUS_EMPTY);
	
	m_nTypeAvail[BLOCK_A]=2;
	m_nTypeAvail[BLOCK_E]=2;
	m_nTypeAvail[BLOCK_I]=2;
	m_nTypeAvail[BLOCK_O]=0;
	m_nTypeAvail[BLOCK_U]=1;
	m_nTypeAvail[BLOCK_Y]=1;
	
	UpdateGameStatus();
	m_bGameRunning=TRUE;
	return TRUE;
}
void CSYGame::EndGame()
{
	m_nGameSizeX=0;
	m_nGameSizeY=0;
	
	for(DWORD i=0; i<6; i++)
	{
		m_nTypeAvail[i]=0;
	}
	
	if(m_GameMatrix)
	{
		delete[]m_GameMatrix;
		m_GameMatrix=NULL;
	}
	
	
	m_bGameRunning=FALSE;
}
CSYGame::PLACE_RESULT CSYGame::PlaceLetter(CSYGame::BLOCK_TYPE nLetter, DWORD nX, DWORD nY, LPTSTR szOutMsg)
{
	if(m_nTypeAvail[nLetter]==0)
		return CSYGame::PLACE_BAD;
	
	const Block* pBlk = GetBlock(nX, nY);
	
	if(pBlk->nType==BLOCK_UNUSABLE || pBlk->nType!=BLOCK_EMPTY)
		return CSYGame::PLACE_BAD;
		
	SetBlock(nX, nY, nLetter, STATUS_PLACED);
	m_nTypeAvail[nLetter]--;
	UpdateGameStatus();
	return CSYGame::PLACE_OK;
}

BOOL CSYGame::RemoveLetter(DWORD nX, DWORD nY)
{
	const Block* pBlk = GetBlock(nX, nY);
	
	if(pBlk->nType==BLOCK_UNUSABLE || pBlk->nStatus==STATUS_FIXED || pBlk->nType==BLOCK_EMPTY)
		return FALSE;
	
	m_nTypeAvail[pBlk->nType]++;
	SetBlock(nX, nY, BLOCK_EMPTY, STATUS_EMPTY);
	UpdateGameStatus();
	return TRUE;
}

void CSYGame::GetSize(DWORD& x, DWORD& y)
{
	x=m_nGameSizeX;
	y=m_nGameSizeY;
}
const CSYGame::Block* CSYGame::GetBlock(const DWORD x, const DWORD y)
{
	static const Block blkEmpty={BLOCK_UNUSABLE, STATUS_EMPTY, FALSE};
	if(!m_bGameRunning || x>=m_nGameSizeX || y>=m_nGameSizeY)
	{
		return &blkEmpty;
	}
	else
	{
		return &m_GameMatrix[x+y*m_nGameSizeX];
	}
}

void CSYGame::GetAvailLetters(DWORD* pCounts)
{
	for(DWORD i=0; i<6; i++)
	{
		pCounts[i]=this->m_nTypeAvail[i];
	}
}

void CSYGame::SetBlock(DWORD x, DWORD y, CSYGame::BLOCK_TYPE nType, CSYGame::BLOCK_STATUS nStatus)
{
	if(x>=m_nGameSizeX || y>=m_nGameSizeY)
		return;
		
	m_GameMatrix[x+y*m_nGameSizeX].nType=nType;
	m_GameMatrix[x+y*m_nGameSizeX].nStatus=nStatus;
	m_GameMatrix[x+y*m_nGameSizeX].bValid=FALSE;
}

void CSYGame::SetBlockValid(DWORD x, DWORD y, BOOL bValid)
{
	if(x>=m_nGameSizeX || y>=m_nGameSizeY)
		return;
		
	m_GameMatrix[x+y*m_nGameSizeX].bValid=bValid;
}

void CSYGame::UpdateGameStatus()
{
	for(DWORD x=0; x<m_nGameSizeX; x++)
	{
		for(DWORD y=0; y<m_nGameSizeY; y++)
		{
			const Block* pBlk = GetBlock(x, y);
			
			switch(pBlk->nType)
			{
			case BLOCK_A:
				SetBlockValid(x, y, CheckA(x, y));
				break;
			case BLOCK_E:
				SetBlockValid(x, y, CheckE(x, y));
				break;
			case BLOCK_I:
				SetBlockValid(x, y, CheckI(x, y));
				break;
			case BLOCK_O:
				SetBlockValid(x, y, CheckO(x, y));
				break;
			case BLOCK_U:
				SetBlockValid(x, y, CheckU(x, y));
				break;
			case BLOCK_Y:
				SetBlockValid(x, y, CheckY(x, y));
				break;
			//By default we aren't checking anything that isn't
			//a letter.
			}
		}
	}
}


BOOL CSYGame::CheckA(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	BOOL bResult=FALSE;
	//A must share a side with an A or U, and not O
	for(DWORD i=0; i<4; i++)
	{
		if(sides[i]==BLOCK_O)
			return FALSE;
			
		if(sides[i]==BLOCK_A || sides[i]==BLOCK_U)
			bResult=TRUE;
	}
	
	return bResult;
}

BOOL CSYGame::CheckE(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	BOOL bResult=FALSE;
	DWORD nSideCount=0;
	//Must side 3 or more letters, cannot side with U.
	for(DWORD i=0; i<4; i++)
	{
		if(sides[i]==BLOCK_U)
			return FALSE;
		if(sides[i]!=BLOCK_EMPTY && sides[i]!=BLOCK_UNUSABLE)
		{
			nSideCount++;
		}
	}

	return (nSideCount>=3)?TRUE:FALSE;
}

BOOL CSYGame::CheckI(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	//Cannot share a side with I, O, U.
	
	for(DWORD i=0; i<4; i++)
	{
		if(sides[i]==BLOCK_I || sides[i]==BLOCK_O || sides[i]==BLOCK_U)
			return FALSE;
	}
	
	return TRUE;
}
BOOL CSYGame::CheckO(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	BOOL bResult=FALSE;
	
	//Must side E cannot side A
	for(DWORD i=0; i<4; i++)
	{
		if(sides[i]==BLOCK_A)
			return FALSE;
			
		if(sides[i]==BLOCK_E)
			bResult=TRUE;
	}
	
	return bResult;
}

BOOL CSYGame::CheckU(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	//Cannot side E or I
	for(DWORD i=0; i<4; i++)
	{
		if(sides[i]==BLOCK_E || sides[i]==BLOCK_I)
			return FALSE;
	}
	
	return TRUE;
}

BOOL CSYGame::CheckY(DWORD x, DWORD y)
{
	BLOCK_TYPE sides[4];
	GetSides(x, y, sides);
	
	//No rules

	return TRUE;
}

void CSYGame::GetSides(DWORD x, DWORD y, CSYGame::BLOCK_TYPE* sides)
{
	const Block* pBlkSide;
	
	//Top
	pBlkSide=GetBlock(x, y-1);
	sides[0]=pBlkSide->nType;
	//Right
	pBlkSide=GetBlock(x+1, y);
	sides[1]=pBlkSide->nType;
	//Bottom
	pBlkSide=GetBlock(x, y+1);
	sides[2]=pBlkSide->nType;
	//Left
	pBlkSide=GetBlock(x-1, y);
	sides[3]=pBlkSide->nType;
}