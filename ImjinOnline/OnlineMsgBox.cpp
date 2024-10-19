// ���ϸ� : OnlineMsgBox.cpp
// ����� : ������

#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineMsgBox.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include "OnlineFieldArea.h"

#define ON_MSGBOX_CHAR_NUM_PER_LINE			40		// �� ���ο� �� �� �ִ� �ִ� ���ڼ�
#define ON_MSGBOX_CHAR_WIDTH				6		// �� ������ ���� �ȼ� ������ 
#define ON_MSGBOX_CHAR_HEIGHT				14		// �� ������ ���� �ȼ� ������ 
#define ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE 1

extern _InputDevice				IpD;

///////////////////////////////////////////////////////////////////////////////
//	����	: 
VOID OnlineMsgBox::Init(cltOnlineWorld *pOnlineWorld, BOOL b)
{
	m_pOnlineWorld		= pOnlineWorld;
	m_bMouseDown		= FALSE;
	m_bActive			= FALSE;
	m_siNoField			= -1;
	m_bLogin			= b;

	if(b == TRUE)
	{
		m_pXsprBox[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BOX1);		// �ǳ�(��, �Ʒ�)
		m_pXsprBox[1]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BOX2);		// �ǳ�(�߰�)
		m_pXsprButt[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BUTTON);		// Ȯ�� ��ư
	}
	else if( b == FALSE)
	{
		m_pXsprBox[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOXBOARD1);		// �ǳ�(��, �Ʒ�)
		m_pXsprBox[1]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOXBOARD2);		// �ǳ�(�߰�)
		m_pXsprButt[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOX_BUTTON);		// Ȯ�� ��ư
	}
	
	m_Butt[0].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE );
	m_Butt[1].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, TRUE );
}

