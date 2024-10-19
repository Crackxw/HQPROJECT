//담 당 자 : 이 상 원
#include <GSL.h>

#include	"OnlineQuest-Parser.h"
#include	"OnlineText.h"
#include	"bindjxfile.h"


OnlineQuestParser::OnlineQuestParser()
{
	m_lpQuestData    = NULL;
	m_QuestDataCount = 0;

	m_NPCSortData_MaxCount  = 0;
	m_NPCSortData_DataCount = NULL;
	m_NPCSortData_Data      = NULL;
	m_siEventQuestCount		= 0;
}

OnlineQuestParser::~OnlineQuestParser()
{
	Free();
}

BOOL OnlineQuestParser::ReadText(char *pFileName,char* szNationCode,SI16 siYear,SI16 siMon,SI16 siDay)
{
	FILE	*fp	 	 = NULL;
	char	*pToken  = NULL;
	char	*pToken1 = NULL;
	char	*pToken2 = NULL;
	char	*pToken3 = NULL;
	char	szBuffer[1024];	
	char	szTemp[32];
	SI32	TempCount;
	SI16	i;
	SI32	siQuestReqConditionalCount;
	SI32	siQuestSkipConditionalCount;
	SI32	siQuestEndReqObjectCount;
	SI32	siQuestPayCount;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	while( fgets(szBuffer, 1024, fp) )
		m_QuestDataCount++;
	
	fseek( fp, SEEK_CUR, 0 );	
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	if(m_lpQuestData) delete[] m_lpQuestData;
	m_lpQuestData = new SQuestHeader[m_QuestDataCount];
	ZeroMemory(m_lpQuestData, sizeof(SQuestHeader) * m_QuestDataCount);

	m_NPCSortData_MaxCount = 0;
	for(i = 0; i < m_QuestDataCount; i++)
	{
		ZeroMemory(szTemp, 32);
		ZeroMemory(szBuffer, 1024);
		fgets(szBuffer, 1024, fp);

		siQuestReqConditionalCount  = 0;
		siQuestSkipConditionalCount = 0;
		siQuestEndReqObjectCount    = 0;
		siQuestPayCount             = 0;

		// Quest Code
		pToken = strtok(szBuffer, "\n\t\r");
		if(pToken) m_lpQuestData[i].siQuestCode = atoi(pToken);

		// Name
		pToken = strtok(NULL, "\n\t\r");

		// Desc
		pToken = strtok(NULL, "\n\t\r");

		// 캐릭터상태(우선 건너뜀)
		pToken = strtok(NULL, "\n\t\r");

		// End Type & End Code & End Quantity
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		pToken2 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Item") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_ITEM;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = atoi(pToken1);
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Work") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_WORK;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "BankBook") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_BANKBOOK;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Book") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_BOOK;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Mercenary") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_MERCENARY;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Monster") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_MONSTER;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = GetCharacterID(pToken1);
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Satiety") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_SATIETY;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Level") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_LEVEL;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Quest") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_QUEST;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Money") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_MONEY;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken, "Health") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_HEALTH;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = 0;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken,"Portal") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_PORTAL;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = atoi(pToken1);
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}
			else if(strcmp(pToken,"Npc") == 0)
			{
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectType     = QUESTOBJECTTYPE_NPC;
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectCode     = atoi(pToken1);
				m_lpQuestData[i].sQuestEndReqObject[siQuestEndReqObjectCount].m_QuestObjectQuantity = atoi(pToken2);
				siQuestEndReqObjectCount++;
			}   
		}

		// 회수여부
		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].m_QuestItemCollect			= atoi(pToken);

		//
		/*pToken  = strtok(NULL, "\n\t\r");
		if(strcmp(pToken,"0") != 0)
		{
			if(strstr(strlwr(pToken),strlwr(szNationCode)) != NULL)
				m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_PLAY;
			else
				m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_CANCEL;
		}
		else 
		{	
			m_lpQuestData[i].m_siQuestEvent					=	NORMAL_QUEST;
		}*/
		char* pTemp;
		int roop = 0;
		pTemp  = strtok(NULL, "&\n\t\r");
		if(strcmp(pTemp,"0") != 0)
		{
			while(roop == 0)
			{
				if(stricmp(pTemp,szNationCode) == 0)
				{
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_PLAY;
					pTemp = strtok(NULL,"\n\t\r");
					break;
				}
				else
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_CANCEL;
				pTemp = strtok(NULL,"&\n\t\r");

				//if(pTemp == NULL)	roop = 1;
				if(strcmp(pTemp,"0") == 0) roop = 1;
			}
		}
		else 
		{	
			m_lpQuestData[i].m_siQuestEvent					=	NORMAL_QUEST;
		}

		//이벤트 시작 날짜.
		pToken	= strtok(NULL,"\n\t\r");
		if(strcmp(pToken,"0") != 0)
		{
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stStartDate.siYear			=	atoi(strncpy(szTemp,pToken,4));
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stStartDate.siMon			=	atoi(strncpy(szTemp,(char *)&pToken[4],2));
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stStartDate.siDay			=	atoi(strncpy(szTemp,(char *)&pToken[6],2));

			if (QUEST_EVENT_PLAY == m_lpQuestData[i].m_siQuestEvent)	// robypark (2004/9/17 10:50) 해당 서비스 국가와 같을 경우에만 활성화되도록 체크
			{
				SI16 siEventDay = GetComPareDate(m_lpQuestData[i].m_stStartDate.siYear,m_lpQuestData[i].m_stStartDate.siMon,m_lpQuestData[i].m_stStartDate.siDay,
												siYear,siMon,siDay);

				if(siEventDay >= 0 )
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_PLAY;
				else
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_CANCEL;
			}
		}

		//이벤트 끝나는 날짜.
		pToken	= strtok(NULL,"\n\t\r");
		if(strcmp(pToken,"0") != 0)
		{	
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stEndDate.siYear			=	atoi(strncpy(szTemp,pToken,4));
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stEndDate.siMon			=	atoi(strncpy(szTemp,(char *)&pToken[4],2));
			ZeroMemory(szTemp, 32);
			m_lpQuestData[i].m_stEndDate.siDay			=	atoi(strncpy(szTemp,(char *)&pToken[6],2));

			if (QUEST_EVENT_PLAY == m_lpQuestData[i].m_siQuestEvent)	// robypark (2004/9/17 10:50) 해당 서비스 국가와 같을 경우에만 활성화되도록 체크, 이벤트 시작날짜가 지나서 이벤트가 시작되었는지 체크
			{
				SI16 siEventDay = GetComPareDate(m_lpQuestData[i].m_stEndDate.siYear,m_lpQuestData[i].m_stEndDate.siMon,m_lpQuestData[i].m_stEndDate.siDay,
													siYear,siMon,siDay);

				if(siEventDay > 0 )
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_CANCEL;
				else
					m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_PLAY;
			}
		}
		
		if(m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_PLAY)
			m_siEventQuestCount++;

		if(m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_PLAY || m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_CANCEL)
			m_siPossibleEventQuest++;

		// Reward Type 1 & Reward Quantity 1
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Money") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_MONEY;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_CREDIT;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Exp") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_EXP;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
		}

		// Reward Type 2 & Reward Quantity 2
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Money") == 0)
			{ 
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_MONEY;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_CREDIT;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Exp") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_EXP;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken1);
				siQuestPayCount++;
			}
		}

		// Reward Type 3 & Reward Code 3 & Reward Quantity 3
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		pToken2 = strtok(NULL, "\n\t\r");
		pToken3 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Money") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_MONEY;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken3);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_CREDIT;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken3);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Exp") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_EXP;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = 0;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken3);
				siQuestPayCount++;
			}
			else if(strcmp(pToken, "Item") == 0)
			{
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayType     = QUESTPAYTYPE_ITEM;
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayCode     = atoi(pToken2);
				m_lpQuestData[i].sQuestPay[siQuestPayCount].m_QuestPayQuantity = atoi(pToken3);
				siQuestPayCount++;
			}
		}

		// Nation
		pToken = strtok(NULL, "\n\t\r");
		if(pToken) m_lpQuestData[i].siNation = atoi(pToken);

		// Need Quest
		pToken = strtok(NULL, "\n\t\r");
		if(atoi(pToken) != 0)
		{
			m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_COMPLETIONQUEST;
			m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = 0;
			m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken);

			siQuestReqConditionalCount++;
		}

		// Need Type & Need Quantity
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		pToken2 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_CREDIT;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken2);
				siQuestReqConditionalCount++;
			}
			else if(strcmp(pToken, "Level") == 0)
			{
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_LEVEL;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken2);
				siQuestReqConditionalCount++;
			}
			else if(strcmp(pToken, "Battle") == 0)
			{
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_BATTLE;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken2);
				siQuestReqConditionalCount++;
			}
			else if(strcmp(pToken, "Quest") == 0)
			{
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_COMPLETIONQUEST;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken2);
				siQuestReqConditionalCount++;
			}
			else if(strcmp(pToken, "Item") == 0)
			{
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_ITEM;
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalCode     = atoi(pToken1);
				m_lpQuestData[i].sQuestReqConditional[siQuestReqConditionalCount].m_QuestConditionalQuantity = atoi(pToken2);
				siQuestReqConditionalCount++;
			}
		}

		// Skip Type & Skip Quantity
		pToken  = strtok(NULL, "\n\t\r");
		pToken1 = strtok(NULL, "\n\t\r");
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_CREDIT;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalQuantity = atoi(pToken1);
				siQuestSkipConditionalCount++;
			}
			else if(strcmp(pToken, "Level") == 0)
			{
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_LEVEL;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalQuantity = atoi(pToken1);
				siQuestSkipConditionalCount++;
			}
			else if(strcmp(pToken, "Battle") == 0)
			{
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_BATTLE;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalQuantity = atoi(pToken1);
				siQuestSkipConditionalCount++;
			}
			else if(strcmp(pToken, "Quest") == 0)
			{
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalType     = QUESTCONDITIONALTYPE_COMPLETIONQUEST;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalCode     = 0;
				m_lpQuestData[i].sQuestSkipConditional[siQuestSkipConditionalCount].m_QuestConditionalQuantity = atoi(pToken1);
				siQuestSkipConditionalCount++;
			}
			else if(strcmp(pToken, "Item") == 0)
			{
			}
		}

		// NPC Number
		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].siNPCCode = atoi(pToken);
		if((m_lpQuestData[i].siNPCCode + 1) > m_NPCSortData_MaxCount) m_NPCSortData_MaxCount = m_lpQuestData[i].siNPCCode + 1;

		// Subject
		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].siSubject = atoi(pToken);

		// Start Text
		for(TempCount = 0; TempCount < 6; TempCount++)
		{
			pToken  = strtok(NULL, "\n\t\r");
			m_lpQuestData[i].siText_Start[TempCount] = atoi(pToken);
		}

		// Middle Text
		for(TempCount = 0; TempCount < 2; TempCount++)
		{
			pToken  = strtok(NULL, "\n\t\r");
			m_lpQuestData[i].siText_Playing[TempCount] = atoi(pToken);
		}
		// Middle Text(우선 건너뜀)
		for(TempCount = 0; TempCount < 3; TempCount++)
			pToken  = strtok(NULL, "\n\t\r");

		// End Text
		for(TempCount = 0; TempCount < 6; TempCount++)
		{
			pToken  = strtok(NULL, "\n\t\r");
			m_lpQuestData[i].siText_End[TempCount] = atoi(pToken);
		}

		// Need Text
		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].siText_EndAfter[0] = 7;
		if(strcmp(pToken, "0") != 0)
		{
			if(strcmp(pToken, "Credit") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = ON_QUESTTEXT_STARTBEFORETEXT_CREDIT;
			}
			else if(strcmp(pToken, "Level") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = ON_QUESTTEXT_STARTBEFORETEXT_LV;
			}
			else if(strcmp(pToken, "Battle") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = ON_QUESTTEXT_STARTBEFORETEXT_BATTLE;
			}
			else if(strcmp(pToken, "Battle1") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = ON_QUESTTEXT_STARTBEFORETEXT_BATTLE;
				m_lpQuestData[i].siText_EndAfter[0]    = 9;
			}
			else if(strcmp(pToken, "Item") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = ON_QUESTTEXT_STARTBEFORETEXT_ITEM;
			}
			// actdoll (2004/03/31 13:53) : = 이벤트 = 독도지키기로 인하여 이벤트 메시지를 넣을 때 쓴다.
			else if(strcmp(pToken, "Event01") == 0)
			{
				m_lpQuestData[i].siText_StartBefore[0] = 20000230;
			}
		}

		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].m_siDesc = atoi(pToken);

		// robypark 2004/9/17 14:52
		// 추후 배송되는 아이템 당첨 관련 텍스트(메시지 박스에서 사용) ID 얻기
		pToken  = strtok(NULL, "\n\t\r");
		m_lpQuestData[i].siJackpotTextID = atoi(pToken);
	}

	fclose(fp);

	// NPC 별로 정렬
	if(m_NPCSortData_MaxCount)
	{
		m_NPCSortData_DataCount = new SI32[m_NPCSortData_MaxCount];
		ZeroMemory(m_NPCSortData_DataCount, sizeof(SI32) * m_NPCSortData_MaxCount);
		m_NPCSortData_Data      = new SI32*[m_NPCSortData_MaxCount];
		ZeroMemory(m_NPCSortData_Data, sizeof(SI32*) * m_NPCSortData_MaxCount);
		for(TempCount = 0; TempCount < m_QuestDataCount; TempCount++)
		{
			m_NPCSortData_DataCount[m_lpQuestData[TempCount].siNPCCode]++;
		}
		for(TempCount = 0; TempCount < m_NPCSortData_MaxCount; TempCount++)
		{
			m_NPCSortData_Data[TempCount]      = new SI32[m_NPCSortData_DataCount[TempCount]];
			m_NPCSortData_DataCount[TempCount] = 0;
		}
		for(TempCount = 0; TempCount < m_QuestDataCount; TempCount++)
		{
			m_NPCSortData_Data[m_lpQuestData[TempCount].siNPCCode][m_NPCSortData_DataCount[m_lpQuestData[TempCount].siNPCCode]] = TempCount;
			m_NPCSortData_DataCount[m_lpQuestData[TempCount].siNPCCode]++;
		}
	}

	return TRUE;
}

