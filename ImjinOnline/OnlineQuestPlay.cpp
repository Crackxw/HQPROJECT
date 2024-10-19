//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 수행원과의 대화창
//	File Name		: OnlineQuestPlay.cpp
//	Birth Date		: 2002. 07. 08.
//	Creator			: 이 윤 석
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		인터페이스에서 수행인에게 퀘스트 관련 정보를 받고 퀘스트의 수락/취소를 하는 창을 띄우는 부분입니다.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineQuestPlay
// Code: All (2004-02-16)
// Desc: 포함 헤더 선언
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineQuestPlay.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include <OnlineText.h>
#include "OnlineFieldArea.h"
#include "OnlineQuest-Parser.h"
#include "OnlineItem.h"
#include "OnlineQuestInterface.h"

//-----------------------------------------------------------------------------
// Name: OnlineQuestPlay
// Code: All (2004-02-16)
// Desc: 정의 및 선언부
//-----------------------------------------------------------------------------
extern _InputDevice		IpD;			// 입력 장치 외부에 선언한 것


//-----------------------------------------------------------------------------
// Name: OnlineQuestPlay()
// Code: All (2004-02-16)
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineQuestPlay::OnlineQuestPlay()
{
	m_pOnlineWorld		= NULL;	
	m_bAction			= FALSE;
	m_siNoFieldHandle	= -1;
	m_bMouseDown		= FALSE;

	m_pSpr_Frm			= NULL;
	ZeroMemory(&m_Spr_Btn, sizeof(XSPR));

	m_QuestCode			= 0;
	m_ChattingType		= 0;
	m_ChattingNowPage	= 0;
	m_ChattingMaxPage	= 0;

	m_ChattingMaxSize	= 0;
	m_ChattingScroll	= 0;

	m_TextClipper		= 0;
	m_TextClipperOld	= 0;

	m_QuestStartBeforePrivQuestNPC = -1;
}

//-----------------------------------------------------------------------------
// Name: ~OnlineQuestPlay()
// Code: All (2004-02-16)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineQuestPlay::~OnlineQuestPlay()
{
	Free();
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: All (2004-02-16)
// Desc: 클래스 초기화. 게임 생성시 한번만 호출한다.
//		pOnlineWorld	- 외부에서 적재될 게임 본체 포인터
//-----------------------------------------------------------------------------
BOOL	OnlineQuestPlay::Init(cltOnlineWorld *pOnlineWorld)
{
	m_pOnlineWorld		= pOnlineWorld;	
	m_bAction			= FALSE;
	m_siNoFieldHandle	= -1;
	m_bMouseDown		= FALSE;

	// 배경 프레임은 외부에서 받고, 버튼은 내부에서 생성한다.
	m_pSpr_Frm = m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_QUESTWINDOW);
	clGrp.LoadXspr( "Online\\VillageBack\\ALLIMAGE_BUTTONEM.Spr", m_Spr_Btn);

	// 버튼 생성
	m_Accept.Create		( 382, 277, m_Spr_Btn.Header.Xsize, m_Spr_Btn.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_ACCEPT), BUTTON_PUT_LEFT, TRUE );
	m_QuestCancel.Create( 382, 277, m_Spr_Btn.Header.Xsize, m_Spr_Btn.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_QUESTCANCEL), BUTTON_PUT_LEFT, TRUE );
	m_Cancel.Create		( 462, 277, m_Spr_Btn.Header.Xsize, m_Spr_Btn.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE );
	m_Next.Create		( 462, 277, m_Spr_Btn.Header.Xsize, m_Spr_Btn.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_NEXT), BUTTON_PUT_LEFT, TRUE );
	m_End.Create		( 462, 277, m_Spr_Btn.Header.Xsize, m_Spr_Btn.Header.Ysize, m_pOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_EXIT), BUTTON_PUT_LEFT, TRUE );

	m_QuestCode       = 0;
	m_ChattingType    = 0;
	m_ChattingNowPage = 0;
	m_ChattingMaxPage = 0;

	m_ChattingMaxSize = 0;
	m_ChattingScroll  = 0;

	m_TextClipper    = CreateRectRgn(249, 155, 525, 271);
	m_TextClipperOld = CreateRectRgn(0, 0, 799, 599);

	m_QuestStartBeforePrivQuestNPC = -1;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: All (2004-02-16)
