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
extern	unsigned char		DarkTable[][256];						// ��ο� �ܰ� 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_Smoke::COnlineFieldEffect_SiegeWarfare_Smoke()
{
	// �ʱ�ȭ
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke));
}

COnlineFieldEffect_SiegeWarfare_Smoke::~COnlineFieldEffect_SiegeWarfare_Smoke()
{

}

void COnlineFieldEffect_SiegeWarfare_Smoke::Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/, SI32 siEffectSndIdx/* = 0*/, bool bSoundDelayor/* = false*/) 
{
	// �ʱ�ȭ
	ZeroMemory(this, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke));
		
	m_ssID					=	id;
	m_ssKind				=	kind;

	// �̹� ���ǵ� Smoke �Ӽ������� ���� �Ӽ� ����
	m_ssFile				=	g_SKI[m_ssKind].m_ssFile;					// ���ҽ�
	m_ssStartFont			=	g_SKI[m_ssKind].m_ssStartFont;				// ���� ��Ʈ��ȣ
    m_scMoveFrameNumber		=	(char)g_SKI[m_ssKind].m_ssMoveFrameNumber;	// �� �ִϸ��̼ǿ� �ش�Ǵ� ������ ��
    m_ssMaxRepeatNumber		=	g_SKI[m_ssKind].m_ssMaxRepeatNumber;		// �ݺ�ȸ��
	m_ssRisingSwitch		=	g_SKI[m_ssKind].m_ssRisingSwitch;			// �̹����� �׸� �� �ð��� ���� �׸��� ���ʿ� �׸��°�? NOT UESD
	m_ssTransparentMode		=	g_SKI[m_ssKind].m_siTransparentMode;		// ���� ���
	m_siStepDelay			=	g_SKI[m_ssKind].m_siStepDelay;				// �ִϸ��̼� �����ð�

	// �׷����� ����ġ
	m_scDrawSwitch			=	TRUE;

	// Smoke�� ��ġ(Ÿ��)
	m_ssRealX				=	realx;
	m_ssRealY				=	realy;

	// ���� ���
	m_ssCurrentOrder		=	ORDER_SMOKE_EXP;

	// �ִϸ��̼� Ÿ��
	m_ssAnimation			=	ANI_EXP;

	// �ִϸ��̼��� ����� ������ ��ȣ
	m_uiLastChangedFrame	=	pGame->pOnlineWorld->GetTotalFrame();

	// �̹��� ũ��
	m_ssDrawXsize			=	GetFileXsize(m_ssFile);
	m_ssDrawYsize			=	GetFileYsize(m_ssFile);

	// �ִϸ��̼� ���� ���� �ð� ���� �� �ʱ�ȭ
	m_ssCurrentAnimationDelay = 0;
	m_ssAnimationDelay = ssAnimationDelay;

	// robypark 2004/12/17 17:3
	// ȿ���� Index
	m_siEffectSndIdx	= siEffectSndIdx;

	// ȿ������ ���� �ð� �Ŀ� �����ϸ� �׸��� �׸��� �ʴ����� ��Ÿ���� �÷���
	m_bSoundDelayor		= bSoundDelayor;

	// robypark 2004/12/20 17:59
	m_uiTargetUniqueID = uiTargetUniqueID;		// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
												// �������ϴ� �ǰ���(�ǰ� ����)�� ID
	m_uiTargetType	   = uiTargetType;			// �ǰ� ����� ��������, ĳ���������� ��Ÿ����.

	// �ִϸ��̼� ���� ���� ���� ó��
	if (m_ssAnimationDelay > 0)
	{
		m_scDrawSwitch = FALSE;	// �ִϸ��̼� ���� ���� �ð��� ��� ����� �� ���� �׷����� �ʵ��� ����
	}

	// robypark 2004/12/29 16:8
	// Smoke Ÿ�� �� �׷����� ��ġ �����ϱ�. �׻� ���� �ڸ����� �׷����� �ʵ����ϱ� ����.
	m_scDrawPositionInTileX = 0;
	m_scDrawPositionInTileY = 0;
}

// robypark 2004/12/29 16:8
// Smoke Ÿ�� �� �׷����� ��ġ �����ϱ�. �׻� ���� �ڸ����� �׷����� �ʵ����ϱ� ����.
void COnlineFieldEffect_SiegeWarfare_Smoke::SetSiegeWarfare_DrawPositionRandomizeInTile(SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY)
{
	m_scDrawPositionInTileX = scDrawPositionInTileX;
	m_scDrawPositionInTileY = scDrawPositionInTileY;
}

// robypark 2005/1/14 18:39
// �׸� �� ĳ���� �߽ɿ� �׷������� �����ϱ�
void COnlineFieldEffect_SiegeWarfare_Smoke::SetSiegeWarfare_SetDrawInCharacterCenterPos(UI16 uiUniqueID)
{
	m_uiSmokeEffectCharUniqueID = uiUniqueID;
}

