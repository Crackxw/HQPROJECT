//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 퀘스트창
//	File Name		: OnlineQuestInterface.cpp
//	Birth Date		: 2004. 01. 01.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용자 콘솔에서 임무 수행 목록 창을 띄워주는 부분입니다.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: 포함 헤더 선언
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <mouse.h>
#include "OnlineWorld.h"
#include "OnlineQuestInterface.h"
#include "../ImjinOnline/OnlineTradeBook.h"
#include "../ImjinOnline/OnlineVillageSave.h"

#include "OnlineFieldArea.h"
//#include "OnlineClient.h"		// actdoll (2004/02/16 16:58) : 없어도 컴파일이 되는 헤더. 진짜 필요없던가 상위 헤더 내에 선언되어 있는 경우임. 이것 때문에 문제 발생시 다시 켜기 바람
#include "OnlineQuest-Parser.h"
#include "OnlineMegaText.h"

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: 정의 및 선언부
//-----------------------------------------------------------------------------
extern _InputDevice		IpD;			// 입력 장치 외부에 선언한 것

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-01-01)
// Desc: 클래스 초기화. 생성시 한번만 호출된다.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Init()
{
	// 각종 정보 초기화
	m_nNoField		= -1;
	m_bAction		= FALSE;
	m_bMousePush	= FALSE;
	pfDraw			= Draw_QuestList;
	pfAction		= Action_QuestList;

	// 이미지 로딩한다.
	LoadImage();

	// 버튼들 생성
	m_btOK.Create		( 292 + 80,		155 + 146,								m_Spr_Btn_Common.Header.Xsize,		m_Spr_Btn_Common.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(6300005),	BUTTON_PUT_LEFT, FALSE );
	m_btClose.Create	( 262 + 187,	150 + 156,								m_Spr_Btn_Common.Header.Xsize,		m_Spr_Btn_Common.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(6300005),	BUTTON_PUT_LEFT, FALSE );
	m_btUp.Create		( 493,			155 + 18,								m_Spr_Btn_ScrollUp.Header.Xsize,	m_Spr_Btn_ScrollUp.Header.Ysize,	"",												BUTTON_PUT_LEFT, FALSE );
	m_btDown.Create		( 493,			155 + 18 + m_Spr_Bar_List.Header.Ysize,	m_Spr_Btn_ScrollDown.Header.Xsize,	m_Spr_Btn_ScrollDown.Header.Ysize,	"",												BUTTON_PUT_LEFT, FALSE );
	
	// 리스트 박스 생성
	m_LB_QuestList.Create( 292 + 4, 155 + 19, 178, 100, 292+16, 155+30, 178, 100, 170, 20, NULL, &m_Spr_Btn_ScrollHandle, 292 + 207, 155 + 18 + m_Spr_Btn_ScrollDown.Header.Ysize, 155 + 18 + m_Spr_Bar_List.Header.Ysize, TRUE);
	m_LB_QuestList.EnableDrawItemColor( true );	// actdoll (2004/01/13 14:50) : 아이템 색깔별로 지정된 색을 표기하게끔 한다.
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: actdoll (2004-01-01)
// Desc: 클래스 제거. 소멸시 한번만 호출된다.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Free()
{
	// 이미지 해제한다.
	FreeImage();
}

//-----------------------------------------------------------------------------
// Name: LoadImage()
// Code: actdoll (2004-01-01)
// Desc: 스프라이트 이미지 로딩
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::LoadImage()
{
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_1.Spr",			m_Spr_Frm_List );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_2.Spr",			m_Spr_Frm_Info );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_up.Spr",		m_Spr_Btn_ScrollUp );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_down.Spr",	m_Spr_Btn_ScrollDown );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_1_SCRBAR.Spr",	m_Spr_Bar_List );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_2_SCRBAR.Spr",	m_Spr_Bar_Info );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_handle.Spr",	m_Spr_Btn_ScrollHandle );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_button.Spr",		m_Spr_Btn_Common );	
}