// Desc: 클래스 제거. 소멸시 한번만 호출된다.
//-----------------------------------------------------------------------------
VOID	OnlineQuestPlay::Free(void)
{
	if(m_TextClipper)
	{
		DeleteObject(m_TextClipper);
		m_TextClipper    = 0;
	}
	if(m_TextClipperOld)
	{
		DeleteObject(m_TextClipperOld);
		m_TextClipperOld = 0;
	}

	m_pSpr_Frm = NULL;
	if(m_Spr_Btn.Image)
	{
		clGrp.FreeXspr(m_Spr_Btn);
		m_Spr_Btn.Image = NULL;
		ZeroMemory(&m_Spr_Btn, sizeof(XSPR));
	}

	if(IsAction())
	{
		SetAction(FALSE, 0, 0);
	}

	m_pOnlineWorld    = NULL;	
	m_bAction         = FALSE;
	m_siNoFieldHandle = -1;
	m_bMouseDown      = FALSE;


	m_QuestCode       = 0;
	m_ChattingType    = 0;
	m_ChattingNowPage = 0;
	m_ChattingMaxPage = 0;

	m_ChattingMaxSize = 0;
	m_ChattingScroll  = 0;

	m_QuestStartBeforePrivQuestNPC = -1;
}

//-----------------------------------------------------------------------------
// Name: Poll()
// Code: All (2004-02-16)
// Desc: 메인 폴. 루프 시 동작 처리.
//-----------------------------------------------------------------------------
VOID OnlineQuestPlay::Poll(void)
{
	if(m_bAction)
	{
		if(m_ChattingNowPage >= (m_ChattingMaxPage - 1))
		{
			switch(m_ChattingType)
			{
				case ON_RET_QUEST_SELECTNPC_STARTQUEST:			// 수행원 선택 시 퀘스트 시작 관련 화면
					if(m_Accept.Process(m_bMouseDown) == TRUE)			// 퀘스트 시작
					{
						m_pOnlineWorld->pOnlineClient->SendRequestQuestAcceptQuest(m_QuestCode);
						SetAction(FALSE, 0, 0);
					}
					if(m_Cancel.Process(m_bMouseDown) == TRUE)			// 화면 끄기
					{
						SetAction(FALSE, 0, 0);
					}
					break;

				case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:		// 수행원 선택 시 퀘스트 진행 중 관련 화면
					if(m_QuestCancel.Process(m_bMouseDown) == TRUE)		// 퀘스트 취소
					{
						m_pOnlineWorld->pOnlineClient->SendRequestQuestCancelQuest(m_QuestCode);
						SetAction(FALSE, 0, 0);
					}
					if(m_Cancel.Process(m_bMouseDown) == TRUE)			// 화면 끄기
					{
						SetAction(FALSE, 0, 0);
					}
					break;

				case ON_RET_QUEST_SELECTNPC_ERROR:				// 수행원 선택 시 퀘스트 시작 전
				case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:	// 수행원 선택 시 퀘스트 시작 전
				case ON_RET_QUEST_SELECTNPC_ENDQUEST:			// 수행원 선택 시 퀘스트 종료
				case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:		// 수행원 선택 시 퀘스트 종료 후
				default:
					if(m_End.Process(m_bMouseDown) == TRUE)				// 화면 끄기
					{
						SetAction(FALSE, 0, 0);
					}
					break;
			}
		}
		else
		{
			if(m_Next.Process(m_bMouseDown) == TRUE)			// 다음 페이지로 넘기기
			{
				m_ChattingNowPage++;
				m_ChattingMaxSize = 0;
				m_ChattingScroll  = 0;
			}
		}

		// Scroll 처리
		if(m_bMouseDown)
		{
			if((IpD.Mouse_X > 522) && (IpD.Mouse_X < 543))
			{
				if((IpD.Mouse_Y > 148) && (IpD.Mouse_Y < 274))
				{
					SI32		TempMaxSize;
					SI32		TempYPos;

					TempMaxSize = m_ChattingMaxSize;
					if(TempMaxSize > 109)
					{
						TempYPos = IpD.Mouse_Y - 154;
						if(TempYPos < 0)   TempYPos = 0;
						if(TempYPos > 109) TempYPos = 109;

						m_ChattingScroll = (SI32)((float)(TempMaxSize - 109) * ((float)TempYPos / 109.0f));
					}
				}
			}
		}
	}

	if(IpD.LeftPressSwitch) m_bMouseDown = TRUE;
	else					m_bMouseDown = FALSE;
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: All (2004-02-16)
// Desc: 메인 출력.
//-----------------------------------------------------------------------------
VOID OnlineQuestPlay::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!m_bAction) return;

	HDC					hDC;
	UI32				TextCode;
	SI32				TempMaxSize;
	SI32				TempYPos;
	XSPR*				lpXSpr;
	SI32				TempCount;
	SQuestHeader*		lpSQuestHeader;

	TempMaxSize = m_ChattingMaxSize;
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		lpXSpr = m_pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SCROLLV);

		// 프레임 출력
		clGrp.PutSpriteT(242, 147, m_pSpr_Frm->Header.Xsize, m_pSpr_Frm->Header.Ysize, m_pSpr_Frm->Image);

		// 버튼부 출력
		if(m_ChattingNowPage >= (m_ChattingMaxPage - 1))
		{
			switch(m_ChattingType)
			{
				case ON_RET_QUEST_SELECTNPC_STARTQUEST:
					m_Accept.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
					m_Cancel.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
					break;

				case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:
					m_QuestCancel.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
					m_Cancel.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
					break;

				case ON_RET_QUEST_SELECTNPC_ERROR:
				case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
				case ON_RET_QUEST_SELECTNPC_ENDQUEST:
				case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:
				case ON_RET_QUEST_SELECTNPC_NOTSAMENTION:
				default:
					m_End.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
					break;
			}
		}
		else
		{
			m_Next.Put(&m_Spr_Btn, 0, 2, 1, BUTTON_PUT_NOMOVE);
		}

		// Scroll 처리
		TempYPos = 0;
		if(TempMaxSize > 109) TempYPos = (SI32)(((float)m_ChattingScroll / (float)(TempMaxSize - 109)) * 109.0f);
		clGrp.PutSpriteT(529, 154 + TempYPos, lpXSpr->Header.Xsize, lpXSpr->Header.Ysize, lpXSpr->Image);

		clGrp.UnlockSurface(pSurface);
	}

	// 텍스트 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		::SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetTextColor(hDC, m_pOnlineWorld->PaletteWhiteColor);

		// Text
		TextCode = GetTextCode();

		if(TextCode)
		{
			// Scroll 처리
			SelectObject(hDC, m_TextClipper);

			switch(m_ChattingType)
			{
				case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
					{
						char			TempTextBuffer[1024];
						char			TempTextBuffer2[1024];
						CItemHeader*	lpCItemHeader;

						lpSQuestHeader = m_pOnlineWorld->pQuestParser->GetQuest(m_QuestCode);

						ZeroMemory(TempTextBuffer,  1024);
						ZeroMemory(TempTextBuffer2, 1024);
						strcpy((char*)TempTextBuffer, m_pOnlineWorld->pOnlineText->GetQuestText(TextCode));
						if(m_QuestStartBeforePrivQuestNPC == -1)
						{
							// Quest가 아닌 다른 능력치가 부족해서 일경우
							switch(TextCode)
							{
								case ON_QUESTTEXT_STARTBEFORETEXT_CREDIT:
									for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; TempCount++)
									{
										if(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalType == QUESTCONDITIONALTYPE_CREDIT)
										{
											sprintf((char*)TempTextBuffer2, (char*)TempTextBuffer, lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity);
											break;
										}
									}
									break;

								case ON_QUESTTEXT_STARTBEFORETEXT_BATTLE:
									for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; TempCount++)
									{
										if(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalType == QUESTCONDITIONALTYPE_BATTLE)
										{
											sprintf((char*)TempTextBuffer2, (char*)TempTextBuffer, lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity);
											break;
										}
									}
									break;

								case ON_QUESTTEXT_STARTBEFORETEXT_LV:
									for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; TempCount++)
									{
										if(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalType == QUESTCONDITIONALTYPE_LEVEL)
										{
											sprintf((char*)TempTextBuffer2, (char*)TempTextBuffer, lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalQuantity);
											break;
										}
									}
									break;

								case ON_QUESTTEXT_STARTBEFORETEXT_ITEM:
									for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; TempCount++)
									{
										if(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalType == QUESTCONDITIONALTYPE_ITEM)
										{
											lpCItemHeader = m_pOnlineWorld->pItemsInfo->GetItem(lpSQuestHeader->sQuestReqConditional[TempCount].m_QuestConditionalCode);										
											sprintf((char*)TempTextBuffer2, (char*)TempTextBuffer, m_pOnlineWorld->pOnlineText->Get(lpCItemHeader->m_siItemName), m_pOnlineWorld->pOnlineText->Get(lpCItemHeader->m_siItemName));
											break;
										}
									}
									break;
							}
						}
						else
						{
							// 이전 Quest를 진행 못한 경우
							NPCIndexHeader*		lpNPC;

							lpNPC = m_pOnlineWorld->pOnlineMap->m_NPCParser.GetQuestNPCAllMap(m_QuestStartBeforePrivQuestNPC);

							sprintf((char*)TempTextBuffer2, (char*)TempTextBuffer, m_pOnlineWorld->pOnlineText->Get(lpNPC->siName));
						}
						if(TempTextBuffer2[0] == 0)
						{
							strcpy((char*)TempTextBuffer2, (char*)TempTextBuffer);
						}
