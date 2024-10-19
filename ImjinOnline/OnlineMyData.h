/************************************************************************************************
	���ϸ� : OnlineMyData.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_MYDATA_H
#define _ONLINE_MYDATA_H

#include <GSL.h>
#include "Online.h"
#include "../SCCommon/Defines.h"
#include "../SCCommon/GSCDefine.h"
#include "../SCCommon/OnlineCommonStruct.h"
#include "../SCCommon/IOnlineFollower.h"
#include "../AuthServer/_LoginDefine.h"
#include "Friend.h"


struct  CItemHeader;


class	MyData
{
public:
	UI16				uiPropertyVillageCode;
	UI08				uiPlantID;	
	cltFriendList		m_GoodFriendList;								// ģ���̸� ���
	cltFriendList		m_BadFriendList;								// ���Űź� �̸� ���
	SI08				siMaxGeneral;									// ��� ������ �屺 �ִ� �屺��

	int					nScore;
	int					nRank;

private:		
	cltOnlineWorld*		pWorld;

	// �α��� �ϱ� ���� ���̴� ������
	CHAR				szMyAccountGuid[36];							// ���� ���� GUID
	LOGIN::sCharInfo	MyCharacters[LOGIN::CHAR_MAX_NUM];				// �� ĳ���͵��� ���� ����.
	UI08				uiPrevSelectCharacterSlot;						// ���� ������ ĳ���� ����.
	UI08				uiSelectCharacterSlot;							// ������ ĳ���� ����.

	// ���� ������ �α��� ���� ���̴� ������
	UI16				uiMyCharUniqueID;								// �� ĳ������ ID.
	UI32				siMyCharDBAccount;								// �� ĳ������ DBAccount.
	UI16				uiMyCharKind;									// �� ĳ������ ����.
	char				cMyCharName[128];								// �� ĳ������ �̸�
	MONEY				uiMoney;										// �� ������� ��.	
	MONEY               uiSaving;                                       // �� ������� ���ݾ�.
	UI32				uiBatleCredit;									// �� ������� ���� �ſ뵵.
	UI08				uiBatleCreditPercent;							// �� ������� ���� �ſ뵵.
	UI32				uiTradeCredit;									// �� ������� ���� �ſ뵵.
	UI08				uiTradeCreditPercent;							// �� ������� ���� �ſ뵵.
	SI08				siVMercenarySlot;
	
	
	// �� ��� ����
	BOOL				fGuild;											// ��忡 ���Եž��ִ��� Flag
	char				szGuildName[ON_GUILDNAME_LENGTH + 1];			// ��� Name
	SI32				siGuildClass;									// ��� ����
	SI32				siGuildFlag;									// ��� ���

	SI16				siSelectNPCID;									// Select NPC�� ID
	BOOL				bSelectNPC;										// Select NPC �����ΰ�
	BOOL				bGotoField;										// �ʵ� ������ �����°�?
	BOOL				bGotoVillage;									// ������ ���°�?	
	BOOL				bGotoPortal;									// ������ ���°�?	
	BOOL				bWaitSelectNPCResponse;							// ������ ���� Select NPC�� ������ ��ٸ���.
	BOOL				bWaitGotoVillageResponse;						// ������ ���� ������ �� �� �ִ����� ������ ��ٸ���.
	BOOL				bWaitGotoPortalResponse;						// ������ ���� ������ �� �� �ִ����� ������ ��ٸ���.
	BOOL				bWaitBattleResponse;							// ������ ���� �ٸ� �÷��̾�� ������ �� �� �ִ����� ������ ��ٸ���.
	BOOL				bWaitPickUpItemResponse;						// ������ ���� �������� �ݰ� �ִ� ���� ������ ��ٸ���.
	UI16				uiGotoVillageCode;								// ���� ������ Code 	
	SI32				siGotoPortalID;									// ���� Portal�� ID (�ܼ��� x �� y �� �����̴�.)		
	SI16				siTotalHealth;									// ��Ƽ�� �� �����	
	IOnlineFollower		Followers;										// ���ϵ��� ����Ÿ.

	// robypark 2004/11/9 20:53
	// ������ ������ �� ���� ��
	SI32				m_siPrevCharOnlineKind;							// ������ ������ �� ���. ���� �� ���ΰ� ĳ���� Online Kind ID
	bool				m_bCanTransformSiegeWarfareUnit;				// ������ �������� ������ �����Ѱ�?

	// robypark 2004/12/10 20:11
	BOOL				m_bIsInVillageDefenceInterface;					// ����ڰ� ���� ���� �������̽� �� �ִ°�?

	// robypark 2005/1/27 19:00
	BOOL				m_bIsSiegeWarfareDay;							// ������ ������ �ϴ°�?

public:
	MyData();
	~MyData();

	VOID	Initialize();
	VOID	Init(cltOnlineWorld* lpWorld);							// �������� �ʱ�ȭ�� �����ϰ� �ʿ��� �޸𸮸� �Ҵ� �޴´�.
	VOID	Free();													// �� Ŭ�������� ����ߴ� �޸𸮸� ���� �����Ѵ�.
	VOID	SetMyCharUniqueID(UI16 siCharUniqueID);					// ���� ĳ���͸� �����Ѵ�.
	UI16	GetMyCharUniqueID();									// ���� ĳ���͸� ���´�.
	VOID	SetMyCharDBAccount(UI32 DBAccount);						// ���� ĳ������ DBAccount�� ���´�.
	SI32	GetMyCharDBAccount();									// ���� ĳ������ DBAccount�� ���´�.
	VOID	SetMyCharKind(UI16 uiCharKind);							// ���� ĳ���� ������ �����Ѵ�.
	UI16	GetMyCharKind();										// ���� ĳ���� ������ ���´�.
	VOID	SetMyCharName(char* lpName);							// ���� ĳ���� �̸��� �����Ѵ�.
	char*	GetMyCharName(void);									// ���� ĳ���� �̸��� ����Ѵ�.
	VOID	SetGuild(char* lpGuildName, SI32 GuildClass, SI32 GuildFlag);	// ���� ĳ���� �� ��� ����
	char*	GetGuildName(void);										// ���� ĳ���� �� ��� Name ���
	SI32	GetGuildClass(void);									// ���� ĳ���� �� ��� Class ��� 
	SI32	GetGuildFlag(void);										// ���� ĳ���� �� ��� ��� ��� 
	VOID	SetMoney(MONEY uiMoney);								// ���� ���� ���� �����Ѵ�.
	MONEY	GetMoney();												// ���� ���� ���� ���´�.	
	VOID	SetSaving(MONEY uiMoney);								// ���� ���� ���ݾ��� �����Ѵ�.
	MONEY   ModifySaving(MONEY uiMoney);                            // ���� ���� ���ݾ��� �����Ѵ�.
	MONEY	GetSaving();											// ���� ���� ���ݾ��� ���´�.		
	VOID	SetBatleCredit(UI32 BatleCredit, UI08 Percent);			// ���� ���� ���� �ſ뵵�� �����Ѵ�.
	UI32	GetBatleCredit();										// ���� ���� ���� �ſ뵵�� ���´�.	
	UI08	GetBatleCreditPercent();								// ���� ���� ���� �ſ뵵�� ���´�.	
	VOID	SetTradeCredit(UI32 TradeCredit, UI08 Percent);			// ���� ���� ���� �ſ뵵�� �����Ѵ�.
	UI32	GetTradeCredit();										// ���� ���� ���� �ſ뵵�� ���´�.
	UI08	GetTradeCreditPercent();								// ���� ���� ���� �ſ뵵�� ���´�.
	UI08	GetTradeGrade();										// ���� ���� �ſ� ����� �˾ƿ´�.
	UI32	GetNextTradeCredit(UI08 uiGrade);						// ���� �ſ� ����� �˾ƿ´�.
	SI32	GetHealth();											// ��Ƽ�� �� ������� ���´�.
	VOID	SetHealth( SI32	uiHealth );								// ��Ƽ�� ������� �����Ѵ�.

	// robypark 2004/11/18 20:2
	SI32	GetPrevCharOnlineKind(void);							// ������ ������ �� ���� �� ���� ���ΰ� ĳ���� Online Kind ID ���
	VOID	SetPrevCharOnlineKind(SI32 siPrevCharOnlineKind);		// ������ ������ �� ���� �� ���� ���ΰ� ĳ���� Online Kind ID ����
	bool	CanTransformSiegeWarfareUnit(void);									// ������ �������� �������� ���
	VOID	SetTransformSiegeWarfareUnit(bool bCanTransformSiegeWarfareUnit);	// ������ �������� ������ �������� ����

	VOID	SetSelectNPC(BOOL bSelectNPC, SI16 siNPCID);			// Select NPC ����
	BOOL	IsSelectNPC();											// Select NPC�� ���� �ϴ��� �˾ƿ´�.
	SI16	GetSelectNPC();											// Select NPC�� �̸��� ���´�.
	BOOL	IsWaitSelectNPCResponse();								// Select NPC�� ���� �޽����� ������ ������ ������ ��ٸ����� �˾ƿ´�.
	VOID	SetWaitSelectNPCResponse(BOOL bWaitResponse);			// Select NPC�� ���� �޽����� ������ ������ ������ ��ٸ������� �����Ѵ�.
	VOID	SetGotoVillage(BOOL bGoVillage, UI16 VillageCode);		// ������ ����� �����Ѵ�.
	VOID	SetGotoPortal(BOOL bGoPortal, SI32 siPortalID);			// Portal�� ����� �����Ѵ�.
	VOID	SetGotoField(BOOL bGoField);							// �ʵ�� ������� �����Ѵ�.
	BOOL	IsGotoVillage();										// ������ ���� �ϴ��� �˾ƿ´�.
	BOOL	IsGotoPortal();											// Portal�� ���� �ϴ��� �˾ƿ´�.
	SI32	GetGotoVillage();										// ���� ������ �̸��� ���´�.
	SI32	GetGotoPortal();										// ���� Portal�� �̸��� ���´�.
	BOOL	IsGotoField();											// �ʵ�� ���� �ϴ��� �˾ƿ´�.	
	BOOL	IsWaitGotoVillageResponse();							// ������ ���� �޽����� ������ ������ ������ ��ٸ����� �˾ƿ´�.
	VOID	SetWaitGotoVillageResponse(BOOL bWaitResponse);			// ������ ���� �޽����� ������ ������ ������ ��ٸ������� �����Ѵ�.
	BOOL	IsWaitGotoPortalResponse();								// Portal�� ���� �޽����� ������ ������ ������ ��ٸ����� �˾ƿ´�.
	VOID	SetWaitGotoPortalResponse(BOOL bWaitResponse);			// Portal�� ���� �޽����� ������ ������ ������ ��ٸ������� �����Ѵ�.
	BOOL	IsWaitBattleResponse();									// �ٸ� �÷��̾�� ������ �Ѵٰ� �޽����� ������ ������ ������ ��ٸ����� �˾ƿ´�.
	VOID	SetWaitBattleResponse(BOOL bWaitRespone);				// �ٸ� �÷��̾�� ������ �Ѵٰ� �޽����� ������ ������ ������ ��ٸ������� �����Ѵ�.
	BOOL	IsWaitPickUpItemResponse();								// �������� �ݴ´ٰ� �޽����� ������ ������ ������ ��ٸ����� �˾ƿ´�.
	VOID	SetWaitPickUpItemResponse(BOOL bWaitResponse);			// �������� �ݴ´ٰ� �޽����� ������ ������ ������ ��ٸ����� �����Ѵ�.

	UI16				FindEmptyItemBuffer(UI08 uiFollowerSlot);		// ������ ���� �߿� ����ִ� ���۸� ã�´�.	

	// robypark 2004/12/10 20:11
	BOOL	IsInVillageDefenceInterface(void);									// ����ڰ� ���� ���� �������̽� �� �ִ°�?

	// robypark 2004/12/10 20:11
	void	SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface);	// ����ڰ� ���� ���� �������̽� �� �ִ°�?

	// robypark 2005/1/27 19:00
	// ������ ������ �ϴ°�?
	BOOL	IsSiegeWarfareDay(void);

	// robypark 2005/1/27 19:00
	// ������ ������ �ϴ°��� ����
	void	SetSiegeWarfareDay(BOOL bIsSiegeWarfareDay);

	// ���� ���� �Լ�.
	IOnlineFollower*	GetFollowersInterface();															// ���ϵ��� Data ���� Class�� �����͸� ���´�.
	VOID				InitFollowers();																	// ���ϵ��� ���� ��� ������ �ʱ�ȭ �Ѵ�.
	BOOL				InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo);					// ���ϸ� �ʱ�ȭ �Ѵ�.
	BOOL				UpdateFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo);		// ���ϸ� ���� �Ѵ�.
	BOOL				DeleteFollower(UI08 uiFollowerSlot);												// ���ϸ� ���� �Ѵ�.
	BOOL				IsValidFollower(UI08 uiFollowerSlot);												// ��ȿ�� ���� Slot �ΰ�?
	UI16				GetFollowerCharID(UI08 uiFollowerSlot);												// ���� ĳ������ ID�� ���´�.
	CHAR				*GetFollowerCharName(UI08 uiFollowerSlot);											// ���� ĳ���� �̸��� ���´�.
	VOID				SetFollowerCharName(UI08 uiFollowerSlot, CHAR *pName);								// ���� ĳ���� �̸��� ������
	UI08				GetFollowerNum();																	// ���� ĳ������ �� ��
	

	// Get Follower Parameter Data
	SI32	GetFollowerParameterLevel(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCurExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterPrevExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterNextExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterNation(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalStr(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalDex(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalVit(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalInt(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalAC(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMaxLife(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterLife(UI08 uiFollowerSlot);
	VOID	SetFollowerParameterLife(UI08 uiFollowerSlot, SI16 siLife);
	SI32	GetFollowerParameterMaxMana(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMana(UI08 uiFollowerSlot);
	VOID	SetFollowerParameterMana(UI08 uiFollowerSlot, SI16 siMana);
	SI32	GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMaxWeight(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCurWeight(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCredit(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterLiftPower(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterBattlePower(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterBattlePower(void);
	SI32	GetFollowerParameterKind( UI08 uiFollowerSlot );
	SI32	GetFollowerParameterBonus(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalItemWeight(UI08 uiFollowerSlot);
	

	// ������ ó�� ���� �Լ���.		
	UI08			AddItem(UI08 uiFollowerSlot, MyItemData *pItem);										// �������� ������ ����Ʈ�� �߰��Ѵ�.
	UI08			DelItem(UI08 uiFollowerSlot, MyItemData *pItem	);										// �������� ������ ����Ʈ���� �����Ѵ�.			
	BOOL			GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem);							// �������� ���´�.
	MyItemData*		GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);						// �������� ���´�.
	MyItemData*		GetItem(UI08 uiFollowerSlot, UI16 uiItemID);											// �������� ���´�.
	BOOL			IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory);										// �������� �ִ��� �˻�
	MyItemData*		GetItemList(UI08 uiFollowerSlot);														// �����۸���Ʈ�� �����ش�.
	BOOL			AttachItem( UI08 uiFollowerSlot, UI32 uiItemID, SI16 siGroup);							// �������� ������ �Ѵ�.
	BOOL			StripItem( UI08 uiFollowerSlot, SI16 siGroup );											// �������� ���´�.
	VOID			UseItem( UI08 uiFollowerSlot, SI16 siHp, SI16 siMp, SI16 siHealth);						// �������� �Ҹ� �Ѵ�.
	VOID			UseItemToAll( SI16 siHp, SI16 siMp, BOOL bRevive);			                        	// ��ο��� ����Ǵ� ������
	VOID			UseItemToAllPer( SI16 siHp, SI16 siMp, SI16 siHealth );									// ��ο��� %��ŭ ������ �ȴ�.
	SI32			GetItemCount(UI16 uiItemID);															// ������ �ִ� Item�� ���� ���´�.

	

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////// �α��� �����Լ� /////////////////////////////////////////////////////////////////////

	VOID				SetMyCharacters(UI08 uiCharacterNum, LOGIN::sCharInfo *pMyCharactersInfo);
	LOGIN::sCharInfo*	GetMyCharacters(UI08 uiSlot);									// uiSlot = 1, 2, 3
	UI08				AddMyCharacter(CHAR *pID, WORD siKind, SI08 siNewSlot = 0);
	VOID				DeleteCharacter( UI08 uiSlot );							
	UI08				GetEmptySlot();
	VOID				SetSelectMyCharacterSlot(UI08 uiSlot);					
	UI08				GetSelectMyCharacterSlot();	
	VOID				SetPrevSelectMyCharacterSlot(UI08 uiSlot);				
	UI08				GetPrevSelectMyCharacterSlot();							

///////////////////////////////////////////////////////////////////////////////////////////////////

	VOID				InitMoveDestInfo();																// ĳ���Ͱ� �̵� ���� ���� �������� ���� ������ �ʱ�ȭ �Ѵ�.
	VOID				SetMoveDestInfo(SI16 siX, SI16 siY, SI32 siPlayerInfo, SI32 siItemInfo);		// ĳ���Ͱ� �̵� ���� ���� �������� ���� ������ �����Ѵ�.

	BOOL				SetMercenary(SI08 siSlot);
	SI08				GetMercenary(void);
	BOOL				ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser 
													*pCharParser, UI16 uiFollowerID, CItem* pItem);		// �⺻������ ���� ��� ���ʽ��� ��ȯ�� ��ġ
};

#endif