//-----------------------------------------------------------------------------
// Name: FreeImage()
// Code: actdoll (2004-01-01)
// Desc: 스프라이트 이미지 해제
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::FreeImage()
{
	clGrp.FreeXspr( m_Spr_Frm_List );
	clGrp.FreeXspr( m_Spr_Frm_Info );

	clGrp.FreeXspr( m_Spr_Btn_ScrollUp );
	clGrp.FreeXspr( m_Spr_Btn_ScrollDown );
	
	clGrp.FreeXspr( m_Spr_Bar_List );
	clGrp.FreeXspr( m_Spr_Bar_Info );
	
	clGrp.FreeXspr( m_Spr_Btn_ScrollHandle );
	clGrp.FreeXspr( m_Spr_Btn_Common );	
}

//-----------------------------------------------------------------------------
// Name: Draw_QuestList()
// Code: actdoll (2004-01-01)
// Desc: 퀘스트 창 출력 - 퀘스트 리스트
//		pSurface	- 찍을 화면의 서피스
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Draw_QuestList( LPDIRECTDRAWSURFACE7 pSurface )
{	
	// 그림
	if(clGrp.LockSurface(pSurface))
	{
		// 프레임 출력
		clGrp.PutSpriteJin(292, 155, m_Spr_Frm_List.Header.Xsize, m_Spr_Frm_List.Header.Ysize, &m_Spr_Frm_List.Image[m_Spr_Frm_List.Header.Start[0]]);
		clGrp.PutSpriteT(292, 155, m_Spr_Frm_List.Header.Xsize, m_Spr_Frm_List.Header.Ysize, &m_Spr_Frm_List.Image[m_Spr_Frm_List.Header.Start[1]]);

		// OK버튼 출력
		m_btOK.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE );

//		if(IsShowMainScroll())
		{
			// 스크롤 바 출력
			clGrp.PutSpriteT(495, 173, m_Spr_Bar_List.Header.Xsize, m_Spr_Bar_List.Header.Ysize, m_Spr_Bar_List.Image);

			// 위아래 스크롤 버튼 출력
			m_btUp.Put(&m_Spr_Btn_ScrollUp, 0, 0, 1, BUTTON_PUT_NOMOVE);
			m_btDown.Put(&m_Spr_Btn_ScrollDown, 0, 0, 1, BUTTON_PUT_NOMOVE);
		}

		clGrp.UnlockSurface(pSurface);
	}

	// 글씨
	HDC hDC;
	if(pSurface->GetDC(&hDC) == DD_OK)
	{		
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 297, 159, 206, 13, m_pMyOnlineWorld->pOnlineText->Get(8000567),RGB(245, 245, 245));	// 제목 표시줄

		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));		
		m_btOK.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}

//	if(IsShowMainScroll())
	// 퀘스트 리스트 출력
	m_LB_QuestList.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), RGB(64, 64, 255), RGB(0, 0, 0), true, m_SelectColor, RGB(200, 200, 200) );
}

//-----------------------------------------------------------------------------
// Name: Draw_QuestInfo()
// Code: actdoll (2004-01-01)
// Desc: 퀘스트 창 출력 - 퀘스트 상세정보
//		pSurface	- 찍을 화면의 서피스
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Draw_QuestInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 그림
	if(clGrp.LockSurface(pSurface))
	{
		// 프레임 출력
		clGrp.PutSpriteJin(262, 150, m_Spr_Frm_Info.Header.Xsize, m_Spr_Frm_Info.Header.Ysize, &m_Spr_Frm_Info.Image[m_Spr_Frm_Info.Header.Start[0]]);
		clGrp.PutSpriteT(262, 150, m_Spr_Frm_Info.Header.Xsize, m_Spr_Frm_Info.Header.Ysize, &m_Spr_Frm_Info.Image[m_Spr_Frm_Info.Header.Start[1]]);

		// 버튼 출력
		m_btOK.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE);
		m_btClose.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	// 글씨
	HDC hDC;
	char	pszTemp[512], pszBuff[512], pszCount[64] = {0,};	
	if(pSurface->GetDC(&hDC) == DD_OK)
	{		
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 271, 153, 267, 13, m_pMyOnlineWorld->pOnlineText->Get(8000568),RGB(245, 245, 245));		// 제목표시줄
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 266, 170, 74, 18, m_pMyOnlineWorld->pOnlineText->Get(8000093), RGB(245, 245, 245));		// "임무"
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 266, 191, 74, 18, m_pMyOnlineWorld->pOnlineText->Get(8000402), RGB(245, 245, 245));		// "임무수행원"

		// 선택한 아이템의 퀘스트 코드를 받고
		SQuestHeader *pQH = (SQuestHeader*)m_LB_QuestList.GetData(m_siSelect);
		if( !pQH )
		{
			pSurface->ReleaseDC(hDC);
			return;
		}
		SI32	siSelectedQuestCode = pQH->siQuestCode;

