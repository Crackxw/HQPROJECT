//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

#include "..\Gersang\maindata.h"
#include <etc.h>
#include "..\Gersang\calcattack.h"
#include "..\Gersang\music.h"
#include "..\Gersang\mine.h"
#include <CharOrg.h>
#include "..\Gersang\yprg.h"

#include <Map.h>
#include <magic.h>
#include <yprg2.h>

#include <bullet.h>
#include <smoke.h>
#include <charinterface.h>
#include <mapinterface.h>
#include <SPrg.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern	_MainData			GD;
extern	_Map				Map;
extern	int					MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값, 
extern	int					MapCenterY[][MAX_MAP_YSIZE];
extern	_Char				CR[];								//  캐릭터의 포인터 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_NewID				ZeroID;
extern	cltMapInterface		clifMap;


static SI32	targetid[50];
static SI32	targetidCount;

_Magic::_Magic(SI32 simagickind, SI32 six, SI32 siy, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	m_siMagicKind = simagickind;

	m_siX = six;
	m_siY = siy;

	m_MagicID = newid;
	m_siMagicPlayerNumber = siplayernumber;
	m_siArea = siarea;
	m_siApower = siapower;
	m_siHitRate = sihitrate;
	m_AttackID = attackid;
	m_siSmokeID = 0;
}


SI32 _Magic::Action()
{
	SetMessage("000000");
	return 0;
}


// 폭뢰격 
_KimBomb::_KimBomb(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) 
: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 1;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_KIMBOMB, m_siX, m_siY);
	}
}


SI32 _KimBomb::Action()
{
	SI32 i;
	SI32 siStepCount = 0 ;
	POINT Point[100];
	SI16 id;

	if(GD.Frame % 3 )return 0;
	
	if(Circle(m_siStep, m_siX, m_siY, &siStepCount, Point) == TRUE)
	{
		for(i = 0;i<siStepCount;i++)
		{
			if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
			{
				id = FindEmptySmoke();
				if(id)
				{
					switch(GD.Random() % 3)
					{
					case 0:
						SetSmoke(KIND_SMOKE_KIMBOMB1, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
						break;
					case 1:
						SetSmoke(KIND_SMOKE_KIMBOMB2, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
						break;
					default:
						SetSmoke(KIND_SMOKE_KIMBOMB3, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
						break;
					}
				}
				SI32 attackid = Map.GetIDMap(Point[i].x, Point[i].y);
				if(IsAlive(attackid) )
				{
					CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_KIMBOMB, 0, m_siArea, m_siApower, CR[attackid].m_NewID, (SHORT)Point[i].x, (SHORT)Point[i].y, m_siHitRate);
				}
				
/*				if(GD.Random() % 10 == 0)
				{
					Map.PutFire(Point[i].x, Point[i].y, 1, GROUNDFIRE_LIFETIME);
				}
				*/
			}
			
		}

	}


	m_siStep++;

	if(m_siStep == 6)return TRUE;

	return 0;
}

// 만월도끼춤. 
_Magic5j::_Magic5j(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate,  attackid)
{

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_SPEAR2, m_siX, m_siY);
	}

}


SI32 _Magic5j::Action()
{
	SI32	i, j, k;
	SI32	bulletid;
	SI32	firetilex, firetiley, desttilex, desttiley;
	_NewID	tempid;


	for( k=0; k<50; k++ )
	{
		targetid[k] = -1;
	}

	for(i = -1;i <= 1;i++)
		for(j = -1;j <= 1;j++)
		{
			if(i==0 && j ==0)continue;

			bulletid = FindEmptyBullet();
			if(bulletid)
			{
				firetilex = m_siX + j;
				firetiley = m_siY + i;
				if(Map.IsInMapArea(firetilex, firetiley) == TRUE)
				{
					tempid = FindNearEnemy(firetilex, firetiley);
					if(tempid.IsEmpty()==FALSE)
					{
						desttilex = clifChar.GetX(tempid.ID);
						desttiley = clifChar.GetY(tempid.ID);
					}
					else
					{
						desttilex = firetilex;
						desttiley = firetiley;
					}
					SI32 enemydr = CR[tempid.ID].m_ParmeterBox.GetDR();
					SI32 hitrate = CR[m_MagicID.ID].m_ParmeterBox.GetHitRate(enemydr) + 20;
					if(hitrate>100)		hitrate = 100;


					SetBullet(KIND_BULLET_MAGIC5J, bulletid, 
						firetilex, firetiley, 
						desttilex, desttiley, 
						MapCenterX[firetilex][firetiley], MapCenterY[firetilex][firetiley],
						MapCenterX[desttilex][desttiley], MapCenterY[desttilex][desttiley], 
						m_MagicID, tempid, m_siApower, hitrate, 0, AREA_LAND);
				}
			}
			
		}

	return TRUE;
}

_NewID _Magic5j::FindNearEnemy(SI32 six, SI32 siy)
{
	SI32	i, j;
	SI32	id;
	SI32	tilex, tiley;
	_NewID  attack_newid, tempid;
	SI32	length;
	SI32	min_length = 1000;
	SI32	min_attack_length = 1000;
	BOOL	bExist;

	targetidCount = 0;

	attack_newid.Init();

	for( i=-8; i <= 8; i++ )
		for( j=-8; j <=8; j++ )
		{
			tilex = six + j;
			tiley = siy + i;
			
			if(Map.IsInMapArea(tilex, tiley)==TRUE)
			{
				id = Map.GetIDMap(tilex, tiley);
				if(IsAlive(id) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, clifChar.GetPlayerNumber(id) ) == FALSE)
				{
					
					length = max(abs(j), abs(i));
					
					if(length < min_attack_length)
					{
						tempid = clifChar.GetID(id);
						
						bExist = FALSE;

						// 하나의 도끼날이 하나의 적에게만 가게 하기 위한 조치. 
/*						for( k=0; k<50; k++ )
						{
							if( tempid.ID == targetid[k] )
								bExist = TRUE;
						}
*/						
						if( bExist == FALSE )
						{
							targetid[targetidCount] = tempid.ID;
							targetidCount++;
							if( targetidCount >= 50 ) targetidCount = 0;
							
							min_attack_length = length;
							attack_newid = tempid;
						}
					}
				}
			}
		}


	return attack_newid;
}


// 일정 범위의 지뢰를 제거한다. 
_MagicDetectMine::_MagicDetectMine(SI32 simagickind, SI32 siX, SI32 siY, SI32 siplayernumber) : _Magic(simagickind, siX, siY,  ZeroID,  siplayernumber,  AREA_LAND,  0, 0, ZeroID)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		// PushEffect(EFFECT_TORNADO, m_siX, siY);
	}
}

SI32 _MagicDetectMine::Action()
{
	SI32 siStepCount = 0 ;
	
	SI32 i;
	SI16 id;
	POINT Point[100];

	if(GD.Frame % 6 )return 0;
	
	if(Circle(m_siStep, m_siX, m_siY, &siStepCount, Point) == TRUE)
	{

		for(i = 0;i<siStepCount;i++)
		{
			if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
			{
				id = FindEmptySmoke();
				if(id)
				{
					SetSmoke(KIND_SMOKE_DETECTMINEEFFECT, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
				}
				
				// 적의 지뢰가 매설되어 있다면. 
				if(IsEnemyMine(Point[i].x, Point[i].y, m_siMagicPlayerNumber))
				{
					DetectEnemyMine(Point[i].x, Point[i].y);
					
				}
			}
			
		}
		
	}
	
	m_siStep++;

	if(m_siStep == 6)return TRUE;

	return 0;
}


// 일정 범위의 불을 끈다.
_MagicExtinguish::_MagicExtinguish(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower, 0, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_EXTINGUISH, m_siX, m_siY);
	}
}

