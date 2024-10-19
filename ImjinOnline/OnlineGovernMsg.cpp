#include <GSL.h>

#include <clGame.h>
#include "OnlineWorld.h"
#include "OnlineVillage.h" 
#include "OnlineTradeParser.h"
#include "OnlineNumberControl.h"
#include "OnlineGovernMgr.h"
 
extern	_clGame* pGame;

// GovernMgr에서 쓰임
UI16 OnlineGovernMgr::GovernEnter(BYTE *pMsg)
{
	m_uiMainStatus = ON_GOVERN_MAIN_STATUS_VILLAGEINFO;
	return 0;
}

UI16 OnlineGovernMgr::GovernLeave(BYTE *pMsg)
{
	m_uiMainStatus = ON_GOVERN_MAIN_STATUS_EXIT;
	return 0;
}

// GovernVillageInfo에서 쓰임
// 마을 정보를 서버로부터 받는다.
UI16 OnlineGovernMgr::GovernVillageInfo(BYTE *pMsg)
{
	OnResGovernmentVillageInfo* lpOnResGovernmentVillageInfo = (OnResGovernmentVillageInfo*)pMsg;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	m_VillageData.m_pEditBox->SetString( lpOnResGovernmentVillageInfo->m_VillageInfo_Descript );
//	::SetFocus(pGame->Hwnd);
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_VillageData.m_hOecNotice );
	if( !pOecEx )		return 0;
	pOecEx->SetString( lpOnResGovernmentVillageInfo->m_VillageInfo_Descript );
	Handler_OnlineEditControlEx::ClearCurrentFocus();

	m_VillageData.m_siInOutPlayer = lpOnResGovernmentVillageInfo->m_VillageInfo_InOutPlayerCount;
	m_VillageData.m_CurDefPower = lpOnResGovernmentVillageInfo->CurDefencePower;
	m_VillageData.m_MaxDefPower = lpOnResGovernmentVillageInfo->MaxDefencePower;

	m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;

	return 0;
}

// 마을안내문 작성후 응답
UI16 OnlineGovernMgr::GovernDescript(BYTE *pMsg)
{
	OnResWriteDescript *pWriteDescript = (OnResWriteDescript*)pMsg;

	if(pWriteDescript->dwResponse == ON_RET_DESCRIPT_OK)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONNOTIFY));
	else if(pWriteDescript->dwResponse == ON_RET_DESCRIPT_NO)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONNOTIFYNO));
	else if(pWriteDescript->dwResponse == ON_RET_DESCRIPT_NOTBEST)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BESTINVESTMENTNO));

	m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;

	return 0;
}

UI16 OnlineGovernMgr::GovernVillageDefenceInfo(BYTE *pMsg)
{
	OnResMaxVillageDefenceInvestment* lpMaxVillageDefence = (OnResMaxVillageDefenceInvestment*)pMsg;

	if(lpMaxVillageDefence->dwResponse == ON_RET_OK)
	{
		m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_SETDEFENCE;

		m_VillageData.m_CurDefPower = lpMaxVillageDefence->NowDef;
		m_VillageData.m_MaxDefPower = lpMaxVillageDefence->MaxDef;
		m_VillageData.m_MoneyPerDef = lpMaxVillageDefence->DefPerMoney;
		m_VillageData.m_MaxMoney = lpMaxVillageDefence->MaxMoney;
		
		if(m_VillageData.m_CurDefPower > m_VillageData.m_MaxDefPower)
			m_VillageData.m_MaxDefPower = m_VillageData.m_CurDefPower;
	
		MONEY MaxMoney = min((m_VillageData.m_MaxDefPower-m_VillageData.m_CurDefPower)*m_VillageData.m_MoneyPerDef, m_VillageData.m_MaxMoney);
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(min(pMyOnlineWorld->pMyData->GetMoney(), MaxMoney));
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
	}
	else
	{
		m_VillageData.m_BDefence.SetAction(TRUE);
		m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONFAIL));
	}

	return 0;
}

