//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - SMS 메시지 출력부
//	File Name		: DirectX.cpp
//	Birth Date		: 2003. 06.
//	Creator			: 거 상 팀
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		뭐에 쓰는 놈인지 정확히는 모르겠으나 아마도 서피스와 관련된 작업을 할 때 쓰는 놈인 듯 합니다. -_-;
//		이것 역시 임진록때부터 적용되어왔던 소스인 듯..
//
//===================================================================================================

#include <GSL.h>
#include <main.h>
#include <clGame.h>
#include <Graphics16.h>

#include <Mouse.h>

#include <myfile.h>


#include "maindata.h"
#include <colortable.h>
#include <etc.h>
#include "help.h"

#include "yprg.h"
#include <Map.h>
#include "bindJXFile.h"

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : 공통 파서기

extern _clGame* pGame;

extern	char 				SavePath[];
extern	_MainData			GD;									// 게임의 주데이터 



extern XSPR GameSpr[];      // 일반적으로 사용될 스프라이트 

// 판넬용 면 
LPDIRECTDRAWSURFACE7		lpPannelSurface;
// 캐릭터 그림용
LPDIRECTDRAWSURFACE7		lpScreenBuffer;

// 창모드를 위해
HBITMAP                     g_hBattleScreenBitmap = NULL;
HDC                         g_hBattleScreenDC = NULL;


// 브리핑 화면용 
LPDIRECTDRAWSURFACE7		lpBriefingBuffer;
// 하이퍼텍스트용 
LPDIRECTDRAWSURFACE7		lpHelpBuffer;
// 스크롤용 
LPDIRECTDRAWSURFACE7		lpScrollBuffer;
// 브리핑 스크롤용
LPDIRECTDRAWSURFACE7		lpBriefingScrollBuffer;
// 브리핑 텍스트 출력용 
LPDIRECTDRAWSURFACE7		lpBriefingTextBuffer;


// 온라인용 
LPDIRECTDRAWSURFACE7		lpOnlineScreenSurface;

SHORT						FadeOutStep = 0;
unsigned char				OriginalCurrentPal[768];
SHORT						FadeInSwitch = FALSE;

extern int WindowModeSwitch;