SI32 _MagicExtinguish::Action()
{
	SI32 siStepCount = 0 ;
	
	SI32 i;
	SI16 id;
	POINT Point[100];

	if(GD.Frame % 3 )return 0;
	
	if(Circle(m_siStep, m_siX, m_siY, &siStepCount, Point) == TRUE)
	{

		for(i = 0;i<siStepCount;i++)
		{
			if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
			{
				id = FindEmptySmoke();
				if(id)
				{
						SetSmoke(KIND_SMOKE_WATEREFFECT, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
				}

				if(Map.GetFireMap(Point[i].x,Point[i].y))
				{
					Map.DecreaseFireLifeTime(Point[i].x, Point[i].y, 100);
				}
			}
			
		}

	}

	m_siStep++;

	if(m_siStep == 7)return TRUE;

	return 0;
}

_MagicIce::_MagicIce(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;
}

SI32 _MagicIce::Action()
{
	SI16 id;

	if(GD.Frame % 6 )return 0;

	for(int i = -1 ; i < 2 ; i++)
	{
		for(int j = - 1; j < 2 ; j++)
		{
			id = Map.GetIDMap(m_siX + i, m_siY + j);

			if(IsAlive(id) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, CR[id].GetPlayerNumber()) == FALSE)
			{
				OrderStop(CR[id].m_NewID, BY_PLAYER);
				CR[id].SetHalt(HALT_REASON_ICE, 150 + min(150, m_siApower));
			}
		}
	}

	m_siStep++;

	if(m_siStep == 6)return TRUE;



	return 0;
}

// 포박술. 
_Magic6c::_Magic6c(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 0;

}

SI32 _Magic6c::Action()
{
	SI32 i;
	SI32 siStepCount = 0;
	POINT Point[100];
	SI16 Smokeid, crid;

	if(GD.Frame % 6 )	return 0;
	
	if(!(m_siStep % 2))
	{
		if(Circle((m_siStep/2)*2+1, m_siX, m_siY, &siStepCount, Point) == TRUE)
		{
			for(i = 0; i < siStepCount; i++)
			{
				if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
				{
					Smokeid = FindEmptySmoke();
					if(Smokeid)
					{
						switch(GD.Random() % 3)
						{
						case 0:
							SetSmoke(KIND_SMOKE_MAGIC6C1, Smokeid, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
							break;
						case 1:
							SetSmoke(KIND_SMOKE_MAGIC6C2, Smokeid, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
							break;
						default:
							SetSmoke(KIND_SMOKE_MAGIC6C3, Smokeid, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
							break;
						}
					}
				}
			}
		}
	}

	if(Circle(m_siStep, m_siX, m_siY, &siStepCount, Point) == TRUE)
	{
		for(i = 0; i < siStepCount; i++)
		{
			// 영역내의 캐릭에 마비를 건다.
			if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
			{
				crid = Map.GetIDMap(Point[i].x, Point[i].y);
				
				//비호는 포박술에 걸리지 않는다.
				//if(GD.CharInfo[crid] == KIND_ON_AGILETIGER)
				//	continue;
				if(IsAlive(crid) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, CR[crid].GetPlayerNumber()) == FALSE)
				{
					OrderStop(CR[crid].m_NewID, BY_PLAYER);
					CR[crid].SetHalt(HALT_REASON_IVY, m_siApower);
				}
			}
		}
	}


	m_siStep++;

	if(m_siStep == 6)return TRUE;

	return 0;
}
//마법 테스트
////////////////////////////////////////////////////////////////////////////////
_MagicTest::_MagicTest(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicTest::Action()
{
	SI32 siStepCount = 0 ;
	SI16 Length = 0;
	SI16 Path[1000];
	SI16 Ax,Ay;
	SI16 id;
	SI16 tilex, tiley;
	SI16 siRandX;
	SI16 siRandY;
	
	if(GD.Frame % 5 )return 0;

	if(IsAlive(m_AttackID) == FALSE)
	{
		return TRUE;
	}

	Ax = CR[m_AttackID.ID].m_cltCharStatus.GetCenterX();
	Ay = CR[m_AttackID.ID].m_cltCharStatus.GetCenterY();

	// 탄의 이동경로를 추적한다. 
	FindStraightLine(MapCenterX[m_siX][m_siY],MapCenterY[m_siX][m_siY], Ax, Ay, 
			     				 Length,  Path, 1000/2);

	
	

	SI16 x = Path[m_siStep*2+0];
	SI16 y = Path[m_siStep*2+1];

	
	m_siStep+=20;

	// 폭발. 
	Map.GetRealTileByRealDot(x, y, &tilex, &tiley);
	id = Map.GetIDMap(tilex,tiley);
	if( id != 0)
	{
		SI32 smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, x,y, AREA_LAND);
		}


		if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
		{

			PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, clifChar.GetID(id), tilex, tiley, m_siHitRate);
		}

	}
	if(m_siStep >= Length  || m_siStep > 700)
	{

		PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

		CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, m_AttackID, tilex, tiley, m_siHitRate);

		//tilex tiley
		

		/////////////////////////////////////////////////////////////////
		for(int i = 0 ; i < 3 ; i++)
		{
			siRandX = rand() % 3 + 1;
			siRandY = rand() % 3 + 1;
			id = Map.GetIDMap(tilex + siRandX ,tiley + siRandY);
			if(id != 0)
			{
				PushEffect(KIND_SMOKE_GHOSTCATEXP, tilex + siRandX, tiley + siRandY);
				CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, clifChar.GetID(id), tilex + siRandX, tiley + siRandY, m_siHitRate);

			}
			id = Map.GetIDMap(tilex - siRandX ,tiley - siRandY);
			if(id != 0)
			{
				PushEffect(KIND_SMOKE_GHOSTCATEXP, tilex - siRandX, tiley - siRandY);
				CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, clifChar.GetID(id), tilex - siRandX, tiley - siRandY, m_siHitRate);

			}
			
		}
		return TRUE;
		////////////////////////////////////////////////////////////
	}
	/*
	if(m_siStep >= Length  || m_siStep > 700)
	{
		SI32 smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, x,y, AREA_LAND);
		}


		if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
		{

			PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, siApower, clifChar.GetID(id), tilex, tiley, m_siHitRate);
		}
		return TRUE;
	}*/
		
	

	id = FindEmptySmoke();
	if(id)
	{
		SetSmoke(KIND_SMOKE_MAGIC4C, id, x, y, AREA_LAND);
		
	}

	return 0;
	
}
//장풍
_MagicEnergyPa::_MagicEnergyPa(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicEnergyPa::Action()
{
	SI16 x = m_siX;
	SI16 y = m_siY;

	SI32 temp_id = Map.GetIDMap(x,y);
	CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_ENERGYPA, 1, AREA_LAND, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
	return TRUE;
}

_MagicFireEnergyPa::_MagicFireEnergyPa(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind,siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}

	// 효과이미지를 뿌려준다.
	SHORT smokeid1=FindEmptySmoke();
	if(smokeid1)
	{
		SetSmoke(KIND_FIREENERGYPA_HIT1, smokeid1, MapCenterX[m_siX][siY], MapCenterY[m_siX][siY], AREA_LAND);
	}
	SHORT smokeid2=FindEmptySmoke();
	if(smokeid2)
	{
		SetSmoke(KIND_FIREENERGYPA_HIT2, smokeid2, MapCenterX[m_siX][siY], MapCenterY[m_siX][siY], AREA_LAND);
	}
}
 
