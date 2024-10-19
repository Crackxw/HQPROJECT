#include <GSL.h>
#include	<Main.h> 
#include	<Mouse.h>

#include "..\Gersang\Message.h"
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include "..\Gersang\directx.h"

#include <Graphics16.h>
#include "OnlineWorld.h"
#include "OnlineVillageManager.h"
#include <DebugTool.h>
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineCharUI.h"
//#include "OnlineManContactVillage.h"
#include "OnlineVillage.h"
#include "OnlineVillageSave.h"
#include "OnlineQuestPlay.h"
#include "OnlineFieldAuction.h"
#include "OnlineMsgBox.h"
#include "OnlineSmith.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �� ĳ������ ����� ó���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::ProcessMyCharOrder()
{
	OnOrder		Order;	
	SI16		siHeroX, siHeroY, siHeroRX, siHeroRY, siAttackCharX, siAttackCharY;	
	SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

//	if (FALSE == pOnlineCharUI->IsAction())
//		return;

	// ���� �� ĳ���Ͱ� �����ϴ¸���� ���´�.
	Order	=	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());

	switch(Order.siOrder)
	{	
	case ON_ORDER_ATTACK:
		// �켱 ���� �����Ϸ��� ĳ���Ͱ� ������ �ִ��� �˻��Ѵ�.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// ���� ��ġ�� ���� ���� �Ϸ��� ĳ������ ��ġ�� ���´�.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);


			// �� ĳ���� ���� ���̰� ������� ����.(�� ������ �� �� �ִ� ��Ȳ���� ����.)
			// �׸��� Ȥ�� ���� ������ �Ѵٴ� �޽����� ���������� �� ������ ��ٸ��� ��Ȳ������ üũ�Ѵ�.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, 1) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// �ο� �� �ִ� ��Ȳ�̴�. �������� �޽����� ������.
					// �ٸ� �÷��̾�� ������ �Ѵ� �ϰ� �����κ����� ������ ��ٸ��ٰ� �����Ѵ�.
					pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// ĳ���Ϳ��� ���� ����� �����ش�.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// �� �̻��� ����� �Է��� ���� �ʴ´�.
					pOnlineCharUI->SetAction(FALSE);
				}	
			}					
		}	
		break;
	case ON_ORDER_FIELDATTACK_READY:
		// �켱 ���� �����Ϸ��� ĳ���Ͱ� ������ �ִ��� �˻��Ѵ�.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// ���� ��ġ�� ���� ���� �Ϸ��� ĳ������ ��ġ�� ���´�.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// �� ĳ���� ���� ���̰� ������� ����.(�� ������ �� �� �ִ� ��Ȳ���� ����.)
			// �׸��� Ȥ�� ���� ������ �Ѵٴ� �޽����� ���������� �� ������ ��ٸ��� ��Ȳ������ üũ�Ѵ�.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				// robypark 2004/10/27 16:3
				// ������ ĳ���� ���� ���� �ε���
				UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID());

				// robypark 2004/10/27 16:3
				// �� ĳ���� ���� ���ݰ��� �Ÿ� �˻縦 KindInfo���� ������ ���� �̿��ϵ��� ����.
				// ManContactvillage(x1, y1, 1, 1, x2, y2, 1) => ManContactvillage(x1, y1, 1, 1, x2, y2, KI[].GetAttackRange())
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, KI[uiKindInfoIdx_Attacker].GetAttackRange(0)) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// �ο� �� �ִ� ��Ȳ�̴�. �������� �޽����� ������.
					// �ٸ� �÷��̾�� ������ �Ѵ� �ϰ� �����κ����� ������ ��ٸ��ٰ� �����Ѵ�.
					/*pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// ĳ���Ϳ��� ���� ����� �����ش�.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// �� �̻��� ����� �Է��� ���� �ʴ´�.
					pOnlineCharUI->SetAction(FALSE);*/

					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// �� �̻��� ����� �Է��� ���� �ʴ´�.
						pOnlineCharUI->Init();
						pOnlineCharUI->SetAction(FALSE);
						
						// Server�� �޼��� ����
						pOnlineClient->SendFieldAttackReady(Order.siOrderParam1,siHeroX,siHeroY);					
						m_sSiegeWarfare_AttackMode.siAutoAttackTimes++;
					}
				}
