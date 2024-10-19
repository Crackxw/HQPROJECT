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

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 내 캐릭터의 명령을 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::ProcessMyCharOrder()
{
	OnOrder		Order;	
	SI16		siHeroX, siHeroY, siHeroRX, siHeroRY, siAttackCharX, siAttackCharY;	
	SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

//	if (FALSE == pOnlineCharUI->IsAction())
//		return;

	// 현재 내 캐릭터가 수행하는명령을 얻어온다.
	Order	=	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());

	switch(Order.siOrder)
	{	
	case ON_ORDER_ATTACK:
		// 우선 내가 공격하려는 캐릭터가 실제로 있는지 검사한다.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// 나의 위치와 내가 공격 하려는 캐릭터의 위치를 얻어온다.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);


			// 두 캐릭터 간의 사이가 가까운지 본다.(즉 전투를 할 수 있는 상황인지 본다.)
			// 그리고 혹시 벌써 전투를 한다는 메시지를 보내가지고 그 응답을 기다리는 상황인지도 체크한다.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, 1) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// 싸울 수 있는 상황이다. 서버에게 메시지를 보낸다.
					// 다른 플레이어와 전투를 한다 하고 서버로부터의 응답을 기다린다고 설정한다.
					pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// 캐릭터에게 정지 명령을 내려준다.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// 더 이상의 사용자 입력을 받지 않는다.
					pOnlineCharUI->SetAction(FALSE);
				}	
			}					
		}	
		break;
	case ON_ORDER_FIELDATTACK_READY:
		// 우선 내가 공격하려는 캐릭터가 실제로 있는지 검사한다.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// 나의 위치와 내가 공격 하려는 캐릭터의 위치를 얻어온다.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// 두 캐릭터 간의 사이가 가까운지 본다.(즉 전투를 할 수 있는 상황인지 본다.)
			// 그리고 혹시 벌써 전투를 한다는 메시지를 보내가지고 그 응답을 기다리는 상황인지도 체크한다.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				// robypark 2004/10/27 16:3
				// 공격자 캐릭터 종류 정보 인덱스
				UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID());

				// robypark 2004/10/27 16:3
				// 두 캐릭터 간의 공격가능 거리 검사를 KindInfo에서 설정한 값을 이용하도록 수정.
				// ManContactvillage(x1, y1, 1, 1, x2, y2, 1) => ManContactvillage(x1, y1, 1, 1, x2, y2, KI[].GetAttackRange())
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, KI[uiKindInfoIdx_Attacker].GetAttackRange(0)) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// 싸울 수 있는 상황이다. 서버에게 메시지를 보낸다.
					// 다른 플레이어와 전투를 한다 하고 서버로부터의 응답을 기다린다고 설정한다.
					/*pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// 캐릭터에게 정지 명령을 내려준다.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// 더 이상의 사용자 입력을 받지 않는다.
					pOnlineCharUI->SetAction(FALSE);*/

					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// 더 이상의 사용자 입력을 받지 않는다.
						pOnlineCharUI->Init();
						pOnlineCharUI->SetAction(FALSE);
						
						// Server로 메세지 전송
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
			// 거래 중이거나
			if (pOnlineTrade->IsActive())
			{
				// 오브젝트 사용 중지, 대기모드 설정
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}

			// 거래 요청 메시지 박스가 활성화 되었다면
			if (pOnlineTrade->m_pMsgBox->IsActive())
			{
				// 오브젝트 사용 중지, 대기모드 설정
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}
			//-------------------------------------------------//

			if(siObjectID != 0)
			{
				// 지금 마을에 들어가는 중인지 알아본다.
				// 마을인지 알아본다.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// 마을인지 포탈인지 구분
						if(pIOnlineObject->IsPortal(siObjectID))
						{
							// 포탈이다.
							// 나의 위치를 구한다.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// 그 오브젝트의 본 위치를 구한다.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// 그 오브젝트의 사이즈를 구한다.
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
								// 마을에 근접해 있다. 마을 안으로 들어간다.										
								pMyData->SetGotoPortal(TRUE, pIOnlineObject->GetPortalID(siObjectID));

								// 캐릭터에게 정지 명령을 내려준다.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// 더 이상의 사용자 입력을 받지 않는다.
								pOnlineCharUI->SetAction(FALSE);					
							}
						}
						else
						{
							// 마을이다.
							// 나의 위치를 구한다.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// 그 오브젝트의 본 위치를 구한다.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// 그 오브젝트의 사이즈를 구한다.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

							if(ManContactvillage(siVillageX - siVillageXsize / 2, 
											siVillageY - siVillageYsize / 2, 
											siVillageXsize, siVillageYsize, siHeroX, siHeroY, 1) == TRUE)
							{
								
								// 마을에 근접해 있다.
								// 마을 안으로 들어간다.										
								pMyData->SetGotoVillage(TRUE, pIOnlineObject->GetVillageCode(siObjectID));

								// 캐릭터에게 정지 명령을 내려준다.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// 더 이상의 사용자 입력을 받지 않는다.
								pOnlineCharUI->SetAction(FALSE);					

								m_VillageInStruct = -1;
							}				
						}
					}
				}
			}
			else
			{			
				// 정지명령을 내린다.			
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
			// 거래 중이거나
			if (pOnlineTrade->IsActive())
			{
				// 오브젝트 사용 중지, 대기모드 설정
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}

			// 거래 요청 메시지 박스가 활성화 되었다면
			if (pOnlineTrade->m_pMsgBox->IsActive())
			{
				// 오브젝트 사용 중지, 대기모드 설정
				pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
				break;
			}
			//-------------------------------------------------//

			if(siObjectID != 0)
			{
				// 지금 마을에 들어가는 중인지 알아본다.
				// 마을인지 알아본다.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// 마을이다.
						// 나의 위치를 구한다.
						pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

						// 그 오브젝트의 본 위치를 구한다.
						pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

						// 그 오브젝트의 사이즈를 구한다.
						pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

						if(ManContactvillage(siVillageX - siVillageXsize / 2, 
										siVillageY - siVillageYsize / 2, 
										siVillageXsize, siVillageYsize, siHeroX, siHeroY, 1) == TRUE)
						{
							if (0 != m_sVillageDefenceInterface.m_uiVillageCode)
							{
								// 캐릭터에게 정지 명령을 내려준다.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
								break;
							}
							
							// 마을에 근접해 있다.
							// robypark 2004/12/12 14:44
							//  공성전 시간이며
							if (IsInTimeSiegeWarfare())
							{
								// 수성하기 인터페이스로 진입 요청
								pOnlineClient->SendEnterVillageDefenceInterface(pIOnlineObject->GetVillageCode(siObjectID));

								m_sVillageDefenceInterface.m_uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

								// 캐릭터에게 정지 명령을 내려준다.
								pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

								// 더 이상의 사용자 입력을 받지 않는다.
								pOnlineCharUI->Init();
								pOnlineCharUI->SetAction(FALSE);

								m_VillageInStruct = -1;
							}
							else
							{
								// 캐릭터에게 정지 명령을 내려준다.
								pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
								break;
							}
						}				
					}
				}
			}
			else
			{			
				// 정지명령을 내린다.			
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
				// 지금 마을에 들어가는 중인지 알아본다.
				// 마을인지 알아본다.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// 마을인지 포탈인지 구분
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// 마을이다.
							// 나의 위치를 구한다.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// 그 오브젝트의 본 위치를 구한다.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// 그 오브젝트의 사이즈를 구한다.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

							// robypark 2004/10/27 16:3
							// 공격자 캐릭터 종류 정보 인덱스
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
										// 마을에 근접해 있다. 마을을 공격한다.
										// 캐릭터에게 공격 명령을 내려준다.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION, siVillageX, siVillageY);

										// 더 이상의 사용자 입력을 받지 않는다.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server로 메세지 전송
										// robypark 2004/11/4 13:56	
										// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
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
				// 정지명령을 내린다.			
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
				// 지금 마을에 들어가는 중인지 알아본다.
				// 마을인지 알아본다.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// 마을인지 포탈인지 구분
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// 마을이다.
							// 나의 위치를 구한다.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// 그 오브젝트의 본 위치를 구한다.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// 그 오브젝트의 사이즈를 구한다.
							pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					
							
							// robypark 2004/10/27 16:3
							// 공격자 캐릭터 종류 정보 인덱스
							UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID());

							// robypark 2004/10/27 16:3
							// 마을과 캐릭터 간의 공격가능 거리 검사를 KindInfo에서 설정한 값을 이용하도록 수정.
							// ManContactvillage(x1, y1, 1, 1, x2, y2, 1) => ManContactvillage(x1, y1, 1, 1, x2, y2, KI[].GetAttackRange())
							//뒤에 붙은 5라는 숫자가 마을과 캐릭터간에 떨어져 있는 거리를 나타낸다.
							//캐릭터의 특성에 따라 캐릭터에서 불러서 사용하면 됨.
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
										// 마을에 근접해 있다. 마을을 공격한다.
										// 캐릭터에게 공격 명령을 내려준다.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

										// 더 이상의 사용자 입력을 받지 않는다.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server로 메세지 전송
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
				// 정지명령을 내린다.			
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

			// 우선 그 아이템이 그 위치에 있는지 본다.
			uiItemID	=	pOnlineMap->GetItemID(siItemX, siItemY);			

			// 내가 원래 주울려고 했던 아이템과 같은 아이템이여야 한다.
			if(uiItemID == UI32(Order.siOrderParam2))
			{
				// 나의 위치를 구한다.
				pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
				pIOnlineChar->GetRPos(pMyData->GetMyCharUniqueID(), &siHeroRX, &siHeroRY);

				// 아이템을 주울때 캐릭터과 타일 가운데 았을 때 수행한다.
				// 안그러면 이상해보인다.			
				if(siHeroRX == 0 && siHeroRY == 0)
				{
					// 아이템 가까이에 내가 있는지본다.
					if(ManContactvillage(siItemX, siItemY, 1, 1, siHeroX, siHeroY, 1) == TRUE)
					{
						// 서버에게 아이템을 줍는다고 요청한다.
						// Inventory를 켠다.
/*						if(pOnlineInventory->IsActive() == FALSE) 
						{
							pOnlineInventory->SetItemPickUp(TRUE);
							pOnlineInventory->SetActive(TRUE);
						}
*/
						pOnlineInventory->SetItemPickUp(TRUE);

						pOnlineInventory->SetFieldItemDrag(uiItemID, siItemX, siItemY);

						pMyData->SetWaitPickUpItemResponse(TRUE);			
						
						// 캐릭터에게 정지 명령을 내려준다.
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// 더 이상의 사용자 입력을 받지 않는다.
						pOnlineCharUI->SetAction(FALSE);
					}				
				}
			}
			else
			{
				// 아이템이 없다.
				// 정지명령을 내린다.						
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);				
			}			
		}
		break;
	// robypark 2004/11/9 17:16
	// 공성전 유닛 보급수레에 공성전 유닛이 보급품을 요청 처리
	case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON:
		// 우선 캐릭터가 실제로 있는지 검사한다.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// 나의 위치와 보급수레 캐릭터의 위치를 얻어온다.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// 두 캐릭터 간의 사이가 가까운지 본다.(즉 전투를 할 수 있는 상황인지 본다.)
			// 그리고 혹시 벌써 전투를 한다는 메시지를 보내가지고 그 응답을 기다리는 상황인지도 체크한다.
			if(pMyData->IsWaitBattleResponse() == FALSE)
			{
				if(ManContactvillage(siAttackCharX, siAttackCharY, 1, 1, siHeroX, siHeroY, VALID_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == TRUE)
				{	
					if(pOnlineTrade->IsWait())
					{
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
						break;
					}
					// 싸울 수 있는 상황이다. 서버에게 메시지를 보낸다.
					// 다른 플레이어와 전투를 한다 하고 서버로부터의 응답을 기다린다고 설정한다.
					/*pOnlineClient->SendBattle(Order.siOrderParam1);		
					pMyData->SetWaitBattleResponse(TRUE);

					// 캐릭터에게 정지 명령을 내려준다.
					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// 더 이상의 사용자 입력을 받지 않는다.
					pOnlineCharUI->SetAction(FALSE);*/

					
					if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) == FALSE)
					{
						pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
						pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

						// 더 이상의 사용자 입력을 받지 않는다.
						//pOnlineCharUI->Init();
						//pOnlineCharUI->SetAction(FALSE);
						
						// Server로 메세지 전송
						pOnlineClient->SendSiegeWarfareSupplyGoodsFromSupplyWagon(Order.siOrderParam1, 0);
					}
				}
