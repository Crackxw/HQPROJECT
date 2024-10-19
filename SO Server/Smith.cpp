#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "Smith.h"

SOWorld *CSmith::m_pWorld;

CSmith::CSmith(SOWorld *pWorld)
{
	m_pWorld = pWorld;

}

CSmith::~CSmith()
{
	;

}

void CSmith::RecvSmithItem(SOPlayer *pPlayer,UI16 uiFollowerNum,UI16 uiItemCode)
{
	// 응답 할값 0이면 제련자체를 할수없음 1이면 특수아이템 2이면 기본아이템 3이면 그대로 4이면 박살.
	SI16 siResponse = 0;
	SI16 emptyitemslot;
	MyItemData *pOriginalItemData = NULL;
	MyItemData *pAddItemData = NULL;
	SmithItemHeader *pSmithItemHeader = NULL;

	// 용병이 실제로 있는가.
	if (pPlayer->IFollower.IsValidFollower(uiFollowerNum) == TRUE) {
		
		pSmithItemHeader = m_pWorld->clSmithParser.GetSmithItem(uiItemCode);
		// 그아이템이 제련이 가능한가?
		if (pSmithItemHeader != NULL) {
			// 충분한 돈이있는가?
			if (pPlayer->GetMoney() >= pSmithItemHeader->moNeedMoney) {
				// 그용병이 빈슬롯이 있는가.
				if ((emptyitemslot = pPlayer->IFollower.FindEmptyItemBuffer(uiFollowerNum)) != -1)
				{
					
					// 그 용병이 그 아이템을 가지고 있는가.
					if ((pOriginalItemData = pPlayer->IFollower.GetItemByID(uiFollowerNum,uiItemCode)) != NULL)
					{
						// 확률계산
						UI16 randomvalue = (rand() % 10000);
						

						

						// 돈은 무조건 빠진다.
						pPlayer->DecreaseMoney(pSmithItemHeader->moNeedMoney,FALSE);
						
						//  일반아이템 확률 이상이면 고급아이템
						if (randomvalue >= pSmithItemHeader->uiNormalProbablity)
						{
							pAddItemData = pPlayer->AddItemNoQuery(uiFollowerNum,pSmithItemHeader->usSpecialItem,1,emptyitemslot);							
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 1;

						}
						// 기본 이상이면 일반아이템
						else if (randomvalue >= pSmithItemHeader->uiOriginalProbablity)
						{
							pAddItemData = pPlayer->AddItemNoQuery(uiFollowerNum,pSmithItemHeader->usNormalItem,1,emptyitemslot);							
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 2;
							
						}
						// 부서질 확률 이상이면 그대로
						else if (randomvalue >= pSmithItemHeader->uiBrokenProbablity)
						{
							// 아무일 없다.
							siResponse = 3;
						}
						// 박살
						else
						{
							// 아이템만 없앤다.
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 4;
						}

						
					}
					
					
					
					
				}
			}
			
		}
	}

	// 유저에게 응답.

	OnResponseRefineItem Msg;

	Msg.dwMsgType = ON_RESPONSE_REFINE_ITEM;
	Msg.uiResult = siResponse;
	if (pAddItemData)
	{
		Msg.uiFollowerSlot = uiFollowerNum;
		Msg.uiNewItemID = pAddItemData->uiID;
		Msg.uiNewItemSlot = pAddItemData->siPosInInventory;
	}

	

	pPlayer->Write(&Msg,sizeof(Msg));


	// 처리가 성공적 (아이템이 부숴졌던 고급아이템이 됐던 무언가 작업이 이루어졌으면) 이면 db에 기록한다.
	if (siResponse != 0) {

		DBIKRefineItem Dbinfo;
		ZeroMemory(&Dbinfo,sizeof(Dbinfo));
		Dbinfo.siDBAccount = pPlayer->stAccountInfo.siDBAccount;
		Dbinfo.uiDBSlot = pPlayer->stAccountInfo.uiSlot;
		Dbinfo.uiOldItemID = uiItemCode;
		Dbinfo.uiMercenarySlot = uiFollowerNum;
		
		if (pSmithItemHeader)
		{
			Dbinfo.moMoney = pSmithItemHeader->moNeedMoney;
		}
		if (pAddItemData) {
			Dbinfo.uiItemSlot = pAddItemData->siPosInInventory;
			Dbinfo.uiNewItemID = pAddItemData->uiID;
			
			
		}

		
		// 결과에따라서 보내는 쿼리의 종류가 다르다.
		switch(siResponse)
		{
			// 일반,고급아이템일경우 success쿼리
		case 1:
		case 2:

			m_pWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SMITH_REFINE_SUCCESS,&Dbinfo,sizeof(Dbinfo));
				break;
		case 3:
			m_pWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SMITH_REFINE_NO_CHANGES,&Dbinfo,sizeof(Dbinfo));
				break;
		case 4:
			m_pWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SMITH_REFINE_FAILED,&Dbinfo,sizeof(Dbinfo));
				break;
		}
	}

	

}