//				else
//					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}	
		}
		break;
	case ON_ORDER_USEOBJECT:				
		{
			SI32		siObjectID;
			SI16		siMX;
			SI16		siMY;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			//-------------------------------------------------//
			// robypark 2005/1/13 17:39
			// �ŷ� ���̰ų�
			if (pOnlineTrade->IsActive())
			{
				// ������Ʈ ��� ����, ����� ����
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}

			// �ŷ� ��û �޽��� �ڽ��� Ȱ��ȭ �Ǿ��ٸ�
			if (pOnlineTrade->m_pMsgBox->IsActive())
			{
				// ������Ʈ ��� ����, ����� ����
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}
			//-------------------------------------------------//

			if(siObjectID != 0)
			{
				// ���� ������ ���� ������ �˾ƺ���.
				// �������� �˾ƺ���.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// �������� ��Ż���� ����
						if(pIOnlineObject->IsPortal(siObjectID))
						{
							// ��Ż�̴�.
							// ���� ��ġ�� ���Ѵ�.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// �� ������Ʈ�� ����� ���Ѵ�.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);

							if(siVillageYsize % 2)
							{
								siMX = siVillageXsize / 2;
								siMY = siVillageYsize / 2;
								siVillageX -= siMX;
								siVillageY -= siMY;
							}
							else
							{
								siMX = siVillageXsize / 2;
								siMY = siVillageYsize / 2;
							}
							if((siVillageXsize & 0x0001) == 0) siMX--;
							if((siVillageYsize & 0x0001) == 0) siMY--;
							if(ManContactvillage(siVillageX, siVillageY, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, 1) == TRUE)
							{
								// ������ ������ �ִ�. ���� ������ ����.										
								pMyData->SetGotoPortal(TRUE, pIOnlineObject->GetPortalID(siObjectID));

								// ĳ���Ϳ��� ���� ����� �����ش�.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// �� �̻��� ����� �Է��� ���� �ʴ´�.
								pOnlineCharUI->SetAction(FALSE);					
							}
						}
						else
						{
							// �����̴�.
							// ���� ��ġ�� ���Ѵ�.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// �� ������Ʈ�� ����� ���Ѵ�.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

							if(ManContactvillage(siVillageX - siVillageXsize / 2, 
											siVillageY - siVillageYsize / 2, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, 1) == TRUE)
							{
								
								// ������ ������ �ִ�.
								// ���� ������ ����.										
								pMyData->SetGotoVillage(TRUE, pIOnlineObject->GetVillageCode(siObjectID));

								// ĳ���Ϳ��� ���� ����� �����ش�.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// �� �̻��� ����� �Է��� ���� �ʴ´�.
								pOnlineCharUI->SetAction(FALSE);					

								m_VillageInStruct = -1;
							}				
						}
					}
				}
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;	
	case ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE:
		{
			SI32		siObjectID;
//			SI16		siMX;
//			SI16		siMY;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			//-------------------------------------------------//
			// robypark 2005/1/13 17:39
			// �ŷ� ���̰ų�
			if (pOnlineTrade->IsActive())
			{
				// ������Ʈ ��� ����, ����� ����
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}

			// �ŷ� ��û �޽��� �ڽ��� Ȱ��ȭ �Ǿ��ٸ�
			if (pOnlineTrade->m_pMsgBox->IsActive())
			{
				// ������Ʈ ��� ����, ����� ����
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}
			//-------------------------------------------------//

			if(siObjectID != 0)
			{
				// ���� ������ ���� ������ �˾ƺ���.
				// �������� �˾ƺ���.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// �����̴�.
						// ���� ��ġ�� ���Ѵ�.
						pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

						// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
						pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

						// �� ������Ʈ�� ����� ���Ѵ�.
						pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

						if(ManContactvillage(siVillageX - siVillageXsize / 2, 
										siVillageY - siVillageYsize / 2, 
										siVillageXsize, siVillageYsize, siHeroX, siHeroY, 1) == TRUE)
						{
							if (0 != m_sVillageDefenceInterface.m_uiVillageCode)
							{
								// ĳ���Ϳ��� ���� ����� �����ش�.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
								break;
							}
							
							// ������ ������ �ִ�.
							// robypark 2004/12/12 14:44
							//  ������ �ð��̸�
							if (IsInTimeSiegeWarfare())
							{
								// �����ϱ� �������̽��� ���� ��û
								pOnlineClient->SendEnterVillageDefenceInterface(pIOnlineObject->GetVillageCode(siObjectID));

								m_sVillageDefenceInterface.m_uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

								// ĳ���Ϳ��� ���� ����� �����ش�.
								pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// �� �̻��� ����� �Է��� ���� �ʴ´�.
								pOnlineCharUI->Init();
								pOnlineCharUI->SetAction(FALSE);

								m_VillageInStruct = -1;
							}
							else
							{
								// ĳ���Ϳ��� ���� ����� �����ش�.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
								break;
							}
						}				
					}
				}
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;	
	case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE:				
		{
			SI32		siObjectID;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			if(siObjectID != 0)
			{
				// ���� ������ ���� ������ �˾ƺ���.
				// �������� �˾ƺ���.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// �������� ��Ż���� ����
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// �����̴�.
							// ���� ��ġ�� ���Ѵ�.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// �� ������Ʈ�� ����� ���Ѵ�.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

							// robypark 2004/10/27 16:3
							// ������ ĳ���� ���� ���� �ε���
							UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID());

							if(ManContactvillage(siVillageX - siVillageXsize / 2, 
											siVillageY - siVillageYsize / 2, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, KI[uiKindInfoIdx_Attacker].GetAttackRange(0)) == TRUE)
							{
								if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
								{
									VillageHeader*		lpVillageHeader;
		
									lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode(siObjectID));
									if(lpVillageHeader)
									{
										// ������ ������ �ִ�. ������ �����Ѵ�.
										// ĳ���Ϳ��� ���� ����� �����ش�.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION, siVillageX, siVillageY);

										// �� �̻��� ����� �Է��� ���� �ʴ´�.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server�� �޼��� ����
										// robypark 2004/11/4 13:56	
										// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
										SI16 scGeneralEffect = SiegeWarfare_IsInAreaGeneralEffect();
										if (-1 != scGeneralEffect)
										{
											//siHeroX,siHeroY
											pOnlineClient->SendShortDistanceAttackVillage(lpVillageHeader->uiVillageCode, scGeneralEffect);
											m_sSiegeWarfare_AttackMode.siAutoAttackTimes++;
										}
									}
								}
							}				
						}
					}
				}
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;	
	case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY:
		{
			SI32		siObjectID;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			if(siObjectID != 0)
			{
				// ���� ������ ���� ������ �˾ƺ���.
				// �������� �˾ƺ���.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// �������� ��Ż���� ����
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// �����̴�.
							// ���� ��ġ�� ���Ѵ�.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// �� ������Ʈ�� ����� ���Ѵ�.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					
							
							// robypark 2004/10/27 16:3
							// ������ ĳ���� ���� ���� �ε���
							UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID());

							// robypark 2004/10/27 16:3
							// ������ ĳ���� ���� ���ݰ��� �Ÿ� �˻縦 KindInfo���� ������ ���� �̿��ϵ��� ����.
							// ManContactvillage(x1, y1, 1, 1, x2, y2, 1) => ManContactvillage(x1, y1, 1, 1, x2, y2, KI[].GetAttackRange())
							//�ڿ� ���� 5��� ���ڰ� ������ ĳ���Ͱ��� ������ �ִ� �Ÿ��� ��Ÿ����.
							//ĳ������ Ư���� ���� ĳ���Ϳ��� �ҷ��� ����ϸ� ��.
							if(ManContactvillage(siVillageX - siVillageXsize / 2, 
											siVillageY - siVillageYsize / 2, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, KI[uiKindInfoIdx_Attacker].GetAttackRange(0)) == TRUE)
							{
								if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
								{
									VillageHeader*		lpVillageHeader;
		
									lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode(siObjectID));
									if(lpVillageHeader)
									{
										// ������ ������ �ִ�. ������ �����Ѵ�.
										// ĳ���Ϳ��� ���� ����� �����ش�.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

										// �� �̻��� ����� �Է��� ���� �ʴ´�.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server�� �޼��� ����
										pOnlineClient->SendLongDistanceAttackVillage(lpVillageHeader->uiVillageCode,siHeroX,siHeroY);
									}
								}
							}				
						}
					}
				}
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;
	case ON_ORDER_PICKUPITEM:
		{ 
			UI32	uiItemID;
			SI32	siItemX, siItemY;		
			
			siItemX		=	LOWORD(Order.siOrderParam1);
			siItemY		=	HIWORD(Order.siOrderParam1);

			// �켱 �� �������� �� ��ġ�� �ִ��� ����.
			uiItemID	=	pOnlineMap->GetItemID(siItemX, siItemY);			

			// ���� ���� �ֿ���� �ߴ� �����۰� ���� �������̿��� �Ѵ�.
			if(uiItemID == UI32(Order.siOrderParam2))
			{
				// ���� ��ġ�� ���Ѵ�.
				pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
				pIOnlineChar->GetRPos(pMyData->GetMyCharUniqueID(), &siHeroRX, &siHeroRY);

				// �������� �ֿﶧ ĳ���Ͱ� Ÿ�� ��� ���� �� �����Ѵ�.
				// �ȱ׷��� �̻��غ��δ�.			
				if(siHeroRX == 0 && siHeroRY == 0)
				{
					// ������ �����̿� ���� �ִ�������.
					if(ManContactvillage(siItemX, siItemY, 1, 1, siHeroX, siHeroY, 1) == TRUE)
					{
						// �������� �������� �ݴ´ٰ� ��û�Ѵ�.
						// Inventory�� �Ҵ�.
/*						if(pOnlineInventory->IsActive() == FALSE) 
						{
							pOnlineInventory->SetItemPickUp(TRUE);
							pOnlineInventory->SetActive(TRUE);
						}
*/
						pOnlineInventory->SetItemPickUp(TRUE);

						pOnlineInventory->SetFieldItemDrag(uiItemID, siItemX, siItemY);

						pMyData->SetWaitPickUpItemResponse(TRUE);			
						
						// ĳ���Ϳ��� ���� ����� �����ش�.
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// �� �̻��� ����� �Է��� ���� �ʴ´�.
						pOnlineCharUI->SetAction(FALSE);
					}				
				}
			}
			else
			{
				// �������� ����.
				// ��������� ������.						
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);				
			}			
		}
		break;
	// robypark 2004/11/9 17:16
	// ������ ���� ���޼����� ������ ������ ����ǰ�� ��û ó��
	case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON:
		// �켱 ĳ���Ͱ� ������ �ִ��� �˻��Ѵ�.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// ���� ��ġ�� ���޼��� ĳ������ ��ġ�� ���´�.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// �� ĳ���� ���� ���̰� ������� ����.(�� ������ �� �� �ִ� ��Ȳ���� ����.)
			// �׸��� Ȥ�� ���� ������ �Ѵٴ� �޽����� ���������� �� ������ ��ٸ��� ��Ȳ������ üũ�Ѵ�.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, VALID_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// �ο� �� �ִ� ��Ȳ�̴�. �������� �޽����� ������.
					// �ٸ� �÷��̾�� ������ �Ѵ� �ϰ� �����κ����� ������ ��ٸ��ٰ� �����Ѵ�.
					/*pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// ĳ���Ϳ��� ���� ����� �����ش�.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// �� �̻��� ����� �Է��� ���� �ʴ´�.
					pOnlineCharUI->SetAction(FALSE);*/

					
					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// �� �̻��� ����� �Է��� ���� �ʴ´�.
						//pOnlineCharUI->Init();
						//pOnlineCharUI->SetAction(FALSE);
						
						// Server�� �޼��� ����
						pOnlineClient->SendSiegeWarfareSupplyGoodsFromSupplyWagon(Order.siOrderParam1, 0);
					}
				}
