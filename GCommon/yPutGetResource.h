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

// ���� �ڿ��� ����ϴµ� ���Ǵ� ������ �ʱ�ȭ �Ѵ�.
VOID	InitGetResource();

// ���� �ڿ��� �����Ѵ�.
VOID	SetGetResource(SI16 siAbsoluteX, SI16 siAbsoluteY, SI16 siMoney, SI16 siTree, SI16 siBow1, SI16 siBow2, SI16 siBow3, SI16 siBow4, const char* string = NULL);

// ���� �ڿ��� ȭ�鿡 ������ش�.
VOID	DrawGetResource(HDC hdc);

#endif
