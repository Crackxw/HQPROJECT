// Latest Update 2002/6/26, 10:22 

#include "SOMain.h"
#include <onlinebattlemsg.h>

#include "SOWorld.h"
#include "SOBaseChar.h"
#include "SOPlayer.h"
#include "../Element.h"
#include "game.h"
#include "../Battle.h"
#include "SODBStructure.h"


void LogPrintf(const char *format,...)
{	
	if(FALSE)
	{
		if(strlen(format) < 200)
		{
			char buf[300];
			wvsprintf(buf, format, (LPSTR)(&format+1) );
			printf(buf);
		}
	}
}

//---------------------------------------------------------------------------
// _ChatMsg ���� 
//---------------------------------------------------------------------------
VOID _ChatMsg::SetDataSize(SI32 size)
{
	Data  = ((Data & 0x00ffffff) | (size<<28)) ;
}

SI32 _ChatMsg::GetDataSize()
{
	return ((Data & 0xff000000) >> 28) ;
}

VOID _ChatMsg::CopyStrData(char* str)
{
	strcpy(StrData, str);
}

BOOL _ChatMsg::IsAlly()
{
	if(((Data & 0x000f0000) >> 16))return TRUE;

	return FALSE;
}

void _ChatMsg::SetAlly(char allymode)
{
	Data = ((Data & 0xfff0ffff) | (allymode << 16)) ;
}



void _PlayerInfo::Init(SHORT pnumber)
{
	Country      = COUNTRY_NO;
	PlayerNumber = (unsigned char)pnumber;
	Host         = BY_COMPUTER;

	PlayerGroup  = PlayerNumber;

	MyBit=(1<<pnumber);
}

void _PlayerInfo::ChangePlayerGroup(SHORT pgroup)
{
	PlayerGroup=(unsigned char)pgroup;
}


bool _Game::Init(CBattle *pbattle)
{
	SI32 i;
	LastClientCheckClock = 0;
	ZeroMemory(this, sizeof(_Game));

	pBattle = pbattle;

	DecideTransVary(300);

	int j=0;
	//-----------------------------------------------------------------------------------------------
	for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{		
		BaseChar* pChar = pBattle->GetPlayer(i);

		if(pChar)
		{
			PlayersdpId.dpId[j] = pChar->uiAccount;	
			++j;
		}
	}
	//-----------------------------------------------------------------------------------------------


	for( i = 0; i < MAX_MULTI_PLAYER; i++ )
	{		
		PlayersdpId.Player[i].Init(i);

		// ��ȿ�� �÷����̾��� ��� 
		if(PlayersdpId.dpId[i]) 
		{
			PlayersdpId.Player[i].Host		= BY_PLAYER;
			PlayersdpId.Player[i].Country	= COUNTRY_KOREA;     
		}
	}

	UserPlayerNumber = MAX_MULTI_PLAYER - 1;

	// ���� ���� �޽��� �ʱ�ȭ 
	ZeroMemory(&GoOutMsg,       sizeof(GOOUTMSG));
	GoOutMsg.dwType   = GAME_MSG_GOOUT;

	return TRUE;
}

BOOL _Game::PushData(char *data, UI32 length)
{
	if( DataIndex >= MAX_DATA_NUMBER || DataIndex < 0 )	return FALSE;

	memcpy(Data[DataIndex], data, length);
	Length[DataIndex]	= length;

	DataIndex++;
	
	return TRUE;
}

BOOL _Game::PopData(char *data, UI32 &length)
{
	SI32 i;
	if( DataIndex <= 0 )	return FALSE;

	// ���ۿ� ����� ���� ������. 
	memcpy(data, Data[0], Length[0]);
	length = Length[0];

	for( i=1; i<=DataIndex; i++ )
	{
		memcpy(Data[i-1], Data[i], Length[i]);
		Length[i-1] = Length[i];
	}
	DataIndex--;
	
	return TRUE;
}


// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
void _Game::DecideTransVary(SI32 latency)
{
	// �ִ� ������ ���� 	
	DPMaxFrameDiff = ( latency / DEFAULT_MULTI_PLAY_DELAY_TIME) + 1;
	if(DPMaxFrameDiff > MAX_FRAME_DIFF) DPMaxFrameDiff = MAX_FRAME_DIFF;
	if(DPMaxFrameDiff < 4)DPMaxFrameDiff = 4;
	
	// �޽����� �����ϴ� ���� 
	DPFrameRateForTrans = DPMaxFrameDiff / 3;
	if(DPFrameRateForTrans < 1)DPFrameRateForTrans = 1;				
}


