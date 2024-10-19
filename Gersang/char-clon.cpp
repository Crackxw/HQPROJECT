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

#include <map.h>
#include <Etc.h>
#include <Smoke.h>

#include <Magic.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Char				CR[];								// ĳ������ ������ 
extern	_Map 				Map;
extern SI32	UserPlayerNumber;
extern _clGame* pGame;
extern OnlineBattleInfo OBInfo;

BOOL _Char::CanClon(_NewID targetnewid)
{

	if( IsAtb3(ATB3_CLON) == TRUE )
	{
		// �������� ����ؾ� �Ѵ�. 
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_CLON)	return FALSE;
	}
	else if( IsAtb3(ATB3_ENEMYCLON) == TRUE )
	{
		// �������� ����ؾ� �Ѵ�. 
		if( m_ParmeterBox.IP.GetMana() < MAGICPOWER_ENEMYCLON )	return FALSE;


		// ����� ������ �־�� �Ѵ�. 
		if( IsAlive(targetnewid) == FALSE )		return FALSE;


		// ����� ���� �������̾�� �Ѵ�. 
		if( IsFriendToByPlayerNumber(GetPlayerNumber(), CR[targetnewid.ID].GetPlayerNumber()) == TRUE )	return FALSE;
	}
	else if( IsAtb3(ATB3_SEAHORSECLON) == TRUE )
	{
		// �������� ����ؾ� �Ѵ�. 
		if( m_ParmeterBox.IP.GetMana() < MAGICPOWER_CLON )	return FALSE;
	}
	else if( IsAtb3(ATB3_CLON2) == TRUE )
	{
		// �������� ����ؾ� �Ѵ�. 
		if( m_ParmeterBox.IP.GetMana() < MAGICPOWER_CLON )	return FALSE;
	}

	else
		return FALSE;

	
	return TRUE;
}


