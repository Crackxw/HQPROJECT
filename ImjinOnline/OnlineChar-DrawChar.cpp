//----------------------------------------------------------------------
//  �ʵ忡�� ĳ���� ��� ����. 
/// ���°� 
//----------------------------------------------------------------------
#include <GSL.h>
#include <Main.h>
#include <OnlineWorld.h>

#include "charanimation.h"
#include <kindinfo.h>
#include "..\Gersang\DirectX.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"


#include <colortable.h>

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 
extern unsigned char TeamDarkTable[][256];
extern unsigned char TeamLightTable[];
extern	unsigned char				DarkTable[][256];					// ��ο� �ܰ� 
extern	UI08						WaterShadowTable[][256];			//[bg][spr]

#define DRAW_MODE_NORMAL			1
#define DRAW_MODE_SHADOW			2
#define DRAW_MODE_TRANSPARENT		3	// PutSpriteLightImjinCT() �Լ� �̿�
#define DRAW_MODE_WATERNORMAL		4
#define DRAW_MODE_WATERSHADOW		5
#define	DRAW_MODE_HALF_TRANSPARENT	6	// PutSpriteLightCT() �Լ� �̿� PutSpriteLightImjinCT()�Լ��� �̿��� ��º��� �� ������

// robypark 2005/1/27 13:29
// ���� �������̽� �� ĳ���� �ִϸ��̼� ó��
#define VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT			0		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ��ġ��
#define VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT		8		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ���� ����
#define VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT			16		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ����
#define VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER				8		// ������ �����������̽� ���� ���� �� �� ���ۿ� ���� ĳ���� �ִϸ��̼� ������ ��.