//		pQH->sQuestReqConditional.m_QuestConditionalQuantity;
//		m_QuestData.m_sPlayingQuest[TempCount]
		char	*pTextTitle	= m_pMyOnlineWorld->pOnlineText->GetQuestText(pQH->siSubject);
		char	*pTextNPC	= m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetQuestNPCAllMap(pQH->siNPCCode)->siName);
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 262+88, 150+23, pTextTitle, RGB(245, 245, 245));	// 임무 제목명
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 262+88, 150+43, pTextNPC, RGB(245, 245, 245));		// 대상 NPC 이름

		// 임무 내용 문장 병합
		char			*pText;
		SI32			siLimitCount, siCurrentCount;		// 조건을 맞추기위한 최종 수치와 현재 수치
		SI32			siRealSelIndex;
		COLORREF		textColor;
		SI32			siObjectCountForFinish;				// 
		SQuestHeader*	lpSQuestHeader;
		BOOL			bOpen		= false;
		BOOL			bIsPlaying	= false;

		// 선택한 퀘스트가 플레이중인지 해결한 놈인지 찾는다.
		for( SI32 i=0; i<ON_QUSETDATA_MAXPLAYINGQUEST; i++ )
		{
			// 현재 플레이중인 퀘스트라면
			if( m_QuestData.m_sPlayingQuest[i].m_QuestCode == siSelectedQuestCode )		
			{
				siRealSelIndex	= i;		// 배열 번호 넣어주고
				bIsPlaying		= true;		// 진행중이다.
				break;
			}
		}

		// 본문을 찍어준다.
		strcpy( pszTemp, m_pMyOnlineWorld->pOnlineText->GetQuestText(pQH->m_siDesc) );	// 본문 찍고

		// 퀘스트 조건 찍는다.
		if( bIsPlaying )		// 진행중이라면
		{
			if( siRealSelIndex >= 0 )
			{
				// 퀘스트 헤더 받고
				lpSQuestHeader = m_pMyOnlineWorld->pQuestParser->GetQuest( m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestCode );
				
				// 퀘스트 종료를 위해 필요한 모든 조건을 뒤진다.
				for( siObjectCountForFinish = 0; siObjectCountForFinish < ON_QUSETDATA_MAXQUESTENDREQOBJECT; siObjectCountForFinish++ )
				{
					// 종료 목적이 들어있는 헤더를 받고
					SQuestObject	*pEndQuestObject = &lpSQuestHeader->sQuestEndReqObject[siObjectCountForFinish];
					// 해당 타입이 존재할 때
					if( pEndQuestObject->m_QuestObjectType != QUESTOBJECTTYPE_NONE )
					{
						if( !bOpen )	// 첫방일때는 가로 쳐주고
						{
							strcat( pszTemp, "\n\n[" );
							bOpen = true;
						}
						else			// 그렇지 않을 때는 구분을 해준다.
						{
							strcat( pszTemp, ", " );
						}
						
						siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;		// 해당 조건에 대해 충족할 수 있는 수치 받고
						siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
						switch(pEndQuestObject->m_QuestObjectType)	// 요구하는 종류를 체크한다.
						{
						case QUESTOBJECTTYPE_ITEM:		// 아이템을 가져오라고 요구
							{
								// 해당 아이템명 : 현재갯수 / 도달갯수
								CItemHeader		*lpCItemHeader;
								lpCItemHeader	= m_pMyOnlineWorld->pItemsInfo->GetItem( pEndQuestObject->m_QuestObjectCode );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( lpCItemHeader->m_siItemName );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetItemCount( pEndQuestObject->m_QuestObjectCode );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_WORK:		// 일을 하라고 요구
							{
								// 해당 작업명 : 현재 작업량 / 도달 작업량
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_WORKDATA );
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_HEALTH:		// 체력을 채워라(?)
							{
								// 해당 작업명 : 현재 체력 / 최대 체력
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_WORKDATA );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife( 0 );
								siLimitCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife( 0 );
								
								sprintf( pszBuff, " %s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
							
						case QUESTOBJECTTYPE_BANKBOOK:		// 계좌 만들기
							{
								// 해당 작업명 : 현재 계좌수 / 최대 계좌수
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_BANKACCOUNT );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %I64d/%d", pText, m_pMyOnlineWorld->pOnlineTradeBook->Quest_GetBankBookCount(), siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_PORTAL:		// 저잣거리 돌아댕기기
							{
								// 해당 작업명 : 현재 수  / 최대 수
								pText			= m_pMyOnlineWorld->pOnlineText->Get(8000569);
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_NPC:			// NPC 만나기
							{
								// 해당 작업명 : 현재 수 / 최대 수
								pText			= m_pMyOnlineWorld->pOnlineText->Get(8000570);
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_BOOK:			// 교역지 방문하기
							{	
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_TRADEBOOK );
								siCurrentCount	= m_pMyOnlineWorld->pOnlineVillageSave->GetVillageNum();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_MERCENARY:		// 용병 고용하기
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_SOILDERCOUNT );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerNum() - 1;
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_MONSTER:		// 몬스터 잡기
							{
								CharHead*		lpCharHead;
								lpCharHead		= m_pMyOnlineWorld->pIOnlineCharKI->GetCharHeader( pEndQuestObject->m_QuestObjectCode );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( lpCharHead->uiCharName );
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
							
						case QUESTOBJECTTYPE_SATIETY:		// 포만감 채우기
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_HUNGRY );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetHealth();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_LEVEL:			// 레벨 올리기
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_LV );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( 0 );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_QUEST:			// 퀘스트 깨기
							{
								SQuestHeader*	lpTemp;
								lpTemp			= m_pMyOnlineWorld->pQuestParser->GetQuest( siLimitCount );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_QUESTNO );
								for( SI32 j=0; j<ON_QUSETDATA_MAXCOMLETEDQUEST; j++ )
								{
									if( m_QuestData.m_siCompletedQuest[j] > 0 )
									{
										if( m_QuestData.m_siCompletedQuest[j] >= siLimitCount )
										{
											pText	= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_QUESTOK );
											break;
										}
									}
								}
								sprintf( pszBuff, pText, m_pMyOnlineWorld->pOnlineText->GetQuestText( lpTemp->siSubject ) );
							}
							break;
							
						case QUESTOBJECTTYPE_MONEY:			// 돈
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_MONEY );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetMoney();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %I64d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
						}// eof switch()
						// 본문에 덧붙임
						strcat( pszTemp, pszBuff );
					}
				}
			}
			
			// 괄호가 완료된 상태에서 튀어나왔다면 닫아라
			if( bOpen )
			{
				strcat( pszTemp, "]" );
			}
			textColor = RGB( 255, 128, 128 );	
		}
		else		// 끝난 놈이라면
		{
			strcat( pszTemp, "\n\n" );
			strcat( pszTemp, m_pMyOnlineWorld->pOnlineText->Get(8000571) );			// 작업완료
			textColor = RGB( 128, 128, 255 );		
		}
