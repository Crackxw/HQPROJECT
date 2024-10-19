/************************************************************************************************
	���ϸ� : IOnlineCharKI.h
	����� : ������
************************************************************************************************/
#ifndef _ONLINE_CHAR_IKI_H
#define _ONLINE_CHAR_IKI_H

#include <Online.h>
#include <OnlineCharKI.h>		
#include "OnlineChar-Parser.h"

class	IOnlineCharKI
{
private:
	SI32			siTotalCharKINum;
	OnlineCharKI	*pOnlineCharKI;
public:
	IOnlineCharKI();
	~IOnlineCharKI();

	BOOL	Init(cltOnlineWorld *pOnlineWorld);				// ������.
	VOID	Free();											// ����.	

	SI32		GetIndex(UI16 uiCharID);						// Char ID�� �����Ǿ��ִ� Index�� ���´�.
	UI16		GetCharID(SI32 siKindIndex);					// �ִϸ��̼��� ID�� ���´�.
	SI32		GetPlayerCharIDList(UI16 *pIndexArray);			// ���ΰ� ĳ������ ID �ε����� ���´�.
	UI32		GetName(SI32 siKindIndex);						// ������ ��� ������ �̸��� ���´�.
	SI32		GetDescText(SI32 siKindIndex);					// ĳ���� ���� Text
	SI32		GetClass(SI32 siKindIndex);						// ĳ������ Ŭ������ ���´�.
	SI32		GetNation(SI32 siKindIndex);					// ĳ������ Nation�� ���´�.
	SI16		GetMoveSpeed(SI32 siKindIndex);					// Move Speed �� ���´�.
	BOOL		IsPlayer(UI16 uiCharID);						// �ش� �Ƶ� ���ΰ����� �˾� ����.
	BOOL		IsSoldier(UI16 uiCharID);
	UI08		GetWeaponType(UI16 uiCharID);					// ���� Ÿ�Ը� ���´�.
	SI16		GetGeneral(UI16 uiCharID);						// ��� Flag
	CharHead*	GetCharHeader(UI16 uiCharID);					// ĳ���� ����(�谰����� �����Ͱ� ����)
};


#endif
