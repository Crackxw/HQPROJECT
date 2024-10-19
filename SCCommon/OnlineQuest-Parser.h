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

	SI32				siNation;											// ����
	SI16				siNPCCode;											// NPC Code
	SI32				siSubject;											// Quest ����

	SQuestConditional	sQuestReqConditional[ON_QUSETDATA_MAXQUESTREQCONDITIONAL];			// Quest �ʿ�����
	SQuestConditional	sQuestSkipConditional[ON_QUSETDATA_MAXQUESTSKIPCONDITIONAL];		// Quest Skip����

	SQuestObject		sQuestEndReqObject[ON_QUSETDATA_MAXQUESTENDREQOBJECT];				// Quest �������
	SQuestPay			sQuestPay[ON_QUSETDATA_MAXQUESTPAY];								// Quest ����� ����

	SI32				siText_StartBefore[ON_QUSETDATA_TEXT];								// Quest ������ Text
	SI32				siText_Start[ON_QUSETDATA_TEXT];									// Quest ���۽� Text
	SI32				siText_Playing[ON_QUSETDATA_TEXT];									// Quest ������ Text
	SI32				siText_End[ON_QUSETDATA_TEXT];										// Quest ����� Text
	SI32				siText_EndAfter[ON_QUSETDATA_TEXT];									// Quest ������ Text
	
	SI32				m_QuestItemCollect;
	SI16				m_siQuestEvent;
	SI32				m_siDesc;

	SDate				m_stStartDate;														// �̺�Ʈ ����Ʈ ������
	SDate				m_stEndDate;														// �̺�Ʈ ����Ʈ ������

	// robypark 2004/9/17 14:53
	SI32				siJackpotTextID;													// ���� ��۵Ǵ� ������ ��÷ ���� �ؽ�Ʈ(�޽��� �ڽ����� ���)
};


class OnlineQuestParser
{
public:
	SQuestHeader*			m_lpQuestData;
	SI32					m_QuestDataCount;
	SI16					m_siEventQuestCount;											// ���� Play�Ҽ� �ִ� Event����.
	SI16					m_siPossibleEventQuest;											// �̺�Ʈ ������ ����Ʈ�� ����.

	SI32					m_NPCSortData_MaxCount;
	SI32*					m_NPCSortData_DataCount;
	SI32**					m_NPCSortData_Data;

public:
	OnlineQuestParser();
	~OnlineQuestParser();

	BOOL					ReadText(char *pFileName,char* szNationCode,SI16 siYear,SI16 siMon,SI16 siDay);					// Quest �ε�
	VOID					Free();										// �����

	SQuestHeader*			GetQuest(SI16 siQuestCode);					// Get Quest
	SQuestHeader*			GetQuest(SI16 siQuestCode, SI16 siNPCCode);	// Get Quest
	SI32					GetCharacterID(char* lpCode);				// Get Character Code

	SI16			GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay);
	VOID			GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay);
	SI16			GetMDay(SI16 siMon);
};


#endif
