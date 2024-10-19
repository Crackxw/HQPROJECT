//--------------------------------------------------------------------
//  설명: 불랑기포차 앙천대소 기술을 사용할 때 일반 모드와 시즈 모드 변신을
//		 처리하기위한 함수를 가지고 있다.
//
//  생성일: 2004.6.4 13:30,	robypark
//  최근수정일: 2004.6.4 13:30
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
// 지도의 각 좌표마다의 중심값, 
extern int MapCenterX[][MAX_MAP_YSIZE];
extern int MapCenterY[][MAX_MAP_YSIZE];
extern _KindInfo KI[];
extern _NewID ZeroID;
extern _clGame *pGame;
extern	_Text 				Text;								// 텍스트 
extern _Char   CR[MAX_CHAR_NUMBER];       // 캐릭터의 포인터 

// robypark 2004/6/4 13:33
// 불랑기포차 변신을 위한 함수
// 앙천대소 시즈모드 => 일반 불랑기포차
BOOL _Char::ChangeModeFarangi(void)
{
	SetAnimation(ANI_MAGIC, &m_siMagicAniStep);

	// 애니메이션 중 다른 명령 수행 및 입력 막기
	m_scBusySwitch=TRUE;

	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{
		// 애니메이션이 다 끝나면, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber() - 1) )
		{
			// robypark 2004/6/14 14:35
			// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
			BOOL bSelectedCharacter = m_scSelectedSwitch;

			// 선택 캐릭터 목록에서 자신을 삭제한다. 
			if(m_scSelectedSwitch == TRUE)
			{
				SetSelect(FALSE, FALSE);
			}

			// 앙천대소 기술을 사용하여 현재 시즈모드이다.
			// 일반 불랑기포차로 변신한다.
			UI16 uikind;

			// 불랑기포차로 변신한다.
			uikind = KIND_ON_FARANGI_VEHICLE;

			_ImportantParameta ip;
			ZeroMemory(&ip, sizeof(_ImportantParameta));

			// 앙천대소일 때 능력치 및 상태를 임시 데이터에 저장
			ip = m_ParmeterBox.IP;

			// robypark 2004/6/14 11:03
			// 착용한 아이템을 전달해 준다.
			MyWeapons tempMyWeapons = MW;

			// 체크섬을 재계산하여 오류를 막는다.
//			ip.UpdateCheckSum();
			
//			DeleteID();

			char szTemp[11];
			strcpy(szTemp,GetName());

			// 불랑기포차로 변신한다. 유닛 생산(생성)이 아니라 변신이다.
			// 앙천대소가 가지고 있던 아이템들도 불랑기포차에게 전달된다.
			// 무기도 포함해서
			SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
						100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

			CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

			// robypark 2004/6/14 14:35
			// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
			if (bSelectedCharacter == TRUE)
				SetSelect(bSelectedCharacter, FALSE);

			// 캐릭터가 변경되었음을 모두에게 알린다.
			pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}

	return TRUE;
}

// robypark 2004/6/4 13:33
// 불랑기포차 변신을 위한 함수
// 일반 불랑기포차 => 앙천대소 시즈모드
BOOL _Char::ChangeModeRoarOfLaughter(void)
{
	if (m_siMagicAniStep == 0)
	{
		// 마법력을 줄여준다. 
		if( m_ParmeterBox.IP.GetMana() >= MAGICPOWER_SIEGE_MODE )
		{
			// 마법력을 줄여준다. 
			ReduceMana(MAGICPOWER_SIEGE_MODE);

			// robypark 2004/6/15 17:59
			// 불랑기포차에서 앙천대소로 변신하는 애니메이션이 앙천대소 스프라이트 파일에 존재
			// 하게 변경되었다.
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
		// 애니메이션이 다 끝나면, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Magic2CharAnimationIndex].GetFrameNumber() - 1) )
		{
			// robypark 2004/6/14 14:35
			// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
			BOOL bSelectedCharacter = m_scSelectedSwitch;

			// 선택 캐릭터 목록에서 자신을 삭제한다. 
			if(m_scSelectedSwitch == TRUE)
			{
				SetSelect(FALSE, FALSE);
			}

			// 불랑기포차가 앙천대소 기술을 사용하여 시즈모드로 변신하려 한다.
			// 시즈모드인 앙천대소로 변신한다.
			UI16 uikind;

			// 앙천대소로 변신
			uikind = KIND_ON_ROAR_OF_LAUGHTER;

			_ImportantParameta ip;
			ZeroMemory(&ip, sizeof(_ImportantParameta));

			// 불랑기포차일 때 능력치 및 상태를 앙천대소를 위해 임시로 저장한다.
			ip = m_ParmeterBox.IP;

			// robypark 2004/6/14 11:03
			// 착용한 아이템을 전달해 준다.
			MyWeapons tempMyWeapons = MW;

			// 오류발생을 막기위해 체크섬을 재계산한다.
//			ip.UpdateCheckSum();
				
			char szTemp[11];
			strcpy(szTemp,GetName());

			// 앙천대소 캐릭터를 생성한다.
			// 유닛 생성(생산)이 아니라 변신이다.
			// 불랑기포차가 가지고 있던 아이템들도 앙천대소에게 전달된다.
			// robypark 2004/6/15 10:49
			// 앙천대소인 상태에서 무기가 변경되는 것을 막기위해 
			// MW.GetWearItem(ON_WEAR_WEAPON_ITEM) 인자를 추가하였다.
			SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
						100, GetPlayerNumber(), &(ip), &tempMyWeapons, 
						m_siOnlineID, CharOrderNumber);//, tempMyWeapons.GetWearItem(ON_WEAR_WEAPON_ITEM));
			// tempMyWeapons.GetWearItem(ON_WEAR_WEAPON_ITEM) 이 부분을 추가할 경우 앙천대소 변신 후에 본 캐릭터가
			// 조종이 안되는 문제가 생긴다.

			CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

			// robypark 2004/6/14 14:35
			// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
			if (bSelectedCharacter == TRUE)
				SetSelect(bSelectedCharacter, FALSE);

			// 캐릭터가 변경되었음을 모두에게 알린다.
			pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}

	return TRUE;
}

// robypark 2004/6/10 11:14
// 앙천대소에서 즉시 불랑기포차로 변신한다.
BOOL _Char::ChangeModeFarangi_Immediately(void)
{
	// 앙천대소 기술을 사용하여 현재 시즈모드이다.
	// 일반 불랑기포차로 변신한다.
	UI16 uikind;

	// robypark 2004/6/14 14:35
	// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
	BOOL bSelectedCharacter = m_bAlreadySelectedBeforeDead;

	// 선택 캐릭터 목록에서 자신을 삭제한다. 
	if(m_scSelectedSwitch == TRUE)
	{
		SetSelect(FALSE, FALSE);
	}

	// 불랑기포차로 변신한다.
	uikind = KIND_ON_FARANGI_VEHICLE;

	_ImportantParameta ip;
	ZeroMemory(&ip, sizeof(_ImportantParameta));

	// 앙천대소일 때 능력치 및 상태를 임시 데이터에 저장
	ip = m_ParmeterBox.IP;

	// robypark 2004/6/14 11:03
	// 착용한 아이템을 전달해 준다.
	MyWeapons tempMyWeapons = MW;

	// 체크섬을 재계산하여 오류를 막는다.
//	ip.UpdateCheckSum();
			
//	DeleteID();

	char szTemp[11];
	strcpy(szTemp,GetName());

	// 불랑기포차를 생성한다.
	// 유닛 생성(생산)이 아니라 변신이다.
	// 앙천대소가 가지고 있던 아이템들도 불랑기포차에게 전달된다.
	SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
					100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

	CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

	// robypark 2004/6/14 14:35
	// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
	if (bSelectedCharacter == TRUE)
		SetSelect(bSelectedCharacter, FALSE);

	// 캐릭터가 변경되었음을 모두에게 알린다.
	pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

	return TRUE;
}

// robypark 2004/6/15 18:13
// 불랑기포차에서 즉시 앙천대소로 변신한다.
BOOL _Char::ChangeModeRoarOfLaughter_Immediately(void)
{
	// 앙천대소 기술을 사용하려 한다.
	// 앙천대소로 즉시 변신한다.
	// 기존에 변신 애니메이션이 불랑기포차 스프라이트 파일에 존재하였으나,
	// A2애니메이션 추가로 앙천대소 스프라이트 파일에 존재하게 되었다.
	UI16 uikind;

	// robypark 2004/6/14 14:35
	// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
	BOOL bSelectedCharacter = m_scSelectedSwitch;

	// 선택 캐릭터 목록에서 자신을 삭제한다. 
	if(m_scSelectedSwitch == TRUE)
	{
		SetSelect(FALSE, FALSE);
	}

	// 앙천대소로 변신한다.
	uikind = KIND_ON_ROAR_OF_LAUGHTER;

	_ImportantParameta ip;
	ZeroMemory(&ip, sizeof(_ImportantParameta));

	// 앙천대소일 때 능력치 및 상태를 임시 데이터에 저장
	ip = m_ParmeterBox.IP;

	// robypark 2004/6/14 11:03
	// 착용한 아이템을 전달해 준다.
	MyWeapons tempMyWeapons = MW;

	// 체크섬을 재계산하여 오류를 막는다.
//	ip.UpdateCheckSum();
			
//	DeleteID();

	char szTemp[11];
	strcpy(szTemp,GetName());

	// 앙천대소를 생성한다.
	// 유닛 생성(생산)이 아니라 변신이다.
	// 불랑기포차가 가지고 있던 아이템들도 앙천대소에게 전달된다.
	SetChar(uikind, m_NewID, GetX(), GetY(), SOUTH, 
					100, GetPlayerNumber(), &(ip), &tempMyWeapons, m_siOnlineID, CharOrderNumber);

	CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

	// robypark 2004/6/14 14:35
	// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
	if (bSelectedCharacter == TRUE)
		SetSelect(bSelectedCharacter, FALSE);

	// 캐릭터가 변경되었음을 모두에게 알린다.
	pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);

	return TRUE;
}
