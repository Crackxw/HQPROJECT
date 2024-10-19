//--------------------------------------------------------------------
//  ��� : ���°�
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
// ������ �� ��ǥ������ �߽ɰ�, 
extern int MapCenterX[][MAX_MAP_YSIZE];
extern int MapCenterY[][MAX_MAP_YSIZE];
extern _KindInfo KI[];
extern _NewID ZeroID;
extern _clGame *pGame;
extern	_Text 				Text;								// �ؽ�Ʈ 

extern _Char   CR[MAX_CHAR_NUMBER];       // ĳ������ ������ 

/////////////////////////////////////////////////////////////////////
////  ���� ���� 
////////////////////////////////////////////////////////////////////
BOOL _Char::DyingOperation()
{
	return DyingOperationNormal();
}


BOOL _Char::DyingOperationNormal()
{
	if(DyingOperationSwitch==TRUE)return FALSE;
	DyingOperationSwitch=TRUE;


	// ����ȣ�̸� Ư��ȿ���� �ִ´�. 
	if(m_cltCharStatus.GetKind() == KIND_ON_FOX)
	{
		SI32 smoke_id=FindEmptySmoke();
		if(smoke_id)
		{
			SetSmoke(KIND_SMOKE_CLONDIE, smoke_id, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AREA_LAND);
		}
	}

	// ĳ���Ͱ� �׾��ٴ� ����� �˷��ش�.
	
	if(m_cltCharStatus.GetPlayerNumber() == UserPlayerNumber)							// ������� ������ ���� ����̰ų�
	{
		
		// ���ΰ� ĳ���Ͱ� ���� ��쿡��,, (�ڱ��� �Ҵ´�.- ��ü �������� 5%)
		if(m_siOnlineID == 0)
		{
			// ��ŷ ����� ���������� �����Ѵ�. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID);
			}

			// ����ġ�� �����. (Level * 10)
			if(m_ParmeterBox.IP.GetCurExp() >= GetExpReduceForDying())
				m_ParmeterBox.IP.SetCurExp(m_ParmeterBox.IP.GetCurExp() - GetExpReduceForDying() );
			else
			{
				m_ParmeterBox.IP.SetCurExp(0);
			}

			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				// ��ŷ ����� ���������� �����Ѵ�. (�������� �ƴ� ��츸..)
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
					OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, -GetExpReduceForDying());
				}
				else
				{
					SetMessage("��ŷ�� ���ɼ��� �ִ� �������Դϴ�.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}

				// �н��� �ƴϾ�� �������ٴ� �޽����� ǥ���Ѵ�. 
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
			// ����ġ�� �����. (Level * 10)
			if(m_ParmeterBox.IP.GetCurExp() >= GetExpReduceForDying())
				m_ParmeterBox.IP.SetCurExp(m_ParmeterBox.IP.GetCurExp() - GetExpReduceForDying());
			else
			{
				m_ParmeterBox.IP.SetCurExp(0);
			}

			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{			
				// ��ŷ ����� ���������� �����Ѵ�. 
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
					OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, -GetExpReduceForDying());
				}
				else
				{
					SetMessage("��ŷ�� ���ɼ��� �ִ� �������Դϴ�.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}

				// �н��� �ƴϾ�� �������ٴ� �޽����� ǥ���Ѵ�. 
				if(HostID.IsEmpty() == TRUE)
				{
					char buffer[128];
					wsprintf(buffer, Text.LostExpMessage.Get(), GetName(), GetExpReduceForDying());//(m_ParmeterBox.IP.GetLevel() * 10));
					SetMessage(buffer);
				}
			}

		}
	}
	else if(ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)			// NPC�� ������ ���� ����, 
	{
		// ��ŷ ����� ���������� �����Ѵ�. 
		if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
		{
			OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID);
		}
	}
	

	// ���� ������� �̸�, 
	if(IsDyingAtb(DYINGATB_SHIP)==TRUE)
	{
		PushEffect(EFFECT_SINK, m_ssX, m_ssY);
	}
	// �ǹ��� �������� ����. 
	else if(IsDyingAtb(DYINGATB_BUILDING)==TRUE)
	{
		PushEffect(EFFECT_EXP, m_ssX, m_ssY);
	}
	// �����ϴ� ���̶��, 
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

	// �ٸ� ����� �޾� ������ �ʴ´�. 
	m_scBusySwitch=TRUE;

	//if(siCharType != 3)
	{
		// ���� �� ���ϴ� ��ġ 
		DyingOperation();

		// �ױ����� ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
		if(m_scSelectedSwitch==TRUE)
		{
			SetSelect(FALSE, FALSE);
		}

		// �δ뿡�� �����Ѵ�. 
		GD.Player[GetPlayerNumber()].Team.DeleteTeam(m_NewID);
		//GD.Team.DeleteTeam(NewID,GetPlayerNumber());

		// �ǹ��� ���ĸ� ��ü���� �ʰ� ������ 
		// ȭ���� �������� �Ͷ߸���. 
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
					// ������ ����� ������. 
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
		// ������ ħ���̸�, 
		else if(IsDyingAtb(DYINGATB_SHIP)==TRUE)
		{
			smokeid=FindEmptySmoke();
			if(smokeid)
			{
			   SetSmoke(KIND_SMOKE_SINK, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), m_cltCharStatus.GetArea());
			}

			return CHAR_DEAD;
		}
		// �����ؼ� ������� �Ӽ��̸� , ��ü���� �ʰ� �״´�. 
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

		// �ִϸ��̼��� �� ������, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[DyingCharAnimationIndex].GetFrameNumber()-1) )
		{
			// ���ذ� ���� �ʴ� �Ӽ��̸�, ��ü���� �ʰ� �״´�. 
			if(IsDyingAtb(DYINGATB_IMMEDIATE)==TRUE)
			{
				return CHAR_DEAD;
			}
			// ����� ��ü�ϴ� �Ӽ��̸� ��ü�� �Ŀ� �״´�. 
			else
			{
				DeadDelayor++;           // ������ ���� ��� ��� �Ŀ� �������. 
				if(DeadDelayor>DeadDelay)  
				{
					return CHAR_DEAD;          // DyingDelay*WorkDelay ������ ����Ѵ�. 
				}
			}
		}
	}

    return CHAR_DYING;
}