BOOL CreateSurface()
{
	XSPR	spr;
	CHAR	szFileNameBuffer[1024];


	if(clGrp.CreateOffScreen(lpPannelSurface, clGrp.GetScreenXsize(), PANNEL_YSIZE, TRUE)==FALSE)
	{
		clGrp.Error("1r8732", "297skjd");
	}

	if (!WindowModeSwitch) {
		if(clGrp.CreateOffScreen(lpScreenBuffer, max(clGrp.GetScreenXsize(), CLIENT_SCREEN_XSIZE), clGrp.GetScreenYsize(), FALSE)==FALSE)
		{
			clGrp.Error("1r8732", "297skjd");
		}
	}
	else 
	{
	/*	g_hBattleScreenDC = CreateCompatibleDC(NULL);
		
		PBITMAPINFO pbmInfo = NULL;
		pbmInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFO) + (sizeof(RGBQUAD)*255));
//		ASSERT(pbmInfo && "GQGraphic::Init() ... memory allocate fail");
		ZeroMemory(pbmInfo, sizeof(BITMAPINFO) + (sizeof(RGBQUAD)*255));
		pbmInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pbmInfo->bmiHeader.biWidth			= max(clGrp.GetScreenXsize(), CLIENT_SCREEN_XSIZE);
		pbmInfo->bmiHeader.biHeight			= -clGrp.GetScreenYsize();
		pbmInfo->bmiHeader.biPlanes			= 1;
		pbmInfo->bmiHeader.biBitCount		= 8;
		pbmInfo->bmiHeader.biCompression	= BI_RGB;
		pbmInfo->bmiHeader.biSizeImage		= pbmInfo->bmiHeader.biWidth * abs(pbmInfo->bmiHeader.biHeight);
		pbmInfo->bmiHeader.biClrUsed		= 256;
		pbmInfo->bmiHeader.biClrImportant	= 0;

		char *buffer;

		
		g_hBattleScreenBitmap = CreateDIBSection(g_hBattleScreenDC, pbmInfo, DIB_RGB_COLORS, (void **)&buffer, NULL, 0);
		clGrp.CreateOffScreenWithBuffer(lpScreenBuffer,pbmInfo->bmiHeader.biWidth,clGrp.GetScreenYsize(),buffer);
		SelectObject(g_hBattleScreenDC,g_hBattleScreenBitmap);*/
		if(clGrp.CreateOffScreen(lpScreenBuffer, max(clGrp.GetScreenXsize(), CLIENT_SCREEN_XSIZE), clGrp.GetScreenYsize(), FALSE)==FALSE)
		{
			clGrp.Error("1r8732", "297skjd");
		}

	}

	if(clGrp.CreateOffScreen(lpBriefingBuffer, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), TRUE)==FALSE)
	{
		clGrp.Error("21r8732", "12329dfd7skjd");
	}
	if(clGrp.LockSurface(lpBriefingBuffer)==TRUE)
	{
		clGrp.FillBox32(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1, (UI08)TRANSCOLOR);
		clGrp.UnlockSurface(lpBriefingBuffer);
	}

	if(clGrp.CreateOffScreen(lpBriefingScrollBuffer, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), TRUE)==FALSE)
	{
		clGrp.Error("1r8732", "297skjdds");
	}

	if(clGrp.CreateOffScreen(lpHelpBuffer, SURFACE_WIDTH_HELP, SURFACE_HEIGHT_HELP, TRUE)==FALSE)
	{
		clGrp.Error("21rfds82", "12329df3fjd");
	}

	if(clGrp.CreateOffScreen(lpBriefingTextBuffer, MAX_BRIEFING_TEXT_XSIZE, MAX_BRIEFING_TEXT_YSIZE, TRUE)==FALSE)
	{
		clGrp.Error("1r87dsdff32", "2d37skjd");
	}
	
	if(clGrp.CreateOffScreen(lpOnlineScreenSurface, 640, 480, TRUE) == FALSE)
	{
		clGrp.Error("1r87dsdff32", "2d37skjd");
	}

	// 스크롤을 불러온다. 
	GetFileNamePath("fnt\\scroll.spr", SavePath, szFileNameBuffer);

	if(clGrp.LoadXspr(szFileNameBuffer, spr)==TRUE)
	{
		if(clGrp.CreateOffScreen(lpScrollBuffer, spr.Header.Xsize, spr.Header.Ysize, TRUE)==TRUE)
		{
			if(clGrp.LockSurface(lpScrollBuffer)==TRUE)
			{
				clGrp.PutImageT(0, 0, spr.Header.Xsize, spr.Header.Ysize, spr.Image);
				clGrp.UnlockSurface(lpScrollBuffer);
			}
		}
		else
		{
		   clGrp.Error("1r87dsf32", "2d37skjd");
		}

		clGrp.FreeXspr(spr);
	}

	return TRUE;
}

