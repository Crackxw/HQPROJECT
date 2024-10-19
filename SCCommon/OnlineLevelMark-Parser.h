#ifndef __ONLINELEVELMARK_PARSER_H
#define __ONLINELEVELMARK_PARSER_H

typedef struct
{
	SI32	m_LevelMarkCode;

	SI32	m_MinLevel;
	SI32	m_MaxLevel;
} LevelMarkHeader;


class OnlineLevelMarkParser
{
protected:
	SI32					m_MinLevel;												// 최소 Level

	SI32					m_MaxLevelMark;											// LevelMark 수
	LevelMarkHeader*		m_lpLevelMark;											// LevelMark Data
	
public:
	OnlineLevelMarkParser();
	virtual	~OnlineLevelMarkParser();

	BOOL					Create(char* lpFileName);								// Create
	void					Release();												// Release

	LevelMarkHeader*		GetLevelMarkByLevel(SI32 NowLevel);						// Level정보를 이용하여 Level Mark를 얻어온다.
	LevelMarkHeader*		GetLevelMarkByMarkCode(SI32 LevelCode);					// MarkCode정보를 이용하여 Level Mark를 얻어온다.
};


#endif
