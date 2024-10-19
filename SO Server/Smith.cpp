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
	// ���� �Ұ� 0�̸� ������ü�� �Ҽ����� 1�̸� Ư�������� 2�̸� �⺻������ 3�̸� �״�� 4�̸� �ڻ�.
	SI16 siResponse = 0;
	SI16 emptyitemslot;
	MyItemData *pOriginalItemData = NULL;
	MyItemData *pAddItemData = NULL;
	SmithItemHeader *pSmithItemHeader = NULL;

	// �뺴�� ������ �ִ°�.
	if (pPlayer->IFollower.IsValidFollower(uiFollowerNum) == TRUE) {
		
		pSmithItemHeader = m_pWorld->clSmithParser.GetSmithItem(uiItemCode);
		// �׾������� ������ �����Ѱ�?
		if (pSmithItemHeader != NULL) {
			// ����� �����ִ°�?
			if (pPlayer->GetMoney() >= pSmithItemHeader->moNeedMoney) {
				// �׿뺴�� �󽽷��� �ִ°�.
				if ((emptyitemslot = pPlayer->IFollower.FindEmptyItemBuffer(uiFollowerNum)) != -1)
				{
					
					// �� �뺴�� �� �������� ������ �ִ°�.
					if ((pOriginalItemData = pPlayer->IFollower.GetItemByID(uiFollowerNum,uiItemCode)) != NULL)
					{
						// Ȯ�����
						UI16 randomvalue = (rand() % 10000);
						

						

						// ���� ������ ������.
						pPlayer->DecreaseMoney(pSmithItemHeader->moNeedMoney,FALSE);
						
						//  �Ϲݾ����� Ȯ�� �̻��̸� ��޾�����
						if (randomvalue >= pSmithItemHeader->uiNormalProbablity)
						{
							pAddItemData = pPlayer->AddItemNoQuery(uiFollowerNum,pSmithItemHeader->usSpecialItem,1,emptyitemslot);							
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 1;

						}
						// �⺻ �̻��̸� �Ϲݾ�����
						else if (randomvalue >= pSmithItemHeader->uiOriginalProbablity)
						{
							pAddItemData = pPlayer->AddItemNoQuery(uiFollowerNum,pSmithItemHeader->usNormalItem,1,emptyitemslot);							
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 2;
							
						}
						// �μ��� Ȯ�� �̻��̸� �״��
						else if (randomvalue >= pSmithItemHeader->uiBrokenProbablity)
						{
							// �ƹ��� ����.
							siResponse = 3;
						}
						// �ڻ�
						else
						{
							// �����۸� ���ش�.
							pPlayer->DelItemNoQuery(uiFollowerNum,uiItemCode,1);
							siResponse = 4;
						}

						
					}
					
					
					
					
				}
			}
			
		}
	}

	// �������� ����.

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


	// ó���� ������ (�������� �ν����� ��޾������� �ƴ� ���� �۾��� �̷��������) �̸� db�� ����Ѵ�.
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

		
		// ��������� ������ ������ ������ �ٸ���.
		switch(siResponse)
		{
			// �Ϲ�,��޾������ϰ�� success����
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