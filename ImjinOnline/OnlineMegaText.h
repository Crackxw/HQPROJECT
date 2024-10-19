#ifndef __MEGA_TEXT
#define	__MEGA_TEXT	

#include "OnlineResource.h"
#include "OnlineFont.h"

/********************************************************************************************
	파일명	:	OnlineMegaText.cpp
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2001.05.23
	수정일	:	2001.08.06
********************************************************************************************/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 말풍선
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHAT_TEXT_CAP					14			// 채팅시 줄 간격
#define	ON_CHAT_MAX_BOX						150			// 말풍선의 최대 가로 크기 세로 크기는 길이에 따라 변함
#define ON_CHAT_MIN_BOX						10			// 말풍선의 최소 
#define ON_CHAT_MAX_STRING					30			// 말풍선에 들어갈 최대 글자수

#define	ON_CHAT_BAR_RIGTH					7							
#define	ON_CHAT_BAR_LEFT					8
#define	ON_CHAT_BAR_TOP						1
#define	ON_CHAT_BAR_BOTTOM					4

#define	ON_CHAT_RIGTH_UP					2	
#define	ON_CHAT_RIGTH_DOWN					6	
#define	ON_CHAT_LEFT_UP						0	
#define	ON_CHAT_LEFT_DOWN					3	
#define	ON_CHAT_CENTER						5
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 텍스트 버퍼 크기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_TEXT_XSIZE						180
#define ON_TEXT_YSIZE						70
#define ON_TEXT_BITCOUNT					8
#define	ON_TEXT_MAX_BUFFER					410
#define ON_TEXT_MAX_SCROLL					10
#define ON_TEXT_SCROLL						0
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


typedef struct tagMEGATEXTBUFFER
{
	UI08			*pTextBuffer;

	UINT			uiID;

	SI16			siTextWidth;
	SI16			siTextHeight;

} MEGATEXTBUFFER;


class	OnlineMegaText
{
private:

	MEGATEXTBUFFER		_TextBuffer[ON_TEXT_MAX_BUFFER];
	

	HBITMAP			hOldBmp;
	LPBITMAPINFO	m_pBitInfo;
	HDC				m_MegaDC;
	HBITMAP			m_hBmp;
	BYTE			*m_pData;
	static BYTE		m_ColorTable[256];


	OnlineResource	*m_pOnlineResource;
	OnlineFonts		*m_pOnlineFonts;

public:
	OnlineMegaText();
	~OnlineMegaText();

	VOID			Free(UINT uiID);														// ID를 받아 ID와 동일한 아이디만 날린다.
	VOID			ReMoveAll();															// 모든 버퍼를 날린다.
	VOID			destroyBuffer(MEGATEXTBUFFER *b);
	
	MEGATEXTBUFFER	*FindTextBuffer( UINT uiID );

	
	VOID			Init();
	VOID			DrawText( UINT uiID, char *pString, COLORREF rgb, BOOL bFlag);				// 말풍선과 함께 텍스트를 찍는다.
	VOID			DrawText( HDC hdc, SI16 x, SI16 y, char *pString, COLORREF rgb );		// 텍스트를 출력한다.
	VOID			DrawTextCenter( HDC hDC, SI16 x, SI16 y, SI16 siWidth, SI16 siHeight, char *pString, COLORREF rgb = 0x00ffffff);
	VOID			DrawTextLine( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString,  COLORREF rgb = RGB(150, 150, 150));	// 텍스트를 한줄한줄 출력	
	SI16			DrawTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb = 0x00ffffff);	// 텍스트를 한줄한줄 출력 & 리턴를 체크 함	
	VOID			DrawTextLine3( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb = 0x00ffffff);	// 텍스트를 한줄한줄 출력 & 리턴를 체크 함	
	VOID			DrawTextInBox( HDC hDC, SI16 x, SI16 y, SI16 width, SI16 height, LPRECT pDestRect, char *pString, COLORREF rgb, DWORD dwSortFlag = 0 );
	SI32			CheckTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString );	// 텍스트를 한줄한줄 출력 & 리턴를 체크 한 줄 길이를 구함	
	VOID			DrawTextRight( HDC hDC, SI16 x, SI16 y, char *pString, COLORREF rgb );	// 텍스트를 오른쪽으로에 마춰서 찍는다.
	VOID			DrawBackImage( char *pString );											// 텍스트의 배경을 찍어 줌
	VOID			DrawLineCenter( HDC hDC, SI16 x1, SI16 x2, SI16 y, char *pString, COLORREF rgb );


	BOOL			Initialize(OnlineResource *pOnlineResource, OnlineFonts *pOnlineFonts);								// 초기화
	VOID			CreateGDIPage( UINT siID );												// 시스템 메모리에 텍스트 버퍼를 생성	
	VOID			Show( UINT uiID, LPDIRECTDRAWSURFACE7 lpSurface, SI16 x, SI16 y );		// 출력	
};

#endif