//						m_pOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 254, 159 - m_ChattingScroll, 260, (char*)TempTextBuffer2);
						m_pOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, 254, 159 - m_ChattingScroll, 260, 80, NULL, (char*)TempTextBuffer2, RGB(250, 250, 250) );
					}
					break;

				default:
//					m_pOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 254, 159 - m_ChattingScroll, 260, (char*)m_pOnlineWorld->pOnlineText->GetQuestText(TextCode));
					m_pOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, 254, 159 - m_ChattingScroll, 260, 80, NULL, (char*)m_pOnlineWorld->pOnlineText->GetQuestText(TextCode), RGB(250, 250, 250) );
					break;
			}

			SelectObject(hDC, m_TextClipperOld);
		}

		// Button
		if(m_ChattingNowPage >= (m_ChattingMaxPage - 1))
		{
			switch(m_ChattingType)
			{
				case ON_RET_QUEST_SELECTNPC_STARTQUEST:
					m_Accept.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_Cancel.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					break;

				case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:
					m_QuestCancel.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_Cancel.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					break;

				case ON_RET_QUEST_SELECTNPC_ERROR:
				case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
				case ON_RET_QUEST_SELECTNPC_ENDQUEST:
				case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:
				case ON_RET_QUEST_SELECTNPC_NOTSAMENTION:
				default:
					m_End.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					break;
			}
		}
		else
		{
			m_Next.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}

		// Text Max Size 검사
		m_ChattingMaxSize = GetTextMaxLine(hDC);

		pSurface->ReleaseDC(hDC);
	}
}

