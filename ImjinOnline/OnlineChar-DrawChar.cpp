//----------------------------------------------------------------------
//  필드에서 캐릭터 출력 관리. 
/// 김태곤 
//----------------------------------------------------------------------
#include <GSL.h>
#include <Main.h>
#include <OnlineWorld.h>

#include "charanimation.h"
#include <kindinfo.h>
#include "..\Gersang\DirectX.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"


#include <colortable.h>

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 
extern unsigned char TeamDarkTable[][256];
extern unsigned char TeamLightTable[];
extern	unsigned char				DarkTable[][256];					// 어두운 단계 
extern	UI08						WaterShadowTable[][256];			//[bg][spr]

#define DRAW_MODE_NORMAL			1
#define DRAW_MODE_SHADOW			2
#define DRAW_MODE_TRANSPARENT		3	// PutSpriteLightImjinCT() 함수 이용
#define DRAW_MODE_WATERNORMAL		4
#define DRAW_MODE_WATERSHADOW		5
#define	DRAW_MODE_HALF_TRANSPARENT	6	// PutSpriteLightCT() 함수 이용 PutSpriteLightImjinCT()함수를 이용한 출력보다 덜 반투명

// robypark 2005/1/27 13:29
// 수성 인터페이스 중 캐릭터 애니메이션 처리
#define VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT			0		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 망치질
#define VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT		8		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 망루 수리
#define VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT			16		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 톱질
#define VDI_ANIMATION_TYPE_REPAIR_FONT_NUMBER				8		// 공성전 수성인터페이스 마을 수리 중 한 동작에 관한 캐릭터 애니메이션 프레임 수.

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터만 출력
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawChar(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY)
{
	SI32		DrawXSize, DrawYSize;
	SI32		DrawXMaxSize, DrawYMaxSize;
	UI08		TempPB[256];
	SI32		TempCount;
	SI08		scBottomIndent = 5;

	unsigned char *darktable = NULL;

	// 피난민 캐릭터라면
	if (m_bIsRefugee)
	{
		DrawCharRefugee(WaterP, siStartX, siStartY, siCenterX, siCenterY);	// 피난민 캐릭터 그리기
		return;
	}

	// 대상물 선택이 된 것이면 표시해준다.
	if(GetNewSelectedFlag() == TRUE)
	{
		if(NewSelectedDelay % 2 == 0) darktable = TeamLightTable;
		else                          darktable = TeamDarkTable[0];
	}
	else
	{
		// robypark 2005/2/17 21:51
		// 커서 아래 캐릭터인가?
		if (m_bCursorHoverCharacter)
		{
			darktable = TeamLightTable;
			m_bCursorHoverCharacter = FALSE;	// 다시 커서 아래 캐릭터 초기화
		}
		else
			darktable = TeamDarkTable[0];
	}

	// robypark 2005/2/19 10:19
	if ((pMyOnlineWorld->pMyData->IsSiegeWarfareDay())	// 사용자가 공성이 있고,
		&& (m_bIsSiegeWarfareUnit)	// 이 캐릭터가 공성 유닛이며,
		&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != GetUniqueID())	// 사용자 유닛이 아니며
		&& (GetDataType() == 1))	// 이 캐릭터의 상세 정보가 없다면
	{
		// 캐릭터 상세 정보를 서버에 요청한다.
		pMyOnlineWorld->pOnlineClient->SendPlayerDetailInfo(GetUniqueID());
		SetDataType(2);	// 정보 요청 중...
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
					SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

					if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
					else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
				// 그림자 찍기
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
					// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
						// 아군
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// 적군
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// 기타 등등
						else
							Draw_PutSpriteT(siStartX,  siStartY, m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
				else
				{
					// robypark 2004/12/21 18:53
					// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
						// 아군
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// 적군
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteRT(siStartX, siStartY, m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// 기타 등등
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
					SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

					if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
					else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
						scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
					// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
						// 아군
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// 적군
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// 기타 등등
						else
							Draw_PutSpriteT(siStartX,  siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
				else
				{
					// robypark 2004/12/20 15:49
					// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
					if (TRUE == m_bIsInVillageDefenceInterface)
					{
						XSPR *psprInVillageDefenceInterface;
						psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

						// 다양한 모습의 수성 중인 캐릭터를 그리기 위한 처리
						SI08 scStartFont = VDI_ANIMATION_TYPE_REPAIR_HAMMER_BEGIN_FONT;

						if (VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER_BEGIN_FONT;
						else if (VDI_ANIMATION_TYPE_REPAIR_SAW == m_scInVillageDefenceInterfaceAniType)
							scStartFont = VDI_ANIMATION_TYPE_REPAIR_SAW_BEGIN_FONT;

						// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
						// 아군
						if (m_scForceRelation == SIEGEWARFARE_FORCE_FRIENDLY)
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Blue, m_siFont, darktable);
						// 적군
						else if (m_scForceRelation == SIEGEWARFARE_FORCE_ENEMY)
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siAnimationResourceID_Red, m_siFont, darktable);
						// 기타 등등
						else
							Draw_PutSpriteRT(siStartX, siStartY + (DrawYMaxSize - DrawYSize), m_siXSize, m_siYSize, m_siImFile, m_siFont, darktable);
					}
				}
			}
		}
	}

	// robypark 2004/10/29 15:46
	// 공성전 유닛 <==> 일반 주인공 캐릭터 간의 변신 이펙트 그리기
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// 변신 이펙트 리소스 얻기
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// 공성전 유닛으로 처음 변하는 경우
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// 공성전 유닛인 상태에서 변신하는 경우

		if (lpSprEffect)
		{
			// 변신 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectTransformFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/2 19:7
	// 공성전 유닛이 보급품 관련 효과 그리기. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	if ((0 != m_siEffectSupplyGoodsType) && (FALSE == m_bIsInVillageDefenceInterface))
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// 보급품 아이콘 이펙트 리소스 얻기
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// 공성전 유닛이 보급품을 받았다
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// 공성전 유닛의 보급품이 바닥났다.

		if (lpSprEffect)
		{
			// 보급품 관련 이펙트 그리기
			clGrp.PutSpriteCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectSupplyGoodsFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/10 14:53
	// 공성전 유닛이 마을에서 받는 반격 관련 애니메이션 그리기
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// 공성전 유닛이 마을에서 받는 반격 관련 이미지 리소스 얻기
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// 공성전 유닛이 마을 반격을 받았다. 기름
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// 공성전 유닛이 마을 반격을 받았다. 돌

		if (lpSprEffect)
		{
			// 공성전 유닛이 마을에서 받는 반격 관련 이미지 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 25
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectVillageConterAttackFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// 공성전 유닛 견고한 방어 이펙트 그리기
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect;

		// 견고한 방어 이펙트 리소스 얻기
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		if (lpSprEffect)
		{
			// 견고한 방어 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectEvasionFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// 공성전 유닛 치명적 공격 이펙트 그리기
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect;

		// 치명적 공격 이펙트 리소스 얻기
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		if (lpSprEffect)
		{
			// 치명적 공격 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectDeathblowFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}
} 

// 피난민 캐릭터만 Draw(내부에서 쓰는 함수)
VOID	cltOnlineChar::DrawCharRefugee(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY)
{
	SI32		DrawXSize, DrawYSize;
	SI32		DrawXMaxSize, DrawYMaxSize;
	UI08		TempPB[256];
	SI32		TempCount;
	SI08		scBottomIndent = 5;

	unsigned char *darktable = NULL;

	// 대상물 선택이 된 것이면 표시해준다.
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
			// 그림자 찍기
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
				// 공성전 수성인터페이스에 있는 캐릭터라면 투명하게 => 수성 인터페이스 캐릭터이미지 사용
				if (TRUE == m_bIsInVillageDefenceInterface)
				{
					XSPR *psprInVillageDefenceInterface;
					psprInVillageDefenceInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE);

					// 수성 인터페이스에 있을 경우 보여지는 캐릭터 이미지
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
	// 공성전 유닛 <==> 일반 주인공 캐릭터 간의 변신 이펙트 그리기
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// 변신 이펙트 리소스 얻기
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// 공성전 유닛으로 처음 변하는 경우
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// 공성전 유닛인 상태에서 변신하는 경우

		if (lpSprEffect)
		{
			// 변신 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2)
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectTransformFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/2 19:7
	// 공성전 유닛이 보급품 관련 효과 그리기. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	if ((0 != m_siEffectSupplyGoodsType) && (FALSE == m_bIsInVillageDefenceInterface))
	{
		// Effect
		XSPR*		lpSprEffect = NULL;

		// 보급품 아이콘 이펙트 리소스 얻기
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// 공성전 유닛이 보급품을 받았다
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// 공성전 유닛의 보급품이 바닥났다.

		if (lpSprEffect)
		{
			// 보급품 관련 이펙트 그리기
			clGrp.PutSpriteCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectSupplyGoodsFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/12/10 14:53
	// 공성전 유닛이 마을에서 받는 반격 관련 애니메이션 그리기
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// 공성전 유닛이 마을에서 받는 반격 관련 이미지 리소스 얻기
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// 공성전 유닛이 마을 반격을 받았다. 기름
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// 공성전 유닛이 마을 반격을 받았다. 돌

		if (lpSprEffect)
		{
			// 공성전 유닛이 마을에서 받는 반격 관련 이미지 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
						, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 25
						, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectVillageConterAttackFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// 공성전 유닛 견고한 방어 이펙트 그리기
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect;

		// 견고한 방어 이펙트 리소스 얻기
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		if (lpSprEffect)
		{
			// 견고한 방어 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectEvasionFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}

	// robypark 2004/11/24 18:50
	// 공성전 유닛 치명적 공격 이펙트 그리기
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect;

		// 치명적 공격 이펙트 리소스 얻기
		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		if (lpSprEffect)
		{
			// 치명적 공격 이펙트 그리기
			clGrp.PutSpriteLightImjinCT(siStartX + (m_siXSize / 2) - (lpSprEffect->Header.Xsize / 2)
					, siStartY + (m_siYSize / 2) - (lpSprEffect->Header.Ysize / 2) - 20
					, lpSprEffect->Header.Xsize, lpSprEffect->Header.Ysize, &lpSprEffect->Image[lpSprEffect->Header.Start[(m_siEffectDeathblowFrame / 3) % lpSprEffect->Header.font_number]]);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	따라다니는 용병만 Draw(내부에서 쓰는 함수)
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
		// 그림자 찍기
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
//	설명	:	조합형 캐릭터를 찍는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawHero(SI32 siDrawMode, SI32 siStartX, SI32 siStartY, UI08 *ctable, SI32 siCenterX, SI32 siCenterY, SI16 WaterP)
{ 
	// 조합형 캐릭터인 경우에만 찍어준다. 
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
	// 캐릭터의 아이디를 얻어온다.(HERO_KM, HERO_KW, HERO_JM, HERO_JW)
	SI32 heroid = KI[m_siImKind].m_ssHeroID;


	// 화면에 찍을 Body의 갯수를 얻어온다.
	siTotalPiece  =  pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[heroid].siTotalPiece[m_siHeroAniAction][m_siHeroBaseWClass];	


	bFlag = FALSE;	
 
	TempPiece = 0; 
	bDefault  = FALSE;
	for( i=0; i<siTotalPiece; i++ )
	{
		// 찍을 Bodypart를 얻어온다.
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
					
				// 현재 화면에 출력할 폰트를 설정한다.
				siCharFont[i]	+=	m_siFont;
				
				// 좌표 설정
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
			// 찍을 Bodypart를 얻어온다.
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
				// 캐릭터를 찍을 좌표(DrawStartX, DrawStartY), 이미지 사이즈(DrawXsize, DrawYsize)를 구한다.  
				xsize = pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Xsize;
				ysize = pMyOnlineWorld->pCharFileLoadManager->m_DefaultSpr[siFileID[i]].Header.Ysize;
				
				// 현재 화면에 출력할 폰트를 설정한다.
				siCharFont[i] += m_siFont;
				
				// 좌표 설정
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

