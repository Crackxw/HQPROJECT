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

extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_Map 				Map;
extern SI32	UserPlayerNumber;
extern _clGame* pGame;
extern OnlineBattleInfo OBInfo;

BOOL _Char::CanClon(_NewID targetnewid)
{

	if( IsAtb3(ATB3_CLON) == TRUE )
	{
		// 마법력이 충분해야 한다. 
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_CLON)	return FALSE;
	}
	else if( IsAtb3(ATB3_ENEMYCLON) == TRUE )
	{
		// 마법력이 충분해야 한다. 
		if( m_ParmeterBox.IP.GetMana() < MAGICPOWER_ENEMYCLON )	return FALSE;


		// 대상이 생존해 있어야 한다. 
		if( IsAlive(targetnewid) == FALSE )		return FALSE;


		// 대상이 나와 적대적이어야 한다. 
		if( IsFriendToByPlayerNumber(GetPlayerNumber(), CR[targetnewid.ID].GetPlayerNumber()) == TRUE )	return FALSE;
	}
	else if( IsAtb3(ATB3_SEAHORSECLON) == TRUE )
	{
		// 마법력이 충분해야 한다. 
		if( m_ParmeterBox.IP.GetMana() < MAGICPOWER_CLON )	return FALSE;
	}
	else if( IsAtb3(ATB3_CLON2) == TRUE )
	{
		// 마법력이 충분해야 한다. 
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
	SHORT		count;				// 만들 분신의 수
	_NewID		targetnewid;		// 만들 분신의 newid
	SI32		targetKind;			// 만들 분신의 종류
	SI32		magicpower;			// 분신술을 만드는 데 필요한 마법력
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

		// 분신이 만들어지는 기준 위치. 
		x = m_ssX;
		y = m_ssY;
		
		if( IsAtb3(ATB3_CLON) == TRUE )
		{
			magicpower		= MAGICPOWER_CLON;

			targetKind = m_cltCharStatus.GetKind();

			//한번에 하나의 분신만 만들어 진다. 
			count =  1;	

			// 분신이 만들어지는 기준 위치. 
			x = m_ssX;
			y = m_ssY;
		}
		else if( IsAtb3(ATB3_ENEMYCLON) == TRUE )
		{
			magicpower		= MAGICPOWER_ENEMYCLON;

			// 복제할 캐릭터의 종류를 알아온다.
			targetKind		= CR[targetnewid.ID].m_cltCharStatus.GetKind();

			count = 1;

			// 분신이 만들어지는 기준 위치. 
			x = CR[targetnewid.ID].GetX(); 
			y = CR[targetnewid.ID].GetY(); 

		}
		else if( IsAtb3(ATB3_SEAHORSECLON) == TRUE )
		{

			magicpower		= MAGICPOWER_CLON;

			// 복제할 캐릭터의 종류를 알아온다.
			targetKind		= KIND_ON_SEAHORSE;

			count = 1;

			// 분신이 만들어지는 기준 위치. 
			x = m_ssX;
			y = m_ssY;

		}
		else if( IsAtb3(ATB3_CLON2) == TRUE )
		{

			magicpower		= MAGICPOWER_CLON;

			// 복제할 캐릭터의 종류를 알아온다.
			if(GD.Random() % 2)
				targetKind		= KIND_ON_HUNGRYDEMON;
			else
				targetKind		= KIND_ON_EATERDEMON;

			count = 1;

			// 분신이 만들어지는 기준 위치. 
			x = m_ssX;
			y = m_ssY;

		}

		else
		{
			clGrp.Error("f0262_Clon", "kind_data:%d", m_cltCharStatus.GetKind());
		}

		
		// 주변에 복제품을 만든다. 
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
							
							// 캐릭터를 심을 수 있는 곳에만 심는다. 
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

									// 캐릭터를 심는다. 
									// 혼자하기 모드이거나, 
									// 업데이트 해야 할 캐릭터라면, 
									//if(//OBInfo.IsSingleMode == TRUE ||
									if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
									{
										// 캐릭터를 심는다. 
										SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);

										// 심은 캐릭터의 속성을 설정한다. 
										if( IsAtb3(ATB3_CLON) == TRUE)
										{
											CR[id].m_cltCharDrawInfo.SetTransparentSwitch(TRUE);
										}
										
										CR[id].HostID = m_NewID;

										// 분신에 따른 특수효과를 넣는다. 
										SI32 smoke_id=FindEmptySmoke();
										if(smoke_id)
										{
											SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
										}

										//다른 사용자에게 알려야 할 캐릭터라면, 
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
	
	