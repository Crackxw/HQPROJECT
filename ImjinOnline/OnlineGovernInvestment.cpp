#include <GSL.h>

#include <clGame.h>
#include "OnlineWorld.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineVillage.h"
#include "OnlineNumberControl.h"
#include "OnlineMessenger.h"
#include "OnlineGovernMgr.h"


extern	_clGame* pGame;


VOID OnlineGovernMgr::InitInvestment()
{
	m_BInvest.SetAction(FALSE);

	// 변수 초기화
	m_InvestData.m_TotInvestMoney = 0;
	m_InvestData.m_TotCapital = 0;
	m_InvestData.m_siAllInInvestmentPercent = 0;	// 폭투 권한 여부 및 %

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_invest.spr", m_InvestData.m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_InvestData.m_InvestButImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_invest_scrbar.spr", m_InvestData.m_ScrBackImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_handle.Spr",m_InvestData.m_ScrBarImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Up.Spr",m_InvestData.m_UpImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Down.Spr",m_InvestData.m_DownImage);

	// 관청대사 결정
	m_InvestData.m_siTalk = rand()%2;

	// 각 버튼 생성
	m_InvestData.m_BGetOut.Create(m_siX+329, m_siY+112, m_InvestData.m_InvestButImage.Header.Xsize, m_InvestData.m_InvestButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT), BUTTON_PUT_LEFT, FALSE);
	m_InvestData.m_BInvest.Create(m_siX+427, m_siY+112, m_InvestData.m_InvestButImage.Header.Xsize, m_InvestData.m_InvestButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT), BUTTON_PUT_LEFT, FALSE);
	m_InvestData.m_BYearUp.Create(m_siX+99, m_siY+194, m_InvestData.m_UpImage.Header.Xsize, m_InvestData.m_UpImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_InvestData.m_BYearDown.Create(m_siX+99, m_siY+204, m_InvestData.m_DownImage.Header.Xsize, m_InvestData.m_DownImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_InvestData.m_BScrollUp.Create(m_siX+508, m_siY+175, m_InvestData.m_UpImage.Header.Xsize, m_InvestData.m_UpImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_InvestData.m_BScrollDown.Create(m_siX+508, m_siY+371, m_InvestData.m_DownImage.Header.Xsize, m_InvestData.m_DownImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	// 스크롤
	SetRect(&m_InvestData.m_rtScroll, m_siX + 508, m_siY + 184, m_siX + 508 + 9, m_siY + 184 + 187);
	m_InvestData.m_siScrollPos = m_siY + 184;

	// 서버에게 투자 정보를 요청한다
	pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(0);
	pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetVillageProfitStatistics();

	// 폭탄 투자 권한이 있는지 요청한다.
	pMyOnlineWorld->pOnlineClient->SendRequest_village_investment_power(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode, pMyOnlineWorld->pMyData->GetMyCharName());
}

VOID OnlineGovernMgr::FreeInvestment()
{
	clGrp.FreeXspr(m_InvestData.m_MainImage);
	clGrp.FreeXspr(m_InvestData.m_InvestButImage);
	clGrp.FreeXspr(m_InvestData.m_ScrBackImage);
	clGrp.FreeXspr(m_InvestData.m_ScrBarImage);
	clGrp.FreeXspr(m_InvestData.m_UpImage);
	clGrp.FreeXspr(m_InvestData.m_DownImage);




	if(m_InvestData.m_pCapitalList)
	{
		delete [] m_InvestData.m_pCapitalList;
		m_InvestData.m_pCapitalList = NULL;
	}
	
	if(m_InvestData.m_pInvestList)
	{
		delete [] m_InvestData.m_pInvestList;
		m_InvestData.m_pInvestList = NULL;
	}

	m_BInvest.SetAction(TRUE);
	m_InvestData.m_uiStatus = ON_SUB_INVEST_INIT;
}

UI08 OnlineGovernMgr::PollInvestment(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI16 i;
	switch(m_InvestData.m_uiStatus)
	{
	case ON_SUB_INVEST_INIT:
		{
			InitInvestment();
			m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
		}
		break;
	case ON_SUB_INVEST_POLL:
		{
			// 메인메뉴 전환
			if(m_BVillageInfo.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_VILLAGEINFO;
				m_InvestData.m_uiStatus = ON_SUB_INVEST_EXIT;
			}
			else if(m_BGuild.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_GUILD;
				m_InvestData.m_uiStatus = ON_SUB_INVEST_EXIT;
			}
			else if(m_BExit.Process(m_bMouseDown))
			{
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Main_Leave();
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_EXIT_WAIT;
				m_InvestData.m_uiStatus = ON_SUB_INVEST_EXIT;
			}

			// 년도버튼
			if(m_InvestData.m_pCapitalList )
			{
				if(m_InvestData.m_BYearUp.Process(m_bMouseDown))
				{
					m_InvestData.m_siCurCapital--;
					if(m_InvestData.m_siCurCapital < 0)	m_InvestData.m_siCurCapital = 0;

					m_InvestData.m_TotCapital = 0;
					for(i=0; i<7; i++)
						m_InvestData.m_TotCapital += m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].Profit[i];
				}
				else if(m_InvestData.m_BYearDown.Process(m_bMouseDown))
				{
					m_InvestData.m_siCurCapital++;
					if(m_InvestData.m_siCurCapital >= m_InvestData.m_siCapitalNum)	m_InvestData.m_siCurCapital = m_InvestData.m_siCapitalNum - 1;

					m_InvestData.m_TotCapital = 0;
					for(i=0; i<7; i++)
						m_InvestData.m_TotCapital += m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].Profit[i];
				}
			}

			// 투자금 인출, 투자
			if(m_InvestData.m_BGetOut.Process(m_bMouseDown))
			{
				if(pMyOnlineWorld->pOnlineMessenger->IsMini() || !pMyOnlineWorld->pOnlineMessenger->IsAction())
				{
					if(m_InvestData.m_MyInvestMoney > 0)
					{
						// robypark 2004/11/26 10:37
						// 공성시간 중에는 어느 누구도 마을에 투자금을 인출할 수 없다. 운영자 권한5는 제외
						if ((pMyOnlineWorld->IsInTimeSiegeWarfare())
							&& (pMyOnlineWorld->m_GMFlag != ON_USERGRADE_ADMIN5))
						{
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
																pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), 
																pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_WITHDRAWAL_TIEM_ERROR), 
																pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_BEGIN, 
																pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_END);
						}
						else
						{
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_InvestData.m_MyInvestMoney);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
							m_InvestData.m_BGetOut.SetAction(FALSE);
							m_InvestData.m_BInvest.SetAction(FALSE);
							m_InvestData.m_uiStatus = ON_SUB_INVEST_GETOUT;
						}
					}
				}
				else
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "귓말창을 닫고 실행해 주십시오.");
			}
			else if(m_InvestData.m_BInvest.Process(m_bMouseDown))
			{
				// robypark 2004/11/26 10:37
				// 공성시간 중에는 어느 누구도 마을에 투자할 수 없다. 운영자 권한5는 제외
				if ((pMyOnlineWorld->IsInTimeSiegeWarfare())
					&& (pMyOnlineWorld->m_GMFlag != ON_USERGRADE_ADMIN5))
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
													pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), 
													pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_INVESTMENT_TIME_ERROR), 
													pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_BEGIN, 
													pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_END);
				}
				else
				{
					// 사용자에게 폭투 권한이 있다면
					if (m_InvestData.m_siAllInInvestmentPercent)
					{
						// 폭투 권한 사용할 때에는 소지금과 전장에 있는 돈 모두를 이용한다.
						MONEY MaxMoney = min(m_InvestData.m_TotInvestMoney*m_InvestData.m_siAllInInvestmentPercent/*폭투 %*//100, pMyOnlineWorld->pMyData->GetMoney() + pMyOnlineWorld->pMyData->GetSaving());
						MONEY MinMoney = m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMinPersent/100;

						pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment(MaxMoney, MinMoney, m_InvestData.m_TotInvestMoney, 0);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
					
						m_InvestData.m_BGetOut.SetAction(FALSE);
						m_InvestData.m_BInvest.SetAction(FALSE);
						m_InvestData.m_uiStatus = ON_SUB_INVEST_SETINVEST_ALLIN;
					}
					else // 폭투권한이 없다면
					{
						// 한달에 한번만 투자 할수있다. 현재시간과 투자할수있는 시간 비교
						if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
						{
							MONEY MaxMoney = min(m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMaxPersent/100, pMyOnlineWorld->pMyData->GetMoney());
							MONEY MinMoney = m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMinPersent/100;

							pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment(MaxMoney, MinMoney, m_InvestData.m_TotInvestMoney, 0);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
					
							m_InvestData.m_BGetOut.SetAction(FALSE);
							m_InvestData.m_BInvest.SetAction(FALSE);
							m_InvestData.m_uiStatus = ON_SUB_INVEST_SETINVEST;
						}
						else
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENTONEMONTH));
					}
				}
			}

			// 스크롤바 동작
			if(m_bMouseDown)
				ActionInvestScroll();
		}
		break;
	case ON_SUB_INVEST_WAIT:
		{
			// 응답을 기다리고 있다는 메시지 박스
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		}
		break;
	case ON_SUB_INVEST_SETINVEST:
		{
			if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				MONEY MaxMoney = min(m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMaxPersent/100, pMyOnlineWorld->pMyData->GetMoney());
				MONEY MinMoney = m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMinPersent/100;

				if(m_InvestData.m_MyInvestMoney == 0)
				{
					// 투자가능한 최소퍼센트, 최대퍼센트 금액이 있다.
					if(pMyOnlineWorld->pOnlineNumPad->siCount < MinMoney || pMyOnlineWorld->pOnlineNumPad->siCount > MaxMoney)
					{
						if(m_InvestData.m_MyInvestMoney)	m_InvestData.m_BGetOut.SetAction(TRUE);
						m_InvestData.m_BInvest.SetAction(TRUE);
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIRSTINVESTMENT), m_InvestData.m_siMyInvestMinPersent, m_InvestData.m_siMyInvestMaxPersent);

						pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
						m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
						break;
					}
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pOnlineNumPad->siCount);
				}
				else
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pOnlineNumPad->siCount);

				m_InvestData.m_InvestAfterMoney = pMyOnlineWorld->pMyData->GetMoney() - pMyOnlineWorld->pOnlineNumPad->siCount;
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_WAIT;
			}
			else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
			{
				if(m_InvestData.m_MyInvestMoney)	m_InvestData.m_BGetOut.SetAction(TRUE);
				m_InvestData.m_BInvest.SetAction(TRUE);
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
			}
		}
		break;
	// 폭탄 투자 처리
	case ON_SUB_INVEST_SETINVEST_ALLIN:
		{
			if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				// 폭투 권한 사용할 때에는 소지금과 전장에 있는 돈 모두를 이용한다.
				MONEY MaxMoney = min(m_InvestData.m_TotInvestMoney*m_InvestData.m_siAllInInvestmentPercent/*폭투 %*//100, pMyOnlineWorld->pMyData->GetMoney() + pMyOnlineWorld->pMyData->GetSaving());
				MONEY MinMoney = m_InvestData.m_TotInvestMoney*m_InvestData.m_siMyInvestMinPersent/100;

				// 이 마을에 처음 투자하는 경우
				if(m_InvestData.m_MyInvestMoney == 0)
				{
					// 투자가능한 최소퍼센트, 최대퍼센트 금액이 있다.
					if(pMyOnlineWorld->pOnlineNumPad->siCount < MinMoney || pMyOnlineWorld->pOnlineNumPad->siCount > MaxMoney)
					{
						if(m_InvestData.m_MyInvestMoney)	m_InvestData.m_BGetOut.SetAction(TRUE);
						m_InvestData.m_BInvest.SetAction(TRUE);
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIRSTINVESTMENT), m_InvestData.m_siMyInvestMinPersent, m_InvestData.m_siMyInvestMaxPersent);

						pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
						m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
						break;
					}
					else
					{
						// 소지금을 우선적으로 투자금으로 사용한다.
						if (pMyOnlineWorld->pMyData->GetMoney() > pMyOnlineWorld->pOnlineNumPad->siCount)
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pOnlineNumPad->siCount);
						else	// 소지금 + 전장 잔고
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pMyData->GetMoney(), pMyOnlineWorld->pOnlineNumPad->siCount - pMyOnlineWorld->pMyData->GetMoney());
					}
				}
				else
				{
					// 소지금을 우선적으로 투자금으로 사용한다.
					if (pMyOnlineWorld->pMyData->GetMoney() > pMyOnlineWorld->pOnlineNumPad->siCount)
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pOnlineNumPad->siCount);
					else	// 소지금 + 전장 잔고
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(0, pMyOnlineWorld->pMyData->GetMoney(), pMyOnlineWorld->pOnlineNumPad->siCount - pMyOnlineWorld->pMyData->GetMoney());
				}

				// 투자 후에 변경되는 소지금과 은행 잔고 저장
				// 소지금을 우선적으로 투자금으로 사용한다.
				if (pMyOnlineWorld->pMyData->GetMoney() > pMyOnlineWorld->pOnlineNumPad->siCount)
					m_InvestData.m_InvestAfterMoney = pMyOnlineWorld->pMyData->GetMoney() - pMyOnlineWorld->pOnlineNumPad->siCount;
				else
				{
					m_InvestData.m_InvestAfterMoney_InBank = pMyOnlineWorld->pMyData->GetSaving() - (pMyOnlineWorld->pOnlineNumPad->siCount - pMyOnlineWorld->pMyData->GetMoney());
					m_InvestData.m_InvestAfterMoney = 0;
				}

				pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_WAIT;
			}
			else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
			{
				if(m_InvestData.m_MyInvestMoney)
					m_InvestData.m_BGetOut.SetAction(TRUE);

				m_InvestData.m_BInvest.SetAction(TRUE);

				pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
			}
		}
		break;
	case ON_SUB_INVEST_GETOUT:
		{
			if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				MONEY MaxMoney = m_InvestData.m_MyInvestMoney;
				MONEY MinMoney = 10;

				if(pMyOnlineWorld->pOnlineNumPad->siCount > MaxMoney)
				{
					m_InvestData.m_BGetOut.SetAction(TRUE);
					if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
						m_InvestData.m_BInvest.SetAction(TRUE);
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000480));

					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
					break;
				}
				else if(pMyOnlineWorld->pOnlineNumPad->siCount < MinMoney)
				{
					m_InvestData.m_BGetOut.SetAction(TRUE);
					if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
						m_InvestData.m_BInvest.SetAction(TRUE);
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000481));

					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
					break;
				}
				else
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_InvestmentGetOut(pMyOnlineWorld->pOnlineNumPad->siCount);
				
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_WAIT;
			}
			else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
			{
				m_InvestData.m_BGetOut.SetAction(TRUE);
				if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
					m_InvestData.m_BInvest.SetAction(TRUE);
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;
			}
		}
		break;
	case ON_SUB_INVEST_EXIT:
		{
			DrawInvestment(pSurface);
			FreeInvestment();
			return m_uiReturnStatus;
		}
		break;
	}

	DrawInvestment(pSurface);
	
	return ON_GOVERN_MAIN_STATUS_INVESTMENT;
}

