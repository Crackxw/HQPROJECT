// OnlineFieldEffect_SiegeWarfare_Bullet.cpp: implementation of the COnlineFieldEffect_SiegeWarfare_Bullet class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineFieldEffect_SiegeWarfare_Bullet.h"
#include "..\Gersang\DirectX.h"
#include "..\Gersang\music.h"
#include <colortable.h>
#include <etc.h>
#include <clGame.h>

extern _clGame*				pGame;

extern	unsigned char		SqrtVary[];
extern	unsigned char		TeamDarkTable[MAX_DARK_STEP][256];
extern	unsigned char		DarkTable[][256];						// 어두운 단계 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_Bullet::COnlineFieldEffect_SiegeWarfare_Bullet()
{
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet));

	// robypark (2004/5/21 18:44) 화룡출수, 발석거 불 붙은 돌 애니메이션 보간을 위한 추가
	//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	//		따라 프레임 스킵이 필요하다.
	ZeroMemory(m_snFrameIndex, sizeof(SHORT) * MAX_SHOOT_RANGE);
}

COnlineFieldEffect_SiegeWarfare_Bullet::~COnlineFieldEffect_SiegeWarfare_Bullet()
{

}

// void COnlineFieldEffect_SiegeWarfare_Bullet::Init(...)
// OnlineMap	*pOnlineMap: 온라인 필드 맵
// SHORT kind: bullet 종류
// SHORT id: Bullet ID
// SHORT sx: Bullet 시작 좌표(X)
// SHORT sy: Bullet 시작 좌표(Y)
// SHORT mx: Bullet 끝 좌표(X)
// SHORT my: Bullet 끝 좌표(Y)
// SHORT x: 공격자의 위치(x)
// SHORT y: 공격자의 위치(y)
// SHORT dx: 목표의 위치(x)
// SHORT dy: 목표의 위치(y)
// SHORT targetdir: 목표물 방향(0)
void COnlineFieldEffect_SiegeWarfare_Bullet::Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT targetdir, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/)
{
	// 메모리 초기화
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet));

	ZeroMemory(m_snFrameIndex, sizeof(SHORT) * MAX_SHOOT_RANGE);

	m_ssKind		 = kind;
    m_ssCurrentOrder = ORDER_BULLET_MOVE;

	m_ssID			 = id;

	// 총알 발사 지점
	pGame->pOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(x, y, (SI32 &)m_ssRealX, (SI32 &)m_ssRealY);

	// 총알 목표 지점
	pGame->pOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(dx, dy, (SI32 &)m_ssRealDX, (SI32 &)m_ssRealDY);
	
	// NOT USED
	m_ssOriginalRealDX=dx;
	m_ssOriginalRealDY=dy;

	// 총알 발사 지점(타일)
	m_ssMapSX = sx;
	m_ssMapSY = sy;

	// 총알 목표 지점(타일)
	m_ssMapDX = mx;
	m_ssMapDY = my;

	// 곡선 진행 총알 좌표
	m_ssCurveBulletRealX	= m_ssRealX;
	m_ssCurveBulletRealY	= m_ssRealY;
	m_ssOldCurveBulletRealX	= m_ssRealX;
	m_ssOldCurveBulletRealY	= m_ssRealY;
	m_ssCurveBulletDrawX	= 0;
	m_ssCurveBulletDrawY	= 0;

	m_siTargetDirection		= targetdir;

	// 곡선 진행 총알의 곡선 정도.
	// 값이 클수록 곡선 정도가 커진다.
	m_ssCurveRate			= 5;

	// 총알 방향 설정
	DecideDirection(m_ssRealX, m_ssRealY, m_ssRealDX, m_ssRealDY);

	// NOT USED
	m_ssX	= 0;
	m_ssY	= 0;

	// 그리기 스위치
	m_ssDrawSwitch=TRUE;

	// 총알 이동 단계
	m_ssMoveStep=0;
	m_ssMaxMoveStep=0;

	// 그릴 때 뒤집어 찍는가?
	m_ssReverseSwitch=FALSE;

	// 총알 속성
	m_ssTransparentSwitch=TRUE;
	m_ssChaseSwitch=FALSE;
	m_ssChaseRepeatNumber=0;

	// 곡선 진행시 사용
	m_ssVaryX=0;
	m_ssVaryY=0;

	// 이미 선언한 총알 속성 값에서 얻어와 현재 총알에 속성 설정
	m_ssStepWidth			= g_BKI[m_ssKind].m_ssStepWidth;			// 이동 속도
	m_ssFile				= g_BKI[m_ssKind].m_ssFile;					// 리소스
	m_ssFont				= g_BKI[m_ssKind].m_ssFont;					// 스프라이트 시작 폰트
	m_ssTailSwitch			= g_BKI[m_ssKind].m_ssTailSwitch;			// 총알 그릴 때 총일 뒤에 연기를 그리는지
	m_ssChaseSwitch			= g_BKI[m_ssKind].m_ssChaseSwitch;			// 유도탄
	m_ssCurveSwitch			= g_BKI[m_ssKind].m_ssCurveSwitch;			// 탄도가 곡선인가?
	m_ssTransparentSwitch	= g_BKI[m_ssKind].m_ssTransparentSwitch;	// 반투명하게 그리는가?
	m_ssHeightSwitch		= g_BKI[m_ssKind].m_ssHeightSwitch;			// 고저에 따라 데미지 변경. NOT USED

	// 총알 이동 코스 설정
	GetCourse();

	// 총알 이미지 셋팅
	SetFileNFont();

	// robypark (2004/5/21 18:44) 화룡출수, 발석거의 불 붙은 돌, 애니메이션 보간을 위한 추가
	//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	//		따라 프레임 스킵이 필요하다.
	switch (m_ssKind)
	{
		// 공성전 궁병 화살
		case KIND_BULLET_SIEGEWARFARE_ARROW:
		// 화룡출수
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// 발석거 기본 공격(불 붙은 돌)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// 발석거 승천포 포탄
		case KIND_BULLET_BOMB_OF_HEAVEN:
			// robypark (2004/5/21 18:44) 애니메이션 보간을 위한 추가
			//		단 한번의 애니메이션이 이루어져야 하며, 거리에
			//		따라 프레임 스킵이 필요하다.
			ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);

			// 거리에 맞게 프레임 조절
			CreateFrameIndex_4FireDragonMissile();
			break;

		default:
			break;
	}

	// 애니메이션 시작 지연 시간 관련 값 초기화
	m_ssCurrentAnimationDelay = 0;
	m_ssAnimationDelay = ssAnimationDelay;

	// robypark 2004/12/20 17:59
	m_uiTargetUniqueID = uiTargetUniqueID;		// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
												// 보내야하는 피격자(피격 마을)의 ID
	m_uiTargetType	   = uiTargetType;			// 피격 대상이 마을인지, 캐릭터인지를 나타낸다.

	// 애니메이션 시작 지연 관련 처리
	if (m_ssAnimationDelay > 0)
	{
		m_ssDrawSwitch = FALSE;	// 애니메이션 시작 지연 시간이 모두 경과될 때 까지 그려지지 않도록 설정
	}
}