//-----------------------------------------------------------------------------
// Name: SetAction()
// Code: All (2004-02-16)
// Desc: 콘솔 신호를 활성화 시키고 화면을 찍어줄 것인가?
//		fAction			- 활성화시 true, 아니라면 false
//		QuestCode		- 주어진 퀘스트 코드
//		ChattingType	- 주어진 대화 형태
//-----------------------------------------------------------------------------
VOID OnlineQuestPlay::SetAction(BOOL fAction, SI16 QuestCode, SI16 ChattingType)
{
	SQuestHeader*		lpSQuestHeader;
	SI32				TempCount;
	SI32*				lpTextList;
	SI32				TempTextBuffer[ON_QUSETDATA_TEXT];

	if(m_bAction == fAction) return;

	if(fAction)
	{
		m_bAction = TRUE;

		// 전지역을 NoField로 지정
		m_siNoFieldHandle = m_pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 599);

		m_QuestCode       = QuestCode;
		m_ChattingType    = ChattingType;
		m_ChattingNowPage = 0;
		m_ChattingMaxPage = 0;
		m_ChattingScroll  = 0;

		// Text
		lpSQuestHeader = m_pOnlineWorld->pQuestParser->GetQuest(m_QuestCode);

		m_QuestStartBeforePrivQuestNPC = -1;

		switch(m_ChattingType)
		{
			case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
				m_QuestStartBeforePrivQuestNPC = GetPrevQuestCompleted(lpSQuestHeader);
				if(m_QuestStartBeforePrivQuestNPC == -1)
				{
					// Quest가 아닌 능력치가 부족해서 진행이 안되는 경우
					lpTextList = (SI32*)lpSQuestHeader->siText_StartBefore;
				}
				else
				{
					// Quest를 진행하지 않아서 진행이 안되는 경우
					ZeroMemory(TempTextBuffer, sizeof(TempTextBuffer));
					TempTextBuffer[0] = ON_QUESTTEXT_STARTBEFORETEXT_NPCQUEST;

					lpTextList = (SI32*)TempTextBuffer;
				}
				break;

			case ON_RET_QUEST_SELECTNPC_STARTQUEST:
				lpTextList = (SI32*)lpSQuestHeader->siText_Start;
				break;

			case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:
				lpTextList = (SI32*)lpSQuestHeader->siText_Playing;
				break;

			case ON_RET_QUEST_SELECTNPC_ENDQUEST:
				lpTextList = (SI32*)lpSQuestHeader->siText_End;
				break;

			case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:
				lpTextList = (SI32*)lpSQuestHeader->siText_EndAfter;
				break;

			case ON_RET_QUEST_SELECTNPC_NOTSAMENTION:
				m_ChattingMaxPage = 1;
				goto GOTOEND;
				break;

			case ON_RET_QUEST_SELECTNPC_ERROR:
				m_ChattingMaxPage = 1;
				goto GOTOEND;
				break;

			default:
				SetAction(FALSE, 0, 0);
				return;
				break;
		}
		for(TempCount = 0; TempCount < ON_QUSETDATA_TEXT; TempCount++)
		{
			if(lpTextList[TempCount])
			{
				m_ChattingMaxPage = TempCount + 1;
			}
		}
GOTOEND:;
	}
	else
	{
		m_bAction = FALSE;

		if(m_siNoFieldHandle != -1) m_pOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldHandle);
		m_siNoFieldHandle = -1;

		m_QuestCode       = 0;
		m_ChattingType    = 0;
		m_ChattingNowPage = 0;
		m_ChattingMaxPage = 0;

		m_ChattingScroll = 0;

		m_QuestStartBeforePrivQuestNPC = -1;
	}
}

