#include <GSL.h>

#include <main.h>
#include <clGame.h>


#include "maindata.h"
#include <colortable.h>
#include <etc.h>
#include <item.h>
#include "screenpoint.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include "yprg.h"
#include ".\CharInfo\CharFileLoadManager.h"
#include "help.h"	
#include <Mouse.h>	
#include "text.h"
#include <CharOrg.h>
#include <OnlinePortrait.h>

extern	_clGame				*pGame;
extern	unsigned char			TeamLightTable[];
extern	unsigned char			TeamLight2Table[];
extern	unsigned char			TeamDarkTable[][256];
extern	unsigned char			CTable2[];
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
extern	UI08					WaterShadowTable[][256];			//[bg][spr]
extern	_NewID 					UnderMouseID;
extern	_ScreenPoint 			SP;									// ȭ�鿡 ���� ���� ���� 
extern	SHORT 					PointNumber;						// ���� ���� ���� 
extern	SHORT 					PointArray[];
extern	_MainData				GD;									// ������ �ֵ����� 
extern	SI32					UserPlayerNumber;					// ����� ���� 
extern	_Char					CR[];								// ĳ������ ������ 
extern	char 					SavePath[];
extern	_Order 					Order[];
extern	_Item 					Item[];
extern	_Help 					Help;
extern _InputDevice				IpD;
extern	int 					MapDragSwitch;
extern	SHORT					GameMouseX, GameMouseY;
extern	_Text 					Text;								// �ؽ�Ʈ 

#define INFO_BAR_X_PERCENT 100

#define COLOR_BORDER       246

#ifdef _DEBUG
// robypark 2004/8/4 18:42
// ġƮŰ ���� �÷��� ���� ����
extern bool g_bShowID;	// ĳ���� ID �׸���
extern bool g_bShowArrayIdx;	// ĳ���Ͱ� ��ġ�ϴ� ���� ���� �迭 �ε����� ���̵��� �������ش�.
#endif

// ���콺�� �簢�� �ȿ� �ִ°�?
BOOL IsMouseInRect(SI32 startX, SI32 startY, SI32 endX, SI32 endY)
{
	if( MapDragSwitch == TRUE )	return FALSE;

	if( GameMouseX > startX && GameMouseX < endX && GameMouseY > startY && GameMouseY < endY )
		return TRUE;
	return FALSE;
}




void _Char::DrawSelectedBox(SHORT startx, SHORT starty, SHORT endx, SHORT endy)
{
	int xsize=GetFileXsize(FILE_SELECTBOX);
	int ysize=GetFileYsize(FILE_SELECTBOX);
	int font=0;

	// ����ڸ�, 
	if(UserPlayerNumber==GetPlayerNumber())
	{
		font=0;
	}
	// ���̸�,
	else if(IsFriendToByPlayerNumber(UserPlayerNumber, GetPlayerNumber())==FALSE)
	{
		font=4;
	}
	// �����̸�, 
	else
	{
		font=8;
	}
	
	// left up
	Draw_PutSpriteT(startx, starty, xsize, ysize, FILE_SELECTBOX, font);
	// right up
	Draw_PutSpriteT(endx-xsize, starty, xsize, ysize, FILE_SELECTBOX, font+2);
	// left down
	Draw_PutSpriteT(startx, endy-ysize, xsize, ysize, FILE_SELECTBOX, font+1);
	// right down
	Draw_PutSpriteT(endx-xsize, endy-ysize, xsize, ysize, FILE_SELECTBOX, font+3);
}