VOID OnlineQuestParser::Free()
{
	SI32		TempCount;
	if(m_lpQuestData)
	{
		delete[] m_lpQuestData;
		m_lpQuestData = NULL;
	}
	m_QuestDataCount = 0;

	if(m_NPCSortData_Data)
	{
		for(TempCount = 0; TempCount < m_NPCSortData_MaxCount; TempCount++)
		{
			if(m_NPCSortData_Data[TempCount])
			{
				delete[] m_NPCSortData_Data[TempCount];
			}
		}
		delete[] m_NPCSortData_Data;
		m_NPCSortData_Data = NULL;
	}
	if(m_NPCSortData_DataCount)
	{
		delete[] m_NPCSortData_DataCount;
		m_NPCSortData_DataCount = NULL;
	}
	m_NPCSortData_MaxCount  = 0;

	// 이벤트 퀘스트 개수 초기화
	m_siEventQuestCount		= 0;
}

SQuestHeader* OnlineQuestParser::GetQuest(SI16 siQuestCode)
{
	SI16		TempCount;

	// 우선 갯수를 검사한다.
	for(TempCount = 0; TempCount < m_QuestDataCount; TempCount++)
	{
		if(m_lpQuestData[TempCount].siQuestCode == siQuestCode)
		{
			return &m_lpQuestData[TempCount];
		}
	}

	return NULL;
}

