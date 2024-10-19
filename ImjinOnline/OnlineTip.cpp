/********************************************************************************************************
	파일명	:	OnlineTip.cpp
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2002.04.**
	수정일	:	
********************************************************************************************************/
#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineTip.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include <OnlineText.h>
#include "OnlineChatList.h"
#include "OnlineFieldArea.h"
#include "OnlineTradeBook.h"
#include "OnlineMsgBox.h"

extern _InputDevice				IpD;


OnlineTip::OnlineTip()
{
	m_bAction		=	FALSE;
	m_bFlag			=	FALSE;
	m_bMouseDown	=	FALSE;	  
	m_pOnlineWorld	=	NULL;
	m_siX			=	0;
	m_siY			=	0;
//	m_siCurLine		=	1;
	m_siNoField		=	-1;
	m_TipSelect     =   0;
	m_MainTipCount  =   0;
	m_SubTipCount   =   0;
	m_ContentsTipCount = 0;
	pOnlineTipContentsHeader = NULL;
	pOnlineTipHeader		 = NULL;
	m_iSelectTarget   =  0;	
	

}

OnlineTip::~OnlineTip()
{
	Free();
}


BOOL	OnlineTip::Init( cltOnlineWorld *pOnlineWorld )
{
	ZeroMemory(pOnlineTotalCount,sizeof(OnlineTotalCount) * 200);
	
	m_pOnlineWorld	=	pOnlineWorld;
	m_bAction		=	TRUE;

	clGrp.LoadXspr( "Online\\GameSpr\\HelpBoard.Spr", m_XsprTipBoard, TRUE);
	clGrp.LoadXspr( "Online\\GameSpr\\HelpButton.Spr", m_XsprButton );	

	m_siX		=	( ON_SCREEN_XSIZE - m_XsprTipBoard.Header.Xsize );
	m_siY		=	0;//( ON_SCREEN_YSIZE - m_XsprTipBoard.Header.Ysize ) / 2;
	m_siNoField	=	m_pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siX, m_siY, m_siX + m_XsprTipBoard.Header.Xsize, m_siY + m_XsprTipBoard.Header.Ysize );
	
	ZeroMemory(m_szTipTextList, 20 * 128);

	INT	siScrollBarX = m_siX + 222;

	INT	siScrollBarUp = m_siY + 42;
	INT siScrollBarDn = m_siY + 306;

	siScroll_Bar  = m_siY + 63;
	
	//버튼을 생성한다.
	m_BBack.Create( 0, 0, m_XsprButton.Header.Xsize, m_XsprButton.Header.Ysize, m_pOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_SUBJECT ), BUTTON_PUT_LEFT, TRUE );
	m_BExit.Create( 0, 0, m_XsprButton.Header.Xsize, m_XsprButton.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE );

	clGrp.LoadXspr("Online\\GameSpr\\HelpScrollBttnUp.Spr",pScroll_Up);
	clGrp.LoadXspr("Online\\GameSpr\\HelpScrollBttnDn.Spr",pScroll_Down);
	clGrp.LoadXspr("Online\\GameSpr\\HelpScrollBar.Spr",pScroll_Bar);
	 //= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_HELPSCROLLBTTON_UP);
	 //= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_HELPSCROLLBTTON_DN);
	 //= m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_HELPSCROLLBAR);

	m_Button_Scroll_Up.Create(siScrollBarX, siScrollBarUp, pScroll_Up.Header.Xsize, pScroll_Up.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Scroll_Down.Create(siScrollBarX,siScrollBarDn, pScroll_Down.Header.Xsize, pScroll_Down.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Scroll_Bar.Create(siScrollBarX,siScroll_Bar, pScroll_Bar.Header.Xsize, pScroll_Bar.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_BBack.SetX( m_siX + 19 );
	m_BBack.SetY( m_siY + 349 );
		
	m_BExit.SetX( m_siX + 164 );
	m_BExit.SetY( m_siY + 349 );

	pOnlineTipParser.ReadText("Online\\help.txt");

	m_TipSelect = 1;
	SetAction(FALSE);
	return TRUE;
}

UI16 OnlineTip::SelectContents(UI16 code)
{
	int i,j;
	UI16 iTotal,iSelect;
	int line;
	
	m_LineCount	 = 0;
	line		= 0;
	iSelect	    = 0;
	iTotal      = 0;

	iSelect = pOnlineTipParser.GetTipContentsSelect(code);
	iTotal = pOnlineTipParser.GetContentsTotal(iSelect);

	ZeroMemory(pOnlineTotalCount,sizeof(OnlineTotalCount) * 200);

	for(i = 0 ; i < iTotal ; i++)
	{
		pOnlineTipContentsHeader = pOnlineTipParser.GetTipContents(iSelect,i);

		strcpy(pOnlineTotalCount[m_LineCount].szLine,m_pOnlineWorld->pOnlineText->GetHelpText( pOnlineTipContentsHeader->uiSubject));
		pOnlineTotalCount[m_LineCount].uiVariable = pOnlineTipContentsHeader->uiVariable;
		pOnlineTotalCount[m_LineCount].uicode = pOnlineTipContentsHeader->uicode;
		pOnlineTotalCount[m_LineCount].uiSubject = 1;
		m_LineCount++;

//		strcpy(pOnlineTotalCount[m_LineCount].szLine,"");
//		m_LineCount++;

		ZeroMemory(m_Contents[i],sizeof(m_Contents[i]));

		strcpy(m_Contents[i],m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipContentsHeader->uiContents));

		line = DevideByLine(m_Contents[i],32);
		for(j = 0 ; j < line ; j++)
		{
			strcpy(pOnlineTotalCount[m_LineCount].szLine,m_szTipTextList[j]);
			pOnlineTotalCount[m_LineCount].uiVariable = 0;//pOnlineTipHeader[code].pOnlineTipContentsHeader[i].uiVariable;
			pOnlineTotalCount[m_LineCount].uicode = 0;//pOnlineTipHeader[code].pOnlineTipContentsHeader[i].uicode;
			pOnlineTotalCount[m_LineCount].uiSubject = 0;
			m_LineCount++;
		}
		strcpy(pOnlineTotalCount[m_LineCount].szLine,"");
		m_LineCount++;
	}
	m_TipSelect = 3;
	return iSelect;
}

BOOL OnlineTip::SelectSubject(UI16 Variable)
{
	
	int i,j;
	int line;
	UI16 iTotal;
	
	
	m_LineCount = 0;
	line = 0;
	iTotal = 0; 

	ZeroMemory(pOnlineTotalCount,sizeof(OnlineTotalCount) * 200);

	iTotal = pOnlineTipParser.GetSubjectTotal(Variable);

	for(i = 0 ; i < iTotal ; i++)
	{
		pOnlineTipContentsHeader = pOnlineTipParser.GetTipSubject(Variable,i);
		strcpy(pOnlineTotalCount[m_LineCount].szLine,m_pOnlineWorld->pOnlineText->GetHelpText( pOnlineTipContentsHeader->uiSubject));
		pOnlineTotalCount[m_LineCount].uiVariable = pOnlineTipContentsHeader->uiVariable;
		pOnlineTotalCount[m_LineCount].uicode = pOnlineTipContentsHeader->uicode;
		pOnlineTotalCount[m_LineCount].uiSubject = 1;
		m_LineCount++;

//		strcpy(pOnlineTotalCount[m_LineCount].szLine,"");
//		m_LineCount++;
		
		ZeroMemory(m_Contents[i],sizeof(m_Contents[i]));
		strcpy(m_Contents[i],m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipContentsHeader->uiContents));

		line = DevideByLine(m_Contents[i],32);
		for(j = 0 ; j < line ; j++)
		{
			strcpy(pOnlineTotalCount[m_LineCount].szLine,m_szTipTextList[j]);
			pOnlineTotalCount[m_LineCount].uiVariable = 0;//pOnlineTipHeader[code].pOnlineTipContentsHeader[i].uiVariable;
			pOnlineTotalCount[m_LineCount].uicode = 0;//pOnlineTipHeader[code].pOnlineTipContentsHeader[i].uicode;
			pOnlineTotalCount[m_LineCount].uiSubject = 0;
			m_LineCount++;
		}
		strcpy(pOnlineTotalCount[m_LineCount].szLine,"");
		m_LineCount++;

	}
	m_TipSelect = 2;

	return true;
	
}