BOOL _Char::DrawHero(unsigned char *darktable, SHORT shadowysize)
{
	SI32 siCountA, siFileID, siCharFont;
	SI32 siBodyPart, siTotalPiece;
	SI32 siDrawYsize;
	
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
			
			// ���� ���� ������ �Ϻδ� �׷����� �ʴ´�. (���� ��� �ִ� �κ��� �׸��� �ʴ´�.)
			if(ShallowWaterSwitch==TRUE)
				siDrawYsize		= ImageYsize - shadowysize;
			else
				siDrawYsize		= ImageYsize;

			
			// ���� ȭ�鿡 ����� ��Ʈ�� �����Ѵ�.
			siCharFont	+=	AniStartFont;

			// �¿츦 ����� ��´�. 
			if( m_bReverseSwitch == TRUE )
			{	
				// ���ÿ��� �ȿ� ������, 
				if( clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+ImageYsize-1) == TRUE )
				{
					// �����ϰ� ���� �ϴ� ���̸�, 
					if( m_cltCharDrawInfo.GetTransparentSwitch() == TRUE )
					{
						clGrp.PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]]);
					}
					// ȭ�鿡 ���̰ų� ������� ���̸�, 
					else if(GetSeeStep(UserPlayerNumber) != 0)
					{
						clGrp.PutSpriteLightRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							CTable2, 
							GetSeeStep(UserPlayerNumber));
					}
					else
					{	
						clGrp.PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]], darktable);
							
					}
				}
				else
				{
					// �����ϰ� ���� �ϴ� ���̸�, 
					if(m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
					{
						clGrp.PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]]);
						
					}
					// ȭ�鿡 ���̰ų� ������� ���̸�, 
					else if(GetSeeStep(UserPlayerNumber) != 0)
					{
						clGrp.PutSpriteLightCRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							CTable2, 
							GetSeeStep(UserPlayerNumber));
					}
					else
					{
						clGrp.PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],darktable);
					}
				}
				
				// ���� ��ģ �׸��ڸ� ��´�. 
				if( ShallowWaterSwitch == TRUE )
				{
					if(clGrp.IsInClipArea(m_ssDrawStartX,	m_ssDrawStartY+siDrawYsize, 
						m_ssDrawStartX+ImageXsize-1,	m_ssDrawStartY+siDrawYsize+(ImageYsize - shadowysize)-1)==TRUE)
					{
						clGrp.PutSpriteWaterShadowRT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
							ImageXsize,  (ImageYsize - shadowysize), 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							darktable, WaterShadowTable);
					}
					else
					{
						clGrp.PutSpriteWaterShadowCRT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
							ImageXsize,  (ImageYsize - shadowysize), 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							darktable, WaterShadowTable);
					}
				}
			}
			// �¿� �ٲ��� �ʰ� ���������� ������, 
			else
			{
				// ȭ�� �����ȿ� �ִٸ�, 
				if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+siDrawYsize-1)==TRUE)
				{
					// �����ϰ� ���� �ϴ� ���̸�, 
					if(m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
					{
						clGrp.PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]]);							
					}
					// ȭ�鿡 ���̰ų� ������� ���̸�, 
					else if(GetSeeStep(UserPlayerNumber)!=0)
					{
						clGrp.PutSpriteLightT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							CTable2, 
							GetSeeStep(UserPlayerNumber));
					}
					else
					{
						clGrp.PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]], darktable);
					}
				}
				else
				{
					// �����ϰ� ���� �ϴ� ���̸�, 
					if(m_cltCharDrawInfo.GetTransparentSwitch()==TRUE)
					{
						clGrp.PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]]);
						
					}
					else if(GetSeeStep(UserPlayerNumber)!=0)
					{
						// ȭ�鿡 ���̰ų� ������� ���̸�, 
						clGrp.PutSpriteLightCT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							CTable2, 
							GetSeeStep(UserPlayerNumber));
					}
					else
					{
						clGrp.PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, 
							ImageXsize,  siDrawYsize, 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],darktable);
					}
				}
				// ���� ��ģ �׸��ڸ� ��´�. 
				if( ShallowWaterSwitch == TRUE )
				{
					if(clGrp.IsInClipArea(m_ssDrawStartX,				m_ssDrawStartY+siDrawYsize, 
						m_ssDrawStartX+ImageXsize-1,	m_ssDrawStartY+siDrawYsize+(ImageYsize-shadowysize)-1)==TRUE )
					{
						clGrp.PutSpriteWaterShadowT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
							ImageXsize,  (ImageYsize-shadowysize), 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							darktable, WaterShadowTable);
					}
					else
					{
						clGrp.PutSpriteWaterShadowCT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
							ImageXsize,  (ImageYsize-shadowysize), 
							&g_FileLoadManager.m_XSpr[siFileID].Image[g_FileLoadManager.m_XSpr[siFileID].Header.Start[siCharFont]],
							darktable, WaterShadowTable);
					}
				}
			}	// �¿� �ٲ��� �ʰ� ���������� ������, 
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ĳ���͸� ȭ�鿡 �׷��ش�. 
BOOL _Char::Draw()
{
	SI32 yindent ;
	SI32 siDrawYsize;

	// �׷��ֱⰡ �����Ǿ� ���� ������ �׷��ش�.
	if(m_cltCharDrawInfo.GetDrawSwitch() == FALSE)return FALSE;

	unsigned char *darktable;
	if(m_cltCharDrawInfo.GetDrawFrame() == GD.Frame)return FALSE;

	m_cltCharDrawInfo.SetDrawFrame(GD.Frame);

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();
	SHORT shadowysize = (rect.bottom - rect.top)/3 + KI[m_cltCharStatus.GetKind()].m_ssBottomIndent;

	
	// ���� ��ġ���� ��ο� ������ ���Ѵ�.
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	SI32 file = m_cltCharDrawInfo.GetFile();
	SI32 font = m_cltCharDrawInfo.GetFont();

	// ���� ���� ������ �Ϻδ� �׷����� �ʴ´�. (���� ��� �ִ� �κ��� �׸��� �ʴ´�.)
	if(ShallowWaterSwitch==TRUE)
	{
        siDrawYsize=ImageYsize-shadowysize;
	}
	else
	{
		siDrawYsize=ImageYsize;
	}

	if(GetSoulUse())
		DrawWangSoul();

	// �׸��ڰ� �ִ� ���� �׸��ڸ� �׷��ش�. 
	DrawShadow(darkstep);

	//��ȣ����  �׷��ش�.
	DrawProtector();
	DrawGuardian();

//	DrawHorseUse();
//	DrawRabbitUse();
//	DrawSheepUse();
	


	// ��ü�� ��Ӱ� ��´�.
	if(CurrentOrder==ORDER_DEAD || CurrentOrder==ORDER_CARCASS)
	{
		darkstep=max(4, darkstep);
	}
	else
	{
		darkstep=darkstep;
	}

	// ��� ������ �� ���̸� ǥ�����ش�.
	if(NewSelectedSwitch==TRUE)
	{
		if(NewSelectedDelay%2==0)
		    darktable=TeamLightTable;
		else
			darktable=TeamDarkTable[darkstep];
	}
	else
	{
	   darktable=TeamDarkTable[darkstep];
    }


	// ���õ� ���¸�ǥ���Ѵ�. 
	if(m_scSelectedSwitch==TRUE)
	{

		DrawSelectedBox(m_ssDrawStartX+rect.left,	m_ssDrawStartY+rect.top, 
			            m_ssDrawStartX+rect.right,  m_ssDrawStartY+rect.bottom);
	}

	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		DrawHero(darktable, shadowysize);
	}
	else
	{
		// �¿츦 ����� ��´�. 
		if(m_bReverseSwitch==TRUE)
		{
			
			// ���ÿ��� �ȿ� ������, 
			if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+siDrawYsize-1)==TRUE)
			{
				// �����ϰ� ���� �ϴ� ���̸�, 
				if(m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
				{
					Draw_PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, siDrawYsize, file, font);
				}
				// ȭ�鿡 ���̰ų� ������� ���̸�, 
				else if(GetSeeStep(UserPlayerNumber) != 0)
				{
					Draw_PutSpriteLightRT(m_ssDrawStartX, m_ssDrawStartY,ImageXsize,  siDrawYsize, file,font,CTable2,
						  GetSeeStep(UserPlayerNumber));	
				}
				else
				{
					Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, siDrawYsize, file, font, darktable);	
				}
			}
			else
			{
				// �����ϰ� ���� �ϴ� ���̸�, 
				if(m_cltCharDrawInfo.GetTransparentSwitch()==TRUE)
				{
					Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, siDrawYsize, file, font);
				}
				// ȭ�鿡 ���̰ų� ������� ���̸�, 
				else if(GetSeeStep(UserPlayerNumber) != 0)
				{
					Draw_PutSpriteLightCRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, siDrawYsize, file, font,
							CTable2,
						  GetSeeStep(UserPlayerNumber));	
				}
				else
				{
					Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, siDrawYsize, file, font, darktable);
				}
			}
			
			// ���� ��ģ �׸��ڸ� ��´�. 
			if(ShallowWaterSwitch==TRUE)
			{
				
				
				if(clGrp.IsInClipArea(m_ssDrawStartX,				m_ssDrawStartY+siDrawYsize, 
					m_ssDrawStartX+ImageXsize-1,	m_ssDrawStartY+siDrawYsize+(ImageYsize - shadowysize)-1)==TRUE)
				{
					
					Draw_PutSpriteWaterShadowRT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
						ImageXsize,  (ImageYsize - shadowysize), 
						file, font,
						darktable, WaterShadowTable);
				}
				else
				{
					
					Draw_PutSpriteWaterShadowCRT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
						ImageXsize, (ImageYsize - shadowysize),
						file, font,
						darktable, WaterShadowTable);	
				}
				
			}
			
		}
		// �¿� �ٲ��� �ʰ� ���������� ������, 
		else
		{
			// ȭ�� �����ȿ� �ִٸ�, 
			if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+siDrawYsize-1)==TRUE)
			{
				// �����ϰ� ���� �ϴ� ���̸�, 
				if(m_cltCharDrawInfo.GetTransparentSwitch() == TRUE)
				{
					Draw_PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font);	
				}
				// ȭ�鿡 ���̰ų� ������� ���̸�, 
				else if(GetSeeStep(UserPlayerNumber) != 0)
				{
					Draw_PutSpriteLightT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font,
						CTable2, 
						GetSeeStep(UserPlayerNumber));
				}
				else
				{
					Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font, darktable);
				}
			}
			else
			{
				// �����ϰ� ���� �ϴ� ���̸�, 
				if(m_cltCharDrawInfo.GetTransparentSwitch()==TRUE)
				{
					Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font);	
				}
				else if(GetSeeStep(UserPlayerNumber) != 0)
				{
					// ȭ�鿡 ���̰ų� ������� ���̸�, 
					Draw_PutSpriteLightCT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font,
						CTable2, 
						GetSeeStep(UserPlayerNumber));	
				}
				else
				{
					Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, 
						ImageXsize,  siDrawYsize, file, font, darktable);
				}
			}
			// ���� ��ģ �׸��ڸ� ��´�. 
			if(ShallowWaterSwitch==TRUE)
			{
				
				
				if(clGrp.IsInClipArea(m_ssDrawStartX,	m_ssDrawStartY+siDrawYsize, 
					m_ssDrawStartX+ImageXsize-1,	m_ssDrawStartY+siDrawYsize+(ImageYsize-shadowysize)-1)==TRUE )
				{
					
					Draw_PutSpriteWaterShadowT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
						ImageXsize, (ImageYsize - shadowysize),
						file, font,
						 darktable, WaterShadowTable);	
				}
				else
				{
					
					Draw_PutSpriteWaterShadowCT(m_ssDrawStartX, m_ssDrawStartY+siDrawYsize, 
						ImageXsize, (ImageYsize - shadowysize),
						file, font,
						 darktable, WaterShadowTable);	
				}
			}
		}
	}
	

	// ��ȣ���� �׷��ش�.