SI32 _Game::GetIDFromInClientNoMsg(LPCLIENTNOMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0x00f00000) >> 20);
	}

	return 0;
}


UI32 _Game::GetFrameInClientNoMsg(LPCLIENTNOMSG lpmsg)
{
	if(lpmsg)
	{
		return (lpmsg->Data & 0x000fffff);
	}

	return 0;
}

UI32 _Game::GetSignalInClientNoMsg(LPCLIENTNOMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0xf0000000) >> 28);
	}

	return 0;
}

BOOL _Game::SetFrameInMyMsg(LPMYMSG lpmsg, UI32 frame)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xfff00000) | frame) ;
		return TRUE;
	}

	return FALSE;
}

UI32 _Game::GetFrameInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return (lpmsg->Data & 0x000fffff);
	}

	return 0;
}

BOOL _Game::SetDataSizeInMyMsg(LPMYMSG lpmsg,  SI32 size)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0x00ffffff) | (size<<24)) ;
		return TRUE;
	}

	return FALSE;
}

SI32 _Game::GetDataSizeInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0xff000000) >> 24) ;
	}

	return 0;
}

BOOL _Game::SetIDFromInMyMsg(LPMYMSG lpmsg, SI32 idfrom)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xff0fffff) | (idfrom<<20)) ;
		return TRUE;
	}

	return FALSE;
}

SI32 _Game::GetIDFromInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0x00f00000) >> 20);
	}

	return 0;
}

BOOL _Game::SetFrameInNoMsg(LPNOMSG lpmsg, UI32 frame)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xfff00000) | frame) ;
		return TRUE;
	}

	return FALSE;
}

UI32 _Game::GetFrameInNoMsg(LPNOMSG lpmsg)
{
	if(lpmsg)
	{
		return (lpmsg->Data & 0x000fffff);
	}

	return 0;
}

BOOL _Game::SetIDFromInNoMsg(LPNOMSG lpmsg, SI32 idfrom)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xff0fffff) | (idfrom<<20)) ;
		return TRUE;
	}

	return FALSE;
}

BOOL _Game::SetOldFrameVaryInNoMsg(LPNOMSG lpmsg, SI32 oldframebit, BOOL mode)
{
	SI32 oldframeinfo = 0 ;

	if(oldframebit<0 || oldframebit>=MAX_NOMSG_BACKUP_STORAGE)
	{
		//clGrp.Error("2394620d", "[%d]", oldframebit);
		return FALSE;
	}

	if(lpmsg)
	{
		// oldframe�� ���õ� ������ �����Ѵ�. 
		oldframeinfo = ((lpmsg->Data & 0xff000000) >> 24);

		if(mode == TRUE)
		{
			oldframeinfo = (oldframeinfo | (1<<oldframebit));
		}
		else
		{
			oldframeinfo = (oldframeinfo & (~(1<<oldframebit)) );
		}

		lpmsg->Data  = ((lpmsg->Data & 0x00ffffff) | (oldframeinfo<<24)) ;

		return TRUE;
	}

	return FALSE;
}


