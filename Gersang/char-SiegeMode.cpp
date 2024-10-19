//--------------------------------------------------------------------
//  ����: �Ҷ������� ��õ��� ����� ����� �� �Ϲ� ���� ���� ��� ������
//		 ó���ϱ����� �Լ��� ������ �ִ�.
//
//  ������: 2004.6.4 13:30,	robypark
//  �ֱټ�����: 2004.6.4 13:30
// 
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

#include "..\Gersang\text.h"
#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include "..\Gersang\music.h"
#include <CharOrg.h>
#include "..\Gersang\yprg.h"
#include <OnlineImjin2i.h>
#include <OnlineBattleMsg.h>
#include <clGame.h>

#include <bullet.h>
#include <smoke.h>
#include <MyNet-Game.h>
#include <Etc.h>
#include <setchar.h>
#include "OnlineGeneral-Parser.h"

#include <myfile.h>
#include <magic.h>

extern OnlineBattleInfo OBInfo;
extern _OnlineBattleMsgQue OBMQue;

extern SI32	UserPlayerNumber;
extern _MyNetGame MyNet;
extern _MainData GD;
// ������ �� ��ǥ������ �߽ɰ�, 
extern int MapCenterX[][MAX_MAP_YSIZE];
extern int MapCenterY[][MAX_MAP_YSIZE];
extern _KindInfo KI[];
extern _NewID ZeroID;
extern _clGame *pGame;
extern	_Text 				Text;								// �ؽ�Ʈ 
extern _Char   CR[MAX_CHAR_NUMBER];       // ĳ������ ������ 

