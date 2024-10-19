// 파일명 : OnlineMsgBox.cpp
// 담당자 : 정진욱

#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineMsgBox.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include "OnlineFieldArea.h"

#define ON_MSGBOX_CHAR_NUM_PER_LINE			40		// 한 라인에 들어갈 수 있는 최대 문자수
#define ON_MSGBOX_CHAR_WIDTH				6		// 한 문자의 가로 픽셀 사이즈 
#define ON_MSGBOX_CHAR_HEIGHT				14		// 한 문자의 세로 픽셀 사이즈 
#define ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE 1

extern _InputDevice				IpD;

///////////////////////////////////////////////////////////////////////////////
//	설명	: 
VOID OnlineMsgBox::Init(cltOnlineWorld *pOnlineWorld, BOOL b)
{
	m_pOnlineWorld		= pOnlineWorld;
	m_bMouseDown		= FALSE;
	m_bActive			= FALSE;
	m_siNoField			= -1;
	m_bLogin			= b;

	if(b == TRUE)
	{
		m_pXsprBox[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BOX1);		// 판넬(위, 아래)
		m_pXsprBox[1]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BOX2);		// 판넬(중간)
		m_pXsprButt[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_LOGIN_MESSAGE_BUTTON);		// 확인 버튼
	}
	else if( b == FALSE)
	{
		m_pXsprBox[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOXBOARD1);		// 판넬(위, 아래)
		m_pXsprBox[1]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOXBOARD2);		// 판넬(중간)
		m_pXsprButt[0]		= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_MSGBOX_BUTTON);		// 확인 버튼
	}
	
	m_Butt[0].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE );
	m_Butt[1].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, TRUE );
}

///////////////////////////////////////////////////////////////////////////////
//	설명	: 
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
	// 왼쪽 마우스가 눌러졌는가?
	if( IpD.LeftPressSwitch )	m_bMouseDown = TRUE;
	else						m_bMouseDown = FALSE;

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	설명	: 
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
//	설명	: '취소' 또는 'X'(닫기 아이콘)을 눌렀는가?
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
//	설명	: 
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

	// 노 필드 지역 선언
	SetRect( &m_rtMsgBox, m_ptBoxStart.x, m_ptBoxStart.y, m_ptBoxStart.x + ( ( m_siBoardXNum * m_pXsprBox[1]->Header.Xsize ) + ( m_pXsprBox[0]->Header.Xsize * 2 ) ), 
		   ( m_ptBoxStart.y + ( m_siBoardYNum * m_pXsprBox[1]->Header.Ysize ) + ( m_pXsprBox[0]->Header.Ysize * 2 ) + m_pXsprButt[0]->Header.Ysize ) );
	m_siNoField = m_pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( &m_rtMsgBox );
}