UI16 OnlineGovernMgr::GovernVillageDefenceSet(BYTE *pMsg)
{
	OnResSetVillageDefenceInvestment* lpSetVillageDefence = (OnResSetVillageDefenceInvestment*)pMsg;

	m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;
	m_VillageData.m_BDefence.SetAction(TRUE);

	if(lpSetVillageDefence->dwResponse == ON_RET_OK)
	{
		m_VillageData.m_CurDefPower = lpSetVillageDefence->NowDef;
		// Village 정보를 찾아서 수정
		SVillageData *lpSVillageData  = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		if(!lpSVillageData)		return 0;

		lpSVillageData->m_vdChangedDefence = lpSetVillageDefence->NowDef;
		
		if(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode)
		{
			if(pMyOnlineWorld->pOnlineVillageInfoData->m_lpVillageHeader->uiVillageCode == pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
				pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.vdVillageDefence = lpSVillageData->m_vdChangedDefence;
		}
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENTFAIL));

	return 0;
}

// GovernInvestment에서 쓰임
UI16 OnlineGovernMgr::GovernInvestProfitInfo(BYTE *pMsg)
{
	OnResProfitStatistics *lpOnResProfitStatistics = (OnResProfitStatistics*)pMsg;
	
	BYTE *TempBuffer = (BYTE*)((BYTE*)lpOnResProfitStatistics + sizeof(OnResProfitStatistics));
	
	if(m_InvestData.m_pCapitalList) 
	{
		delete[] m_InvestData.m_pCapitalList;
		m_InvestData.m_pCapitalList = NULL;
	}

	if(lpOnResProfitStatistics->siNum > 0)
	{
		m_InvestData.m_pCapitalList = new stCapital[lpOnResProfitStatistics->siNum];
		memcpy(m_InvestData.m_pCapitalList, TempBuffer, sizeof(stCapital) * lpOnResProfitStatistics->siNum);
		
		// 총 수익
		for(int i=0; i<7; i++)
			m_InvestData.m_TotCapital += m_InvestData.m_pCapitalList[0].Profit[i];
	}

	m_InvestData.m_siCapitalNum = lpOnResProfitStatistics->siNum;
	m_InvestData.m_siCurCapital = 0;

	return 0;
}

UI16 OnlineGovernMgr::InvestmentList(BYTE *pMsg)
{
	OnResGovernmentGetInvestmentList *lpInvestMain = (OnResGovernmentGetInvestmentList*)pMsg;

	if(lpInvestMain->dwResponse == ON_RET_OK)
	{
		OnInvestmentInfo *lpInvestSub = (OnInvestmentInfo*)((BYTE*)lpInvestMain + sizeof(OnResGovernmentGetInvestmentList));

		// 투자자목록
		if(m_InvestData.m_pInvestList) 
		{
			delete[] m_InvestData.m_pInvestList;
			m_InvestData.m_pInvestList = NULL;
		}
		m_InvestData.m_pInvestList = new OnInvestmentInfo[lpInvestMain->m_InvestmentListCount];
		memcpy(m_InvestData.m_pInvestList, lpInvestSub, sizeof(OnInvestmentInfo) * lpInvestMain->m_InvestmentListCount);
		m_InvestData.m_siInvestNum = lpInvestMain->m_InvestmentListCount;

		// 내가투자할수있는날짜
		m_InvestData.m_siMyInvestCanTime = lpInvestMain->m_MyInvestmentCanTime;
		if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
			m_InvestData.m_BInvest.SetAction(TRUE);
		else
			m_InvestData.m_BInvest.SetAction(FALSE);

		// 총 투자금, 내가 투자한 금액
		m_InvestData.m_MyInvestMoney = 0;
		m_InvestData.m_TotInvestMoney = 0;
		m_InvestData.m_BGetOut.SetAction(FALSE);
		for(int i = 0; i < lpInvestMain->m_InvestmentListCount; i++)
		{
			m_InvestData.m_TotInvestMoney += lpInvestSub[i].m_InvestmentMoney;
			if(strcmp((char*)lpInvestSub[i].m_InvestmentName, (char*)pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
			{
				m_InvestData.m_MyInvestMoney = lpInvestSub[i].m_InvestmentMoney;
				m_InvestData.m_BGetOut.SetAction(TRUE);
			}
		}

		// 투자금 인출가능여부, 인출시 세금 퍼센트
		m_InvestData.m_bGetOut = lpInvestMain->m_fGetOut;
		m_InvestData.m_GetOutTexPersent = lpInvestMain->m_GetOutTex;
		
		// 마을의 최고 투자자 아이디 변경
		strcpy((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.m_MaxInvestmentID, (char*)lpInvestMain->m_MaxInvestmentID);

		// 마을정보 다시요청
		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
	}
	
	return 0;
}

// robypark 2004/12/2 14:15
// 공성전 종료 후에 마을 폭탄 투가(권한 여부)가 가능한지에 대한 응답
// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
UI16 OnlineGovernMgr::OnResponseVillageInvestmentPower(BYTE *pMsg)
{
	OnResInvestmentPower	*pOnResInvestmentPower = (OnResInvestmentPower *)pMsg;

	m_InvestData.m_siAllInInvestmentPercent = 0;
	
	if (ON_RET_OK == pOnResInvestmentPower->dwResponse)
	{
		m_InvestData.m_siAllInInvestmentPercent = pOnResInvestmentPower->siPercent;

		// 유효한 투자 권한이라면
		if (m_InvestData.m_siAllInInvestmentPercent)
		{
			// 공성전 폭투 가능 시간이면, 투자 버튼 활성화
			if (pMyOnlineWorld->IsInTimeAllInInvestment())
			{
				m_InvestData.m_BInvest.SetAction(TRUE);

				// 투자 권한에 대한 메시지 박스로 알려주기
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), 
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_INVESTMENT_POWER_MESSAGE), 
														pMyOnlineWorld->pOnlineVillage->GetVillageName(),pOnResInvestmentPower->siPercent,
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_AllInInvestment_Begin,
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_AllInInvestment_End);
			}
		}
	}

	return 0;
}