// robypark (2004/5/21 18:44) 화룡출수, 발석거의 불 붙은 돌 애니메이션 보간을 위한 추가
//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
//		따라 프레임 스킵이 필요하다.
//  이동 단계에 따른 애니메이션 프레임을 설정한다.
void COnlineFieldEffect_SiegeWarfare_Bullet::CreateFrameIndex_4FireDragonMissile(void)
{
	// 짧은 거리의 경우
	if (m_ssMaxMoveStep <= 2)
	{
		m_snFrameIndex[0] = 0;
		m_snFrameIndex[1] = m_scMoveFrameNumber / 2;
		m_snFrameIndex[2] = m_scMoveFrameNumber - 1;
		return;
	}

	m_snFrameIndex[0] = 0;
	m_snFrameIndex[m_ssMaxMoveStep] = m_scMoveFrameNumber - 1;

	// 한 프레임(폰트) 당 이동 단계 수
	int nStepPerFrame = (((float)m_ssMaxMoveStep / (float)m_scMoveFrameNumber) * 100);

	// 적절한 프레임을 배분해 준다.
	for (SHORT i = 1; i < m_ssMaxMoveStep; i++)
	{
		m_snFrameIndex[i] = (i * 100) / nStepPerFrame;
	}
}

void COnlineFieldEffect_SiegeWarfare_Bullet::SetFileNFont()
{
	int i;

	m_scRealImageXPercent=100;
	m_scRealImageYPercent=100;
	m_ssRealImageStartX=(GetFileXsize(m_ssFile) - GetFileXsize(m_ssFile)*m_scRealImageXPercent/100)/2;
	m_ssRealImageEndX=   GetFileXsize(m_ssFile) - m_ssRealImageStartX;
	m_ssRealImageStartY=(GetFileYsize(m_ssFile) - GetFileYsize(m_ssFile)*m_scRealImageYPercent/100)/2;
	m_ssRealImageEndY=   GetFileYsize(m_ssFile) - m_ssRealImageStartY;

	// 이동용 화일과 폰트 
	m_scMoveFrameNumber = g_BKI[m_ssKind].m_siMoveFrameNumber;


	// 각 방향별로 그림이 준비되어 있다. 
	if(g_BKI[m_ssKind].m_bDirSwitch == TRUE)
	{
		SetMoveImage16(g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont, m_scMoveFrameNumber);
	}
	// 모든 방향이 같은 그림을 사용한다. 
	else
	{
		for(i=0;i<9;i++)
			SetMoveImage(i, g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont);
	}

}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::Hit()
{
/*
	// 고저차에 의한 효과가 있는 총알이면 높이 차이에 따라 10%의 배율로 공격력을 높여준다. 
	if(m_ssHeightSwitch==TRUE)
	{
		// 발사지점의 높이를구한다. 
	    SHORT startlevel = GD.LastLevel[m_ssMapSX][m_ssMapSY];

		// 명중지점의 높이를 구한다. 
		SHORT endlevel = GD.LastLevel[m_ssMapDX][m_ssMapDY];

		// 높이 차에 의한 공격력을 보정해준다. 
		if(startlevel>endlevel)
		{
			m_ssApower=m_ssApower+m_ssApower*(startlevel-endlevel)/10;
		}

	}
*/
	if(g_BKI[m_ssKind].m_siAtb & BULLETATB_HITNORMAL)
	{
		return HitNormal();
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FRAGMENT)
	{
		// 파편
		return HitFragment1();
	}


	return FALSE;

}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::HitFragment1()
{
/*	SHORT id;

	// 명중한 화염을 그린다. 
	id=FindEmptySmoke();

	// 좌표를 구한다. 
	if(clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssMapDX, &m_ssMapDY)==TRUE)
	{
	   if(id)
	   {
		// 물에 떨어지면, 
		if(clifMap.IsColl(m_ssMapDX, m_ssMapDY, BLOCK_DEEPWATER))
		{
			SetSmoke(KIND_SMOKE_WATEREXP, id, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else
		{
		   if(id & 1)
		      SetSmoke(KIND_SMOKE_EXP4, id, m_ssRealX, m_ssRealY, AREA_LAND);
		   else 
		      SetSmoke(KIND_SMOKE_SMALLEXP2, id, m_ssRealX, m_ssRealY, AREA_LAND);
		}
	   }
	}
*/	return TRUE;
}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::HitNormal()
{
	SI32 siRealX, siRealY;
	pGame->pOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(m_ssMapDX, m_ssMapDY, siRealX, siRealY);
	siRealX = siRealY = 0;

	// 시작 ///////////////////////////////////////////////////////////////////////////////
	// 효과음 처리 
	if(g_BKI[m_ssKind].m_siHitEffectSoundIdx)	// 효과음이 있다면
	{
		// 연주. Hash Table 이용한 연주
		HashCodePushEffect(g_BKI[m_ssKind].m_siHitEffectSoundIdx, siRealX, siRealY);
	}
	// 끝 ///////////////////////////////////////////////////////////////////////////////


	// 시작 ///////////////////////////////////////////////////////////////////////////////
	// 명중한 화염을 그린다. 
	// 화룡출수(대만-화룡차, 손유창 2차 전직)에 맞았을 경우
	if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT, m_ssMapDX, m_ssMapDY, 0, 0);
	}		
	// 발석거 공격-불붙은 돌(중국-발석거, 순비연 2차 전직)에 맞았을 경우
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_EXP7, m_ssMapDX, m_ssMapDY, 0, 0);
	}
	// 발석거 공격-불붙은 돌(중국-발석거, 순비연 2차 전직)에 맞았을 경우
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_VOLCANO_BULLET)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_VOLCANO_HIT, m_ssMapDX, m_ssMapDY, 0, 0);
	}
	// 끝///////////////////////////////////////////////////////////////////////////////

	// robypark 2004/11/24 17:42
	// 공성전 발석거 돌 히트 효과
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT)
	{
		SI32 siSmokeIdx = pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT, m_ssMapDX, m_ssMapDY, 0, 0);

		// robypark 2004/12/29 16:20
		// 항상 같은 위치에 히트 smoke를 생성하지 않도록 수정
		if (siSmokeIdx)
		{
			SI08 scDrawPositionInTileX = 0;
			SI08 scDrawPositionInTileY = 0;

			if (TARGET_TYPE_VILLAGE == m_uiTargetType)
			{
				scDrawPositionInTileX = 16 - (rand() % 32);
				scDrawPositionInTileY = 16 - (rand() % 32);
			}
			else if (TARGET_TYPE_CHARACTER == m_uiTargetType)
			{
//				scDrawPositionInTileX = 4 - (rand() % 8);
//				scDrawPositionInTileY = 4 - (rand() % 8);
			}

			pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_DrawPositionRandomizeInTile(siSmokeIdx, scDrawPositionInTileX, scDrawPositionInTileY);
		}
	}

	// robypark 2004/12/6 13:40
	// 공성전 궁병 화살 히트 효과
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT)
	{
		SI32 siSmokeIdx = pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_ARROW_HIT, m_ssMapDX, m_ssMapDY, 0, 0);

		// robypark 2004/12/29 16:20
		// 항상 같은 위치에 히트 smoke를 생성하지 않도록 수정
		if (siSmokeIdx)
		{
			SI08 scDrawPositionInTileX = 0;
			SI08 scDrawPositionInTileY = 0;

			if (TARGET_TYPE_VILLAGE == m_uiTargetType)
			{
				scDrawPositionInTileX = 16 - (rand() % 32);
				scDrawPositionInTileY = 16 - (rand() % 32);
			}
			else if (TARGET_TYPE_CHARACTER == m_uiTargetType)
			{
//				scDrawPositionInTileX = 4 - (rand() % 8);
//				scDrawPositionInTileY = 4 - (rand() % 8);
			}

			pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_DrawPositionRandomizeInTile(siSmokeIdx, scDrawPositionInTileX, scDrawPositionInTileY);
		}
	}

	// robypark 2004/12/20 17:59
	if (m_uiTargetUniqueID)			// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
									// 보내야하는 피격자(피격 마을)의 ID
	{
		// 공격자인 자신이
		// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
		SI16 scGeneralEffect = pGame->pOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect();
		if (-1 != scGeneralEffect)
		{
			// 피격 대상이 캐릭터인지
			if (TARGET_TYPE_CHARACTER == m_uiTargetType)
			{
				if(pGame->pOnlineWorld->pIOnlineChar->IsExistUniqueID(m_uiTargetUniqueID))
					pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
			}
			// 피격 대상이 마을인지
			else if (TARGET_TYPE_VILLAGE == m_uiTargetType)
				pGame->pOnlineWorld->pOnlineClient->SendLongDistanceAttackVillageAction(m_uiTargetUniqueID, scGeneralEffect);
		}
	}

	return TRUE;
}