// 임의로 만든 서피스를 모두 해제한다. 
void CustomSurfaceRelease()
{

	if(lpPannelSurface)
	{
		lpPannelSurface->Release();
		lpPannelSurface=NULL;
	}

	if(lpScreenBuffer)
	{
		lpScreenBuffer->Release();
		lpScreenBuffer=NULL;
		if (WindowModeSwitch)
		{

			if (g_hBattleScreenDC) DeleteDC(g_hBattleScreenDC);
		}
	}

	if(lpBriefingBuffer)
	{
		lpBriefingBuffer->Release();
		lpBriefingBuffer=NULL;
	}


	if(lpHelpBuffer)
	{
		lpHelpBuffer->Release();
		lpHelpBuffer=NULL;
	}

	if(lpScrollBuffer)
	{
		lpScrollBuffer->Release();
		lpScrollBuffer=NULL;
	}

	if(lpBriefingScrollBuffer)
	{
		lpBriefingScrollBuffer->Release();
		lpBriefingScrollBuffer=NULL;
	}

	if(lpBriefingTextBuffer)
	{
		lpBriefingTextBuffer->Release();
		lpBriefingTextBuffer = NULL;
	}

	if(lpOnlineScreenSurface)
	{
		lpOnlineScreenSurface->Release();
		lpOnlineScreenSurface	=	NULL;
	}


}
void UpdateScreen()
{
	DrawMouse();
	if(clGrp.lpClipper)
	{
		clGrp.lpDDSPrimary->Blt(NULL, clGrp.lpDDSBack, NULL, DDBLT_WAIT, NULL);
	}
	else
	{
		clGrp.lpDDSPrimary->BltFast(0, 0, clGrp.lpDDSBack, NULL, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
	}
}


void SetGamePalette(unsigned char *pal)
{
	int i;

    for(i=0;i<256;i++)
	{
	      clGrp.PalEntries[i].peRed=  pal[i*3]*4;
	      clGrp.PalEntries[i].peGreen=pal[i*3+1]*4;
	      clGrp.PalEntries[i].peBlue= pal[i*3+2]*4;
	} 

	clGrp.SetPaletteEntries(0, 256, clGrp.PalEntries);

	// 현재의 팔레트를 복사해 둔다.
	memcpy(clGrp.CurrentPal, pal, 768);
}

// 게임에 사용될 팔레트를 불러온다. 
void _clGame::LoadPalette(unsigned char *pal)
{
	unsigned char temppal[768];

	SHORT i;

	if(pal == NULL)
	{
		if(clGrp.LoadOnlyPaletteFile("pal\\imjin2.pal", temppal)==FALSE)
		{
			clGrp.Error("1045", "Cannot Open Palette");
		}
	}
	else
		memcpy(temppal, pal, 768);

	for(i=0;i<768;i++)
	{
		clGrp.Palette[i]=temppal[i];
	}

	clGrp.Palette[255*3]  =63;
	clGrp.Palette[255*3+1]=63;
    clGrp.Palette[255*3+2]=63;


	// 임시로 시행한다. 
	for(i = 0;i<256;i++)
	{
		clGrp.TempPaletteTable[i] = clGrp.GetPalette16(clGrp.Palette[i*3]*4, clGrp.Palette[i*3+1]*4, clGrp.Palette[i*3+2]*4);
	}


	// 오리지날 팔레트를 저장해둔다.
	memcpy(OriginalGamePalette, clGrp.Palette, 768);

	// 번개용 팔레트를 설정한다. 
	for(i=0;i<256;i++)
	{
	   ThunderPalette[i*3]  =min(63, clGrp.Palette[i*3]+20);
	   ThunderPalette[i*3+1]=min(63, clGrp.Palette[i*3+1]+20);
       ThunderPalette[i*3+2]=min(63, clGrp.Palette[i*3+2]+20);
	}

}

// 흰색을 바꿔준다. 
void InitFadeOut()
{
	FadeOutStep = 0;
	memcpy(OriginalCurrentPal, clGrp.CurrentPal, 768);
}



BOOL FadeOut()
{
	int j=0;
	int temp=0;

	Sleep(1);

	clGrp.WaitForVerticalBlank();


    for(j=0;j<256;j++)
    {
		temp=OriginalCurrentPal[j*3];
	    if(clGrp.CurrentPal[j*3]>0)clGrp.CurrentPal[j*3]=(temp-( temp*FadeOutStep/100) );

	    temp=OriginalCurrentPal[j*3+1];
        if(clGrp.CurrentPal[j*3+1] >0)clGrp.CurrentPal[j*3+1]=(temp-( temp*FadeOutStep/100) );

		temp=OriginalCurrentPal[j*3+2];
	    if(clGrp.CurrentPal[j*3+2] >0)clGrp.CurrentPal[j*3+2]=(temp-( temp*FadeOutStep/100) );

      clGrp.PalEntries[j].peRed   = clGrp.CurrentPal[j*3]*4;
      clGrp.PalEntries[j].peGreen = clGrp.CurrentPal[j*3+1]*4;
      clGrp.PalEntries[j].peBlue  = clGrp.CurrentPal[j*3+2]*4;
    }

	clGrp.SetPaletteEntries(0, 256, clGrp.PalEntries);

	if(FadeOutStep>=100)
	{
		if(clGrp.lpDDSPrimary->IsLost() != DD_OK)
		{
			clGrp.lpDDSPrimary->Restore();
		}
	   clGrp.ClearSurface(clGrp.lpDDSPrimary);

	   SetGamePalette(clGrp.Palette);
	   InitDarkTable(clGrp.Palette);
	   SetMouse(MOUSE_NORMAL);
 
	   return TRUE;
	}

	FadeOutStep+=5;

	return FALSE;
}


void InitFadeIn()
{
	FadeOutStep=0;
	FadeInSwitch=TRUE;
}

BOOL FadeIn()
{

	if(FadeInSwitch==FALSE)return FALSE;

	clGrp.WaitForVerticalBlank();


	int j=0;
	int temp=0;

	Sleep(1);

	for(j=0;j<256;j++)
    {


		temp=OriginalCurrentPal[j*3];
	    clGrp.CurrentPal[j*3]=temp*FadeOutStep/100;

	    temp=OriginalCurrentPal[j*3+1];
        clGrp.CurrentPal[j*3+1]=temp*FadeOutStep/100;

		temp=OriginalCurrentPal[j*3+2];
	    clGrp.CurrentPal[j*3+2]=temp*FadeOutStep/100;

      clGrp.PalEntries[j].peRed   = clGrp.CurrentPal[j*3]*4;
      clGrp.PalEntries[j].peGreen = clGrp.CurrentPal[j*3+1]*4;
      clGrp.PalEntries[j].peBlue  = clGrp.CurrentPal[j*3+2]*4;
    }
    
	clGrp.SetPaletteEntries(0, 256, clGrp.PalEntries);	

	if(FadeOutStep>=100)
	{
	   SetGamePalette(OriginalCurrentPal);

	   FadeInSwitch=FALSE;
	   return TRUE;
	}

	FadeOutStep+=5;


	return FALSE;
}


SI32 GetTextFormat(char *nation, char *name, char *kind)
{
	OnlineCommonParser	ocp;
	CHAR	filename[128];
	FILE	*fp				= NULL;
	HDC		hdc;
	SI32	siTemp			= 0;

	// actdoll (2004/02/23 15:55) : 
	//	다른 방식으로 해당 정보를 로딩한다.
	strcpy( filename, pGame->m_pszHQNation );		// 앞에 해당 국가의 폴더명 붙이고
	strcat( filename, "\\GameFontInfo.dat" );		// 해당 파일명을 붙인다.

	if( (fp = BindJXFile::GetInstance()->GetFile( filename )) == NULL )
	{
		clGrp.Error("GameFontInfo", "%s - File Open Error!", filename);
		return -1;
	}

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", filename );
	ocp.SetErrFileName( pszErrName );

	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "GameFontInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", filename, iRet, iErrLine, bLess );
		fclose( fp );
		return -1;
	}

	// 자료 받기
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		char	pszNation[64], pszFontName[64], pszWindowFont[64];
		SI32	siSize, siDepth;					// 번호, 크기, 굵기
		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		ocp.GetValue( pszNation,		63, iLineNo, iColNo );		// 국가명
		ocp.GetValue( pszFontName,		63,	iLineNo, iColNo );		// 지정 폰트명
		ocp.GetValue( pszWindowFont,	63,	iLineNo, iColNo );		// 윈도 폰트명
		ocp.GetValue( siSize,				iLineNo, iColNo );		// 폰트 크기
		ocp.GetValue( siDepth,				iLineNo, iColNo );		// 폰트 굵기

		// 맞는 거 찾아내기
		if( stricmp( nation, pszNation ) == 0 && stricmp( name, pszFontName ) == 0 )
		{
			if( stricmp( kind, "Size" ) == 0 )				// 사이즈를 알려달라는 주문이다.
			{
				hdc			= GetDC( NULL );
				siTemp		= GetDeviceCaps( hdc, LOGPIXELSY );
				iRet		= -MulDiv( siSize, siTemp, 72 );
				ReleaseDC( NULL, hdc );
				return iRet;
			}
			else if( stricmp( kind, "Thick" ) == 0 )		// 글씨 두께를 알려달라는 주문이다.
			{
				iRet	= siDepth;
				return iRet;
			}
			else											// 이도 저도 아니면 에러
			{
				fclose( fp );
				ocp.Release();
				return -1;
			}
		}

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "GameFontInfo Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", filename, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			ocp.Release();
			return -1;
		}
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
	}
	
	
	return iRet;
}


