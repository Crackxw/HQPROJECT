#include <GSL.h>

#include <main.h>
#include <setchar.h>
#include "maindata.h"
#include "music.h"
//#include <OnlineWorld.h>
#include <KindInfo.h>
#include <CharOrg.h>
#include <clGame.h>
#include <OnlineImjin2i.h>
#include <findpath5.h>

#include <map.h>
#include <Etc.h>
#include <Smoke.h>

#include <Magic.h>
#include <charinterface.h>
#include "calcattack.h"

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Char				CR[];								// ĳ������ ������ 
extern	_Map 				Map;
extern  SI32	UserPlayerNumber;
extern  _clGame* pGame;
extern  OnlineBattleInfo OBInfo;
extern	cltCharInterface clifChar;




//��ȣ�� ĳ���� ��ȯ.
BOOL _Char::ReCallGuardianChar()
{

	SHORT		i, j;
	SHORT		id;
	SHORT		count = 0;				// ��ȯ�� ������ ��
	SI32		targetKind;			// ��ȯ�� ������ ����
	SI32		x, y; 

	targetKind = TransKindOnlineToImjin(m_uiGuardianRecallKind);

	if(m_siGuardianKind == ON_GUARDIAN_MOUSE)
	{
		PushEffect(EFFECT_GUARDIAN_MOUSE_RECALL, 0, 0);
	}
	else if(m_siGuardianKind == ON_GUARDIAN_SNAKE)
	{
		PushEffect(EFFECT_GUARDIAN_SNAKE_RECALL, 0, 0);
	}
	else if(m_siGuardianKind == ON_GUARDIAN_MONKEY)
	{
		PushEffect(EFFECT_GUARDIAN_MONKEY_RECALL, 0, 0);
	}
	else if(m_siGuardianKind == ON_GUARDIAN_CHICKEN)
	{
		PushEffect(EFFECT_GUARDIAN_CHICK_RECALL, 0, 0);
	}
	else if(m_siGuardianKind == ON_GUARDIAN_PIG)
	{
		PushEffect(EFFECT_GUARDIAN_PIG_RECALL, 0, 0);
	}



	// �н��� ��������� ���� ��ġ. 
	x = m_ssX;
	y = m_ssY;

	// �ֺ��� ����ǰ�� �����. 
	for(i=-3;i<=3;i++)
	{
		for(j=-3;j<=3;j++)
		{
			if(i==0 && j==0)continue;

			if(Map.IsInMapArea(x+j, y+i)==TRUE)
			{
				id = FindEmptyID(m_cltCharStatus.GetPlayerNumber());
				if(id)
				{ 							
						// ĳ���͸� ���� �� �ִ� ������ �ɴ´�. 
					if(Map.IsEmpty(x+j, y+i)==TRUE)
					{
						if(count < m_siGuardianRecallNum)
						{												
							_ImportantParameta ip;   //ĳ���� ����.
							ZeroMemory(&ip, sizeof(_ImportantParameta));

							UI16 siKind;
							siKind = TransKindImjinToOnline(targetKind);

							ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetStr());
							ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetDex());
							ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetVit());
							//ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetInt());
							ip.SetInt(125);
							ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetLife());

							ip.SetMana(500);
							//ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetMana());
							ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetAC());
							ip.SetLevel(1);
							{
								// ĳ���͸� �ɴ´�. 
								SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);

								count++;
								
								//������ �Ǵ� ���̵� ����Ѵ�.
								CR[id].HostID = m_NewID;
								//�� ĳ���ʹ� ��ȯ���� ������� ĳ�����̴�.
								CR[id].m_bGuardianChar = TRUE;

								//��ȣ��[������] ��ȯ�� ������ ����� ����Ѵ�.
								if(m_siGuardianKind == ON_GUARDIAN_MONKEY)
								{
									_NewID targetid = CR[m_NewID.ID].m_GuardianAttackID;

									GetAtbSkill(id,targetid.ID);
								}
								//�ڵ����.
								clifChar.SetAutoSwitch(id,TRUE);

								// �нſ� ���� Ư��ȿ���� �ִ´�. 
								SI32 smoke_id=FindEmptySmoke();
								if(smoke_id)
								{
									SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
								}

								//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
								pGame->pBattle->PushUpdateCharInfo(id);
								Step = 0;
								
							}
						}//count;
					}   //if(Map.IsEmpty(x+j, y+i)==TRUE)						
				}	//if(id)
			}
		}	// for
	}	// for

	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);
	return TRUE;
}

BOOL _Char::GetAtbSkill(SI16 id,SI16 targetid)
{								
//	CR[id].m_uiAtb					 = CR[targetid].m_uiAtb;
	CR[id].m_uiAtb3					 = CR[targetid].m_uiAtb3;
	CR[id].m_uiAtb4					 = CR[targetid].m_uiAtb4;
	CR[id].m_uiAtb5					 = CR[targetid].m_uiAtb5;
	CR[id].m_siAttackRange1			 = CR[targetid].m_siAttackRange1;
	CR[id].m_siAttackRange2			 = CR[targetid].m_siAttackRange2;
	CR[id].m_siSearchRange		 = CR[targetid].m_siSearchRange;
//	CR[id].AttackAtb1			 = CR[targetid].AttackAtb1;
//	CR[id].AttackAtb2			 = CR[targetid].AttackAtb2;


	return TRUE;
}