BOOL _Char::Dead()
{

	// �ױ����� ���� ĳ���� ��Ͽ��� �ڽ��� �����Ѵ�. 
	if(m_scSelectedSwitch==TRUE)
	{
	    SetSelect(FALSE, FALSE);
	}

  return TRUE;
}

// ����� �Ұ� �Ǵ� ����ġ�� ���ϴ� �Լ�. (������ ���� ���������� ����ġ�� �Ұ� �ȴ�.)
SI32 _Char::GetExpReduceForDying()
{
	return ::GetExpReduceForDying(m_ParmeterBox.IP.GetLevel());
}

BOOL _Char::GeneralExDying()
{
	SetAnimation(ANI_DYING, &DyingStep);
	if(DoAnimationDelay(TRUE, ANI_ATB_ONCE) == TRUE)
	{
		// �ִϸ��̼��� �� ������, 
		if(m_cltCharDrawInfo.GetAniStep() >= (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[DyingCharAnimationIndex].GetFrameNumber()-1) )
		{
			GeneralHeader* pGeneralHeader = pGame->pOnlineWorld->pGeneralParserEx->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

			if(pGeneralHeader != NULL)
			{	
				// robypark 2004/6/14 14:35
				// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
				BOOL bSelectedCharacter = m_bAlreadySelectedBeforeDead;

				//kind�� ����Ѵ�.
				_ImportantParameta ip;
				ZeroMemory(&ip, sizeof(_ImportantParameta));

				UI16 uikind;

				uikind = TransKindOnlineToImjin(pGeneralHeader->siGeneralBaseCharacterCode);
				
				// robypark 2004/6/4 19:46
				// 1�� ����� 2������� �ɷ�ġ �� ���¸� ����
				ip = m_ParmeterBox.IP;

				// ü�°� �������� 2�� ����� 10%�� ������ 1�� ����� �����ؾ� �Ѵ�.
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
				// ������ �������� ������ �ش�.
				MyWeapons tempMyWeapons = MW;

				// robypark 2004/6/4 18:52
				// �����߻��� �������� üũ���� �����Ѵ�.
//				ip.UpdateCheckSum();

				char szTemp[11];
				strcpy(szTemp,GetName());

				// ��� 2�� ���� �� ���ĳ���͸� �����Ѵ�.
				// ������ �ƴ϶� �����̴�.
				// 2�� ����� ������ �ִ� �����۵鵵 1�� ������� ���޵ȴ�.
				// ���⵵ �����ؼ�
				SetChar(uikind, m_NewID, GetX(), GetY(), Direction, 
					100, GetPlayerNumber(), &ip, &tempMyWeapons, m_siOnlineID, CharOrderNumber);

				CR[m_NewID.ID].SetName(szTemp,strlen(szTemp));

				// robypark 2004/6/14 14:35
				// ����ڰ� ������ ĳ���Ͷ�� ���� �Ŀ��� ���õ� ���°� �ǵ��� ����
				if (bSelectedCharacter == TRUE)
					SetSelect(bSelectedCharacter, FALSE);

				// ĳ���Ͱ� ����Ǿ����� ��ο��� �˸���.
				pGame->pBattle->PushUpdateCharInfo(m_NewID.ID);
			}

			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
	}
	return TRUE;
}