BOOL _Char::Clon()
{
	SHORT		i, j;
	SHORT		id;
	SHORT		count;				// ���� �н��� ��
	_NewID		targetnewid;		// ���� �н��� newid
	SI32		targetKind;			// ���� �н��� ����
	SI32		magicpower;			// �нż��� ����� �� �ʿ��� ������
	SI32		x, y; 
	SI32		aniindex;
	

	if(MagicCharAnimationIndex)
	{
		m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
		aniindex		= MagicCharAnimationIndex;
	}
	else
	{
		m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);
		aniindex		= AttackCharAnimationIndex;
	}
	
	m_scBusySwitch		= TRUE;

	targetnewid		= Para2;


	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if( m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].siDelay )	 
		return TRUE;
	m_cltCharStatus.SetWorkDelayor(0);
	
	if( KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].GetFrameNumber() == 0 )
	{
		Step=0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
		++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].GetFrameNumber();
		m_cltCharDrawInfo.SetAniStep(Step);
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}
	
	
	if( Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].siInstant )
	{
		if( CanClon(targetnewid) == FALSE )
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return TRUE;
		}

		// �н��� ��������� ���� ��ġ. 
		x = m_ssX;
		y = m_ssY;
		
		if( IsAtb3(ATB3_CLON) == TRUE )
		{
			magicpower		= MAGICPOWER_CLON;

			targetKind = m_cltCharStatus.GetKind();

			//�ѹ��� �ϳ��� �нŸ� ����� ����. 
			count =  1;	

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;
		}
		else if( IsAtb3(ATB3_ENEMYCLON) == TRUE )
		{
			magicpower		= MAGICPOWER_ENEMYCLON;

			// ������ ĳ������ ������ �˾ƿ´�.
			targetKind		= CR[targetnewid.ID].m_cltCharStatus.GetKind();

			count = 1;

			// �н��� ��������� ���� ��ġ. 
			x = CR[targetnewid.ID].GetX(); 
			y = CR[targetnewid.ID].GetY(); 

		}
		else if( IsAtb3(ATB3_SEAHORSECLON) == TRUE )
		{

			magicpower		= MAGICPOWER_CLON;

			// ������ ĳ������ ������ �˾ƿ´�.
			targetKind		= KIND_ON_SEAHORSE;

			count = 1;

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;

		}
		else if( IsAtb3(ATB3_CLON2) == TRUE )
		{

			magicpower		= MAGICPOWER_CLON;

			// ������ ĳ������ ������ �˾ƿ´�.
			if(GD.Random() % 2)
				targetKind		= KIND_ON_HUNGRYDEMON;
			else
				targetKind		= KIND_ON_EATERDEMON;

			count = 1;

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;

		}

		else
		{
			clGrp.Error("f0262_Clon", "kind_data:%d", m_cltCharStatus.GetKind());
		}

		
		// �ֺ��� ����ǰ�� �����. 
		for(i=-2;i<=2;i++)
		{
			for(j=-2;j<=2;j++)
			{
				if(i==0 && j==0)continue;
				
				if(Map.IsInMapArea(x+j, y+i)==TRUE)
				{
					
					id = FindEmptyID(m_cltCharStatus.GetPlayerNumber());
					if(id)
					{ 
						
						if( m_ParmeterBox.IP.GetMana() >= magicpower )
						{
							
							// ĳ���͸� ���� �� �ִ� ������ �ɴ´�. 
							if(Map.IsEmpty(x+j, y+i)==TRUE)
							{
								
								if(count == 1)
								{
									count = 0;
									PushEffect(EFFECT_CLON, x, y);
															
									ReduceMana(magicpower);
														
									_ImportantParameta ip;
									ZeroMemory(&ip, sizeof(_ImportantParameta));

									if( IsAtb3(ATB3_CLON) == TRUE )
									{
										ip = m_ParmeterBox.IP;
										int a = m_ParmeterBox.IP.GetLevel();
									}
									else if( IsAtb3(ATB3_ENEMYCLON) == TRUE )
									{
										ip = CR[targetnewid.ID].m_ParmeterBox.IP;
											
										ip.SetStr( CR[targetnewid.ID].m_ParmeterBox.IP.GetStr()/2);
										ip.SetDex( CR[targetnewid.ID].m_ParmeterBox.IP.GetDex()/2);
										ip.SetVit( CR[targetnewid.ID].m_ParmeterBox.IP.GetVit()/2);
										ip.SetInt( CR[targetnewid.ID].m_ParmeterBox.IP.GetInt()/2);
										ip.SetLife( CR[targetnewid.ID].m_ParmeterBox.IP.GetLife()/2);
										ip.SetMana( CR[targetnewid.ID].m_ParmeterBox.IP.GetMana()/2);
										ip.SetAC( CR[targetnewid.ID].m_ParmeterBox.GetAC()/2);
									}
									else if( IsAtb3(ATB3_SEAHORSECLON) == TRUE )
									{
										ip = m_ParmeterBox.IP;
						
										ip.SetStr( m_ParmeterBox.IP.GetStr()/10);
										ip.SetDex( m_ParmeterBox.IP.GetDex()/10);
										ip.SetVit( m_ParmeterBox.IP.GetVit()/10);
										ip.SetInt( m_ParmeterBox.IP.GetInt()/10);
										ip.SetLife( m_ParmeterBox.IP.GetLife()/10);
										ip.SetMana( m_ParmeterBox.IP.GetMana()/10);
										ip.SetAC( m_ParmeterBox.GetAC()/10);
									}
									else if(IsAtb3(ATB3_CLON2) == TRUE )
									{
										ip = m_ParmeterBox.IP;
						
										ip.SetStr( m_ParmeterBox.IP.GetStr()/2);
										ip.SetDex( m_ParmeterBox.IP.GetDex()/2);
										ip.SetVit( m_ParmeterBox.IP.GetVit()/2);
										ip.SetInt( m_ParmeterBox.IP.GetInt()/2);
										ip.SetLife( m_ParmeterBox.IP.GetLife()/2);
										ip.SetMana( m_ParmeterBox.IP.GetMana()/2);
										ip.SetAC( m_ParmeterBox.GetAC()/2);
									}
									else
									{
										clGrp.Error("NO ADE", "kind:[%d]", m_cltCharStatus.GetKind());
									}

									// ĳ���͸� �ɴ´�. 
									// ȥ���ϱ� ����̰ų�, 
									// ������Ʈ �ؾ� �� ĳ���Ͷ��, 
									//if(//OBInfo.IsSingleMode == TRUE ||
									if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
									{
										// ĳ���͸� �ɴ´�. 
										SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);

										// ���� ĳ������ �Ӽ��� �����Ѵ�. 
										if( IsAtb3(ATB3_CLON) == TRUE)
										{
											CR[id].m_cltCharDrawInfo.SetTransparentSwitch(TRUE);
										}
										
										CR[id].HostID = m_NewID;

										// �нſ� ���� Ư��ȿ���� �ִ´�. 
										SI32 smoke_id=FindEmptySmoke();
										if(smoke_id)
										{
											SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
										}

										//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
										//if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
										//{
										pGame->pBattle->PushUpdateCharInfo(id);
										Step = 0;
										i =100,j=100;
										//}
									}
									else
									{
										SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);
										
										if( IsAtb3(ATB3_CLON) == TRUE)
										{
											CR[id].m_cltCharDrawInfo.SetTransparentSwitch(TRUE);
										}
										Step = 0;
										i =100,j=100;
									}
								}//count;
							}   //if(Map.IsEmpty(x+j, y+i)==TRUE)
						} 
						
					}	//if(id)
				}
			}	// for
		}	// for

		m_scBusySwitch=FALSE;
		OrderStop(m_NewID, BY_COMPUTER);
		return TRUE;
	}
	return TRUE;
}
	
	