VOID	OnlineTip::Action()
{
	
	if( !m_bAction )	return;

//	m_pOnlineWorld->bCharMove = FALSE;

	POINT	pt;
	SI16    TempCount;
	float   TempYFloat;


	pt.x	=	IpD.Mouse_X;
	pt.y	=	IpD.Mouse_Y;
	

	if(m_TipSelect == 1)
	{
		for(TempCount = 0 ; TempCount < 16 ; TempCount++)
		{
			//if(TempCount + m_MainTipScrollCount < 16)
			//{
				
				if( (pt.x >= (m_siX + 25)) && (pt.x <= (m_siX + 130)) &&
					(pt.y >= (m_siY + 48 + (TempCount * 17) ) ) && (pt.y <= (m_siY + 48 + (TempCount * 17) + 17)))
				{
					m_iSelectTarget = TempCount+m_MainTipScrollCount;

				}
			//}
		}
	}

	if(m_bMouseDown)
	{
		//Main
		if(m_TipSelect == 1)
		{
			pOnlineTipHeader = pOnlineTipParser.GetTipHeader();
			m_LineCount = pOnlineTipHeader->uiTotalTipHeader;

			

			for(TempCount = 0 ; TempCount < 16 ; TempCount++)
			{
				if(TempCount + m_MainTipScrollCount < m_LineCount)
				{
					if( (pt.x >= (m_siX + 25)) && (pt.x <= (m_siX + 130)) &&
						(pt.y >= (m_siY + 48 + (TempCount * 17)) ) && (pt.y <= (m_siY + 48 + (TempCount * 17) + 17)) && IpD.LeftPressSwitch)
					{
						//m_MainTipCount == 메인Tip에서 선택된 스트림.
						m_MainTipCount = TempCount + m_MainTipScrollCount;
						SelectSubject(m_MainTipCount);
						//ScrollCount 초기화.
						m_MainTipScrollCount = 0;
						IpD.LeftPressSwitch = FALSE;
					}	
				}
			}
		}
		//Sub
		else if(m_TipSelect == 2)
		{
			for(TempCount = 0 ; TempCount < 16 ; TempCount++)
			{
				if(TempCount + m_MainTipScrollCount < m_LineCount)
				{
					if( (pt.x >= (m_siX + 25)) && (pt.x <= (m_siX + 130)) &&
						(pt.y >= (m_siY + 48 + (TempCount * 17) )) && (pt.y <= (m_siY + 48 + (TempCount * 17) + 17)) && IpD.LeftPressSwitch)
					{
						m_SubTipCount = TempCount + m_MainTipScrollCount;

						//자세히 보기를 누르면.
						if(pOnlineTotalCount[m_SubTipCount].uiVariable == 1)
						{
							m_MainTipScrollCount = 0;
							m_ContentsTipCount = SelectContents(pOnlineTotalCount[m_SubTipCount].uicode);
							IpD.LeftPressSwitch = FALSE;
							
						}
					}
				}
			}
		}
	}
	if(m_bMouseDown)
	{
		//306
		//x = 14 y = 21
		if( ( pt.x >= (m_siX + 222))  &&  (pt.x <= (m_siX + 236)) &&
			( pt.y >= (m_siY + 63))    &&  (pt.y <= (m_siY + 294)) )
		{
			TempCount = (SI16)pt.y;
			if( (TempCount < (m_siY + 63)))
				TempCount = (m_siY + 63);
			if( (TempCount > (m_siY + 294)))
				TempCount = (m_siY + 294);

			TempCount = TempCount - (m_siY + 63);
			TempYFloat = (float)TempCount / 231.0f;

			if(m_TipSelect == 2 || m_TipSelect == 3 )
			{
				if(m_LineCount > 15)
				{
					m_MainTipScrollCount = (SI32)(TempYFloat * (float)(m_LineCount - 15));
				}
			}
		}
	}

	//메인 메뉴에선 스크롤 바를 출력시키지 않는다.
	if(m_TipSelect == 1)
	{
		m_MainTipScrollCount = 0;
	}
	//메인멘가 아닐경우.
	else if(m_TipSelect == 2 || m_TipSelect == 3)
	{
		if(m_Button_Scroll_Down.Process(m_bMouseDown) == TRUE)
			m_MainTipScrollCount++;
		if(m_Button_Scroll_Up.Process(m_bMouseDown) == TRUE)
			m_MainTipScrollCount--;
		if(m_MainTipScrollCount < 0)
			m_MainTipScrollCount = 0;
		if(m_MainTipScrollCount > m_LineCount - 16)
		{
			if(m_LineCount > 15)
				m_MainTipScrollCount = m_LineCount - 16;
			else
			{
				m_MainTipScrollCount = 0;
			}
		}
	}


	if( m_BBack.Process( m_bMouseDown ) )
	{
		m_TipSelect = 1;
		m_MainTipCount = 0;
		m_SubTipCount  = 0;
		m_MainTipScrollCount = 0;
		m_Button_Scroll_Bar.SetY(siScroll_Bar);
	}
	
	if( m_BExit.Process( m_bMouseDown ) )
	{
		SetAction( false );
	}

	if( IpD.LeftPressSwitch )		m_bMouseDown	=	TRUE;
	else							m_bMouseDown	=	FALSE;
	
}