// ���� �޽����� �������α׷��� ���� ���̸� �װͿ� ���� ó���� �Ѵ�.
void _Game::DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize)
{
	
	switch(lpMsg->dwType)
	{
	
	// �� �޽����� ȣ��Ʈ���� Ŭ���̾�Ʈ�κ��� �ް� �ȴ�. �׷��� ȣ��Ʈ�� �� Ŭ���̾�Ʈ�� ���� �����Ȳ(������)�� 
	// �ľ��ϰ� �ʹ� ��߳��� �ʰ� ������ �� �ֵ��� �����Ѵ�. 
	case GAME_MSG_CLIENTNO:
		{	
			SI32 id = 0;
			LPCLIENTNOMSG lpClientNoMsg = (LPCLIENTNOMSG) lpMsg;
			
			// ��� �÷��̾�κ��� ���� �޽��� ���� Ȯ���Ѵ�. 
			id = GetIDFromInClientNoMsg(lpClientNoMsg);
			
			if(PlayersdpId.dpId[id])
			{
				if(CurrentClock > DPCheck[id])
				DPCheck[id] = CurrentClock;
			}	

		}
		break;

	case GAME_MSG_CHAT:
		{
			LogPrintf("-- GAME_MSG_CHAT\n");

			// ä�� �޽����� �޴´�� �ٽ� Ŭ���̾�Ʈ�� ������. 
			pBattle->SendToAll((char*)lpMsg, dwMsgSize);
			
		}
		break;


	case GAME_MSG_CLIENT_ORDER:
		{			
			int				id;
			LPMYMSG				lpClientOrderMsg;
			MYMSG				MyMsg;
			_OrderData          temporder;
			
			
			lpClientOrderMsg = (LPMYMSG) lpMsg;
			
			id = GetIDFromInMyMsg(lpClientOrderMsg);

			if(PlayersdpId.dpId[id])
			{
				if(CurrentClock > DPCheck[id])
				DPCheck[id] = CurrentClock;
			}	

			
			CopyMemory(&MyMsg, lpClientOrderMsg, sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpClientOrderMsg));
			CopyMemory(&temporder, &MyMsg.data, sizeof(_OrderData));
			
			// ���� ȣ��Ʈ�̸� ó���� ���� ����� ���ۿ� �ִ´�.
			// �� ���ۿ� �־��� ����� ����� �ٸ� ����ڵ鿡�Է� ���۵Ǹ� ȣ��Ʈ �ڽ��� 
			// ������ ����� �����Ѵ�.

			
			PushUserOrderBuffer(temporder);

			// ����ڰ� ������ �����ٴ� ���. 
			if(temporder.ODI.Order == 86/*ORDER_QUIT*/)
			{				
				LogPrintf("-- GAME_MSG_GOOUT\n");
				LogPrintf("%d(%d) ������ ������ ���� �Ͽ����ϴ� \n", id, PlayersdpId.dpId[id]);

				// ���� ���� ��Ų��. 
				GoOutMsg.PlayerNumber = id;
				SendGameMessage((LPGWMSG_GENERIC)&GoOutMsg);
				DoLogoutAction( PlayersdpId.dpId[id]);

				
				// ���������� �÷��̾ �����ٴ� ����� �˸���. 
				//pBattleGame->EndBattlePlayer(id);				
			}
			
		}
		break;

	case GAME_MSG_UPDATE:
		{
			int					id;
			LPMYMSG				lpClientOrderMsg;
			MYMSG				MyMsg;
			_OnlineBattleMsg	obmsg;
			_OnlineBattleMsgQue obmq;

			SI32 playerid, charid, value;


			lpClientOrderMsg = (LPMYMSG) lpMsg;
			
			id = GetIDFromInMyMsg(lpClientOrderMsg);

			if(PlayersdpId.dpId[id])
			{
				if(CurrentClock > DPCheck[id])
				DPCheck[id] = CurrentClock;
			}	

			
			CopyMemory(&MyMsg, lpClientOrderMsg, sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpClientOrderMsg));
			CopyMemory(&obmsg, &MyMsg.data, sizeof(_OnlineBattleMsg));

			// ���Ӽ����� �����ϱ� ���ؼ� ť�� �־���´�. 
			switch(obmsg.GetType())
			{
				
			case ONLINE_BATTLE_MSG_DEAD:
				{
					LogPrintf("-- ONLINE_BATTLE_MSG_DEAD\n");
				}
				break;



			case ONLINE_BATTLE_MSG_LIFE:
				{
					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(&obmsg, &playerid, &charid, &value);

					
					int index = IsValidFollower(playerid, charid);
					if(index == -1) break;


					pBattle->GetFollowerParam(index,charid)->SetLife(value);						
					LogPrintf("-- %d�� %d ĳ���� Life %d�� ����\n", playerid, charid, value);								  					
				}
				break;



			case ONLINE_BATTLE_MSG_EXP:
				if( IsEvent() )	break;

				{
					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(&obmsg, &playerid, &charid, &value);
					
					
					
					int index = IsValidFollower(playerid, charid);
					if(index == -1) break;

					
					_ImportantParameta* pParam  = pBattle->GetFollowerParam(index,charid);

					LogPrintf("-- %d�� %d ĳ���� EXP %d ����\n", playerid, charid, value);												  
					if(pParam->IncreaseExp(value) == TRUE)
					{
						LogPrintf("-- %d�� %d ĳ���� LevelUp!\n", playerid, charid);
						pParam->SetLevel(pParam->GetLevel()+1);
						pParam->SetBonus(pParam->GetBonus()+4);
						pParam->SetLife(pParam->GetMaxLifeByIP());
						pParam->SetMana(pParam->GetMaxManaByIP(), 5);
					}						
									
				}
				break;



			case ONLINE_BATTLE_MSG_MANA:
				{
					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(&obmsg, &playerid, &charid, &value);

					int index = IsValidFollower(playerid, charid);
					if(index == -1) break;



					LogPrintf("-- %d�� %d ĳ���� Mana %d�� ����\n", playerid, charid, value);					
					pBattle->GetFollowerParam(index,charid)->SetMana(value, 6);					
				}
				break;



			case ONLINE_BATTLE_MSG_LOGOUT:
				{
					LogPrintf("-- ONLINE_BATTLE_MSG_LOGOUT\n");

					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LOGOUT(&obmsg, &playerid);


					if(GetPlayerIndex(playerid) == -1) break;

					// ���� ���� ��Ų��. 
					GoOutMsg.PlayerNumber = playerid;
					SendGameMessage((LPGWMSG_GENERIC)&GoOutMsg);
					DoLogoutAction( PlayersdpId.dpId[playerid]);					

				}
				break;

			case ONLINE_BATTLE_MSG_KILL:
				{					
					LogPrintf("-- ONLINE_BATTLE_MSG_KILL\n");

					SI32 victimplayerid;
					SI32 victimcharid;

					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(&obmsg, &playerid, &charid, &victimplayerid, &victimcharid);

					
					int winnerIndex = IsValidFollower(playerid, charid);
					if(winnerIndex == -1) break;

					int victimIndex = IsValidFollower(victimplayerid, victimcharid);
					if(victimIndex == -1) break;

					BaseChar*   winner = pBattle->GetPlayer(winnerIndex);
					BaseChar*   victim = pBattle->GetPlayer(victimIndex);
					if(winner == NULL || victim == NULL) break;

					MyItemData* winnerItems  = winner->GetItemList(charid);
					MyItemData* victimItems  = victim->GetItemList(victimcharid);
					if(winnerItems == NULL || victimItems == NULL) break; 

					
					// 1. �����ڷκ��� �������� ������ �ִ� ���Ը���� ���´� 
					//--------------------------------------------------------------------------------									
					int		numItem = 0;
					char	haveItemSlot[ON_MYDATA_MAX_ITEM_NUM];

					for(int i=0; i < ON_MYDATA_MAX_ITEM_NUM; ++i)
					{						
						if(victimItems[i].uiID != 0)
						{
							haveItemSlot[numItem] = i;
							++numItem;																				
						}
					}

					MyItemData *pStealItemData;

					
					// 2. �������� ������ ���Ը���� ��� ���� �ʴٸ� �����ϰ� ���� �������� �ϳ� �����Ѵ� 
					//--------------------------------------------------------------------------------
					if( numItem > 0)
					{
											
						int steelSlot = haveItemSlot[rand() % numItem];
						MyItemData steelItem = victimItems[steelSlot];

						for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
						{
							if(steelItem.uiID == ((SOPlayer*)winner)->pMyWorld->m_FieldBattleParser.GetItemID(j)) break;
						}

						// 3. ���� �������� ���ڿ��� �߰��� �ټ� �ִ��� �˾� ���� 
						//--------------------------------------------------------------------------------												
						if( (pStealItemData = winner->AddItemNoQuery( charid, steelItem.uiID, 1 , 0 )) == NULL )
						{					
							charid = -1;
							for(int i=0; i < ON_MAX_FOLLOWER_NUM; ++i)
							{
								if( (pStealItemData = winner->AddItemNoQuery( i, steelItem.uiID, 1 , 0)) )
								{
									charid = i; 
									break;
								}
							}
						}

						// 4. �߰��� �������� ��쿡�� �����ڿ��Լ� �������� �����ϰ� �װ��� �˸��� 
						//--------------------------------------------------------------------------------												
						if(charid != -1)
						{
							victim->DelItemNoQuery( victimcharid, steelItem.uiID, 1, steelItem.siPosInInventory );											

							// �Ѵ� �÷��̾��� ��Ż ������ ����
							if (ISPLAYER(playerid) && ISPLAYER(victimplayerid))
							{
								DBIKRobItemAtBattle DBInfo;

								DBInfo.siToDBAccount = ((SOPlayer*)winner)->stAccountInfo.siDBAccount;
								DBInfo.uiToDBSlot = ((SOPlayer*)winner)->stAccountInfo.uiSlot;
								DBInfo.uiToMercenaryID = charid;								
								DBInfo.uiToItemSlot = pStealItemData->siPosInInventory;									

								DBInfo.siFromDBAccount = ((SOPlayer*)victim)->stAccountInfo.siDBAccount;
								DBInfo.uiFromDBSlot = ((SOPlayer*)victim)->stAccountInfo.uiSlot;
								DBInfo.uiFromMercenaryID = victimcharid;
									
								

								DBInfo.uiItemID = steelItem.uiID;

								((SOPlayer*)winner)->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_ROBITEMATBATTLE,&DBInfo,sizeof(DBInfo));
									
									
								
							}
							else {
								
								// ���λ���� �÷��̾��� ������ ȹ�� ������ ����
								if (ISPLAYER(playerid))
								{
									DBIKGetItemAtBattle DBInfo;
									
									DBInfo.siDBAccount			=	((SOPlayer*)winner)->stAccountInfo.siDBAccount;
									DBInfo.uiDBSlot				=	((SOPlayer*)winner)->stAccountInfo.uiSlot;
									DBInfo.uiItemID				=	steelItem.uiID;
									DBInfo.uiMercenaryID		=	charid;
									DBInfo.uiItemSlot			=	pStealItemData->siPosInInventory;									
									DBInfo.uiVictimCharKind		=	victim->IFollower.GetFollowerParameterKind( victimcharid );
									((SOPlayer*)winner)->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GETITEMATBATTLE,&DBInfo,sizeof(DBInfo));
									
									
								}
								
								// ��������� �÷��̾��� ������ ���� ������ ����
								if (ISPLAYER(victimplayerid))
								{
									DBIKDieAtBattleLoseItem DBInfo;
									
									DBInfo.siDBAccount = ((SOPlayer*)victim)->stAccountInfo.siDBAccount;
									DBInfo.uiDBSlot = ((SOPlayer*)victim)->stAccountInfo.uiSlot;
									DBInfo.uiMercenaryID = victimcharid;
									DBInfo.uiItemID = steelItem.uiID;
									
									
									((SOPlayer*)victim)->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DIEATBATTLELOSEITEM,&DBInfo,sizeof(DBInfo));
									
								}
							}

							PushBattleItemInfo( winnerIndex, charid, victimIndex, victimcharid, steelItem.uiID );
						}
					}

					// 5. �����ڸ� ��� �Ѵ� ( �¸��ڰ� �����̰� , �����ڰ� ������ ��쿡��.... )
					//------------------------------------------------------------------------------------
					if( ISPLAYER(playerid) && ISMONSTER(victimplayerid) )
					{
						((SOPlayer*)winner)->Quest_AddQuestObject_KillMonster( victim->uiKind, 1 );
					}

					

					// 6. �����ڸ� �����Ѵ� 
					//------------------------------------------------------------------------------------
					
					// �����ڰ� �����̰� ���ΰ�ĳ���� �� ��� 
					if( ISPLAYER(victimplayerid) )
					{						
						if(victimcharid == 0)
						{
							LogPrintf("(���ΰ�)"); 
							
							MONEY money = ((SOPlayer*)victim)->siMoney;

							// �׾ ���海.
							MONEY moneypenalty = (money  * PENALTY_MONEY_OF_DEADTH/100);

							LogPrintf("(%d ---> ", money);
								
							// ���� �谨�Ѵ� 
							((SOPlayer*)victim)->siMoney = money - moneypenalty;							

							// ������ ���� ����

							DBIKDieAtBattleLoseMoney DBInfo;

							DBInfo.siDBAccount = ((SOPlayer*)victim)->stAccountInfo.siDBAccount;
							DBInfo.uiDBSlot = ((SOPlayer*)victim)->stAccountInfo.uiSlot;

							DBInfo.moMoney = moneypenalty;

							((SOPlayer*)victim)->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DIEATBATTLELOSEMONEY,&DBInfo,sizeof(DBInfo));

							

							



							LogPrintf("%d Money) ���� \n", ((SOPlayer*)victim)->siMoney);						
						}						
					}	
					
					// �����ڰ� ���� �� ��� 
					else if( ISMONSTER(victimplayerid) )
					{									
						LogPrintf("(MONSTER) %d �� %d ��� \n", victimplayerid, victimcharid);
									
						// ���� �Ѵ� 							
						victim->IFollower.DeleteFollower(victimcharid);						
					}					
				}
				break;



			case ONLINE_BATTLE_MSG_CHANGEKIND:
				{
					LogPrintf("-- ONLINE_BATTLE_MSG_KIND\n");

					SI32 playerid ;
					SI32 charid;
					SI32 onlinekind;

					if(obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(&obmsg, &playerid, &charid, &onlinekind) == TRUE)
					{
						
						int index = IsValidFollower(playerid, charid);
						if(index == -1) break;		
						
						LogPrintf("%d �� %d kind change ( %d --> %d ) \n", playerid, charid, pBattle->GetFollower(index, charid)->uiCharID, onlinekind );
						
						
						pBattle->GetFollower(index, charid)->uiCharID = onlinekind;					
					}
					else
					{
						int index = IsValidFollower(playerid, charid);
						if(index == -1) break;		

						writeInfoToFile("kindError.txt", "Wrong 'Kind(%d:%d)' detected after battle",
							pBattle->GetFollower(index, charid)->uiCharID,
							onlinekind  );
					}
				}
				break;


			case ONLINE_BATTLE_MSG_EXPALL:
				{
					LogPrintf("-- ONLINE_BATTLE_MSG_EXPALL\n");

					SI32 playerid ;
					SI32 exp;

					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(&obmsg, &playerid, &exp);

					// playerid�� ��� �뺴���� ����ġ exp�� �÷��ش�. 										
					for(int i=0; i < ON_MAX_FOLLOWER_NUM; ++i)
					{
						int index = IsValidFollower(playerid, i);
						if(index != -1) 
						{					
							_ImportantParameta* pParam  = pBattle->GetFollowerParam(index,i);

							// ����ִ� �뺴���Ը� ����ġ�� �ش� 
							if( pParam->GetLife() > 0 )
							{
								LogPrintf("Level: %d, Exp: %d \n",pParam->GetLevel(), pParam->GetCurExp() );

								LogPrintf("-- %d�� %d ĳ���� EXP %d ����\n", playerid, i, exp);												  
								if(pParam->IncreaseExp(exp) == TRUE)
								{
									LogPrintf("-- %d�� %d ĳ���� LevelUp!\n", playerid, i);

									pParam->SetLevel(pParam->GetLevel()+1);
									pParam->SetBonus(pParam->GetBonus()+4);
									pParam->SetLife(pParam->GetMaxLifeByIP());
									pParam->SetMana(pParam->GetMaxManaByIP(), 7);
								}						
							}
						}
					}										
				}
				break;

			// ��ŷ�� �õ��Ǿ����� �˷��ش�. 
			case ONLINE_BATTLE_MSG_HACK:
				{
					LogPrintf("-- ONLINE_BATTLE_MSG_HACK");
					
					// ��ŷ�� �õ��� ������� ���̵�. 
					SI32 playerid ;
					obmq.InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(&obmsg, &playerid);
				}
				break;


			default:
				break;
			}
		}
		break;

	default:
		printf("-- Do application:%d\n", lpMsg->dwType);
		break;

	}
}