BOOL GetTextFormatFont(char *nation, char *name, char *font)
{
	OnlineCommonParser	ocp;
	FILE	*fp = NULL;
	CHAR	buffer[1024];
	CHAR	filename[128];

	// actdoll (2004/02/23 15:55) : 
	//	다른 방식으로 해당 정보를 로딩한다.
	strcpy( filename, pGame->m_pszHQNation );		// 앞에 해당 국가의 폴더명 붙이고
	strcat( filename, "\\GameFontInfo.dat" );		// 해당 파일명을 붙인다.

	// savepath를 고려하여 화일이름을 보정한다. 
	GetFileNamePath(filename, SavePath, buffer);
	if( (fp = BindJXFile::GetInstance()->GetFile( filename )) == NULL )
	{
		clGrp.Error("GameFontInfo", "%s - File Open Error!", filename);
		return FALSE;
	}

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", filename );
	ocp.SetErrFileName( pszErrName );

	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "GameFontInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", filename, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 받기
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		char	pszNation[64], pszFontName[64], pszWindowFont[64];
		SI32	siSize, siDepth;					// 번호, 크기, 굵기
		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		ocp.GetValue( pszNation,		63, iLineNo, iColNo );		// 국가명
		ocp.GetValue( pszFontName,		63,	iLineNo, iColNo );		// 지정 폰트명
		ocp.GetValue( pszWindowFont,	63,	iLineNo, iColNo );		// 윈도 폰트명
		ocp.GetValue( siSize,				iLineNo, iColNo );		// 폰트 크기
		ocp.GetValue( siDepth,				iLineNo, iColNo );		// 폰트 굵기

		// 맞는 거 찾아내기
		if( stricmp( nation, pszNation ) == 0 && stricmp( name, pszFontName ) == 0 )
		{
			strcpy( font, pszWindowFont );
			fclose( fp );
			ocp.Release();
			return TRUE;
		}

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "GameFontInfo Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", filename, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			ocp.Release();
			return FALSE;
		}
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
	}
	
	return FALSE;
}




