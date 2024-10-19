/************************************************************************************************
	���ϸ� : IOnlineCharKI.cpp
	����� : ������

	Character�� ������ �����Ѵ�.
************************************************************************************************/
#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineChar-Parser.h"

//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
//----------------------------------------------------------------------------------------------------------------	
IOnlineCharKI::IOnlineCharKI()
{
	pOnlineCharKI	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	����.
//----------------------------------------------------------------------------------------------------------------
IOnlineCharKI::~IOnlineCharKI()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.(�赵 ĳ���ͷ� �����Ͽ� ���� �ʱ�ȭ�� �Ѵ�.)
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineCharKI::Init(cltOnlineWorld *pOnlineWorld)
{	
	SI32				i;	
	CharHead			*pCharHead;
	OnlineShipKI		*pOnlineShipKI;
	SI32				siFieldCharNum;


	// �켱 �� �������̽� Ŭ������ ���������� �ʱ�ȭ �Ǿ����� �˾ƿ´�.
	if(pOnlineWorld->pIOnlineShipKI == NULL)			return	FALSE;
	if(pOnlineWorld->pIOnlineShipKI->IsInit() == FALSE)	return	FALSE;


	pOnlineWorld->pOnlineCharParser = new OnlineCharParser;
	if(pOnlineWorld->pOnlineCharParser->Init("Online\\CharInfo\\Character List.txt") == FALSE)	
		return FALSE;

	// �ؽ�Ʈ ȭ�Ͽ� �ִ� �ʵ� ĳ������ ���� ���´�.
	siTotalCharKINum	=	siFieldCharNum	=	pOnlineWorld->pOnlineCharParser->GetCharTotalNum();

	// ���� ���� ���´�.
	siTotalCharKINum	+=	pOnlineWorld->pIOnlineShipKI->GetTotalShipNum();

	pOnlineCharKI	=	new	OnlineCharKI[siTotalCharKINum + 1];

	for(i = 1; i <= siTotalCharKINum; i++)
	{
		if(i <= siFieldCharNum)
		{
			// �ʵ忡 �ִ� ĳ���͵��̴�.
			pCharHead	=	pOnlineWorld->pOnlineCharParser->GetCharHeader((i - 1));
			
			// ĳ���͸� �ʱ�ȭ �Ѵ�.
			if(pOnlineCharKI[i].Init(pCharHead->uiID, pCharHead->siNation, pCharHead->uiCharName, pCharHead->siDescText, pCharHead->siMoveSpeed, pCharHead->siNeedLv, pCharHead->uiWeaponType, pCharHead->siGeneral, pCharHead, pOnlineWorld) == FALSE)
			{
				clGrp.Error("Character Initialize Error", "[%d]", pCharHead->uiID);				
			}
		}
		else
		{
			// ���̴�.
			pOnlineShipKI	=	pOnlineWorld->pIOnlineShipKI->GetInfoByIndex(i - siFieldCharNum - 1);		

			// ĳ���͸� �ʱ�ȭ �Ѵ�.
			if(pOnlineCharKI[i].Init(pOnlineShipKI->GetID(), 0, pOnlineShipKI->GetName(), 0, pOnlineShipKI->GetMoveSpeed(), 0, 0, 0, NULL, pOnlineWorld) == FALSE)
			{
				clGrp.Error("Character Initialize Error", "[%d]", pCharHead->uiID);				
			}
			
		}	
	}


	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	����.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineCharKI::Free()
{
	if(pOnlineCharKI)
	{
		delete [] pOnlineCharKI;
		pOnlineCharKI = NULL;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ִϸ��̼��� ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	IOnlineCharKI::GetCharID(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetCharID();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Char ID�� �����Ǿ��ִ� Index�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineCharKI::GetIndex(UI16 uiCharID)
{
	SI32	i;

	for(i = 1; i <= siTotalCharKINum; i++)
	{
		if(pOnlineCharKI[i].GetCharID() == uiCharID)
			return i;
	}

	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ΰ� ĳ������ ID �ε����� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineCharKI::GetPlayerCharIDList(UI16 *pIndexArray)
{
	SI32	i;
	SI32	siIndexArrayCounter;

	siIndexArrayCounter	=	0;
	for(i = 1; i <= siTotalCharKINum; i++)
	{		
		if(pOnlineCharKI[i].GetClass() == ON_CHAR_KI_CLASS_PLAYER)
		{
			pIndexArray[siIndexArrayCounter]	=	pOnlineCharKI[i].GetCharID();			
			siIndexArrayCounter++;			
		}						
	}

	return siIndexArrayCounter;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ��� ������ �̸��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	IOnlineCharKI::GetName(SI32 siKindIndex)
{
	if(siKindIndex <= 0)	return 0;
	return pOnlineCharKI[siKindIndex].GetName();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���� ���� Text
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineCharKI::GetDescText(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetDescText();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ Ŭ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineCharKI::GetClass(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetClass();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ Nation�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineCharKI::GetNation(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetNation();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Move Speed �� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	IOnlineCharKI::GetMoveSpeed(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetMoveSpeed();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� Ÿ�Ը� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	IOnlineCharKI::GetWeaponType(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetWeaponType();
	else				return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� �Ƶ� ���ΰ����� �˾� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineCharKI::IsPlayer(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if(siIndex == -1)	return	FALSE;
	else
		if(pOnlineCharKI[siIndex].GetClass() == ON_CHAR_KI_CLASS_PLAYER)	return	TRUE;	

	return	FALSE;
}

BOOL	IOnlineCharKI::IsSoldier(UI16 uiCharID)
{
	SI32	siIndex =	GetIndex(uiCharID);

	if(siIndex == -1)	return	FALSE;
	else
		if(pOnlineCharKI[siIndex].GetClass() == ON_CHAR_KI_CLASS_SOLDER)	return	TRUE;	

	return	FALSE;
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� Flag
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	IOnlineCharKI::GetGeneral(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetGeneral();
	else				return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���� ����(�谰����� �����Ͱ� ����)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
CharHead*	IOnlineCharKI::GetCharHeader(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetCharHeader();
	else				return NULL;
}