//		m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine( hDC, 262+16, 150+69, 239, pszTemp, textColor );	// 임무 내용
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, 262+16, 150+69, 242, 292, NULL, pszTemp, textColor, DT_VCENTER );
		// 버튼 텍스트
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
		m_btOK.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_btClose.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}
}

//-----------------------------------------------------------------------------
// Name: Action_QuestList()
// Code: actdoll (2004-01-01)
// Desc: 퀘스트 창 작동 - 퀘스트 리스트
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Action_QuestList()
{
	// 버튼 눌렸나 확인
	if( m_btOK.Process	( m_bMousePush ) )		OnButtonPush();				// OK
	if( m_btUp.Process	( m_bMousePush ) )		m_LB_QuestList.PrevSell();	// 위 스크롤
	if( m_btDown.Process	( m_bMousePush ) )	m_LB_QuestList.NextSell();	// 아래 스크롤

	// 리스트 박스 상태 체
	m_LB_QuestList.Action(IpD.Mouse_X, IpD.Mouse_Y, m_bMousePush, &m_siSelect);

	// 선택이 왔을 경우
	if(m_siSelect > -1)
	{
		// 각각의 함수 포인터 물려주고
		pfDraw   = Draw_QuestInfo;
		pfAction = Action_QuestInfo;
		
		// 버튼 좌표 변경
		m_btOK.SetX(262+32);
		m_btOK.SetY(150+156);

		// 버튼별 기능 체크
		m_btOK.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(1000114));
		m_btClose.SetAction(TRUE);

		// actdoll (2004/01/16 17:31) : 선택한 데이터의 퀘스트 코드를 받고
		SQuestHeader *pQH = (SQuestHeader*)m_LB_QuestList.GetData(m_siSelect);
		if( !pQH )	return;
		
		// 선택한 퀘스트가 플레이중인지 해결한 놈인지 찾는다.
		m_SelectColor	= RGB( 128, 128, 255 );
		for( SI32 i=0; i<ON_QUSETDATA_MAXPLAYINGQUEST; i++ )
		{
			// 현재 플레이중인 퀘스트라면
			if( m_QuestData.m_sPlayingQuest[i].m_QuestCode == pQH->siQuestCode )		
			{
				m_SelectColor	= RGB( 255, 128, 128 );
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Action_QuestInfo()
// Code: actdoll (2004-01-01)
// Desc: 퀘스트 창 작동 - 퀘스트 정보
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Action_QuestInfo()
{
	BOOL	bPushOK		= m_btOK.Process(m_bMousePush);
	BOOL	bPushClose	= m_btClose.Process(m_bMousePush);

	// 버튼 암것도 안눌렸다면 리턴
	if( !bPushOK && !bPushClose )		return;

	// 위치 보정
	m_btOK.SetX(292+80);
	m_btOK.SetY(155+146);
	
	// 버튼별 설정
	m_btOK.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(6300005));
	m_btClose.SetAction(FALSE);
	
	// 함수 포인터 보종
	pfDraw   = Draw_QuestList;
	pfAction = Action_QuestList;		

	// 닫기 버튼이 눌린 것이라면 처리
	if( bPushClose )		OnButtonPush();
}

//-----------------------------------------------------------------------------
// Name: SetQuestList()
// Code: actdoll (2004-01-01)
// Desc: 퀘스트 리스트 창에 해당 퀘스트들의 제목을 적용한다.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::SetQuestList()
{
	int		TempCount, nCount = 1;
	char	szTemp[512];

	// 일단 홀랑 지우고
	m_LB_QuestList.DelAllSell();
	m_LB_QuestList.ClearItemColor();

	// 현재 진행하고 있는 퀘스트 리스트 먼저 적재
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			// 해당 진행 퀘스트 헤더를 받고
			m_QuestData.m_sPlayingQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode);
			sprintf(szTemp, "%d. %s", nCount, m_pMyOnlineWorld->pOnlineText->GetQuestText(m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode)->siSubject) );
			m_LB_QuestList.AddSell(m_QuestData.m_sPlayingQuestHeader[TempCount],szTemp);
			m_LB_QuestList.SetItemColor( nCount - 1, RGB( 255, 64, 64 ) );
			nCount++;
		}
	}

	// 이미 끝낸 퀘스트 리스트를 적재
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount]);

			sprintf(szTemp, "%d. %s", nCount, m_pMyOnlineWorld->pOnlineText->GetQuestText(m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount])->siSubject));
			m_LB_QuestList.AddSell(m_QuestData.m_siCompletedQuestHeader[TempCount],szTemp);
			nCount++;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-01-01)
