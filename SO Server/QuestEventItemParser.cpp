#include <stdio.h>
#include "QuestEventItemParser.h"
#include "stdlib.h"
#include "Defines.h"


CQuestEventItemParser::CQuestEventItemParser()
{

}

CQuestEventItemParser::~CQuestEventItemParser()
{

}

int CQuestEventItemParser::Load(char *szFilename,int siQuestCode)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = "\t";
	CHAR	*pToken;
	int		siCount	= 0;

	m_siEventQuestItemIDCount = 0;

	m_siQuestCode		= siQuestCode;

	ZeroMemory(&sQuestEventItem,sizeof(stQuestEventItem) * MAX_ITEM_EVENT);

	if((fp = fopen(szFilename, "r")) != NULL)
	{
		fgets(szReadBuffer, 1023, fp);
		fgets(szReadBuffer, 1023, fp);

		while(fgets(szReadBuffer, 1023, fp) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			sQuestEventItem[siCount].siItemNum  = atoi(pToken);

			pToken =	strtok(NULL, cSep);
			sQuestEventItem[siCount].bDeliveryItem	= atoi(pToken);

			pToken =	strtok(NULL, cSep);
			sQuestEventItem[siCount].bLogUseItemID = atoi(pToken);

			pToken =	strtok(NULL, cSep);
			strcpy(sQuestEventItem[siCount].szItemName,pToken);

			pToken =	strtok(NULL, cSep);
			sQuestEventItem[siCount].siDropPrecent = atoi(pToken);

			pToken =	strtok(NULL, cSep);
			sQuestEventItem[siCount].siNextItemNum = atoi(pToken);

			siCount++;
		}
	}

	m_siEventQuestItemIDCount = siCount;

	fclose(fp);
	return siCount;
}

stQuestEventItem* CQuestEventItemParser::GetItemName(int siItemNum)
{
	int siCount;

	for(siCount = 0; siCount < MAX_ITEM_EVENT ; siCount++)
	{
		if(sQuestEventItem[siCount].siItemNum == siItemNum)
		{
			return &sQuestEventItem[siCount];
		}
	}
	
	return NULL;
}

BOOL	CQuestEventItemParser::IsLogUseItem(int siItemNum)
{
	if(sQuestEventItem[siItemNum].bLogUseItemID == TRUE)
		return TRUE;
	else
		return FALSE;

	return FALSE;
}

int		CQuestEventItemParser::GetNextItemID(int siItemNum)
{
	return sQuestEventItem[siItemNum].siNextItemNum;
}


CQuestEventItemMgr::CQuestEventItemMgr()
{
	m_pQuestEventItem	   = NULL;
	m_siEventQuestMgrCount = 0;
}

CQuestEventItemMgr::~CQuestEventItemMgr()
{
	if(m_pQuestEventItem)
	{
		delete[] m_pQuestEventItem;
		m_pQuestEventItem = NULL;
	}
}

VOID	CQuestEventItemMgr::Init(short siCount)
{
	m_pQuestEventItem	=	new CQuestEventItemParser[siCount];	
	
}

VOID	CQuestEventItemMgr::Load(char* szFilename,int siQuestCode)
{
	m_pQuestEventItem[m_siEventQuestMgrCount].Load(szFilename,siQuestCode);
	m_siEventQuestMgrCount++;
}

CQuestEventItemParser* CQuestEventItemMgr::GetEventQuest(int siQuestCode)
{
	for(int i = 0 ; i < m_siEventQuestMgrCount ; i++)
	{
		if(m_pQuestEventItem[i].m_siQuestCode == siQuestCode)
		{
			return &m_pQuestEventItem[i];
		}
	}
	return NULL;
}
