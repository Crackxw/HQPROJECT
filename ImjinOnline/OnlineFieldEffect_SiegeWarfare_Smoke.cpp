// OnlineFieldEffect_SiegeWarfare_Smoke.cpp: implementation of the COnlineFieldEffect_SiegeWarfare_Smoke class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineFieldEffect_SiegeWarfare_Smoke.h"
#include "..\Gersang\DirectX.h"
#include "..\Gersang\music.h"
#include <colortable.h>
#include <etc.h>
#include <CharAnimation.h>
#include <clGame.h>

extern _clGame*				pGame;

extern	unsigned char		SqrtVary[];
extern	unsigned char		TeamDarkTable[MAX_DARK_STEP][256];
extern	unsigned char		DarkTable[][256];						// 어두운 단계 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_Smoke::COnlineFieldEffect_SiegeWarfare_Smoke()
{
	// 초기화
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke));
}

COnlineFieldEffect_SiegeWarfare_Smoke::~COnlineFieldEffect_SiegeWarfare_Smoke()
{

}

void COnlineFieldEffect_SiegeWarfare_Smoke::Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/, SI32 siEffectSndIdx/* = 0*/, bool bSoundDelayor/* = false*/) 
{
	// 초기화
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke));
		
	m_ssID					=	id;
	m_ssKind				=	kind;

	// 이미 정의된 Smoke 속성값에서 얻어와 속성 설정
	m_ssFile				=	g_SKI[m_ssKind].m_ssFile;					// 리소스
	m_ssStartFont			=	g_SKI[m_ssKind].m_ssStartFont;				// 시작 폰트번호
    m_scMoveFrameNumber		=	(char)g_SKI[m_ssKind].m_ssMoveFrameNumber;	// 한 애니메이션에 해당되는 프레임 수
    m_ssMaxRepeatNumber		=	g_SKI[m_ssKind].m_ssMaxRepeatNumber;		// 반복회수
	m_ssRisingSwitch		=	g_SKI[m_ssKind].m_ssRisingSwitch;			// 이미지를 그릴 때 시간에 따라 그림을 위쪽에 그리는가? NOT UESD
	m_ssTransparentMode		=	g_SKI[m_ssKind].m_siTransparentMode;		// 투명 모드
	m_siStepDelay			=	g_SKI[m_ssKind].m_siStepDelay;				// 애니메이션 지연시간

	// 그려지기 스위치
	m_scDrawSwitch			=	TRUE;

	// Smoke의 위치(타일)
	m_ssRealX				=	realx;
	m_ssRealY				=	realy;

	// 동작 명령
	m_ssCurrentOrder		=	ORDER_SMOKE_EXP;

	// 애니메이션 타입
	m_ssAnimation			=	ANI_EXP;

	// 애니메이션이 변경된 프레임 번호
	m_uiLastChangedFrame	=	pGame->pOnlineWorld->GetTotalFrame();

	// 이미지 크기
	m_ssDrawXsize			=	GetFileXsize(m_ssFile);
	m_ssDrawYsize			=	GetFileYsize(m_ssFile);

	// 애니메이션 시작 지연 시간 관련 값 초기화
	m_ssCurrentAnimationDelay = 0;
	m_ssAnimationDelay = ssAnimationDelay;

	// robypark 2004/12/17 17:3
	// 효과음 Index
	m_siEffectSndIdx	= siEffectSndIdx;

	// 효과음만 지연 시간 후에 연주하며 그림은 그리지 않는지를 나타내는 플래그
	m_bSoundDelayor		= bSoundDelayor;

	// robypark 2004/12/20 17:59
	m_uiTargetUniqueID = uiTargetUniqueID;		// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
												// 보내야하는 피격자(피격 마을)의 ID
	m_uiTargetType	   = uiTargetType;			// 피격 대상이 마을인지, 캐릭터인지를 나타낸다.

	// 애니메이션 시작 지연 관련 처리
	if (m_ssAnimationDelay > 0)
	{
		m_scDrawSwitch = FALSE;	// 애니메이션 시작 지연 시간이 모두 경과될 때 까지 그려지지 않도록 설정
	}

	// robypark 2004/12/29 16:8
	// Smoke 타일 내 그려지는 위치 보정하기. 항상 같은 자리에만 그려지지 않도록하기 위함.
	m_scDrawPositionInTileX = 0;
	m_scDrawPositionInTileY = 0;
}

// robypark 2004/12/29 16:8
// Smoke 타일 내 그려지는 위치 보정하기. 항상 같은 자리에만 그려지지 않도록하기 위함.
void COnlineFieldEffect_SiegeWarfare_Smoke::SetSiegeWarfare_DrawPositionRandomizeInTile(SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY)
{
	m_scDrawPositionInTileX = scDrawPositionInTileX;
	m_scDrawPositionInTileY = scDrawPositionInTileY;
}

// robypark 2005/1/14 18:39
// 그릴 때 캐릭터 중심에 그려지도록 설정하기
void COnlineFieldEffect_SiegeWarfare_Smoke::SetSiegeWarfare_SetDrawInCharacterCenterPos(UI16 uiUniqueID)
{
	m_uiSmokeEffectCharUniqueID = uiUniqueID;
}