SI32 _MagicFireEnergyPa::Action()
{
	m_siStep++;
	if(m_siStep >= 60)			
		return TRUE;
	SI16 x = m_siX;
	SI16 y = m_siY;
	


	//Ax = clifChar.GetX(m_AttackID.ID);
	//Ay = clifChar.GetY(m_AttackID.ID);

	//Ax = CR[m_AttackID.ID].m_cltCharStatus.GetCenterX();
	//Ay = CR[m_AttackID.ID].m_cltCharStatus.GetCenterY();

//	for(int i = -1 ; i < 2 ; i++)
//	{
//		for(int j = -1 ; j < 2 ; j++)
//		{
			SI32 temp_id = Map.GetIDMap(x ,y);
			//if(temp_id)
			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_ENERGYPA, 2, AREA_LAND, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
//		}
//	}
	return FALSE;
}

// 기공신포. 
_Magic4c::_Magic4c(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _Magic4c::Action()
{
	SI32 siStepCount = 0 ;
	SI16 Length = 0;
	SI16 Path[1000];
	SI16 Ax,Ay;
	SI16 id;
	SI16 tilex, tiley;

	
	if(GD.Frame % 5 )return 0;

	if(IsAlive(m_AttackID) == FALSE)
	{
		return TRUE;
	}

	Ax = CR[m_AttackID.ID].m_cltCharStatus.GetCenterX();
	Ay = CR[m_AttackID.ID].m_cltCharStatus.GetCenterY();

	// 탄의 이동경로를 추적한다. 
	FindStraightLine(MapCenterX[m_siX][m_siY],MapCenterY[m_siX][m_siY], Ax, Ay, 
			     				 Length,  Path, 1000/2);

	
	

	SI16 x = Path[m_siStep*2+0];
	SI16 y = Path[m_siStep*2+1];

	
	m_siStep+=20;

	// 폭발. 
	Map.GetRealTileByRealDot(x, y, &tilex, &tiley);
	id = Map.GetIDMap(tilex,tiley);
	if( id != 0)
	{
		SI32 smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, x,y, AREA_LAND);
		}


		if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
		{

			PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, clifChar.GetID(id), tilex, tiley, m_siHitRate);
		}

	}
	if(m_siStep >= Length  || m_siStep > 700)
	{

		PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

		CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, m_siApower, m_AttackID, tilex, tiley, m_siHitRate);
		return TRUE;
	}
	/*
	if(m_siStep >= Length  || m_siStep > 700)
	{
		SI32 smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_BUILDINGEXP, smokeid, x,y, AREA_LAND);
		}


		if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
		{

			PushEffect(EFFECT_MAGIC4EXP, tilex, tiley);

			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_MAGIC, 2, AREA_LAND, siApower, clifChar.GetID(id), tilex, tiley, m_siHitRate);
		}
		return TRUE;
	}*/
		
	

	id = FindEmptySmoke();
	if(id)
	{
		SetSmoke(KIND_SMOKE_MAGIC4C, id, x, y, AREA_LAND);
		
	}

	return 0;
}

_MagicIvory::_MagicIvory(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicIvory::Action()
{
	return TRUE;
}

_MagicBlueDragon::_MagicBlueDragon(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicBlueDragon::Action()
{
	return TRUE;
}

_MagicRunningFire::_MagicRunningFire(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;
	m_siCount = 0;
	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
//		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicRunningFire::Action()
{
	if(GD.Frame % 5) return 0;

	SI16 x = m_siX;
	SI16 y = m_siY;
	SI16 i,j;
	SI32 smokeid;

	SI16 siTemp_x,siTemp_y;

	SI32 temp_id ;

	m_siCount++;

	if(m_siCount > 5)
		return TRUE;

	for(i = -2 ; i <= 2 ; i++)
	{
		for(j = -2 ; j <= 2 ; j++)
		{
			if(GD.Random() % 10 == 0)
			{
				siTemp_x = x + i;
				siTemp_y = y + j;

				smokeid = FindEmptySmoke();
				if(smokeid)
				{
					SetSmoke(KIND_SMOKE_RUNNINGFIRE, smokeid, MapCenterX[siTemp_x][siTemp_y], MapCenterY[siTemp_x][siTemp_y], AREA_LAND);

					temp_id = Map.GetIDMap(siTemp_x,siTemp_y);

					CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_RUNNINGFIRE, 1, AREA_LAND, m_siApower, clifChar.GetID(temp_id), siTemp_x, siTemp_y, m_siHitRate);
				}
			}
		}
	}

	OrderStop(m_MagicID, BY_COMPUTER);


	return FALSE;
}

_MagicFireAttack::_MagicFireAttack(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;
	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
//		PushEffect(EFFECT_MAGIC4C, m_siX, siY);
	}
}

SI32 _MagicFireAttack::Action()
{

	if(GD.Frame % 5 )return 0;
	SI16 x = m_siX;
	SI16 y = m_siY;

	SI32 temp_id = Map.GetIDMap(x, y);
	 // 마법력을 줄여준다. 
	
	 SI32 smokeid=FindEmptySmoke();

	 if(smokeid)
	 {
		SetSmoke(KIND_SMOKE_FIREATTACK, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
	 }

	 
	 {
		CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_FIREATTACK, 3, AREA_LAND, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
	 }
	 

	 OrderStop(m_MagicID, BY_COMPUTER);

     return TRUE;
}

// 설녀 눈폭풍.  
_SnowMagic::_SnowMagic(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_SNOWWOMANMAGIC, m_siX, m_siY);
	}
}

SI32 _SnowMagic::Action()
{
	SI32 siStepCount = 0 ;
	SI16 Length = 0;
	SI16 Path[1000];

	
	if(GD.Frame % 5 )return 0;

	if(IsAlive(m_AttackID) == FALSE)
	{
		return TRUE;
	}

	// 탄의 이동경로를 추적한다. 
	FindStraightLine(MapCenterX[m_siX][m_siY],        MapCenterY[m_siX][m_siY], CR[m_AttackID.ID].m_cltCharStatus.GetCenterX(), CR[m_AttackID.ID].m_cltCharStatus.GetCenterY(), 
			     				 Length,  Path, 1000/2);

	SI16 x = Path[m_siStep*2+0];
	SI16 y = Path[m_siStep*2+1];

	m_siStep+=30;

	// 폭발. 
	if(m_siStep >= Length || m_siStep > 700)
	{
		SI32 smokeid=FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_SNOWMAGICEXP, smokeid, x,y, AREA_LAND);
		}

		PushEffect(EFFECT_CANNONEXP, x, y);

		SI16 tilex, tiley;
		if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
		{
			CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_ANTIARMOUR, 2, AREA_LAND, m_siApower, m_AttackID, tilex, tiley, m_siHitRate);
		}

		return TRUE;
	}


	SI16 id = FindEmptySmoke();
	if(id)
	{
		SetSmoke(KIND_SMOKE_SNOWMAGIC, id, x, y, AREA_LAND);
		
	}

	return 0;
}

//---------------------------------
// 독무술
//---------------------------------

_Magic2T::_Magic2T(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		//PushEffect(EFFECT_SNOWWOMANMAGIC, m_siX, m_siY);
	}
}

