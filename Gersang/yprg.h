#ifndef	_YPRG_HEADER
#define _YPRG_HEADER

#define	MAX_STARIGHT_LINE_LENGTH		50
#define MAX_FIND_RANDOM_STRAIGHT_LINE	100

#define	Y_PORTRAIT_INIT_RANKING			255

#define	BRIEFING_PORTRAIT_FILENAME		"yfnt\\hero.spr"

BOOL FindRandomStraightLine(SHORT sx, SHORT sy, SHORT dx, SHORT dy, SHORT& num, SHORT* array, SHORT XUnit, SHORT YUnit, SHORT Gap);
void DrawRandomLine(DWORD number, SHORT* array, BOOL IsSecond);


VOID	PutTextToTextBuffer(SHORT nLimitWidth, LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor = RGB(200, 200, 200));
VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor = NORMAL_TEXT_COLOR);
VOID	PutTextToTextBufferLines(CHAR lpStrings[][128], SHORT LineNum, LPRECT pRect, COLORREF TextColor = NORMAL_TEXT_COLOR);
VOID	PutTextToTextBufferTwoLine(LPSTR lpString1, LPSTR lpString2, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor = NORMAL_TEXT_COLOR);
VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor, HGDIOBJ hFont);
VOID	ClearBriefingTextBuffer();
VOID	ClearBriefingBuffer();
VOID	ClearBriefingScrollBuffer();
VOID	ClearTextBuffer();
VOID	ClearTextBuffer(SHORT x, SHORT y, SHORT width, SHORT height);
SHORT	GetTextHeight(HDC hdc, LPSTR lpString);	

extern	LPDIRECTDRAWSURFACE7	lpBriefingTextBuffer;
#endif