// robypark 2004/6/4 13:33
// �Ҷ������� ������ ���� �Լ�
// ��õ��� ������ => �Ϲ� �Ҷ�������
BOOL _Char::ChangeModeFarangi(void)
{
	SetAnimation(ANI_MAGIC, &m_siMagicAniStep);

	// �ִϸ��̼� �� �ٸ� ��� ���� �� �Է� ����
	m_scBusySwitch=TRUE;

	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{
		// �ִϸ��̼��� �� ������, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber() - 1) )
		{
			// robypark 2004/6/14 14:35
			// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
			BOOL bSelectedCharacter = m_scSelectedSwitch;

			// ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
			if(m_scSelectedSwitch == TRUE)
			{
				SetSelect(FALSE, FALSE);
			}

			// ��õ��� ����� ����Ͽ� ���� �������̴�.
			// �Ϲ� �Ҷ��������� �����Ѵ�.
			UI16 uikind;

			// �Ҷ��������� �����Ѵ�.
			uikind = KIND_ON_FARANGI_VEHICLE;

			_ImportantParameta ip;
			ZeroMemory(&ip, sizeof(_ImportantParameta));

			// ��õ����� �� �ɷ�ġ �� ���¸� �ӽ� �����Ϳ� ����
			ip = m_ParmeterBox.IP;

			// robypark 2004/6/14 11:03
			// ������ �������� ������ �ش�.
			MyWeapons tempMyWeapons = MW;

			// üũ���� �����Ͽ� ������ ���´�.
//			ip.UpdateCheckSum();
			
//			DeleteID();

			char szTemp[11];
			strcpy(szTemp,GetName());

			// �Ҷ��������� �����Ѵ�. ���� ����(����)�� �ƴ϶� �����̴�.
			// ��õ��Ұ� ������ �ִ� �����۵鵵 �Ҷ����������� ���޵ȴ�.
			// ���⵵ �����ؼ�
			SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
						100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

			CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

			// robypark 2004/6/14 14:35
			// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
			if (bSelectedCharacter == TRUE)
				SetSelect(bSelectedCharacter, FALSE);

			// ĳ���Ͱ� ����Ǿ����� ��ο��� �˸���.
			pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}

	return TRUE;
}

// robypark 2004/6/4 13:33
// �Ҷ������� ������ ���� �Լ�
// �Ϲ� �Ҷ������� => ��õ��� ������
BOOL _Char::ChangeModeRoarOfLaughter(void)
{
	if (m_siMagicAniStep == 0)
	{
		// �������� �ٿ��ش�. 
		if( m_ParmeterBox.IP.GetMana() >= MAGICPOWER_SIEGE_MODE )
		{
			// �������� �ٿ��ش�. 
			ReduceMana(MAGICPOWER_SIEGE_MODE);

			// robypark 2004/6/15 17:59
			// �Ҷ����������� ��õ��ҷ� �����ϴ� �ִϸ��̼��� ��õ��� ��������Ʈ ���Ͽ� ����
			// �ϰ� ����Ǿ���.
			ChangeModeRoarOfLaughter_Immediately();
			m_scBusySwitch = TRUE;
			CurrentOrder = ORDER_SIEGE_MODE;
		}
		else
		{
			m_scBusySwitch = FALSE;
			CurrentOrder = ORDER_WAIT;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
	}

	SetAnimation(ANI_MAGIC2, &m_siMagicAniStep);
	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{
		// �ִϸ��̼��� �� ������, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Magic2CharAnimationIndex].GetFrameNumber() - 1) )
		{
			// robypark 2004/6/14 14:35
			// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
			BOOL bSelectedCharacter = m_scSelectedSwitch;

			// ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
			if(m_scSelectedSwitch == TRUE)
			{
				SetSelect(FALSE, FALSE);
			}

			// �Ҷ��������� ��õ��� ����� ����Ͽ� ������� �����Ϸ� �Ѵ�.
			// �������� ��õ��ҷ� �����Ѵ�.
			UI16 uikind;

			// ��õ��ҷ� ����
			uikind = KIND_ON_ROAR_OF_LAUGHTER;

			_ImportantParameta ip;
			ZeroMemory(&ip, sizeof(_ImportantParameta));

			// �Ҷ��������� �� �ɷ�ġ �� ���¸� ��õ��Ҹ� ���� �ӽ÷� �����Ѵ�.
			ip = m_ParmeterBox.IP;

			// robypark 2004/6/14 11:03
			// ������ �������� ������ �ش�.
			MyWeapons tempMyWeapons = MW;

			// �����߻��� �������� üũ���� �����Ѵ�.
//			ip.UpdateCheckSum();
				
			char szTemp[11];
			strcpy(szTemp,GetName());

			// ��õ��� ĳ���͸� �����Ѵ�.
			// ���� ����(����)�� �ƴ϶� �����̴�.
			// �Ҷ��������� ������ �ִ� �����۵鵵 ��õ��ҿ��� ���޵ȴ�.
			// robypark 2004/6/15 10:49
			// ��õ����� ���¿��� ���Ⱑ ����Ǵ� ���� �������� 
			// MW.GetWearItem(ON_WEAR_WEAPON_ITEM) ���ڸ� �߰��Ͽ���.
			SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
						100, GetPlayerNumber(), &(ip), &tempMyWeapons, 
						m_siOnlineID, CharOrderNumber);//, tempMyWeapons.GetWearItem(ON_WEAR_WEAPON_ITEM));
			// tempMyWeapons.GetWearItem(ON_WEAR_WEAPON_ITEM) �� �κ��� �߰��� ��� ��õ��� ���� �Ŀ� �� ĳ���Ͱ�
			// ������ �ȵǴ� ������ �����.

			CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

			// robypark 2004/6/14 14:35
			// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
			if (bSelectedCharacter == TRUE)
				SetSelect(bSelectedCharacter, FALSE);

			// ĳ���Ͱ� ����Ǿ����� ��ο��� �˸���.
			pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}

	return TRUE;
}

// robypark 2004/6/10 11:14
// ��õ��ҿ��� ��� �Ҷ��������� �����Ѵ�.
BOOL _Char::ChangeModeFarangi_Immediately(void)
{
	// ��õ��� ����� ����Ͽ� ���� �������̴�.
	// �Ϲ� �Ҷ��������� �����Ѵ�.
	UI16 uikind;

	// robypark 2004/6/14 14:35
	// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
	BOOL bSelectedCharacter = m_bAlreadySelectedBeforeDead;

	// ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
	if(m_scSelectedSwitch == TRUE)
	{
		SetSelect(FALSE, FALSE);
	}

	// �Ҷ��������� �����Ѵ�.
	uikind = KIND_ON_FARANGI_VEHICLE;

	_ImportantParameta ip;
	ZeroMemory(&ip, sizeof(_ImportantParameta));

	// ��õ����� �� �ɷ�ġ �� ���¸� �ӽ� �����Ϳ� ����
	ip = m_ParmeterBox.IP;

	// robypark 2004/6/14 11:03
	// ������ �������� ������ �ش�.
	MyWeapons tempMyWeapons = MW;

	// üũ���� �����Ͽ� ������ ���´�.
//	ip.UpdateCheckSum();
			
//	DeleteID();

	char szTemp[11];
	strcpy(szTemp,GetName());

	// �Ҷ��������� �����Ѵ�.
	// ���� ����(����)�� �ƴ϶� �����̴�.
	// ��õ��Ұ� ������ �ִ� �����۵鵵 �Ҷ����������� ���޵ȴ�.
	SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
					100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

	CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

	// robypark 2004/6/14 14:35
	// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
	if (bSelectedCharacter == TRUE)
		SetSelect(bSelectedCharacter, FALSE);

	// ĳ���Ͱ� ����Ǿ����� ��ο��� �˸���.
	pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

	return TRUE;
}

// robypark 2004/6/15 18:13
// �Ҷ����������� ��� ��õ��ҷ� �����Ѵ�.
BOOL _Char::ChangeModeRoarOfLaughter_Immediately(void)
{
	// ��õ��� ����� ����Ϸ� �Ѵ�.
	// ��õ��ҷ� ��� �����Ѵ�.
	// ������ ���� �ִϸ��̼��� �Ҷ������� ��������Ʈ ���Ͽ� �����Ͽ�����,
	// A2�ִϸ��̼� �߰��� ��õ��� ��������Ʈ ���Ͽ� �����ϰ� �Ǿ���.
	UI16 uikind;

	// robypark 2004/6/14 14:35
	// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
	BOOL bSelectedCharacter = m_scSelectedSwitch;

	// ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
	if(m_scSelectedSwitch == TRUE)
	{
		SetSelect(FALSE, FALSE);
	}

	// ��õ��ҷ� �����Ѵ�.
	uikind = KIND_ON_ROAR_OF_LAUGHTER;

	_ImportantParameta ip;
	ZeroMemory(&ip, sizeof(_ImportantParameta));

	// ��õ����� �� �ɷ�ġ �� ���¸� �ӽ� �����Ϳ� ����
	ip = m_ParmeterBox.IP;

	// robypark 2004/6/14 11:03
	// ������ �������� ������ �ش�.
	MyWeapons tempMyWeapons = MW;

	// üũ���� �����Ͽ� ������ ���´�.
//	ip.UpdateCheckSum();
			
//	DeleteID();

	char szTemp[11];
	strcpy(szTemp,GetName());

	// ��õ��Ҹ� �����Ѵ�.
	// ���� ����(����)�� �ƴ϶� �����̴�.
	// �Ҷ��������� ������ �ִ� �����۵鵵 ��õ��ҿ��� ���޵ȴ�.
	SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
					100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

	CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

	// robypark 2004/6/14 14:35
	// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
	if (bSelectedCharacter == TRUE)
		SetSelect(bSelectedCharacter, FALSE);

	// ĳ���Ͱ� ����Ǿ����� ��ο��� �˸���.
	pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

	return TRUE;
}