///////////////////////////////////////////////////////////////////////////////
//	����	: 
SI32 OnlineMsgBox::Process()
{
	if( m_bActive == FALSE )						return -1;

	if( IsOk() )									
		return ON_MSGBOX_RETURN_OK;
	else if( IsCancel() )							return ON_MSGBOX_RETURN_CANCEL;
	else if( m_siType == ON_MSGBOX_TYPE_NONE )
	{
		m_bActive = FALSE;
		if( m_siNoField != -1 )	m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField  = -1;
		m_bMouseDown = FALSE;
		return ON_MSGBOX_TYPE_NONE;
	}

	//---------------------------------
	// ���� ���콺�� �������°�?
	if( IpD.LeftPressSwitch )	m_bMouseDown = TRUE;
	else						m_bMouseDown = FALSE;

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	����	: 
BOOL OnlineMsgBox::IsOk()
{
	if( m_bActive == FALSE )	return FALSE;

	if( m_siType == ON_MSGBOX_TYPE_CANCEL )			return FALSE;

	if( m_Butt[0].Process(m_bMouseDown) == TRUE )
	{
		m_bActive = FALSE;
		m_bMouseDown = FALSE;
		if( m_siNoField != -1 )	m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField  = -1;
		return TRUE;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//	����	: '���' �Ǵ� 'X'(�ݱ� ������)�� �����°�?
BOOL OnlineMsgBox::IsCancel()
{
	SI32 siIndex;

	if( m_bActive == FALSE )	return FALSE;

	if( m_siType == ON_MSGBOX_TYPE_CANCEL )			siIndex = 0;
	else if( m_siType == ON_MSGBOX_TYPE_OKCANCEL )	siIndex = 1;
	else if( m_siType == ON_MSGBOX_TYPE_ACCEPTDECLINE )	siIndex = 1;
	else	return FALSE;

	if( m_Butt[siIndex].Process(m_bMouseDown) == TRUE )
	{
		m_bActive	 = FALSE;
		if( m_siNoField != -1 )	m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField  = -1;
		m_bMouseDown = FALSE;
		return TRUE;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//	����	: 
VOID OnlineMsgBox::SetMsg(SI32 type, CHAR *pCaption, CHAR *pMsgFormat, ...)
{
	if( pMsgFormat == NULL )	return;

	CHAR	buffer[1024];
	CHAR	*pArguments;

	m_bActive	= TRUE;
	m_siType	= type;	

	pArguments  = (char*)&pMsgFormat + sizeof(pMsgFormat);

	vsprintf(buffer, pMsgFormat, pArguments);

	SetData(pCaption, buffer);

	if( m_siNoField != -1 )		m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

	// �� �ʵ� ���� ����
	SetRect( &m_rtMsgBox, m_ptBoxStart.x, m_ptBoxStart.y, m_ptBoxStart.x + ( ( m_siBoardXNum * m_pXsprBox[1]->Header.Xsize ) + ( m_pXsprBox[0]->Header.Xsize * 2 ) ), 
		   ( m_ptBoxStart.y + ( m_siBoardYNum * m_pXsprBox[1]->Header.Ysize ) + ( m_pXsprBox[0]->Header.Ysize * 2 ) + m_pXsprButt[0]->Header.Ysize ) );
	m_siNoField = m_pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( &m_rtMsgBox );
}


///////////////////////////////////////////////////////////////////////////////
//	����	: ����������� �����Ѵ�.
VOID OnlineMsgBox::SetData(CHAR *pCaption, CHAR *pMsg)
{
	SI32	siCapLen, siMsgLen, siLen;
	SI32	siIndex, siLine;
	BOOL	bSwitch;
	SI32	siMsgWidth, siMsgHeight;			// ���� ���ڿ��� �ʺ�� ����
	CHAR	*pToken;
	CHAR	buffer[ON_MSGBOX_MAX_MSG_LENGTH];
	SI32	i;

	if( pCaption == NULL || pMsg == NULL )	return;



	//---------------------------------
	// ���� ���ڿ� ����
	strcpy(m_strCaption, pCaption);			
	siCapLen = strlen(m_strCaption);
	if( siCapLen > ON_MSGBOX_MAX_CAPTION_LENGTH ) return;
		
	//---------------------------------
	// ���� ���ڿ� ����
	// ���ڿ��� �� ��� ���� ������ ���ڿ��� ������ �Ѵ�. '\n' �����ֱ�
	siIndex = 0;
	siLine = 0;
	bSwitch = FALSE;
	SI32 siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// ���� �޽��� �ڽ� �� ���ο� �ִ� ���� ����

	for( i=0; pMsg[i] != '\0'; i++ )
	{
		if( siIndex > ON_MSGBOX_MAX_MSG_LENGTH )	break;		// ���ڿ��� ���ѹ������� Ŭ ��� �׳� �߶� ������.

		// robypark 2004/12/23 14:00
		// OnlineText.dat�� \n�� �߰��Ǿ��� ��� �̸� New Line���� �ν��ϵ��� �����Ǿ���.
		// �޽��� �ڽ����� ����� �� ó���ǵ��� ����
		// carriage return ����
		if (pMsg[i] == '\r' )
			continue;

		m_strMsg[siIndex++] = pMsg[i];
		siCurrent_On_Msgbox_Char_Num_In_Line++;	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� ���ڼ� ���
		if( pMsg[i] == '\n' )
		{
			siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� ���ڼ� �ʱ�ȭ
			siLine++;
		}

		// �ѱ��� ���
		if( UI08(pMsg[i]) > 127 )
		{
			// �ѱ��� �ϼ����� ���ߴµ� ���۸� �ѱ�� �� �ѱ��� ���� �������� �ѱ��.
//			if (((siIndex-siLine) >= ON_MSGBOX_CHAR_NUM_PER_LINE) && ((siIndex-siLine)%ON_MSGBOX_CHAR_NUM_PER_LINE == 0) 
			if ((siCurrent_On_Msgbox_Char_Num_In_Line >= ON_MSGBOX_CHAR_NUM_PER_LINE)	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� �ִ� ���ڼ� �˻�
				&& (bSwitch == FALSE) )													// OnlineText.dat�� \n�� �߰��Ǿ��� ��� �̸� New Line���� �ν��ϵ��� �����Ǿ���.
																						// �޽��� �ڽ����� ����� �� ó���ǵ��� ����
			{
				m_strMsg[siIndex-1] = '\n';
				siLine++;

				siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� ���ڼ� �ʱ�ȭ

				i--;
				bSwitch = TRUE;
				continue;
			}
			else
			{
				// �� ����Ʈ �� �д´�.
				m_strMsg[siIndex++] = pMsg[++i];

				siCurrent_On_Msgbox_Char_Num_In_Line++;	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� ���ڼ� ���
			}
		}

//		if( (siIndex-siLine) >= ON_MSGBOX_CHAR_NUM_PER_LINE && (siIndex-siLine)%ON_MSGBOX_CHAR_NUM_PER_LINE == 0 )
		if ( siCurrent_On_Msgbox_Char_Num_In_Line >= ON_MSGBOX_CHAR_NUM_PER_LINE )	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� �ִ� ���ڼ� �˻�
																					// OnlineText.dat�� \n�� �߰��Ǿ��� ��� �̸� New Line���� �ν��ϵ��� �����Ǿ���.
																					// �޽��� �ڽ����� ����� �� ó���ǵ��� ����
		{
			m_strMsg[siIndex++] = '\n';

			siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, �� ���ο� �� �� �ִ� ���ڼ� �ʱ�ȭ

			siLine++;			
		}

		bSwitch = FALSE;
	}

	if( m_strMsg[siIndex-1] != '\n' )
	{
		siLine++;
		m_strMsg[siIndex] = '\0';
		siIndex++;
	}
	else 
	{
		m_strMsg[siIndex-1] = '\0';
	}
		

	// �� ���� ���� ���ڿ��� �ִ� ����(siMsgLen) �˾ƿ���
	strcpy(buffer, m_strMsg);
	pToken = strtok(buffer, "\n");
	siMsgLen = 0;
	for( i=0; pToken != NULL; i++ )
	{
		if( strlen(pToken) > (UI32)siMsgLen )
			siMsgLen = strlen(pToken);	

		pToken = strtok(NULL, "\n");
	}
	if( siMsgLen > siCapLen )	siLen = siMsgLen;
	else						siLen = siCapLen;


	//---------------------------------
	// ���� ���ڿ� �ʺ� ����
	siMsgWidth = siLen * ON_MSGBOX_CHAR_WIDTH;

	//---------------------------------
	// ���� ���ڿ� ���� ����
	siMsgHeight = siLine * ON_MSGBOX_CHAR_HEIGHT;			
	if( siMsgHeight == 0 ) 	siMsgHeight = ON_MSGBOX_CHAR_HEIGHT;



	//---------------------------------
	// ���ڼ��� ���� ��� ������ ���� Ÿ�ϼ��� �����Ѵ�.
	m_siBoardXNum = siMsgWidth / m_pXsprBox[1]->Header.Xsize;		
	if( siMsgWidth % m_pXsprBox[1]->Header.Xsize != 0 )		m_siBoardXNum++;
	if( m_siBoardXNum == 0 ) m_siBoardXNum = 1;

	//---------------------------------
	// ���ڼ��� ���� ��� ������ ���� Ÿ�ϼ��� �����Ѵ�.
	m_siBoardYNum = siMsgHeight / m_pXsprBox[1]->Header.Ysize;
	if( siMsgHeight % m_pXsprBox[1]->Header.Ysize != 0 )	m_siBoardYNum++;
	if( m_siBoardYNum == 0 ) m_siBoardYNum = 1;
	m_siBoardYNum += 1;		// ��ư�� ���� ��ĭ
	
	//---------------------------------
	// �޽��� �ڽ� ���� ��ǥ �����ϱ�
	m_ptBoxStart.x = clGrp.GetScreenXsize() / 2 - (((m_siBoardXNum+2) * m_pXsprBox[1]->Header.Xsize) / 2);
	m_ptBoxStart.y = clGrp.GetScreenYsize() / 2 - (((m_siBoardYNum+3) * m_pXsprBox[1]->Header.Ysize) / 2);

	// robypark 2004/12/20 17:39
	// Ȯ��, ��� ��ư�� �ؽ�Ʈ ���� Init() �Լ����� ������. ����, ���� �ؽ�Ʈ �߰��� ����
	m_Butt[0].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE );
	m_Butt[1].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, TRUE );

	//---------------------------------
	// Ȯ��, ��� ��ư�� ���� ��ǥ �����ϱ�
	if( m_siType == ON_MSGBOX_TYPE_OKCANCEL )
	{
		m_ptButtStart[0].x = m_ptBoxStart.x + (m_pXsprBox[1]->Header.Xsize * m_siBoardXNum) + (m_pXsprBox[1]->Header.Xsize * 2) - (m_pXsprButt[0]->Header.Xsize * 2 + 20) ;
		m_ptButtStart[0].y = m_ptBoxStart.y + (m_pXsprBox[1]->Header.Ysize * m_siBoardYNum) + (m_pXsprBox[0]->Header.Ysize * 2) - (m_pXsprButt[0]->Header.Ysize + 10);
		m_ptButtStart[1].x = m_ptButtStart[0].x + m_pXsprButt[0]->Header.Xsize + 6;
		m_ptButtStart[1].y = m_ptButtStart[0].y;
	}
	else if (m_siType == ON_MSGBOX_TYPE_ACCEPTDECLINE )
	{
		// robypark 2004/12/20 17:39
		// ����, ���� ��ư�� �ؽ�Ʈ ����.
		m_Butt[0].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_ACCEPT), BUTTON_PUT_LEFT, TRUE );
		m_Butt[1].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_PROVISION_NO), BUTTON_PUT_LEFT, TRUE );

		m_ptButtStart[0].x = m_ptBoxStart.x + (m_pXsprBox[1]->Header.Xsize * m_siBoardXNum) + (m_pXsprBox[1]->Header.Xsize * 2) - (m_pXsprButt[0]->Header.Xsize * 2 + 20) ;
		m_ptButtStart[0].y = m_ptBoxStart.y + (m_pXsprBox[1]->Header.Ysize * m_siBoardYNum) + (m_pXsprBox[0]->Header.Ysize * 2) - (m_pXsprButt[0]->Header.Ysize + 10);
		m_ptButtStart[1].x = m_ptButtStart[0].x + m_pXsprButt[0]->Header.Xsize + 6;
		m_ptButtStart[1].y = m_ptButtStart[0].y;
	}
	else
	{
		m_ptButtStart[0].x = m_ptBoxStart.x + (m_pXsprBox[1]->Header.Xsize * m_siBoardXNum) + (m_pXsprBox[1]->Header.Xsize * 2) - (m_pXsprButt[0]->Header.Xsize + 15);
		m_ptButtStart[0].y = m_ptBoxStart.y + (m_pXsprBox[1]->Header.Ysize * m_siBoardYNum) + (m_pXsprBox[0]->Header.Ysize * 2) - (m_pXsprButt[0]->Header.Ysize + 10);
		m_ptButtStart[1].x = m_ptButtStart[0].x;
		m_ptButtStart[1].y = m_ptButtStart[0].y;
	}
	
	// �޽��� �ڽ� ��ư ��ġ ����
	m_Butt[0].SetX((SI16)m_ptButtStart[0].x);
	m_Butt[0].SetY((SI16)m_ptButtStart[0].y);
	m_Butt[1].SetX((SI16)m_ptButtStart[1].x);
	m_Butt[1].SetY((SI16)m_ptButtStart[1].y);
}