// SendRequestGovernment_Invenstment_Enter()에 대한 응답
UI16 OnlineGovernMgr::GovernInvestMylist(BYTE *pMsg)
{
	OnResGovernmentEnterInvestment *lpInvestment = (OnResGovernmentEnterInvestment*)pMsg;

	m_InvestData.m_siMyInvestMaxPersent = lpInvestment->m_InvestmentMaxPercent;
	m_InvestData.m_siMyInvestMinPersent = lpInvestment->m_InvestmentMinPercent;
	
	return 0;
}

UI16 OnlineGovernMgr::GovernInvestLeave(BYTE *pMsg)
{
	return 0;
}

UI16 OnlineGovernMgr::GovernInvestSet(BYTE *pMsg)
{
	OnResGovernmentSetInvestment *lpOnResGovernmentSetInvestment = (OnResGovernmentSetInvestment*)pMsg;

	if(m_InvestData.m_MyInvestMoney)	m_InvestData.m_BGetOut.SetAction(TRUE);
	if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
		m_InvestData.m_BInvest.SetAction(TRUE);

	if(lpOnResGovernmentSetInvestment->dwResponse == ON_RET_OK)
	{
		// 소지금 변경
		pMyOnlineWorld->pMyData->SetMoney(m_InvestData.m_InvestAfterMoney);
		
		// 전장 잔고 변경. 폭투한 것이라면
		if (m_InvestData.m_siAllInInvestmentPercent)
		{
			m_InvestData.m_siAllInInvestmentPercent = 0;
			pMyOnlineWorld->pMyData->SetSaving(m_InvestData.m_InvestAfterMoney_InBank);
		}
	}
	else if(lpOnResGovernmentSetInvestment->dwResponse == ON_RET_NOTINVESTMENT_MANYMAXINVESTMENT)
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BESTINVESTMENT90));
	}
	else
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIRSTINVESTMENT), m_InvestData.m_siMyInvestMinPersent, m_InvestData.m_siMyInvestMaxPersent);
	}

	// 폭탄 투자에 실패한 것이라면 투자 버튼 재활성화
	if (m_InvestData.m_siAllInInvestmentPercent)
		m_InvestData.m_BInvest.SetAction(TRUE);

	m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;

	return 0;
}

