#include "SOCharKI.h"
#include "OnlineChar-Parser.h"
#include <stdio.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltSOCharKI::cltSOCharKI() : cltOnlineBaseCharKI()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltSOCharKI::~cltSOCharKI()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltSOCharKI::Init(OnlineText *pOnlineText, CHAR *pszFileName)
{	
	SI32					i;
	OnlineCharParser		CharParser;
	CharHead				*pCharHead;
	SI32					siTotalCharNum;

	if(CharParser.Init("Online\\CharInfo\\Character List.txt") == TRUE)
	{
		// 전체 캐릭터를 수를 얻어와서 메모리를 할당 받는다.
		cltOnlineBaseCharKI::Init(pOnlineText, siTotalCharNum = CharParser.GetCharTotalNum());		
				
		for(i = 0; i < siTotalCharNum; i++)
		{
			// 초기화를 수행한다.						
			pCharHead	=	CharParser.GetCharHeader(i);

			cltOnlineBaseCharKI::Init(i, pCharHead->uiID, pCharHead->uiCharName, pCharHead->siNation, pCharHead->siMoveSpeed, &pCharHead->Parameta,
										pCharHead->siItemDrop, pCharHead->siDropPercent, pCharHead->siGeneral);			
		}		

		CharParser.Free();		
		return TRUE;
	}	
	else
		return FALSE;
}	

