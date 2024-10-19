//--------------------------------------------------------------------
//  담당 : 김태곤
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

/////////////////////////////////////////////////////////////////////
////  죽음 관련 
////////////////////////////////////////////////////////////////////
BOOL _Char::DyingOperation()
{
	return DyingOperationNormal();
}


BOOL _Char::DyingOperationNormal()
{
	if(DyingOperationSwitch==TRUE)return FALSE;
	DyingOperationSwitch=TRUE;


	// 구미호이면 특수효과를 넣는다. 
	if(m_cltCharStatus.GetKind() == KIND_ON_FOX)
	{
		SI32 smoke_id=FindEmptySmoke();
		if(smoke_id)
		{
			SetSmoke(KIND_SMOKE_CLONDIE, smoke_id, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AREA_LAND);
		}
	}

	// 캐릭터가 죽었다는 사실을 알려준다.
	
	if(m_cltCharStatus.GetPlayerNumber() == UserPlayerNumber)							// 사용자의 유닛이 죽은 경우이거나
	{
		
		// 주인공 캐릭터가 죽은 경우에는,, (자금을 잃는다.- 전체 소지금의 5%)
		if(m_siOnlineID == 0)
		{
			// 해킹 우려가 없을때에만 전송한다. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID);
			}

			// 경험치를 낮춘다. (Level * 10)
			if(m_ParmeterBox.IP.GetCurExp() >= GetExpReduceForDying())
				m_ParmeterBox.IP.SetCurExp(m_ParmeterBox.IP.GetCurExp() - GetExpReduceForDying() );
			else
			{
				m_ParmeterBox.IP.SetCurExp(0);
			}

			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				// 해킹 우려가 없을때에만 전송한다. (무도장이 아닌 경우만..)
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
					OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, -GetExpReduceForDying());
				}
				else
				{
					SetMessage("해킹의 가능성이 있는 데이터입니다.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}

				// 분신이 아니어야 쓰러졌다는 메시지를 표시한다. 
				if(HostID.IsEmpty() == TRUE)
				{
					
					char buffer[128];
					wsprintf(buffer, Text.LostExpMessage.Get(), GetName(), GetExpReduceForDying());
					SetMessage(buffer);
					
					sprintf(buffer, Text.LostMoneyMessage.Get(), OBInfo.UserMoney * PENALTY_MONEY_OF_DEADTH / 100);
					SetMessage(buffer, -1, 0, RED_TEXT_COLOR);
				}
			}
			

		}
		else 
		{
			// 경험치를 낮춘다. (Level * 10)
			if(m_ParmeterBox.IP.GetCurExp() >= GetExpReduceForDying())
				m_ParmeterBox.IP.SetCurExp(m_ParmeterBox.IP.GetCurExp() - GetExpReduceForDying());
			else
			{
				m_ParmeterBox.IP.SetCurExp(0);
			}

			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{			
				// 해킹 우려가 없을때에만 전송한다. 
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
					OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, -GetExpReduceForDying());
				}
				else
				{
					SetMessage("해킹의 가능성이 있는 데이터입니다.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}

				// 분신이 아니어야 쓰러졌다는 메시지를 표시한다. 
				if(HostID.IsEmpty() == TRUE)
				{
					char buffer[128];
					wsprintf(buffer, Text.LostExpMessage.Get(), GetName(), GetExpReduceForDying());//(m_ParmeterBox.IP.GetLevel() * 10));
					SetMessage(buffer);
				}
			}

		}
	}
	else if(ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)			// NPC의 유닛이 죽은 경우는, 
	{
		// 해킹 우려가 없을때에만 전송한다. 
		if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
		{
			OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID);
		}
	}
	

	// 배의 가라앉음 이면, 
	if(IsDyingAtb(DYINGATB_SHIP)==TRUE)
	{
		PushEffect(EFFECT_SINK, m_ssX, m_ssY);
	}
	// 건물은 폭파음을 낸다. 
	else if(IsDyingAtb(DYINGATB_BUILDING)==TRUE)
	{
		PushEffect(EFFECT_EXP, m_ssX, m_ssY);
	}
	// 폭발하는 것이라면, 
	else if(IsDyingAtb(DYINGATB_EXP)==TRUE)
	{
		SHORT bulletid=FindEmptySmoke();
		if(bulletid)
		{
			PushEffect(EFFECT_AIRPLANEDIE, m_ssX, m_ssY);
			SetSmoke(KIND_SMOKE_EXP0, bulletid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), m_cltCharStatus.GetArea());
		}
	}
	else if(KI[m_cltCharStatus.GetKind()].m_siDyingEffect)
	{
#ifndef _USE_HASHCODE_EFFECTSOUND		
		PushEffect(KI[m_cltCharStatus.GetKind()].m_siDyingEffect, m_ssX, m_ssY);
#else
		HashCodePushEffect(KI[m_cltCharStatus.GetKind()].m_siDyingEffect, m_ssX, m_ssY);			
#endif
	}
		

	return TRUE;
}





