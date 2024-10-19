#ifndef	_ONLINE_CHATLIST_HEADER
#define _ONLINE_CHATLIST_HEADER

#define ON_INN_MAX_USER_CHAT_STRING			3000	// ����� �Է��� ä�� ���ڿ��� �ִ� ����
#define ON_INN_MAX_CHAT_STRING				256		// ä�� ���ڿ� ����Ʈ�� �� ���ڿ��� �ִ� ����
#define ON_INN_MAX_CHAT_ARRAY				600		// ä�� ���ڿ� ����Ʈ

#define ON_INN_CONTENT_ONLY					1		// ���븸 �ִ� ��
#define ON_INN_ID_CONTENT					2		// ���̵�� ������ �ִ� ��

// ���ڻ�(0~9������ ����)
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

	SI32			m_siKind;															// �� ����(ä�ñ�(���̵�,����), ��������(����))
	CHAR			m_szChatList[ON_INN_MAX_CHAT_ARRAY][ON_INN_MAX_CHAT_STRING];		// ä�� ���ڿ� ����Ʈ
	SI32			m_siChatCurrIndex;													// ������ ���ڿ� ���� �ε���
	SI32			m_siChatScreenFirstIndex;											// ȭ�鿡�� �� ���� �ִ� ���� �ε���
	
	SI32			m_siMaxStrLine;														// ȭ�鿡�� �� �ٱ��� ���� ���ΰ�
	SI32			m_siMaxStrLen;														// ���η� ���ڱ��� ���� ���ΰ�?
	
	SI32			m_siBoardX;															// ��� �ǳ��� ���� X��ǥ 
	SI32			m_siBoardY;															// ��� �ǳ��� ���� Y��ǥ 
	
	SI32			m_siScrollBarStartX;												// ��ũ�ѹ��� ���� X��ǥ
	SI32			m_siScrollBarStartY;												// ��ũ�ѹ��� ���� Y��ǥ
	SI32			m_siScrollBarY;														// ��ũ�ѹ��� Y��ǥ
	SI32			m_siScroll;
		
	RECT			m_rtScrollArea;														// ��ũ�ѹ� ������ ����
	SI32			m_siScrollAreaHeight;												// ��ũ�ѹ� ������ ����
	SI32			m_siScrollAreaWidth;
	
	SI32			m_siTextX;															// �ؽ�Ʈ�� ���� X��ǥ
	SI32			m_siTextY;															// �ؽ�Ʈ�� ���� Y��ǥ
	SI32			m_siTextHeight;
	
	XSPR			*m_pXsprChatScrollBar;									 			// ��ũ�ѹ� �̹���
	XSPR			*m_pXsprChatBoard;													// ä��â �ǳ�

	SI32			m_siScrUpX, m_siScrUpY, m_siScrDownX, m_siScrDownY;
	XSPR			m_UpImage, m_DownImage;
	_Button			m_BScrUp, m_BScrDown;
	BOOL			m_bScroll;

	BOOL			m_bMouseDown;

private:
					// siAddMode : (���̵�,����)�� ����� (����)�� �����
	BOOL			AddChatList(SI32 siAddMode, CHAR *pID, CHAR *pContent, SI32 siIDColor, SI32 siContentColor);				// ID�� ����, ����
	VOID			DrawChatListText(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont);		// ä�ñ۵��� ȭ�鿡 �����ش�.
	VOID			TextOutByColor(HDC hDC, SI32 siColor, SI32 x, SI32 y, CHAR *str);
	VOID			DrawChatListScroll(LPDIRECTDRAWSURFACE7 pSurface);					// ä�ñ� ����� ��ũ���� �׸���.
	VOID			ScrollChatList();													// ä�ñ� ��� ��ũ���ϱ�
public:
	OnlineChatList();																	// ������
	~OnlineChatList();																	// �Ҹ���
		
																						// ä�� ��� ����Ÿ �ʱ�ȭ
	BOOL			Init(cltOnlineWorld *pOnlineWorld,								
						SI32 siKind,													// ���� �Ǵ� ���̵�,����
						SI32 siBoardImg, SI32 siScrollBarImg,							// ����, ��ũ�ѹ� �̹���
						SI32 siMaxStrLine,												// ȭ�鿡 ���� �� �ִ� �ִ� ���μ�
						SI32 siMaxStrLen,												// ���η� ���ڱ��� ���� ���ΰ�?
						SI32 siBoardX, SI32 siBoardY,									// ������ ���� X, Y��ǥ
						SI32 siScrollBarX, SI32 siScrollBarY,							// ��ũ�ѹ��� ���� X, Y��ǥ
						SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siScrollAreaWidth, 
						SI32 siScrollAreaHeight,										// ��ũ�� �ǳ��� ���� X, Y, �ʺ�, ����
						SI32 siTextX, SI32 siTextY, SI32 siTextHeight=15,				// �ؽ�Ʈ�� ���� X, Y��ǥ
						CHAR *pScrUp=NULL, CHAR *pScrDown=NULL);
	VOID				Process(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont);														
																						// ä�� ��Ͽ� ���� �߰��Ѵ�.
	BOOL			Add(CHAR *pContent,	SI32 siColor=ON_INN_COLOR_WHITE);				// ����, ����
	BOOL			Add(CHAR *pID, CHAR *pContent, SI32 siIDColor=ON_INN_COLOR_ORANGE, SI32 siContentColor=ON_INN_COLOR_WHITE);	// ID, ����, ����
	VOID			Initialize();														//	���ڿ� ����Ʈ, ������ �ʱ�ȭ
	VOID			SetX(SI16 siX);
	VOID			SetY(SI16 siY);
	VOID			SetXY(SI32 siBoardX, SI32 siBoardY, SI32 siScrollBarX, SI32 siScrollBarY, 
						SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siTextX, SI32 siTextY);
	VOID			SetMaxStrLine(SI32 siMaxStrLine)	{ m_siMaxStrLine = siMaxStrLine; };
};

#endif