SI32 _Magic2T::Action()
{
	SI32 i, j;
	SI32 x, y;
	SI32 smokeid;

	
	if(GD.Frame % 5 )return 0;

	m_siStep++;

	if(m_siStep >= 60)return TRUE;


	   for(i =  -3 ;i <= 3;i++)
		   for(j = -3 ; j <= 3; j++)
		   {
			   x = m_siX + j;
			   y = m_siY + i;
			   
			   if(Map.IsInMapArea(x, y) == FALSE)continue;
			   
			   if(GD.Random() % 10 == 0)
			   {
				   smokeid = FindEmptySmoke();
				   if(smokeid)
				   {
					   SI32 xvary = 10-GD.Random() % 20;
					   SI32 yvary = 10-GD.Random() % 20;

					   SetSmoke(KIND_SMOKE_SMOKE3, smokeid, MapCenterX[x][y]+xvary, MapCenterY[x][y]+yvary, m_siArea);

					   // 화염이 있는 곳의 유닛을 구한다. 
					   SI32 temp_id = Map.GetIDMap(x, y);
					   
						 // 유닛이 적이면 타격을 준다. 
						 if(IsSee(temp_id) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, clifChar.GetPlayerNumber(temp_id)) == FALSE)
						 {
							CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_POISONARROW, 1, m_siArea, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
						 }
					   
				   }
			   }
			   
		   }
		   
		   
		   return 0;
}

//-----------------------------------
// 거대수
//-----------------------------------
_BigTree::_BigTree(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{

	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_SNOWWOMANMAGIC, m_siX, m_siY);
	}
}

SI32 _BigTree::Action()
{
	SI32 siStepCount = 0 ;
	SI16 Length = 0;
	SI16 Path[1000];

	
	
	if(GD.Frame % 5 )return 0;

	if(IsAlive(m_AttackID) == FALSE)
	{
		return TRUE;
	}

	// 탄의 이동경로를 추적한다. 
	FindStraightLine(MapCenterX[m_siX][m_siY],        MapCenterY[m_siX][m_siY], CR[m_AttackID.ID].m_cltCharStatus.GetCenterX(), CR[m_AttackID.ID].m_cltCharStatus.GetCenterY(), 
			     				 Length,  Path, 1000/2);

	SI16 x = Path[m_siStep*2+0];
	SI16 y = Path[m_siStep*2+1];

	m_siStep+=20;

	SI16 tilex, tiley;
	if(Map.GetRealTileByRealDot(x, y, &tilex, &tiley)==TRUE)
	{
		CalculateHit(m_MagicID, m_siMagicPlayerNumber , ATTACKATB_ANTIARMOUR, 2, AREA_LAND, m_siApower, m_AttackID, tilex, tiley, m_siHitRate);
	}

	if(m_siStep >= Length || m_siStep > 700)
	{
		return TRUE;
	}


	SI16 id = FindEmptySmoke();
	if(id)
	{
		SetSmoke(KIND_SMOKE_BIGTREE, id, x, y, AREA_LAND);
		
	}

	return 0;
}

_MagicManaBurn::_MagicManaBurn(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	if(CR[attackid.ID].m_ParmeterBox.IP.GetMana() >= 300)
		CR[attackid.ID].ReduceMana(300);
	else 
		CR[attackid.ID].ReduceMana(CR[attackid.ID].m_ParmeterBox.IP.GetMana());
}

SI32 _MagicManaBurn::Action()
{
	return 0;
}

