// �ۼ��� : �ڿ���
// ����� : ������
// ü�ÿ�

#include <GSL.h>
#include <Main.h>
#include <Mouse.h>


#include "OnlineWorld.h"
#include "OnlineChatList.h"
#include "OnlineFont.h"
#include "OnlineResource.h"

//-------------------------------------
// m_siKind�� ON_INN_ID_CONTENT�� ��쿡�� �̷��� ��������.
#define ON_INN_ADD_MODE_ID_CONTENT					1		// ���̵�� ���� ��� ��´�.
#define ON_INN_ADD_MODE_CONTENT_ONLY				2		// ���븸 ��´�.

extern _InputDevice			IpD;


///////////////////////////////////////////////////////////////////////////////
//	����	: ������.
OnlineChatList::OnlineChatList()
{
	m_siScroll = 0;
	m_bScroll = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	����	: �Ҹ���.
OnlineChatList::~OnlineChatList()
{	
	clGrp.FreeXspr(m_UpImage);
	clGrp.FreeXspr(m_DownImage);
}


///////////////////////////////////////////////////////////////////////////////
//	����	: �ʱ�ȭ
BOOL	OnlineChatList::Init(cltOnlineWorld *pOnlineWorld, SI32 siKind, 
							 SI32 siBoardImg, SI32 siScrollBarImg, 
							 SI32 siMaxStrLine, SI32 siMaxStrLen,
							 SI32 siBoardX, SI32 siBoardY,
							 SI32 siScrollBarX, SI32 siScrollBarY,
							 SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siScrollAreaWidth, SI32 siScrollAreaHeight,
							 SI32 siTextX, SI32 siTextY, SI32 siTextHeight,
							 CHAR *pScrUp, CHAR *pScrDown)
{	
	ZeroMemory(m_szChatList, ON_INN_MAX_CHAT_ARRAY * ON_INN_MAX_CHAT_STRING);

	m_siKind					= siKind;

	m_pOnlineWorld				= pOnlineWorld;

	// robypark 2004/10/15 18:53
	// ��ũ�� ��ư �̹����� ������� ���� ��� ó�� �߰�
	if (siScrollBarImg == -1)
		m_pXsprChatScrollBar		= NULL;
	else
		m_pXsprChatScrollBar		= m_pOnlineWorld->pOnlineResource->GetxsprGame(siScrollBarImg);

//	m_pXsprChatScrollBar		= m_pOnlineWorld->pOnlineResource->GetxsprGame(siScrollBarImg);
	if( siBoardImg == -1 )		m_pXsprChatBoard	= NULL;
	else						m_pXsprChatBoard	= m_pOnlineWorld->pOnlineResource->GetxsprGame(siBoardImg);

	m_siChatCurrIndex			= -1;
	m_siScroll	= 0; 


	m_siMaxStrLine				= siMaxStrLine;
	m_siMaxStrLen				= siMaxStrLen;
	if( m_siMaxStrLen > ON_INN_MAX_CHAT_STRING-4 ) m_siMaxStrLen = ON_INN_MAX_CHAT_STRING-4;		// 4: �÷��� 3���� �� ����

	m_siBoardX					= siBoardX;
	m_siBoardY					= siBoardY;

	m_siScrollBarStartX			= siScrollBarX;
	m_siScrollBarStartY			= siScrollBarY;

	m_siScrollAreaHeight		= siScrollAreaHeight;
	m_siScrollAreaWidth			= siScrollAreaWidth;

	m_rtScrollArea.left			= siScrollAreaX;
	m_rtScrollArea.top			= siScrollAreaY;
	m_rtScrollArea.right		= siScrollAreaX + siScrollAreaWidth;
	m_rtScrollArea.bottom		= siScrollAreaY + siScrollAreaHeight; 

	// ä�ñ��� ��ũ���� �⺻���� ��, �Ϲݱ��� ��ũ���� �⺻���� ��.
	if( m_siKind == ON_INN_ID_CONTENT )
	{
		// robypark 2004/10/15 18:53
		// ��ũ�� ��ư �̹����� ������� ���� ��� ó�� �߰�
		if (NULL == m_pXsprChatScrollBar)
			m_siScrollBarY			= m_rtScrollArea.top + m_siScrollAreaHeight;
		else
			m_siScrollBarY			= m_rtScrollArea.top + m_siScrollAreaHeight - m_pXsprChatScrollBar->Header.Ysize;
//		m_siScrollBarY			= m_rtScrollArea.top + m_siScrollAreaHeight - m_pXsprChatScrollBar->Header.Ysize;
	}
	else
		m_siScrollBarY			= m_siScrollBarStartY;
									  
	m_siTextX					= siTextX;
	m_siTextY					= siTextY;
	m_siTextHeight				= siTextHeight;

	if(pScrUp && pScrDown)
	{
		m_bScroll = TRUE;
		clGrp.LoadXspr(pScrUp, m_UpImage);
		clGrp.LoadXspr(pScrDown, m_DownImage);
		m_siScrUpX = m_siScrDownX = m_rtScrollArea.left;
		m_siScrUpY = siScrollAreaY - m_UpImage.Header.Ysize;
		m_siScrDownY = m_rtScrollArea.bottom;
		
		m_BScrUp.Create(m_siScrUpX, m_siScrUpY, m_UpImage.Header.Xsize, m_UpImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
		m_BScrDown.Create(m_siScrDownX, m_siScrDownY, m_DownImage.Header.Xsize, m_DownImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
	}
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//	����	: ä�� �� ��Ͽ� ���� �߰��Ѵ�.
BOOL	OnlineChatList::Add(CHAR *pContent, SI32 siColor)
{
	return AddChatList(ON_INN_ADD_MODE_CONTENT_ONLY, NULL, pContent, siColor, siColor);
}

///////////////////////////////////////////////////////////////////////////////
//	����	: ä�� �� ��Ͽ� ���� �߰��Ѵ�.
BOOL	OnlineChatList::Add(CHAR *pID, CHAR *pContent, SI32 siIDColor, SI32 siContentColor)
{	
	return AddChatList(ON_INN_ADD_MODE_ID_CONTENT, pID, pContent, siIDColor, siContentColor);
}

///////////////////////////////////////////////////////////////////////////////
//	����	: ä�� �� ��Ͽ� ���� �߰��Ѵ�.
BOOL	OnlineChatList::AddChatList(SI32 siAddMode, CHAR *pID, CHAR *pContent, SI32 siIDColor, SI32 siContentColor)
{	
	SI32	line;									// �� ������ ���ڿ��ΰ�?
	CHAR	cIDColor[2], cContentColor[2];			// �÷� �÷���
	CHAR	buffer[128][ON_INN_MAX_CHAT_STRING];

//>	for(UI16 k = 0; k < 128; k++)
//		ZeroMemory(buffer[k], sizeof(ON_INN_MAX_CHAT_STRING));

	ZeroMemory( buffer, sizeof( buffer ) );

	CHAR	content[ON_INN_MAX_USER_CHAT_STRING];
	ZeroMemory(content, sizeof(content));
	SI32	contentLen, siMaxStrLen;
	SI32	index;
	SI32	i, j;

	if( m_siKind == ON_INN_ID_CONTENT && siAddMode == ON_INN_ADD_MODE_ID_CONTENT )
	{
		if( pID == NULL )	return FALSE;
		// ��ȭ ���ڿ��� �÷��̾��� ���̵� ÷���Ѵ�.
		sprintf(content, "%s : %s", pID, pContent);		
	}
	else
	{
		sprintf(content, "%s", pContent);		
	}

	
	// +3 �� �÷��� ����(ù��° �÷���: (���̵�,����)or(����)or(���̵�,���� ������ ���� �������� �Ѿ�� ���븸 �ִ� ���ڿ�), 
	//		�ι�°: ID ����, �׹�°: ���� ����)
	contentLen	= strlen(content) + 3;			
	siMaxStrLen	= m_siMaxStrLen + 3;

	itoa(siIDColor,			cIDColor,		10);
	itoa(siContentColor,	cContentColor,	10);
	

	//---------------------------------
	// ���ڿ��� �� ��� ���� �������� ������ ���ۿ� �ִ´�.
	if( contentLen > siMaxStrLen )	
	{
		line = 0;
		index = 0;	
		for( i=0; content[i] != '\0'; i++ )
		{
			if( index == 0 )	// ù����
			{
				if( m_siKind == ON_INN_ID_CONTENT && siAddMode == ON_INN_ADD_MODE_ID_CONTENT )
				{
					if( line > 0 )	// �ι�° �� �̻�
					{
						buffer[line][index++] = '0';				// �÷��� �߰�
						buffer[line][index++] = cIDColor[0];		// �÷��� �߰�
						buffer[line][index++] = cContentColor[0];	// �÷��� �߰�
						for( j=0; j<4; j++ )	buffer[line][index++] = ' ';	// ���� �� ĭ�� �ִ´�.
					}
					else
					{
						buffer[line][index++] = '1';				// �÷��� �߰�(1�� ���ڿ��� ������ �˸��� �÷���, �׿ܴ� 0)
						buffer[line][index++] = cIDColor[0];		// �÷��� �߰�
						buffer[line][index++] = cContentColor[0];	// �÷��� �߰�
					}
				}
				else
				{
					buffer[line][index++] = '2';				// �÷��� �߰�(2�� ���븸 �ִ� �������� ����)
					buffer[line][index++] = cIDColor[0];		// �÷��� �߰�
					buffer[line][index++] = cContentColor[0];	// �÷��� �߰�
					if(line > 0 && content[i] == ' ')
						i++;
				}
			}  

			// �޾ƿ� ���ڿ� �� �� ���ڸ� ���ۿ� �ִ´�.
			buffer[line][index++] = content[i];

			// �ѱ��� ���
			if( UI08(content[i]) > 127 )
			{
				// �ѱ��� �ϼ����� ���ߴµ� ���۸� �ѱ�� �� �ѱ��� ���� �������� �ѱ��.
				if( index >= siMaxStrLen )
				{
					buffer[line][index-1] = '\0';
					line++;
					index = 0;
					i--;
					continue;
				}
				else
				{
					// �� ����Ʈ �� �д´�.
					buffer[line][index++] = content[++i];
				}
			}

			if( index >= siMaxStrLen )
			{
				buffer[line][index] = '\0';
				line++;
				index = 0;
			}
		}

		// ������ �� ä���� ���ϰ� ���ڿ��� ������ �� �� ���ڸ� �߰��ϰ�, ���μ��� �����Ѵ�.
		if( index != 0 )
		{
			buffer[line][index] = '\0';
			line++;
		}
	}
	else
	{
		if( m_siKind == ON_INN_ID_CONTENT && siAddMode == ON_INN_ADD_MODE_ID_CONTENT )
			sprintf(buffer[0], "1%c%c%s", cIDColor[0], cContentColor[0], content);
		else
			sprintf(buffer[0], "2%c%c%s", cIDColor[0], cContentColor[0], content);
		line = 1;
	}


	//---------------------------------
	// ä�� ���ڿ� ����Ʈ�� �޾ƿ� ���ڿ� �ֱ�

	
	// ���ڿ��� �߰��Ѵ�
	for( i=0; i<line; i++ )
	{
		m_siChatCurrIndex++;
		
		// �ִ� ���ۼ��� �ѱ��
		if( m_siChatCurrIndex >= ON_INN_MAX_CHAT_ARRAY )
		{		
			// ���پ� ������ ��� ��
			for( j=0; j<ON_INN_MAX_CHAT_ARRAY-1; j++ )	
				strcpy(m_szChatList[j], m_szChatList[j+1]);
			m_siChatCurrIndex = ON_INN_MAX_CHAT_ARRAY - 1;
		}
		
		// ���ڿ� �� ���� �߰��Ѵ�
		strncpy(m_szChatList[m_siChatCurrIndex], buffer[i], ON_INN_MAX_CHAT_STRING);
	}
	//
	//---------------------------------


	
	// ä�ø���� ���
	if( m_siKind == ON_INN_ID_CONTENT )
	{
		// ȭ�鿡�� �� ���� �ִ� ���� �ε��� ����
		if( (m_siChatCurrIndex+1) >= m_siMaxStrLine )
			m_siScroll = (m_siChatCurrIndex+1) - m_siMaxStrLine;
		else
			m_siScroll = 0;
		// ��ũ�ѹ��� Y��ǥ�� ���ϴ����� ����

		// robypark 2004/10/15 18:53
		// ��ũ�� ��ư �̹����� ������� ���� ��� ó�� �߰�
		if (NULL == m_pXsprChatScrollBar)
			m_siScrollBarY	= m_rtScrollArea.top + m_siScrollAreaHeight;
		else
			m_siScrollBarY	= m_rtScrollArea.top + m_siScrollAreaHeight - m_pXsprChatScrollBar->Header.Ysize;
//		m_siScrollBarY	= m_rtScrollArea.top + m_siScrollAreaHeight - m_pXsprChatScrollBar->Header.Ysize;
	}
	else
	{
		m_siScroll = 0;
		m_siScrollBarY = m_siScrollBarStartY;
	}
	
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
//	����	: ä�ñ��� ȭ�鿡 �����ش�.
VOID	OnlineChatList::TextOutByColor(HDC hDC, SI32 siColor, SI32 x, SI32 y, CHAR *str)
{	
	SetTextColor(hDC, RGB(10, 10, 10));
	TextOut(hDC, x+1, y+1, str, strlen(str));

	switch( siColor )
	{
	case ON_INN_COLOR_ORANGE:
		SetTextColor(hDC, RGB(255, 155, 0));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	case ON_INN_COLOR_LIGHT_ORANGE:
		SetTextColor(hDC, RGB(255, 200, 150));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	case ON_INN_COLOR_DARK_ORANGE:
		SetTextColor(hDC, RGB(200, 100, 0));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	case ON_INN_COLOR_RED:
		SetTextColor(hDC, RGB(255, 0, 0));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	case ON_INN_COLOR_GREEN:
		SetTextColor(hDC, RGB(0, 255, 0));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	case ON_INN_COLOR_BLUE:
		SetTextColor(hDC, RGB(50, 50, 255));
		TextOut(hDC, x, y, str, strlen(str));
		break; 
	case ON_INN_COLOR_GRAY:
		SetTextColor(hDC, RGB(150, 150, 150));
		TextOut(hDC, x, y, str, strlen(str));
		break; 
	default: // ON_INN_COLOR_WHITE:
		SetTextColor(hDC, RGB(210, 210, 210));
		TextOut(hDC, x, y, str, strlen(str));
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	����	: ä�ñ��� ȭ�鿡 �����ֱ� ���� �غ� �۾�
VOID	OnlineChatList::DrawChatListText(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont)
{	
	HDC		hDC;
	CHAR	buffer[1024];
	CHAR	id[128], *content, cIDColor[2], cContentColor[2];
	SI32	siMaxStrLen;
	SI32	i, j, index;
	SI32	y;

	ZeroMemory(cIDColor, sizeof(cIDColor));
	ZeroMemory(cContentColor, sizeof(cContentColor));
	ZeroMemory(buffer, sizeof(buffer));

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(siFont));
		SetBkMode(hDC, TRANSPARENT);


		y = m_siTextY;
		for( i=m_siScroll; i<m_siScroll+m_siMaxStrLine; i++ )
		{		
			if( i > m_siChatCurrIndex )	break; 

			siMaxStrLen			= m_siMaxStrLen + 3;		// �÷��� ����
			cIDColor[0]			= m_szChatList[i][1];
			cContentColor[0]	= m_szChatList[i][2];
			strncpy(buffer, &m_szChatList[i][3], ON_INN_MAX_CHAT_STRING);
			
			// ��ȭ ���ڿ��� ù ���� ��츸 ���̵�� ���ڿ��� �����ؼ� ��´�.
			if( m_szChatList[i][0] == '1' )	
			{
				// ��ȭ ���ڿ��� �и��ؼ� content�� �ִ´�.
				content = strstr(buffer, " : ");
				
				if( content ) 
				{
					// ���̵� ���ڿ��� �и��ؼ� id�� �ִ´�.
					index = 0;
					for( j=0; j<siMaxStrLen; j++ )
					{
						// ������ ������ ���̵� ���ڿ��� ������.
						if( buffer[j] == ' ' )
						{
							id[index] = '\0';
							break;
						}
						id[index++] = buffer[j];
					}
					
					
					TextOutByColor(hDC, atoi(cIDColor), m_siTextX, y, id);
					TextOutByColor(hDC, atoi(cContentColor), m_siTextX + strlen(id)*6, y, content);
				}
			}
			else
			{
				TextOutByColor(hDC, atoi(cContentColor), m_siTextX, y, buffer);
			}
			y += m_siTextHeight;

		}
		pSurface->ReleaseDC( hDC );
	}
}


///////////////////////////////////////////////////////////////////////////////
//	����	: ä�ñ� ����� ��ũ���� �׸���.
VOID	OnlineChatList::DrawChatListScroll(LPDIRECTDRAWSURFACE7 pSurface)
{	
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		if( m_pXsprChatBoard )
		{
			SI32 siX, siY, siXsize, siYsize;
			siX = m_siBoardX;
			siY = m_siBoardY;
			siXsize = m_pXsprChatBoard->Header.Xsize;
			siYsize = m_pXsprChatBoard->Header.Ysize;

			if(siX < 0)		siX = 0;
			if(siY < 0)		siY = 0;

			if((siX + siXsize) > ON_SCREEN_XSIZE)		siXsize = ON_SCREEN_XSIZE - siX;
			if((siY + siYsize) > ON_SCREEN_YSIZE)		siYsize = ON_SCREEN_YSIZE - siY;

			clGrp.PutSpriteJin(siX, siY, siXsize, siYsize, m_pXsprChatBoard->Image);
		}	

		// robypark 2004/10/15 18:53
		// ��ũ�� ��ư �̹����� ������� ���� ��� �׷����� �ʵ��� ����
		if (NULL != m_pXsprChatScrollBar)
		{
			clGrp.PutSpriteT(m_siScrollBarStartX, m_siScrollBarY, 
								m_pXsprChatScrollBar->Header.Xsize, m_pXsprChatScrollBar->Header.Ysize,
								&m_pXsprChatScrollBar->Image[m_pXsprChatScrollBar->Header.Start[0]]);
		}

		if(m_bScroll)
		{
			m_BScrUp.Put(&m_UpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
			m_BScrDown.Put(&m_DownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		}
		clGrp.UnlockSurface( pSurface );
	}
}


///////////////////////////////////////////////////////////////////////////////
//	����	: ä�ñ� ��� ��ũ���ϱ�.
VOID	OnlineChatList::ScrollChatList()
{
	SI16 siHeight, siListCount;
	siListCount = m_siChatCurrIndex+1;

	if(!m_bMouseDown) return;

	POINT m_ptMouse;
	m_ptMouse.x = IpD.Mouse_X;
	m_ptMouse.y = IpD.Mouse_Y;

	if(siListCount > m_siMaxStrLine)
	{
		if(m_bScroll)
		{
			if(m_BScrUp.Process(m_bMouseDown))
			{
				m_siScroll--;
				if(m_siScroll < 0)		m_siScroll = 0;
				
				siHeight = (SI16)(m_rtScrollArea.bottom - m_rtScrollArea.top - m_pXsprChatScrollBar->Header.Ysize);
				m_siScrollBarY = m_rtScrollArea.top + (SI16)(((float)siHeight / (float)(siListCount - m_siMaxStrLine)) * (float)m_siScroll);
			}		
			else if(m_BScrDown.Process(m_bMouseDown))
			{
				m_siScroll++;
				if(m_siScroll > (siListCount - m_siMaxStrLine))	m_siScroll = siListCount - m_siMaxStrLine;
				
				siHeight		=	(SI16)((m_rtScrollArea.bottom - m_rtScrollArea.top - m_pXsprChatScrollBar->Header.Ysize));
				m_siScrollBarY = m_rtScrollArea.top + (SI16)(((float)siHeight / (float)(siListCount - m_siMaxStrLine))  * (float)m_siScroll);
			}
		}

		// robypark 2004/10/15 18:53
		// ��ũ�� ��ư �̹����� ������� ���� ��� ��ũ�ѿ� ���� ���콺 ó���� ���� �ʴ´�.
		if ((NULL != m_pXsprChatScrollBar) && (PtInRect(&m_rtScrollArea, m_ptMouse) == TRUE))
		{
			siHeight		=	(SI16)(m_rtScrollArea.bottom - m_rtScrollArea.top - m_pXsprChatScrollBar->Header.Ysize);
			m_siScrollBarY = m_ptMouse.y;
			if(m_siScrollBarY > (m_rtScrollArea.bottom - m_pXsprChatScrollBar->Header.Ysize))
				m_siScrollBarY = m_rtScrollArea.bottom - m_pXsprChatScrollBar->Header.Ysize;
			
			m_siScroll = max(0, (m_siScrollBarY - m_rtScrollArea.top) / ((float)siHeight / (float)(siListCount - m_siMaxStrLine)));
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	����	: ä�ñ� ��� ó��
VOID	OnlineChatList::Process(LPDIRECTDRAWSURFACE7 pSurface, SI32 siFont)
{	
	// ä�ñ۵��� ��ũ���Ѵ�.
	ScrollChatList();
	// ä�� ȭ���� ��ũ���� �׸���.
	DrawChatListScroll(pSurface);	
	// ä�ñ۵��� ȭ�鿡 �����ش�.
	DrawChatListText(pSurface, siFont);	

	if( IpD.LeftPressSwitch )		m_bMouseDown	=	TRUE;
	else							m_bMouseDown	=	FALSE;
}

VOID OnlineChatList::Initialize()
{
	ZeroMemory(m_szChatList, ON_INN_MAX_CHAT_ARRAY * ON_INN_MAX_CHAT_STRING);
	m_siChatCurrIndex			= -1;
	m_siScroll	= 0;
	m_siScroll = 0;
}

VOID OnlineChatList::SetXY(SI32 siBoardX, SI32 siBoardY, SI32 siScrollBarX, SI32 siScrollBarY, SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siTextX, SI32 siTextY)
{
	m_siBoardX					= siBoardX;
	m_siBoardY					= siBoardY;

	m_siScrollBarStartX			= siScrollBarX;
	m_siScrollBarStartY			= siScrollBarY;
	m_siScrollBarY				= siScrollBarY;

	m_rtScrollArea.left			= siScrollAreaX;
	m_rtScrollArea.top			= siScrollAreaY;
	m_rtScrollArea.right		= siScrollAreaX + m_siScrollAreaWidth;
	m_rtScrollArea.bottom		= siScrollAreaY + m_siScrollAreaHeight; 
									  
	m_siTextX					= siTextX;
	m_siTextY					= siTextY;

	m_siScrUpX = m_siScrDownX = m_rtScrollArea.left;
	m_siScrUpY = m_rtScrollArea.top - m_UpImage.Header.Ysize;
	m_siScrDownY = m_rtScrollArea.bottom;

	if(m_bScroll)
	{
		m_BScrUp.SetX(m_siScrUpX);		m_BScrUp.SetY(m_siScrUpY);
		m_BScrDown.SetX(m_siScrDownX);	m_BScrDown.SetY(m_siScrDownY);
	}
}

VOID OnlineChatList::SetX(SI16 siX)
{
	SI16 siAddX = siX - m_siBoardX;

	m_siBoardX = siX;
	m_siScrollBarStartX += siAddX;
	m_rtScrollArea.left += siAddX;
	m_rtScrollArea.right += siAddX;
	m_siTextX += siAddX;

	m_siScrUpX = m_siScrDownX = m_rtScrollArea.left;

	if(m_bScroll)
	{
		m_BScrUp.SetX(m_siScrUpX);
		m_BScrDown.SetX(m_siScrDownX);
	}
}

VOID OnlineChatList::SetY(SI16 siY)
{
	SI16 siAddY = siY - m_siBoardY;

	m_siBoardY = siY;
	m_siScrollBarStartY += siAddY;
	m_rtScrollArea.top += siAddY;

	if(m_rtScrollArea.top > m_rtScrollArea.bottom)
		m_rtScrollArea.top = m_rtScrollArea.bottom;

	m_siScrollAreaHeight = m_rtScrollArea.bottom - m_rtScrollArea.top;
	m_siScrollBarY = m_rtScrollArea.top + m_siScrollAreaHeight - m_pXsprChatScrollBar->Header.Ysize;
	m_siTextY += siAddY;

	m_siScrUpY = m_rtScrollArea.top - m_UpImage.Header.Ysize;
	m_siScrDownY = m_rtScrollArea.bottom;
	if(m_bScroll)
	{
		m_BScrUp.SetY(m_siScrUpY);
		m_BScrDown.SetY(m_siScrDownY);
	}
}
