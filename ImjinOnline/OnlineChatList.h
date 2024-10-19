#ifndef	_ONLINE_CHATLIST_HEADER
#define _ONLINE_CHATLIST_HEADER

#define ON_INN_MAX_USER_CHAT_STRING			3000	// 사용자 입력의 채팅 문자열의 최대 길이
#define ON_INN_MAX_CHAT_STRING				256		// 채팅 문자열 리스트에 들어갈 문자열의 최대 길이
#define ON_INN_MAX_CHAT_ARRAY				600		// 채팅 문자열 리스트

#define ON_INN_CONTENT_ONLY					1		// 내용만 있는 글
#define ON_INN_ID_CONTENT					2		// 아이디와 내용이 있는 글

// 글자색(0~9까지만 가능)
#define ON_INN_COLOR_WHITE					0		
#define ON_INN_COLOR_ORANGE					1		
#define ON_INN_COLOR_LIGHT_ORANGE			2		
#define ON_INN_COLOR_DARK_ORANGE			3		
#define ON_INN_COLOR_RED					4		
#define ON_INN_COLOR_GREEN					5	
#define ON_INN_COLOR_BLUE					6		
#define	ON_INN_COLOR_GRAY					7


class	OnlineChatList
{
private:
	cltOnlineWorld					*m_pOnlineWorld;

	SI32			m_siKind;															// 글 종류(채팅글(아이디,내용), 공지사항(내용))
	CHAR			m_szChatList[ON_INN_MAX_CHAT_ARRAY][ON_INN_MAX_CHAT_STRING];		// 채팅 문자열 리스트
	SI32			m_siChatCurrIndex;													// 마지막 문자열 버퍼 인덱스
	SI32			m_siChatScreenFirstIndex;											// 화면에서 맨 위에 있는 글의 인덱스
	
	SI32			m_siMaxStrLine;														// 화면에서 몇 줄까지 보일 것인가
	SI32			m_siMaxStrLen;														// 가로로 몇자까지 보일 것인가?
	
	SI32			m_siBoardX;															// 목록 판넬의 시작 X좌표 
	SI32			m_siBoardY;															// 목록 판넬의 시작 Y좌표 
	
	SI32			m_siScrollBarStartX;												// 스크롤바의 시작 X좌표
	SI32			m_siScrollBarStartY;												// 스크롤바의 시작 Y좌표
	SI32			m_siScrollBarY;														// 스크롤바의 Y좌표
	SI32			m_siScroll;
		
	RECT			m_rtScrollArea;														// 스크롤바 영역의 범위
	SI32			m_siScrollAreaHeight;												// 스크롤바 영역의 높이
	SI32			m_siScrollAreaWidth;
	
	SI32			m_siTextX;															// 텍스트의 시작 X좌표
	SI32			m_siTextY;															// 텍스트의 시작 Y좌표
	SI32			m_siTextHeight;
	
	XSPR			*m_pXsprChatScrollBar;									 			// 스크롤바 이미지
	XSPR			*m_pXsprChatBoard;													// 채팅창 판넬

	SI32			m_siScrUpX, m_siScrUpY, m_siScrDownX, m_siScrDownY;
	XSPR			m_UpImage, m_DownImage;
	_Button			m_BScrUp, m_BScrDown;
	BOOL			m_bScroll;

	BOOL			m_bMouseDown;

private:
					// siAddMode : (아이디,내용)만 찍느냐 (내용)만 찍느냐
	BOOL			AddChatList(SI32 siAddMode, CHAR *pID, CHAR *pContent, SI32 siIDColor, SI32 siContentColor);				// ID와 내용, 색깔
	VOID			DrawChatListText(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont);		// 채팅글들을 화면에 보여준다.
	VOID			TextOutByColor(HDC hDC, SI32 siColor, SI32 x, SI32 y, CHAR *str);
	VOID			DrawChatListScroll(LPDIRECTDRAWSURFACE7 pSurface);					// 채팅글 목록의 스크롤을 그린다.
	VOID			ScrollChatList();													// 채팅글 목록 스크롤하기
public:
	OnlineChatList();																	// 생성자
	~OnlineChatList();																	// 소멸자
		
																						// 채팅 목록 데이타 초기화
	BOOL			Init(cltOnlineWorld *pOnlineWorld,								
						SI32 siKind,													// 내용 또는 아이디,내용
						SI32 siBoardImg, SI32 siScrollBarImg,							// 보드, 스크롤바 이미지
						SI32 siMaxStrLine,												// 화면에 나올 수 있는 최대 라인수
						SI32 siMaxStrLen,												// 가로로 몇자까지 보일 것인가?
						SI32 siBoardX, SI32 siBoardY,									// 보드의 시작 X, Y좌표
						SI32 siScrollBarX, SI32 siScrollBarY,							// 스크롤바의 시작 X, Y좌표
						SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siScrollAreaWidth, 
						SI32 siScrollAreaHeight,										// 스크롤 판넬의 시작 X, Y, 너비, 높이
						SI32 siTextX, SI32 siTextY, SI32 siTextHeight=15,				// 텍스트의 시작 X, Y좌표
						CHAR *pScrUp=NULL, CHAR *pScrDown=NULL);
	VOID				Process(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont);														
																						// 채팅 목록에 글을 추가한다.
	BOOL			Add(CHAR *pContent,	SI32 siColor=ON_INN_COLOR_WHITE);				// 내용, 색깔
	BOOL			Add(CHAR *pID, CHAR *pContent, SI32 siIDColor=ON_INN_COLOR_ORANGE, SI32 siContentColor=ON_INN_COLOR_WHITE);	// ID, 내용, 색깔
	VOID			Initialize();														//	문자열 리스트, 변수들 초기화
	VOID			SetX(SI16 siX);
	VOID			SetY(SI16 siY);
	VOID			SetXY(SI32 siBoardX, SI32 siBoardY, SI32 siScrollBarX, SI32 siScrollBarY, 
						SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siTextX, SI32 siTextY);
	VOID			SetMaxStrLine(SI32 siMaxStrLine)	{ m_siMaxStrLine = siMaxStrLine; };
};

#endif