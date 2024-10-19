//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include <colortable.h>
#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include <etc.h>
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <MyNet-Game.h>

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 
extern	unsigned char				DarkTable[][256];					// ��ο� �ܰ� 
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����
extern	SI32						UserPlayerNumber;					// ����� ���� 
extern	_MainData					GD;									// ������ �ֵ����� 
extern	_MyNetGame			MyNet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �׸��� ���
BOOL _Char::DrawShadow(SHORT darkstep)
{
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		DrawShadowHero();
		return TRUE;
	}

	if( IsAtb(ATB_SHADOW) == FALSE )	return FALSE;


	SHORT ylead=70;
	
	darkstep=max(darkstep, 4);

	SI32 file = m_cltCharDrawInfo.GetFile();
	SI32 font = m_cltCharDrawInfo.GetFont();

	if(m_bReverseSwitch==TRUE)
	{

	   if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY+ylead, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+ylead+ImageYsize-1)==TRUE)
	   {
		   Draw_PutSpriteShadowRT(m_ssDrawStartX, m_ssDrawStartY+ylead, file, font, DarkTable[SHADOW_DARK_STEP]);
	   }
	   else
	   {
		   Draw_PutSpriteShadowCRT(m_ssDrawStartX, m_ssDrawStartY+ylead, file, font, DarkTable[SHADOW_DARK_STEP]);
	   }

   	}
	else
	{
	   if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY+ylead, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+ylead+ImageYsize-1)==TRUE)
	   {
		   Draw_PutSpriteShadowT(m_ssDrawStartX, m_ssDrawStartY+ylead, file, font, DarkTable[SHADOW_DARK_STEP]);
	   }
	   else
	   {
		   Draw_PutSpriteShadowCT(m_ssDrawStartX, m_ssDrawStartY+ylead, file, font, DarkTable[SHADOW_DARK_STEP]);
	   }
    }

	return TRUE;
}

// ������ ĳ����(���ΰ�)�� �׸��ڸ� ��´�.
BOOL _Char::DrawShadowHero()
{
	SHORT ylead=70;
	SI32 siCountA, siFileID, siCharFont;
	SI32 siBodyPart, siTotalPiece;
	SI32 siShadowX, siShadowY, siShadowCX, siShadowCY;
	
	// ȭ�鿡 ���� Body�� ������ ���´�.
	siTotalPiece  =  g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalPiece[AniAction][CharBaseWClass];
	
	for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
	{
		// ���� Bodypart�� ���´�.
		siBodyPart = g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siPartDrawSort[AniAction][CharBaseWClass][DirectionNo][siCountA];
		
		if( g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].bPart[AniAction][CharBaseWClass][siBodyPart] == TRUE )
		{
			siCharFont		=  g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siPartFrame[AniAction][CharBaseWClass][CharWeaponEnum][AniStep_Hero];
			siFileID		=  g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].siFileID[AniAction][siBodyPart];
			//siRender		=  g_FileLoadManager.m_HeroConfig.siRender[AniAction][CharBaseWClass][siBodyPart];
			
			if( siFileID <= 0 )	continue;

			if(!g_FileLoadManager.m_XSpr[siFileID].bLoadComplete)	continue;
	
			// ���� ȭ�鿡 ����� ��Ʈ�� �����Ѵ�.
			siCharFont	+=	AniStartFont;
			
			//-------------------------
			// �׸��� ���
			if(clGrp.GetShadowInfo(m_ssDrawStartX, m_ssDrawStartY,
				g_FileLoadManager.m_XSpr[siFileID].Header.Xsize, g_FileLoadManager.m_XSpr[siFileID].Header.Ysize,
				g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siCenterX[AniAction][CharBaseWClass],
				g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siCenterY[AniAction][CharBaseWClass],
				&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
			{
				// Clipping Area�ȿ� ������ �׳� ��� �ۿ� ������ Clipping �Լ��� ��´�.
				if( clGrp.IsInClipArea(siShadowX, siShadowY, siShadowX + siShadowCX, siShadowY + siShadowCY ) == TRUE )
				{
					clGrp.PutSpriteShadowAT(siShadowX, siShadowY, siShadowCX, siShadowCY,
						g_FileLoadManager.m_XSpr[siFileID].Header.Xsize, g_FileLoadManager.m_XSpr[siFileID].Header.Ysize,
						&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
						m_bReverseSwitch);
				}
				else
				{
					clGrp.PutSpriteShadowACT(siShadowX, siShadowY, siShadowCX, siShadowCY,
						g_FileLoadManager.m_XSpr[siFileID].Header.Xsize, g_FileLoadManager.m_XSpr[siFileID].Header.Ysize,
						&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
						m_bReverseSwitch);
				}
			}
			//
			//-------------------------
		}
	}
	return TRUE;	
}

// �δ� ��ȣ�� ǥ���Ѵ�. 
void _Char::DrawTeamNumber()
{

	if(GetPlayerNumber() !=UserPlayerNumber)return ;
	
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(Group>=0)
	{
		Draw_PutSpriteT(m_ssDrawStartX + rect.left, m_ssDrawStartY + rect.top, FILE_TEAMNUMBER, Group);
	}

}

// ĳ������ ������ ǥ���Ѵ�. 
void _Char::DrawLevel()
{
	SI32 level;
	SI32 startx, starty;
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if( m_ParmeterBox.IP.GetLife() <= 0 ) return;

	// ������ ��쿡�� ������ ǥ������ �ʴ´�. 
	if(ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()] ) == TRUE) return ;
	
	level = m_ParmeterBox.IP.GetLevel();
	
	startx = m_ssDrawStartX + rect.left + ((rect.right - rect.left)-GetFileXsize(FILE_ON_LEVEL))/2;
	starty = m_ssDrawStartY + rect.top  - GetFileYsize(FILE_ON_LEVEL)-2;

	Draw_PutSpriteT(	startx,	starty,	FILE_ON_LEVEL,	0);
	DrawNumber(startx + GetFileXsize(FILE_ON_LEVEL), starty, FILE_ON_LEVEL_NUM, 0, level, 0, TRUE);

}


void _Char::SetHealDraw(SHORT mode)
{
	if(HealDrawSwitch==TRUE)
	{
		if(mode==TRUE)return ;
	}

	HealDrawSwitch=mode;
	HealDrawStep=0;

}



void _Char::SetProtectorDraw(SHORT mode)
{

	if(ProtectorDrawSwitch==TRUE)
	{
		if(mode==TRUE)return ;
	}

	ProtectorDrawSwitch=mode;
	ProtectorDrawStep=0;

}

// ġ��޴� ����� �׷��ش�. 
void _Char::DrawHeal()
{

	if(HealDrawSwitch==FALSE)return ;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	SHORT file=FILE_HEAL;
	SHORT font=0+HealDrawStep;

	SHORT startx=(m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-GetFileXsize(file)/2;
	SHORT starty=(m_ssDrawStartY+rect.top+ (rect.bottom-rect.top)/2)-GetFileYsize(file)/2;

	Draw_PutSpriteT(startx, starty, file, font);

}



// ��ȣ���� �׷��ش�.
void _Char::DrawShield()
{

	if(ProtectorDrawSwitch==FALSE)return ;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();
	SHORT file=FILE_SHIELD;
	SHORT font=0+ProtectorDrawStep;

	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);

	SHORT startx=(m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
	SHORT starty=(m_ssDrawStartY+rect.top +(rect.bottom-rect.top)/2)-ysize/2;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);
	}

}

