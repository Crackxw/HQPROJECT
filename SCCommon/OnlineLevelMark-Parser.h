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
	SI32					m_MinLevel;												// �ּ� Level

	SI32					m_MaxLevelMark;											// LevelMark ��
	LevelMarkHeader*		m_lpLevelMark;											// LevelMark Data
	
public:
	OnlineLevelMarkParser();
	virtual	~OnlineLevelMarkParser();

	BOOL					Create(char* lpFileName);								// Create
	void					Release();												// Release

	LevelMarkHeader*		GetLevelMarkByLevel(SI32 NowLevel);						// Level������ �̿��Ͽ� Level Mark�� ���´�.
	LevelMarkHeader*		GetLevelMarkByMarkCode(SI32 LevelCode);					// MarkCode������ �̿��Ͽ� Level Mark�� ���´�.
};


#endif