VOID OnlineGovernMgr::DrawInvestment(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI16 i;
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_InvestData.m_MainImage.Header.Xsize, m_InvestData.m_MainImage.Header.Ysize, m_InvestData.m_MainImage.Image);

		// 스크롤
		if(m_InvestData.m_siInvestNum > INVESTOR_MAX_LINE)
		{
			clGrp.PutSpriteT(m_siX + 504, m_siY + 176, m_InvestData.m_ScrBackImage.Header.Xsize, m_InvestData.m_ScrBackImage.Header.Ysize, m_InvestData.m_ScrBackImage.Image );
			clGrp.PutSpriteT(m_siX + 508, m_InvestData.m_siScrollPos, m_InvestData.m_ScrBarImage.Header.Xsize, m_InvestData.m_ScrBarImage.Header.Ysize, m_InvestData.m_ScrBarImage.Image );
			m_InvestData.m_BScrollUp.Put(&m_InvestData.m_UpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
			m_InvestData.m_BScrollDown.Put(&m_InvestData.m_DownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		}

		// 각 버튼들
		m_InvestData.m_BGetOut.Put(&m_InvestData.m_InvestButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_InvestData.m_BInvest.Put(&m_InvestData.m_InvestButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_InvestData.m_BYearUp.Put(&m_InvestData.m_UpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		m_InvestData.m_BYearDown.Put(&m_InvestData.m_DownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	HDC	hdc;
	CHAR szTemp[256];
	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 관청대사
		if(m_InvestData.m_siTalk == 0)
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hdc, m_siX + 112, m_siY + 74, 160, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_ISGUILDINVESTMENTHELP), RGB(200, 200, 200));
		else if(m_InvestData.m_siTalk == 1)
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hdc, m_siX + 112, m_siY + 74, 160, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_NONEGUILDINVESTMENTHELP), RGB(200, 200, 200));

		// 투자한 금액
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 318, m_siY + 73, 72, 13, pMyOnlineWorld->pOnlineText->Get(8000482), RGB(200, 200, 200));
		MakeMoneyText(m_InvestData.m_MyInvestMoney, (char*)szTemp);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 395, m_siY + 71, 123, 17, szTemp, RGB(200, 200, 200));

		// 총 투자금
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 162, 72, 13, pMyOnlineWorld->pOnlineText->Get(8000483), RGB(200, 200, 200));
		MakeMoneyText(m_InvestData.m_TotInvestMoney, (char*)szTemp);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 111, m_siY + 160, 123, 17, szTemp, RGB(200, 200, 200));

		// 년도
		if(m_InvestData.m_pCapitalList)
		{
			sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEAR), m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].Year);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 33, m_siY + 195, 62, 17, szTemp, RGB(200, 200, 200));
		}

		// 각시설별 수익
		for(i=0; i<7; i++)
		{
			if(!m_InvestData.m_pCapitalList)		continue;

			switch(i)
			{
			case 0:		// 교역수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVEST_TRADE_PROFIT), RGB(200, 200, 200));
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].TradeProfit, (char*)szTemp);		break;
			case 1:		// 전장수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVEST_BANK_PROFIT), RGB(200, 200, 200));
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].BankProfit, (char*)szTemp);		break;
			case 2:		// 용병수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVEST_FOLLOWER_PROFIT), RGB(200, 200, 200));	
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].EmployProfit, (char*)szTemp);	break;
			case 3:		// 치료수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVEST_HEAL_PROFIT), RGB(200, 200, 200));
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].CureProfit, (char*)szTemp);		break;
			case 4:		// 부두수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVEST_WHARF_PROFIT), RGB(200, 200, 200));
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].TicketProfit, (char*)szTemp);	break;
			case 5:		// 주막수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(8000484), RGB(200, 200, 200));	
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].InnProfit, (char*)szTemp);		break;
			case 6:		// 생산시설수익
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 222 + 20*i, 72, 12, pMyOnlineWorld->pOnlineText->Get(8000485), RGB(200, 200, 200));
				MakeMoneyText(m_InvestData.m_pCapitalList[m_InvestData.m_siCurCapital].PlantProfit, (char*)szTemp);		break;
			}
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 111, m_siY + 221 + 20*i, 123, 14, szTemp, RGB(200, 200, 200));
		}

		// 총 수익
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 34, m_siY + 363, 72, 14, pMyOnlineWorld->pOnlineText->Get(8000486), RGB(200, 200, 200));
		MakeMoneyText(m_InvestData.m_TotCapital, (char*)szTemp);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 111, m_siY + 361, 123, 17, szTemp, RGB(200, 200, 200));

		// 투자자 목록
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 267, m_siY + 161, 72, 12, pMyOnlineWorld->pOnlineText->Get(8000487), RGB(200, 200, 200));
		DrawInvestorList(hdc);

		// 각 버튼들
		m_InvestData.m_BGetOut.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_InvestData.m_BInvest.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hdc);
	}	
}