// 찍어
VOID	OnlineTip::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	
	int TempCount;
	if( !m_bAction )	return;

	HDC		hDC;
	int i;
	int size;
	float TempYFloat;
	POINT pt;

//	m_siCurLine = 1;
	i = 0;
	size = 0 ;
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;
	
	// 이미지
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutMSprite( m_siX, m_siY, m_XsprTipBoard.Header.Xsize, m_XsprTipBoard.Header.Ysize, m_XsprTipBoard.Image );

		m_BBack.Put( &m_XsprButton, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BExit.Put( &m_XsprButton, 0, 2, 1, BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface( pSurface );
	}


	// TEXT
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		::SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON2));
		SetTextColor(hDC, m_pOnlineWorld->PaletteWhiteColor);

		m_BBack.Put( hDC );
		m_BExit.Put( hDC );
		pSurface->ReleaseDC( hDC );
	}

	if(m_TipSelect != 1)
	{
		m_Button_Scroll_Up.Put(&pScroll_Up, 0, 1, 2, BUTTON_PUT_NOMOVE);
		m_Button_Scroll_Down.Put(&pScroll_Down, 0, 1, 2, BUTTON_PUT_NOMOVE);
		m_Button_Scroll_Bar.Put(&pScroll_Bar, 0, 1, 2, BUTTON_PUT_NOMOVE);
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		::SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON2));
		SetTextColor(hDC, m_pOnlineWorld->PaletteWhiteColor);


		TempYFloat = 231.0f;
		if(m_LineCount > 16)
		{
			TempYFloat = (float)m_MainTipScrollCount / (float)(m_LineCount - 16);
			TempYFloat = (TempYFloat * 231.0f) + (float)(m_siY+ 63);
			m_Button_Scroll_Bar.SetY( (SHORT)TempYFloat );
		}

		pSurface->ReleaseDC( hDC );
	}
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		switch(m_TipSelect)
		{
		//Main 
		case 1:
			for(TempCount = 0 ; TempCount < 16 ; TempCount++)
			{	
				//목차.
				pOnlineTipHeader = pOnlineTipParser.GetTipHeader();
				m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 17) ,
					m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipHeader->uiMainSubjectIndex), RGB(216, 201, 183));
				
				if(TempCount + m_MainTipScrollCount < (int)pOnlineTipHeader->uiTotalTipHeader)
				{
					pOnlineTipContentsHeader = pOnlineTipParser.GetTipMain(TempCount + m_MainTipScrollCount);

					//마우스를 리스트위에 올리면.
					if(m_iSelectTarget == TempCount + m_MainTipScrollCount)
					{
						m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
						m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipContentsHeader->uiSubject), RGB(255, 255, 255));	
					}
					else
					{
						m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
						m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipContentsHeader->uiSubject), RGB(216,201, 183));
					}
				}
			}
		break;
		//Sub
		case 2:
			for(TempCount = 0 ; TempCount < 16; TempCount++)
			{
				//Main
				pOnlineTipHeader = pOnlineTipParser.GetTipSubjectHeader(m_MainTipCount);
				m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 17),
					m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipHeader->uiMainSubjectIndex), RGB(216, 201, 183));

				if(TempCount + m_MainTipScrollCount < m_LineCount)
				{
					if(pOnlineTotalCount[TempCount + m_MainTipScrollCount].uiSubject == 1)
					{
						//제목
						//Contents로 들어갈수 있으면.
						//밑줄 긋기.
						if(pOnlineTotalCount[TempCount + m_MainTipScrollCount].uiVariable == 1)
						{
							m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
							pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine, RGB(255, 255, 255));

							size = strlen(pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine);
							
							for(i = 0 ; i < size ; i++)
							{	
								m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25) + (i * 5) + 1, (m_siY + 48)+ (TempCount * 17) +8,
									"ㅡ",RGB(0, 255, 255));
							}	
						}
						else
						{
							m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
							pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine, RGB(216, 201, 183));
						}
					}
					//내용
					else
					{
						m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17) ,
						pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine, RGB(216, 201, 183));
						

					}
					
				}
			}
		break;
		//Contents
		case 3:
			for(TempCount = 0 ; TempCount < 16; TempCount++)
			{
				//Sub
				pOnlineTipHeader = pOnlineTipParser.GetTipContentsHeader(m_ContentsTipCount);
				m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 17),
					m_pOnlineWorld->pOnlineText->GetHelpText(pOnlineTipHeader->uiMainSubjectIndex), RGB(216, 201, 183));


				if(TempCount + m_MainTipScrollCount < m_LineCount)
				{
					//제목.
					if(pOnlineTotalCount[TempCount + m_MainTipScrollCount].uiSubject == 1)
					{
						m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
						pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine, RGB(216, 201, 183));
					}
					//내용.
					else
					{
						m_pOnlineWorld->pOnlineMegaText->DrawText(hDC, (m_siX + 25), (m_siY + 48) + (TempCount * 17),
						pOnlineTotalCount[TempCount + m_MainTipScrollCount].szLine, RGB(216, 201, 183));
					}
				}
			}
		break;
		}


		pSurface->ReleaseDC( hDC );
	}
	
	
}

