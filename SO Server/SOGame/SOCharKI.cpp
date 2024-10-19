#include "SOCharKI.h"
#include "OnlineChar-Parser.h"
#include <stdio.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltSOCharKI::cltSOCharKI() : cltOnlineBaseCharKI()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltSOCharKI::~cltSOCharKI()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ�� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltSOCharKI::Init(OnlineText *pOnlineText, CHAR *pszFileName)
{	
	SI32					i;
	OnlineCharParser		CharParser;
	CharHead				*pCharHead;
	SI32					siTotalCharNum;

	if(CharParser.Init("Online\\CharInfo\\Character List.txt") == TRUE)
	{
		// ��ü ĳ���͸� ���� ���ͼ� �޸𸮸� �Ҵ� �޴´�.
		cltOnlineBaseCharKI::Init(pOnlineText, siTotalCharNum = CharParser.GetCharTotalNum());		
				
		for(i = 0; i < siTotalCharNum; i++)
		{
			// �ʱ�ȭ�� �����Ѵ�.						
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