UI16 OnlineGovernMgr::GovernInvestGetout(BYTE *pMsg)
{
	OnResGovernmentInvestmentGetOut *pResInvestmentGetOut = (OnResGovernmentInvestmentGetOut*)pMsg;

	m_InvestData.m_uiStatus = ON_SUB_INVEST_POLL;

	if(m_InvestData.m_MyInvestMoney)	m_InvestData.m_BGetOut.SetAction(TRUE);
	if(m_InvestData.m_siMyInvestCanTime <= MakeTimeInt(pMyOnlineWorld->pTimer->GetNowTime()))
		m_InvestData.m_BInvest.SetAction(TRUE);
				
	switch(pResInvestmentGetOut->dwSuccess)
	{
	case 0:
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_OK));
		pMyOnlineWorld->pMyData->SetSaving(pMyOnlineWorld->pMyData->GetSaving() + pResInvestmentGetOut->GetOutMoney);
		break;
	case 1:
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_ERRORMONEY));
		break;
	case 2:
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_ERRORINDEX));
		break;
	}
	pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;

	return 0;
}

// GovernGuild에서 쓰임
UI16 OnlineGovernMgr::GovernGuildCreateCost(BYTE *pMsg)
{
	OnResponseGuildCreateCost *lpGuildCreateCost = (OnResponseGuildCreateCost*)pMsg;

	m_GuildData.m_GuildOpenMoney = lpGuildCreateCost->mnGuildCreateCost;
	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
	return 0;
}

UI16 OnlineGovernMgr::GovernGuildCreate(BYTE *pMsg)
{
	OnResCreateGuildMsg* lpCreateGuildMsg = (OnResCreateGuildMsg*)pMsg;
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_hOecEditBox );

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;

	if(lpCreateGuildMsg->dwResponse == ON_RET_OK)
	{
		m_GuildData.m_BOpen.SetAction(FALSE);
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONINS));
		
		// 길드 설립시 현재 마을의 길드을 수정한다.
		CHAR szGuildName[ON_GUILDNAME_LENGTH+1];
		ZeroMemory(szGuildName, sizeof(szGuildName));