BOOL _Game::WaitForMessageFrom(void)
{
	char temp[1024];
	SI32 size;
	SI32 i;

	_OrderData temporder;

	
	// �ڱ� �ڽ��� �ð���  �Է��Ѵ�. 
	DPCheck[UserPlayerNumber] = CurrentClock;

	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		if(PlayersdpId.dpId[i])
		{
			// ���������� �޽��� ������ 30�ʰ� ������, 
			if(DPCheck[i] && abs(CurrentClock - DPCheck[i]) > 30000)
			{
				// ���� ���� ��Ų��. 
				GoOutMsg.PlayerNumber = i;
				SendGameMessage((LPGWMSG_GENERIC)&GoOutMsg);
				DoLogoutAction( PlayersdpId.dpId[i]);
			}
		}
	}

	// ����ڷκ��� �Էµ� ����� �ִٸ�,
	if(Frame  % DPFrameRateForTrans == 0)
    {
		// �� �����ӿ��� �޽����� �ѹ��� ���� �� �ִ�. 
		if(MessageSendClock != Frame)
		{
			// �޽����� ���� �ð��� �� �����Ѵ�. 
			MessageSendClock  = Frame;
			
			// �޽����� ������ 
			while(PopUserOrderBuffer(&temporder) == TRUE)
			{
				MYMSG MyMsg;
				ZeroMemory(&MyMsg, sizeof(MYMSG));
				MyMsg.dwType    = GAME_MSG_MY;
				SetFrameInMyMsg(&MyMsg, Frame) ;// ���� ���� �������� �Ѱ��ش�.

				// ������ �ؼ� �����͸� ������.
				size = EncodeData((char*)&temporder, temp, sizeof(_OrderData));
				if(size)
				{
					CopyMemory(MyMsg.data, temp, size);
				}
				SetDataSizeInMyMsg(&MyMsg, size); 
				
				SendGameMessage((LPGWMSG_GENERIC)&MyMsg);
			}
		}
	}


	return TRUE;
}

