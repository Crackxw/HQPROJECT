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

BOOL _Char::CanReCall()
{
//	_NewID targetid=Para2;
	//�ӽ�.
	if(IsAtb3(ATB3_MAGIC_RECALL_PANDA) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_RECALL_PANDA)
			return FALSE;

		if(CR[m_NewID.ID].m_siReCallCount == 1)
			return FALSE;

		//for(int i = 0 ; i < MAX_CHAR_NUMBER ; i++)
		//{
		//	if(KIND_ON_PANDER == GD.CharInfo[i])
		//		return FALSE;
		//}
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_RECALL_MONKEY)
			return FALSE;

/*		for(int i = 0 ; i < MAX_CHAR_NUMBER ; i++)
		{
			if(KIND_ON_GOLDDRAGON == GD.CharInfo[i])
				return FALSE;
		}
*/
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_AMAKUSA) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_RECALL_AMAKUSA)
			return FALSE;
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_TIGER) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_RECALL_TIGER)
			return FALSE;
	}

	return TRUE;
}

BOOL _Char::ReCallPanda()
{	
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ������ ����ų �� ���ٸ�, �ߴ��Ѵ�.
		 if(CanReCall()==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, healrange))
		 { 
			 WorkStep=WS_HEAL;
		 }
		 else // ���� ��Ÿ� �ۿ� ������, 
		 {  
			  WorkStep=WS_GOTO_TARGET;
		      // ���� �ִ� ������ �̵��Ѵ�. 
			  SetMove(CR[targetid.ID].GetX(), CR[targetid.ID].GetY());
		 }
		break;


	case WS_GOTO_TARGET:

		status=Move(BASIC_MOVE_BLOCK_COUNT);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}

		   if(m_scBusySwitch==FALSE)
		   {
			   if(CanReCall()==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, healrange))
			   {
				  WorkStep=WS_HEAL;
				  Step=0;
			   }

 		       // �������� �����ϰų� �߰� ������ �����ϸ�, 
		       if(status>0)
			   {
			       WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				   
			       AttackGiveUpDelayor++;
		           if(AttackGiveUpDelayor>AttackGiveUpDelay)
				      OrderStop(m_NewID, BY_COMPUTER);
			   }

		   }

		break;

	case WS_HEAL:
		 PandaRecallOperation(targetid);
	     break;
	}
	return TRUE;
}



BOOL _Char::ReCall()
{
	////////////////////////////////////////////////////////////////////
	SHORT		i, j;
	SHORT		id;
	SHORT		count;				// ��ȯ�� ������ ��
	_NewID		targetnewid;		// ��ȯ�� ������ newid
	SI32		targetKind;			// ��ȯ�� ������ ����
	SI32		magicpower;			// ��ȯ���� ����� �� �ʿ��� ������
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
		if( CanReCall() == FALSE )
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return TRUE;
		}

		////////////////////////////////////////////////////////////////////
		// �н��� ��������� ���� ��ġ. 
		x = m_ssX;
		y = m_ssY;
		
		if( IsAtb3(ATB3_MAGIC_RECALL_MONKEY) ==TRUE )
		{
			magicpower		= MAGICPOWER_RECALL_MONKEY;

			//targetkind == Ȳ��
			targetKind = KIND_ON_MONKEY;

			//�ѹ��� �ϳ��� �нŸ� ����� ����. 

			count =  1;	

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;
		}

		//
		else if(  IsAtb3(ATB3_MAGIC_RECALL_AMAKUSA) ==TRUE )
		{
			magicpower		= MAGICPOWER_RECALL_AMAKUSA;

			if(GD.Random() % 2)
				targetKind = KIND_ON_MAGICIAN; //�����.
			else 
				targetKind = KIND_ON_SKYNINE;  //õ��.


			//�ѹ��� �ϳ��� �нŸ� ����� ����. 
			count =  1;	

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;
		} 
		//��ȣ�� ����ȣ���̸� ȣ���Ѵ�.
		else if( IsAtb3(ATB3_MAGIC_RECALL_TIGER) ==TRUE )
		{

			magicpower		= MAGICPOWER_RECALL_TIGER;


			targetKind = KIND_ON_PAWNTIGER;  //����ȣ����


			//�ѹ��� �ϳ��� �нŸ� ����� ����. 
			count =  1;	

			// �н��� ��������� ���� ��ġ. 
			x = m_ssX;
			y = m_ssY;
		}
		else
		{
			clGrp.Error("f0262_ReCall", "kind_data:%d", m_cltCharStatus.GetKind());
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
									//effect ����.
									PushEffect(EFFECT_CLON, x, y);
															
									ReduceMana(magicpower);  //���� ����.
														
									_ImportantParameta ip;   //ĳ���� ����.
									ZeroMemory(&ip, sizeof(_ImportantParameta));

									if( IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE )
									{
										ip = m_ParmeterBox.IP;

										ip.SetStr(m_ParmeterBox.IP.GetStr()/3);
										ip.SetDex( m_ParmeterBox.IP.GetDex()/3);
										ip.SetVit( m_ParmeterBox.IP.GetVit()/3);
										ip.SetInt( m_ParmeterBox.IP.GetInt()/3);
										ip.SetLife( m_ParmeterBox.IP.GetLife()/3);
										ip.SetMana( m_ParmeterBox.IP.GetMana()/3);
										ip.SetAC( m_ParmeterBox.IP.GetAC()/3);
										ip.SetLevel(m_ParmeterBox.IP.GetLevel()/3);
									}
									else if( IsAtb3(ATB3_MAGIC_RECALL_AMAKUSA) == TRUE )
									{
										UI16 siKind;
										siKind = TransKindImjinToOnline(targetKind);
											
										ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetStr());
										ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetDex());
										ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetVit());
										ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetInt());
										ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetLife());
										ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetMana());
										ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetAC());
										ip.SetLevel(1);
									}
									else if( IsAtb3(ATB3_MAGIC_RECALL_TIGER) == TRUE )
									{
										ip = m_ParmeterBox.IP;
											
										ip.SetStr( m_ParmeterBox.IP.GetStr()/2);
										ip.SetDex( m_ParmeterBox.IP.GetDex()/2);
										ip.SetVit( m_ParmeterBox.IP.GetVit()/2);
										ip.SetInt(m_ParmeterBox.IP.GetInt()/2);
										ip.SetLife( m_ParmeterBox.IP.GetLife()/2);
										ip.SetMana( m_ParmeterBox.IP.GetMana()/2);
										ip.SetAC( m_ParmeterBox.IP.GetAC()/2);

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

										
										CR[id].HostID = m_NewID;

										// �нſ� ���� Ư��ȿ���� �ִ´�. 
										SI32 smoke_id=FindEmptySmoke();
										if(smoke_id)
										{
											SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
										}

										//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
										pGame->pBattle->PushUpdateCharInfo(id);
										Step = 0;
										i =100,j=100;
									}
									else
									{
										SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);
										
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

