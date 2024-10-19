//--------------------------------------------------------------------
//  담당 : 김태곤
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

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 
extern	unsigned char				DarkTable[][256];					// 어두운 단계 
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터
extern	SI32						UserPlayerNumber;					// 사용자 관련 
extern	_MainData					GD;									// 게임의 주데이터 
extern	_MyNetGame			MyNet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 그림자 찍기
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

// 조합형 캐릭터(주인공)의 그림자를 찍는다.
BOOL _Char::DrawShadowHero()
{
	SHORT ylead=70;
	SI32 siCountA, siFileID, siCharFont;
	SI32 siBodyPart, siTotalPiece;
	SI32 siShadowX, siShadowY, siShadowCX, siShadowCY;
	
	// 화면에 찍을 Body의 갯수를 얻어온다.
	siTotalPiece  =  g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalPiece[AniAction][CharBaseWClass];
	
	for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
	{
		// 찍을 Bodypart를 얻어온다.
		siBodyPart = g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siPartDrawSort[AniAction][CharBaseWClass][DirectionNo][siCountA];
		
		if( g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].bPart[AniAction][CharBaseWClass][siBodyPart] == TRUE )
		{
			siCharFont		=  g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siPartFrame[AniAction][CharBaseWClass][CharWeaponEnum][AniStep_Hero];
			siFileID		=  g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].siFileID[AniAction][siBodyPart];
			//siRender		=  g_FileLoadManager.m_HeroConfig.siRender[AniAction][CharBaseWClass][siBodyPart];
			
			if( siFileID <= 0 )	continue;

			if(!g_FileLoadManager.m_XSpr[siFileID].bLoadComplete)	continue;
	
			// 현재 화면에 출력할 폰트를 설정한다.
			siCharFont	+=	AniStartFont;
			
			//-------------------------
			// 그림자 찍기
			if(clGrp.GetShadowInfo(m_ssDrawStartX, m_ssDrawStartY,
				g_FileLoadManager.m_XSpr[siFileID].Header.Xsize, g_FileLoadManager.m_XSpr[siFileID].Header.Ysize,
				g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siCenterX[AniAction][CharBaseWClass],
				g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siCenterY[AniAction][CharBaseWClass],
				&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
			{
				// Clipping Area안에 있으면 그냥 찍고 밖에 있으면 Clipping 함수로 찍는다.
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

// 부대 번호를 표시한다. 
void _Char::DrawTeamNumber()
{

	if(GetPlayerNumber() !=UserPlayerNumber)return ;
	
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(Group>=0)
	{
		Draw_PutSpriteT(m_ssDrawStartX + rect.left, m_ssDrawStartY + rect.top, FILE_TEAMNUMBER, Group);
	}

}

// 캐릭터의 레벨을 표시한다. 
void _Char::DrawLevel()
{
	SI32 level;
	SI32 startx, starty;
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if( m_ParmeterBox.IP.GetLife() <= 0 ) return;

	// 몬스터인 경우에는 레벨을 표시하지 않는다. 
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

// 치료받는 모습을 그려준다. 
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



// 보호막을 그려준다.
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