///////////////////////////////////////////////////////////////////////////////
//	설명	: 멤버변수값을 설정한다.
VOID OnlineMsgBox::SetData(CHAR *pCaption, CHAR *pMsg)
{
	SI32	siCapLen, siMsgLen, siLen;
	SI32	siIndex, siLine;
	BOOL	bSwitch;
	SI32	siMsgWidth, siMsgHeight;			// 내용 문자열의 너비와 높이
	CHAR	*pToken;
	CHAR	buffer[ON_MSGBOX_MAX_MSG_LENGTH];
	SI32	i;

	if( pCaption == NULL || pMsg == NULL )	return;



	//---------------------------------
	// 제목 문자열 설정
	strcpy(m_strCaption, pCaption);			
	siCapLen = strlen(m_strCaption);
	if( siCapLen > ON_MSGBOX_MAX_CAPTION_LENGTH ) return;
		
	//---------------------------------
	// 내용 문자열 설정
	// 문자열이 길 경우 여러 라인의 문자열로 만들어야 한다. '\n' 끼워넣기
	siIndex = 0;
	siLine = 0;
	bSwitch = FALSE;
	SI32 siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// 현재 메시지 박스 한 라인에 있는 문자 개수

	for( i=0; pMsg[i] != '\0'; i++ )
	{
		if( siIndex > ON_MSGBOX_MAX_MSG_LENGTH )	break;		// 문자열이 제한범위보다 클 경우 그냥 잘라 버리자.

		// robypark 2004/12/23 14:00
		// OnlineText.dat에 \n이 추가되었을 경우 이를 New Line으로 인식하도록 수정되었다.
		// 메시지 박스에서 출력할 때 처리되도록 수정
		// carriage return 빼기
		if (pMsg[i] == '\r' )
			continue;

		m_strMsg[siIndex++] = pMsg[i];
		siCurrent_On_Msgbox_Char_Num_In_Line++;	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 문자수 계산
		if( pMsg[i] == '\n' )
		{
			siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 문자수 초기화
			siLine++;
		}

		// 한글일 경우
		if( UI08(pMsg[i]) > 127 )
		{
			// 한글을 완성하지 못했는데 버퍼를 넘기면 이 한글은 다음 라인으로 넘긴다.
//			if (((siIndex-siLine) >= ON_MSGBOX_CHAR_NUM_PER_LINE) && ((siIndex-siLine)%ON_MSGBOX_CHAR_NUM_PER_LINE == 0) 
			if ((siCurrent_On_Msgbox_Char_Num_In_Line >= ON_MSGBOX_CHAR_NUM_PER_LINE)	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 최대 문자수 검사
				&& (bSwitch == FALSE) )													// OnlineText.dat에 \n이 추가되었을 경우 이를 New Line으로 인식하도록 수정되었다.
																						// 메시지 박스에서 출력할 때 처리되도록 수정
			{
				m_strMsg[siIndex-1] = '\n';
				siLine++;

				siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 문자수 초기화

				i--;
				bSwitch = TRUE;
				continue;
			}
			else
			{
				// 한 바이트 더 읽는다.
				m_strMsg[siIndex++] = pMsg[++i];

				siCurrent_On_Msgbox_Char_Num_In_Line++;	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 문자수 계산
			}
		}

//		if( (siIndex-siLine) >= ON_MSGBOX_CHAR_NUM_PER_LINE && (siIndex-siLine)%ON_MSGBOX_CHAR_NUM_PER_LINE == 0 )
		if ( siCurrent_On_Msgbox_Char_Num_In_Line >= ON_MSGBOX_CHAR_NUM_PER_LINE )	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 최대 문자수 검사
																					// OnlineText.dat에 \n이 추가되었을 경우 이를 New Line으로 인식하도록 수정되었다.
																					// 메시지 박스에서 출력할 때 처리되도록 수정
		{
			m_strMsg[siIndex++] = '\n';

			siCurrent_On_Msgbox_Char_Num_In_Line = 0;	// robypark 2004/12/23 14:00, 한 라인에 들어갈 수 있는 문자수 초기화

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
		

	// 한 줄의 내용 문자열의 최대 길이(siMsgLen) 알아오기
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
	// 내용 문자열 너비 설정
	siMsgWidth = siLen * ON_MSGBOX_CHAR_WIDTH;

	//---------------------------------
	// 내용 문자열 높이 설정
	siMsgHeight = siLine * ON_MSGBOX_CHAR_HEIGHT;			
	if( siMsgHeight == 0 ) 	siMsgHeight = ON_MSGBOX_CHAR_HEIGHT;



	//---------------------------------
	// 글자수에 따라 가운데 보드의 가로 타일수를 결정한다.
	m_siBoardXNum = siMsgWidth / m_pXsprBox[1]->Header.Xsize;		
	if( siMsgWidth % m_pXsprBox[1]->Header.Xsize != 0 )		m_siBoardXNum++;
	if( m_siBoardXNum == 0 ) m_siBoardXNum = 1;

	//---------------------------------
	// 글자수에 따라 가운데 보드의 세로 타일수를 결정한다.
	m_siBoardYNum = siMsgHeight / m_pXsprBox[1]->Header.Ysize;
	if( siMsgHeight % m_pXsprBox[1]->Header.Ysize != 0 )	m_siBoardYNum++;
	if( m_siBoardYNum == 0 ) m_siBoardYNum = 1;
	m_siBoardYNum += 1;		// 버튼의 공간 세칸
	
	//---------------------------------
	// 메시지 박스 시작 좌표 설정하기
	m_ptBoxStart.x = clGrp.GetScreenXsize() / 2 - (((m_siBoardXNum+2) * m_pXsprBox[1]->Header.Xsize) / 2);
	m_ptBoxStart.y = clGrp.GetScreenYsize() / 2 - (((m_siBoardYNum+3) * m_pXsprBox[1]->Header.Ysize) / 2);

	// robypark 2004/12/20 17:39
	// 확인, 취소 버튼의 텍스트 설정 Init() 함수에서 복사함. 수락, 거절 텍스트 추가를 위한
	m_Butt[0].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE );
	m_Butt[1].Create(0, 0, m_pXsprButt[0]->Header.Xsize, m_pXsprButt[0]->Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, TRUE );

	//---------------------------------
	// 확인, 취소 버튼의 시작 좌표 설정하기
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
		// 수락, 거절 버튼의 텍스트 설정.
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
	
	// 메시지 박스 버튼 위치 보정
	m_Butt[0].SetX((SI16)m_ptButtStart[0].x);
	m_Butt[0].SetY((SI16)m_ptButtStart[0].y);
	m_Butt[1].SetX((SI16)m_ptButtStart[1].x);
	m_Butt[1].SetY((SI16)m_ptButtStart[1].y);
}




///////////////////////////////////////////////////////////////////////////////
//	설명	: 
VOID OnlineMsgBox::DrawBox()
{
	POINT	ptBoard;			// 보드의 좌표
	SI32	i, j;	

	ptBoard = m_ptBoxStart;

	if(m_bLogin)
	{
		//---------------------------------
		// 메시지 박스 판넬
		// 왼쪽 위
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[0]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// 중간 위
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[1]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// 오른쪽 위
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[2]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		ptBoard.y += m_pXsprBox[0]->Header.Ysize;
		
		
		
		for( i=0; i<m_siBoardYNum; i++ )
		{
			ptBoard.x = m_ptBoxStart.x;
			// 왼쪽 중간
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[0]]);
			ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			
			// 중간 중간
			for( j=0; j<m_siBoardXNum; j++ )
			{
				clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
					m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
					&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[1]]);
				ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			}
			
			// 오른쪽 중간
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[2]]);
			ptBoard.y += m_pXsprBox[1]->Header.Ysize;
		}
		
		
		ptBoard.x = m_ptBoxStart.x;
		// 왼쪽 아래
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[3]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// 중간 아래
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[4]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// 오른쪽 아래
		clGrp.PutSpriteT(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[5]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
	}
	else
	{
		//---------------------------------
		// 메시지 박스 판넬
		// 왼쪽 위
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[0]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// 중간 위
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[1]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// 오른쪽 위
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[2]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		ptBoard.y += m_pXsprBox[0]->Header.Ysize;
		
		for( i=0; i<m_siBoardYNum; i++ )
		{
			ptBoard.x = m_ptBoxStart.x;
			// 왼쪽 중간
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[0]]);
			ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			
			// 중간 중간
			for( j=0; j<m_siBoardXNum; j++ )
			{
				clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
					m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
					&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[1]]);
				ptBoard.x += m_pXsprBox[1]->Header.Xsize;
			}
			
			// 오른쪽 중간
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[1]->Header.Xsize, m_pXsprBox[1]->Header.Ysize,
				&m_pXsprBox[1]->Image[m_pXsprBox[1]->Header.Start[2]]);
			ptBoard.y += m_pXsprBox[1]->Header.Ysize;
		}
		
		
		ptBoard.x = m_ptBoxStart.x;
		// 왼쪽 아래
		clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
			m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
			&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[3]]);
		ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		
		// 중간 아래
		for( i=0; i<m_siBoardXNum; i++ )
		{
			clGrp.PutSpriteJin(ptBoard.x, ptBoard.y, 
				m_pXsprBox[0]->Header.Xsize, m_pXsprBox[0]->Header.Ysize,
				&m_pXsprBox[0]->Image[m_pXsprBox[0]->Header.Start[4]]);
			ptBoard.x += m_pXsprBox[0]->Header.Xsize;
		}
		
		// 오른쪽 아래
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
//	설명	: 
VOID OnlineMsgBox::DrawText(HDC hDC)
{
	POINT	ptTextStart, ptText;			// 보드의 좌표
	CHAR	*pToken;
	CHAR	buffer[ON_MSGBOX_MAX_MSG_LENGTH];
	SI32	i;	

	
	SetBkMode(hDC, TRANSPARENT); // 투명 모드 설정.
	SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHATLIST));
	SetTextColor(hDC, RGB(255, 255, 255));


	
	// 버튼을 찍는다.
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

	// 제목을 찍는다.
	ptText = ptTextStart;
	TextOut(hDC, ptText.x, ptText.y, m_strCaption, strlen(m_strCaption));

	// 내용을 찍는다.
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
//	설명	: 
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
//	설명	:	현재 메시지 박스가 출력 중인가?
BOOL	OnlineMsgBox::IsActive()
{
	return m_bActive;
}

///////////////////////////////////////////////////////////////////////////////
//	설명	:	메시지 박스 초기화
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