VOID OnlineGovernMgr::DrawInvestorList(HDC hdc)
{
	if(!m_InvestData.m_pInvestList)		return;

	SI16 i, index;
	SI32 siInvestTime;
	CHAR szTemp1[256], szTemp2[256];
	for(i=0; i<min(m_InvestData.m_siInvestNum, INVESTOR_MAX_LINE); i++)
	{
		index = i + m_InvestData.m_siScroll;

		// 투자자이름
		sprintf((char*)szTemp1, "%2d. %s", index+1, m_InvestData.m_pInvestList[index].m_InvestmentName);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 267, m_siY + 190 + 36*i, szTemp1, RGB(200, 200, 200));

		// 투자한날짜
		siInvestTime = m_InvestData.m_pInvestList[index].m_InvestmentUpdateTime;
		switch(pGame->LanguageCode)
		{
		case ELANGAGECODE_KOREAN:	case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_JAPAN:	case ELANGAGECODE_HONGKONG:	case ELANGAGECODE_CHINA:
			sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (siInvestTime / 1000000), (siInvestTime / 10000) % 100, (siInvestTime / 100) % 100);
			break;
		case ELANGAGECODE_INDONESIA:
			sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (siInvestTime / 10000) % 100, (siInvestTime / 100) % 100, (siInvestTime / 1000000));
			break;
		}
		pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX + 495, m_siY + 190 + 36*i, szTemp1, RGB(200, 200, 200));

		// 점유율
		MakeMoneyText(m_InvestData.m_pInvestList[index].m_InvestmentMoney, (char*)szTemp1);
		sprintf((char*)szTemp2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_POSSETION), (SI32)m_InvestData.m_pInvestList[index].m_InvestmentPercent, szTemp1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 285, m_siY + 190 + 18 + 36*i, szTemp2, RGB(200, 200, 200));
	}
}

