#ifndef __ONLINEQUEST_PARSER_H
#define __ONLINEQUEST_PARSER_H

#include	"OnlineCommonStruct.h"


#define		QUEST_EVENT_PLAY				0
#define		QUEST_EVENT_CANCEL				1
#define		NORMAL_QUEST					2

struct SDate
{
	SI16	siYear;
	SI16	siMon;
	SI16	siDay;
};

struct	SQuestHeader
{
	SI16				siQuestCode;										// Code

	SI32				siNation;											// 국가
	SI16				siNPCCode;											// NPC Code
	SI32				siSubject;											// Quest 제목

	SQuestConditional	sQuestReqConditional[ON_QUSETDATA_MAXQUESTREQCONDITIONAL];			// Quest 필요조건
	SQuestConditional	sQuestSkipConditional[ON_QUSETDATA_MAXQUESTSKIPCONDITIONAL];		// Quest Skip조건

	SQuestObject		sQuestEndReqObject[ON_QUSETDATA_MAXQUESTENDREQOBJECT];				// Quest 종료목적
	SQuestPay			sQuestPay[ON_QUSETDATA_MAXQUESTPAY];								// Quest 종료시 보상

	SI32				siText_StartBefore[ON_QUSETDATA_TEXT];								// Quest 시작전 Text
	SI32				siText_Start[ON_QUSETDATA_TEXT];									// Quest 시작시 Text
	SI32				siText_Playing[ON_QUSETDATA_TEXT];									// Quest 진행중 Text
	SI32				siText_End[ON_QUSETDATA_TEXT];										// Quest 종료시 Text
	SI32				siText_EndAfter[ON_QUSETDATA_TEXT];									// Quest 종료후 Text
	
	SI32				m_QuestItemCollect;
	SI16				m_siQuestEvent;
	SI32				m_siDesc;

	SDate				m_stStartDate;														// 이벤트 퀘스트 시작일
	SDate				m_stEndDate;														// 이벤트 퀘스트 종료일

	// robypark 2004/9/17 14:53
	SI32				siJackpotTextID;													// 추후 배송되는 아이템 당첨 관련 텍스트(메시지 박스에서 사용)
};


class OnlineQuestParser
{
public:
	SQuestHeader*			m_lpQuestData;
	SI32					m_QuestDataCount;
	SI16					m_siEventQuestCount;											// 지금 Play할수 있는 Event갯수.
	SI16					m_siPossibleEventQuest;											// 이벤트 가능한 퀘스트의 갯수.

	SI32					m_NPCSortData_MaxCount;
	SI32*					m_NPCSortData_DataCount;
	SI32**					m_NPCSortData_Data;

public:
	OnlineQuestParser();
	~OnlineQuestParser();

	BOOL					ReadText(char *pFileName,char* szNationCode,SI16 siYear,SI16 siMon,SI16 siDay);					// Quest 로드
	VOID					Free();										// 지운다

	SQuestHeader*			GetQuest(SI16 siQuestCode);					// Get Quest
	SQuestHeader*			GetQuest(SI16 siQuestCode, SI16 siNPCCode);	// Get Quest
	SI32					GetCharacterID(char* lpCode);				// Get Character Code

	SI16			GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay);
	VOID			GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay);
	SI16			GetMDay(SI16 siMon);
};


#endif