//				else
//					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_MOVE, siAttackCharX, siAttackCharY);
			}	
		}
		break;
	// robypark 2004/11/9 18:29
	// ������ ���� ���޼����� ������ ����ǰ�� ��û ó��
	case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE:
		{
			SI32		siObjectID;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			if(siObjectID != 0)
			{
				// ���� ������ ���� ������ �˾ƺ���.
				// �������� �˾ƺ���.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// �������� ��Ż���� ����
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// �����̴�.
							// ���� ��ġ�� ���Ѵ�.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// �� ������Ʈ�� �� ��ġ�� ���Ѵ�.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// �� ������Ʈ�� ����� ���Ѵ�.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

							if(ManContactvillage(siVillageX - siVillageXsize / 2, 
											siVillageY - siVillageYsize / 2, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, 2) == TRUE)
							{
								if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
								{
									VillageHeader*		lpVillageHeader;
		
									lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode(siObjectID));
									if(lpVillageHeader)
									{
										// ������ ������ �ִ�. ������ �����Ѵ�.
										// ĳ���Ϳ��� ���� ����� �����ش�.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

										// �� �̻��� ����� �Է��� ���� �ʴ´�.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server�� �޼��� ����
										pOnlineClient->SendDecreaseGuildSupply(0, lpVillageHeader->uiVillageCode);
									}
								}
							}				
						}
					}
				}
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;
	// robypark 2005/1/25 19:58
	// ������ ���� ���޼����� ������ ������ ġ�� �õ�
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
		// �켱 ĳ���Ͱ� ������ �ִ��� �˻��Ѵ�.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// ���� ��ġ�� �ش� ĳ������ ��ġ�� ���´�.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// �� ĳ���� ���� ���̰� ������� ����.(�� ������ �� �� �ִ� ��Ȳ���� ����.)
			// �׸��� Ȥ�� ���� ������ �Ѵٴ� �޽����� ���������� �� ������ ��ٸ��� ��Ȳ������ üũ�Ѵ�.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, VALID_HEAL_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					
					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// �� �̻��� ����� �Է��� ���� �ʴ´�.
						pOnlineCharUI->Init();
						//pOnlineCharUI->SetAction(FALSE);
						
						// Server�� �޼��� ����
						pOnlineClient->SendSiegeWarfareSupplyGoodsFromSupplyWagon(Order.siOrderParam1, 0);
					}
				}