//	if(GetSoulUse())
//		DrawWangSoulBottom();

	DrawShield();
	
	DrawHeal();
	DrawStealExp();

	// (�Ż�) ������ ȿ���� �׷��ش�.
	DrawLevelUp();

	
	POINT drawstartpoint;
	drawstartpoint.x = m_ssDrawStartX;
	drawstartpoint.y = m_ssDrawStartY;

	//������ �������� �����ش�.
	m_cltElectronShock.Draw(drawstartpoint, m_cltCharDrawInfo.GetRealImageRect());
	//�ߵ��� �������� �����ش�. 
	m_cltPoisonEffect.Draw(drawstartpoint, m_cltCharDrawInfo.GetRealImageRect());


	// ���ڵ� ������ ��� �� ����� �����ش�. 
	DrawHalt();

	DrawGuardianUse();

	// ��Ÿ �μ����� �׸����� �׷��ش�. 
	DrawEtcInChar(darkstep, darktable);
	
	if(m_scSelectedSwitch==TRUE)
	{
	  if(IsAlive(m_NewID))
	  {
		  // ���õ� ĳ���͸� ������ �ٸ� �����ش�. 
		  yindent = DrawHealthBar();
		  if(yindent != 0)
		  {
			  
			  // �������� �����ش�.
			  yindent = DrawHealBar(yindent);

			  // ���� �������� �����ش�. 
			  yindent = DrawShieldBar(yindent);
			  
 			  // �Ҽӵ� �δ� ��ȣ�� ǥ���Ѵ�.
			  DrawTeamNumber();

  			  // ĳ������ ������ ǥ�����ش�. 
			  DrawLevel();
		  }
	  }
	}
	// ���콺 �Ʒ� ĳ�����̸�, 
	else if(UnderMouseID.IsSame(m_NewID))
	{
	  if(IsAlive(m_NewID))
	  {
		  // ���õ� ĳ���͸� ������ �ٸ� �����ش�. 
		  yindent = DrawHealthBar();
		  if(yindent != 0)
		  {
			  // �������� �����ش�.
			  yindent = DrawHealBar(yindent);

			  // ���� �������� �����ش�. 
			  yindent = DrawShieldBar(yindent);

			  // ĳ������ ������ ǥ�����ش�. 
			  DrawLevel();
		  }
	  }
	}

	// �������� �׸���. 
	if(ElectronSwitch == TRUE  && m_ParmeterBox.IP.GetLife() > 0)	
	{
		// �������� �����. 
		for(SI32 electronnumber = 0;electronnumber < 5;electronnumber++)
		{
			FindRandomStraightLine(ElectronInfo1, ElectronInfo2, ElectronInfo3, ElectronInfo4, PointNumber, PointArray, 10, 10, 20);
			DrawRandomLine(PointNumber, PointArray, FALSE);
		}
	}
	
