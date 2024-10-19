#include "SOMain.h"
#include "ISONPCKI.h"
#include "ParametaBox.h"
#include "OnlineChar-Parser.h"

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
INPCKI::INPCKI()
{
	pNPCKI				=	NULL;
	siTotalNPCKINum		=	0;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
INPCKI::~INPCKI()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.
//----------------------------------------------------------------------------------------------------------------
BOOL	INPCKI::Init()
{
	SI32					i;
	OnlineCharParser		CharParser;
	CharHead				*pCharHead;
	SI32					siTotalCharNum;
	SI32					siNpcCounter;

	if(CharParser.Init("Online\\CharInfo\\Character List.txt") == TRUE)
	{
		// 전체 캐릭터 수를 얻어온다.
		siTotalCharNum		=	CharParser.GetCharTotalNum();					

		// 전체 몬스터 수를 얻어온다.
		siTotalNPCKINum		=	CharParser.GetMonsterTotalNum();				

		pNPCKI	=	new	NPCKI[siTotalNPCKINum];		
		if(pNPCKI == NULL) return FALSE;

		for(i = siNpcCounter = 0; i < siTotalCharNum; i++)
		{
			if(siNpcCounter >= siTotalNPCKINum)
			{
				break;
			}

			// 초기화를 수행한다.						
			pCharHead	=	CharParser.GetCharHeader(i);						

			if(pCharHead->siAttri == ON_ATTRI_MONSTER)
			{			

				pNPCKI[siNpcCounter].Init(pCharHead->uiID, pCharHead->siNation, pCharHead->siMoveSpeed);			
				siNpcCounter++;			
			}				
		}	

		CharParser.Free();		
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제.
//----------------------------------------------------------------------------------------------------------------
VOID	INPCKI::Free()
{
	SI32	i;

	if(pNPCKI != NULL)
	{
		for(i = 0; i < siTotalNPCKINum; i++)	pNPCKI[i].Free();					

		delete [] pNPCKI;
		pNPCKI				=	NULL;
		siTotalNPCKINum		=	0;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	전체 NPC 수를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	INPCKI::GetTotalMonsterNum()
{
	return siTotalNPCKINum;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	NPC의 ID를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
UI16	INPCKI::GetID(UI16 uiIndex)
{
	return pNPCKI[uiIndex].GetID();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	UI16	FindIndex(UI16 uiKind);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	INPCKI::FindIndex(UI16 uiKind)
{
	SI32 i;

	for(i = 0; i < siTotalNPCKINum; i++)
	{
		if(pNPCKI[i].GetID() == uiKind)
			return i;	
	}
	return -1;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 이동 속도를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	INPCKI::GetMoveSpeed(UI16 uiKind)
{
	SI32	FoundIndex;

	if((FoundIndex = FindIndex(uiKind)) != -1)	
		return pNPCKI[FoundIndex].GetMoveSpeed();	
	else
		return 0;
}