// Desc: 메인 처리
//-----------------------------------------------------------------------------
void OnlineQuestInterface::Action()
{
	if(!IsAction())		return;

	(this->*pfAction)();

	m_bMousePush = IpD.LeftPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-01-01)
// Desc: 메인 출력
//-----------------------------------------------------------------------------
void OnlineQuestInterface::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!IsAction())		return;

	(this->*pfDraw)(pSurface);
}

//-----------------------------------------------------------------------------
// Name: OnButtonPush()
// Code: actdoll (2004-01-01)
// Desc: 이 콘솔을 호출하는 버튼이 눌렸다!
//-----------------------------------------------------------------------------
void OnlineQuestInterface::OnButtonPush()
{
	ToggleAction();

//	FreeImage();

	m_btOK.SetAction(IsAction());
	m_btUp.SetAction(IsAction());
	m_btDown.SetAction(IsAction());


	if(m_nNoField != -1)	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_nNoField);
	m_nNoField = -1;

	if(IsAction())
	{
		m_pMyOnlineWorld->pOnlineClient->SendRequestQuestGetBankBookCount();

//		LoadImage();

		m_nNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(292, 155, 292+m_Spr_Frm_List.Header.Xsize, 155+m_Spr_Frm_List.Header.Ysize);
	}
}