///////////////////////////////////////////////////////////////////////////////
//	����	: 
VOID OnlineMsgBox::DrawBox()
{
	POINT	ptBoard;			// ������ ��ǥ
	SI32	i, j;	

	ptBoard = m_ptBoxStart;

	if(m_bLogin)
	{
		//---------------------------------
		// �޽��� �ڽ� �ǳ�
		// ���� ��
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[0]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// �߰� ��
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[1]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// ������ ��
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[2]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		ptBoard.y += m_pXsprBox[0]->Header.Ysize;
		
		
		
		for( i=0; i<m_siBoardYNum; i++ )
		{
			ptBoard.x = m_ptBoxStart.x;
			// ���� �߰�
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[0]]);
			ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			
			// �߰� �߰�
			for( j=0; j<m_siBoardXNum; j++ )
			{
				clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
					m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
					&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[1]]);
				ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			}
			
			// ������ �߰�
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[2]]);
			ptBoard.y += m_pXsprBox[1]->Header.Ysize;
		}
		
		
		ptBoard.x = m_ptBoxStart.x;
		// ���� �Ʒ�
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[3]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// �߰� �Ʒ�
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[4]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// ������ �Ʒ�
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[5]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
	}
	else
	{
		//---------------------------------
		// �޽��� �ڽ� �ǳ�
		// ���� ��
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[0]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// �߰� ��
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[1]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// ������ ��
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[2]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		ptBoard.y += m_pXsprBox[0]->Header.Ysize;
		
		for( i=0; i<m_siBoardYNum; i++ )
		{
			ptBoard.x = m_ptBoxStart.x;
			// ���� �߰�
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[0]]);
			ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			
			// �߰� �߰�
			for( j=0; j<m_siBoardXNum; j++ )
			{
				clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
					m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
					&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[1]]);
				ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			}
			
			// ������ �߰�
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[2]]);
			ptBoard.y += m_pXsprBox[1]->Header.Ysize;
		}
		
		
		ptBoard.x = m_ptBoxStart.x;
		// ���� �Ʒ�
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[3]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// �߰� �Ʒ�
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[4]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// ������ �Ʒ�
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[5]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
	}
	

	if( ON_MSGBOX_TYPE_NONE != m_siType )
	{
		if( m_siType == ON_MSGBOX_TYPE_OK )
		{
			m_Butt[0].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
		else if( m_siType == ON_MSGBOX_TYPE_CANCEL )
		{
			m_Butt[1].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
		else if( m_siType == ON_MSGBOX_TYPE_OKCANCEL )
		{
			m_Butt[0].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_Butt[1].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
		else if( m_siType == ON_MSGBOX_TYPE_ACCEPTDECLINE )
		{
			m_Butt[0].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_Butt[1].Put(m_pXsprButt[0], 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	����	: 
VOID OnlineMsgBox::DrawText(HDC hDC)
{
	POINT	ptTextStart, ptText;			// ������ ��ǥ
	CHAR	*pToken;
	CHAR	buffer[ON_MSGBOX_MAX_MSG_LENGTH];
	SI32	i;	

	
	SetBkMode(hDC, TRANSPARENT); // ���� ��� ����.
	SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHATLIST));
	SetTextColor(hDC, RGB(255, 255, 255));


	
	// ��ư�� ��´�.
	if( m_siType == ON_MSGBOX_TYPE_OK )
	{
		m_Butt[0].Put(hDC);
	}
	else if( m_siType == ON_MSGBOX_TYPE_CANCEL )
	{
		m_Butt[1].Put(hDC);
	}
	else if( m_siType == ON_MSGBOX_TYPE_OKCANCEL )
	{
		m_Butt[0].Put(hDC);
		m_Butt[1].Put(hDC);
	}
	else if( m_siType == ON_MSGBOX_TYPE_ACCEPTDECLINE )
	{
		m_Butt[0].Put(hDC);
		m_Butt[1].Put(hDC);
	}
	
	ptTextStart.x = m_ptBoxStart.x + m_pXsprBox[1]->Header.Xsize;
	ptTextStart.y = m_ptBoxStart.y + 15;

	// ������ ��´�.
	ptText = ptTextStart;
	TextOut(hDC, ptText.x, ptText.y, m_strCaption, strlen(m_strCaption));

	// ������ ��´�.
	ptTextStart.y += 10;
	strcpy(buffer, m_strMsg);
	pToken = strtok(buffer, "\n");
	for( i=0; pToken != NULL; i++ )
	{
		ptText.x = ptTextStart.x;
		ptText.y = ptTextStart.y + m_pXsprBox[1]->Header.Ysize/2 + ON_MSGBOX_CHAR_HEIGHT * i;
		TextOut(hDC, ptText.x, ptText.y, pToken, strlen(pToken));

		pToken = strtok(NULL, "\n");
	}
}
///////////////////////////////////////////////////////////////////////////////
//	����	: 
VOID	OnlineMsgBox::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC	hDC;

	if( m_bActive == FALSE ) 
		return;


	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		DrawBox();

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		DrawText( hDC );
		pSurface->ReleaseDC( hDC );
	}
	
}
///////////////////////////////////////////////////////////////////////////////
//	����	:	���� �޽��� �ڽ��� ��� ���ΰ�?
BOOL	OnlineMsgBox::IsActive()
{
	return m_bActive;
}

///////////////////////////////////////////////////////////////////////////////
//	����	:	�޽��� �ڽ� �ʱ�ȭ
VOID	OnlineMsgBox::Initialize()
{
	m_bActive		=	FALSE;
	m_bMouseDown	=	FALSE;

	ZeroMemory( m_strCaption, ON_MSGBOX_MAX_CAPTION_LENGTH );
	ZeroMemory( m_strMsg, ON_MSGBOX_MAX_MSG_LENGTH );

	if( m_siNoField != -1 )	m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
	m_siNoField  = -1;
}

VOID	OnlineMsgBox::SetLocation(SI32 x, SI32 y)
{
	m_ptBoxStart.x = x;
	m_ptBoxStart.y = y;

	m_ptButtStart[0].x = m_ptBoxStart.x + (m_pXsprBox[1]->Header.Xsize * m_siBoardXNum) + (m_pXsprBox[1]->Header.Xsize * 2) 
		- (m_pXsprButtBack->Header.Xsize * 2 + 17);
	m_ptButtStart[0].y = m_ptBoxStart.y + (m_pXsprBox[1]->Header.Ysize * m_siBoardYNum) + (m_pXsprBox[0]->Header.Ysize * 2) 
		- (m_pXsprButtBack->Header.Ysize + 15);
	m_ptButtStart[1].x = m_ptButtStart[0].x + m_pXsprButtBack->Header.Xsize + 2;
	m_ptButtStart[1].y = m_ptButtStart[0].y;

	m_Butt[0].SetX((SI16)m_ptButtStart[0].x);
	m_Butt[0].SetY((SI16)m_ptButtStart[0].y);
	if( m_siType == ON_MSGBOX_TYPE_OKCANCEL )
	{
		m_Butt[1].SetX((SI16)m_ptButtStart[1].x);
		m_Butt[1].SetY((SI16)m_ptButtStart[1].y);
	}
	else if( m_siType == ON_MSGBOX_TYPE_ACCEPTDECLINE )
	{
		m_Butt[1].SetX((SI16)m_ptButtStart[1].x);
		m_Butt[1].SetY((SI16)m_ptButtStart[1].y);
	}
}

VOID	OnlineMsgBox::SetAction(BOOL flag)
{
	m_bActive = flag;
	if(!flag)
	{
		if( m_siNoField != -1 )	m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField  = -1;
	}
}
