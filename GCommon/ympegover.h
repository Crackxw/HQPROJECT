#ifndef	IMJIN2_YMPEGOVERLAPPED_HEADER
#define IMJIN2_YMPEGOVERLAPPED_HEADER

#include <mpeg.h>

class	_MpegPlayer
{
private:
	BOOL		m_IsInitialize;
	_Mpeg		m_Mpeg;	
public:
	_MpegPlayer();
	~_MpegPlayer();

	BOOL		Init();
	BOOL		Load(CHAR* lpszFileName);
	BOOL		Play(BOOL IsFullScreen = TRUE, SHORT Width = 0, SHORT Height = 0, SHORT siScreenXsize = 0, SHORT siScreenYsize = 0);
	BOOL		IsPlay();
	BOOL		IsInitialize();
	SHORT		Action();
	VOID		Free();
};

VOID	ClearSurfaceToColor(LPDIRECTDRAWSURFACE7	lpSurface, SI32 color);
extern	_MpegPlayer		MpegPlayer;
#endif