//-----------------------------------------------------------------------------
// Name: GetCompletedQuestCount()
// Code: actdoll (2004-01-01)
// Desc: 완료된 퀘스트의 갯수를 받는다.
//-----------------------------------------------------------------------------
SI16	OnlineQuestInterface::GetCompletedQuestCount()
{
	SI16		TempCount;
	SI16		TempQuestCount;

	TempQuestCount = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			TempQuestCount++;
		}
	}

	return TempQuestCount;
}

//-----------------------------------------------------------------------------
// Name: GetCompletedQuestCount()
// Code: actdoll (2004-01-01)
// Desc: 진행중인 퀘스트의 갯수를 받는다.
//-----------------------------------------------------------------------------
SI16 OnlineQuestInterface::GetPlayingQuestCount()
{
	SI16		TempCount;
	SI16		TempQuestCount;

	TempQuestCount = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			TempQuestCount++;
		}
	}

	return TempQuestCount;
}




//===================================================================================================
//	서버에서 응답 수신 시 처리	- START
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: RecvMsg_SetQuestData()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 해당 캐릭터의 퀘스트 진행 정보를 적재한다. 새 캐릭터로 로딩을 했을 때라던지에 떨어진다.
//		PlayingQuestCount		- 진행중인 퀘스트의 갯수
//		CompletedQuestCount		- 종료된 퀘스트의 갯수
//		lpPlayingQuestData		- 진행중인 퀘스트의 상세 정보가 들어있는 포인터
//		lpCompletedQuestData	- 종료된 퀘스트의 인덱스가 들어있는 포인터
//		return					- 무조건 true 리턴
//-----------------------------------------------------------------------------
BOOL	OnlineQuestInterface::RecvMsg_SetQuestData(SI16 PlayingQuestCount, SI16 CompletedQuestCount, SPlayingQuest* lpPlayingQuestData, SI16* lpCompletedQuestData)
{
	SI32	TempCount;
	BOOL	bTempQuestComplete;

	bTempQuestComplete = FALSE;
	ZeroMemory(&m_QuestData, sizeof(SPlayerQuest));

	// 현재 진행하고 있는 퀘스트 리스트를 섭에서 받아와 버퍼에 적재한다.
	CopyMemory(m_QuestData.m_sPlayingQuest, lpPlayingQuestData, sizeof(SPlayingQuest) * PlayingQuestCount);
	for(TempCount = 0; TempCount < PlayingQuestCount; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			m_QuestData.m_sPlayingQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode);
			bTempQuestComplete = TRUE;
		}
	}

	// 현재 완료된 퀘스트 리스트를 섭에서 받아와 버퍼에 적재한다.
	CopyMemory(m_QuestData.m_siCompletedQuest, lpCompletedQuestData, sizeof(SI16) * CompletedQuestCount);
	for(TempCount = 0; TempCount < CompletedQuestCount; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount]);
			bTempQuestComplete = TRUE;
		}
	}

	// 버퍼에 들어있는 퀘스트 정보를 리스트 박스에 표기한다.
	SetQuestList();

	// 퀘스트 리스트가 하나도 없을 경우 각 국가별로 메시지창을 띄워
	// 어디서 시작될지에 대한 메시지를 사용자에게 보여준다.
	if(!bTempQuestComplete)
	{
		UI32	uiCharID	= m_pMyOnlineWorld->pMyData->GetFollowerCharID( 0 );
		// 한국 캐릭터일 경우(조철희, 이명화)
		if		( ( uiCharID == KIND_ON_KEON )		|| ( uiCharID == KIND_ON_MYUNGHWA ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000210));
		// 일본 캐릭터일 경우(시마다 료, 겐노 하나히네)
		else if	( ( uiCharID == KIND_ON_RYO )		|| ( uiCharID == KIND_ON_HANAHINE ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000211));
		// 대만 캐릭터일 경우(왕소룡, 왕미령)
		else if	( ( uiCharID == KIND_ON_SORYUNG )	|| ( uiCharID == KIND_ON_MIRYUNG ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000214));
		// 중국 캐릭터일 경우(당호엽, 노수연)
		else if	( ( uiCharID == KIND_ON_DANG )		|| ( uiCharID == KIND_ON_SUYOUN ) )		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000215));	
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_SetPlayingQuestData()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 진행중인 퀘스트의 정보가 변경됐을 경우 떨어진다.
//		lpPlayingQuestData		- 진행중인 퀘스트의 상세 정보가 들어있는 포인터
//		return					- 무조건 true 리턴
//-----------------------------------------------------------------------------
BOOL	OnlineQuestInterface::RecvMsg_SetPlayingQuestData(SPlayingQuest* lpPlayingQuestData)
{
	SI32		TempCount;

	// 진행중인 퀘스트를 검색해서 일치하는 인덱스가 있다면 새로 떨어진 정보를 적재해준다.
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode == lpPlayingQuestData->m_QuestCode)
		{
			CopyMemory(&m_QuestData.m_sPlayingQuest[TempCount], lpPlayingQuestData, sizeof(SPlayingQuest));
			break;
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_ClearAllQuest()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 현재 들어있는 모든 퀘스트 정보를 날려버린다.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::RecvMsg_ClearAllQuest()
{
	ZeroMemory(m_QuestData.m_siCompletedQuest,       sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	ZeroMemory(m_QuestData.m_siCompletedQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	
	ZeroMemory(m_QuestData.m_sPlayingQuest,       sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
	ZeroMemory(m_QuestData.m_sPlayingQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXPLAYINGQUEST);

	m_LB_QuestList.DelAllSell();
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_QuestStarted()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 수행원을 만나 퀘스트를 시작했을 때의 처리
//		QuestCode	- 시작한 퀘스트의 코드
//		return		- 무조건 true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_QuestStarted(SI16 QuestCode)
{
	SI32		TempCount;

	// 새로 받은 퀘스트 정보를 적재한다.
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode <= 0)
		{
			ZeroMemory(&m_QuestData.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
			m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode = QuestCode;
			m_QuestData.m_sPlayingQuestHeader[TempCount]       = m_pMyOnlineWorld->pQuestParser->GetQuest(QuestCode);
			break;
		}
	}

	// 퀘스트 리스트 갱신
	SetQuestList();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_QuestFinished()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 퀘스트를 취소 혹은 완료 등으로 종료했을 때의 처리
//		QuestCode	- 종료된 퀘스트의 코드
//		return		- 무조건 true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_QuestFinished(SI16 QuestCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			ZeroMemory(&m_QuestData.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
			m_QuestData.m_sPlayingQuestHeader[TempCount]       = NULL;
			break;
		}
	}

	// 퀘스트 리스트 갱신
	SetQuestList();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_AddCompletedQuest()
// Code: actdoll (2004-01-01)
// Desc: 서버에서 정보 수신 - 완료한 퀘스트의 코드를 처리
//		QuestCode	- 완료된 퀘스트의 코드
//		return		- 무조건 true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_AddCompletedQuest(SI16 QuestCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] <= 0)
		{
			m_QuestData.m_siCompletedQuest[TempCount]       = QuestCode;
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(QuestCode);
			break;
		}
	}

	// 퀘스트 리스트 갱신
	SetQuestList();

	return TRUE;
}

//===================================================================================================
//	서버에서 응답 수신 시 처리	- END
//===================================================================================================