//
// 가는길을 설정(직선)  .     .     .  -> 이렇게...
//

void COnlineFieldEffect_SiegeWarfare_Bullet::GetCourse()
{
	SHORT x1=m_ssRealX,  y1=m_ssRealY;
	SHORT x2=m_ssRealDX, y2=m_ssRealDY;
	SHORT x, y;
	SHORT delta_x, delta_y, half, error=0;
	SHORT step=0;
	SHORT index=0;

	if(m_ssKind==KIND_BULLET_GODSPEAR)
	{
		if(x2>x1 && y1>y2 || x2>x1 && y1<y2)			// 1, 4사분면
		{
			y2 += (float)(y2-y1) / (x2-x1) * 60;
			x2 += 60;
		}
		else if(x2<x1 && y1>y2 || x2<x1 && y1<y2)		// 2, 3사분면
		{
			y2 -= (float)(y2-y1) / (x2-x1) * 60;
			x2 -= 60;
		}
	}

	if(x1>x2)	delta_x=x1-x2;
	else     	delta_x=x2-x1;

	if(y1>y2)	delta_y=y1-y2;
	else		delta_y=y2-y1;

	m_ssCourseX[step]=x1;
	m_ssCourseY[step]=y1;
	step++;

	if(delta_x>delta_y)
	{
	   y=y1;
	   half=(SHORT)(delta_x/2);

	   if(x1<x2)
	   {
		   for(x=x1+1;x<=x2;x++)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;

			  }
		  }
	   }
	   else
	   {
		   for(x=x1+1;x>=x2;x--)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	}
	else
	{
	   x=x1;
	   half=(SHORT)(delta_y/2);

	   if(y1<y2)
	   {
		  for(y=y1+1;y<=y2;y++)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	   else
	   {
		  for(y=y1+1;y>=y2;y--)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	}

	m_ssMaxMoveStep=step-1;
}


//
// 찍을 위치를 계산한다. 
//
void COnlineFieldEffect_SiegeWarfare_Bullet::CalculateDrawStartXY()
{
	SI32 screenx=0, screeny=0;
	pGame->pOnlineWorld->pOnlineMap->TransformRealDotXYToScreenDotXY(m_ssRealX, m_ssRealY, screenx, screeny);
	m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX;
	m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY;

	if(m_ssCurveSwitch==TRUE)
	{
	   pGame->pOnlineWorld->pOnlineMap->TransformRealDotXYToScreenDotXY(m_ssCurveBulletRealX, m_ssCurveBulletRealY, screenx, screeny);
	   m_ssCurveBulletDrawX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX;
	   m_ssCurveBulletDrawY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY;
	}
}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::CalculateRealXY()
{
	m_ssDrawXsize = GetFileXsize(m_ssFile);
	m_ssDrawYsize = GetFileYsize(m_ssFile);

	// X, Y좌표를 구한다. 
	pGame->pOnlineWorld->pOnlineMap->TransformRealDotXYToMapXY(m_ssRealX, m_ssRealY, m_ssX, m_ssY);

	m_ssCenterX=m_ssRealX;
	m_ssCenterY=m_ssRealY;


	////////////////////////////////////////////	
	// 곡선으로 발사된 총알의 경우 그림자는 직선으로 이동하지만 
	// 총알 자신은 포물선을 그린다. 
	// 위치에 따라서 방향이 계속바뀐다. 

	if(m_ssCurveSwitch==TRUE)
	{
		SHORT incy;
		SHORT vary;

		// 현재의 총알 위치를 구한다. 
		if(m_ssMoveStep<( (m_ssMaxMoveStep) /2))
		{
			vary=m_ssMoveStep*m_ssStepWidth;

			if(vary<MAX_SQRT_RANGE)
				incy=SqrtVary[vary]*m_ssCurveRate;
			else
			{
#ifdef _DEBUG
				SI16 siCharX, siCharY;
				pGame->pOnlineWorld->pIOnlineChar->GetPos(pGame->pOnlineWorld->pMyData->GetMyCharUniqueID(), &siCharX, &siCharY);
				TRACE("1974672-1" "vary:%d, current step:%d, max step:%d, Movespeed:%d\n"
									"Shooter(%d, %d) Target(%d, %d) MyChar(%d, %d)\n\n", 
									vary, m_ssMoveStep, m_ssMaxMoveStep, m_ssStepWidth,
									m_ssMapSX, m_ssMapSY, m_ssMapDX, m_ssMapDY, siCharX, siCharY);
#endif //_DEBUG
				return FALSE;
			}
		}
		else
		{
			vary=(m_ssMaxMoveStep-m_ssMoveStep)*m_ssStepWidth;
			if(vary<MAX_SQRT_RANGE)
				incy=SqrtVary[vary]*m_ssCurveRate;
			else
			{
#ifdef _DEBUG
				SI16 siCharX, siCharY;
				pGame->pOnlineWorld->pIOnlineChar->GetPos(pGame->pOnlineWorld->pMyData->GetMyCharUniqueID(), &siCharX, &siCharY);
				TRACE("1974672-2" "vary:%d, current step:%d, max step:%d, Movespeed:%d\n"
									"Shooter(%d, %d) Target(%d, %d) MyChar(%d, %d)\n\n",
									vary, m_ssMoveStep, m_ssMaxMoveStep, m_ssStepWidth,
									m_ssMapSX, m_ssMapSY, m_ssMapDX, m_ssMapDY, siCharX, siCharY);
#endif //_DEBUG

				return FALSE;
			}

		}
	
		m_ssCurveBulletRealX=m_ssCenterX;
		m_ssCurveBulletRealY=m_ssCenterY-incy;

		if(m_ssKind==KIND_BULLET_FIRETANKARROW)
		{

			switch(m_siTargetDirection)
			{
				case SOUTH|EAST:
					m_siTempDirection=0;
					break;

				case SOUTH:
					m_siTempDirection=9-m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;

				case NORTH:
					m_siTempDirection=18+m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;

				case NORTH|WEST:
					m_siTempDirection=16;
					break;

				case WEST:
					m_siTempDirection=14-m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;

				case EAST:
					m_siTempDirection=22+m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;

				case NORTH|EAST:
					m_siTempDirection=20+m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;

				case SOUTH|WEST:
					m_siTempDirection=12-m_ssMoveStep*8/(m_ssMaxMoveStep);
					break;
			}	// switch(m_siTargetDirection)
		}	// if(m_ssKind==KIND_BULLET_FIRETANKARROW)

		m_ssOldCurveBulletRealX=m_ssCurveBulletRealX;
		m_ssOldCurveBulletRealY=m_ssCurveBulletRealY;
	} 	// if(m_ssCurveSwitch==TRUE)

	return TRUE;
}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::Draw()
{
	// 이미지가 준비되지 않은 것은 그리지 않는다. 
	if(m_ssFile == 0)
		return FALSE;

	unsigned char *darktable;
/*	// 어둔 단계를 설정한다. 
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
*/
	SHORT darkstep = 0;
	darktable = TeamDarkTable[darkstep];

	if(m_ssDrawSwitch==FALSE)
		return FALSE;

	if(m_ssCurveSwitch==TRUE)
	{

		// 그림자 찍기 
        if(m_ssReverseSwitch==FALSE)
		{
		     if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
			 {
				 Draw_PutSpriteShadowT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		     else
			 {
				 Draw_PutSpriteShadowCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }

		}
        else if(m_ssReverseSwitch==TRUE)
		{
		     if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
			 { 
				 Draw_PutSpriteShadowRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		     else
			 {
				 Draw_PutSpriteShadowCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		}

		// 실제 화살 찍기 
		if(m_ssReverseSwitch==FALSE)
		{
//			Draw_PutSpriteT(CurveBulletDrawX, CurveBulletDrawY, File, Font, darktable);
			if(clGrp.IsInClipArea(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssCurveBulletDrawX+GetFileXsize(m_ssFile)-1,
															m_ssCurveBulletDrawY+GetFileYsize(m_ssFile)-1)==TRUE)
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
			else
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinCT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
		}
		else if(m_ssReverseSwitch==TRUE)
		{
//			Draw_PutSpriteRT(CurveBulletDrawX, CurveBulletDrawY, File, Font, darktable);
			if(clGrp.IsInClipArea(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssCurveBulletDrawX+GetFileXsize(m_ssFile)-1,
															m_ssCurveBulletDrawY+GetFileYsize(m_ssFile)-1)==TRUE)
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
			else
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinCRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
		}
	}
	// 곡선 탄도가 아닐때 
	else
	{
             if(m_ssReverseSwitch==FALSE)
			 {
		        if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
		        else
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}

			 }
             else if(m_ssReverseSwitch==TRUE)
			 {
		        if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
				{ 
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
		        else
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
			 }

	}

	return TRUE;
}



//
// 찍어야 할 그림을 선정한다. 
//
void COnlineFieldEffect_SiegeWarfare_Bullet::DecideFont()
{
	switch(m_ssAnimation)
	{
	case ANI_BULLET_MOVE:
		DecideFontMove();
    	break;
	default:
		break;
	}
}

//
// 결정된 방향의 폰트를 불러온다. 
//
void COnlineFieldEffect_SiegeWarfare_Bullet::DecideFontMove()
{
	switch(m_ssKind)
	{
		case KIND_BULLET_FIRETANKARROW:
				DecideFontMoveFireTankArrow();
				break;

		// 공성전 궁병 화살
		case KIND_BULLET_SIEGEWARFARE_ARROW:
		// 화룡출수
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// 발석거 기본 공격(불 붙은 돌)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// 발석거 승천포 포탄
		case KIND_BULLET_BOMB_OF_HEAVEN:
				m_ssAniStep = m_snFrameIndex[m_ssMoveStep];
				DecideFontMoveNormal();
				break;

		default:
				DecideFontMoveNormal();
				break;
	}
}

void COnlineFieldEffect_SiegeWarfare_Bullet::DecideFontMoveNormal()
{
	switch(m_siTempDirection)
	{
		case SOUTH:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2] + m_ssAniStep;
			break;
		case SOUTH|WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[4];
			m_ssFont=m_ssMoveFont[4] + m_ssAniStep;
			break;
		case WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[6];
			m_ssFont=m_ssMoveFont[6]+ m_ssAniStep;
			break;
		case NORTH|WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[8];
			m_ssFont=m_ssMoveFont[8]+ m_ssAniStep;
			break;
		case NORTH:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[6];
			m_ssFont=m_ssMoveFont[6]+ m_ssAniStep;
			break;
		case NORTH|EAST:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[4];
			m_ssFont=m_ssMoveFont[4]+ m_ssAniStep;
			break;
		case EAST:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2]+ m_ssAniStep;
			break;
		case EAST|SOUTH:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[0];
			m_ssFont=m_ssMoveFont[0]+ m_ssAniStep;
			break;
		case SSW:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[3];
			m_ssFont=m_ssMoveFont[3]+ m_ssAniStep;
			break;
		case EEN:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[3];
			m_ssFont=m_ssMoveFont[3]+ m_ssAniStep;
			break;
		case WWS:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[5];
			m_ssFont=m_ssMoveFont[5]+ m_ssAniStep;
			break;
		case NNE:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[5];
			m_ssFont=m_ssMoveFont[5]+ m_ssAniStep;
			break;
		case WWN:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[7];
			m_ssFont=m_ssMoveFont[7]+ m_ssAniStep;
			break;
		case NNW:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[7];
			m_ssFont=m_ssMoveFont[7]+ m_ssAniStep;
			break;
		case SSE:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[1];
			m_ssFont=m_ssMoveFont[1]+ m_ssAniStep;
			break;
		case EES:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[1];
			m_ssFont=m_ssMoveFont[1]+ m_ssAniStep;
			break;
		default:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2]+ m_ssAniStep;

			TRACE("잘못된 총알 폰트가 들어가고 있습니다.(Error Bullet Fonts!)");
			break;
	}
}

void COnlineFieldEffect_SiegeWarfare_Bullet::DecideFontMoveFireTankArrow()
{
	m_ssReverseSwitch=FALSE;
	m_ssFile=m_ssMoveFile[0];
	m_ssFont=m_siTempDirection;
}

//
// 방향을 설정 한다.
//
void COnlineFieldEffect_SiegeWarfare_Bullet::DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	switch(m_ssKind)
	{
	case KIND_BULLET_FIRETANKARROW: DecideDirectionFireTankArrow(x1, y1, x2, y2);  break;
	default: DecideDirectionNormal(x1, y1, x2,  y2); break;
	}
}