// ���� �޽����� ������.
BOOL _Game::SendGameMessage(LPGWMSG_GENERIC lpMsg)
{
	int               nBytes;

	LPGOOUTMSG        lpGoOutMsg;
	LPNOMSG           lpNoMsg;
	LPMYMSG           lpMyMsg;

	// �޽��� Ÿ�Կ� ���� ���� ����Ʈ���� ���Ѵ�.
	switch(lpMsg->dwType)
	{
		// �κ񿡼� ���� ���� ��Ű�� �޽��� 
	case GAME_MSG_GOOUT:				
		lpGoOutMsg = (LPGOOUTMSG)lpMsg;
		nBytes     = sizeof(GOOUTMSG);
		
		pBattle->SendToAll((char*)lpMsg, nBytes);
		break;



        // �׹ۿ� ��ɿ����� ������ ũ�⸦ �����Ѵ�.
	case GAME_MSG_MY:
		{

			lpMyMsg = (LPMYMSG) lpMsg;
			
			if(GetDataSizeInMyMsg(lpMyMsg) > MY_MSG_DATA_SIZE)
			{
				writeInfoToFile("BattleGameError.txt", "Failed command sent in wrong size to the server. Command size :%d",
					GetDataSizeInMyMsg(lpMyMsg));
				
			}
			else
			{
				
				SetIDFromInMyMsg(lpMyMsg, UserPlayerNumber);
				
				nBytes = sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpMyMsg);
				
				pBattle->SendToAll((char*)lpMsg, nBytes);
			}
		}
		
		break;

		
		// ���� ������ Ŭ���̾�Ʈ�� ���� ���θ� Ȯ���ϱ� ���� ���� �������� ������ Ȯ�� �޽���. 
	case GAME_MSG_BATTLESERVERCHECK:
		{
			lpNoMsg			= (LPNOMSG)lpMsg;
			nBytes			= sizeof(NOMSG);

			pBattle->SendToAll((char*)lpMsg, nBytes);
		}
		break;

	default:
		printf("No Send Message:%d", lpMsg->dwType);

	}

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: DoLogoutAction()
// Desc: ���� Logout �޽����� ó���Ѵ�.
//--------------------------------------------------------------------------
BOOL _Game::DoLogoutAction(DWORD dpId)
{
	if(dpId == 0)return FALSE;

/*
	GetPlayerNumber(dpId, &id);
	wsprintf(buffer, "[%s]%s", PlayersName.Name[id], Text.QuitGameMessage.Get());
	SetMessage(buffer);
*/	
	
	// ����ü�� �Ҹ�� dpId���� �����.
	DeletePlayerNumber(dpId);
	
	return TRUE;
}