//				else
//					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_MOVE, siAttackCharX, siAttackCharY);
			}	
		}
		break;
	case ON_ORDER_NPC:
		{
			SI16				siNPCCode;
			SI16				siNPCX;
			SI16				siNPCY;
			NPCIndexHeader*		lpNPCIndexHeader;
			
			siNPCX		=	LOWORD(Order.siOrderParam1);
			siNPCY		=	HIWORD(Order.siOrderParam1);
			siNPCCode	=	(SI16)Order.siOrderParam2;
			
			lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(siNPCCode);
			if(lpNPCIndexHeader != NULL)
			{
				// NPC�� �����Ÿ��� �Ŵ��� �˻�
				pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

				BOOL bCheck = FALSE;

				// ���  NPC�� ��� �������̸� �Ÿ��� �� �ִ�~
				if (lpNPCIndexHeader->siType == 2 && lpNPCIndexHeader->siDataCode == 1)
				{
					bCheck = ManContactvillage(siNPCX, siNPCY, 1, 1, siHeroX, siHeroY, 7);

				}
				else
				{
					bCheck = ManContactvillage(siNPCX, siNPCY, 1, 1, siHeroX, siHeroY, 8);
				}

				if(bCheck)
				{
					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						if(pMyData->IsWaitSelectNPCResponse() == FALSE)
						{
							// NPC�� ������ �ִ�. NPC�� �����Ѵ�.
							switch(lpNPCIndexHeader->siType)
							{
								case 0:					// �ǹ�
									{
										MapIndexHeader*		lpMapIndexHeader;
										//VillageHeader*		pHeadr;

										if(SI16(pMousePointInfo->GetParam()))
										{
											lpMapIndexHeader = pOnlineMap->MapIndexParser.GetMapIndexData(pOnlineMap->m_MapIndex);

											if(lpNPCIndexHeader && lpMapIndexHeader)
											{
												if(lpMapIndexHeader->uiVillageCode)
												{
													if(lpMapIndexHeader->uiVillageCode == pOnlineVillageInfoData->m_uiVillageCode)
													{
														/*
														pHeadr = pOnlineVillageManager->GetVillageByCode(lpMapIndexHeader->VillageSeries);

														// ������ ������ �ִ�. ���� ������ ����.
														pMyData->SetGotoVillage(TRUE, (pHeadr->siX << 16) | pHeadr->siY);

														// ĳ���Ϳ��� ���� ����� �����ش�.
														pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

														// �� �̻��� ����� �Է��� ���� �ʴ´�.
														pOnlineCharUI->SetAction(FALSE);					

														m_VillageInStruct = lpNPCIndexHeader->siDataCode;
														*/
													}
												}
											}
										}
									}
									break;

								case 1:					// ����Ʈ��
									BOOL		fSameNation;

									fSameNation  = FALSE;
									switch(pMyData->GetFollowerParameterNation(0))
									{
										case 1:
											if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_KOREA)
											{
												fSameNation = TRUE;
											}
											else if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_ALL)
											{
												fSameNation = TRUE;
											}
											break;

										case 10:
											if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_JAPAN)
											{
												fSameNation = TRUE;
											}
											else if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_ALL)
											{
												fSameNation = TRUE;
											}
											break;

										case 100:
											if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_CHINA)
											{
												fSameNation = TRUE;
											}
											else if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_ALL)
											{
												fSameNation = TRUE;
											}
											break;

										case 1000:
											if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_TAIWAN)
											{
												fSameNation = TRUE;
											}
											else if(lpNPCIndexHeader->siNationCode == ON_COUNTRY_ALL)
											{
												fSameNation = TRUE;
											}
											break;
									}


									// Sound
									if(lpNPCIndexHeader->siDataCode == 5)
									{
										PushEffect(EFFECT_SELECT_NPC_HONG, 0, 0);
									}

									if(fSameNation == FALSE)
									{
										pOnlineQuestPlay->SetAction(TRUE, 0, ON_RET_QUEST_SELECTNPC_NOTSAMENTION);
									}
									else
									{
										// ���߰�, Server���� �� NPC���� ���� �ɾ��ٴ� �޼����� ����
										pMyData->SetSelectNPC(TRUE, siNPCCode);
										pMyData->SetWaitSelectNPCResponse(TRUE);
										pOnlineClient->SendRequestQuestSelectNPC(lpNPCIndexHeader->siDataCode, pOnlineVillageSave->GetVillageNum());

										// ��� �Է��� ���´�.
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);
									}
									break;

								case 2:					// ��ſ�
									switch (lpNPCIndexHeader->siDataCode )
									{
									case 1:
										{
											if (!pOnlineReception->IsActive()) pOnlineReception->SetAction(TRUE);
										}
										break;
									case 2:
										{
											// �̹� ��������� ������ 
											if (!pOnlineFieldAuction->IsActive()) {
												// ��ſ� ���� �� ���¸�
												if (pOnlineFieldAuction->IsEntered()) {
													if (pOnlineFieldAuction->IsUnderAuction()) {
														
														if (pOnlineFieldAuction->CanBid()) 
														{
															if(pOnlineFieldAuction->CanWeight())
															{
																pOnlineFieldAuction->SetAction(TRUE);
															}
															else
															{
																this->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", pOnlineText->Get(ON_TEXT_AUCTION_NO_MORE_ITEMSPACE) );
															}
														}
														else
														{
															this->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", pOnlineText->Get(ON_TEXT_AUCTION_NO_INVEN_EMPTY_SLOT));
														}
													}
												}
											}
											
										}
										break;
									}
									break;
								case 3:					// ��������
									{
										switch (lpNPCIndexHeader->siDataCode )
										{
										case 0:
											{
												if (!pOnlineSmith->IsActive()) pOnlineSmith->SetAction(TRUE);
											}
											break;
										}
									}
									break;
							}
						}
					}
				}				
			}
			else
			{			
				// ��������� ������.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;
	}	
}

BOOL ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis)
{
	// ������ �����ϰ� ��, ��, ��, �Ϸ� dis��ŭ ������ �ִ� ���ڸ� ��������.
	if( (cx>=vx-dis && cx<=vx+wi+dis-1) && (cy>=vy-dis && cy<=vy+hi+dis-1) ) return TRUE;
	else return FALSE;
}
