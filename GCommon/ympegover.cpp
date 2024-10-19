//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <main.h>
#include <ympegover.h>
#include <Mouse.h>


extern _InputDevice				IpD;

static	VOID				RestoreVideoMode();

_MpegPlayer					MpegPlayer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _MpegPlayer::_MpegPlayer()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_MpegPlayer::_MpegPlayer()
{
	m_IsInitialize = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _MpegPlayer::_MpegPlayer()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_MpegPlayer::~_MpegPlayer()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_MpegPlayer::Init()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_MpegPlayer::Init()
{
	ClearSurfaceToColor(clGrp.lpDDSPrimary, 0);
	ClearSurfaceToColor(clGrp.lpDDSBack, 0);

	if(clGrp.ChangeVideoMode(MODE_GRAPHICS640_480_16) == FALSE)
	{
		clGrp.Error("FKJE8567", "YMPEGOVER000");
		return FALSE;
	}

	m_IsInitialize  = TRUE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_MpegPlayer::Load(CHAR* lpszFileName)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_MpegPlayer::Load(CHAR* lpszFileName)
{
	if(m_IsInitialize == FALSE)
		return FALSE;

	if(!m_Mpeg.Load(lpszFileName, clGrp.lpDD7, clGrp.lpDDSBack))
	{		
		Free();
		return FALSE;
	}


	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _MpegPlayer::Action()
/// 설명 : 새로운 프레임으로 갱신한다.
/// 리턴값 : 0 - 계속 진행중이다.
///          1 - 사용자 키가 눌리어져서 중단되었다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	 _MpegPlayer::Action()
{
	BOOL	IsBreakKey = FALSE;	
	
	if(m_IsInitialize == TRUE)
	{
		if(m_Mpeg.IsLoad() == TRUE)
		{
			IsBreakKey = (GetAsyncKeyState(VK_ESCAPE) < 0) || (GetAsyncKeyState(VK_SPACE) < 0) ||
						 (GetAsyncKeyState(VK_RETURN) < 0 || IpD.LeftPressSwitch==TRUE );

			

			if(m_Mpeg.IsPlay() == FALSE || (IsBreakKey == TRUE))				
			{
				if(m_Mpeg.IsPlay() == TRUE)
				{
					m_Mpeg.Stop();						
				}
			}
			else 
			{			
				m_Mpeg.Action();
				m_Mpeg.UpdateScreen(clGrp.lpDDSBack, clGrp.lpDDSPrimary);			
			}		
		}	
	}

	if(IsBreakKey)	return 1;
	else			return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _MpegPlayer::IsInitialize()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _MpegPlayer::IsInitialize()
{
	return m_IsInitialize;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _MpegPlayer::IsPlay()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	 _MpegPlayer::IsPlay()
{
	if(m_IsInitialize == TRUE)
	{
		if(m_Mpeg.IsLoad() == TRUE)
		{
			return m_Mpeg.IsPlay();
		}
	}

	return	FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _MpegPlayer::Play(BOOL IsFullScreen, SHORT Width, SHORT Height)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _MpegPlayer::Play(BOOL IsFullScreen, SHORT Width, SHORT Height, SHORT ScreenXsize, SHORT ScreenYsize)
{
	if(m_IsInitialize == TRUE)
	{
		if(m_Mpeg.IsLoad() == TRUE)
		{	
			if(IsPlay() == FALSE)	
			{			
				m_Mpeg.Play(IsFullScreen, Width, Height, ScreenXsize, ScreenYsize);				
			}


			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	 _MpegPlayer::Free()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	 _MpegPlayer::Free()
{	
	if(m_IsInitialize == TRUE)
	{
		if(m_Mpeg.IsLoad() == TRUE)
		{
			if(IsPlay() == TRUE)	m_Mpeg.Stop();			
			m_Mpeg.Free();
		}

		RestoreVideoMode();
		m_IsInitialize = FALSE;
	}
}

VOID	RestoreVideoMode()
{
	ClearSurfaceToColor(clGrp.lpDDSPrimary, 0);
	ClearSurfaceToColor(clGrp.lpDDSBack, 0);
	clGrp.SetAllPalette();
}	



VOID	ClearSurfaceToColor(LPDIRECTDRAWSURFACE7 lpSurface, SI32 color)
{
	DDBLTFX		ddbltfx;

	if(lpSurface)
	{
		ZeroMemory(&ddbltfx, sizeof(ddbltfx));

		ddbltfx.dwSize			=	sizeof(ddbltfx);
		switch( color )
		{
		case 0:	// 검정색
			ddbltfx.dwFillColor		=	0;
			break;
		case 1:	// 흰색
			ddbltfx.dwFillColor		=	0xFFFFFFFF;
			break;
		default:
			break;
		}
		
		if(FAILED(lpSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx))==TRUE)			
			clGrp.Error("FKJE8567", "YMPEGOVER002");
	}
	else
		clGrp.Error("FKJE8567", "YMPEGOVER003");
}


