//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - OnlineWorld.cpp의 인트로 화면 처리
//	File Name		: OnlineWorld-IntroScreen.cpp
//	Birth Date		: 2004. 12. 11.
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 맨 처음의 인트로 화면을 출력시켜 준다.
//		OnlineWorld.cpp를 너무 많은 사람들이 컨트롤 하고 있으므로 일단 여기서 처리하도록 한다.
//
//===================================================================================================

#include	<GSL.h>
#include	<clGame.h>
#include	"bindJxFile.h"
#include	"OnlineCommonParser.h"		// actdoll (2004/06/10 15:42) : 공용 파서기 추가


extern _clGame* pGame;
extern int WindowModeSwitch;

// actdoll (2004/11/15 16:46) : 
//	이 루틴은 어차피 게임 실행중에 한번밖에 거치지 않으므로 복잡한 작업을 피하기 위해 변수들을 전역부에 선언해놓는다.
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

const	int		DF_COUNT_INTRO_MAX_SCENE		= 10;		// 인트로 화면의 최대 갯수
const	int		DF_VALUE_INTRO_FADEFRAMETIME	= 20;		// 페이드 인 아웃 한 프레임에 걸리는 시간(ms)


unsigned int	g_uiIntroPollOrder		= EN_VALUE_INTRO_POLL_FADEIN_READY;
unsigned int	g_uiCurrentSceneIndex	= 0;		// 현재 찍고 있는 화면 번호
int				g_iFadeCountScene		= 0;		// 100이 되면 OK

unsigned char	g_uiOriginalPallete[768]= {0,};
unsigned char	g_uiCurrentPallete[768]	= {0,};

XSPR			g_sprIntroScreen[DF_COUNT_INTRO_MAX_SCENE]		= {0,};		// 인트로 등장하는 화면 스프라이트
DWORD			g_dwScreenDelay[DF_COUNT_INTRO_MAX_SCENE]		= {0,};		// 인트로 뿌리고 있는 시간

//-----------------------------------------------------------------------------
// Name: IntroPoll()
// Code: actdoll (2004-11-15)
// Desc: 폴
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
// Desc: 인트로 폴 초기화
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroInit()
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	int					iRet, iErrLine, iCount;
	BOOL				bLess;

	// 원본 팔레트 로딩
	clGrp.LoadOnlyPaletteFile("pal\\IntroCI.pal", g_uiOriginalPallete );

	// 화면 클리어
	clGrp.ClearSurface(clGrp.lpDDSPrimary);

	// 일단 여기서 인트로 데이터 판독
	if((fp = BindJXFile::GetInstance()->GetFile( "IntroInfo.txt" )) != NULL)
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\Err_IntroInfo.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "IntroInfo Error : Cannot Init! [ Ret-%d | Line-%d | Less-%d ]", iRet, iErrLine, bLess );
			fclose( fp );
			return;
		}

		// 자료 받기
		char	pszFileName[MAX_PATH];
		int		iLineNo, iColNo;
		int		iDummy;
		DWORD	dwDelay;
		BOOL	bRet = false;

		BOOL	bIsActive;
		iCount				= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )				break;		// 데이터 라인 추출
			if( iCount >= DF_COUNT_INTRO_MAX_SCENE )	break;		// 로딩 스크린이 10개가 넘어갔다면 취소

			ocp.GetValue( iDummy,						iLineNo, iColNo );		// 설명문(사용안함)
			ocp.GetValue( pszFileName, MAX_PATH - 1,	iLineNo, iColNo );		// 파일명
			ocp.GetValue( (int&)dwDelay,				iLineNo, iColNo );		// 표시시간

			// 한국이 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_KOREA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 대만이 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_TAIWAN && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 일본이 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_JAPAN && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 홍콩이 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_HONGKONG && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 중국이 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_CHINA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 인도네시아가 활성화라면 로딩하고 돌아간다.
			ocp.GetValue( (int&)bIsActive,				iLineNo, iColNo );
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA && bIsActive )
			{
				clGrp.LoadXspr( pszFileName, g_sprIntroScreen[iCount] );	// 로딩하고
				if( g_sprIntroScreen[iCount].Image )						// 이미지가 존재한다면 다음거 찾는다.
				{
					g_dwScreenDelay[iCount]	= dwDelay;
					iCount++;
					continue;
				}
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
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

	// 만약 로딩된 놈이 하나도 없다면 그냥 리턴
	if( iCount <= 0 )		g_uiIntroPollOrder		= EN_VALUE_INTRO_POLL_EXIT;	
}