VOID OnlineGovernMgr::ActionInvestScroll()
{
	SI16 siHeight, siListCount;

	siListCount = m_InvestData.m_siInvestNum;

	// 스크롤 처리
	if(siListCount > INVESTOR_MAX_LINE)
	{
		if(m_InvestData.m_BScrollUp.Process(m_bMouseDown))
		{
			m_InvestData.m_siScroll--;
			if(m_InvestData.m_siScroll < 0)		m_InvestData.m_siScroll = 0;
			
			siHeight = (SI16)(m_InvestData.m_rtScroll.bottom - m_InvestData.m_rtScroll.top - m_InvestData.m_ScrBarImage.Header.Ysize);
			m_InvestData.m_siScrollPos = m_siY + 184 + (SI16)(((float)siHeight / (float)(siListCount - INVESTOR_MAX_LINE)) * (float)m_InvestData.m_siScroll);
		}		
		else if(m_InvestData.m_BScrollDown.Process(m_bMouseDown))
		{
			m_InvestData.m_siScroll++;
			if(m_InvestData.m_siScroll > (siListCount - INVESTOR_MAX_LINE))	m_InvestData.m_siScroll = siListCount - INVESTOR_MAX_LINE;

			siHeight		=	(SI16)((m_InvestData.m_rtScroll.bottom - m_InvestData.m_rtScroll.top - m_InvestData.m_ScrBarImage.Header.Ysize));
			m_InvestData.m_siScrollPos = m_siY + 184 + (SI16)(((float)siHeight / (float)(siListCount - INVESTOR_MAX_LINE))  * (float)m_InvestData.m_siScroll);
		}
		else if(PtInRect(&m_InvestData.m_rtScroll, ptMouse) == TRUE)
		{
			siHeight		=	(SI16)(m_InvestData.m_rtScroll.bottom - m_InvestData.m_rtScroll.top - m_InvestData.m_ScrBarImage.Header.Ysize);
			m_InvestData.m_siScrollPos = ptMouse.y;
			if(m_InvestData.m_siScrollPos > (m_InvestData.m_rtScroll.bottom - m_InvestData.m_ScrBarImage.Header.Ysize))
				m_InvestData.m_siScrollPos = m_InvestData.m_rtScroll.bottom - m_InvestData.m_ScrBarImage.Header.Ysize;

			m_InvestData.m_siScroll = max(0, (m_InvestData.m_siScrollPos - (m_siY + 184)) / ((float)siHeight / (float)(siListCount - INVESTOR_MAX_LINE)));
		}
	}
}