#ifdef _DEBUG
	// robypark 2004/7/26 17:16
	// ĳ������ ID�� �׸���.
	if( m_ParmeterBox.IP.GetLife() > 0 )
	{
		SI32 _ID = m_NewID.ID;
		SI32 _startx, _starty;
		RECT _rect = m_cltCharDrawInfo.GetRealImageRect();

		_startx = m_ssDrawStartX + _rect.left + ((_rect.right - _rect.left)-GetFileXsize(FILE_ON_LEVEL))/2;
		_starty = m_ssDrawStartY + _rect.top  - GetFileYsize(FILE_ON_LEVEL)-2;

		// ĳ������ ID�� �׸���.
		if (true == g_bShowID)
		{
			DrawNumber(_startx + GetFileXsize(FILE_ON_LEVEL) + 10, _starty + 25, FILE_ON_LEVEL_NUM, 0, _ID, 0, TRUE);
		}

		// ������ �迭���� ��� ��ġ�� �ִ°�? �� �׸���.
		if (true == g_bShowArrayIdx)
		{
			_ID = CharOrderNumber;
			DrawNumber(_startx + GetFileXsize(FILE_ON_LEVEL) + 10, _starty + 40, FILE_ON_LEVEL_NUM, 0, _ID, 0, TRUE);
		}
	}
#endif

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// ü���� �����ش�. 
SI32 _Char::DrawHealthBar()
{
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	SI32 totalxsize = rect.right - rect.left;
	SI32 drawstartx = m_ssDrawStartX + rect.left;
	SI32 unit = 30;
	POINT start;

	start.x = drawstartx;
	start.y = m_ssDrawStartY+rect.top;

	return DrawBoxBar(m_ParmeterBox.IP.GetLife() + m_siGuardianLife, m_ParmeterBox.GetMaxLife() + m_siGuardianMaxLife, unit, start, totalxsize);
}

// ġ����� �����ش�. 
SI32 _Char::DrawHealBar(SI32 yindent)
{

	if(m_ParmeterBox.GetMaxMana() == 0)return yindent;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	SHORT totalxsize = rect.right - rect.left - 1;

	if(m_ParmeterBox.GetMaxMana() <= 0)
	{
		SetMessage("healbar:1000");
		return 0;
	}
	SHORT drawxsize = min(totalxsize, m_ParmeterBox.IP.GetMana()*totalxsize/m_ParmeterBox.GetMaxMana());

	clGrp.BoxC(m_ssDrawStartX + rect.left, m_ssDrawStartY + rect.top + yindent, 
			   m_ssDrawStartX + rect.left + totalxsize, m_ssDrawStartY + rect.top + yindent + 3, clGrp.GetPalette16(17, 17, 17));

	clGrp.FillBoxC(	m_ssDrawStartX + rect.left+1, m_ssDrawStartY + rect.top + yindent + 1, 
					m_ssDrawStartX + rect.left + drawxsize-1, m_ssDrawStartY + rect.top + yindent + 2, clGrp.GetPalette16(22, 28, 37));

	if(drawxsize != totalxsize)
	{
		clGrp.FillBoxC(	m_ssDrawStartX + rect.left + drawxsize-1, m_ssDrawStartY + rect.top + yindent + 1, 
			m_ssDrawStartX + rect.left + totalxsize-1, m_ssDrawStartY + rect.top + yindent + 2, clGrp.GetPalette16(55, 55, 55));
	}

	yindent+=2;

	return yindent;

}

// ���� �������� �����ش�. 
SI32 _Char::DrawShieldBar(SI32 yindent)
{

	
	if(ShieldDelay<=0)return yindent;

	yindent += 2;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	SHORT totalxsize = rect.right - rect.left - 1;

	
	SHORT drawxsize=ShieldDelay*totalxsize/CR[m_NewID.ID].m_ParmeterBox.GetMaxLife();

	clGrp.BoxC(	m_ssDrawStartX + rect.left,		m_ssDrawStartY + rect.top+yindent, 
				m_ssDrawStartX + rect.right,	m_ssDrawStartY + rect.top+yindent +2   , (UI08)COLOR_BORDER);

	clGrp.FillBoxC(	m_ssDrawStartX + rect.left + 1,            m_ssDrawStartY + rect.top + 1  +yindent, 
					m_ssDrawStartX + rect.left + 1+drawxsize-1,m_ssDrawStartY + rect.top + 1  +yindent, 
			 (UI08)GAMECOLOR_YELLOW);

	yindent+=2;

	return yindent;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ʻ�ȭ�� �׸���. 
void _Char::DrawPortrait(LPDIRECTDRAWSURFACE7 surface)
{
	SHORT file, font;

	font = KI[m_cltCharStatus.GetKind()].GetOnlinePortraitFont();
	// actdoll (2004/01/16 18:45) : ��Ʈ�� -1�� �� ��� �� �̹��� ��Ʈ�� ��´�.
	if( font < 0 )		return;
	file	= FILE_ON_PORTRAIT;

	pGame->pOnlineWorld->m_pPortrait->DrawAsNormalEx(surface,726,13,m_cltCharStatus.GetKind(),false,60,60);
	//Draw_PutSpriteT(SP.PortraitStartX, SP.PortraitStartY, file, font);
}


void _Char::SetStealExpDraw(SHORT mode)
{
	if(StealExpDrawSwitch==TRUE)
	{
		if(mode==TRUE)return ;
	}

	StealExpDrawSwitch=mode;
	StealExpDrawStep=0;
}



// ����ġ�� ���ѱ�� ����� �׷��ش�.
void _Char::DrawStealExp()
{

	if(StealExpDrawSwitch==FALSE)return ;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	SHORT file=FILE_STEALEXP;
	SHORT font=0+StealExpDrawStep;

	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);

	SHORT startx=(m_ssDrawStartX + rect.left + (rect.right - rect.left)/2)-xsize/2;
	SHORT starty=(m_ssDrawStartY - ImageYsize/2+ rect.top + (rect.bottom - rect.top)/2)-ysize/2;

	Draw_PutSpriteT(startx, starty, xsize, ysize, file, font);	// hojae_append

}

