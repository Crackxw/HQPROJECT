//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - OnlineWorld.cpp�� ��Ʈ�� ȭ�� ó��
//	File Name		: OnlineWorld-IntroScreen.cpp
//	Birth Date		: 2004. 12. 11.
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �� ó���� ��Ʈ�� ȭ���� ��½��� �ش�.
//		OnlineWorld.cpp�� �ʹ� ���� ������� ��Ʈ�� �ϰ� �����Ƿ� �ϴ� ���⼭ ó���ϵ��� �Ѵ�.
//
//===================================================================================================

#include	<GSL.h>
#include	<clGame.h>
#include	"bindJxFile.h"
#include	"OnlineCommonParser.h"		// actdoll (2004/06/10 15:42) : ���� �ļ��� �߰�


extern _clGame* pGame;
extern int WindowModeSwitch;

// actdoll (2004/11/15 16:46) : 
//	�� ��ƾ�� ������ ���� �����߿� �ѹ��ۿ� ��ġ�� �����Ƿ� ������ �۾��� ���ϱ� ���� �������� �����ο� �����س��´�.
enum	EN_VALUE_INTRO_POLL_ORDER
{
	EN_VALUE_INTRO_POLL_FADEIN_READY,
	EN_VALUE_INTRO_POLL_FADEIN,
	EN_VALUE_INTRO_POLL_DRAW,
	EN_VALUE_INTRO_POLL_FADEOUT_READY,
	EN_VALUE_INTRO_POLL_FADEOUT,
	EN_VALUE_INTRO_POLL_FINISH,
	EN_VALUE_INTRO_POLL_EXIT
};

const	int		DF_COUNT_INTRO_MAX_SCENE		= 10;		// ��Ʈ�� ȭ���� �ִ� ����
const	int		DF_VALUE_INTRO_FADEFRAMETIME	= 20;		// ���̵� �� �ƿ� �� �����ӿ� �ɸ��� �ð�(ms)


unsigned int	g_uiIntroPollOrder		= EN_VALUE_INTRO_POLL_FADEIN_READY;
unsigned int	g_uiCurrentSceneIndex	= 0;		// ���� ��� �ִ� ȭ�� ��ȣ
int				g_iFadeCountScene		= 0;		// 100�� �Ǹ� OK

unsigned char	g_uiOriginalPallete[768]= {0,};
unsigned char	g_uiCurrentPallete[768]	= {0,};

XSPR			g_sprIntroScreen[DF_COUNT_INTRO_MAX_SCENE]		= {0,};		// ��Ʈ�� �����ϴ� ȭ�� ��������Ʈ
DWORD			g_dwScreenDelay[DF_COUNT_INTRO_MAX_SCENE]		= {0,};		// ��Ʈ�� �Ѹ��� �ִ� �ð�

//-----------------------------------------------------------------------------
// Name: IntroPoll()
// Code: actdoll (2004-11-15)
// Desc: ��
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll()
{
	IntroInit();

	while( g_uiIntroPollOrder != EN_VALUE_INTRO_POLL_EXIT )
	{
		switch( g_uiIntroPollOrder )
		{
			case EN_VALUE_INTRO_POLL_FADEIN_READY:
				IntroPoll_FadeIn_Ready();
				break;

			case EN_VALUE_INTRO_POLL_FADEIN:
				IntroPoll_FadeIn();
				break;
				
			case EN_VALUE_INTRO_POLL_DRAW:
				IntroPoll_Draw();
				break;

			case EN_VALUE_INTRO_POLL_FADEOUT_READY:
				IntroPoll_FadeOut_Ready();
				break;

			case EN_VALUE_INTRO_POLL_FADEOUT:
				IntroPoll_FadeOut();
				break;
				
			case EN_VALUE_INTRO_POLL_FINISH:
				IntroPoll_Finish();
				break;
		}
	}

	IntroFree();
}

