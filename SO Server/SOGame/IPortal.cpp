#include "SOMain.h"
#include "IPortal.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IPortal::IPortal() : OnlinePortalParser()
{
	m_pstPortalData	=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IPortal::~IPortal()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IPortal::Init(CHAR *pszPortalFileName)
{
	SI32		i;

	if(ReadText(pszPortalFileName) == TRUE)
	{		
		m_pstPortalData	=	new stPortalData[siPortalNum];

		for(i = 0; i < siPortalNum; i++)
		{
			// 陝 ん驍曖 等檜顫蒂 蟾晦�倆挬�.
			m_pstPortalData[i].uiMaxUsablePlayerNumPerDay	=	pPortalHeader[i].siLimitNum;
			m_pstPortalData[i].uiUsedPlayerNumPerDay		=	0;
		}

		return TRUE;
	}
	else
		return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п薯.
//	熱薑 橾濠	:	2002-08-05 螃�� 5:16:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IPortal::Free()
{
	OnlinePortalParser::Free();	

	if(m_pstPortalData)
	{
		delete [] m_pstPortalData;
		m_pstPortalData	=	NULL;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	跡瞳雖 裘曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-24 螃瞪 11:47:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	IPortal::GetDestMapID(UI16 uiPortalID)
{
	PortalHeader	*pPotal	=	GetPortal(uiPortalID);

	if(pPotal != NULL)
		return UI16(pPotal->siPortalMoveMapIndex);
	else
	{
		CHAR	szErrorText[1024];
		
		sprintf(szErrorText, "GetDestMapID :: Can't Find Slot Error : [%d]", uiPortalID);
		OutputDebugString(szErrorText);
		return 0;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跡瞳雖曖 謝ル蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
POINT	IPortal::GetDestPos(UI16 uiPortalID)
{
	PortalHeader		*pPotal	=	GetPortal(uiPortalID);
	POINT				ptDestPos = {0, 0};

	if(pPotal != NULL)
	{
		ptDestPos.x	=	pPotal->siPortalMoveMapXPosition;
		ptDestPos.y	=	pPotal->siPortalMoveMapYPosition;
	}

	return ptDestPos;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ん驍擊 餌辨и棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 5:22:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IPortal::UsePortal(UI16 uiPortalID)
{
	SI32	siPortalIndex;
	
	if((siPortalIndex =	GetPortalIndex(uiPortalID)) == -1)			return;	
	
	// ん驍擊 餌辨ж晦 嬪ж罹 薯и檜 氈朝雖 憲嬴螞棻.
	if(m_pstPortalData[siPortalIndex].uiMaxUsablePlayerNumPerDay != 0)
	{
		// ん驍擊 餌辨ц棻賊 ん驍擊 餌辨и Ы溯檜橫熱蒂 隸陛衛麵遽棻.
		m_pstPortalData[siPortalIndex].uiUsedPlayerNumPerDay++;		
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ん驍擊 餌辨й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 5:29:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IPortal::CanUsePortal(UI16 uiPortalID)
{
	SI32	siPortalIndex;
	
	if((siPortalIndex =	GetPortalIndex(uiPortalID)) == -1)																		return	FALSE;	
	
	// ん驍縑 餌辨 薯и檜 橈戲賊 餌辨й 熱 氈棻.
	if(m_pstPortalData[siPortalIndex].uiMaxUsablePlayerNumPerDay != 0)
	{	
		// ん驍擊 檜嘐 棻 餌辨ц朝雖 憲嬴螞棻.
		if(m_pstPortalData[siPortalIndex].uiUsedPlayerNumPerDay >= m_pstPortalData[siPortalIndex].uiMaxUsablePlayerNumPerDay)	return	FALSE;
	}

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ж瑞陛 滲唳腎歷棻.
//	熱薑 橾濠	:	2002-08-05 螃�� 5:32:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IPortal::DayChanged()
{
	SI32	i;

	// ж瑞縑 ん驍擊 餌辨и �蝦鷏� 蟾晦�� п遽棻.
	for(i = 0; i < siPortalNum; i++)
	{
		m_pstPortalData[i].uiUsedPlayerNumPerDay	=	0;
	}
}