void _Char::DrawGuardian()
{
	if(m_bGuardianEquip == true)
	{
		if(IsAlive(m_NewID))
		{
			SHORT file;
			SHORT font = 0;

			file= GetGuardianStartImage();

			SHORT xsize=GetFileXsize(file);
			SHORT ysize=GetFileYsize(file);

			RECT rect = m_cltCharDrawInfo.GetRealImageRect();

			font=font+(GD.Frame/3)%14;

			SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
			SHORT starty = m_ssDrawStartY+ImageYsize-KI[m_cltCharStatus.GetKind()].m_ssBottomIndent-ysize;

		//	Draw_PutSpriteT(startx, starty, xsize, ysize, file, font);
			Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
		}
	}

}

BOOL _Char::DrawGuardianUse()
{
	SHORT file = 0;
	SHORT font = 0;
	float size = 0.0;

	if(m_bRabbitAttack == true)
	{
		file = FILE_ON_GUARDIAN_RABBIT_USE;
		font = font+(GD.Frame/3)%8;
		size = 1.1f;
	}
	else if(m_bHorseAttack == true)
	{
		file = FILE_ON_GUARDIAN_HORSE_USE;
		font = font+(GD.Frame/3)%13;
		size = 1.5f;
	}
	else if(m_bGuardianLifeUp == true)
	{
		file = FILE_ON_GUARDIAN_SHEEP_LIFEUP_USE;
		font = font+(GD.Frame/3)%8;
		size = 1.8f;
	}
	else if(m_bDamageResist == true)
	{
		file = FILE_ON_GUARDIAN_SHEEP_MAGICRESIST_USE;
		font = font+(GD.Frame/3)%8;
		size = 1.0f;
	}

	if(file != 0)
	{
		if((TransKindImjinToOnline(CR[m_NewID.ID].m_cltCharStatus.GetKind()) & 255) != 'C')
		{
			if(IsAlive(m_NewID))
			{
				SHORT xsize=GetFileXsize(file);
				SHORT ysize=GetFileYsize(file);

				RECT rect = m_cltCharDrawInfo.GetRealImageRect();

				SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
				SHORT starty = m_ssDrawStartY+ImageYsize-KI[m_cltCharStatus.GetKind()].m_ssBottomIndent-(SHORT)(ysize * size);

				Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
			}
		}
	}
	return TRUE;
}

//BOOL _Char::DrawRabbitUse()
//{
	/*
	if(m_bRabbitAttack == true)
	{
		if((TransKindImjinToOnline(CR[NewID.ID].CharStatus.GetKind()) & 255) != 'C')
		{
			if(IsAlive(NewID))
			{
				SHORT file;
				SHORT font = 0;

				file= FILE_ON_GUARDIAN_RABBIT_USE;

				SHORT xsize=GetFileXsize(file);
				SHORT ysize=GetFileYsize(file);

				RECT rect = CharDrawInfo.GetRealImageRect();

				font=font+(GD.Frame/3)%8;

				SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
				SHORT starty = m_ssDrawStartY+ImageYsize-KI[CharStatus.GetKind()].BottomIndent-ysize;

			//	Draw_PutSpriteT(startx, starty, xsize, ysize, file, font);
				Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
			}
		}
	}*/
//	return TRUE;
//}

//BOOL _Char::DrawHorseUse()
//{
	/*
	if(m_bHorseAttack == true)
	{
		if((TransKindImjinToOnline(CR[NewID.ID].CharStatus.GetKind()) & 255) != 'C')
		{
			if(IsAlive(NewID))
			{
				SHORT file;
				SHORT font = 0;

				file= FILE_ON_GUARDIAN_HORSE_USE;

				SHORT xsize=GetFileXsize(file);
				SHORT ysize=GetFileYsize(file);

				RECT rect = CharDrawInfo.GetRealImageRect();

				font=font+(GD.Frame/3)%13;

				SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
				SHORT starty = m_ssDrawStartY+ImageYsize-KI[CharStatus.GetKind()].BottomIndent-ysize*3/2;

				Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
			}
		}
	}*/
//	return TRUE;
//}

//BOOL _Char::DrawSheepSheepUse()
//{
	/*
	if(m_bSheepLifeUp == true)
	{
		if((TransKindImjinToOnline(CR[NewID.ID].CharStatus.GetKind()) & 255) != 'C')
		{
			if(IsAlive(NewID))
			{
				SHORT file;
				SHORT font = 0;

				file= FILE_ON_GUARDIAN_SHEEP_LIFEUP_USE;

				SHORT xsize=GetFileXsize(file);
				SHORT ysize=GetFileYsize(file);

				RECT rect = CharDrawInfo.GetRealImageRect();

				font=font+(GD.Frame/3)%8;

				SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
				SHORT starty = m_ssDrawStartY+ImageYsize-KI[CharStatus.GetKind()].BottomIndent-ysize*3/2;

				Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
			}
		}
	}*/
//	return TRUE;
//}