//---------------------------------------------------------------------------------------------------
// 2001.11.09	hojae_append
//---------------------------------------------------------------------------------------------------

UI08 *Return_Image(SI32 file)
{
	return GameSpr[file].Image;
};

// actdoll (031229)
XSPR_HEADER *Return_Header(SI32 file)
{
	return &GameSpr[file].Header;
};

SI32 GetFileXsize(SI32 file)
{
	return GameSpr[file].Header.Xsize;
}

SI32 GetFileYsize(SI32 file)
{
	return GameSpr[file].Header.Ysize;
}

//----------------------------------------------------------------------------------------
void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}


void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}


//---------------------------------------------------------------------------------------
void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteLightImjinT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinCT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinCT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinRT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinCRT(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinCRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteShadowT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteShadowT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteShadowCT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteShadowCT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteShadowRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteShadowRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteShadowCRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteShadowCRT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}



//---------------------------------------------------------------------------------------------------
void Draw_PutImage32(SI32 startx, SI32 starty, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutImage32(startx, starty, 
		GameSpr[file].Header.Xsize,
		GameSpr[file].Header.Ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutImage32(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutImage32(startx, starty, 
		xsize,
		ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteT(startx, starty, 
		xsize,
		ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

     clGrp.PutSpriteRT(startx, starty, xsize, ysize, 
		 &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

//---------------------------------------------------------------------------------------------------

void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteT(startx, starty, 
		xsize,
		ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
		ctable);
}

void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteRT(startx, starty, 
		xsize,
		ysize,
		&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
		ctable);
}

//---------------------------------------------------------------------------------------------------

void Draw_PutSpriteLightT(SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightT(xsize, ysize,
			GameSpr[file].Header.Xsize, 
			GameSpr[file].Header.Ysize,
			&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightCT(SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightCT(xsize, ysize,
			GameSpr[file].Header.Xsize, 
			GameSpr[file].Header.Ysize,
			&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightRT(SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightRT(xsize, ysize,
			GameSpr[file].Header.Xsize, 
			GameSpr[file].Header.Ysize,
			&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightCRT(SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightCRT(xsize, ysize,
			GameSpr[file].Header.Xsize, 
			GameSpr[file].Header.Ysize,
			&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}


//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

     clGrp.PutSpriteLightT(startx, starty, 
	                       xsize,  ysize, 
   			               &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

     clGrp.PutSpriteLightCT(startx, starty, 
	                       xsize,  ysize, 
   			               &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

     clGrp.PutSpriteLightRT(startx, starty, 
	                       xsize,  ysize, 
   			               &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

     clGrp.PutSpriteLightCRT(startx, starty, 
	                       xsize,  ysize, 
   			               &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}
//---------------------------------------------------------------------------------------------------

void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

  clGrp.PutSpriteLightT(startx, starty, 
		                  GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
				         &GameSpr[file].Image[GameSpr[file].Header.Start[font]],
 				          ctable,
						  step);
}

void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

  clGrp.PutSpriteLightCT(startx, starty, 
		                  GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
				         &GameSpr[file].Image[GameSpr[file].Header.Start[font]],
 				          ctable,
						  step);
}

void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightRT(startx, starty, 
		                  GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
				         &GameSpr[file].Image[GameSpr[file].Header.Start[font]],
 				          ctable,
						  step);
}

void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightCRT(startx, starty, 
		                  GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
				         &GameSpr[file].Image[GameSpr[file].Header.Start[font]],
 				          ctable,
						  step);
}
//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, step);
}

void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightCT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, step);
}

void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightRT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, step);
}

void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightCRT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, step);
}
//---------------------------------------------------------------------------------------------------
void Draw_PutSpriteDotT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteDotT(startx, starty, xsize, ysize, 
		   &GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteDotCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteDotCT(startx, starty, xsize, ysize, 
		   &GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteDotRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteDotRT(startx, starty, xsize, ysize, 
		   &GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

void Draw_PutSpriteDotCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteDotCRT(startx, starty, xsize, ysize, 
		   &GameSpr[file].Image[GameSpr[file].Header.Start[font]], ctable);
}

//--------------------------------------------------------------------------------------------------------
void Draw_PutSpriteLightImjinT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinT(startx, starty, xsize, ysize,
                     &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinCT(startx, starty, xsize, ysize,
                     &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinRT(startx, starty, xsize, ysize,
                     &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}

void Draw_PutSpriteLightImjinCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font)
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteLightImjinCRT(startx, starty, xsize, ysize,
                     &GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
}
//--------------------------------------------------------------------------------------------------------
void Draw_PutSpriteWaterShadowT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256])
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteWaterShadowT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, watershadowtable);
}

void Draw_PutSpriteWaterShadowCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256])
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteWaterShadowCT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, watershadowtable);
}
void Draw_PutSpriteWaterShadowRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256])
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteWaterShadowRT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, watershadowtable);
}

void Draw_PutSpriteWaterShadowCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256])
{
	if(GameSpr[file].Image == NULL)		return;

	clGrp.PutSpriteWaterShadowCRT(startx, starty, xsize,  ysize, 
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]],
						ctable, watershadowtable);
}

BOOL Error(char * szCaption, char*szFormat,... )
{
	return clGrp.Error(szCaption, szFormat);
}

BOOL IsInClipArea(SI32 file, SI32 x, SI32 y)
{
	return clGrp.IsInClipArea(x, y, x + GameSpr[file].Header.Xsize-1, y + GameSpr[file].Header.Ysize-1);

}