void COnlineFieldEffect_SiegeWarfare_Bullet::DecideDirectionFireTankArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	m_siTempDirection=DecideDirection32(x1, y1, x2, y2);
}



void COnlineFieldEffect_SiegeWarfare_Bullet::DecideDirectionNormal(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	SHORT dir=DecideDirection16(x1, y1, x2, y2);
	
	switch(dir)
	{
	     case 0:
			  m_siTempDirection=SOUTH|EAST;
			  break;
	     case 1:
			  m_siTempDirection=SSE;
			  break;
	     case 2:
			  m_siTempDirection=SOUTH;
			  break;
	     case 3:
			  m_siTempDirection=SSW;
			  break;
	     case 4:
			  m_siTempDirection=SOUTH|WEST;
			  break;
	     case 5:
			  m_siTempDirection=WWS;
			  break;
	     case 6:
			  m_siTempDirection=WEST;
			  break;
	     case 7:
			  m_siTempDirection=WWN;
			  break;
	     case 8:
			  m_siTempDirection=WEST|NORTH;
			  break;
	     case 9:
			  m_siTempDirection=NNW;
			  break;
	     case 10:
			  m_siTempDirection=NORTH;
			  break;
	     case 11:
			  m_siTempDirection=NNE;
			  break;
	     case 12:
			  m_siTempDirection=NORTH|EAST;
			  break;
	     case 13:
			  m_siTempDirection=EEN;
			  break;
	     case 14:
			  m_siTempDirection=EAST;
			  break;
	     case 15:
			  m_siTempDirection=EES;
			  break;
	     default:
			  m_siTempDirection=SOUTH|EAST;
			  break;
	}
}