// ��ȣ���� �׷��ش�. 
void _Char::DrawProtector()
{

	SHORT file=FILE_PROTECTOR;
	SHORT font;

	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(m_ParmeterBox.AssistApowerByGEffect == 0 && m_ParmeterBox.AssistDpowerByGEffect == 0)return ;

	// ���� �������� ��ȣ���� �׷����� �ʴ´�.
	if(ShallowWaterSwitch==TRUE)return ;


	// ���ݷ°� ������ ��� ���Ǿ� �ִٸ�, 
	if(m_ParmeterBox.AssistApowerByGEffect && m_ParmeterBox.AssistDpowerByGEffect)
	{
		font=0;
	}
	// ���ݷ¸� ���Ǿ� �ִٸ�, 
	else if(m_ParmeterBox.AssistApowerByGEffect)
	{
		font=10;
	}
	// ���¸� ���Ǿ� �ִٸ�, 
	else 
	{
		font=20;
	}
	
/*	// �ΰ� ĳ���ʹ� ����� �۴�. 
	if(IsAtb(ATB_PATIENT))
	{
		font+=30;
	}
*/

	font=font+(GD.Frame/3)%10;

	SHORT startx = (m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
	SHORT starty = m_ssDrawStartY+ImageYsize-KI[m_cltCharStatus.GetKind()].m_ssBottomIndent-ysize/2;

//	Draw_PutSpriteT(startx, starty, xsize, ysize, file, font);
	Draw_PutSpriteLightImjinCT(startx, starty, xsize, ysize, file, font);
}


// (�Ż�) �������� ȿ��
void _Char::DrawLevelUp()
{
	SHORT file, font, startx, starty;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if( LevelUpDrawSwitch == FALSE )	return;

	file = FILE_ON_LEVELUP;
	font = 0 + LevelUpDrawStep;

	startx = (m_ssDrawStartX + rect.left + (rect.right - rect.left)/2) - GetFileXsize(file)/2;
	starty = (m_ssDrawStartY + rect.top  + (rect.bottom- rect.top)/2) - GetFileYsize(file)/2;

	Draw_PutSpriteT(startx, starty, file, font);
}


// (�Ż�) ������ ȿ���� �׷��ִ°�
void _Char::SetLevelUpDraw(SHORT mode)
{
	if( LevelUpDrawSwitch == TRUE )
		if( mode == TRUE )	return; 

	LevelUpDrawSwitch	= mode;
	LevelUpDrawStep		= 0;
}


// ĳ���� ������ �����ش�. 
void _Char::ShowCharInfo(LPDIRECTDRAWSURFACE7 surface) 
{
	HDC hdc;
//	SIZE size;

	if(clGrp.LockSurface(surface)==TRUE)
	{
		// �¶��� ������忡�� ĳ���� ����â�� �����ش�.
		DrawCharInfo();

		DrawPortrait(surface);

		m_cltCharDrawInfo.SetDrawCharInfoStartY(SP.HealthInfoStartY);
		
		// ü���� �����ش�. 
		DrawHealth();
		// �������� �����ش�. 
		DrawMagic();

		clGrp.UnlockSurface(surface);
	}

	//-----------------------------------------------------------------------------------------
	// ������ �̸��� ���ش�. 
	
	char cCharName[256];
	char szLife[256];
	char szMana[256];
	char szApower[256];
	char szDpower[256];

	wsprintf(cCharName, "%s", GetName());
	
	wsprintf(szLife,"HP : %4d/%4d",m_siGuardianLife + m_ParmeterBox.IP.GetLife(),m_siGuardianMaxLife + m_ParmeterBox.GetMaxLife());
	wsprintf(szMana,"MP : %4d/%4d",m_ParmeterBox.IP.GetMana(),m_ParmeterBox.GetMaxMana());
	
	if(m_ParmeterBox.AssistApowerByGEffect)
		wsprintf(szApower,"%s:%d~%d+%d",Text.Apower.Get(),m_ParmeterBox.GetMinApower(),m_ParmeterBox.GetMaxApower(),m_ParmeterBox.AssistApowerByGEffect);
	else
		wsprintf(szApower,"%s:%d~%d",Text.Apower.Get(),m_ParmeterBox.GetMinApower(),m_ParmeterBox.GetMaxApower());

	if(m_ParmeterBox.AssistDpowerByGEffect)
		wsprintf(szDpower,"%s:%d+%d",Text.Dpower.Get(),m_ParmeterBox.GetDpower(),m_ParmeterBox.AssistDpowerByGEffect);
	else
		wsprintf(szDpower,"%s:%d",Text.Dpower.Get(),m_ParmeterBox.GetDpower());

	
	if(surface->GetDC(&hdc) == DD_OK)
	{
//		SelectObject(hdc, pGame->pBattle->VersionInfoPlusFont);
//		SetTextColor(hdc, RGB(200, 200, 200));
//		SetBkMode(hdc, TRANSPARENT);					

		SelectObject(hdc, pGame->pBattle->VersionInfoFont);
		SetBkMode(hdc, TRANSPARENT);

		// actdoll (2004/08/27 19:19) : �ؽ�Ʈ ��� ���� ��ȭ
		// ü��ġ
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, SP.OnlineCharInfoStartX + 19 ,SP.OnlineCharInfoStartY + 7,szLife,lstrlen(szLife));
		SetTextColor(hdc, RGB(250, 250, 250));
		TextOut(hdc, SP.OnlineCharInfoStartX + 18 ,SP.OnlineCharInfoStartY + 6,szLife,lstrlen(szLife));
		// ����ġ
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, SP.OnlineCharInfoStartX + 19 ,SP.OnlineCharInfoStartY + 23,szMana,lstrlen(szMana));
		SetTextColor(hdc, RGB(250, 250, 250));
		TextOut(hdc, SP.OnlineCharInfoStartX + 18 ,SP.OnlineCharInfoStartY + 22,szMana,lstrlen(szMana));
		// ���ݷ�
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, SP.OnlineCharInfoStartX + 7 ,SP.OnlineCharInfoStartY + 41,szApower,lstrlen(szApower));
		SetTextColor(hdc, RGB(250, 250, 250));
		TextOut(hdc, SP.OnlineCharInfoStartX + 6 ,SP.OnlineCharInfoStartY + 40,szApower,lstrlen(szApower));
		// ����
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, SP.OnlineCharInfoStartX + 7 ,SP.OnlineCharInfoStartY + 56,szDpower,lstrlen(szDpower));
		SetTextColor(hdc, RGB(250, 250, 250));
		TextOut(hdc, SP.OnlineCharInfoStartX + 6 ,SP.OnlineCharInfoStartY + 55,szDpower,lstrlen(szDpower));
		// ĳ���͸�

		RECT	rcRectShadow = { 721, 66, 799, 86 }, rcRectMain = { 720, 65, 798, 85 };
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawTextEx( hdc, cCharName, -1, &rcRectShadow, DT_CENTER | DT_SINGLELINE, NULL );
//		TextOut(hdc, 736 ,66,cCharName,lstrlen(cCharName));
		SetTextColor(hdc, RGB(250, 250, 250));
		DrawTextEx( hdc, cCharName, -1, &rcRectMain, DT_CENTER | DT_SINGLELINE, NULL );