//				else
//					pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_MOVE, siAttackCharX, siAttackCharY);
			}	
		}
		break;
	// robypark 2004/11/9 18:29
	// 공성전 유닛 보급수레가 마을에 보급품을 요청 처리
	case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE:
		{
			SI32		siObjectID;
			
			siObjectID	=	pOnlineMap->GetObjectID(Order.siOrderParam1, Order.siOrderParam2);			
			
			if(siObjectID != 0)
			{
				// 지금 마을에 들어가는 중인지 알아본다.
				// 마을인지 알아본다.
				if(pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					if(pMyData->IsWaitGotoPortalResponse() == FALSE)
					{
						// 마을인지 포탈인지 구분
						if(pIOnlineObject->IsPortal(siObjectID) == FALSE)
						{
							// 마을이다.
							// 나의 위치를 구한다.
							pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

							// 그 오브젝트의 본 위치를 구한다.
							pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

							// 그 오브젝트의 사이즈를 구한다.
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
										// 마을에 근접해 있다. 마을을 공격한다.
										// 캐릭터에게 공격 명령을 내려준다.
										pIOnlineChar->ClearOrder(pMyData->GetMyCharUniqueID());
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

										// 더 이상의 사용자 입력을 받지 않는다.
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);

										// Server로 메세지 전송
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
				// 정지명령을 내린다.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;
	// robypark 2005/1/25 19:58
	// 공성전 유닛 보급수레가 공성전 유닛을 치료 시도
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
		// 우선 캐릭터가 실제로 있는지 검사한다.
		if( pIOnlineChar->IsExistUniqueID(Order.siOrderParam1) == TRUE )
		{
			// 나의 위치와 해당 캐릭터의 위치를 얻어온다.
			pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			pIOnlineChar->GetPos(Order.siOrderParam1, &siAttackCharX, &siAttackCharY);
			
			if(pOnlineBooth->IsActionIn())
				pOnlineBooth->SetActionIn(FALSE);

			// 두 캐릭터 간의 사이가 가까운지 본다.(즉 전투를 할 수 있는 상황인지 본다.)
			// 그리고 혹시 벌써 전투를 한다는 메시지를 보내가지고 그 응답을 기다리는 상황인지도 체크한다.
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

						// 더 이상의 사용자 입력을 받지 않는다.
						pOnlineCharUI->Init();
						//pOnlineCharUI->SetAction(FALSE);
						
						// Server로 메세지 전송
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
				// NPC와 일정거리가 돼는지 검사
				pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

				BOOL bCheck = FALSE;

				// 경매  NPC고 경매 진행인이면 거리가 더 멀다~
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
							// NPC에 근접해 있다. NPC를 가동한다.
							switch(lpNPCIndexHeader->siType)
							{
								case 0:					// 건물
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

														// 마을에 근접해 있다. 마을 안으로 들어간다.
														pMyData->SetGotoVillage(TRUE, (pHeadr->siX << 16) | pHeadr->siY);

														// 캐릭터에게 정지 명령을 내려준다.
														pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

														// 더 이상의 사용자 입력을 받지 않는다.
														pOnlineCharUI->SetAction(FALSE);					

														m_VillageInStruct = lpNPCIndexHeader->siDataCode;
														*/
													}
												}
											}
										}
									}
									break;

								case 1:					// 퀘스트용
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
										// 멈추고, Server에게 이 NPC에게 말을 걸었다는 메세지를 전송
										pMyData->SetSelectNPC(TRUE, siNPCCode);
										pMyData->SetWaitSelectNPCResponse(TRUE);
										pOnlineClient->SendRequestQuestSelectNPC(lpNPCIndexHeader->siDataCode, pOnlineVillageSave->GetVillageNum());

										// 모든 입력을 막는다.
										pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
										pOnlineCharUI->Init();
										pOnlineCharUI->SetAction(FALSE);
									}
									break;

								case 2:					// 경매용
									switch (lpNPCIndexHeader->siDataCode )
									{
									case 1:
										{
											if (!pOnlineReception->IsActive()) pOnlineReception->SetAction(TRUE);
										}
										break;
									case 2:
										{
											// 이미 띄워져있지 않으며 
											if (!pOnlineFieldAuction->IsActive()) {
												// 경매에 참여 한 상태면
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
								case 3:					// 대장장이
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
				// 정지명령을 내린다.			
				pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
			}
		}
		break;
	}	
}

BOOL ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis)
{
	// 마을을 포함하고 좌, 우, 상, 하로 dis만큼 떨어져 있는 상자를 생각하자.
	if( (cx>=vx-dis && cx<=vx+wi+dis-1) && (cy>=vy-dis && cy<=vy+hi+dis-1) ) return TRUE;
	else return FALSE;
}