BOOL  _Game::DeletePlayerNumber(DWORD dpId)
{
	int i;
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(PlayersdpId.dpId[i]==dpId)
		{
			PlayersdpId.dpId[i]                  = NULL;
			PlayersdpId.Player[i].Country        = COUNTRY_NO;
			PlayersdpId.Player[i].Host           = BY_COMPUTER;		
			PlayersdpId.bImReady[i]              = FALSE;			
			return TRUE;
		}
	}
	return FALSE;
}




// ������� ����� ȣ��Ʈ�� ��� ���ۿ� �ִ´�. 
BOOL _Game::PushUserOrderBuffer(_OrderData order)
{
	if(UserOrderBufferIndex >= MAX_USER_ORDER_BUFFER_INDEX)return FALSE;

	UserOrderBuffer[UserOrderBufferIndex] = order;
	UserOrderBufferIndex ++ ;

	return TRUE;
}


// ȣ��Ʈ�� ����� ��� ���ۿ��� ����� �ϳ� ������. 
BOOL _Game::PopUserOrderBuffer(_OrderData * order)
{
	int i;

	if(UserOrderBufferIndex == 0) return FALSE;

	*order = UserOrderBuffer[0];

	for(i = 1 ;i < UserOrderBufferIndex ;i++)
	{
		UserOrderBuffer[i - 1] = UserOrderBuffer[i];
	}

	UserOrderBufferIndex --;

	return TRUE;
}