// 활성화 한다.
VOID	OnlineTip::SetAction( BOOL bflag )
{
	// 활성 
	if( bflag )
	{
		m_bAction	=	bflag;
//		m_siCurLine	=	1;

		m_siX		=	( ON_SCREEN_XSIZE - m_XsprTipBoard.Header.Xsize );
		m_siY		=	0;//( ON_SCREEN_YSIZE - m_XsprTipBoard.Header.Ysize ) / 2;

		m_siNoField	=	m_pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siX, m_siY, m_siX + m_XsprTipBoard.Header.Xsize, m_siY + m_XsprTipBoard.Header.Ysize );

		m_BBack.SetX( m_siX + 19 );
		m_BBack.SetY( m_siY + 349 );
		
		m_BExit.SetX( m_siX + 164 );
		m_BExit.SetY( m_siY + 349 );

	// 팁 상자 영역 
		SetRect( &m_rtTip, m_siX, m_siY, m_siX + m_XsprTipBoard.Header.Xsize, m_siY + m_XsprTipBoard.Header.Ysize );
	}
	else
	{
		m_bAction	=	bflag;
		if( m_siNoField != -1 )
			m_pOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

		m_siNoField	=	-1;
//		m_pOnlineWorld->bCharMove = TRUE;
	}
}