//		m_GuildData.m_pEditBox->GetString(szGuildName);
		strcpy( szGuildName, pOecEx->GetString() );
		
		pMyOnlineWorld->pMyData->SetGuild((char*)szGuildName, ON_GUILDNAME_LENGTH, 0);
		strcpy((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, (char*)szGuildName);
		
		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
		{
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		}
		
		// 마을 정보 수정
		SVillageData *lpSVillageData = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				strcpy(lpSVillageData->m_VillageGuildName, (char*)szGuildName);
				lpSVillageData->m_VillageGuildFlag = 0;
				
				if(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode)
				{
					if(pMyOnlineWorld->pOnlineVillageInfoData->m_lpVillageHeader->uiVillageCode == pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
						pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode);
				}
			}
		}
		pMyOnlineWorld->pOnlineVillageDescriptList->ClearVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - m_GuildData.m_GuildOpenMoney);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONINSNO));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildDelete(BYTE *pMsg)
{
	OnResDeleteGuildMsg* lpDeleteGuildMsg = (OnResDeleteGuildMsg*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;

	if(lpDeleteGuildMsg->dwResponse == ON_RET_OK)
	{
		m_GuildData.m_BClose.SetAction(FALSE);
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONDIS));
		
		// 길드 해체시 현재 마을의 길드를 수정한다.
		pMyOnlineWorld->pMyData->SetGuild(NULL, 0, 0);
		ZeroMemory((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, sizeof(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID));
		
		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
		{
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		}
		
		// 마을의 정보를 찾아서 수정한다.
		SVillageData *lpSVillageData = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				ZeroMemory(lpSVillageData->m_VillageGuildName, sizeof(lpSVillageData->m_VillageGuildName));
				lpSVillageData->m_VillageGuildFlag = 0;
				
				if(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode)
				{
					if(pMyOnlineWorld->pOnlineVillageInfoData->m_lpVillageHeader->uiVillageCode == pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
						pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode);
				}
			}
		}
		pMyOnlineWorld->pOnlineVillageDescriptList->ClearVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONINSNO));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildJoin(BYTE *pMsg)
{
	OnResJoinLeaveGuildMsg* lpJoinLeaveGuildMsg = (OnResJoinLeaveGuildMsg*)pMsg;

	// 상단가입 응답
	if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_WAIT)
	{
		if(lpJoinLeaveGuildMsg->dwResponse == ON_RET_GLD_JOIN_OK)
		{
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONCOMLOETE));
			
			// 길드 해체시 현재 마을의 길드을 수정한다.
			pMyOnlineWorld->pMyData->SetGuild((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ), 0);
			
			if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		}
		else
		{
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT),
				pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_NOT_APPLY));
		}
	}
	// 상단탈퇴 응답
	else if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_WAIT2)
	{
		switch(lpJoinLeaveGuildMsg->dwResponse)
		{
		case ON_RET_GLD_LEAVE_OK_DELETEGLD:
			// 마을의 길드 삭제
			ZeroMemory((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, sizeof(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID));
			if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
			// 여기서 Break를 일부로 뺏음. 다음 처리가 아래 ON_RET_GLD_LEAVE_OK 부분하고 같기 때문에
		case ON_RET_GLD_LEAVE_OK:
			if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONCENCLE));
			else
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONLEAVE));
			
			// 길드 해체시 현재 마을의 길드을 수정한다.
			pMyOnlineWorld->pMyData->SetGuild(NULL, 0, 0);
			break;
		default:
			if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONCENCLENO));
			else
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONLEAVENO));
			break;
		}
	}
	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildFlagList(BYTE *pMsg)
{
	OnResUsedFlagList *lpUsedFlagList = (OnResUsedFlagList*)pMsg;

	// 상단원수가 GUILD_FLAG_MIN_NEED_MEMBER를 넘지않으면 깃발선택 불가능!
	if(lpUsedFlagList->uiGuildMemberNum < GUILD_FLAG_MIN_NEED_MEMBER)
	{
		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_CHANGEFLAGNEEDPEOPLE));
		return 0;
	}

	// 각각의 깃발들이 사용중인지에 대한 정보
	strFlagInfo *lpstrFlagInfo = (strFlagInfo*)((char*)lpUsedFlagList + sizeof(OnResUsedFlagList));

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
	m_GuildData.m_siFlagCount = 0;
	ZeroMemory(m_GuildData.m_siFlagIndex, sizeof(SI32)*MAX_ONLINE_FLAG_FILE);

	for(SI32 i = 0; i < MAX_ONLINE_FLAG_FILE; i++)
	{
		XSPR *lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(i);
		if(!lpXSPR)			continue;
		if(!lpXSPR->Image)	continue;

		m_GuildData.m_siFlagIndex[m_GuildData.m_siFlagCount] = i;
		m_GuildData.m_siFlagCount++;
	}
	
	for(i = 0; i < lpUsedFlagList->siUsedFlagNum; i++)
	{
		for(SI32 j = 0; j < MAX_ONLINE_FLAG_FILE; j++)
		{
			if(m_GuildData.m_siFlagIndex[j] == lpstrFlagInfo[i].siFlag)
			{
				m_GuildData.m_siFlagIndex[j] |= 0x8000;
				break;
			}
		}
	}

	return 0;
}