//-----------------------------------------------------------------------------
// Name: IntroInit()
// Code: actdoll (2004-11-15)
// Desc: ��Ʈ�� �� �ʱ�ȭ
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroInit()
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	int					iRet, iErrLine, iCount;
	BOOL				bLess;

	// ���� �ȷ�Ʈ �ε�
	clGrp.LoadOnlyPaletteFile("pal\\IntroCI.pal", g_uiOriginalPallete );

	// ȭ�� Ŭ����
	clGrp.ClearSurface(clGrp.lpDDSPrimary);

	// �ϴ� ���⼭ ��Ʈ�� ������ �ǵ�
	if((fp = BindJXFile::GetInstance()->GetFile( "IntroInfo.txt" )) != NULL)
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\Err_IntroInfo.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "IntroInfo Error : Cannot Init! [ Ret-%d | Line-%d | Less-%d ]", iRet, iErrLine, bLess );
			fclose( fp );
			return;
		}

		// �ڷ� �ޱ�
		char	pszFileName[MAX_PATH];
		int		iLineNo, iColNo;
		int		iDummy;
		DWORD	dwDelay;
		BOOL	bRet = false;

		BOOL	bIsActive;
		iCount				= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )				break;		// ������ ���� ����
			if( iCount >= DF_COUNT_INTRO_MAX_SCENE )	break;		// �ε� ��ũ���� 10���� �Ѿ�ٸ� ���

			ocp.GetValue( iDummy,						iLineNo, iColNo );		// ����(������)
			ocp.GetValue( pszFileName, MAX_PATH - 1,	iLineNo, iColNo );		// ���ϸ�
			ocp.GetValue( (int&)dwDelay,				iLineNo, iColNo );		// ǥ�ýð�

			// �ѱ��� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_KOREA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// �븸�� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_TAIWAN && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// �Ϻ��� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_JAPAN && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// ȫ���� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_HONGKONG && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// �߱��� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_CHINA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// �ε��׽þư� Ȱ��ȭ��� �ε��ϰ� ���ư���.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// �ε��ϰ�
				if( g_sprIntroScreen[iCount].Image )						// �̹����� �����Ѵٸ� ������ ã�´�.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "IntroText Error : File Doesn't Exist!" );
		return;
	}

	// ���� �ε��� ���� �ϳ��� ���ٸ� �׳� ����
	if( iCount <= 0 )		g_uiIntroPollOrder		= EN_VALUE_INTRO_POLL_EXIT;	
}