//-----------------------------------------------------------------------------
// Name: GetTextCode()
// Code: All (2004-02-16)
// Desc: 현재 출력해야 할 텍스트 코드를 받는다.
//		return	- 텍스트 코드 번호
//-----------------------------------------------------------------------------
UI32 OnlineQuestPlay::GetTextCode(void)
{
	UI32				TextCode;
	SQuestHeader*		lpSQuestHeader;

	TextCode = 0;

	lpSQuestHeader = m_pOnlineWorld->pQuestParser->GetQuest(m_QuestCode);
	switch(m_ChattingType)
	{
		case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
			if(m_QuestStartBeforePrivQuestNPC == -1)
			{
				// Quest가 아닌 능력치가 부족해서 진행이 안되는 경우
				TextCode = lpSQuestHeader->siText_StartBefore[m_ChattingNowPage];
			}
			else
			{
				// Quest를 진행하지 않아서 진행이 안되는 경우
				TextCode = ON_QUESTTEXT_STARTBEFORETEXT_NPCQUEST;
			}
			break;

		case ON_RET_QUEST_SELECTNPC_STARTQUEST:
			TextCode = lpSQuestHeader->siText_Start[m_ChattingNowPage];
			break;

		case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:
			TextCode = lpSQuestHeader->siText_Playing[m_ChattingNowPage];
			break;

		case ON_RET_QUEST_SELECTNPC_ENDQUEST:
			TextCode = lpSQuestHeader->siText_End[m_ChattingNowPage];
			break;

		case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:
			TextCode = lpSQuestHeader->siText_EndAfter[m_ChattingNowPage];
			break;

		case ON_RET_QUEST_SELECTNPC_NOTSAMENTION:
			TextCode = ON_QUESTTEXT_NOTSAMENATION;
			break;

		case ON_RET_QUEST_SELECTNPC_ERROR:
		default:
			TextCode = 7;
			break;
	}

	return TextCode;
}

