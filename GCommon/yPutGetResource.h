#ifndef	MYTH_PUTGETRESOURCE_HEADER
#define MYTH_PUTGETRESOURCE_HEADER


struct	strResourceInfo
{
	SI16	siAbsoluteX, siAbsoluteDX;
	SI16	siFont;
	SI16	siValue;	
	
};

struct	strPutGetResourceInfo
{
	BOOL	bUse;
	SI16	siAbsoluteX;
	SI16	siAbsoluteY;
	SI16	siActionAbsoluteX;
	SI16	siActionAbsoluteY;
	char    String[32];

	strResourceInfo	stResourceInfo[6];	
};

// 얻은 자원을 출력하는데 사용되는 정보를 초기화 한다.
VOID	InitGetResource();

// 얻은 자원을 설정한다.
VOID	SetGetResource(SI16 siAbsoluteX, SI16 siAbsoluteY, SI16 siMoney, SI16 siTree, SI16 siBow1, SI16 siBow2, SI16 siBow3, SI16 siBow4, const char* string = NULL);

// 얻은 자원을 화면에 출력해준다.
VOID	DrawGetResource(HDC hdc);

#endif
