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
extern	unsigned char		DarkTable[][256];						// ��ο� �ܰ� 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_Bullet::COnlineFieldEffect_SiegeWarfare_Bullet()
{
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet));

	// robypark (2004/5/21 18:44) ȭ�����, �߼��� �� ���� �� �ִϸ��̼� ������ ���� �߰�
	//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	//		���� ������ ��ŵ�� �ʿ��ϴ�.
	ZeroMemory(m_snFrameIndex, sizeof(SHORT) * MAX_SHOOT_RANGE);
}

COnlineFieldEffect_SiegeWarfare_Bullet::~COnlineFieldEffect_SiegeWarfare_Bullet()
{

}

// void COnlineFieldEffect_SiegeWarfare_Bullet::Init(...)
// OnlineMap	*pOnlineMap: �¶��� �ʵ� ��
// SHORT kind: bullet ����
// SHORT id: Bullet ID
// SHORT sx: Bullet ���� ��ǥ(X)
// SHORT sy: Bullet ���� ��ǥ(Y)
// SHORT mx: Bullet �� ��ǥ(X)
// SHORT my: Bullet �� ��ǥ(Y)
// SHORT x: �������� ��ġ(x)
// SHORT y: �������� ��ġ(y)
// SHORT dx: ��ǥ�� ��ġ(x)
// SHORT dy: ��ǥ�� ��ġ(y)
// SHORT targetdir: ��ǥ�� ����(0)
void COnlineFieldEffect_SiegeWarfare_Bullet::Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT targetdir, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/)
{
	// �޸� �ʱ�ȭ
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet));

	ZeroMemory(m_snFrameIndex, sizeof(SHORT) * MAX_SHOOT_RANGE);

	m_ssKind		 = kind;
    m_ssCurrentOrder = ORDER_BULLET_MOVE;

	m_ssID			 = id;

	// �Ѿ� �߻� ����
	pGame->pOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(x, y, (SI32 &)m_ssRealX, (SI32 &)m_ssRealY);

	// �Ѿ� ��ǥ ����
	pGame->pOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(dx, dy, (SI32 &)m_ssRealDX, (SI32 &)m_ssRealDY);
	
	// NOT USED
	m_ssOriginalRealDX=dx;
	m_ssOriginalRealDY=dy;

	// �Ѿ� �߻� ����(Ÿ��)
	m_ssMapSX = sx;
	m_ssMapSY = sy;

	// �Ѿ� ��ǥ ����(Ÿ��)
	m_ssMapDX = mx;
	m_ssMapDY = my;

	// � ���� �Ѿ� ��ǥ
	m_ssCurveBulletRealX	= m_ssRealX;
	m_ssCurveBulletRealY	= m_ssRealY;
	m_ssOldCurveBulletRealX	= m_ssRealX;
	m_ssOldCurveBulletRealY	= m_ssRealY;
	m_ssCurveBulletDrawX	= 0;
	m_ssCurveBulletDrawY	= 0;

	m_siTargetDirection		= targetdir;

	// � ���� �Ѿ��� � ����.
	// ���� Ŭ���� � ������ Ŀ����.
	m_ssCurveRate			= 5;

	// �Ѿ� ���� ����
	DecideDirection(m_ssRealX, m_ssRealY, m_ssRealDX, m_ssRealDY);

	// NOT USED
	m_ssX	= 0;
	m_ssY	= 0;

	// �׸��� ����ġ
	m_ssDrawSwitch=TRUE;

	// �Ѿ� �̵� �ܰ�
	m_ssMoveStep=0;
	m_ssMaxMoveStep=0;

	// �׸� �� ������ ��°�?
	m_ssReverseSwitch=FALSE;

	// �Ѿ� �Ӽ�
	m_ssTransparentSwitch=TRUE;
	m_ssChaseSwitch=FALSE;
	m_ssChaseRepeatNumber=0;

	// � ����� ���
	m_ssVaryX=0;
	m_ssVaryY=0;

	// �̹� ������ �Ѿ� �Ӽ� ������ ���� ���� �Ѿ˿� �Ӽ� ����
	m_ssStepWidth			= g_BKI[m_ssKind].m_ssStepWidth;			// �̵� �ӵ�
	m_ssFile				= g_BKI[m_ssKind].m_ssFile;					// ���ҽ�
	m_ssFont				= g_BKI[m_ssKind].m_ssFont;					// ��������Ʈ ���� ��Ʈ
	m_ssTailSwitch			= g_BKI[m_ssKind].m_ssTailSwitch;			// �Ѿ� �׸� �� ���� �ڿ� ���⸦ �׸�����
	m_ssChaseSwitch			= g_BKI[m_ssKind].m_ssChaseSwitch;			// ����ź
	m_ssCurveSwitch			= g_BKI[m_ssKind].m_ssCurveSwitch;			// ź���� ��ΰ�?
	m_ssTransparentSwitch	= g_BKI[m_ssKind].m_ssTransparentSwitch;	// �������ϰ� �׸��°�?
	m_ssHeightSwitch		= g_BKI[m_ssKind].m_ssHeightSwitch;			// ������ ���� ������ ����. NOT USED

	// �Ѿ� �̵� �ڽ� ����
	GetCourse();

	// �Ѿ� �̹��� ����
	SetFileNFont();

	// robypark (2004/5/21 18:44) ȭ�����, �߼����� �� ���� ��, �ִϸ��̼� ������ ���� �߰�
	//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	//		���� ������ ��ŵ�� �ʿ��ϴ�.
	switch (m_ssKind)
	{
		// ������ �ú� ȭ��
		case KIND_BULLET_SIEGEWARFARE_ARROW:
		// ȭ�����
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// �߼��� �⺻ ����(�� ���� ��)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// �߼��� ��õ�� ��ź
		case KIND_BULLET_BOMB_OF_HEAVEN:
			// robypark (2004/5/21 18:44) �ִϸ��̼� ������ ���� �߰�
			//		�� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
			//		���� ������ ��ŵ�� �ʿ��ϴ�.
			ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);

			// �Ÿ��� �°� ������ ����
			CreateFrameIndex_4FireDragonMissile();
			break;

		default:
			break;
	}

	// �ִϸ��̼� ���� ���� �ð� ���� �� �ʱ�ȭ
	m_ssCurrentAnimationDelay = 0;
	m_ssAnimationDelay = ssAnimationDelay;

	// robypark 2004/12/20 17:59
	m_uiTargetUniqueID = uiTargetUniqueID;		// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
												// �������ϴ� �ǰ���(�ǰ� ����)�� ID
	m_uiTargetType	   = uiTargetType;			// �ǰ� ����� ��������, ĳ���������� ��Ÿ����.

	// �ִϸ��̼� ���� ���� ���� ó��
	if (m_ssAnimationDelay > 0)
	{
		m_ssDrawSwitch = FALSE;	// �ִϸ��̼� ���� ���� �ð��� ��� ����� �� ���� �׷����� �ʵ��� ����
	}
}