SQuestHeader* OnlineQuestParser::GetQuest(SI16 siQuestCode, SI16 siNPCCode)
{
	SI16		TempCount;

	if(siNPCCode < 0) return NULL;

	// 우선 갯수를 검사한다.
	for(TempCount = 0; TempCount < m_NPCSortData_DataCount[siNPCCode]; TempCount++)
	{
		if(m_lpQuestData[m_NPCSortData_Data[siNPCCode][TempCount]].siQuestCode == siQuestCode)
		{
			return &m_lpQuestData[TempCount];
		}
	}

	return NULL;
}

SI32 OnlineQuestParser::GetCharacterID(char* lpCode)
{
	SI32	uiVal;

	uiVal		=	MAKEWORD(lpCode[0], atoi(&lpCode[1]));	

	return uiVal;
}

SI16 OnlineQuestParser::GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay)
{
	SI16 siCount;
	siCount = 0;
	
	SI16 siTempYear = siEndYear - siNowYear;
	//이것도 윤달 신경 안섰음.
	if(siTempYear > 0)
		siCount += siTempYear * 365;
	else
		siCount += siTempYear * 365;

	SI16 siTempMon = siEndMon - siNowMon;
	if(siTempMon > 0)
	{
		while(siEndMon != siNowMon)
		{
			siEndMon--;
			siCount += GetMDay(siEndMon);
		}
	}
	else
	{
		while(siEndMon != siNowMon)
		{
			siNowMon--;
			siCount -= GetMDay(siNowMon);
		}
	}
	SI16 siTempDay = siEndDay - siNowDay;
		siCount += siTempDay;

	return siCount;

}

VOID OnlineQuestParser::GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay)
{
	SI16 siTempItemTime = 0;
	SI16 siTempCount = 0;

	for(SI16 i = 1 ; i < siItemTime + 2;i++)
	{
		siNowDay++;
		if(GetMDay(siNowMon) == (siNowDay -1))
		{
			if(siNowMon >= 12)
			{
				siNowYear++;
				siNowMon = 1;
				siNowDay = 1;
			}
			else
			{
				siNowMon++;
				siNowDay = 1;
			}
		}
	}
	siEndYear = siNowYear;
	siEndMon  = siNowMon;
	siEndDay  = siNowDay;
}

SI16 OnlineQuestParser::GetMDay(SI16 siMon)
{
	//윤달 신경 안썼음.
	if(siMon == 1 || siMon == 3 || siMon == 5 || siMon == 7 || siMon == 8 || siMon == 10 || siMon == 12)
		return 31;
	else if(siMon == 4 || siMon == 6 || siMon == 9 || siMon == 11)
		return 30;
	else if(siMon == 2)
	{
		//if(siYear%4 == 0)
		//	return 29;
		//else 
			return 28;
	}
	return 0;
}
