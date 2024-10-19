//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <main.h>
#include <ympegover.h>
#include <Mouse.h>


extern _InputDevice				IpD;

static	VOID				RestoreVideoMode();

_MpegPlayer					MpegPlayer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _MpegPlayer::_MpegPlayer()
/// ���� : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_MpegPlayer::_MpegPlayer()
{
	m_IsInitialize = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _MpegPlayer::_MpegPlayer()
/// ���� : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_MpegPlayer::~_MpegPlayer()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_MpegPlayer::Init()
/// ���� : 
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
/// �Լ� : BOOL	_MpegPlayer::Load(CHAR* lpszFileName)
/// ���� : 
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
/// �Լ� : SHORT _MpegPlayer::Action()
/// ���� : ���ο� ���������� �����Ѵ�.
/// ���ϰ� : 0 - ��� �������̴�.
///          1 - ����� Ű�� ���������� �ߴܵǾ���.
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
/// �Լ� : BOOL _MpegPlayer::IsInitialize()
/// ���� : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _MpegPlayer::IsInitialize()
{
	return m_IsInitialize;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _MpegPlayer::IsPlay()
/// ���� : 
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
/// �Լ� : BOOL _MpegPlayer::Play(BOOL IsFullScreen, SHORT Width, SHORT Height)
/// ���� : 
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
/// �Լ� : VOID	 _MpegPlayer::Free()
/// ���� : 
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
		case 0:	// ������
			ddbltfx.dwFillColor		=	0;
			break;
		case 1:	// ���
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