//-----------------------------------------------------------------------------
// Name: GetTextMaxLine()
// Code: All (2004-02-16)
// Desc: 현재 출력하는 텍스트의 최대 줄 수를 얻는다.
//		return	- 최대 줄 수
//-----------------------------------------------------------------------------
SI32 OnlineQuestPlay::GetTextMaxLine(HDC hDC)
{
	UI32		TempText;

	TempText = GetTextCode();
	return m_pOnlineWorld->pOnlineMegaText->CheckTextLine2(hDC, 254, 159, 260, (char*)m_pOnlineWorld->pOnlineText->GetQuestText(TempText));
}

//-----------------------------------------------------------------------------
// Name: GetPrevQuestCompleted()
// Code: All (2004-02-16)
// Desc: 이전에 이 Quest를 한적이 있는지 검사. 이전에 한적이 없으면 그 Quest의 NPC Code를 넘겨준다.
//		lpQuest	- 해당 퀘스트 정보가 들어있는 헤더
//		return	- 최대 줄 수
//-----------------------------------------------------------------------------
SI32 OnlineQuestPlay::GetPrevQuestCompleted(SQuestHeader* lpQuest)
{
	SI32			QuestReqCount;
	SI32			CompletedQuestReqCount;
	BOOL			fOK;
	SPlayerQuest*	psMyQuestData  = m_pOnlineWorld->m_pQuestInterface->GetQuestData();
	if( !psMyQuestData )		return -1;

	for(QuestReqCount = 0; QuestReqCount < ON_QUSETDATA_MAXQUESTREQCONDITIONAL; QuestReqCount++)
	{
		if(lpQuest->sQuestReqConditional[QuestReqCount].m_QuestConditionalType == QUESTCONDITIONALTYPE_COMPLETIONQUEST)
		{
			fOK = FALSE;
			for(CompletedQuestReqCount = 0; CompletedQuestReqCount < ON_QUSETDATA_MAXCOMLETEDQUEST; CompletedQuestReqCount++)
			{
				if(psMyQuestData->m_siCompletedQuest[CompletedQuestReqCount] == lpQuest->sQuestReqConditional[QuestReqCount].m_QuestConditionalQuantity)
				{
					fOK = TRUE;
				}
			}

			if(fOK == FALSE)
			{
				SQuestHeader*	lpFindQuest;

				lpFindQuest = m_pOnlineWorld->pQuestParser->GetQuest(lpQuest->sQuestReqConditional[QuestReqCount].m_QuestConditionalQuantity);

				if(lpFindQuest == NULL) return -1;

				return lpFindQuest->siNPCCode;
			}
		}
	}

	return -1;
}
