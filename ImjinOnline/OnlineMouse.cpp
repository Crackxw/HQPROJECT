#include <GSL.h>
#include <Main.h>
#include "OnlineMouse.h"
#include <DebugTool.h>
#include "../SCCommon/OnlineCommonParser.h"
#include "bindJXFile.h"

//===================================================================================================
CCursorImageMgr::CCursorImageMgr()
{
	ZeroMemory(m_sprCursorImage, sizeof(XSPR) * ON_CURSOR_IMAGE_MAX);
}

CCursorImageMgr::~CCursorImageMgr()
{
	for ( register int i = ON_CURSOR_IMAGE_FIRST; i<ON_CURSOR_IMAGE_MAX; i++ )	
	{
		clGrp.FreeXspr(m_sprCursorImage[i]);
	}
}

BOOL CCursorImageMgr::LoadCursorImage(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\Cursor\\CursorImage.dat";

	// だ橾擊 翮堅
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : 轎溘腆 縑楝だ橾貲 瞳營
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// だ諒擊 衛濛и棻.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 濠猿 匐儀
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 等檜攪 塭檣 蹺轎

		SI32	iLineNo, iColNo;

		// 濠猿蒂 牖離瞳戲煎 嫡擊 遽綠蒂 и棻.
		char szEnumIndex[48];
		char szImageFilePath[512];

		// 濠猿 嫡晦 偃衛
		ocp.GetValue( szEnumIndex,			47,		iLineNo, iColNo );
		ocp.GetValue( szImageFilePath,		511,	iLineNo, iColNo );

		// 醴憮 檜嘐雖 煎萄
		clGrp.LoadXspr(szImageFilePath, m_sprCursorImage[CCurosrImageEnum::GetData(szEnumIndex)]);

		// �挫弇蝏� - 虜擒 雖旎 籀葬и 匙檜 葆雖虞 鏽歲檜歷棻賊 薑鼻檜棻.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}

		// �挫� 餌髦 - 檜匙檜 雖旎 葆雖虞 塭檣簞蒂 籀葬ж堅 氈棻賊 粽溯檜觼
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

XSPR *CCursorImageMgr::GetCursorImage(ENUM_CURSOR_IMAGE enIndex)
{
	return &m_sprCursorImage[enIndex];
}

//===================================================================================================

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineBaseMouse::OnlineBaseMouse()
{	
	m_siCurrentFrameDelay		=	0;
	m_ptHotSpot.x				=	0;
	m_ptHotSpot.y				=	0;
	m_siFont					=	0;
	m_siMouseAniStatus			=	ON_MOUSE_ANI_STATUS_NORMAL;
	m_psprCursorImg				=	NULL;		// 醴憮 檜嘐雖
	m_scCurrentPlayDirection	=	ON_CURSOR_ANIMATION_METHOD_NORMAL;
	m_bPlayedOnce				=	false;				// 擁棲詭檜暮 и 廓 Ы溯檜陛 部陬棻.
	m_bEnableCursor				=	true;

	ZeroMemory(m_sMouse_ani_status, sizeof(SMOUSE_ANI_STATUS) * ON_MOUSE_MAX_ANI_STATUS);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineBaseMouse::~OnlineBaseMouse()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineBaseMouse::Init(XSPR *psprCursorImage, SMOUSE_ANI_STATUS *psMouse_ani_status, POINT ptHotSpot)
{
	m_psprCursorImg = psprCursorImage;

	CopyMemory(m_sMouse_ani_status, psMouse_ani_status, sizeof(SMOUSE_ANI_STATUS) * ON_MOUSE_MAX_ANI_STATUS);

	m_ptHotSpot = ptHotSpot;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineBaseMouse::Free()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	億煎檜 葆辦蝶蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineBaseMouse::SetMouse(bool bEnableCurosr)
{
	DWriteStringToFile("SetMouse() 撲薑");

	m_bEnableCursor = bEnableCurosr;

	if (m_bEnableCursor)
		m_siMouseAniStatus	=	ON_MOUSE_ANI_STATUS_NORMAL;
	else
		m_siMouseAniStatus	=	ON_MOUSE_ANI_STATUS_DISABLED;

	if (ON_CURSOR_ANIMATION_METHOD_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
		m_siFont				=	m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1;
	else
		m_siFont				=	0;

	m_siFont					=	0;
	m_siCurrentFrameDelay		=	0;
	m_scCurrentPlayDirection	=	ON_CURSOR_ANIMATION_METHOD_NORMAL;
	m_bPlayedOnce				=	false;				// 擁棲詭檜暮 и 廓 Ы溯檜陛 部陬棻.
}

VOID	OnlineBaseMouse::Action()
{
	if (ON_CURSOR_DRAW_OPTION_DISABLED == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
		return;

	if (ON_CURSOR_DRAW_OPTION_ONE_FRAME == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
		return;

	m_siCurrentFrameDelay++;

	if (ON_CURSOR_DRAW_OPTION_LOOP == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
	{
		if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFrameDelay < m_siCurrentFrameDelay)
		{
			m_siCurrentFrameDelay = 0;

			if (ON_CURSOR_ANIMATION_METHOD_NORMAL == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
			{
				if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1 <= m_siFont)
					m_siFont = 0;
				else
					m_siFont++;
			}
			else if (ON_CURSOR_ANIMATION_METHOD_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
			{
				if (0 >= m_siFont)
					m_siFont = m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1;
				else
					m_siFont--;
			}
			else if (ON_CURSOR_ANIMATION_METHOD_NORMAL_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
			{
				if (ON_CURSOR_ANIMATION_METHOD_NORMAL == m_scCurrentPlayDirection)
				{
					if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1 <= m_siFont)
					{
						m_siFont = m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1;
						m_scCurrentPlayDirection = ON_CURSOR_ANIMATION_METHOD_REVERSE;
					}
					else
						m_siFont++;
				}
				else
				{
					if (0 >= m_siFont)
					{
						m_siFont = 0;
						m_scCurrentPlayDirection = ON_CURSOR_ANIMATION_METHOD_NORMAL;
					}
					else
						m_siFont--;
				}
			}
		}
	}
	else
	{
		// 擁棲詭檜暮 и 廓 Ы溯檜陛 部陬棻朝雖 匐餌
		if (false == m_bPlayedOnce) 
		{
			if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFrameDelay < m_siCurrentFrameDelay)
			{
				m_siCurrentFrameDelay = 0;

				if (ON_CURSOR_ANIMATION_METHOD_NORMAL == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
				{
					if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1 <= m_siFont)
					{
						m_bPlayedOnce = true;
						m_siFont = m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1;
					}
					else
						m_siFont++;
				}
				else if (ON_CURSOR_ANIMATION_METHOD_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
				{
					if (0 >= m_siFont)
					{
						m_bPlayedOnce = true;
						m_siFont = 0;
					}
					else
						m_siFont--;
				}
				else if (ON_CURSOR_ANIMATION_METHOD_NORMAL_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
				{
					if (ON_CURSOR_ANIMATION_METHOD_NORMAL == m_scCurrentPlayDirection)
					{
						if (m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1 <= m_siFont)
						{
							m_siFont = m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1;
							m_scCurrentPlayDirection = ON_CURSOR_ANIMATION_METHOD_REVERSE;
						}
						else
							m_siFont++;
					}
					else
					{
						if (0 >= m_siFont)
						{
							m_siFont = 0;
							m_scCurrentPlayDirection = ON_CURSOR_ANIMATION_METHOD_NORMAL;
							m_bPlayedOnce = true;
						}
						else
							m_siFont--;
					}
				}
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	萄煎辦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineBaseMouse::Draw(LPPOINT pptMouse)
{
	SI32 siCilpLeft, siClipTop, siClipRight, siClipBottom;

	clGrp.GetClipArea(siCilpLeft, siClipTop, siClipRight, siClipBottom);
	clGrp.SetClipFullScreen();
	
	clGrp.PutSpriteT(pptMouse->x - m_ptHotSpot.x, pptMouse->y - m_ptHotSpot.y,
						m_psprCursorImg->Header.Xsize, m_psprCursorImg->Header.Ysize, 
						&m_psprCursorImg->Image[m_psprCursorImg->Header.Start[m_siFont + m_sMouse_ani_status[m_siMouseAniStatus].m_ssStartFont]]);
	
	clGrp.SetClipArea(siCilpLeft, siClipTop, siClipRight, siClipBottom);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶 擁棲詭檜暮 鼻鷓蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineBaseMouse::SetAniStatus(SI16 siStatus)
{
	if (m_bEnableCursor)
		m_siMouseAniStatus	=	siStatus;
	else
		m_siMouseAniStatus	=	ON_MOUSE_ANI_STATUS_DISABLED;

	m_scCurrentPlayDirection	=	ON_CURSOR_ANIMATION_METHOD_NORMAL;

	if (ON_CURSOR_ANIMATION_METHOD_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
		m_siFont				=	m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber;
	else
		m_siFont				=	0;

	m_siCurrentFrameDelay		=	0;
	m_bPlayedOnce				=	false;				// 擁棲詭檜暮 и 廓 Ы溯檜陛 部陬棻.
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶 擁棲詭檜暮 鼻鷓蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineBaseMouse::GetAniStatus()
{
	return	m_siMouseAniStatus;
}

// 葆辦蝶 擁棲詭檜暮檜 部陬朝陛?
BOOL	OnlineBaseMouse::IsDoneAnimation()
{
	if (ON_CURSOR_DRAW_OPTION_DISABLED == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
		return FALSE;
	else if (ON_CURSOR_DRAW_OPTION_LOOP == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
		return FALSE;
	else if (ON_CURSOR_DRAW_OPTION_ONE_FRAME == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationOption)
		return TRUE;

	if (ON_CURSOR_ANIMATION_METHOD_NORMAL == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
	{
		if (m_siFont >= m_sMouse_ani_status[m_siMouseAniStatus].m_ssFontNumber - 1)
			return TRUE;
	}
	else if (ON_CURSOR_ANIMATION_METHOD_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
	{
		if (0 == m_siFont)
			return TRUE;
	}
	else if (ON_CURSOR_ANIMATION_METHOD_NORMAL_REVERSE == m_sMouse_ani_status[m_siMouseAniStatus].m_scAnimationMethod)
	{
		if (m_scCurrentPlayDirection == ON_CURSOR_ANIMATION_METHOD_REVERSE)
		{
			if (0 == m_siFont)
				return TRUE;
		}
	}

	return FALSE;
}