// 상단 깃발을 변경	
UI16 OnlineGovernMgr::ResChangeGuildFlag(BYTE *pMsg)
{
	OnResGuildChangeFlag *lpGuildChangeFlag = (OnResGuildChangeFlag*)pMsg;

	if(lpGuildChangeFlag->dwResponse == ON_RET_OK)
	{
		// 현재 자신의 Data 교체
		pMyOnlineWorld->pMyData->SetGuild(pMyOnlineWorld->pMyData->GetGuildName(), pMyOnlineWorld->pMyData->GetGuildClass(), lpGuildChangeFlag->siFlag);
		pMyOnlineWorld->pIOnlineChar->SetGuildData(pMyOnlineWorld->pMyData->GetMyCharUniqueID(), pMyOnlineWorld->pMyData->GetGuildName(), pMyOnlineWorld->pMyData->GetGuildClass(), lpGuildChangeFlag->siFlag);

		// 마을의 정보를 검색하여, 같은 Guild이면 Flag 교체
		for(SI16 i = 0; i < pMyOnlineWorld->pOnlineVillageDescriptList->m_VillageDataCount; i++)
		{
			if(strcmp((char*)pMyOnlineWorld->pOnlineVillageDescriptList->m_lpVillageData[i].m_VillageGuildName, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
			{
				pMyOnlineWorld->pOnlineVillageDescriptList->m_lpVillageData[i].m_VillageGuildFlag = lpGuildChangeFlag->siFlag;

				if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
					pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
			}
		}

		lpGuildChangeFlag->dwMsgType = ON_RESPONSE_CHANGE_GUILD_FLAG1;
		pMyOnlineWorld->pOnlineClient->AddMsgBuffer( ON_RESPONSE_CHANGE_GUILD_FLAG1, lpGuildChangeFlag, sizeof(OnResGuildChangeFlag) );
	}

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildFlagSet(BYTE *pMsg)
{
	if(!pMyOnlineWorld->pOnlineVillage)		return 0;

	OnResGuildChangeFlag* lpGuildChangeFlag = (OnResGuildChangeFlag*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;

	if(lpGuildChangeFlag->dwResponse == ON_RET_OK)
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_CHANGEFLAG));
		
		// 현재 자신의 Data 교체
		pMyOnlineWorld->pMyData->SetGuild(pMyOnlineWorld->pMyData->GetGuildName(), pMyOnlineWorld->pMyData->GetGuildClass(), m_GuildData.m_siFlagIndex[m_GuildData.m_siSelect]);
		
		// 마을 정보 수정
		SVillageData *lpSVillageData = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		if(lpSVillageData)
		{
			lpSVillageData->m_VillageGuildFlag = m_GuildData.m_siFlagIndex[m_GuildData.m_siSelect];
			
			if(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode)
			{
				if(pMyOnlineWorld->pOnlineVillageInfoData->m_lpVillageHeader->uiVillageCode == pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
					pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode);
			}
		}
		pMyOnlineWorld->pOnlineVillageDescriptList->ClearVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - GUILD_FLAG_COST);
	}
	else	// 실패시
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_CHANGEFLAGFAIL));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildList(BYTE *pMsg)
{
	OnResGuildListMsg *lpGuildListMsg = (OnResGuildListMsg*)pMsg;
	BYTE *lpTempBuffer = (BYTE*)lpGuildListMsg + sizeof(OnResGuildListMsg);

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	m_GuildData.m_uiGuildNum = lpGuildListMsg->uiGuildNum;

	if(m_GuildData.m_pGuildList) 
	{
		delete[] m_GuildData.m_pGuildList;
		m_GuildData.m_pGuildList = NULL;
	}

	if(lpGuildListMsg->uiGuildNum > 0)
	{
		m_GuildData.m_pGuildList = new strGuildInfo[lpGuildListMsg->uiGuildNum];
		ZeroMemory(m_GuildData.m_pGuildList, sizeof(strGuildInfo) * lpGuildListMsg->uiGuildNum);
		memcpy(m_GuildData.m_pGuildList, lpTempBuffer, sizeof(strGuildInfo) * lpGuildListMsg->uiGuildNum);
	}

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildSetRelation(BYTE *pMsg)
{
	OnResRelationMsg* lpOnResponseMsg = (OnResRelationMsg*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpOnResponseMsg->dwResponse == ON_RET_OK)
	{ 
		if(m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation == ON_RET_GLD_ENEMY)
		{
			m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation			  = ON_RET_GLD_NEUTRAL;
			m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelationOfOpponent = ON_RET_GLD_NEUTRAL;
		}
		else
		{
			m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation			  = ON_RET_GLD_ENEMY;
			m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelationOfOpponent = ON_RET_GLD_ENEMY;
		}
	}
	else
	{
		if(lpOnResponseMsg->bRelation == TRUE)
		{
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), "18시부터 22시 사이에는 상단 관계를 설정 할 수 없습니다. ");
		}
		else
		{
			if(m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation == ON_RET_GLD_ENEMY)
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSON1));
			else
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSOFF1));
		}
	}

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildDetailInfo(BYTE *pMsg)
{
	OnResGuildDetailInfoMsg *lpGuildDetailInfoMsg = (OnResGuildDetailInfoMsg*)pMsg;

	if(lpGuildDetailInfoMsg->dwResponse == ON_RET_OK)
	{
		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_VILLAGELIST;

		BYTE *lpTempBuffer = (BYTE*)lpGuildDetailInfoMsg + sizeof(OnResGuildDetailInfoMsg);

		if(m_GuildData.m_pVillageList) 
		{
			delete[] m_GuildData.m_pVillageList;
			m_GuildData.m_pVillageList = NULL;
		}

		m_GuildData.m_pVillageList = new OnGuildVillageInfo[lpGuildDetailInfoMsg->uiOwnVillageNum];

		memcpy(m_GuildData.m_pVillageList, lpTempBuffer, sizeof(OnGuildVillageInfo) * lpGuildDetailInfoMsg->uiOwnVillageNum);
		m_GuildData.m_uiVillageNum = lpGuildDetailInfoMsg->uiOwnVillageNum;
		m_GuildData.m_siCurPage = 1;
		m_GuildData.m_siMaxPage = (SI16)((lpGuildDetailInfoMsg->uiOwnVillageNum-1) / PAGE_LIST_MAX_LINE) + 1;
		m_GuildData.m_BOk.SetAction(FALSE);
		m_GuildData.m_BBack.SetX(m_siX+227);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_RECEIVE_INFO_FORM_SERVER));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildWaitingMemberList(BYTE *pMsg)
{
	OnResGuildWaitingJoinMembersListMsg *lpWaitingListMsg = (OnResGuildWaitingJoinMembersListMsg*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpWaitingListMsg->dwResponse == ON_RET_OK)
	{
		BYTE *lpTempBuffer = (BYTE*)lpWaitingListMsg + sizeof(OnResGuildWaitingJoinMembersListMsg);
		
		if(m_GuildData.m_pWaitingList) 
		{
			delete[] m_GuildData.m_pWaitingList;
			m_GuildData.m_pWaitingList = NULL;
		}

		m_GuildData.m_uiWaitingNum = lpWaitingListMsg->uiCandidateMembersNum;
		m_GuildData.m_siMaxPage = lpWaitingListMsg->dwTotalCandidateListPageNum;
		m_GuildData.m_siSelect = 0;
		
		if(lpWaitingListMsg->uiCandidateMembersNum > 0)
		{
			m_GuildData.m_pWaitingList = new strCandidateInfo[lpWaitingListMsg->uiCandidateMembersNum];
			memcpy(m_GuildData.m_pWaitingList, lpTempBuffer, sizeof(strCandidateInfo) * lpWaitingListMsg->uiCandidateMembersNum);
			
			for(UI16 i = 0; i < lpWaitingListMsg->uiCandidateMembersNum; i++)
			{
				const TradeHeader* pHeader = pMyOnlineWorld->pOnlineTradeParser->GetHeader(m_GuildData.m_pWaitingList[i].uiTradeCredit);
				m_GuildData.m_pWaitingList[i].uiTradeCredit = (pHeader == NULL) ? 0 : pHeader->uiTradeGrade;
			}
		}
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONFAIL));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildMemberList(BYTE *pMsg)
{
	OnResGuildMembersListMsg *lpMembersListMsg = (OnResGuildMembersListMsg*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpMembersListMsg->dwResponse == ON_RET_OK)
	{
		BYTE *lpTempBuffer = (BYTE*)lpMembersListMsg + sizeof(OnResGuildMembersListMsg);

		if(m_GuildData.m_pMemberList) 
		{
			delete[] m_GuildData.m_pMemberList;
			m_GuildData.m_pMemberList = NULL;
		}
		
		m_GuildData.m_pMemberList = new strGuildMemberInfo[lpMembersListMsg->uiGuildMemebersNum];
		
		m_GuildData.m_uiMemberNum = lpMembersListMsg->uiGuildMemebersNum;
		m_GuildData.m_siMaxPage = lpMembersListMsg->dwTotalMemberListPageNum;
		
		memcpy(m_GuildData.m_pMemberList, lpTempBuffer, sizeof(strGuildMemberInfo) * lpMembersListMsg->uiGuildMemebersNum);
		
		for(UI16 i = 0; i < lpMembersListMsg->uiGuildMemebersNum; i++)
		{
			const TradeHeader* pHeader = pMyOnlineWorld->pOnlineTradeParser->GetHeader(m_GuildData.m_pMemberList[i].uiTradeCredit);
			m_GuildData.m_pMemberList[i].uiTradeCredit = (pHeader == NULL) ? 0 : pHeader->uiTradeGrade;
		}

		if(m_GuildData.m_pMemberList[0].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			m_GuildData.m_siSelect = -1;
		else
			m_GuildData.m_siSelect = 0;
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONFAIL));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildWaitingMemberJoin(BYTE *pMsg)
{
	OnResJoinGuildOkNoMsg* lpJoinGuildOkNoMsg = (OnResJoinGuildOkNoMsg*)pMsg;

	if(lpJoinGuildOkNoMsg->dwResponse == ON_RET_OK)
	{
		// 가입을 하였으면 그 User는 삭제
		m_GuildData.m_uiWaitingNum--;
		for(UI16 i = m_GuildData.m_siSelect; i < m_GuildData.m_uiWaitingNum; i++)
		{
			m_GuildData.m_pWaitingList[i] = m_GuildData.m_pWaitingList[i+1];
		}

		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
		
		// 새로운 Page 정보 요청
		pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(m_GuildData.m_siCurPage);
	}
	else
	{
		if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_WAIT)			// 입단허용 실패
		{
			if(lpJoinGuildOkNoMsg->dwResponse == ON_RET_NO_GLD_ADMIT_FULL)
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_JOIN_ERROR1));
			else if(lpJoinGuildOkNoMsg->dwResponse == ON_RET_NO_GLD_ADMIT_MAXINVESTOR)
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_JOIN_ERROR2));
			else
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_JOINTRADEASSOCIATION));
		}
		else if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_WAIT2)	// 입단취소 실패
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_OUTTRADEASSOCIATION));

		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
	}
	return 0;
}