VOID	OnlineTip::Free()
{
	pOnlineTipContentsHeader = NULL;
	pOnlineTipHeader         = NULL;

	clGrp.FreeXspr( m_XsprButton );
	clGrp.FreeXspr( m_XsprTipBoard );
	clGrp.FreeXspr( pScroll_Up );
	clGrp.FreeXspr( pScroll_Down );
	clGrp.FreeXspr( pScroll_Bar );
}


UI32 OnlineTip::DevideByLine(CHAR *pContent, SI32	siMaxStrLen)
{
	UI32	line;									// 몇 라인의 문자열인가?
	CHAR	buffer[64][128];
	CHAR	content[1024];
	SI32	Num;
	SI32	i, contentLen;
	
	wsprintf(content, "%s", pContent);
	contentLen = strlen(pContent);
	//---------------------------------
	// 문자열이 길 경우 여러 라인으로 나누어 버퍼에 넣는다.
	if( contentLen > siMaxStrLen )	
	{
		line = 0;
		Num = 0;	
		for( i=0; content[i] != '\0'; i++ )
		{
			// 받아온 문자열 중 한 문자를 버퍼에 넣는다.
			buffer[line][Num] = content[i];
			if(Num == 0)
			{
				if(buffer[line][Num] == ' ')
				{
					//i++;
					Num = 0;
					continue;
				}
			}
			Num++;

			// 한글일 경우
			if( UI08(content[i]) > 127 )
			{
				// 한글을 완성하지 못했는데 버퍼를 넘기면 이 한글은 다음 라인으로 넘긴다.
				if( Num >= siMaxStrLen )
				{
					buffer[line][Num-1] = '\0';
					line++;
					Num = 0;
					if(content[i] != ' ')
						i--;
					continue;
				}
			
				else
				{
					// 한 바이트 더 읽는다.
					buffer[line][Num++] = content[++i];
				}
			}

			if( Num >= siMaxStrLen )
			{
				buffer[line][Num] = '\0';
				line++;
				Num = 0;
			}
		}

		// 라인을 다 채우지 못하고 문자열이 끝났을 때 널 문자를 추가하고, 라인수를 설정한다.
		if( Num != 0 )
		{
			buffer[line][Num] = '\0';
			line++;
		}
	}
	else
	{
		wsprintf(buffer[0], "%s", content);
		line = 1;
	}
	
	//---------------------------------
	// TIP 문자열 리스트에 받아온 문자열 넣기

	for( i=0; i<(SI32)line; i++ )
	{
		// 문자열 한 줄을 추가한다
		strcpy(m_szTipTextList[i], buffer[i]);
	}

	//현재 출력되어야 하는 라인 설정

	return line;
}