_MagicDoubleSword::_MagicDoubleSword(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;
	m_siSmokeID = FindEmptySmoke();

	if(GD.IsInBlackFog(siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicDoubleSword::Action()
{
	SI32 i, j;
	short TileX, TileY;
	SI32 x, y;
	
	if(GD.Frame % 3 )	return FALSE;

	if(m_siStep == 0)
		SetSmoke(KIND_SMOKE_DOUBLE_SWORD, m_siSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], m_siArea);

	m_siStep++;

	if(IsLiveSmoke(m_siSmokeID) == FALSE)
		return TRUE;

	if(IsAlive(m_MagicID) == FALSE)		
	{
		DeleteSmoke(m_siSmokeID);
		return TRUE;
	}

	SetSmokeRealXY(m_siSmokeID, CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY());

	if(GD.Random() % 8 != 0)	return FALSE;

//	clifMap.GetRealTileByRealDot(CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY(), &TileX, &TileY);
	// robypark 2004/8/4 17:35
	// 캐릭터보다 아래에 있는 적 유닛에 대한 스플래쉬 존이 제대로 적용안되는 문제 해결
	TileX = CR[m_MagicID.ID].m_ssX;
	TileY = CR[m_MagicID.ID].m_ssY;

	for(i = -2; i <= 2; i++)
	{
		for(j = -2; j <= 2; j++)
		{
			x = TileX + j;
			y = TileY + i;
			
			if(Map.IsInMapArea(x, y) == FALSE)continue;

			SI32 temp_id = Map.GetIDMap(x, y);
			
			// 유닛이 적이면 타격을 준다.
			if(IsSee(temp_id) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, clifChar.GetPlayerNumber(temp_id)) == FALSE)
				CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_MAGIC, 0, m_siArea, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
		}
	}
	return FALSE;
}

_MagicLongSpear::_MagicLongSpear(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicLongSpear::Action()
{
	return TRUE;
}

_MagicIceBarrier::_MagicIceBarrier(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	SI32 i;
	short CharX, CharY;

	m_siStep = 0;

	// 빙석벽을 세팅한다.
	clifMap.GetRealTileByRealDot(CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY(), &CharX, &CharY);
	SI32 DifX = abs(CharX-m_siX);
	SI32 DifY = abs(CharY-m_siY);
	for(i = -4; i <= 4; i++)
	{
		if(DifX >= DifY)
		{
			m_siBlockX[i+4] = m_siX;
			m_siBlockY[i+4] = m_siY + i;
		}
		else
		{
			m_siBlockX[i+4] = m_siX + i;
			m_siBlockY[i+4] = m_siY;
		}

		if(Map.IsInMapArea(m_siBlockX[i+4], m_siBlockY[i+4]) == FALSE)continue;


		// 충돌속성을 세팅한다.
		SI32 temp_id = Map.GetIDMap(m_siBlockX[i+4], m_siBlockY[i+4]);
		if(!temp_id) 
		{
			Map.SetCollMap(m_siBlockX[i+4], m_siBlockY[i+4], BLOCK_ICEWALL);
			Map.SetBarrierCollMap(m_siBlockX[i+4], m_siBlockY[i+4]);
			
			m_siSmokeID = FindEmptySmoke();
			if(m_siSmokeID)
			{
				SetSmoke(KIND_SMOKE_ICE_BARRIER, m_siSmokeID, MapCenterX[m_siBlockX[i+4]][m_siBlockY[i+4]], 
					MapCenterY[m_siBlockX[i+4]][m_siBlockY[i+4]], m_siArea);
			}
		}
	}
}

SI32 _MagicIceBarrier::Action()
{
	if(IsLiveSmoke(m_siSmokeID) == FALSE)
	{
		// 충돌속성을 해제한다.
		for(SI32 i = -4; i <= 4; i++)
		{
			if(Map.IsInMapArea(m_siBlockX[i+4], m_siBlockY[i+4]) == FALSE)
				continue;

			Map.DeleteBarrierCollMap(m_siBlockX[i+4], m_siBlockY[i+4]);

			if(Map.GetBarrierCollMap(m_siBlockX[i+4], m_siBlockY[i+4])==0)
				Map.DeleteCollMap(m_siBlockX[i+4], m_siBlockY[i+4], BLOCK_ICEWALL);
		}
		return TRUE;
	}

	return FALSE;
}


_Magic* pMagic[MAX_MAGIC_NUMBER];


void SetKimBomb(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _KimBomb(SPECIAL_KIMBOMB, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

void SetMagic5j(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _Magic5j(SPECIAL_MAGIC5J, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

void SetMagicExtinguish(SI32 siX, SI32 siY)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _MagicExtinguish(SPECIAL_EXTINGUISH, siX, siY, ZeroID, -1, AREA_LAND, 0, ZeroID);
	}

}


void SetMagicDetectMine(SI32 siX, SI32 siY, SI32 playernumber)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _MagicDetectMine(SPECIAL_DETECTMINE, siX, siY, playernumber);
	}

}


void SetMagic6c(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _Magic6c(SPECIAL_MAGIC6C, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

void SetMagicEnergyPa(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicEnergyPa(SPECIAL_ENERGYPA, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicFireEnergyPa(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicFireEnergyPa(SPECIAL_FIREENERGYPA, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagic4c(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _Magic4c(SPECIAL_MAGIC4C, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

void SetMagicIvory(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicIvory(SPECIAL_IVORY,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicBlueDragon(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicBlueDragon(SPECIAL_BLUEDRAGON,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicFireAttack(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicFireAttack(SPECIAL_FIREATTACK,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicRunningFire(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicRunningFire(SPECIAL_RUNNINGFIRE,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicIce(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicIce(SPECIAL_ICE, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

void SetMagicTest(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _MagicTest(SPECIAL_MAGICTEST, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

// 설녀 눈 폭풍. 
void SetSnowMagic(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _SnowMagic(SPECIAL_SNOWMAGIC, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

// 독무술. 
void SetMagic2T(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();


	if(siID)
	{
		pMagic[siID] = new _Magic2T(SPECIAL_MAGIC2T, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

// 거대수 특수공격. 
void SetBigTree(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _BigTree(SPECIAL_BIGTREE, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicDoubleSword(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicDoubleSword(SPECIAL_DOUBLE_SWORD,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicManaBurn(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicManaBurn(SPECIAL_MANABURN, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicLongSpear(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicLongSpear(SPECIAL_LONG_SPEAR,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void SetMagicIceBarrier(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicIceBarrier(SPECIAL_ICE_BARRIER,siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

void MagicAction()
{
	SI32 i;

	for( i = 0;i < MAX_MAGIC_NUMBER;i++)
	{
		if(pMagic[i])
		{
			
			if(pMagic[i]->Action() == TRUE)
			{
				delete pMagic[i];
				pMagic[i] = NULL;
			}
		}
	}
}


void DeleteMagic()
{
	SI32 i;

	for( i = 0;i < MAX_MAGIC_NUMBER;i++)
	{

		if(pMagic[i])
		{
			delete pMagic[i];
			pMagic[i] = NULL;
		}
	}
}

SI32 FindEmptyMagic()
{
	SI32 i;

	for( i = 1;i < MAX_MAGIC_NUMBER;i++)
	{
		if(pMagic[i] == NULL)return i;
	}

	return 0;
}

// 만천화우
void SetMagicFlowerNeedleRain(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicFlowerNeedleRain(SPECIAL_FLOWER_NEEDLE_RAIN, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

_MagicFlowerNeedleRain::_MagicFlowerNeedleRain(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
	ZeroMemory(m_siSmokesID, sizeof(SI32)*4);
}

SI32 _MagicFlowerNeedleRain::Action()
{
	SI32 x, y;
	SI08 index = -1;

	m_siStep++;

	if(m_siStep >= 180)			return TRUE;

	// 효과이미지를 뿌려준다.
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(i==0 || j==0)
				continue;
			index++;
			if(IsLiveSmoke(m_siSmokesID[index]))
				continue;
			if(GD.Random() % 4 != 0)
				continue;
			x = m_siX + j*2;
			y = m_siY + i*2;
			if(Map.IsInMapArea(x, y) == FALSE)
				continue;
			
			m_siSmokesID[index] = FindEmptySmoke();
			if(m_siSmokesID[index])
			{ 
				switch(GD.Random()%5)
				{
				case 0:
					SetSmoke(KIND_SMOKE_FLOWER_NEEDLE_RAIN1, m_siSmokesID[index], MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
					break;
				case 1:	
					SetSmoke(KIND_SMOKE_FLOWER_NEEDLE_RAIN2, m_siSmokesID[index], MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
					break;	
				case 2:
					SetSmoke(KIND_SMOKE_FLOWER_NEEDLE_RAIN3, m_siSmokesID[index], MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
					break;
				case 3:
					SetSmoke(KIND_SMOKE_FLOWER_NEEDLE_RAIN4, m_siSmokesID[index], MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
					break;
				case 4:
					SetSmoke(KIND_SMOKE_FLOWER_NEEDLE_RAIN5, m_siSmokesID[index], MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
					break;
				}
			}
		}
	}

	if(GD.Random() % 20 != 0)	return FALSE;

	// 실제 타격을 입힌다.
	CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_FLOWERRAIN, 2, m_siArea, m_siApower, m_AttackID, m_siX, m_siY, m_siHitRate);

	return FALSE;
}

// 빙의술
void SetMagicSoulChange(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicSoulChange(SPECIAL_SOUL_CHANGE	, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

_MagicSoulChange::_MagicSoulChange(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	UI16 uiRand = GD.Random() % 75 + 1;
	m_siStep = 0;

	PushEffect(EFFECT_HEAL, m_siX, m_siY);
	CR[newid.ID].SetHealDraw(TRUE);

	if( (CR[attackid.ID].m_ParmeterBox.GetTotalMagicResist() < 20) || (CR[attackid.ID].m_ParmeterBox.GetTotalMagicResist() < uiRand))
	{
		PushEffect(EFFECT_HEAL, siX, siY);
	    CR[attackid.ID].SetHealDraw(TRUE);

		CR[attackid.ID].HostID = newid;
		CR[attackid.ID].m_bChangePlayer = TRUE;

		CR[attackid.ID].m_cltCharStatus.SetPlayerNumber(siplayernumber);
		CR[attackid.ID].m_cltCharStatus.SetOriginalPlayerNumber(siplayernumber);

		CR[attackid.ID].m_siOnlineID = -1;

		OrderStop(newid, BY_COMPUTER);

		if(CR[attackid.ID].CurrentOrder == ORDER_ATTACK)
		{
			OrderStop(attackid,   BY_PLAYER);
		}

		SI16 smokeid;
		smokeid = FindEmptySmoke(); 
		if(smokeid)
		{
			SI32 siWidth = CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().right - CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().left;
			SI32 siHeight = CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().bottom - CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().top;
			SetSmoke(KIND_SMOKE_SOUL_CHANGE_HIT, smokeid, siX+siWidth/3, siY, AREA_LAND);
			Smoke_AttackID(smokeid, m_AttackID);
			Smoke_SetYIndent(KIND_SMOKE_SOUL_CHANGE_HIT,siHeight*2/3);
		}
	}
	// 타격치를 준다.
	CalculateHit(newid, m_siMagicPlayerNumber, ATTACKATB_STONEMAGIC, 1, siarea, siapower, attackid, m_siX, m_siY, sihitrate);
}

SI32 _MagicSoulChange::Action()
{
	return TRUE;
}

// 석괴술
void SetMagicStoneMagic(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicStoneMagic(SPECIAL_STONE_MAGIC, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

_MagicStoneMagic::_MagicStoneMagic(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;
	SI16 HaltId;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}

	// 효과이미지를 뿌려준다.
	SHORT smokeid1=FindEmptySmoke();
	if(smokeid1)
	{
		SetSmoke(KIND_SMOKE_STONE_MAGIC_STONE, smokeid1, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], AREA_LAND);
	}
	SHORT smokeid2=FindEmptySmoke();
	if(smokeid2)
	{
		SetSmoke(KIND_SMOKE_STONE_MAGIC_SMOKE, smokeid2, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], AREA_LAND);
	}

	// 영역내의 캐릭터를 마비시킨다.
	for(int i = -2 ; i <= 2 ; i++)
	{
		for(int j = - 2; j <= 2 ; j++)
		{
			HaltId = Map.GetIDMap(m_siX + i, m_siY + j);

			if(Map.IsInMapArea(m_siX + i, m_siY + j)==FALSE)	continue;

			if(IsAlive(HaltId) && IsFriendToByPlayerNumber(m_siMagicPlayerNumber, CR[HaltId].GetPlayerNumber()) == FALSE)
			{
//				if(GD.CharInfo[HaltId] != KIND_ON_AGILETIGER)
//				{
					OrderStop(CR[HaltId].m_NewID, BY_PLAYER);
					CR[HaltId].SetHalt(HALT_REASON_IVY, 80);
//				}
			}
		}
	}

	// 타격치를 준다.
	CalculateHit(newid, m_siMagicPlayerNumber, ATTACKATB_STONEMAGIC, 1, m_siArea, m_siApower, m_AttackID, m_siX, m_siY, m_siHitRate);
}

SI32 _MagicStoneMagic::Action()
{
	return TRUE;
}

// 신창노도
void SetMagicGodSpear(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicGodSpear(SPECIAL_GOD_SPEAR, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}

}

_MagicGodSpear::_MagicGodSpear(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

SI32 _MagicGodSpear::Action()
{
	return TRUE;
}

// 풍모술
void SetMagicCloudWind(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _MagicCloudWind(SPECIAL_CLOUD_WIND, siX, siY, newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

_MagicCloudWind::_MagicCloudWind(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid) : _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}

	UI16 uiRand = GD.Random() % 75 + 1;

	if( (CR[attackid.ID].m_ParmeterBox.GetTotalMagicResist() < 20) || (CR[attackid.ID].m_ParmeterBox.GetTotalMagicResist() < uiRand))
	{
		m_siSmokeID = FindEmptySmoke();
		if(m_siSmokeID)
		{
			SetSmoke(KIND_SMOKE_CLOUD_WIND_HIT, m_siSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], AREA_LAND);
		}
		CR[attackid.ID].m_CloudWindFrame = 150;
	}
}

SI32 _MagicCloudWind::Action()
{
	if(CR[m_AttackID.ID].m_CloudWindFrame == 0)
	{
		DeleteSmoke(m_siSmokeID);
		return TRUE;
	}
	else
		CR[m_AttackID.ID].m_CloudWindFrame--;

	SetSmokeRealXY(m_siSmokeID, CR[m_AttackID.ID].m_cltCharStatus.GetCenterX(), CR[m_AttackID.ID].m_cltCharStatus.GetCenterY());

	return FALSE;
}

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:25) : 추가 기술 - 장수 2차 전직 특수 기술 [시작]
//----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// 강강수월래(조선-거북차, 이순신 2차 전직 기술) 시전
void SetMagic_FIRE_ROUND_DANCE(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _Magic_FIRE_ROUND_DANCE(SPECIAL_FIRE_ROUND_DANCE, siX, siY, \
										newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

////////////////////////////////////////////////////////////////////////
// 강강수월래(조선-거북차, 이순신 2차 전직 기술) - 클래스 구현
// 생성자
_Magic_FIRE_ROUND_DANCE::_Magic_FIRE_ROUND_DANCE(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid,	\
												 SI32 siplayernumber, SI32 siarea, SI32 siapower,		\
												 SI32 sihitrate, _NewID attackid)						\
	: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_MAGIC4C, m_siX, m_siY);
	}
}

// 마법(기술) 동작 수행
SI32 _Magic_FIRE_ROUND_DANCE::Action()
{
	SI32 i, j;
	short TileX, TileY;
	SI32 x, y;
	
	if (GD.Frame % 2 )
		return FALSE;

	if (m_siStep == 0)
	{
		m_uiBeginTime = timeGetTime();

		// 용 머리 smoke 생성
		m_siSmokeID = FindEmptySmoke();
		SetSmoke(KIND_SMOKE_HEAD_OF_DRAGON, m_siSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], m_siArea);

		// 바닥 smoke 생성
		m_siFloorSmokeID = FindEmptySmoke();
		SetSmoke(KIND_SMOKE_FIRE_ROUND_DANCE_FLOOR, m_siFloorSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], m_siArea, -1, TRUE);
	}

	m_siStep++;

	// 강강수월래는 6초간 시전된다. => 5초로 수정. robypark 2004/11/2 11:31
	if ((timeGetTime() - m_uiBeginTime) >= 5000)
	{
		DeleteSmoke(m_siSmokeID);
		DeleteSmoke(m_siFloorSmokeID);
		return TRUE;
	}

	if(IsAlive(m_MagicID) == FALSE)		
	{
		DeleteSmoke(m_siSmokeID);
		DeleteSmoke(m_siFloorSmokeID);
		return TRUE;
	}

	// 캐릭터 주변에 항상 그림이 그려지도록 수정
	SetSmokeRealXY(m_siSmokeID, CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY());
	SetSmokeRealXY(m_siFloorSmokeID, CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY());

//	clifMap.GetRealTileByRealDot(CR[m_MagicID.ID].m_cltCharStatus.GetCenterX(), CR[m_MagicID.ID].m_cltCharStatus.GetCenterY(), &TileX, &TileY);
	// robypark 2004/8/4 17:35
	// 캐릭터보다 아래에 있는 적 유닛에 대한 스플래쉬 존이 제대로 적용안되는 문제 해결
	TileX = CR[m_MagicID.ID].m_ssX;
	TileY = CR[m_MagicID.ID].m_ssY;

	for(i = -2; i <= 2; i++)
	{
		for(j = -2; j <= 2; j++)
		{
			x = TileX + (j);
			y = TileY + (i);
			
			if(Map.IsInMapArea(x, y) == FALSE)
				continue;

			SI32 temp_id = Map.GetIDMap(x, y);
			
			// 유닛이 적이면 타격을 준다.
//			if(IsSee(temp_id) \
//				&& IsFriendToByPlayerNumber(m_siMagicPlayerNumber, \
//					clifChar.GetPlayerNumber(temp_id)) == FALSE)
			if (IsFriendToByPlayerNumber(
				m_siMagicPlayerNumber, clifChar.GetPlayerNumber(temp_id)) == FALSE)
//			if (IsAlive(temp_id))
			{
				// robypark 2004/6/11 10:35
				// 데미지 속성을 물리공격으로 수정
				CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_THUNDER_VEHICLE, 0,
					m_siArea, m_siApower, clifChar.GetID(temp_id), x, y, m_siHitRate);
			}
/*
			// 캐릭터 주변에 불을 낸다.
			if ((GD.Random() & 127) == 0)
				clifMap.PutFire(x, y, 1, GROUNDFIRE_LIFETIME / 2, m_MagicID, clifChar.GetPlayerNumber(m_MagicID.ID));
*/
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// 격노염폭(일본-지진차, 와키자카 2차 전직 기술) 시전
void SetMagic_FURY_OF_VOLCANO(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _Magic_FURY_OF_VOLCANO(SPECIAL_FURY_OF_VOLCANO, siX, siY, \
										newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

////////////////////////////////////////////////////////////////////////
// 격노염폭(일본-지진차, 와키자카 2차 전직 기술) - 클래스 구현
// 생성자
_Magic_FURY_OF_VOLCANO::_Magic_FURY_OF_VOLCANO(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid,	\
												 SI32 siplayernumber, SI32 siarea, SI32 siapower,		\
												 SI32 sihitrate, _NewID attackid)						\
	: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_uiBeginTime = timeGetTime();
	m_siStep = 0;
}

// 마법(기술) 동작 수행
SI32 _Magic_FURY_OF_VOLCANO::Action()
{
	SI32		i = 0;
	SI32		j = 0;
//	SI32		x, y;
	SI32 id = 0;
	SI32 firex, firey;
	SI32 attackx, attacky;
	SI32 vary;
	SI32 siDamage		 = CalcDamage(SPECIAL_FURY_OF_VOLCANO, m_siApower, 70);

	if (GD.Frame % 2)
		return FALSE;

	// 화산 생성 smoke 생성
	if (m_siStep == 0)
	{
		// 화산 이미지 생성
		m_siSmokeID = FindEmptySmoke();
		SetSmoke(KIND_SMOKE_CREATE_VOLCANO, m_siSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], m_siArea, -1, FALSE);

		// 화산 생성 연기(구름) 생성
		SI32 siDustSmokeID = FindEmptySmoke();
		SetSmoke(KIND_SMOKE_CREATE_VOLCANO_DUST, siDustSmokeID, MapCenterX[m_siX][m_siY], MapCenterY[m_siX][m_siY], m_siArea);		
		
		m_uiBeginTime = timeGetTime();
		m_siStep++;

		// 화면 흔들림을 만든다. 
	    GD.EarthQuake.SetEarthQuake();

		return FALSE;
	}

	if ((m_siStep % 7) == 0)
	{
		// 화면 흔들림을 만든다. 
	    GD.EarthQuake.SetEarthQuake();
	}

	firex = m_siX - 1;
	firey = m_siY - 1;

	// robypark 204/6/12 13:45
	// 화산탄의 시작지점이 맵안에 있지않을 경우 처리
	// 화산탄 시작지점이 맵안에 없다면, 에러가 발생한다.
	if (Map.IsInMapArea(firex, firey) == FALSE)
	{
		firex = m_siX;
		firey = m_siY;
	}

	// 화산탄 생성, 화산 생성 smoke 애니메이션이 끝난 다음에 화산탄을 생성한다
	if (GetSmoke_IsDoneAnimation(m_siSmokeID))
	{
		for (i = 0; i < 2; i++)
		{
			vary = 5 - GD.Random() % 8;

//			if(GD.Random() & 1)
//				vary *= (-1);
			
			attackx = firex + vary + 2;
			
			vary = 5 - GD.Random() % 8;
//			if(GD.Random() & 1)
//				vary *= (-1);
			
			attacky = firey + vary + 2;

			// 화산탄 목표 지점이 맵 안에 있다면,
			if(Map.IsInMapArea(attackx, attacky) == TRUE)
			{
				// 화산탄 생성
				id = FindEmptyBullet();
				if (id)
				{
					SetBullet(KIND_BULLET_VOLCANO_BULLET, id, firex, firey, attackx, attacky, 
								MapCenterX[firex][firey], MapCenterY[firex][firey], 
								MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
								m_MagicID, ZeroID, m_siApower * 0.4, 100, 0, AREA_LAND);

					// robypark 2004/6/12 12:45
					// 화산탄이 화산 앞에(위에) 그려져야 하는가?
					if (attacky > m_siY)
						SetLayerAboveVolcano(id, true);
					else if (attacky == m_siY)
					{
						if (attackx > m_siX)
							SetLayerAboveVolcano(id, true);
					}
				}
			}
		}
	}

	// 화산이 있는 곳에 유닛에게 데미지 주기
	CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_TORNADO, 4, \
			m_siArea, siDamage, ZeroID, \
			m_siX, m_siY, m_siHitRate);

	// 격노염폭은 6초간 지속된다.
	if ((timeGetTime() - m_uiBeginTime) >= 6000)
	{
		DeleteSmoke(m_siSmokeID);

		return TRUE;
	}

	m_siStep++;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술) 시전
void SetMagic_POISONING_HEAVENS(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _Magic_POISONING_HEAVENS(SPECIAL_POISONING_HEAVENS, siX, siY, \
										newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

////////////////////////////////////////////////////////////////////////
// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술) - 클래스 구현
// 생성자
_Magic_POISONING_HEAVENS::_Magic_POISONING_HEAVENS(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid,	\
												 SI32 siplayernumber, SI32 siarea, SI32 siapower,		\
												 SI32 sihitrate, _NewID attackid)						\
	: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 1;

	if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
	{
		PushEffect(EFFECT_KIMBOMB, m_siX, m_siY);
	}
}

// 마법(기술) 동작 수행
SI32 _Magic_POISONING_HEAVENS::Action()
{
	SI32 i;
	SI32 siStepCount = 0 ;
	POINT Point[100];
	SI16 id;

	if(GD.Frame % 3 )
		return FALSE;
	
	// 독안개를 뿌린다.
	if (m_siStep < 6)
	{
		// 원형을 그리면서 기술 시전
		if (Circle(m_siStep, m_siX, m_siY, &siStepCount, Point) == TRUE)
		{
			for(i = 0;i < siStepCount;i++)
			{
				if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
				{
					id = FindEmptySmoke();
					if(id)
					{
						SetSmoke(KIND_SMOKE_FOG_OF_POISON, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
					}
//					else
//						continue;
				}
				else
					continue;

				// 지역 안에 있는 적 캐릭터에 대한 데미지 연산
				SI32 attackid = Map.GetIDMap(Point[i].x, Point[i].y);
				if ((IsAlive(attackid))
						&& (IsFriendToByPlayerNumber(m_siMagicPlayerNumber, CR[attackid].GetPlayerNumber()) == FALSE))
				{
					CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_BLACK_DRAGON_POISON, 0, m_siArea, m_siApower, CR[attackid].m_NewID, (SHORT)Point[i].x, (SHORT)Point[i].y, m_siHitRate);
				}
			}		
		}
	}
	// 냉기를 뿜는다
	else if (m_siStep > 7)
	{
		// 원형을 그리면서 기술 시전
		if(Circle(m_siStep - 7, m_siX, m_siY, &siStepCount, Point) == TRUE)
		{
			for(i = 0;i < siStepCount;i++)
			{
				if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
				{
					id = FindEmptySmoke();
					if(id)
					{
						SetSmoke(KIND_SMOKE_FOG_OF_ICE, id, MapCenterX[Point[i].x][Point[i].y], MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
					}
//					else
//						continue;
				}
				else
					continue;

				// 지역 안에 있는 적 캐릭터에 대한 데미지 연산
				SI32 attackid = Map.GetIDMap(Point[i].x, Point[i].y);
				if ((IsAlive(attackid))
						&& (IsFriendToByPlayerNumber(m_siMagicPlayerNumber, CR[attackid].GetPlayerNumber()) == FALSE))
				{
					CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_BLACK_DRAGON_ICE, 0, m_siArea, m_siApower, CR[attackid].m_NewID, (SHORT)Point[i].x, (SHORT)Point[i].y, m_siHitRate);
				}
			}		
		}
	}
	else
	{
		if(GD.IsInBlackFog(m_siX, m_siY)==FALSE)
		{
			PushEffect(EFFECT_KIMBOMB, m_siX, m_siY);
		}
	}

	m_siStep++;

	// 독안개와 냉기 퍼짐의 효과가 모두 끝났다면,
	// 이 기술(마법)을 종료한다.
	if (m_siStep >= 13)
		return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술) 시전
void SetMagic_BOMB_OF_HEAVEN(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _Magic_BOMB_OF_HEAVEN(SPECIAL_BOMB_OF_HEAVEN, siX, siY, \
										newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

////////////////////////////////////////////////////////////////////////
// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술) - 클래스 구현
// 생성자
_Magic_BOMB_OF_HEAVEN::_Magic_BOMB_OF_HEAVEN(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid,	\
												 SI32 siplayernumber, SI32 siarea, SI32 siapower,		\
												 SI32 sihitrate, _NewID attackid)						\
	: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_siStep = 0;

	// 버섯 구름 효과 생성
	SI32 id = FindEmptySmoke();
	if (id)
	{
		SetSmoke(KIND_SMOKE_MUSHROOM_CLOUD, id, MapCenterX[siX][siY], MapCenterY[siX][siY], AREA_LAND);
	}

	// 큰 폭발음
	if (GD.IsInBlackFog(m_siX, m_siY) == FALSE)
	{
		PushEffect(EFFECT_EXP, m_siX, m_siY);
	}
}

// 마법(기술) 동작 수행
SI32 _Magic_BOMB_OF_HEAVEN::Action()
{
	SI32 i;
	SI32 siStepCount = 0 ;
	POINT Point[100];
	SI16 id;

	if(GD.Frame % 2 )
		return FALSE;
	
	// 작은 폭발
	if ((m_siStep == 4) && (GD.IsInBlackFog(m_siX, m_siY) == FALSE))
	{
		PushEffect(EFFECT_KIMBOMB, m_siX, m_siY);
	}

	if (m_siStep > 3)
	{
		// 원형을 그리면서 기술 시전
		if(Circle(m_siStep - 2, m_siX, m_siY, &siStepCount, Point) == TRUE)
		{
			for(i = 0; i < siStepCount; i++)
			{
				if(Map.IsInMapArea((SHORT)Point[i].x, (SHORT)Point[i].y) == TRUE)
				{
					id = FindEmptySmoke();
					if (id)
					{
						switch(GD.Random() % 3)
						{
							case 0:
								SetSmoke(KIND_SMOKE_BOMB_SMOKE1, id, MapCenterX[Point[i].x][Point[i].y],\
											MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
								break;
							case 1:
								SetSmoke(KIND_SMOKE_BOMB_SMOKE2, id, MapCenterX[Point[i].x][Point[i].y],\
											MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
								break;
							default:
								SetSmoke(KIND_SMOKE_BOMB_SMOKE3, id, MapCenterX[Point[i].x][Point[i].y],\
											MapCenterY[Point[i].x][Point[i].y], AREA_LAND);
								break;
						}
					}

					SI32 attackid = Map.GetIDMap(Point[i].x, Point[i].y);
					if ((IsAlive(attackid)))
//							&& (IsFriendToByPlayerNumber(m_siMagicPlayerNumber,\
//							CR[attackid].GetPlayerNumber()) == FALSE))
					{
						CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_KIMBOMB, 0, \
								m_siArea, m_siApower / (i + 1), CR[attackid].m_NewID, \
								(SHORT)Point[i].x, (SHORT)Point[i].y, m_siHitRate);
					}
				}
			}
		}
	}
	// 중앙 버섯 구름에 위치한 적 캐릭터에게 데미지를 준다.
	// 초반 몇 단계(m_siStep 0, 1 두 단계)에서만 해당된다.
	else if (m_siStep < 2)
	{
//		SI32 attackid = Map.GetIDMap(m_siX, m_siY);
//		if ((IsAlive(attackid)))
//				&& (IsFriendToByPlayerNumber(m_siMagicPlayerNumber,\
//				CR[attackid].GetPlayerNumber()) == FALSE))
//		{
			CalculateHit(m_MagicID, m_siMagicPlayerNumber, ATTACKATB_FIREATTACK, 1, \
					m_siArea, m_siApower, ZeroID, \
//					m_siArea, m_siApower, CR[attackid].NewID, 
					m_siX, m_siY, m_siHitRate);
//		}
	}

	m_siStep++;

	// 폭뢰격이 종료되었다면
	if (m_siStep == 8)
		return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 시전
void SetMagic_FIRERAIN_OF_GOD(SI32 siX, SI32 siY, _NewID newid, SI32 siplayernumber, SI32 siarea, SI32 siapower, SI32 sihitrate, _NewID attackid)
{
	SI32 siID = FindEmptyMagic();

	if(siID)
	{
		pMagic[siID] = new _Magic_FIRERAIN_OF_GOD(SPECIAL_FIRERAIN_OF_GOD, siX, siY, \
										newid, siplayernumber, siarea, siapower, sihitrate, attackid);
	}
}

////////////////////////////////////////////////////////////////////////
// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) - 클래스 구현
// 생성자
_Magic_FIRERAIN_OF_GOD::_Magic_FIRERAIN_OF_GOD(SI32 simagickind, SI32 siX, SI32 siY, _NewID newid,	\
												 SI32 siplayernumber, SI32 siarea, SI32 siapower,		\
												 SI32 sihitrate, _NewID attackid)						\
	: _Magic(simagickind, siX, siY,  newid,  siplayernumber,  siarea,  siapower,  sihitrate, attackid)
{
	m_uiBeginTime = timeGetTime();
}

// 마법(기술) 동작 수행
SI32 _Magic_FIRERAIN_OF_GOD::Action()
{
	SI32 i;

	if(GD.Frame % 3 )
		return FALSE;
	
	// 무작위 좌표를 찾기 위한 변수 설정
	SHORT ScreenXsize = clifMap.GetMaxDrawMapScreenXsize();
	SHORT ScreenYsize = clifMap.GetMaxDrawMapScreenYsize();
	SHORT ScreenHalfXsize = ScreenXsize / 2;
	SHORT ScreenHalfYsize = ScreenYsize / 2;

	// 천벌화시는 4초간 지속된다.
	if ((timeGetTime() - m_uiBeginTime) >= 4000)
	{
		return TRUE;
	}

	// 무작위로 불비를 생성한다.
	for (i = 0; i < 10; i++)
	{
		SI32 siTargetX = m_siX + (ScreenHalfXsize - (GD.Random() % ScreenXsize));
		SI32 siTargetY = m_siY + (ScreenHalfYsize - (GD.Random() % ScreenYsize));

		// 봉황비조 불비 Smoke 생성
		if (clifMap.IsInMapArea(siTargetX, siTargetY))
		{
			SI32 siSmokeID = FindEmptySmoke();

			if (siSmokeID)
			{
				SetSmoke(KIND_SMOKE_FIRERAIN_OF_GOD, siSmokeID, MapCenterX[siTargetX][siTargetY],\
											MapCenterY[siTargetX][siTargetY], AREA_LAND);

				// 불비 시전자를 명시
				Smoke_PlayerNumber(siSmokeID, clifChar.GetPlayerNumber(m_MagicID.ID));
				SetApower(siSmokeID, m_siApower);
				Smoke_AttackerID(siSmokeID, m_MagicID);
			}
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:25) : 추가 기술 - 장수 2차 전직 특수 기술 [끝]
//----------------------------------------------------------------------------