//-----------------------------------------------------------------------------
// Name: IntroInit()
// Code: actdoll (2004-11-15)
// Desc: 인트로 폴 해제
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroFree()
{
	// 화면 클리어
	clGrp.ClearSurface(clGrp.lpDDSPrimary);

	// 로딩한 스프라이트 클리어
	for( int i=0; i<DF_COUNT_INTRO_MAX_SCENE; i++ )
	{
		if( !(XSPR*)(g_sprIntroScreen + i)->Image )	break;
		clGrp.FreeXspr( (XSPR&)*(g_sprIntroScreen + i) );
	}
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn_Ready()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 페이드 인 준비
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeIn_Ready()
{
	// 현재 팔레트 엔트리 갱신
	ZeroMemory( clGrp.PalEntries, sizeof(PALETTEENTRY)*256 );
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// 출력하고자 하는 해당 서피스를 한번 찍어준다.
	if(clGrp.LockSurface(clGrp.lpDDSBack) == TRUE)
	{
		// 언놈을 찍어줄 것인가 결정
		XSPR	*pDummy	= g_sprIntroScreen + g_uiCurrentSceneIndex;
		clGrp.PutSpriteT	( 0, 0, pDummy->Header.Xsize,	pDummy->Header.Ysize,	pDummy->Image );
		clGrp.UnlockSurface	( clGrp.lpDDSBack );
	}

	if(WindowModeSwitch)	UpdateScreen( clGrp.lpDDSBack,FALSE );
	else					UpdateScreen( clGrp.lpDDSBack );

	// 페이드 카운트 0
	g_iFadeCountScene	= 0;

	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEIN;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 페이드 인
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeIn()
{
	clGrp.WaitForVerticalBlank();

	// 페이드 인 개시
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

	// 화면 갱신
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// 씬이 다 돌았을 경우는 다음으로 맞춘다.
	if( g_iFadeCountScene >= 100 )
	{
		g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_DRAW;
		return;
	}

	// 아직 안됐다면 찍고 돈다.
	g_iFadeCountScene	+= 5;
	Sleep( DF_VALUE_INTRO_FADEFRAMETIME );
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_Draw()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 계속 그리기
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_Draw()
{
	// 잠시동안 기둘리고 페이드 아웃 개시
	Sleep( g_dwScreenDelay[g_uiCurrentSceneIndex] );
	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEOUT_READY;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeOut_Ready()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 페이드 아웃 준비
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeOut_Ready()
{
	// 페이드 카운트 0
	g_iFadeCountScene	= 100;
	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FADEOUT;
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeOut()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 페이드 아웃
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_FadeOut()
{
	clGrp.WaitForVerticalBlank();

	// 페이드 아웃 개시
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

	// 화면 갱신
	clGrp.SetPaletteEntries( 0, 256, clGrp.PalEntries );

	// 씬이 다 돌았을 경우는 다음으로 맞춘다.
	if( g_iFadeCountScene <= 0 )
	{
		g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_FINISH;
		return;
	}

	// 아직 안됐다면 찍고 돈다.
	g_iFadeCountScene	-= 5;
	if( g_iFadeCountScene < 0 )	g_iFadeCountScene = 0;
	Sleep( DF_VALUE_INTRO_FADEFRAMETIME );
}

//-----------------------------------------------------------------------------
// Name: IntroPoll_FadeIn()
// Code: actdoll (2004-11-15)
// Desc: 폴 - 작업 종료
//-----------------------------------------------------------------------------
void	cltOnlineWorld::IntroPoll_Finish()
{
	// 일단 진행으로 맞춘다.
	g_uiIntroPollOrder = EN_VALUE_INTRO_POLL_FADEIN_READY;

	// 장면 카운트 늘려준다.
	g_uiCurrentSceneIndex++;

	// 만약 카운트가 넘어갈 경우 종료
	if( g_uiCurrentSceneIndex >= DF_COUNT_INTRO_MAX_SCENE )	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_EXIT;

	// 만약 다음장면이 존재하지 않는다면 여기서 폴 종료
	if( !g_sprIntroScreen[g_uiCurrentSceneIndex].Image )	g_uiIntroPollOrder	= EN_VALUE_INTRO_POLL_EXIT;
}