//		TextOut(hdc, 735 ,65,cCharName,lstrlen(cCharName));
		
		surface->ReleaseDC(hdc);
	}
	
	//-----------------------------------------------------------------------------------------
	// ĳ���� ������ ���� ���� ǥ��
	/*
	char cText[5][128];
	SI32	i;
	i = 0;
	strcpy(cText[i++], Text.Life.Get());
	strcpy(cText[i++], Text.Mana.Get());
	strcpy(cText[i++], Text.Apower.Get());
	strcpy(cText[i++], Text.Dpower.Get());
	strcpy(cText[i++], Text.MoveSpeed.Get());
	
	for( i=0; i<5; i++ )
	{
		if( IsMouseInRect(SP.HealthInfoStartX-25, SP.HealthInfoStartY-7+SP.CharInfoYInterval*i, clGrp.GetScreenXsize(), SP.HealthInfoStartY+15+SP.CharInfoYInterval*i) == TRUE )
		{
			Help.SetHelp(IpD.Mouse_X, IpD.Mouse_Y, 
				cText[i],
				NULL,
				0,
				0,
				0,
				0,
				0,
				NULL);
		}
	}*/	
}

// ���� ü���� �����ش�. 
void _Char::DrawHealth()
{
	SI32		life, maxlife;

	maxlife = m_ParmeterBox.GetMaxLife();
	life = m_ParmeterBox.IP.GetLife();
	
	if( life > maxlife )	life = maxlife;
	// actdoll (2004/03/12 17:46) : ���׹��� ����Ʈ - �ִ� ü���� ���� ��� ���װ� �߻��� �� �ֱ� ������ maxlife�� ���� ��츸 ǥ�ø� �Ѵ�.
	if( maxlife )	pGame->pOnlineWorld->m_pPortrait->Draw_StatusBar(616+6,6+13,100,5,(((float)life + m_siGuardianLife)/((float)maxlife + m_siGuardianMaxLife)) ,true,true);
}

// �������� �����ش�. 
void _Char::DrawMagic()
{
	SI32		mana, maxmana;

	maxmana = m_ParmeterBox.GetMaxMana();
	mana = m_ParmeterBox.IP.GetMana();
	
	if( mana > maxmana )	mana = maxmana;
	// actdoll (2004/03/12 17:46) : ���׹��� ����Ʈ - �ִ� �������� ���� ��� ���װ� �߻��� �� �ֱ� ������ maxmana�� ���� ��츸 ǥ�ø� �Ѵ�.
	if( maxmana )	pGame->pOnlineWorld->m_pPortrait->Draw_StatusBar(616+6,6+28,100,5,(float)mana/(float)maxmana,false,true);
}


// robypark 2004/10/6 15:28
// ĳ������ ������ ������ �����ش�.
// �ʻ�, ü��, ����
void _Char::ShowCharInfoSimple(LPDIRECTDRAWSURFACE7 lpSurface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
//	HDC hdc;
//	SIZE size;

	// �¶��� ������忡�� ĳ���� ����â�� �����ش�.
	DrawPortrait(lpSurface, siX, siY, siWidth, siHeight);

	// ü���� �����ش�. 
	DrawHealth(siX, siY + siHeight - 8, siWidth, 4);

	// �������� �����ش�. 
	DrawMagic(siX, siY + siHeight - 3, siWidth, 2);

	pGame->pOnlineWorld->m_pPortrait->Draw_HighLight(siX, siY, siWidth, siHeight, 0xAD, 1);
}

// robypark 2004/10/6 17:42
// ��ġ, ũ�� ���� ���
// �ʻ�ȭ�� �׸���. 
void _Char::DrawPortrait(LPDIRECTDRAWSURFACE7 surface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
	SHORT file, font;

	font = KI[m_cltCharStatus.GetKind()].GetOnlineMercenaryFont();
	// actdoll (2004/01/16 18:45) : ��Ʈ�� -1�� �� ��� �� �̹��� ��Ʈ�� ��´�.
	if( font < 0 )		return;
	file	= FILE_ON_PORTRAIT;

	pGame->pOnlineWorld->m_pPortrait->DrawAsNormalEx(surface, siX, siY, m_cltCharStatus.GetKind(),false, siWidth, siHeight);
	//Draw_PutSpriteT(SP.PortraitStartX, SP.PortraitStartY, file, font);
}

// robypark 2004/10/6 17:24
// ���� ü���� �����ش�. 
void _Char::DrawHealth(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
	SI32		life, maxlife;

	maxlife = m_ParmeterBox.GetMaxLife();
	life = m_ParmeterBox.IP.GetLife();
	
	if( life > maxlife )	life = maxlife;
	// actdoll (2004/03/12 17:46) : ���׹��� ����Ʈ - �ִ� ü���� ���� ��� ���װ� �߻��� �� �ֱ� ������ maxlife�� ���� ��츸 ǥ�ø� �Ѵ�.
	if( maxlife )	pGame->pOnlineWorld->m_pPortrait->Draw_StatusBar(siX, siY, siWidth, siHeight,(((float)life + m_siGuardianLife)/((float)maxlife + m_siGuardianMaxLife)) ,true,true);
}

// robypark 2004/10/6 17:24
// �������� �����ش�. 
void _Char::DrawMagic(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
	SI32		mana, maxmana;

	maxmana = m_ParmeterBox.GetMaxMana();
	mana = m_ParmeterBox.IP.GetMana();
	
	if( mana > maxmana )	mana = maxmana;
	// actdoll (2004/03/12 17:46) : ���׹��� ����Ʈ - �ִ� �������� ���� ��� ���װ� �߻��� �� �ֱ� ������ maxmana�� ���� ��츸 ǥ�ø� �Ѵ�.
	if( maxmana )	pGame->pOnlineWorld->m_pPortrait->Draw_StatusBar(siX, siY, siWidth, siHeight, (float)mana/(float)maxmana,false,true);
}

void _Char::DrawEtcInChar(SHORT darkstep, unsigned char* darktable)
{
	SI32 file, font;
	BOOL reverseswitch ;

	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	// ���� ������ ���� �ʿ� ����.
	if(m_ParmeterBox.IP.GetLife() <= 0)return ;

	// ���̰��� ���� ��� �ִϸ��̼�. 
	if(SatJEtcCharAnimationIndex)
	{
		if(CanSelfExp() == TRUE)
		{
			file = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SatJEtcCharAnimationIndex].siFile;
			
			font = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SatJEtcCharAnimationIndex].siFont 
				+ (GD.Frame / KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SatJEtcCharAnimationIndex].siDelay  % KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SatJEtcCharAnimationIndex].GetFrameNumber());

			Draw_PutSpriteLightImjinCT(
				m_ssDrawStartX + ImageXsize / 2 - (ImageXsize / 2) + 3, 
				m_ssDrawStartY + ImageYsize / 2 - (ImageYsize / 2) , 
				file, font);	// hojae_append
		}
	}

	// �������, 