SI32 _Game::EncodeData(char* data, char* buffer, int size)
{
	int i;
	int count =0;
	int index = 0;

	memcpy(buffer, data, size);
	return size;

	for(i=0;i<size;i++)
	{

		if(data[i])
		{
			if(count)
			{
				buffer[index] = 0;
				index++;
				buffer[index] = count;
				index++;

				count=0;
			}

			buffer[index] = data[i];
			index++;

		}
		else
		{
			count++;
		}
	}

	return index;



}

SI32 _Game::DecodeData(char* data, char* buffer, int size)
{
	int i, j;
	int count =0;
	int index = 0;

	memcpy(buffer, data, size);
	return size;

	for(i=0;i<size;i++)
	{

		if(data[i])
		{
			buffer[index] = data[i];
			index++;

		}
		else
		{
			i++;
			count = data[i];
			for(j=0;j<count;j++)
			{
				buffer[index]=0;
				index++;
			}
		}
	}

	return index;
}


BOOL _Game::Action()
{
	char data[MAX_DATA_SIZE];
	UI32 length = 0; 

	CurrentClock = timeGetTime(); //pBattleGame->clServer->uiCurrentClock;

	// Ŭ���̾�Ʈ�� ����ִ��� Ȯ���ϱ� ���� üũ�޽����� 10�ʿ� �ѹ��� ������.
	if(abs(CurrentClock - LastClientCheckClock) >= 10000)
	{
		NOMSG nomsg;
		LastClientCheckClock = CurrentClock;

		nomsg.dwType	= GAME_MSG_BATTLESERVERCHECK;
		nomsg.Data		= 0;
		SendGameMessage((LPGWMSG_GENERIC)&nomsg);
	}


	// ó���� �޽����� ������ ó���Ѵ�. 
	if(PopData(data, length) == TRUE)
	{
//		LogFile("RecvMsg.txt", "PopData [%d]\n", length);
//		LogDataString("RecvMsg.txt", data, length, 999, 999);
		DoApplicationMessage((LPGWMSG_GENERIC) data, length);
	}


	//if(!pBattle->IsSingleMode())
	{
	
		if(WaitForMessageFrom() == FALSE) return FALSE;

		// �������� �߰��Ѵ�.
		Frame++;

	}

	return TRUE;	
}



