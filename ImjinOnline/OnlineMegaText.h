#ifndef __MEGA_TEXT
#define	__MEGA_TEXT	

#include "OnlineResource.h"
#include "OnlineFont.h"

/********************************************************************************************
	���ϸ�	:	OnlineMegaText.cpp
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.05.23
	������	:	2001.08.06
********************************************************************************************/


//��������������������������������������������������������������������������������������������������
// ��ǳ��
//��������������������������������������������������������������������������������������������������
#define ON_CHAT_TEXT_CAP					14			// ä�ý� �� ����
#define	ON_CHAT_MAX_BOX						150			// ��ǳ���� �ִ� ���� ũ�� ���� ũ��� ���̿� ���� ����
#define ON_CHAT_MIN_BOX						10			// ��ǳ���� �ּ� 
#define ON_CHAT_MAX_STRING					30			// ��ǳ���� �� �ִ� ���ڼ�

#define	ON_CHAT_BAR_RIGTH					7							
#define	ON_CHAT_BAR_LEFT					8
#define	ON_CHAT_BAR_TOP						1
#define	ON_CHAT_BAR_BOTTOM					4

#define	ON_CHAT_RIGTH_UP					2	
#define	ON_CHAT_RIGTH_DOWN					6	
#define	ON_CHAT_LEFT_UP						0	
#define	ON_CHAT_LEFT_DOWN					3	
#define	ON_CHAT_CENTER						5
//��������������������������������������������������������������������������������������������������
// �ؽ�Ʈ ���� ũ��
//��������������������������������������������������������������������������������������������������
#define ON_TEXT_XSIZE						180
#define ON_TEXT_YSIZE						70
#define ON_TEXT_BITCOUNT					8
#define	ON_TEXT_MAX_BUFFER					410
#define ON_TEXT_MAX_SCROLL					10
#define ON_TEXT_SCROLL						0
//��������������������������������������������������������������������������������������������������


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

	VOID			Free(UINT uiID);														// ID�� �޾� ID�� ������ ���̵� ������.
	VOID			ReMoveAll();															// ��� ���۸� ������.
	VOID			destroyBuffer(MEGATEXTBUFFER *b);
	
	MEGATEXTBUFFER	*FindTextBuffer( UINT uiID );

	
	VOID			Init();
	VOID			DrawText( UINT uiID, char *pString, COLORREF rgb, BOOL bFlag);				// ��ǳ���� �Բ� �ؽ�Ʈ�� ��´�.
	VOID			DrawText( HDC hdc, SI16 x, SI16 y, char *pString, COLORREF rgb );		// �ؽ�Ʈ�� ����Ѵ�.
	VOID			DrawTextCenter( HDC hDC, SI16 x, SI16 y, SI16 siWidth, SI16 siHeight, char *pString, COLORREF rgb = 0x00ffffff);
	VOID			DrawTextLine( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString,  COLORREF rgb = RGB(150, 150, 150));	// �ؽ�Ʈ�� �������� ���	
	SI16			DrawTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb = 0x00ffffff);	// �ؽ�Ʈ�� �������� ��� & ���ϸ� üũ ��	
	VOID			DrawTextLine3( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb = 0x00ffffff);	// �ؽ�Ʈ�� �������� ��� & ���ϸ� üũ ��	
	VOID			DrawTextInBox( HDC hDC, SI16 x, SI16 y, SI16 width, SI16 height, LPRECT pDestRect, char *pString, COLORREF rgb, DWORD dwSortFlag = 0 );
	SI32			CheckTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString );	// �ؽ�Ʈ�� �������� ��� & ���ϸ� üũ �� �� ���̸� ����	
	VOID			DrawTextRight( HDC hDC, SI16 x, SI16 y, char *pString, COLORREF rgb );	// �ؽ�Ʈ�� ���������ο� ���缭 ��´�.
	VOID			DrawBackImage( char *pString );											// �ؽ�Ʈ�� ����� ��� ��
	VOID			DrawLineCenter( HDC hDC, SI16 x1, SI16 x2, SI16 y, char *pString, COLORREF rgb );


	BOOL			Initialize(OnlineResource *pOnlineResource, OnlineFonts *pOnlineFonts);								// �ʱ�ȭ
	VOID			CreateGDIPage( UINT siID );												// �ý��� �޸𸮿� �ؽ�Ʈ ���۸� ����	
	VOID			Show( UINT uiID, LPDIRECTDRAWSURFACE7 lpSurface, SI16 x, SI16 y );		// ���	
};

#endif