BOOL _Char::PandaRecallOperation(_NewID targetnewid)
{
	SHORT		i, j;
	SHORT		id;
	SI32		targetKind;			// ��ȯ�� ������ ����
	SI32		magicpower;			// ��ȯ���� ����� �� �ʿ��� ������
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
	if(Step == 1)
	{
		// �нſ� ���� Ư��ȿ���� �ִ´�. 
		SI32 smoke_id=FindEmptySmoke();
		if(smoke_id)
		{
			SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[targetnewid.ID].m_cltCharStatus.GetCenterX(), CR[targetnewid.ID].m_cltCharStatus.GetCenterY(), AREA_LAND);
		}

		smoke_id=FindEmptySmoke();
		if(smoke_id)
		{
			SetSmoke(KIND_SMOKE_PANDA, smoke_id, CR[targetnewid.ID].m_cltCharStatus.GetCenterX(), CR[targetnewid.ID].m_cltCharStatus.GetCenterY(), AREA_LAND);
		}

		SHORT level=m_ParmeterBox.IP.GetLevel();

		SI32 apower = CalcDamage(SPECIAL_EARTHQUAKE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, KI[m_cltCharStatus.GetKind()].m_siKindAtb); 

		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_EARTHQUAKE, 2, AREA_LAND, 
						apower, targetnewid, CR[targetnewid.ID].m_ssX, CR[targetnewid.ID].m_ssY, m_ParmeterBox.GetHitRate(0));
	}
	
	
	if( Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[aniindex].siInstant )
	{
		if( CanReCall() == FALSE )
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return TRUE;
		}
		
		if( IsAtb3(ATB3_MAGIC_RECALL_PANDA) == TRUE )
		{
			magicpower		= MAGICPOWER_RECALL_PANDA;

			//targetkind == �Ҵ�.
			targetKind = KIND_ON_PANDER;

			//�ѹ��� �ϳ��� �нŸ� ����� ����. 
			//count =  1;	
			CR[m_NewID.ID].m_siReCallCount = 1;

			// �н��� ��������� ���� ��ġ. 
			x = CR[targetnewid.ID].m_ssX;
			y = CR[targetnewid.ID].m_ssY;
		}
		if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
		{	

		}
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
								
								//if(count == 1)
								if(CR[m_NewID.ID].m_siReCallCount == 1)
								{
									//count = 0;
									//CR[targetid.ID].m_siReCallCount = 0;
									//effect ����.
									PushEffect(EFFECT_CLON, x, y);
															
									ReduceMana(magicpower);  //���� ����.
														
									_ImportantParameta ip;   //ĳ���� ����.
									ZeroMemory(&ip, sizeof(_ImportantParameta));

									//��ȯ�Ǵ� �Ҵ��� �ɷ�ġ ����.
							 		if( IsAtb3(ATB3_MAGIC_RECALL_PANDA) == TRUE )
									{
										ip = m_ParmeterBox.IP;
											
										ip.SetStr( m_ParmeterBox.IP.GetStr()*3);
										ip.SetDex( m_ParmeterBox.IP.GetDex()*3);
										ip.SetVit( m_ParmeterBox.IP.GetVit()*3);
										ip.SetInt(0);
										ip.SetLife( m_ParmeterBox.IP.GetLife()*3);
										ip.SetMana(0);
										ip.SetAC( m_ParmeterBox.IP.GetAC()*3);
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
										
										CR[id].HostID = m_NewID;
									
										//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
										pGame->pBattle->PushUpdateCharInfo(id);
										Step = 0;
										i =100,j=100;
									}
									else
									{
										SetChar(targetKind, id, x+j, y+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);
										
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


BOOL _Char::ReCallDamduk()
{
	SI32 aniindex;
	if(MagicCharAnimationIndex)
	{
		m_cltCharDrawInfo.SetAnimation(ANI_MAGIC);
		aniindex = MagicCharAnimationIndex;
	}
	else
	{
		m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);
		aniindex = AttackCharAnimationIndex;
	}
	
	m_scBusySwitch = TRUE;

	// ��ȯ�� ������ ����
	SI32 targetKind = KI[m_cltCharStatus.GetKind()].m_SubData.m_siOptionType_01;

	// ��ȯ�� ������
	SI32 recall_num = KI[m_cltCharStatus.GetKind()].m_SubData.m_siOptionValue1_01;

	// ��ȯ���� ����� �� �ʿ��� ������
	SI32 magicpower = KI[m_cltCharStatus.GetKind()].m_SubData.m_siOptionValue2_01;

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
		if(CanReCallDamduk() == FALSE)
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return TRUE;
		}

		// �ֺ��� ����ǰ�� �����.
		SI32 siCount = 0;
		for(SHORT i=-3;i<=3;i++)
		{
			for(SHORT j=-3;j<=3;j++)
			{
				if(i==0 && j==0)	continue;
				
				if(Map.IsInMapArea(m_ssX+j, m_ssY+i)!=TRUE)	continue;

				if(siCount >= recall_num)			continue;
				
				SHORT id = FindEmptyID(m_cltCharStatus.GetPlayerNumber());
				if(!id)		continue;
				
				// ĳ���͸� ���� �� �ִ� ������ �ɴ´�. 
				if(Map.IsEmpty(m_ssX+j, m_ssY+i)!=TRUE)		continue;

				//effect ����.
				PushEffect(EFFECT_CLON, m_ssX, m_ssY);
				
				ReduceMana(magicpower);  //���� ����.
				
				_ImportantParameta ip;   //ĳ���� ����.
				ZeroMemory(&ip, sizeof(_ImportantParameta));
				
				SI16 siKind = TransKindImjinToOnline(targetKind);
				
				ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetStr());
				ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetDex());
				ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetVit());
				ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetInt());
				ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetLife());
				ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetMana());
				ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetAC());
				ip.SetLevel(1);
				
				// ĳ���͸� �ɴ´�. 
				if(pGame->pBattle->IsProperCharForUpdate(GetPlayerNumber()) == TRUE)
				{
					// ĳ���͸� �ɴ´�. 
					SetChar(targetKind, id, m_ssX+j, m_ssY+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);
					
					CR[id].HostID = m_NewID;
					
					// �нſ� ���� Ư��ȿ���� �ִ´�. 
					SI32 smoke_id=FindEmptySmoke();
					if(smoke_id)
						SetSmoke(KIND_SMOKE_CLONAPPEAR, smoke_id, CR[id].m_cltCharStatus.GetCenterX(), CR[id].m_cltCharStatus.GetCenterY(), AREA_LAND);
					
					//�ٸ� ����ڿ��� �˷��� �� ĳ���Ͷ��, 
					pGame->pBattle->PushUpdateCharInfo(id);
					Step = 0; siCount++;
				}
				else
				{
					SetChar(targetKind, id, m_ssX+j, m_ssY+i, SOUTH, 100, GetPlayerNumber(), &ip, NULL, -1);
					Step = 0; siCount++;
				}
			}
		} 

		m_scBusySwitch=FALSE;
		OrderStop(m_NewID, BY_COMPUTER);
		return TRUE;
	}
	return TRUE;
}

BOOL _Char::CanReCallDamduk()
{
	if(IsAtb4(ATB4_MAGIC_RECALL_DAMDUK) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() < KI[m_cltCharStatus.GetKind()].m_SubData.m_siOptionValue2_01)
			return FALSE;

		if(CR[m_NewID.ID].m_siReCallCount == 1)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}