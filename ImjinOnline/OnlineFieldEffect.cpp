/************************************************************************************************
	ÆÄÀÏ¸í : OnlineFieldEffect.cpp
	´ã´çÀÚ : ÀÌÀ±¼®

	GameÁß ÇÊµå¿¡¼­ »ç¿ëµÅ´Â Æ¯¼öÈ¿°úµéÀ» Ã³¸®
************************************************************************************************/
#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineResource.h"
#include "..\Gersang\music.h"
#include "MakePalette.h"


#define CLOUDBLOCKSIZE			600
#define CLOUDSMALLBLOCKSIZE		400
#define CLOUDSTILEMOVETIME		100

#define RAIN_FALL_COUNT				8
#define SNOW_FALL_COUNT				4

#define RAIN_PERCENT_CHANGE			5
#define SNOW_PERCENT_CHANGE			8

// robypark 2004/8/11 11:6
// ±îÄ¡ ÀÌµ¿¼Óµµ
#define MAGPIETILEMOVETIME		20

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineFieldEffect::OnlineFieldEffect()
{
	m_lpWorld          = NULL;

	m_lpFieldEffect    = NULL;
	m_FieldEffectCount = 0;

	m_fCloud = FALSE;

	m_fWeather = FALSE;
	ZeroMemory(&m_Rain, sizeof(SEffectRain));
	ZeroMemory(&m_Snow, sizeof(SEffectSnow));
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineFieldEffect::~OnlineFieldEffect()
{
	Free();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineFieldEffect::Init(cltOnlineWorld* lpWorld, SI32 MaxEffectCount)
{
	Free();

	m_lpWorld          = lpWorld;

	m_lpFieldEffect    = new SEffectData[MaxEffectCount];
	ZeroMemory(m_lpFieldEffect, sizeof(SEffectData) * MaxEffectCount);
	m_FieldEffectCount = MaxEffectCount;

	clGrp.LoadXspr("Online\\GameSpr\\Cloud.Spr", m_CloudSpr);

	m_fCloud = FALSE;

	// robypark 2004/8/11 10:42
	// ±îÄ¡ ½ºÇÁ¶óÀÌÆ® ·Îµå
	clGrp.LoadXspr("Online\\GameSpr\\Magpie.Spr", m_SprMagpie);

	m_fWeather = FALSE;
	ZeroMemory(&m_Rain, sizeof(SEffectRain));
	ZeroMemory(&m_Snow, sizeof(SEffectSnow));

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇØÁ¦.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineFieldEffect::Free(void)
{
	m_lpWorld          = NULL;

	if(m_lpFieldEffect)
	{
		delete[] m_lpFieldEffect;
		m_lpFieldEffect = NULL;
		clGrp.FreeXspr(m_CloudSpr);

		// robypark 2004/8/11 10:44
		// ±îÄ¡ ½ºÇÁ¶óÀÌÆ® ÇØÁ¦
		clGrp.FreeXspr(m_SprMagpie);
	}

	m_FieldEffectCount = 0;
	m_fCloud = FALSE;

	m_fWeather = FALSE;
	ZeroMemory(&m_Rain, sizeof(SEffectRain));
	ZeroMemory(&m_Snow, sizeof(SEffectSnow));
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Ã³¸®
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineFieldEffect::Pool(void)
{
	SI32				TempCount;
	SI16				TempCount1;
	XSPR*				lpXSPR;
	SWeatherType		NowWeather;
	SI32				NowFallCount;
	SI32				siRealX, siRealY;

	for(TempCount = 0; TempCount < m_FieldEffectCount; TempCount++)
	{
		if(m_lpFieldEffect[TempCount].m_fCreate)
		{
			switch(m_lpFieldEffect[TempCount].m_Type)
			{
				case EFFECTTYPE_VILLAGEDEMAGE:
					m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Frame++;
					if(m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Frame > 30)
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				case EFFECTTYPE_ATTACKVILLAGETOENEMY:
					m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame++;

					switch(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_AttackType)
					{
						case EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_FALLINGROCK:
							if(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame == 1)
							{
								PushEffect(EFFECT_DROPSTONE, 0, 0);
							}
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprVillageAttackEffect(0);
							if(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame >= ((lpXSPR->Header.font_number * 3) + 60))
							{
								ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
							}
							break;

						case EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_SOWOIL:
							if(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame == 1)
							{
								PushEffect(EFFECT_DROPWATER, 0, 0);
							}
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprVillageAttackEffect(1);
							if(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame >= (lpXSPR->Header.font_number * 3))
							{
								ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
							}
							break;
					}
					break;

				case EFFECTTYPE_CHANGENUMBER:
					m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_Frame++;
					if(m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_Frame > 60)
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				// robypark 2005/2/3 14:5
				case EFFECTTYPE_FIELD_CHANGENUMBER:
					m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Frame++;
					if(m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Frame > 30)
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				case EFFECTTYPE_VILLAGEEXPLOSION:
					m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame++;

					lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_VILLAGEEXPLOSION);
					for(TempCount1 = 0; TempCount1 < 4; TempCount1++)
					{
						if(m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame == (TempCount1 * 4))
						{
							PushEffect(EFFECT_EXP, 0, 0);
						}
					}

					if(m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame > ((lpXSPR->Header.font_number * 2) + (4 * 3)))
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				case EFFECTTYPE_WARPEFFECT:
					m_lpFieldEffect[TempCount].m_EffectData_WarpEffect.m_Frame++;

					lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_WARPEFFECT);
					if(m_lpFieldEffect[TempCount].m_EffectData_WarpEffect.m_Frame >= (lpXSPR->Header.font_number * 3))
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				// robypark 2004/12/12 18:23
				// °ø¼ºÀü °ü·Ã ÀÌÆåÆ® Ãß°¡
				// ¸¶À» ¹æ¾î·Â Çâ»ó È¿°ú
				case EFFECTTYPE_VILLAGE_DEFENCE_UP:
					m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_Frame++;

					lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_DEFENCE_UP);
					if(m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_Frame >= (lpXSPR->Header.font_number * 3))
					{
						ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
					}
					break;

				// °ø¼ºÀü ¸¶À» Á¡·É È¿°ú
				case EFFECTTYPE_OCCUPIED_VILLAGE:
					m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_Frame++;

					lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_OCCUPIED_VILLAGE);
					if(m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_Frame >= (lpXSPR->Header.font_number * 3))
					{
						// robypark 2004/12/27 18:19
						// ¹Ýº¹ Animation Àç»ýÀ» À§ÇÑ ºñ±³
						m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_scCurrentRepeatTimes++;

						if (m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_scCurrentRepeatTimes
							> m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_scMaxRepeatTimes)
						{
							ZeroMemory(&m_lpFieldEffect[TempCount], sizeof(SEffectData));
						}
						else
							m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_Frame = 0;
					}
					break;
			}
		}
	}

	// ³¯¾¾
	NowWeather = GetNowPositionWeather();
	m_Rain.m_Percent -= RAIN_PERCENT_CHANGE;
	m_Snow.m_Percent -= SNOW_PERCENT_CHANGE;

	switch(NowWeather)
	{
		case WEATHER_RAIN:
			m_Rain.m_Percent += (RAIN_PERCENT_CHANGE * 2);
			break;

		case WEATHER_SNOW:
			m_Snow.m_Percent += (SNOW_PERCENT_CHANGE * 2);
			break;
	}
 
	if(m_Rain.m_Percent < 0)   m_Rain.m_Percent  = 0;
	if(m_Rain.m_Percent > 1000) m_Rain.m_Percent = 1000;
	if(m_Snow.m_Percent < 0)   m_Snow.m_Percent  = 0;
	if(m_Snow.m_Percent > 1000) m_Snow.m_Percent = 1000;

	// ºñÀ» Ã³¸®ÇÑ´Ù.
	if(m_Rain.m_NowRainCount)
	{
		for(TempCount1 = 0; TempCount1 < RAIN_MAX; TempCount1++)
		{
			switch(m_Rain.m_RainData[TempCount1].m_Type)
			{
				case 1:
					// ¶³¾îÁö´Â Áß
					m_Rain.m_RainData[TempCount1].m_NowPositionX += m_Rain.m_RainData[TempCount1].m_MovePositionX;
					m_Rain.m_RainData[TempCount1].m_NowPositionY += m_Rain.m_RainData[TempCount1].m_MovePositionY;
					if(m_Rain.m_RainData[TempCount1].m_NowPositionY >= m_Rain.m_RainData[TempCount1].m_TargetPositionY)
					{
						m_Rain.m_RainData[TempCount1].m_Type  = 2;
						m_Rain.m_RainData[TempCount1].m_Frame = 0;
					}
					break;

				case 2:
					// ´Ù ¶³¾îÁø ÈÄ
					m_Rain.m_RainData[TempCount1].m_Frame++;
					if(m_Rain.m_RainData[TempCount1].m_Frame >= 16)
					{
						m_Rain.m_RainData[TempCount1].m_Type  = 0;
						m_Rain.m_NowRainCount--;
					}
					break;
			}
		}
	}

	// ´«À» Ã³¸®ÇÑ´Ù.
	if(m_Snow.m_NowSnowCount)
	{
		for(TempCount1 = 0; TempCount1 < RAIN_MAX; TempCount1++)
		{
			switch(m_Snow.m_SnowData[TempCount1].m_Type)
			{
				case 1:
					// ¶³¾îÁö´Â Áß
					m_Snow.m_SnowData[TempCount1].m_FrameRotateFrame += 0.05f;
					m_Snow.m_SnowData[TempCount1].m_NowPositionY     += m_Snow.m_SnowData[TempCount1].m_MovePositionY;
					if(m_Snow.m_SnowData[TempCount1].m_NowPositionY >= m_Snow.m_SnowData[TempCount1].m_TargetPositionY)
					{
						m_Snow.m_SnowData[TempCount1].m_Type  = 2;
						m_Snow.m_SnowData[TempCount1].m_Frame = 0;
					}
					break;

				case 2:
					// ´Ù ¶³¾îÁø ÈÄ
					m_Snow.m_SnowData[TempCount1].m_Frame++;
					if(m_Snow.m_SnowData[TempCount1].m_Frame >= 24)
					{
						m_Snow.m_SnowData[TempCount1].m_Type  = 0;
						m_Snow.m_NowSnowCount--;
					}
					break;
			}
		}
	}

	// ºñ¸¦ Ãß°¡ÇÑ´Ù.
	NowFallCount = (m_Rain.m_Percent * RAIN_FALL_COUNT) / 1000;
	if(NowFallCount)
	{
		m_lpWorld->pOnlineMap->TransformScreenDotXYToRealDotXY(0, 0, siRealX, siRealY);

		siRealX += m_lpWorld->pOnlineMap->GetRX();
		siRealY += m_lpWorld->pOnlineMap->GetRY();

		TempCount1 = 0;
		for(TempCount = 0; TempCount < NowFallCount; TempCount++)
		{
			for(; TempCount1 < RAIN_MAX; TempCount1++)
			{
				if(m_Rain.m_RainData[TempCount1].m_Type == 0)
				{
					m_Rain.m_RainData[TempCount1].m_Type     = 1;
					m_Rain.m_RainData[TempCount1].m_RainType = rand() % 3;
					m_Rain.m_RainData[TempCount1].m_Frame    = 0;

					m_Rain.m_RainData[TempCount1].m_TargetPositionX = (rand() % 900) - 50;
					m_Rain.m_RainData[TempCount1].m_TargetPositionY = (rand() % 600) + 50;

					m_Rain.m_RainData[TempCount1].m_MovePositionX   = -12 - (m_Rain.m_RainData[TempCount1].m_TargetPositionY / 100);
					m_Rain.m_RainData[TempCount1].m_MovePositionY   = 60 + ((m_Rain.m_RainData[TempCount1].m_TargetPositionY / 100) * 2);
					m_Rain.m_RainData[TempCount1].m_NowPositionX    = m_Rain.m_RainData[TempCount1].m_TargetPositionX - (m_Rain.m_RainData[TempCount1].m_MovePositionX * 10);
					m_Rain.m_RainData[TempCount1].m_NowPositionY    = m_Rain.m_RainData[TempCount1].m_TargetPositionY - (m_Rain.m_RainData[TempCount1].m_MovePositionY * 10);

					m_Rain.m_RainData[TempCount1].m_TargetPositionX += siRealX;
					m_Rain.m_RainData[TempCount1].m_TargetPositionY += siRealY;
					m_Rain.m_RainData[TempCount1].m_NowPositionX    += siRealX;
					m_Rain.m_RainData[TempCount1].m_NowPositionY    += siRealY;

					m_Rain.m_NowRainCount++;
					break;
				}
			}
		}
	}

	// ´«À» Ãß°¡ÇÑ´Ù.
	NowFallCount = (m_Snow.m_Percent * SNOW_FALL_COUNT) / 1000;
	if(NowFallCount)
	{
		m_lpWorld->pOnlineMap->TransformScreenDotXYToRealDotXY(0, 0, siRealX, siRealY);

		siRealX += m_lpWorld->pOnlineMap->GetRX();
		siRealY += m_lpWorld->pOnlineMap->GetRY();
		TempCount1 = 0;
		for(TempCount = 0; TempCount < NowFallCount; TempCount++)
		{
			for(; TempCount1 < RAIN_MAX; TempCount1++)
			{
				if(m_Snow.m_SnowData[TempCount1].m_Type == 0)
				{

					m_Snow.m_SnowData[TempCount1].m_Type             = 1;
					m_Snow.m_SnowData[TempCount1].m_SnowType         = rand() % 4;
					m_Snow.m_SnowData[TempCount1].m_Frame            = 0;
					m_Snow.m_SnowData[TempCount1].m_FrameRotateFrame = 0.0f;

					m_Snow.m_SnowData[TempCount1].m_TargetPositionX = (rand() % 2000) - 600;

					m_Snow.m_SnowData[TempCount1].m_NowPositionY    = -10;
					m_Snow.m_SnowData[TempCount1].m_TargetPositionY = (rand() % 950) + 50;
					m_Snow.m_SnowData[TempCount1].m_MovePositionY   = 2 + (m_Snow.m_SnowData[TempCount1].m_TargetPositionY / 200);

					m_Snow.m_SnowData[TempCount1].m_TargetPositionX += siRealX;
					m_Snow.m_SnowData[TempCount1].m_NowPositionY    += siRealY;
					m_Snow.m_SnowData[TempCount1].m_TargetPositionY += siRealY;

					m_Snow.m_NowSnowCount++;
					break;
				}
			}
		}
	}

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Render
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineFieldEffect::Render(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32		TempCount;
	SI32		TempXCount;
	SI32		TempYCount;
	SI32		TempXBlock;
	SI32		TempYBlock;
	SI32		TempXRender;
	SI32		TempYRender;
	SI32		TempXBlockCount;
	SI32		TempYBlockCount;
	SI32		siSrcDotDrawBaseX, siSrcDotDrawBaseY;	
	SI32		siSrcDotDrawBaseX1, siSrcDotDrawBaseY1;	
	char		TempBuffer[64];
	XSPR*		lpXSpr;
	XSPR*		lpXSpr1;
	XSPR*		lpXSPR;
	float		TempPercent;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// °¢Á¾ µî·ÏµÈ Effect
		for(TempCount = 0; TempCount < m_FieldEffectCount; TempCount++)
		{
			if(m_lpFieldEffect[TempCount].m_fCreate)
			{
				switch(m_lpFieldEffect[TempCount].m_Type)
				{
					case EFFECTTYPE_VILLAGEDEMAGE:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_MapXPos, m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_MapYPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							lpXSpr = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBERBIG_RED);

							//if(m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_UpPercent == 100)
							{
								if (0 > m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage)
									sprintf((char*)TempBuffer, "-%I64d", m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage);
								else if (0 < m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage)
									sprintf((char*)TempBuffer, "+%I64d", -m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage);
								else
									sprintf((char*)TempBuffer, "%I64d", m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage);
							}
							/*
							else
							{
								float TempFloat = (float)m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_UpPercent * 0.01f;
								sprintf((char*)TempBuffer, "%I64d*%1.1f", m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Demage, TempFloat);
							}
							*/
							m_lpWorld->DrawNumber(siSrcDotDrawBaseX, siSrcDotDrawBaseY - 60 - (SI16)((float)m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Frame * ((float)m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Frame / 10.0f)), lpXSpr, (char*)TempBuffer);
						}
						break;

					case EFFECTTYPE_ATTACKVILLAGETOENEMY:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_DemageMapXPos, m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_DemageMapYPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							switch(m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_AttackType)
							{
								case EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_FALLINGROCK:
									lpXSpr = m_lpWorld->pOnlineResource->GetxsprVillageAttackEffect(0);
									if((m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame / 3) < 14)
									{
										clGrp.PutSpriteT(siSrcDotDrawBaseX - (lpXSpr->Header.Xsize / 2) + 8, siSrcDotDrawBaseY - 157
											, lpXSpr->Header.Xsize, lpXSpr->Header.Ysize
											, &lpXSpr->Image[lpXSpr->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame / 3]]);
									}
									else
									{
										clGrp.PutSpriteT(siSrcDotDrawBaseX - (lpXSpr->Header.Xsize / 2) + 8, siSrcDotDrawBaseY - 157
											, lpXSpr->Header.Xsize, lpXSpr->Header.Ysize
											, &lpXSpr->Image[lpXSpr->Header.Start[14]]);
									}
									break;

								case EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_SOWOIL:
									lpXSpr  = m_lpWorld->pOnlineResource->GetxsprVillageAttackEffect(1);
									lpXSpr1 = m_lpWorld->pOnlineResource->GetxsprVillageAttackEffect(2);

									clGrp.PutSpriteT(siSrcDotDrawBaseX - (lpXSpr1->Header.Xsize / 2) + 6, siSrcDotDrawBaseY - 163
										, lpXSpr1->Header.Xsize, lpXSpr1->Header.Ysize
										, &lpXSpr1->Image[lpXSpr1->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame / 3]]);
									clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - (lpXSpr->Header.Xsize / 2) + 6, siSrcDotDrawBaseY - 163
										, lpXSpr->Header.Xsize, lpXSpr->Header.Ysize
										, &lpXSpr->Image[lpXSpr->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame / 3]]);
									break;
							}
						}
						break;

					case EFFECTTYPE_VILLAGEEXPLOSION:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_XPos, m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_YPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_VILLAGEEXPLOSION);

							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							//clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - (lpXSPR->Header.Xsize / 2) - 10, siSrcDotDrawBaseY - (lpXSPR->Header.Ysize / 2) - 35, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame / 2]]);
							SI16		TempNowFrame;
							SI16		TempXPos;
							SI16		TempYPos;
							for(SI16 TempCount1 = 0; TempCount1 < 4; TempCount1++)
							{
								if((m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame >= (TempCount1 * 4)) && 
									(m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame < ((TempCount1 * 4) + (lpXSPR->Header.font_number * 2))))
								{
									TempNowFrame = m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame - (TempCount1 * 4);

									switch(TempCount1)
									{
										case 0:
											TempXPos = -50;
											TempYPos = -10;
											break;

										case 1:
											TempXPos = 40;
											TempYPos = -5;
											break;

										case 2:
											TempXPos = 10;
											TempYPos = -40;
											break;

										case 3:
											TempXPos = 0;
											TempYPos = 10;
											break;
									}
									clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - (lpXSPR->Header.Xsize / 2) - 10 + TempXPos, siSrcDotDrawBaseY - (lpXSPR->Header.Ysize / 2) - 35 + TempYPos, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[TempNowFrame / 2]]);
								}
							}
						}
						break;

					case EFFECTTYPE_WARPEFFECT:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_XPos, m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_YPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_WARPEFFECT);

							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - (lpXSPR->Header.Xsize / 2), siSrcDotDrawBaseY - ((lpXSPR->Header.Ysize / 4) * 3), lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_WarpEffect.m_Frame / 3]]);
						}
						break;

					// robypark 2005/2/3 14:7
					case EFFECTTYPE_FIELD_CHANGENUMBER:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_MapXPos, m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_MapYPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							// µ¥¹ÌÁö Ãâ·Â À§Ä¡ º¸Á¤
							if (false == m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_bVillageDefence)
							{
								siSrcDotDrawBaseX -= 20;
								siSrcDotDrawBaseY -= 20;
							}

							// +, - ±âÈ£ Ç¥½Ã ¾ÈÇÔ
							if (m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_ChangeNumber < 0)
								sprintf((char*)TempBuffer, "%I64d", -m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_ChangeNumber);
							else if (m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_ChangeNumber > 0)
								sprintf((char*)TempBuffer, "%I64d", m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_ChangeNumber);
							else
								sprintf((char*)TempBuffer, "%I64d", m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_ChangeNumber);

							XSPR *psprNumber = NULL;

							if (EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN == m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Color)
							{
								psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_GREEN);
							}
							else if (EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED == m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Color)
							{
								psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_RED);
							}
							else if (EFFECTDATA_FIELD_CHANGENUMBER_COLOR_BLUE == m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Color)
							{
								psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_BLUE);
							}
							else if (EFFECTDATA_FIELD_CHANGENUMBER_COLOR_WHITE == m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Color)
							{
								psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_WHITE);
							}

							m_lpWorld->DrawNumber(siSrcDotDrawBaseX
										, siSrcDotDrawBaseY - (m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_Frame)
										, psprNumber
										, (char*)TempBuffer);
						}
						break;

					// robypark 2004/12/12 18:23
					// °ø¼ºÀü °ü·Ã ÀÌÆåÆ® Ãß°¡
					// ¸¶À» ¹æ¾î·Â Çâ»ó È¿°ú
					case EFFECTTYPE_VILLAGE_DEFENCE_UP:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_XPos, m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_YPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_DEFENCE_UP);

							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							clGrp.PutSpriteCT(siSrcDotDrawBaseX - (lpXSPR->Header.Xsize / 2), siSrcDotDrawBaseY - (lpXSPR->Header.Ysize / 2) - 35, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_Frame / 3]]);
						}
						break;

					// °ø¼ºÀü ¸¶À» Á¡·É È¿°ú
					case EFFECTTYPE_OCCUPIED_VILLAGE:
						if(m_lpWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_XPos, m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_YPos, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == TRUE)
						{
							lpXSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_OCCUPIED_VILLAGE);

							siSrcDotDrawBaseX -= m_lpWorld->pOnlineMap->GetRX();
							siSrcDotDrawBaseY -= m_lpWorld->pOnlineMap->GetRY();

							clGrp.PutSpriteCT(siSrcDotDrawBaseX - (lpXSPR->Header.Xsize / 2), siSrcDotDrawBaseY - (lpXSPR->Header.Ysize / 2) - 35, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_Frame / 3]]);
						}
						break;
				}
			}
		}

		// ±¸¸§
		if(m_fCloud)
		{
			if(m_lpWorld->pOnlineMap->TransformMapXYToRealXY(m_lpWorld->pOnlineMap->m_siX, m_lpWorld->pOnlineMap->m_siY, siSrcDotDrawBaseX, siSrcDotDrawBaseY))
			{
				// robypark 2004/8/11 10:45
				// ±îÄ¡ °ü·Ã Ã³¸®
				TempXRender       =  siSrcDotDrawBaseX / CLOUDBLOCKSIZE;
				TempYRender       =  siSrcDotDrawBaseY / CLOUDBLOCKSIZE;
				TempXBlock        =  TempXRender - (((GetTickCount() / MAGPIETILEMOVETIME) / CLOUDBLOCKSIZE) * 2);
				TempYBlock        =  TempYRender + (((GetTickCount() / MAGPIETILEMOVETIME) / CLOUDBLOCKSIZE) * 1);

				SI32 siCount = 0;

				for(TempYCount = -2; TempYCount <= 1; TempYCount++)
				{
					for(TempXCount = -2; TempXCount <= 1; TempXCount++)
					{
						siCount++;

						TempPercent = (float)(GetTickCount() % (MAGPIETILEMOVETIME * CLOUDBLOCKSIZE)) / (float)(MAGPIETILEMOVETIME * CLOUDBLOCKSIZE);
						srand(((TempYBlock + TempYCount) << 8) + (TempXBlock + TempXCount));
						TempXBlockCount = (rand() % CLOUDSMALLBLOCKSIZE) + ((CLOUDBLOCKSIZE - CLOUDSMALLBLOCKSIZE) / 2);
						TempYBlockCount = (rand() % CLOUDSMALLBLOCKSIZE) + ((CLOUDBLOCKSIZE - CLOUDSMALLBLOCKSIZE) / 2);

						siSrcDotDrawBaseX1 = TempXBlockCount - (siSrcDotDrawBaseX - ((TempXRender + TempXCount) * CLOUDBLOCKSIZE));
						siSrcDotDrawBaseY1 = TempYBlockCount - (siSrcDotDrawBaseY - ((TempYRender + TempYCount) * CLOUDBLOCKSIZE));

						// robypark 2004/8/25 17:19
						// ±îÄ¡ ¹ß»ý ºóµµ¸¦ ÁÙÀÎ´Ù.
						if ((rand() % 100) < 50)
							continue;

//						char bDirection = rand() % 2;
						char bDirection = 0;

						if (0 == bDirection)	// North
						{
							siSrcDotDrawBaseX1 = siSrcDotDrawBaseX1 + (SI32)((float)(CLOUDBLOCKSIZE * 2) * TempPercent);
							siSrcDotDrawBaseY1 = siSrcDotDrawBaseY1 - (SI32)((float)(CLOUDBLOCKSIZE * 1) * TempPercent);
						}
						else if (1 == bDirection)	// West
						{
							siSrcDotDrawBaseX1 = siSrcDotDrawBaseX1 - (SI32)((float)(CLOUDBLOCKSIZE * 2) * TempPercent);
							siSrcDotDrawBaseY1 = siSrcDotDrawBaseY1 - (SI32)((float)(CLOUDBLOCKSIZE * 1) * TempPercent);
						}

						siSrcDotDrawBaseX1 -= m_lpWorld->pOnlineMap->GetRX();
						siSrcDotDrawBaseY1 -= m_lpWorld->pOnlineMap->GetRY();
						
						if (0 == bDirection)
						{
							// ±îÄ¡ °ü·Ã Ã³¸®(ºÏÂÊ ¹æÇâ ÀÌµ¿)
							clGrp.PutSpriteCT(siSrcDotDrawBaseX1 - (m_SprMagpie.Header.Xsize / 2), siSrcDotDrawBaseY1 - (m_SprMagpie.Header.Ysize / 2), m_SprMagpie.Header.Xsize, m_SprMagpie.Header.Ysize
								, &m_SprMagpie.Image[m_SprMagpie.Header.Start[((siCount + m_lpWorld->GetTotalFrame()) / 2) % m_SprMagpie.Header.font_number]]);
						}
						else if (1 == bDirection)
						{
							// ±îÄ¡ °ü·Ã Ã³¸®(¼­ÂÊ ¹æÇâ ÀÌµ¿)
							clGrp.PutSpriteRT(siSrcDotDrawBaseX1 - (m_SprMagpie.Header.Xsize / 2), siSrcDotDrawBaseY1 - (m_SprMagpie.Header.Ysize / 2), m_SprMagpie.Header.Xsize, m_SprMagpie.Header.Ysize
								, &m_SprMagpie.Image[m_SprMagpie.Header.Start[((siCount + m_lpWorld->GetTotalFrame()) / 2) % m_SprMagpie.Header.font_number]]);
						}
					}
				}

				TempXRender       =  siSrcDotDrawBaseX / CLOUDBLOCKSIZE;
				TempYRender       =  siSrcDotDrawBaseY / CLOUDBLOCKSIZE;
				TempXBlock        =  TempXRender - (((GetTickCount() / CLOUDSTILEMOVETIME) / CLOUDBLOCKSIZE) * 2);
				TempYBlock        =  TempYRender + (((GetTickCount() / CLOUDSTILEMOVETIME) / CLOUDBLOCKSIZE) * 1);

				for(TempYCount = -3; TempYCount <= 2; TempYCount++)
				{
					for(TempXCount = -3; TempXCount <= 2; TempXCount++)
					{
						TempPercent = (float)(GetTickCount() % (CLOUDSTILEMOVETIME * CLOUDBLOCKSIZE)) / (float)(CLOUDSTILEMOVETIME * CLOUDBLOCKSIZE);
						srand(((TempYBlock + TempYCount) << 8) + (TempXBlock + TempXCount));
						TempXBlockCount = (rand() % CLOUDSMALLBLOCKSIZE) + ((CLOUDBLOCKSIZE - CLOUDSMALLBLOCKSIZE) / 2);
						TempYBlockCount = (rand() % CLOUDSMALLBLOCKSIZE) + ((CLOUDBLOCKSIZE - CLOUDSMALLBLOCKSIZE) / 2);

						siSrcDotDrawBaseX1 = TempXBlockCount - (siSrcDotDrawBaseX - ((TempXRender + TempXCount) * CLOUDBLOCKSIZE));
						siSrcDotDrawBaseY1 = TempYBlockCount - (siSrcDotDrawBaseY - ((TempYRender + TempYCount) * CLOUDBLOCKSIZE));

						siSrcDotDrawBaseX1 = siSrcDotDrawBaseX1 + (SI32)((float)(CLOUDBLOCKSIZE * 2) * TempPercent);
						siSrcDotDrawBaseY1 = siSrcDotDrawBaseY1 - (SI32)((float)(CLOUDBLOCKSIZE * 1) * TempPercent);

						siSrcDotDrawBaseX1 -= m_lpWorld->pOnlineMap->GetRX();
						siSrcDotDrawBaseY1 -= m_lpWorld->pOnlineMap->GetRY();
						
						clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX1 - (m_CloudSpr.Header.Xsize / 2), siSrcDotDrawBaseY1 - (m_CloudSpr.Header.Ysize / 2), m_CloudSpr.Header.Xsize, m_CloudSpr.Header.Ysize
							, &m_CloudSpr.Image[m_CloudSpr.Header.Start[rand() % m_CloudSpr.Header.font_number]]);
					}
				}
			}
		}
		srand(GetTickCount());

		clGrp.UnlockSurface(pSurface);

		// ³¯¾¾
		XSPR*		lpWeatherSPR;
		SI16		Frame;
		SI32		siRealX, siRealY;
		SI32		RotateS;
		SI32		TempCount1;
		
		lpWeatherSPR = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_WEATHER);
		m_lpWorld->pOnlineMap->TransformScreenDotXYToRealDotXY(0, 0, siRealX, siRealY);
		siRealX += m_lpWorld->pOnlineMap->GetRX();
		siRealY += m_lpWorld->pOnlineMap->GetRY();

		TempCount1 = 0;
		for(TempCount = 0; TempCount < m_Rain.m_NowRainCount; TempCount1++)
		{
			switch(m_Rain.m_RainData[TempCount1].m_Type)
			{
				case 1:
					Frame = m_Rain.m_RainData[TempCount1].m_RainType;
					clGrp.PutSpriteT(
						m_Rain.m_RainData[TempCount1].m_NowPositionX - siRealX, m_Rain.m_RainData[TempCount1].m_NowPositionY - siRealY
						, lpWeatherSPR->Header.Xsize, lpWeatherSPR->Header.Ysize
						, &lpWeatherSPR->Image[lpWeatherSPR->Header.Start[Frame]]);

					TempCount++;
					break;

				case 2:
					Frame = (m_Rain.m_RainData[TempCount1].m_Frame / 4) + 3;
					clGrp.PutSpriteT(
						m_Rain.m_RainData[TempCount1].m_NowPositionX - siRealX, m_Rain.m_RainData[TempCount1].m_NowPositionY - siRealY
						, lpWeatherSPR->Header.Xsize, lpWeatherSPR->Header.Ysize
						, &lpWeatherSPR->Image[lpWeatherSPR->Header.Start[Frame]]);

					TempCount++;
					break;
			}
		}

		TempCount1 = 0;
		for(TempCount = 0; TempCount < m_Snow.m_NowSnowCount; TempCount1++)
		{
			switch(m_Snow.m_SnowData[TempCount1].m_Type)
			{
				case 1:
					// Áß°£ À§Ä¡¸¦ °è»êÇÑ´Ù.
					Frame   = m_Snow.m_SnowData[TempCount1].m_SnowType + 7;
					RotateS = (int)(sin(m_Snow.m_SnowData[TempCount1].m_FrameRotateFrame) * 10.0f);

					clGrp.PutSpriteT(
						m_Snow.m_SnowData[TempCount1].m_TargetPositionX - siRealX + RotateS, m_Snow.m_SnowData[TempCount1].m_NowPositionY - siRealY
						, lpWeatherSPR->Header.Xsize, lpWeatherSPR->Header.Ysize
						, &lpWeatherSPR->Image[lpWeatherSPR->Header.Start[Frame]]);

					TempCount++;
					break;

				case 2:
					Frame   = m_Snow.m_SnowData[TempCount1].m_SnowType + 7;
					RotateS = (int)(sin(m_Snow.m_SnowData[TempCount1].m_FrameRotateFrame) * 10.0f);

					if(m_Snow.m_SnowData[TempCount1].m_Frame > 12)
					{
						clGrp.PutSpriteLightCT(
							m_Snow.m_SnowData[TempCount1].m_TargetPositionX - siRealX + RotateS, m_Snow.m_SnowData[TempCount1].m_NowPositionY - siRealY
							, lpWeatherSPR->Header.Xsize, lpWeatherSPR->Header.Ysize
							, &lpWeatherSPR->Image[lpWeatherSPR->Header.Start[Frame]]);
					}
					else
					{
						clGrp.PutSpriteLightCT(
							m_Snow.m_SnowData[TempCount1].m_TargetPositionX - siRealX + RotateS, m_Snow.m_SnowData[TempCount1].m_NowPositionY - siRealY
							, lpWeatherSPR->Header.Xsize, lpWeatherSPR->Header.Ysize
							, &lpWeatherSPR->Image[lpWeatherSPR->Header.Start[Frame]]);
					}

					TempCount++;
					break;
			}
		}
	}

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Render Interface Effect
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineFieldEffect::RenderInterfaceEffect(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32		TempCount;
	char		TempBuffer[128];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		for(TempCount = 0; TempCount < m_FieldEffectCount; TempCount++)
		{
			if(m_lpFieldEffect[TempCount].m_fCreate)
			{
				switch(m_lpFieldEffect[TempCount].m_Type)
				{
					case EFFECTTYPE_CHANGENUMBER:
						XSPR		*psprNumber = NULL;

						if(m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_ChangeNumber > 0)
						{
							sprintf((char*)TempBuffer, "+%I64d", m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_ChangeNumber);
							psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_GREEN);
						}
						else if(m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_ChangeNumber < 0)
						{
							sprintf((char*)TempBuffer, "-%I64d", -m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_ChangeNumber);
							psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_RED);
						}
						else
						{
							sprintf((char*)TempBuffer, "%I64d", m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_ChangeNumber);
							psprNumber = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_GREEN);
						}

						m_lpWorld->DrawNumber(m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_XPos
									, m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_YPos - (m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_Frame / 2)
									, psprNumber
									, (char*)TempBuffer);
						break;
				}
			}
		}

		clGrp.UnlockSurface(pSurface);
	}

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Clear Effect
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void	OnlineFieldEffect::ClearEffect(void)
{
	ZeroMemory(m_lpFieldEffect, sizeof(SEffectData) * m_FieldEffectCount);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Add Effect
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineFieldEffect::AddEffect(SEffectData EffectData)
{
	// ºóÄ­À» Ã£¾Æ Ãß°¡
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_FieldEffectCount; TempCount++)
	{
		if(m_lpFieldEffect[TempCount].m_fCreate == FALSE)
		{
			CopyMemory(&m_lpFieldEffect[TempCount], &EffectData, sizeof(SEffectData));

			m_lpFieldEffect[TempCount].m_fCreate = TRUE;

			switch(m_lpFieldEffect[TempCount].m_Type)
			{
				case EFFECTTYPE_VILLAGEDEMAGE:
					m_lpFieldEffect[TempCount].m_EffectData_VillageDemage.m_Frame = 0;
					break;

				case EFFECTTYPE_ATTACKVILLAGETOENEMY:
					m_lpFieldEffect[TempCount].m_EffectData_AttackVillageToEnemy.m_Frame = 0;
					break;

				case EFFECTTYPE_CHANGENUMBER:
					m_lpFieldEffect[TempCount].m_EffectData_ChangeNum.m_Frame = 0;
					break;

				case EFFECTTYPE_FIELD_CHANGENUMBER:
					m_lpFieldEffect[TempCount].m_EffectData_FieldChangeNumber.m_Frame = 0;
					break;

				case EFFECTTYPE_VILLAGEEXPLOSION:
					m_lpFieldEffect[TempCount].m_EffectData_VillageExplosion.m_Frame = 0;
					break;

				case EFFECTTYPE_WARPEFFECT:
					m_lpFieldEffect[TempCount].m_EffectData_WarpEffect.m_Frame = 0;
					break;

				// robypark 2004/12/12 18:23
				// °ø¼ºÀü °ü·Ã ÀÌÆåÆ® Ãß°¡
				// ¸¶À» ¹æ¾î·Â Çâ»ó È¿°ú
				case EFFECTTYPE_VILLAGE_DEFENCE_UP:
					m_lpFieldEffect[TempCount].m_EffectData_VillageDefenceUp.m_Frame = 0;
					break;

				// °ø¼ºÀü ¸¶À» Á¡·É È¿°ú
				case EFFECTTYPE_OCCUPIED_VILLAGE:
					m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_Frame = 0;
					m_lpFieldEffect[TempCount].m_EffectData_OccupiedVillage.m_scCurrentRepeatTimes = 0;
					break;
			}
			
			return TRUE;
		}
	}

	return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Set Cloud
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void	OnlineFieldEffect::SetCloud(BOOL fCloud)
{
	m_fCloud = fCloud;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Set Weather
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void	OnlineFieldEffect::SetWeather(BOOL fWeather)
{
	m_fWeather = fWeather;

	if(GetNowPositionWeather() == WEATHER_RAIN)
	{
		PushEffect(EFFECT_RAIN, 0, 0);
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Çö À§Ä¡¿¡¼­ÀÇ ³¯¾¾µé
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SWeatherType OnlineFieldEffect::GetNowPositionWeather(void)
{
	if(m_fWeather == FALSE) return WEATHER_NONE;

	if(m_lpWorld)
	{
		if(m_lpWorld->pOnlineMap)
		{
			if(m_lpWorld->pOnlineMap->m_MapInfo.siMapIndex == 0)
			{
				// º»¸ÊÀÏ °æ¿ì
				if(m_lpWorld->pOnlineMap->m_siY < 524)
				{
					// ÇöÀç Y°¡ 524 ÀÌÇÏ´Â ´«
					return WEATHER_SNOW;
				}
				else
				{
					// ÇöÀç Y°¡ 524 ÀÌ»ó´Â ºñ
					return WEATHER_RAIN;
				}
			}
			else
			{
				// ±âÅ¸ ¸ÊÀÏ °æ¿ì
				if(m_lpWorld->pOnlineMap->m_MapInfo.siWeather & MAPINDEXPARSERWEATHER_RAIN)
				{
					return WEATHER_RAIN;
				}
				else if(m_lpWorld->pOnlineMap->m_MapInfo.siWeather & MAPINDEXPARSERWEATHER_SNOW)
				{
					return WEATHER_SNOW;
				}
			}
		}
	}

	return WEATHER_NONE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Map Change
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void OnlineFieldEffect::ChangeMap(SI16 Index)
{
	BOOL		fWeather;

	fWeather = FALSE;
	if(m_fWeather == FALSE)
	{
		fWeather = FALSE;
	}
	else
	{
		if(m_lpWorld)
		{
			if(m_lpWorld->pOnlineMap)
			{
				if(m_lpWorld->pOnlineMap->m_MapInfo.siMapIndex == 0)
				{
					// º»¸ÊÀÏ °æ¿ì
					fWeather = TRUE;
				}
				else
				{
					// ±âÅ¸ ¸ÊÀÏ °æ¿ì
					if(m_lpWorld->pOnlineMap->m_MapInfo.siWeather & MAPINDEXPARSERWEATHER_RAIN)
					{
						fWeather = TRUE;
					}
					else if(m_lpWorld->pOnlineMap->m_MapInfo.siWeather & MAPINDEXPARSERWEATHER_SNOW)
					{
						fWeather = TRUE;
					}
				}
			}
		}
	}

	if(fWeather == FALSE)
	{
		m_Rain.m_Percent = 0;
		m_Snow.m_Percent = 0;
	}
}