BOOL COnlineFieldEffect_SiegeWarfare_Smoke::Action()
{
	switch(m_ssCurrentOrder)
	{
		case ORDER_SMOKE_EXP:
			// �ִϸ��̼� ���� ���� �ð��� ������ ���
			if (m_ssAnimationDelay > m_ssCurrentAnimationDelay)
			{
				// �����ð��� ���� �� ���� ��ٸ���.
				m_ssCurrentAnimationDelay++;

				return TRUE;
			}
			// robypark 2004/12/17 17:2
			else if (m_ssAnimationDelay == m_ssCurrentAnimationDelay)	// ��� �ð� ����� ���
			{
				// robypark 2004/12/20 17:59
				if (m_uiTargetUniqueID)		// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
												// �������ϴ� �ǰ���(�ǰ� ����)�� ID
				{
					// �������� �ڽ���
					// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
					SI16 scGeneralEffect = pGame->pOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect();
					if (-1 != scGeneralEffect)
					{
						// �ǰ� ����� ĳ��������
						if (TARGET_TYPE_CHARACTER == m_uiTargetType)
							pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
						// �ǰ� ����� ��������
						else if (TARGET_TYPE_VILLAGE == m_uiTargetType)
							pGame->pOnlineWorld->pOnlineClient->SendFieldAttackAction(m_uiTargetUniqueID, scGeneralEffect);
					}
				}

				// ȿ���� ���ְ� �ʿ��ϴٸ�
				if (m_siEffectSndIdx)
				{
					HashCodePushEffect(m_siEffectSndIdx, 0 , 0);
				}

				// ȿ������ ���� �ð� �Ŀ� �����ϸ� �׸��� �׸��� �ʴ����� ��Ÿ���� �÷��� Ȱ��ȭ �Ǿ��ٸ�
				if (m_bSoundDelayor)
					return FALSE;

				// One Timer
				m_ssCurrentAnimationDelay++;
			}

			// �ִϸ��̼� ���� ���� �ð��� ����Ǿ��ų� �������� ���� ��� �׷������� ����
			m_scDrawSwitch = TRUE;

			// �ð��� ���� �ִϸ��̼� ������ ����
			if (abs(m_uiLastChangedFrame - pGame->pOnlineWorld->GetTotalFrame()) >= (m_siStepDelay * 2))
			{
				m_uiLastChangedFrame = pGame->pOnlineWorld->GetTotalFrame();
			    m_ssStep++;
			}

			// �ִϸ��̼��� ����� ���
			if(m_ssStep == m_scMoveFrameNumber)
			{
				// �ݺ��Ͽ� �� ���� �������� �ִϸ��̼��� ���
				if(m_ssRepeatNumber < m_ssMaxRepeatNumber)
				{
					m_ssRepeatNumber++;
					m_ssStep=0;
				}
				else
					return FALSE;	// �ִϸ��̼��� ����Ǿ����Ƿ� �� Smoke�� ����Ʈ���� �����ؾ� �Ѵ�.
			}
			break;

		default:
			break;
	}

	// ��ǥ ����
	CalculateRealXY();

	// �������� ��� ó��
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

// �׷��� ��ġ ����
void COnlineFieldEffect_SiegeWarfare_Smoke::CalculateDrawStartXY()
{
	SI32 screenx=0, screeny=0;

	// ĳ���� �߽����� �׷��� �ϴ� ���
	if (m_uiSmokeEffectCharUniqueID)
	{
		if (pGame->pOnlineWorld->pIOnlineChar->IsExistUniqueID(m_uiSmokeEffectCharUniqueID))
		{
			pGame->pOnlineWorld->pIOnlineChar->GetCharacterDrawCenterPos(m_uiSmokeEffectCharUniqueID, (SI16 &)screenx, (SI16 &)screeny);

			// ����� �ڽ��� ĳ���Ϳ��� �Ÿ��� ������־�� �Ѵ�.
			m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) + m_scDrawPositionInTileX;
			m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
		}
		else
		{
			pGame->pOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_ssRealX, m_ssRealY, screenx, screeny);

			// ����� �ڽ��� ĳ���Ϳ��� �Ÿ��� ������־�� �Ѵ�.
			m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX + m_scDrawPositionInTileX;
			m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
		}
	}
	else
	{
		pGame->pOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_ssRealX, m_ssRealY, screenx, screeny);

		// ����� �ڽ��� ĳ���Ϳ��� �Ÿ��� ������־�� �Ѵ�.
		m_ssDrawStartX = screenx - (m_ssDrawXsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRX + m_scDrawPositionInTileX;
		m_ssDrawStartY = screeny - (m_ssDrawYsize / 2) - pGame->pOnlineWorld->pOnlineMap->m_siRY - g_SKI[m_ssKind].m_siYIndent + m_scDrawPositionInTileY;
	}
}

// ���� ��ġ�� ����Ѵ�. 
void COnlineFieldEffect_SiegeWarfare_Smoke::CalculateRealXYNormal()
{
//	int centerx, centery;  // ���� ��ġ�� �߽� ��ǥ 

	// X, Y��ǥ�� ���Ѵ�. 
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
	// ��� �ܰ踦 �����Ѵ�. 
//	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
//	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	SHORT darkstep=0;
	darktable=TeamDarkTable[darkstep];

	if(m_scDrawSwitch==FALSE)
		return FALSE;

	// ������ ��°�?
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