SI16 _Char::Dying()
{
	SHORT i, j;
	SHORT smokeid;

	// 다른 명령은 받아 들이지 않는다. 
	m_scBusySwitch=TRUE;

	//if(siCharType != 3)
	{
		// 죽을 때 취하는 조치 
		DyingOperation();

		// 죽기전에 선택 캐릭터 목록에서 자신을 삭제한다. 
		if(m_scSelectedSwitch==TRUE)
		{
			SetSelect(FALSE, FALSE);
		}

		// 부대에서 제거한다. 
		GD.Player[GetPlayerNumber()].Team.DeleteTeam(m_NewID);
		//GD.Team.DeleteTeam(NewID,GetPlayerNumber());

		// 건물의 폭파면 지체하지 않고 죽으며 
		// 화염을 여러군데 터뜨린다. 
		if(IsDyingAtb(DYINGATB_BUILDING)==TRUE)
		{
			SHORT fragmentid;
			SHORT centerx, centery;

			smokeid=FindEmptySmoke();
			if(smokeid)
			{
				centerx=MapCenterX[m_ssX][m_ssY];
				centery=MapCenterY[m_ssX][m_ssY]-(ImageYsize*40/100);
				SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, centerx,centery, AREA_LAND);

			}

    		for(i=0;i<m_scIDYsize;i++)
				for(j=0;j<m_scIDXsize;j++)
				{
					// 파편을 만들어 보낸다. 
					fragmentid=FindEmptyBullet();
					if(fragmentid)
					{
						SetBullet(KIND_BULLET_FRAGMENT1, fragmentid, m_ssX, m_ssY, m_ssX, m_ssY, 
						m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
						150-GD.Random()%300+m_cltCharStatus.GetCenterX(),
						150-GD.Random()%300+m_cltCharStatus.GetCenterY(),
						m_NewID, ZeroID, 0, 0, 0, AttackArea);
					}
				}

			return CHAR_DEAD;
		}
		// 선박의 침몰이면, 
		else if(IsDyingAtb(DYINGATB_SHIP)==TRUE)
		{
			smokeid=FindEmptySmoke();
			if(smokeid)
			{
			   SetSmoke(KIND_SMOKE_SINK, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), m_cltCharStatus.GetArea());
			}

			return CHAR_DEAD;
		}
		// 폭발해서 사라지는 속성이면 , 지체하지 않고 죽는다. 
		if(IsDyingAtb(DYINGATB_EXP)==TRUE)
		{
			smokeid=FindEmptySmoke();
			if(smokeid)
			{
				if(GD.Random()%2)
				  SetSmoke(KIND_SMOKE_EXP1, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), m_cltCharStatus.GetArea());
				else
				  SetSmoke(KIND_SMOKE_EXP2, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), m_cltCharStatus.GetArea());
			}

			return CHAR_DEAD;
		}
	}

	SetAnimation(ANI_DYING, &DyingStep);
	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{

		// 애니메이션이 다 끝나면, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[DyingCharAnimationIndex].GetFrameNumber()-1) )
		{
			// 잔해가 남지 않는 속성이면, 지체하지 않고 죽는다. 
			if(IsDyingAtb(DYINGATB_IMMEDIATE)==TRUE)
			{
				return CHAR_DEAD;
			}
			// 사망후 지체하는 속성이면 지체한 후에 죽는다. 
			else
			{
				DeadDelayor++;           // 쓰러진 다음 잠시 대기 후에 사라진다. 
				if(DeadDelayor>DeadDelay)  
				{
					return CHAR_DEAD;          // DyingDelay*WorkDelay 프레임 대기한다. 
				}
			}
		}
	}

    return CHAR_DYING;
}