int _Game::GetPlayerIndex(int uniqid)
{				
	SI32 index = -1;

	if( pBattle ) 
	{
		for(int i=0; i < 7; ++i)		
		{											
			if( pBattle->GetPlayer(i) )
			{							
				if( ((DWORD)uniqid) == pBattle->GetPlayer(i)->uiAccount )				
				{						
					index = i;					
					break;
				}				
			}
		}			
	}

	if(index < 0)
	{			
		for(int i=0; i < 7; ++i)		
		{											
			if( pBattle->GetPlayer(i) )
			{
				printf(" (%d)", pBattle->GetPlayer(i)->uiAccount);
			}
		}

		printf("\n Not proper player index:%d\n", uniqid);			
		return -1;
	}

	// ���� ��� ����� �ε��� ���� 
	return index;
}


int _Game::IsValidFollower(int uniqid, int charindex)
{					
	// ��Ͽ� �ش� uniqID�� �ִ°�?
	int index = GetPlayerIndex(uniqid);															
	if(index == -1) return -1;
	
	// �뺴 ������ �ùٸ���?
	if(charindex < 0 || charindex > ON_MAX_FOLLOWER_NUM-1) return -1;
							
	// �÷��̾��� �뺴 ������ �ִ°�?
	if(pBattle->GetFollower(index, charindex) == NULL) return -1;

	return index;
}



// �������� ����ǰ�� ����ٴ� ����� �˷��ش�. 
// playernumber: ����ǰ�� ���� �÷��̾� 
// slotnumber : ����ǰ�� ���� ĳ������ �ε��� 
// lostplayer : ����ǰ�� ���� �÷��̾� 
// lostslotnumber : ����ǰ�� ���� ĳ������ �ε���.
// itemcode:ȹ���� ����ǰ�� �ڵ� 
void _Game::PushBattleItemInfo(SI32 playernumber, SI32 slotnumber, SI32 lostplayer, SI32 lostslotnumber, SI32 itemcode)
{

	_OrderData          temporder;
	_NewID zeroid;
	zeroid.Init();

	ZeroMemory(&temporder, sizeof(_OrderData));

	temporder.ODI.Order			= 87;
	temporder.ODI.HostMode		= BY_PLAYER;
	temporder.ODI.ClearSwitch	= TRUE;

	temporder.ODI.Para1				= 0;
	temporder.ODI.Para2				= zeroid;
	
	temporder.PlayerNumber		= MAX_MULTI_PLAYER-1;
	temporder.CharNumber		= MAX_SELECTED_CHAR_NUMBER;
	
	temporder.IDData[0]			= playernumber;
	temporder.IDData[1]			= slotnumber;

	temporder.IDData[2]			= lostplayer;
	temporder.IDData[3]			= lostslotnumber;


	UI16 item = itemcode;
	memcpy(&temporder.IDData[4], &item, sizeof(UI16));

	PushUserOrderBuffer(temporder);

}