UI16 OnlineGovernMgr::GovernGuildPromotion(BYTE *pMsg)
{
	OnResponseGuildPromotion* lpGuildPromotion = (OnResponseGuildPromotion*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpGuildPromotion->dwResponse == ON_RET_OK)
	{
		// 새로운 Page 정보 요청
		pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_GuildData.m_siCurPage, 0);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_NOT_APPOINT));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildDemotion(BYTE *pMsg)
{
	OnResponseGuildDemotion* lpGuildDemotion = (OnResponseGuildDemotion*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpGuildDemotion->dwResponse == ON_RET_OK)
	{
		// 새로운 Page 정보 요청
		pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_GuildData.m_siCurPage, 0);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIREFAIL));

	return 0;
}

UI16 OnlineGovernMgr::GovernGuildDismiss(BYTE *pMsg)
{
	OnResDismissalMsg* lpDismissalMsg = (OnResDismissalMsg*)pMsg;

	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

	if(lpDismissalMsg->dwResponse == ON_RET_OK)
	{
		// 해고을 하였으면 그 User는 삭제
		m_GuildData.m_uiMemberNum--;
		for(UI16 i = m_GuildData.m_siSelect; i < m_GuildData.m_uiMemberNum; i++)
		{
			m_GuildData.m_pMemberList[i] = m_GuildData.m_pMemberList[i+1];
		}

		// 새로운 Page 정보 요청
		pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_GuildData.m_siCurPage, 0);
	}
	else
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIREFAIL));

	return 0;
}