BOOL _Char::Dead()
{

	// 죽기전에 선택 캐릭터 목록에서 자신을 삭제한다. 
	if(m_scSelectedSwitch==TRUE)
	{
	    SetSelect(FALSE, FALSE);
	}

  return TRUE;
}

// 사망시 잃게 되는 경험치를 구하는 함수. (레벨에 따라 차등적으로 경험치를 잃게 된다.)
SI32 _Char::GetExpReduceForDying()
{
	return ::GetExpReduceForDying(m_ParmeterBox.IP.GetLevel());
}

BOOL _Char::GeneralExDying()
{
	SetAnimation(ANI_DYING, &DyingStep);
	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{
		// 애니메이션이 다 끝나면, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[DyingCharAnimationIndex].GetFrameNumber()-1) )
		{
			GeneralHeader* pGeneralHeader = pGame->pOnlineWorld->pGeneralParserEx->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

			if(pGeneralHeader != NULL)
			{	
				// robypark 2004/6/14 14:35
				// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
				BOOL bSelectedCharacter = m_bAlreadySelectedBeforeDead;

				//kind를 기억한다.
				_ImportantParameta ip;
				ZeroMemory(&ip, sizeof(_ImportantParameta));

				UI16 uikind;

				uikind = TransKindOnlineToImjin(pGeneralHeader->siGeneralBaseCharacterCode);
				
				// robypark 2004/6/4 19:46
				// 1차 장수에 2차장수의 능력치 및 상태를 저장
				ip = m_ParmeterBox.IP;

				// 체력과 마법력을 2차 장수의 10%만 가지고 1차 장수가 등장해야 한다.
				ip.SetStr(  m_ParmeterBox.IP.GetStr());
				ip.SetDex(  m_ParmeterBox.IP.GetDex());
				ip.SetVit(  m_ParmeterBox.IP.GetVit()/10);
				ip.SetInt(  m_ParmeterBox.IP.GetInt()/10);
				ip.SetAC(   m_ParmeterBox.IP.GetAC());
				ip.SetLife( m_ParmeterBox.IP.GetMaxLifeByIP()/10);
				ip.SetMana( m_ParmeterBox.IP.GetMaxManaByIP()/10);
				ip.SetLevel(m_ParmeterBox.IP.GetLevel());
				//ip.SetVit( m_ParmeterBox.IP.GetVit()/10);
		
				// robypark 2004/6/14 11:03
				// 착용한 아이템을 전달해 준다.
				MyWeapons tempMyWeapons = MW;

				// robypark 2004/6/4 18:52
				// 오류발생을 막기위해 체크섬을 재계산한다.
//				ip.UpdateCheckSum();

				char szTemp[11];
				strcpy(szTemp,GetName());

				// 장수 2차 전직 전 장수캐릭터를 생성한다.
				// 생산이 아니라 변신이다.
				// 2차 장수가 가지고 있던 아이템들도 1차 장수에게 전달된다.
				// 무기도 포함해서
				SetChar(uikind, m_NewID, GetX(), GetY(), Direction, 
					100, GetPlayerNumber(), &ip, &tempMyWeapons, m_siOnlineID, CharOrderNumber);

				CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

				// robypark 2004/6/14 14:35
				// 사용자가 선택한 캐릭터라면 변신 후에도 선택된 상태가 되도록 수정
				if (bSelectedCharacter == TRUE)
					SetSelect(bSelectedCharacter, FALSE);

				// 캐릭터가 변경되었음을 모두에게 알린다.
				pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);
			}

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}
	return TRUE;
}