//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���͸� ���
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawChar(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY)
{
	SI32		DrawXSize, DrawYSize;
	SI32		DrawXMaxSize, DrawYMaxSize;
	UI08		TempPB[256];
	SI32		TempCount;
	SI08		scBottomIndent = 5;

	unsigned char *darktable = NULL;

	// �ǳ��� ĳ���Ͷ��
	if (m_bIsRefugee)
	{
		DrawCharRefugee(WaterP, siStartX, siStartY, siCenterX, siCenterY);	// �ǳ��� ĳ���� �׸���
		return;
	}

	// ��� ������ �� ���̸� ǥ�����ش�.
	if(GetNewSelectedFlag() == TRUE)
	{
		if(NewSelectedDelay % 2 == 0) darktable = TeamLightTable;
		else                          darktable = TeamDarkTable[0];
	}
	else
	{
		// robypark 2005/2/17 21:51
		// Ŀ�� �Ʒ� ĳ�����ΰ�?
		if (m_bCursorHoverCharacter)
		{
			darktable = TeamLightTable;
			m_bCursorHoverCharacter = FALSE;	// �ٽ� Ŀ�� �Ʒ� ĳ���� �ʱ�ȭ
		}
		else
			darktable = TeamDarkTable[0];
	}

	// robypark 2005/2/19 10:19
	if ((pMyOnlineWorld->pMyData->IsSiegeWarfareDay())	// ����ڰ� ������ �ְ�,
		&& (m_bIsSiegeWarfareUnit)	// �� ĳ���Ͱ� ���� �����̸�,
		&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != GetUniqueID())	// ����� ������ �ƴϸ�
		&& (GetDataType() == 1))	// �� ĳ������ �� ������ ���ٸ�
	{
		// ĳ���� �� ������ ������ ��û�Ѵ�.
		pMyOnlineWorld->pOnlineClient->SendPlayerDetailInfo(GetUniqueID());
		SetDataType(2);	// ���� ��û ��...
	}

	if(m_ChangeEffectSex != 0)
	{
		// Effect
		XSPR*		lpSprEffect;
		XSPR*		lpSprHero = NULL;

		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEEFFECT);
		switch(m_ChangeEffectSex)
		{
			case 1:
				lpSprHero = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEMAN);
				break;

			case 2:
				lpSprHero = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEWOMAN);
				break;
		}

		clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (lpSprHero->Header.Xsize / 2)
			, siStartY + (m_siYSize / 2) - (lpSprHero->Header.Ysize / 2)
			, lpSprHero->Header.Xsize,   lpSprHero->Header.Ysize,   &lpSprHero->Image[lpSprHero->Header.Start[(m_ChangeEffectFrame / 3) % lpSprHero->Header.font_number]]);
		clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
			, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
			, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_ChangeEffectFrame / 3) % lpSprEffect->Header.font_number]]);
	}
	else
	{
		if(WaterP >= 100)
		{
			if(KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE)
			{
				DrawHero(DRAW_MODE_SHADOW, siStartX, siStartY, darktable, siCenterX, siCenterY);

				// robypark 2004/12/21 18:53
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
					SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

					if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
					else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[
												psprInVillageDefenceInterface->Header.Start[
													scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
											darktable);
					}
					else
						DrawHero(DRAW_MODE_HALF_TRANSPARENT, siStartX, siStartY, darktable, siCenterX, siCenterY);
				}
				else
					DrawHero(DRAW_MODE_NORMAL, siStartX, siStartY, darktable, siCenterX, siCenterY);
			}
			else
			{
				// �׸��� ���
				if(KI[m_siImKind].m_uiAtb & ATB_SHADOW)
				{
					SHORT ylead = 70;
					//darkstep=max(darkstep, 4);
					if(m_bReverse == FALSE)	Draw_PutSpriteShadowCT(siStartX, siStartY + ylead, m_siImFile, m_siFont, DarkTable[SHADOW_DARK_STEP]);
					else                    Draw_PutSpriteShadowCRT(siStartX, siStartY + ylead, m_siImFile, m_siFont, DarkTable[SHADOW_DARK_STEP]);
				}

				if(m_bReverse == FALSE)
				{
					// robypark 2004/12/21 18:53
					// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
						if (psprInVillageDefenceInterface)
						{
							clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
												psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
												&psprInVillageDefenceInterface->Image[
													psprInVillageDefenceInterface->Header.Start[
														scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
												darktable);
						}
						else
							Draw_PutSpriteLightCT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont);
					}
					else
					{
						// �Ʊ�
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// ����
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// ��Ÿ ���
						else
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
				else
				{
					// robypark 2004/12/21 18:53
					// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
						if (psprInVillageDefenceInterface)
						{
							clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
												psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
												&psprInVillageDefenceInterface->Image[
													psprInVillageDefenceInterface->Header.Start[
														scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
												darktable);
						}
						else
							Draw_PutSpriteLightCRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont);
					}
					else
					{
						// �Ʊ�
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// ����
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// ��Ÿ ���
						else
							Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
			}
		}
		else
		{
			DrawXMaxSize = m_siXSize;
			DrawYMaxSize = m_siYSize;

			DrawXSize    = DrawXMaxSize;
			DrawYSize    = (DrawYMaxSize * WaterP) / 100;

			for(TempCount = 0; TempCount < 256; TempCount++)
			{
				TempPB[TempCount] = (UI08)TempCount;
			}


			if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
			{
				DrawHero(DRAW_MODE_WATERSHADOW, siStartX, siStartY, darktable, siCenterX, siCenterY, WaterP);
				
				// robypark 2004/12/20 15:49
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
					SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

					if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
					else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[
												psprInVillageDefenceInterface->Header.Start[
													scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
											darktable);
					}
					else
						DrawHero(DRAW_MODE_HALF_TRANSPARENT, siStartX, siStartY, darktable, siCenterX, siCenterY, WaterP);
				}
				else
					DrawHero(DRAW_MODE_WATERNORMAL, siStartX, siStartY, darktable, siCenterX, siCenterY, WaterP);
			}
			else
			{
				if(m_bReverse == FALSE)
				{
					// robypark 2004/12/20 15:49
					// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
						if (psprInVillageDefenceInterface)
						{
							clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
												psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
												&psprInVillageDefenceInterface->Image[
													psprInVillageDefenceInterface->Header.Start[
														scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
												darktable);
						}
						else
							Draw_PutSpriteLightCT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont);
					}
					else
					{
						// �Ʊ�
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// ����
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// ��Ÿ ���
						else
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
				else
				{
					// robypark 2004/12/20 15:49
					// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// �پ��� ����� ���� ���� ĳ���͸� �׸��� ���� ó��
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
						if (psprInVillageDefenceInterface)
						{
							clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
												psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
												&psprInVillageDefenceInterface->Image[
													psprInVillageDefenceInterface->Header.Start[
														scStartFont + ((GetTickCount() / 100) % VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER)]],
												darktable);
						}
						else
							Draw_PutSpriteLightCRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont);
					}
					else
					{
						// �Ʊ�
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// ����
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// ��Ÿ ���
						else
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
			}
		}
	}

	// robypark 2004/10/29 15:46
	// ������ ���� <==> �Ϲ� ���ΰ� ĳ���� ���� ���� ����Ʈ �׸���
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// ���� ����Ʈ ���ҽ� ���
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// ������ �������� ó�� ���ϴ� ���
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// ������ ������ ���¿��� �����ϴ� ���

		if (lpSprEffect)
		{
			// ���� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectTransformFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/2 19:7
	// ������ ������ ����ǰ ���� ȿ�� �׸���. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	if ((0 != m_siEffectSupplyGoodsType) && (FALSE == m_bIsInVillageDefenceInterface))
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// ����ǰ ������ ����Ʈ ���ҽ� ���
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// ������ ������ ����ǰ�� �޾Ҵ�
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// ������ ������ ����ǰ�� �ٴڳ���.

		if (lpSprEffect)
		{
			// ����ǰ ���� ����Ʈ �׸���
			clGrp.PutSpriteCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectSupplyGoodsFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/10 14:53
	// ������ ������ �������� �޴� �ݰ� ���� �ִϸ��̼� �׸���
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// ������ ������ �������� �޴� �ݰ� ���� �̹��� ���ҽ� ���
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// ������ ������ ���� �ݰ��� �޾Ҵ�. �⸧
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// ������ ������ ���� �ݰ��� �޾Ҵ�. ��

		if (lpSprEffect)
		{
			// ������ ������ �������� �޴� �ݰ� ���� �̹��� �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 25
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectVillageConterAttackFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// ������ ���� �߰��� ��� ����Ʈ �׸���
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect;

		// �߰��� ��� ����Ʈ ���ҽ� ���
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		if (lpSprEffect)
		{
			// �߰��� ��� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectEvasionFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// ������ ���� ġ���� ���� ����Ʈ �׸���
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect;

		// ġ���� ���� ����Ʈ ���ҽ� ���
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		if (lpSprEffect)
		{
			// ġ���� ���� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectDeathblowFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}
} 

// �ǳ��� ĳ���͸� Draw(���ο��� ���� �Լ�)
VOID	cltOnlineChar::DrawCharRefugee(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY)
{
	SI32		DrawXSize, DrawYSize;
	SI32		DrawXMaxSize, DrawYMaxSize;
	UI08		TempPB[256];
	SI32		TempCount;
	SI08		scBottomIndent = 5;

	unsigned char *darktable = NULL;

	// ��� ������ �� ���̸� ǥ�����ش�.
	if(GetNewSelectedFlag() == TRUE)
	{
		if(NewSelectedDelay % 2 == 0) darktable = TeamLightTable;
		else                          darktable = TeamDarkTable[0];
	}
	else
	{
		darktable = TeamDarkTable[0];
	}

	if(m_ChangeEffectSex != 0)
	{
		// Effect
		XSPR*		lpSprEffect;
		XSPR*		lpSprHero = NULL;

		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEEFFECT);
		switch(m_ChangeEffectSex)
		{
			case 1:
				lpSprHero = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEMAN);
				break;

			case 2:
				lpSprHero = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEWOMAN);
				break;
		}

		clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (lpSprHero->Header.Xsize / 2)
			, siStartY + (m_siYSize / 2) - (lpSprHero->Header.Ysize / 2)
			, lpSprHero->Header.Xsize,   lpSprHero->Header.Ysize,   &lpSprHero->Image[lpSprHero->Header.Start[(m_ChangeEffectFrame / 3) % lpSprHero->Header.font_number]]);
		clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
			, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
			, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_ChangeEffectFrame / 3) % lpSprEffect->Header.font_number]]);
	}
	else
	{
		if(WaterP >= 100)
		{
			// �׸��� ���
			if(KI[m_siRefugeeKind].m_uiAtb & ATB_SHADOW)
			{
				SHORT ylead = 70;
				//darkstep=max(darkstep, 4);
				if(m_bReverse == FALSE)	Draw_PutSpriteShadowCT(siStartX, siStartY + ylead, m_siImFile, m_siFont, DarkTable[SHADOW_DARK_STEP]);
				else                    Draw_PutSpriteShadowCRT(siStartX, siStartY + ylead, m_siImFile, m_siFont, DarkTable[SHADOW_DARK_STEP]);
			}

			if(m_bReverse == FALSE)
			{
				// robypark 2004/12/21 18:53
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[psprInVillageDefenceInterface->Header.Start[(GetTickCount() / 100) % psprInVillageDefenceInterface->Header.font_number]],
											darktable);
					}
					else
						Draw_PutSpriteLightCT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont);
				}
				else
					Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
			}
			else
			{
				// robypark 2004/12/21 18:53
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[psprInVillageDefenceInterface->Header.Start[(GetTickCount() / 100) % psprInVillageDefenceInterface->Header.font_number]],
											darktable);
					}
					else
						Draw_PutSpriteLightCRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont);
				}
				else
					Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
			}
		}
		else
		{
			DrawXMaxSize = m_siXSize;
			DrawYMaxSize = m_siYSize;

			DrawXSize    = DrawXMaxSize;
			DrawYSize    = (DrawYMaxSize * WaterP) / 100;

			for(TempCount = 0; TempCount < 256; TempCount++)
			{
				TempPB[TempCount] = (UI08)TempCount;
			}

			if(m_bReverse == FALSE)
			{
				// robypark 2004/12/20 15:49
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[psprInVillageDefenceInterface->Header.Start[(GetTickCount() / 100) % psprInVillageDefenceInterface->Header.font_number]],
											darktable);
					}
					else
						Draw_PutSpriteLightCT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont);
				}
				else
					Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
			}
			else
			{
				// robypark 2004/12/20 15:49
				// ������ �����������̽��� �ִ� ĳ���Ͷ�� �����ϰ� => ���� �������̽� ĳ�����̹��� ���
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// ���� �������̽��� ���� ��� �������� ĳ���� �̹���
					if (psprInVillageDefenceInterface)
					{
						clGrp.PutSpriteT(siStartX + (m_siXSize / 2) - (psprInVillageDefenceInterface->Header.Xsize / 2), siStartY + (m_siYSize / 2) - (psprInVillageDefenceInterface->Header.Ysize / 2) + scBottomIndent,
											psprInVillageDefenceInterface->Header.Xsize, psprInVillageDefenceInterface->Header.Ysize,
											&psprInVillageDefenceInterface->Image[psprInVillageDefenceInterface->Header.Start[(GetTickCount() / 100) % psprInVillageDefenceInterface->Header.font_number]],
											darktable);
					}
					else
						Draw_PutSpriteLightCRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont);
				}
				else
					Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
			}
		}
	}

	// robypark 2004/10/29 15:46
	// ������ ���� <==> �Ϲ� ���ΰ� ĳ���� ���� ���� ����Ʈ �׸���
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// ���� ����Ʈ ���ҽ� ���
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// ������ �������� ó�� ���ϴ� ���
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// ������ ������ ���¿��� �����ϴ� ���

		if (lpSprEffect)
		{
			// ���� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectTransformFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/2 19:7
	// ������ ������ ����ǰ ���� ȿ�� �׸���. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	if ((0 != m_siEffectSupplyGoodsType) && (FALSE == m_bIsInVillageDefenceInterface))
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// ����ǰ ������ ����Ʈ ���ҽ� ���
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// ������ ������ ����ǰ�� �޾Ҵ�
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// ������ ������ ����ǰ�� �ٴڳ���.

		if (lpSprEffect)
		{
			// ����ǰ ���� ����Ʈ �׸���
			clGrp.PutSpriteCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectSupplyGoodsFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/10 14:53
	// ������ ������ �������� �޴� �ݰ� ���� �ִϸ��̼� �׸���
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// ������ ������ �������� �޴� �ݰ� ���� �̹��� ���ҽ� ���
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// ������ ������ ���� �ݰ��� �޾Ҵ�. �⸧
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// ������ ������ ���� �ݰ��� �޾Ҵ�. ��

		if (lpSprEffect)
		{
			// ������ ������ �������� �޴� �ݰ� ���� �̹��� �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 25
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectVillageConterAttackFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// ������ ���� �߰��� ��� ����Ʈ �׸���
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect;

		// �߰��� ��� ����Ʈ ���ҽ� ���
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		if (lpSprEffect)
		{
			// �߰��� ��� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectEvasionFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// ������ ���� ġ���� ���� ����Ʈ �׸���
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect;

		// ġ���� ���� ����Ʈ ���ҽ� ���
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		if (lpSprEffect)
		{
			// ġ���� ���� ����Ʈ �׸���
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectDeathblowFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����ٴϴ� �뺴�� Draw(���ο��� ���� �Լ�)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawCharByVMercenary(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY)
{
	SI32				siVMXSize;
	SI32				siVMYSize;
	SI32				DrawXSize, DrawYSize;
	SI32				DrawXMaxSize, DrawYMaxSize;
	UI08				TempPB[256];
	SI32				TempCount;
	unsigned char*		darktable;
	SI32				siTempImKind;

	siTempImKind = TransKindOnlineToImjin(m_MercenaryKind);

	darktable = TeamDarkTable[0];

	siVMXSize = GetFileXsize(m_siTempImFile);
	siVMYSize = GetFileYsize(m_siTempImFile);

	if(WaterP >= 100)
	{
		// �׸��� ���
		if(KI[siTempImKind].m_uiAtb & ATB_SHADOW)
		{
			SHORT ylead = 70;
			//darkstep=max(darkstep, 4);
			if(m_bReverse == FALSE)	Draw_PutSpriteShadowCT(siStartX, siStartY + ylead, m_siTempImFile, m_VMercenary_siFont, DarkTable[SHADOW_DARK_STEP]);
			else                    Draw_PutSpriteShadowCRT(siStartX, siStartY + ylead, m_siTempImFile, m_VMercenary_siFont, DarkTable[SHADOW_DARK_STEP]);
		}

		if(m_VMercenary_bReverse == FALSE) Draw_PutSpriteT(siStartX,  siStartY, siVMXSize, siVMYSize, m_siTempImFile, m_VMercenary_siFont, darktable);
		else                               Draw_PutSpriteRT(siStartX, siStartY, siVMXSize, siVMYSize, m_siTempImFile, m_VMercenary_siFont, darktable);
	}
	else
	{
		DrawXMaxSize = siVMXSize;
		DrawYMaxSize = siVMYSize;

		DrawXSize    = DrawXMaxSize;
		DrawYSize    = (DrawYMaxSize * WaterP) / 100;

		for(TempCount = 0; TempCount < 256; TempCount++)
		{
			TempPB[TempCount] = (UI08)TempCount;
		}

		if(m_VMercenary_bReverse == FALSE) Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), siVMXSize, siVMYSize, m_siTempImFile, m_VMercenary_siFont, darktable);
		else                               Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), siVMXSize, siVMYSize, m_siTempImFile, m_VMercenary_siFont, darktable);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ĳ���͸� ��´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawHero(SI32 siDrawMode, SI32 siStartX, SI32 siStartY, UI08 *ctable, SI32 siCenterX, SI32 siCenterY, SI16 WaterP)
{ 
	// ������ ĳ������ ��쿡�� ����ش�. 
	if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == FALSE ) return;
	
	SI32	siFileID[10], siCharFont[10];
	SI32	siBodyPart[10], siTotalPiece;
	SI32	xsize, ysize;
	SI32	siShadowX, siShadowY, siShadowCX, siShadowCY;
	BOOL	bFlag;
	SI32	i;
	SI32	TempPiece;
	BOOL    bDefault;

	ZeroMemory(siBodyPart,sizeof(siBodyPart) );
	ZeroMemory(siCharFont,sizeof(siCharFont) );
	ZeroMemory(siFileID,  sizeof(siFileID) );
	// ĳ������ ���̵� ���´�.(HERO_KM, HERO_KW, HERO_JM, HERO_JW)
	SI32 heroid = KI[m_siImKind].m_ssHeroID;


	// ȭ�鿡 ���� Body�� ������ ���´�.
	siTotalPiece  =  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siTotalPiece[m_siHeroAniAction][m_siHeroBaseWClass];	


	bFlag = FALSE;	
 
	TempPiece = 0; 
	bDefault  = FALSE;
	for( i=0; i<siTotalPiece; i++ )
	{
		// ���� Bodypart�� ���´�.
		siBodyPart[i]		=  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siPartDrawSort[m_siHeroAniAction][m_siHeroBaseWClass][m_siDir][i];
		siCharFont[i]		=  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siPartFrame[m_siHeroAniAction][m_siHeroBaseWClass][m_siHeroWeaponEnum][m_siAniStep];
		siFileID[i]			=  pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].siFileID[m_siHeroAniAction][siBodyPart[i]];
		
//		if(!pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].bPart[m_siHeroAniAction][m_siHeroBaseWClass][siBodyPart[i]] && siFileID[i] > 0)
//		{
//			bDefault = TRUE;
///			break;
//		}
//		if(siFileID[i] > 0 )TempPiece++;
	}
	
	if(bDefault	== FALSE)
	{
		for(i = 0 ; i < siTotalPiece ; i++)
		{
			if(!pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].bLoadComplete && siFileID[i] > 0 && 
				pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].bPart[m_siHeroAniAction][m_siHeroBaseWClass][siBodyPart[i]])
			{
				bDefault = TRUE;
				break;
			}
			if(pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].bPart[m_siHeroAniAction][m_siHeroBaseWClass][siBodyPart[i]] == TRUE)
			{
				xsize = pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Xsize;
				ysize = pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Ysize;
					
				// ���� ȭ�鿡 ����� ��Ʈ�� �����Ѵ�.
				siCharFont[i]	+=	m_siFont;
				
				// ��ǥ ����
				if( bFlag == FALSE )
				{
					siStartX		=	siCenterX - xsize / 2;
					siStartY		=	siCenterY - ysize + KI[m_siImKind].m_ssBottomIndent;
					bFlag = TRUE;
				}

				SI32 xxx;

				switch( siDrawMode )
				{
				case DRAW_MODE_NORMAL:				
					if( m_bReverse == FALSE )
						clGrp.PutSpriteT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					else
					{
						XSPR	*pImage = &pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]];

						if(pImage->Image)
						{
							clGrp.PutSpriteRT(siStartX, siStartY, xsize, ysize,	&pImage->Image[pImage->Header.Start[siCharFont[i]]], ctable);
						}
					}
					break;

				case DRAW_MODE_SHADOW:
					if(clGrp.GetShadowInfo(siStartX, siStartY, xsize, ysize,
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterX[m_siHeroAniAction][m_siHeroBaseWClass],
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterY[m_siHeroAniAction][m_siHeroBaseWClass],
						&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
					{
						if(clGrp.IsInClipArea(siShadowX, siShadowY, siShadowX+siShadowCX-1, siShadowY+siShadowCY-1) == TRUE)
						{
							clGrp.PutSpriteShadowAT(siShadowX, siShadowY, siShadowCX, siShadowCY, xsize, ysize,
								&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
								m_bReverse);
						}
						else
						{
							clGrp.PutSpriteShadowACT(siShadowX, siShadowY, siShadowCX, siShadowCY, xsize, ysize,
								&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
								m_bReverse);
						}
					}
					//clGrp.BoxC(siShadowX, siShadowY, siShadowX+siShadowCX-1, siShadowY+siShadowCY-1, 255);
					break;

				case DRAW_MODE_TRANSPARENT:
					if( m_bReverse == FALSE )
						clGrp.PutSpriteLightImjinCT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					else
						clGrp.PutSpriteLightImjinCRT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					break;

				case DRAW_MODE_HALF_TRANSPARENT:
					if( m_bReverse == FALSE )
						clGrp.PutSpriteLightCT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					else
						clGrp.PutSpriteLightCRT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					break;

				case DRAW_MODE_WATERNORMAL:
					xxx = WaterP;
					WaterP = 10;
					if( m_bReverse == FALSE )
						clGrp.PutSpriteT(siStartX, siStartY + WaterP, xsize, ysize - KI[m_siImKind].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					else
						clGrp.PutSpriteRT(siStartX, siStartY + WaterP, xsize, ysize - KI[m_siImKind].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					WaterP = xxx;
					break;

				case DRAW_MODE_WATERSHADOW:
					xxx = WaterP;
					WaterP = 10;
					if(clGrp.GetShadowInfo(siStartX, siStartY, xsize, ysize,
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterX[m_siHeroAniAction][m_siHeroBaseWClass],
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterY[m_siHeroAniAction][m_siHeroBaseWClass],
						&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
					{
						clGrp.PutSpriteShadowACT(siShadowX, siShadowY + WaterP, siShadowCX, siShadowCY, xsize, ysize - KI[m_siImKind].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							m_bReverse);
					}
					WaterP = xxx;
					break;

				default:
					break;
				}
			}
		}
	}

	if(bDefault == TRUE)
	{
		ZeroMemory(siBodyPart,sizeof(siBodyPart) );
		ZeroMemory(siCharFont,sizeof(siCharFont) );
		ZeroMemory(siFileID,  sizeof(siFileID) );

	
		for( i=0; i<siTotalPiece; i++ )
		{
			// ���� Bodypart�� ���´�.
			siBodyPart[i]		=  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siPartDrawSort[m_siHeroAniAction][m_siHeroBaseWClass][m_siDir][i];
			siCharFont[i]		=  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siPartFrame[m_siHeroAniAction][m_siHeroBaseWClass][m_siHeroWeaponEnum][m_siAniStep];
			siFileID[i]			=  pMyOnlineWorld->pCharFileLoadManager->m_DefaultCurrEquip[heroid].siFileID[m_siHeroAniAction][siBodyPart[i]];
			
	//		if(!pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID].bLoadComplete && siFileID > 0)
	//			bDefault = TRUE;

		//	if(siFileID[i] > 0 )TempPiece++;
		}
		
	 	for(i = 0 ; i < siTotalPiece ; i++)
		{
			if(pMyOnlineWorld->pCharFileLoadManager->m_DefaultCurrEquip[heroid].bPart[m_siHeroAniAction][m_siHeroBaseWClass][siBodyPart[i]] = TRUE)
			{
				//siFileID[i]		=  pMyOnlineWorld->pCharFileLoadManager->m_DefaultCurrEquip[heroid].siFileID[m_siHeroAniAction][siBodyPart[i]];
				if(siFileID[i] <= 0 )
					continue;
//				siFileID		=  pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[heroid].siFileID[m_siHeroAniAction][siBodyPart];
				// ĳ���͸� ���� ��ǥ(DrawStartX, DrawStartY), �̹��� ������(DrawXsize, DrawYsize)�� ���Ѵ�.  
				xsize = pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Xsize;
				ysize = pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Ysize;
				
				// ���� ȭ�鿡 ����� ��Ʈ�� �����Ѵ�.
				siCharFont[i] += m_siFont;
				
				// ��ǥ ����
				if( bFlag == FALSE )
				{
				siStartX		=	siCenterX - xsize / 2;
				siStartY		=	siCenterY - ysize + KI[m_siImKind].m_ssBottomIndent;
				bFlag = TRUE;
				}

				SI32 xxx;
				switch( siDrawMode )
				{
				case DRAW_MODE_NORMAL:				
					if( m_bReverse == FALSE )
						clGrp.PutSpriteT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					else
					{
						XSPR	*pImage = &pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]];

						if(pImage->Image)
						{
							clGrp.PutSpriteRT(siStartX, siStartY, xsize, ysize,	&pImage->Image[pImage->Header.Start[siCharFont[i]]], ctable);
						}
					}
					break;

				case DRAW_MODE_SHADOW:
					if(clGrp.GetShadowInfo(siStartX, siStartY, xsize, ysize,
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterX[m_siHeroAniAction][m_siHeroBaseWClass],
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterY[m_siHeroAniAction][m_siHeroBaseWClass],
						&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
					{
						if(clGrp.IsInClipArea(siShadowX, siShadowY, siShadowX+siShadowCX-1, siShadowY+siShadowCY-1) == TRUE)
						{
							clGrp.PutSpriteShadowAT(siShadowX, siShadowY, siShadowCX, siShadowCY, xsize, ysize,
								&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
								m_bReverse);
						}
						else
						{
							clGrp.PutSpriteShadowACT(siShadowX, siShadowY, siShadowCX, siShadowCY, xsize, ysize,
								&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
								m_bReverse);
						}
					}
					break;

				case DRAW_MODE_TRANSPARENT:
					if( m_bReverse == FALSE )
						clGrp.PutSpriteLightImjinCT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					else
						clGrp.PutSpriteLightImjinCRT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					break;

				case DRAW_MODE_HALF_TRANSPARENT:
					if( m_bReverse == FALSE )
						clGrp.PutSpriteLightCT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					else
						clGrp.PutSpriteLightCRT(siStartX, siStartY, xsize, ysize,
							&pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID[i]].Header.Start[siCharFont[i]]]);
					break;

				case DRAW_MODE_WATERNORMAL:
					xxx = WaterP;
					WaterP = 10;
					if( m_bReverse == FALSE )
						clGrp.PutSpriteT(siStartX, siStartY + WaterP, xsize, ysize - KI[siFileID[i]].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					else
						clGrp.PutSpriteRT(siStartX, siStartY + WaterP, xsize, ysize - KI[siFileID[i]].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							ctable);
					WaterP = xxx;
					break;

				case DRAW_MODE_WATERSHADOW:
					xxx = WaterP;
					WaterP = 10;
					if(clGrp.GetShadowInfo(siStartX, siStartY, xsize, ysize,
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterX[m_siHeroAniAction][m_siHeroBaseWClass],
						pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siCenterY[m_siHeroAniAction][m_siHeroBaseWClass],
						&siShadowX, &siShadowY, &siShadowCX, &siShadowCY))
					{
						clGrp.PutSpriteShadowACT(siShadowX, siShadowY + WaterP, siShadowCX, siShadowCY, xsize, ysize - KI[heroid].m_ssBottomIndent - WaterP,
							&pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Image[pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Start[siCharFont[i]]],
							m_bReverse);
					}
					WaterP = xxx;
					break;

				default:
					break;
				}
			}
		}
	} 

}