//-----------------------------------------------------------------------------
// Name: IntroInit()
// Code: actdoll (2004-11-15)
// Desc: ��Ʈ�� �� ����
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroFree()
{
	// ȭ�� Ŭ����
	clGrp.ClearSurface(clGrp.lpDDSPrimary);

	// �ε��� ��������Ʈ Ŭ����
	for( int i=0; i<DF_COUNT_INTRO_MAX_SCENE; i++ )
	{
		if( !(XSPR*)(g_sprIntroScreen + i)->Image )	break;
		clGrp.FreeXspr( (XSPR&)*(g_sprIntroScreen + i) );
	}
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn_Ready()
// Code: actdoll (2004-11-15)
// Desc: �� - ���̵� �� �غ�
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeIn_Ready()
{
	// ���� �ȷ�Ʈ ��Ʈ�� ����
	ZeroMemory( clGrp.PalEntries, sizeof(PALETTEENTRY)*256 );
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// ����ϰ��� �ϴ� �ش� ���ǽ��� �ѹ� ����ش�.
	if(clGrp.LockSurface(clGrp.lpDDSBack) == TRUE)
	{
		// ����� ����� ���ΰ� ����
		XSPR	*pDummy	= g_sprIntroScreen + g_uiCurrentSceneIndex;
		clGrp.PutSpriteT	( 0, 0, pDummy->Header.Xsize,	pDummy->Header.Ysize,	pDummy->Image );
		clGrp.UnlockSurface	( clGrp.lpDDSBack );
	}

	if(WindowModeSwitch)	UpdateScreen( clGrp.lpDDSBack,FALSE );
	else					UpdateScreen( clGrp.lpDDSBack );

	// ���̵� ī��Ʈ 0
	g_iFadeCountScene	= 0;

	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEIN;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn()
// Code: actdoll (2004-11-15)
// Desc: �� - ���̵� ��
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeIn()
{
	clGrp.WaitForVerticalBlank();

	// ���̵� �� ����
	int		iPalIndex;
	int		temp;
	for( int i=0; i<256; i++)
	{
		iPalIndex	= i * 3;
		temp = g_uiOriginalPallete[iPalIndex];
		clGrp.CurrentPal[iPalIndex]= temp * g_iFadeCountScene / 100;
		
		temp=g_uiOriginalPallete[iPalIndex+1];
        clGrp.CurrentPal[iPalIndex+1]= temp * g_iFadeCountScene / 100;
		
		temp=g_uiOriginalPallete[iPalIndex+2];
		clGrp.CurrentPal[iPalIndex+2]= temp * g_iFadeCountScene / 100;
		
		clGrp.PalEntries[i].peRed   = clGrp.CurrentPal[iPalIndex] << 2;		// * 4
		clGrp.PalEntries[i].peGreen = clGrp.CurrentPal[iPalIndex+1] << 2;	// * 4
		clGrp.PalEntries[i].peBlue  = clGrp.CurrentPal[iPalIndex+2] << 2;	// * 4
	}

	// ȭ�� ����
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// ���� �� ������ ���� �������� �����.
	if( g_iFadeCountScene >= 100 )
	{
		g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_DRAW;
		return;
	}

	// ���� �ȵƴٸ� ��� ����.
	g_iFadeCountScene	+= 5;
	Sleep( DF_VALUE_INTRO_FADEFRAMETIME );
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_Draw()
// Code: actdoll (2004-11-15)
// Desc: �� - ��� �׸���
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_Draw()
{
	// ��õ��� ��Ѹ��� ���̵� �ƿ� ����
	Sleep( g_dwScreenDelay[g_uiCurrentSceneIndex] );
	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEOUT_READY;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeOut_Ready()
// Code: actdoll (2004-11-15)
// Desc: �� - ���̵� �ƿ� �غ�
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeOut_Ready()
{
	// ���̵� ī��Ʈ 0
	g_iFadeCountScene	= 100;
	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEOUT;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeOut()
// Code: actdoll (2004-11-15)
// Desc: �� - ���̵� �ƿ�
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeOut()
{
	clGrp.WaitForVerticalBlank();

	// ���̵� �ƿ� ����
	int		iPalIndex;
	int		temp;
	for( int i=0; i<256; i++)
	{
		iPalIndex	= i * 3;
		temp = g_uiOriginalPallete[iPalIndex];
		clGrp.CurrentPal[iPalIndex]= temp * g_iFadeCountScene / 100;
		
		temp=g_uiOriginalPallete[iPalIndex+1];
        clGrp.CurrentPal[iPalIndex+1]= temp * g_iFadeCountScene / 100;
		
		temp=g_uiOriginalPallete[iPalIndex+2];
		clGrp.CurrentPal[iPalIndex+2]= temp * g_iFadeCountScene / 100;
		
		clGrp.PalEntries[i].peRed   = clGrp.CurrentPal[iPalIndex] << 2;		// * 4
		clGrp.PalEntries[i].peGreen = clGrp.CurrentPal[iPalIndex+1] << 2;	// * 4
		clGrp.PalEntries[i].peBlue  = clGrp.CurrentPal[iPalIndex+2] << 2;	// * 4
	}

	// ȭ�� ����
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// ���� �� ������ ���� �������� �����.
	if( g_iFadeCountScene <= 0 )
	{
		g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FINISH;
		return;
	}

	// ���� �ȵƴٸ� ��� ����.
	g_iFadeCountScene	-= 5;
	if( g_iFadeCountScene < 0 )	g_iFadeCountScene = 0;
	Sleep( DF_VALUE_INTRO_FADEFRAMETIME );
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn()
// Code: actdoll (2004-11-15)
// Desc: �� - �۾� ����
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_Finish()
{
	// �ϴ� �������� �����.
	g_uiIntroPollOrder = EN_VALUE_INTRO_POLL_FADEIN_READY;

	// ��� ī��Ʈ �÷��ش�.
	g_uiCurrentSceneIndex++;

	// ���� ī��Ʈ�� �Ѿ ��� ����
	if( g_uiCurrentSceneIndex >= DF_COUNT_INTRO_MAX_SCENE )	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_EXIT;

	// ���� ��������� �������� �ʴ´ٸ� ���⼭ �� ����
	if( !g_sprIntroScreen[g_uiCurrentSceneIndex].Image )	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_EXIT;
}

