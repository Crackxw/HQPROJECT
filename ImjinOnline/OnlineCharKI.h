/************************************************************************************************
	���ϸ� : OnlineCharKI.h
	����� : ������
************************************************************************************************/
#ifndef _ONLINE_CHAR_KI_H
#define _ONLINE_CHAR_KI_H

#define	ON_CHAR_KI_CLASS_UNKNOWN	0		// �� �� ���� ĳ���� Ŭ�����̴�.
#define	ON_CHAR_KI_CLASS_PLAYER		1		// ���ΰ� ĳ�����̴�.
#define	ON_CHAR_KI_CLASS_MONSTER	2		// ���� ĳ�����̴�.
#define	ON_CHAR_KI_CLASS_SOLDER		3		// �뺴 ĳ�����̴�.
#define	ON_CHAR_KI_CLASS_SHIP		4		// ���̴�.


#include <online.h>
#include "OnlineChar-Parser.h"

class	OnlineCharKI
{
private:
	UI16			uiCharID;	

	SI16			siMoveSpeed;	
	SI32			siHeightIndent;
	SI32			siFrameDelay;					// �������� ������

	SI32			siClass;						// ĳ���� Ŭ����.
	SI32			siNation;						// ����
	SI16			siNeedLevel;					// �뺴 ���Խ� �ʿ� ����

	UI32			uiName;							// ĳ������ �̸� �����̴�.(�÷��̾ �ƴ� ������ ��쿡�� ��ȿ�ϴ�.)
	SI32			siDescText;						// ĳ���� ���� Text
	UI08			uiWeaponType;					// ĳ���Ͱ� ��� �� �� �ִ� ���� ����

	SI16			siGeneralFlag;					// ��� Flag
	CharHead		sCharHeader;					// ĳ���� ����(�谰����� �����Ͱ� ����)

	cltOnlineWorld	*pMyOnlineWorld;				// �¶��� ������ Ŭ����.

private:
	VOID			AnalysisCharID();				// ĳ������ ID�� �м��Ͽ� ������ �������ش�.
public:
	OnlineCharKI();
	~OnlineCharKI();
	
	BOOL		Init(UI16 uicharID, SI32 Nation, UI32 uiname, SI32 siDescText, SI16 simovespeed, SI16 siReqLv, UI08 uiWeaponType, SI16 siGeneralFlag, CharHead* lpCharHeader, cltOnlineWorld *pOnlineWorld);// �ʱ�ȭ.	
	VOID		Free();																										// �޸𸮸� �����Ѵ�.
	UI16		GetCharID();																								// �ִϸ��̼��� ID�� ���´�.
	SI32		GetClass();																									// Ŭ������ ���´�.
	SI32		GetNation();																								// ������ ���´�.
	UI32		GetName();																									// ������ �̸��� ���´�.
	SI32		GetDescText();																								// ĳ���� ���� Text
	SI16		GetMoveSpeed();																								// Move Speed �� ���´�.
	SI16		GetNeedLevel();																								// �뺴 ���Խ� �ʿ� ����
	UI08		GetWeaponType();																							// ĳ���Ͱ� ��� �� �� �ִ� ���� Ÿ��
	SI16		GetGeneral();																								// ��� Flag
	CharHead*	GetCharHeader();																							// ĳ���� ����(�谰����� �����Ͱ� ����)
};	

#endif
