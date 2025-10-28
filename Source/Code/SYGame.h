#pragma once

class CSYGame
{
public:
	enum PLACE_RESULT{PLACE_OK, PLACE_WIN, PLACE_BAD};
	
	enum BLOCK_TYPE{
		BLOCK_EMPTY=0x80000000, 
		BLOCK_A=0, 
		BLOCK_E, 
		BLOCK_I, 
		BLOCK_O, 
		BLOCK_U, 
		BLOCK_Y, 
		BLOCK_UNUSABLE=0xFFFFFFFF};
		
	enum BLOCK_STATUS{
		STATUS_EMPTY,
		STATUS_PLACED,
		STATUS_FIXED};
		
	DWORD m_nTypeAvail[6];
public:
	class Block{public:BLOCK_TYPE nType; BLOCK_STATUS nStatus; BOOL bValid;};
		
private:
	static const DWORD EMPTY_BLOCK=0;
	static const DWORD UNUSABLE_BLOCK=0xFFFFFFFF;
	
	BOOL  m_bGameRunning;
	DWORD m_nGameSizeX, m_nGameSizeY;
	Block* m_GameMatrix;
public:
	CSYGame(void);
	~CSYGame(void);
	
	//StartGame - Loads the game file and starts a new game.  
	//Returns TRUE if the game was sucessfully loaded, FALSE 
	//if the game could
	BOOL StartGame(LPCTSTR szGameFile);
	void EndGame();
	PLACE_RESULT PlaceLetter(BLOCK_TYPE type, DWORD nX, DWORD nY, LPTSTR szOutMsg);
	BOOL RemoveLetter(DWORD nX, DWORD nY);
	
	void GetSize(DWORD& x, DWORD& y);
	const Block* GetBlock(const DWORD x, const DWORD y);
	void GetAvailLetters(DWORD* pCounts);
private:
	void SetBlock(DWORD x, DWORD y, BLOCK_TYPE nType, BLOCK_STATUS nStatus);
	void SetBlockValid(DWORD x, DWORD y, BOOL bValid=TRUE);
	void UpdateGameStatus();
	BOOL CheckA(DWORD x, DWORD y);
	BOOL CheckE(DWORD x, DWORD y);
	BOOL CheckI(DWORD x, DWORD y);
	BOOL CheckO(DWORD x, DWORD y);
	BOOL CheckU(DWORD x, DWORD y);
	BOOL CheckY(DWORD x, DWORD y);
	void GetSides(DWORD x, DWORD y, BLOCK_TYPE* sides);
};