//
// 총알을 이동시켜줌.
//

void COnlineFieldEffect_SiegeWarfare_Bullet::SetMoveImage16(SHORT file, SHORT font, SHORT frame)
{
	SHORT i;

	for(i = 0; i < 9; i++)
		SetMoveImage(i, file, font+frame*i);
}

void COnlineFieldEffect_SiegeWarfare_Bullet::SetMoveImage(SHORT ani, SHORT file, SHORT font)
{
	m_ssMoveFile[ani]=file;
	m_ssMoveFont[ani]=font;
}

// 총알 동작
BOOL COnlineFieldEffect_SiegeWarfare_Bullet::Action()
{
//	SHORT i, j;
//	_NewID id, min_id;
//	SHORT smokex, smokey;
//	SHORT min_length, length;
//	SHORT x, y, effectx, effecty;
//	SHORT smokeid;


   switch(m_ssCurrentOrder)
   {
	   case ORDER_BULLET_MOVE:
			m_ssAnimation=ANI_BULLET_MOVE;

			// 애니메이션 시작 지연 시간이 설정된 경우
			if (m_ssAnimationDelay > m_ssCurrentAnimationDelay)
			{
				// 지연시간이 지날 때 까지 기다린다.
				m_ssCurrentAnimationDelay++;

				return TRUE;
			}

			// 애니메이션 시작 지연 시간이 경과되었거나 설정되지 않은 경우 그려지도록 설정
			m_ssDrawSwitch = TRUE;
			
			if(pGame->pOnlineWorld->GetTotalFrame() % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

		   // 좌표를 결정한다. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// 총알이 목표 지점에 도달
			if(m_ssMoveStep==m_ssMaxMoveStep)
			{
				Hit();
				return FALSE;
			}
			else
			{
				// 꼬리를 만들어야 한다면, 
				if(m_ssTailSwitch==TRUE)
				{
/*
				   SHORT smoke_id=FindEmptySmoke();
				   if(smoke_id)
				   {
					   if(m_ssCurveSwitch==TRUE)
					   {
						   smokex=m_ssCurveBulletRealX;
						   smokey=m_ssCurveBulletRealY;
					   }
					   else
					   {
						   smokex=m_ssRealX;
						   smokey=m_ssRealY;
					   }
				   }
*/
				}

			    m_ssMoveStep++;

			}

		   break;
   }

   if (FALSE == CalculateRealXY())
	   return FALSE;

   DecideFont();

   return TRUE;
}