/*	if(Kind == KIND_ON_BOGY)
	{
		if(Animation == ANI_ATTACK)
		{
			file = File;
			font = 160 + Font;
			
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			else
			{
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			
		}

	}
*/

	// �Ű���ȥ�̸�  
	if(m_cltCharStatus.GetKind() == KIND_SINGERMGHOST)
	{
		file = 0; 
		font = 0;

		if(m_cltCharDrawInfo.GetAnimation() == ANI_ATTACK)
		{
			file = GetValueOfGameSprNumID("FILE_SINGERMGHOST3");
			font = m_cltCharDrawInfo.GetFont();		// robypark 2004/9/2 15:16 �Ű��� ȥ ���� ȿ�� ���̵��� ����
		}
		else if(m_cltCharDrawInfo.GetAnimation() == ANI_ATTACK2)
		{
			file = GetValueOfGameSprNumID("FILE_SINGERMGHOST3");
			font = m_cltCharDrawInfo.GetFont();		// robypark 2004/9/2 15:16 �Ű��� ȥ ���� ���� ȿ�� ���̵��� ����
		}

		if(file)
		{
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			else
			{
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
		}
	}

	// ���ϸ�. 
	if(m_cltCharStatus.GetKind() == KIND_ON_ONI)
	{
		if(m_cltCharDrawInfo.GetAnimation() == ANI_ATTACK)
		{
			file = m_cltCharDrawInfo.GetFile();
			font = 156 + m_cltCharDrawInfo.GetFont();
			
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			else
			{
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			
		}

	}

	// ���� 
	if(m_cltCharStatus.GetKind() == KIND_ON_SNOWWOMAN)
	{
		if(m_cltCharDrawInfo.GetAnimation() == ANI_ATTACK)
		{
			file = m_cltCharDrawInfo.GetFile();
			font = 182 + m_cltCharDrawInfo.GetFont();
			
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			else
			{
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, file, font);
			}
			
		}

	}

	if(m_cltCharDrawInfo.GetAnimation() == ANI_MAGIC && MagicEtcCharAnimationIndex)
	{
		file = 0;
		font = 0;
		reverseswitch = FALSE;
		
		KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicEtcCharAnimationIndex].DecideFontAni(Direction, m_cltCharDrawInfo.GetAniStep(), &file, &font, &reverseswitch);
		
		if(reverseswitch==TRUE)
		{
			Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, file, font);
		}
		else
		{
			Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, file, font);
		}
		
	}

	if(EtcCharAnimationIndex)
	{
		file = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[EtcCharAnimationIndex].siFile;
		font = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[EtcCharAnimationIndex].siFont 
			+ GD.Frame / KI[m_cltCharStatus.GetKind()].m_clCharAnimation[EtcCharAnimationIndex].siDelay 
			% KI[m_cltCharStatus.GetKind()].m_clCharAnimation[EtcCharAnimationIndex].GetFrameNumber();
		
		if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+ImageXsize-1, m_ssDrawStartY+ImageYsize-1)==TRUE)
		{
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, ImageYsize, file, font, TeamDarkTable[darkstep]);	
			}
			else
			{
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, ImageYsize, file, font, TeamDarkTable[darkstep]);	
			}
		}
		else
		{
			if(m_bReverseSwitch==TRUE)
			{
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, ImageYsize, file, font, TeamDarkTable[darkstep]);	
			}
			else
			{
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, ImageXsize, ImageYsize, file, font, TeamDarkTable[darkstep]);	
			}
		}

	}

	// �ǰݽ��� ȿ���� ǥ���Ѵ�. (ġ�ᰡ���� ����� ��쿡��.)
	if(m_bBeAttackedDrawSwitch == TRUE && IsAtb(ATB_PATIENT) == TRUE)
	{
		SI32 size = rect.right - rect.left;

		if(size > 100)
		{
			file=FILE_ON_EFFECT_HURT1;  
		}
		else
			file=FILE_ON_EFFECT_HURT2;  

		SI32 drawstartx = (m_ssDrawStartX + ImageXsize / 2) - (GetFileXsize(file) / 2) ;
		SI32 drawstarty = (m_ssDrawStartY + ImageYsize / 2) - (GetFileYsize(file) / 2) ;

		Draw_PutSpriteT(drawstartx, drawstarty, GetFileXsize(file), GetFileYsize(file), 
			       file, m_siBeAttackedDrawStep);

	}
}


///////////////////////////////////////////////////////////////////////////////
// Description	:	�¶��� ������忡�� ĳ���� ����â�� �����ش�.
void _Char::DrawCharInfo()
{
	SI32	siFile;
//	SI32	x, y;
//	SI32	i;

	//-------------------------------------------
	// ĳ���� ����â �ǳ�
	siFile = FILE_ON_CHARINFO;
	Draw_PutSpriteLightT(SP.OnlineCharInfoStartX, SP.OnlineCharInfoStartY, siFile, 0);

	//-------------------------------------------
	// ������(ü��, ������, ���ݷ�, ����, ������)
/*	siFile = FILE_ON_CHARINFO_ICON;
	x = SP.HealthInfoStartX - 29;

	for( i=0; i<5; i++ )
	{
		y = SP.HealthInfoStartY-7 + SP.CharInfoYInterval * i;
		Draw_PutSpriteT(x, y, siFile, i);
	}
*/
}

void _Char::DrawWangSoul()
{
	SHORT file;
	SHORT font;
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();	

	file=GetValueOfGameSprNumID("FILE_ON_WANGSOUL");
	font= m_siFontCount;

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

void _Char::DrawWangSoulBottom()
{
	/*
	SHORT file;
	SHORT font;
	RECT rect = CharDrawInfo.GetRealImageRect();	

	file=GetValueOfGameSprNumID("FILE_ON_WANGSOULBOTTOM");
	font= m_siFontCount;

	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);

	SHORT startx=(m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
	SHORT starty=(m_ssDrawStartY+rect.top +(rect.bottom-rect.top)/2)-ysize/2 + 30;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);
	}
	*/
}