// robypark (2004/5/21 18:44) ȭ�����, �߼����� �� ���� �� �ִϸ��̼� ������ ���� �߰�
//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
//		���� ������ ��ŵ�� �ʿ��ϴ�.
//  �̵� �ܰ迡 ���� �ִϸ��̼� �������� �����Ѵ�.
void COnlineFieldEffect_SiegeWarfare_Bullet::CreateFrameIndex_4FireDragonMissile(void)
{
	// ª�� �Ÿ��� ���
	if (m_ssMaxMoveStep <= 2)
	{
		m_snFrameIndex[0] = 0;
		m_snFrameIndex[1] = m_scMoveFrameNumber / 2;
		m_snFrameIndex[2] = m_scMoveFrameNumber - 1;
		return;
	}

	m_snFrameIndex[0] = 0;
	m_snFrameIndex[m_ssMaxMoveStep] = m_scMoveFrameNumber - 1;

	// �� ������(��Ʈ) �� �̵� �ܰ� ��
	int nStepPerFrame = (((float)m_ssMaxMoveStep / (float)m_scMoveFrameNumber) * 100);

	// ������ �������� ����� �ش�.
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

	// �̵��� ȭ�ϰ� ��Ʈ 
	m_scMoveFrameNumber = g_BKI[m_ssKind].m_siMoveFrameNumber;


	// �� ���⺰�� �׸��� �غ�Ǿ� �ִ�. 
	if(g_BKI[m_ssKind].m_bDirSwitch == TRUE)
	{
		SetMoveImage16(g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont, m_scMoveFrameNumber);
	}
	// ��� ������ ���� �׸��� ����Ѵ�. 
	else
	{
		for(i=0;i<9;i++)
			SetMoveImage(i, g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont);
	}

}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::Hit()
{
/*
	// �������� ���� ȿ���� �ִ� �Ѿ��̸� ���� ���̿� ���� 10%�� ������ ���ݷ��� �����ش�. 
	if(m_ssHeightSwitch==TRUE)
	{
		// �߻������� ���̸����Ѵ�. 
	    SHORT startlevel = GD.LastLevel[m_ssMapSX][m_ssMapSY];

		// ���������� ���̸� ���Ѵ�. 
		SHORT endlevel = GD.LastLevel[m_ssMapDX][m_ssMapDY];

		// ���� ���� ���� ���ݷ��� �������ش�. 
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
		// ����
		return HitFragment1();
	}


	return FALSE;

}

BOOL COnlineFieldEffect_SiegeWarfare_Bullet::HitFragment1()
{
/*	SHORT id;

	// ������ ȭ���� �׸���. 
	id=FindEmptySmoke();

	// ��ǥ�� ���Ѵ�. 
	if(clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssMapDX, &m_ssMapDY)==TRUE)
	{
	   if(id)
	   {
		// ���� ��������, 
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

	// ���� ///////////////////////////////////////////////////////////////////////////////
	// ȿ���� ó�� 
	if(g_BKI[m_ssKind].m_siHitEffectSoundIdx)	// ȿ������ �ִٸ�
	{
		// ����. Hash Table �̿��� ����
		HashCodePushEffect(g_BKI[m_ssKind].m_siHitEffectSoundIdx, siRealX, siRealY);
	}
	// �� ///////////////////////////////////////////////////////////////////////////////


	// ���� ///////////////////////////////////////////////////////////////////////////////
	// ������ ȭ���� �׸���. 
	// ȭ�����(�븸-ȭ����, ����â 2�� ����)�� �¾��� ���
	if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT, m_ssMapDX, m_ssMapDY, 0, 0);
	}		
	// �߼��� ����-�Һ��� ��(�߱�-�߼���, ���� 2�� ����)�� �¾��� ���
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_EXP7, m_ssMapDX, m_ssMapDY, 0, 0);
	}
	// �߼��� ����-�Һ��� ��(�߱�-�߼���, ���� 2�� ����)�� �¾��� ���
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_VOLCANO_BULLET)
	{
		pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_VOLCANO_HIT, m_ssMapDX, m_ssMapDY, 0, 0);
	}
	// ��///////////////////////////////////////////////////////////////////////////////

	// robypark 2004/11/24 17:42
	// ������ �߼��� �� ��Ʈ ȿ��
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT)
	{
		SI32 siSmokeIdx = pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT, m_ssMapDX, m_ssMapDY, 0, 0);

		// robypark 2004/12/29 16:20
		// �׻� ���� ��ġ�� ��Ʈ smoke�� �������� �ʵ��� ����
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
	// ������ �ú� ȭ�� ��Ʈ ȿ��
	else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT)
	{
		SI32 siSmokeIdx = pGame->pOnlineWorld->m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_ARROW_HIT, m_ssMapDX, m_ssMapDY, 0, 0);

		// robypark 2004/12/29 16:20
		// �׻� ���� ��ġ�� ��Ʈ smoke�� �������� �ʵ��� ����
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
	if (m_uiTargetUniqueID)			// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
									// �������ϴ� �ǰ���(�ǰ� ����)�� ID
	{
		// �������� �ڽ���
		// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
		SI16 scGeneralEffect = pGame->pOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect();
		if (-1 != scGeneralEffect)
		{
			// �ǰ� ����� ĳ��������
			if (TARGET_TYPE_CHARACTER == m_uiTargetType)
			{
				if(pGame->pOnlineWorld->pIOnlineChar->IsExistUniqueID(m_uiTargetUniqueID))
					pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
			}
			// �ǰ� ����� ��������
			else if (TARGET_TYPE_VILLAGE == m_uiTargetType)
				pGame->pOnlineWorld->pOnlineClient->SendLongDistanceAttackVillageAction(m_uiTargetUniqueID, scGeneralEffect);
		}
	}

	return TRUE;
}


//
// ���±��� ����(����)  .     .     .  -> �̷���...
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
		if(x2>x1 && y1>y2 || x2>x1 && y1<y2)			// 1, 4��и�
		{
			y2 += (float)(y2-y1) / (x2-x1) * 60;
			x2 += 60;
		}
		else if(x2<x1 && y1>y2 || x2<x1 && y1<y2)		// 2, 3��и�
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
// ���� ��ġ�� ����Ѵ�. 
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

	// X, Y��ǥ�� ���Ѵ�. 
	pGame->pOnlineWorld->pOnlineMap->TransformRealDotXYToMapXY(m_ssRealX, m_ssRealY, m_ssX, m_ssY);

	m_ssCenterX=m_ssRealX;
	m_ssCenterY=m_ssRealY;


	////////////////////////////////////////////	
	// ����� �߻�� �Ѿ��� ��� �׸��ڴ� �������� �̵������� 
	// �Ѿ� �ڽ��� �������� �׸���. 
	// ��ġ�� ���� ������ ��ӹٲ��. 

	if(m_ssCurveSwitch==TRUE)
	{
		SHORT incy;
		SHORT vary;

		// ������ �Ѿ� ��ġ�� ���Ѵ�. 
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
	// �̹����� �غ���� ���� ���� �׸��� �ʴ´�. 
	if(m_ssFile == 0)
		return FALSE;

	unsigned char *darktable;
/*	// ��� �ܰ踦 �����Ѵ�. 
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
*/
	SHORT darkstep = 0;
	darktable = TeamDarkTable[darkstep];

	if(m_ssDrawSwitch==FALSE)
		return FALSE;

	if(m_ssCurveSwitch==TRUE)
	{

		// �׸��� ��� 
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

		// ���� ȭ�� ��� 
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
	// � ź���� �ƴҶ� 
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
// ���� �� �׸��� �����Ѵ�. 
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
// ������ ������ ��Ʈ�� �ҷ��´�. 
//
void COnlineFieldEffect_SiegeWarfare_Bullet::DecideFontMove()
{
	switch(m_ssKind)
	{
		case KIND_BULLET_FIRETANKARROW:
				DecideFontMoveFireTankArrow();
				break;

		// ������ �ú� ȭ��
		case KIND_BULLET_SIEGEWARFARE_ARROW:
		// ȭ�����
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// �߼��� �⺻ ����(�� ���� ��)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// �߼��� ��õ�� ��ź
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

			TRACE("�߸��� �Ѿ� ��Ʈ�� ���� �ֽ��ϴ�.(Error Bullet Fonts!)");
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
// ������ ���� �Ѵ�.
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
// �Ѿ��� �̵�������.
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

// �Ѿ� ����
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

			// �ִϸ��̼� ���� ���� �ð��� ������ ���
			if (m_ssAnimationDelay > m_ssCurrentAnimationDelay)
			{
				// �����ð��� ���� �� ���� ��ٸ���.
				m_ssCurrentAnimationDelay++;

				return TRUE;
			}

			// �ִϸ��̼� ���� ���� �ð��� ����Ǿ��ų� �������� ���� ��� �׷������� ����
			m_ssDrawSwitch = TRUE;
			
			if(pGame->pOnlineWorld->GetTotalFrame() % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

		   // ��ǥ�� �����Ѵ�. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// �Ѿ��� ��ǥ ������ ����
			if(m_ssMoveStep==m_ssMaxMoveStep)
			{
				Hit();
				return FALSE;
			}
			else
			{
				// ������ ������ �Ѵٸ�, 
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
