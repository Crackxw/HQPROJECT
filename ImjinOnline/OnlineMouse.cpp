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

	// ������ ����
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		char szEnumIndex[48];
		char szImageFilePath[512];

		// �ڷ� �ޱ� ����
		ocp.GetValue( szEnumIndex,			47,		iLineNo, iColNo );
		ocp.GetValue( szImageFilePath,		511,	iLineNo, iColNo );

		// Ŀ�� �̹��� �ε�
		clGrp.LoadXspr(szImageFilePath, m_sprCursorImage[CCurosrImageEnum::GetData(szEnumIndex)]);

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineBaseMouse::OnlineBaseMouse()
{	
	m_siCurrentFrameDelay		=	0;
	m_ptHotSpot.x				=	0;
	m_ptHotSpot.y				=	0;
	m_siFont					=	0;
	m_siMouseAniStatus			=	ON_MOUSE_ANI_STATUS_NORMAL;
	m_psprCursorImg				=	NULL;		// Ŀ�� �̹���
	m_scCurrentPlayDirection	=	ON_CURSOR_ANIMATION_METHOD_NORMAL;
	m_bPlayedOnce				=	false;				// �ִϸ��̼� �� �� �÷��̰� ������.
	m_bEnableCursor				=	true;

	ZeroMemory(m_sMouse_ani_status, sizeof(SMOUSE_ANI_STATUS) * ON_MOUSE_MAX_ANI_STATUS);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineBaseMouse::~OnlineBaseMouse()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineBaseMouse::Init(XSPR *psprCursorImage, SMOUSE_ANI_STATUS *psMouse_ani_status, POINT ptHotSpot)
{
	m_psprCursorImg = psprCursorImage;

	CopyMemory(m_sMouse_ani_status, psMouse_ani_status, sizeof(SMOUSE_ANI_STATUS) * ON_MOUSE_MAX_ANI_STATUS);

	m_ptHotSpot = ptHotSpot;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineBaseMouse::Free()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���콺�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineBaseMouse::SetMouse(bool bEnableCurosr)
{
	DWriteStringToFile("SetMouse() ����");

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
	m_bPlayedOnce				=	false;				// �ִϸ��̼� �� �� �÷��̰� ������.
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
		// �ִϸ��̼� �� �� �÷��̰� �����ٴ��� �˻�
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ο�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺 �ִϸ��̼� ���¸� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	m_bPlayedOnce				=	false;				// �ִϸ��̼� �� �� �÷��̰� ������.
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺 �ִϸ��̼� ���¸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineBaseMouse::GetAniStatus()
{
	return	m_siMouseAniStatus;
}

// ���콺 �ִϸ��̼��� �����°�?
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