BOOL _Char::DeleteRecallGuardianChar()
{
	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id))
		{
			if(IsFriendToByPlayerNumber(GetPlayerNumber(),CR[id].GetPlayerNumber()) == TRUE)
			{
				if(CR[id].m_bGuardianChar == TRUE)
				{
					if(m_siGuardianKind == ON_GUARDIAN_PIG)
					{
						SI16 siX,siY;

						siX = CR[id].m_cltCharStatus.GetCenterX();
						siY = CR[id].m_cltCharStatus.GetCenterY();

						SI32 smoke_id=FindEmptySmoke();
						if(smoke_id)
						{
							SetSmoke(KIND_SMOKE_GUARDIAN_CAW, smoke_id, siX, siY, AREA_LAND);
						}
					}
					DeleteChar(id);
				}
			}
		}
	}
	return TRUE;
}

BOOL _Char::RecallGuardianCaw()
{
	SHORT		i, j;
	SHORT		id;
	SHORT		count = 0;				// ��ȯ�� ������ ��
	_NewID		targetnewid;		// ��ȯ�� ������ newid
	SI32		targetKind;			// ��ȯ�� ������ ����
	SI32		x, y; 
	SI16		siCawChar;
	SI16		siTempX,siTempY;
	BOOL		bDeadFlag;
	
	m_scBusySwitch		= TRUE;
	siCawChar		= Para1;
	targetnewid		= Para2;
	bDeadFlag		= FALSE;

	switch(m_siCawNumber)
	{
		case 1:
		{
			targetKind = KIND_ON_GUARDIAN_CAW1;		
			PushEffect(EFFECT_GUARDIAN_CAW_RECALL, 0, 0);
		}
		break;
		
		case 2:
		{
			bDeadFlag = DeleteRecallGuardianCaw();
			targetKind = KIND_ON_GUARDIAN_CAW2;
			PushEffect(EFFECT_GUARDIAN_CAW_RECALL2, 0, 0);
		}
		break;
		case 3:
		{
			bDeadFlag = DeleteRecallGuardianCaw();   
			targetKind = KIND_ON_GUARDIAN_CAW3;
			PushEffect(EFFECT_GUARDIAN_CAW_RECALL2, 0, 0);
		}
		break;
		
	}

	// �н��� ��������� ���� ��ġ. 
	x = m_ssX;
	y = m_ssY;

	// �ֺ��� ����ǰ�� �����. 
	for(i=-3;i<=3;i++)
	{
		for(j=-3;j<=3;j++)
		{

			if(bDeadFlag == TRUE)
			{
				siTempX = m_siDeadCawX;
				siTempY = m_siDeadCawY;
			}
			else
			{
				siTempX = x + j;
				siTempY = y + i;
			}

			if(i==0 && j==0)continue;

			if(Map.IsInMapArea(siTempX, siTempY)==TRUE)
			{
				id = FindEmptyID(m_cltCharStatus.GetPlayerNumber());
				if(id)
				{ 							
						// ĳ���͸� ���� �� �ִ� ������ �ɴ´�. 
					if(Map.IsEmpty(siTempX, siTempY)==TRUE)
					{
						if(count < m_siGuardianRecallNum)
						{																												
							_ImportantParameta ip;   //ĳ���� ����.
							ZeroMemory(&ip, sizeof(_ImportantParameta));

							SI16 siKind;
							siKind = TransKindImjinToOnline(targetKind);

							ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetStr());
							ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetDex());
							ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetVit());
							ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetInt());
							ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetLife());
							ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetMana());
							ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetAC());
							ip.SetLevel(1);


							// ĳ���͸� �ɴ´�. 
							// ȥ���ϱ� ����̰ų�, 
							// ������Ʈ �ؾ� �� ĳ���Ͷ��, 
//								if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
							{
								// ĳ���͸� �ɴ´�. 
								SetChar(targetKind, id, siTempX, siTempY, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);

								count++;
								
								//������ �Ǵ� ���̵� ����Ѵ�.
								CR[id].HostID = m_NewID;
								//��n ĳ���ʹ� ��ȯ���� ������� ĳ�����̴�.
								CR[id].m_bGuardianChar = TRUE;
								
								//�ڵ����.
								clifChar.SetAutoSwitch(id,TRUE);

								// �нſ� ���� Ư��ȿ���� �ִ´�. 
								SI32 smoke_id=FindEmptySmoke();
								if(smoke_id)
								{
									SetSmoke(KIND_SMOKE_GUARDIAN_CAW, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
								}

								//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
								pGame->pBattle->PushUpdateCharInfo(id);
							}
						}//count;
					}   //if(Map.IsEmpty(x+j, y+i)==TRUE)						
				}	//if(id)
			}
		}	// for
	}	// for

	m_scBusySwitch=FALSE;
	OrderStop(m_NewID, BY_COMPUTER);
	return TRUE;		
}

BOOL _Char::DeleteRecallGuardianCaw()
{
	for(SI16 id = 0; id < MAX_CHAR_NUMBER ; id++)
	{
		if(IsAlive(id))
		{
			if(IsFriendToByPlayerNumber(GetPlayerNumber(),CR[id].GetPlayerNumber()) == TRUE)
			{
				if(CR[id].m_bGuardianChar == TRUE)
				{
					m_siDeadCawX = CR[id].GetX();
					m_siDeadCawY = CR[id].GetY();
					DeleteChar(id);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