BOOL COnlineFieldEffect_SiegeWarfare_Smoke::Action()
{
	switch(m_ssCurrentOrder)
	{
		case ORDER_SMOKE_EXP:
			// 애니메이션 시작 지연 시간이 설정된 경우
			if (m_ssAnimationDelay > m_ssCurrentAnimationDelay)
			{
				// 지연시간이 지날 때 까지 기다린다.
				m_ssCurrentAnimationDelay++;

				return TRUE;
			}
			// robypark 2004/12/17 17:2
			else if (m_ssAnimationDelay == m_ssCurrentAnimationDelay)	// 대기 시간 종료된 경우
			{
				// robypark 2004/12/20 17:59
				if (m_uiTargetUniqueID)		// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
												// 보내야하는 피격자(피격 마을)의 ID
				{
					// 공격자인 자신이
					// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
					SI16 scGeneralEffect = pGame->pOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect();
					if (-1 != scGeneralEffect)
					{
						// 피격 대상이 캐릭터인지
						if (TARGET_TYPE_CHARACTER == m_uiTargetType)
							pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
						// 피격 대상이 마을인지
						else if (TARGET_TYPE_VILLAGE == m_uiTargetType)
							pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
					}
				}

				// 효과음 연주가 필요하다면
				if (m_siEffectSndIdx)
				{
					HashCodePushEffect(m_siEffectSndIdx, 0 , 0);
				}

				// 효과음만 지연 시간 후에 연주하며 그림은 그리지 않는지를 나타내는 플래그 활성화 되었다면
				if (m_bSoundDelayor)
					return FALSE;

				// One Timer
				m_ssCurrentAnimationDelay++;
			}

			// 애니메이션 시작 지연 시간이 경과되었거나 설정되지 않은 경우 그려지도록 설정
			m_scDrawSwitch = TRUE;

			// 시간에 따란 애니메이션 프레임 갱신
			if (abs(m_uiLastChangedFrame - pGame->pOnlineWorld->GetTotalFrame()) >= (m_siStepDelay * 2))
			{
				m_uiLastChangedFrame = pGame->pOnlineWorld->GetTotalFrame();
			    m_ssStep++;
			}

			// 애니메이션이 종료된 경우
			if(m_ssStep == m_scMoveFrameNumber)
			{
				// 반복하여 몇 차례 보여지는 애니메이션일 경우
				if(m_ssRepeatNumber < m_ssMaxRepeatNumber)
				{
					m_ssRepeatNumber++;
					m_ssStep=0;
				}
				else
					return FALSE;	// 애니메이션이 종료되었으므로 이 Smoke를 리스트에서 삭제해야 한다.
			}
			break;

		default:
			break;
	}

	// 좌표 갱신
	CalculateRealXY();

	// 빙석술일 경우 처리
	if(m_ssKind == KIND_SMOKE_ICE_BARRIER && m_ssRepeatNumber)
		m_ssFont = m_ssStartFont + m_scMoveFrameNumber - 1;
	else
		DecideFont();

	return TRUE;
}

void COnlineFieldEffect_SiegeWarfare_Smoke::CalculateRealXY()
{
	CalculateRealXYNormal();  
}

// 그려질 위치 보정
void COnlineFieldEffect_SiegeWarfare_Smoke::CalculateDrawStartXY()
{
	SI32 screenx=0, screeny=0;

	// 캐릭터 중심으로 그려야 하는 경우
	if (m_uiSmokeEffectCharUniqueID)
	{
		if (pGame->pOnlineWorld->pIOnlineChar->IsExistUniqueID(m_uiSmokeEffectCharUniqueID))
		{
			pGame->pOnlineWorld->pIOnlineChar->GetCharacterDrawCenterPos(m_uiSmokeEffectCharUniqueID, (SI16 &)screenx, (SI16 &)screeny);

			// 사용자 자신의 캐릭터와의 거리도 계산해주어야 한다.
			m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) + m_scDrawPositionInTileX;
			m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
		}
		else
		{
			pGame->pOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_ssRealX, m_ssRealY, screenx, screeny);

			// 사용자 자신의 캐릭터와의 거리도 계산해주어야 한다.
			m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX + m_scDrawPositionInTileX;
			m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
		}
	}
	else
	{
		pGame->pOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_ssRealX, m_ssRealY, screenx, screeny);

		// 사용자 자신의 캐릭터와의 거리도 계산해주어야 한다.
		m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX + m_scDrawPositionInTileX;
		m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
	}
}

// 찍을 위치를 계산한다. 
void COnlineFieldEffect_SiegeWarfare_Smoke::CalculateRealXYNormal()
{
//	int centerx, centery;  // 현재 위치의 중심 좌표 

	// X, Y좌표를 구한다. 
//	pGame->pOnlineWorld->pOnlineMap->TransformMapXY(m_ssRealX, m_ssRealY, m_ssX, m_ssY);

//	m_ssDrawXsize=GetFileXsize(m_ssFile);
//	m_ssDrawYsize=GetFileYsize(m_ssFile);

//	m_ssCenterX = m_ssRealX;
//	m_ssCenterY = m_ssRealY;
}


void COnlineFieldEffect_SiegeWarfare_Smoke::DecideFont()
{
	DecideFontNormal();
}

void COnlineFieldEffect_SiegeWarfare_Smoke::DecideFontNormal()
{
	switch(m_ssAnimation)
	{
		case ANI_EXP:
			m_ssFont = m_ssStartFont + m_ssStep;
			break;
		default:
			break;
	}
}


BOOL COnlineFieldEffect_SiegeWarfare_Smoke::Draw()
{
	unsigned char *darktable;
	// 어둔 단계를 설정한다. 
//	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
//	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	SHORT darkstep=0;
	darktable=TeamDarkTable[darkstep];

	if(m_scDrawSwitch==FALSE)
		return FALSE;

	// 뒤집어 찍는가?
	if(m_scReverseSwitch==FALSE)
	{
		if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX + GetFileXsize(m_ssFile)-1, m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)	// hojae_repair
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		else
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
	}
	else if(m_scReverseSwitch==TRUE)
	{
		if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+GetFileXsize(m_ssFile)-1, m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)	// hojae_repair
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		else
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		
	}

	return TRUE;
}
