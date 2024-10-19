#ifndef SOWORLD_H
#define SOWORLD_H

#include "Directives.h"
#include "Defines.h"
#include "OnlineProductStructure.h"
#include "OnlineEconomy.h"
#include "OnLineProtocol.h"
#include "SOUtility.h"
#include "SOServer.h"
#include "SOGate.h"
#include "IOnlineMap.h"
#include "SOPlayer.h"
#include "SODatabase.h"  
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"
#include "OnlineMap-Bf.h"
#include "SOVillage.h"
#include "SOCharKI.h"
#include "ISOMonster.h"
#include "OnlineBattleMsg.h"
#include "IFieldMonster.h"
#include "IPortal.h"
#include "OnlineText.h"
#include "IOnlineShipKI.h"
#include "SOFindPath.h"
#include "OnlineShipMoveInfo.h"
#include "ISOShip.h"
#include "Command.h"
#include "IProfit.h"
#include "IGuildClass.h"
#include "IGuild.h"
#include <onlinebattlemsg.h>
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
#include "OnlineTradeParser.h"
#include "OnlineQuest-Parser.h"
#include "BuriedItemParser.h"
#include "OnlineSmithParser.h"
#include "ItemUpgradeParser.h"
#include "OnlineWarpParser.h"
#include "OnlineItem.h"
#include "OnlineChar-Parser.h"
#include "OnlineTicket-Parser.h"
#include "OnlineGeneral-Parser.h"
#include "OnlineMercenary-Parser.h"
#include "OnlineSoilderList-Parser.h"
#include "OnlineVillage-Parser.h"
#include "QuestEventItemParser.h"
#include "WarTimeParser.h"
#include "OnlineCredit.h"
#include "ExpoRank.h"
#include "SOGMList.h"
#include "FieldBattleParser.h"
#include "WarpParser.h"
#include "LogTool1.h"
#include "Smith.h"

class CAuctionMarket;
class CQuestManager;

#define		DOKDO_QUEST_CODE1		22110
#define		DOKDO_QUEST_CODE2		22100

class SOWorld
{
private:
	// ��ȭ���� ī��Ʈ(�������� ����̳� ��ȭ���� ������°�?)
	DWORD				dwChatRoomInVillCounter;

	BOOL				m_IsAttackVillageTimeZone;

public:
	SOGate		*clGate;
	SODatabase	*clDatabase;
	SODatabase  *clLoginDatabase;
	SODatabase	*clItemDatabase;
	SOServer	*clServer;
	
	CBattleMgr			m_BattleMgr;
	
	CAuctionMarket*		m_pAuctionMarket;	
    CSmith        *     m_pSmith;

	
	
public:
	//==================================
	// Time ���� ���� 
	//==================================
	tm					*pRealTime;				//	���� �ð�.	
	time_t				stClock;
	OnRealWorldTime		RealWorldTime;			// ���� �ð�.
	 
	BOOL			bPopulationUpdate;
	BOOL			bUpdateTime;
	SI32			siCurrentDayOfYear;			// 0 ~ 365 ���� �Ϸ�    
	SI32			siCurrentDayOfMonth;		// 0 ~ 31 ���� �Ϸ�    
	SI32			siCurrentDayOfWeek;			// ���� (0 ~ 6; Sunday = 0)
	SI32			siCurrentHour;				// ���� �� (0 ~ 23)	

	SI32			siTotalVillageNum;			// ��ü ���� ����.

	DWORD			dwtimeGetTime;				// �ð�.		

	BOOL			bPowerInvestmentEnd;		// 24�ð� �Ǹ� ���������� �������

	CHAR			szBattleServerIP[20];		// ��Ʋ ���� IP.

	
	char			packetbuf[ 30000 ];
	

	BOOL            m_bFlag;
	BOOL            m_bGiveUp;

public:
	SOPlayer*				clPlayer[ ON_MAX_CONNECTION ];				// ����� Ŭ����	
	SOVillage*				pclVillage;									// ���� Ŭ����	
	CQuestManager*			m_pQuestManager;							// ���� ����Ʈ �޴���	
	UI32					m_uiDokdoQuestCompletedUserCount;			// ���� �ɽ�Ʈ �Ϸ��� ���� ��
	
	IMonster				clMonster;									// ���� Ŭ����	
	IFieldMonster			clFieldMonster;								// �ʵ� ���� ���� Ŭ����.
	
	OnlineCharParser		clCharParser;								// Character Info	
	OnlineTicketParser		clTicketParser;								// Ticket Info		
	OnlineQuestParser		clQuestParser;								// Quest Info	
	OnlineGeneralParser		clGeneralParser;							// General Info	
	OnlineGeneralParser		clGeneralExParser;
	OnlineMercenaryParser	clMercenaryParser;							// Mercenary Info	
	OnlineSolderListParser	clSolderListParser;							// Solder List
	OnlineSmithParser       clSmithParser;      
                        // ���� ������ ����Ʈ
	CBuriedItemParser		m_buriedItemParser;							// Item For Field
	CQuestEventItemParser	m_QuestEventItemParser;
//	CQuestEventItemParser*	m_pQuestEventItemParser;					// QuestEventItemParser 
	CQuestEventItemMgr*		m_QuestEventItemMgr;
	CFieldBattleParser		m_FieldBattleParser;						//
	CAttackPowerParser		m_AttackPowerParser;						// ������ ������ ����
	CWarpParser				m_WarpParser;
	CWarTimeParser			m_WarTimeParser;
	
	
	cltIProfit				clIProfit;									// ���忡�� ������ ���� ������ �˷��ִ� Ŭ����.	
	cltIGuildClass			clIGuildClass;								// Guild Class(����) �Ŵ���	
	cltIGuild				clIGuild;									// Guild �Ŵ���	
	OnlineTradeParser		*pTradeParser;								// OnlineTradeParser

//	BOOL                    bChangeTime;
//	tm                      *pChangeTime;
	

public:	
	BOOL			bSendServerData;									// ������ ����Ÿ�� �������ϴ����� �����Ѵ�.	
	SI32			siErrCode;											// �˴ٿ� ����	
	SI32			siLogOnNumber;										// ���� �α׿��� ����� ��	
	UI32			uiTotalRegAccount;									// ��ϵǾ� �ִ� ��ü ĳ���� ��	
	UI32			uiTotalRegCharactor;								// ��ϵǾ� �ִ� ��ü ���� ��	
	SOPlayer		*pPlayerOnAccount[ ON_MAX_CONNECTION + 1 ];			// ���� �α׿��� ������� �ϷĹ�ȣ	
	ISOMap			clISOMap;											// ��	
	CItem			clItem;												// ������.
	



	CItemUpgradeParser	m_ItemUpgradeParser;							// ������ ���׷��̵忡 ���ؼ�.. (3�� 31�� �̻�� �߰� )
	cltOnlineWarpParser	m_clWarpParser;									// Warp Info



	OnlineBuilding	clBuilding;											// ���� �ǹ� ����
	OnlineEconomy	clOnlineEconomy;									// ���� ����	
	OnlineCredit	clCredit;											// �ſ뵵 ����


	// NPC Kind Info
	cltSOCharKI		clSOCharKI;
	IOnlineShipKI	clIOnlineShipKI;				
	cltSOFindPath	clSOFindPath;
	cltOnlineSMI	clOnlineSMI;
	ISOShip			clSOShip;


	// IPortal
	IPortal			Portal;


	// OnlineText
	OnlineText		clOnlineText;


	cltCommand		clCommand;				// Command (Command)
	
	COnlineTimer	Timer;
	SOnlineTime		PrevCurrentTime, CurrentTime;
	SOnlineTime		PrevSaveTimeToDBTime;	
	BOOL			bHourChanged, bDayChanged, bMonthChanged, bYearChanged;

	// ���������� �������� ������ ���´� �ð�, ���������� ��� �������� ������ ���´°�
	DWORD			dwLastSendPayingBusinessTime;
	UI32			uiLastSendPayingBusinessItem;		

	SI32			siWeatherChangeTime;
	BOOL			bNowWeather;

	DWORD			dwLastThunderCheckTime;

	CLogTool		m_LogMgr;

	// Expo Rank...
	ExpoRank		m_ExpoRankingSystem;
	DWORD			m_dwExpoUpdateRankingTime;

	DWORD			m_dwExpoSendRankingMsgTime;

	cltGMManager	m_clGMManager;

	// GameGuard����
	BOOL			m_bGameGuard;
	BOOL			m_bWarEnd;

	BOOL			m_bQuestEventUpdate;
	SI16			m_siEventItemCount;

	DBIKQuestEventItemInfoMgr*	m_pstQuestEventItemInfoMgr;

public:	
	SOWorld(SOServer *);					// ������	
	~SOWorld();								// �Ҹ���


public:
	SI32		Poll();						// ��� ���� ó��
	VOID		DownServer(BOOL);			// ShutDown �䱸�� ���� ó��

	// ���� ��ȣ�� �˻��Ѵ�.
	BOOL		CheckPlayerSocket(SI32 siSocketNumber);


public:		
	VOID		GetItemDBPrice();
	VOID		SetItemDBPrice();

	stFarmData*	GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );		// �÷��̾ �����ϰ� �ִ� �ü����� ���´�.

	BOOL		InitGuildFromDB();			// DB���� ��带 �о�ͼ� �����Ѵ�.
	
	
	// Event
	BOOL		GetEventDataFromDDB();		// DB���� �̺�Ʈ�� ���� �⺻ ������ ���´�.
	
public:

//����������������������������������������������������������������������������
//	���� ����
//����������������������������������������������������������������������������
	BOOL		InitGuildAndVillage();							// ��ܰ� ������ �ʱ�ȭ �Ѵ�.
	SOVillage*	GetVillageByCode(UI16 uiVillageCode);			// ������ Index Code�� ����Ͽ� ������ �����͸� ���´�.
	SOVillage*	GetVillage(CHAR *pszVillageName);				// ������ �̸��� ����Ͽ� ������ �����͸� ���´�.			
	SI32		GetNearestHospitalVillageIndex(UI16 uiMapID, SI32 siX, SI32 siY);// Ư�� ��ġ���� ���� ����� �ǿ��� ���� ������ ã�´�.		
	SI32		GetVillageIndexByCode(UI16 uiVillageCode);		// �ش� Code�� ������ �ִ� ������ Village Index�� �˾ƿ´�.

//����������������������������������������������������������������������������
//	���� ����
//����������������������������������������������������������������������������
	BOOL		OfficialAnnouncement(DWORD dwResponse,
									LPARAM lParam1 = 0,
									LPARAM lParam2 = 0,
									LPARAM lParam3 = 0,
									LPARAM lParam4 = 0,
									LPARAM lParam5 = 0,
									LPARAM lParam6 = 0,
									LPARAM lParam7 = 0);		// ���� ���ӵ� ����ڿ��� ���� ������ �˷��ش�.


	// �⺻ ĳ���͸� ã���ִ� �Լ�.
	BaseChar*	GetBaseChar(UI16 uiAccount);					// �⺻ ĳ������ �����͸� ���´�.

	// �÷��̾ ã���ִ� �Լ�.	
	SOPlayer*	GetPlayer(CHAR* pszName);										// �÷��̾��� �����͸� ���´�.
	SOPlayer*	GetPlayer(UI16 uiAccount);										// �÷��̾��� �����͸� ���´�.
	SOPlayer*	GetPlayerByDBAccount( DWORD dwDBAccount );						// DB Account�� �̿��Ͽ� �÷��̾��� �����͸� ���´�.
	SOPlayer*	GetPlayerByDBAccount( DWORD dwDBAccount, UI08 uiDBSlot );		// DB Account�� DBSlot�� �̿��Ͽ� �÷��̾��� �����͸� ���´�.
	
	SI32		GetHighestGuildClass(CHAR *pszID);				// ĳ������ �ְ� ��� ������ �˾ƿ´�.
	SI32		GetHighestGuildClassInVillage(CHAR *pszID);		// �������� ĳ������ �ְ� ��� ������ �˾ƿ´�.
	SI32		GetOwnVillageNum( CHAR *pszID );				// ĳ���Ͱ� ������ ������ ������ �˾ƿ´�.

	VOID		DeleteGuildClassInVillage(CHAR *pszName);		// �� �������� �� ĳ���Ͱ� ������ �ִ� ������ �ʱ�ȭ �Ѵ�.
	VOID		GetRealTime(OnRealWorldTime *pRealWorldTime);	// ���� ������ �ð��� ���´�.

	VOID		SendPayingBusinessAnnoucement();				// ������ �Ǵ� �������� ������ ������ �ش�
	
	CharHead*	GetCharHeader(UI16 uiCharKind);					// ĳ���� ������ ����� ���Ѵ�.

	DWORD		IncChatRoomInVillCounter();						// ������ �������� ��ȭ���� ���� Ƚ���� ���������ش�.
	DWORD		LogOutAllPlayer();

	BOOL		IsIndustrialComplexOwner( SOPlayer *pPlayer );	// �÷��̾ ��� ������ ������ �ִ��� �˾ƿ´�.
    SOVillage_IndustrialComplex  *GetIndustrialComplex( SOPlayer *pPlayer );
	BOOL        SellIndustrialComplex(SOPlayer *pOldOwner,SOPlayer *pNewOwner);

	VOID		EnterAttackVillageTimeZone();					// ���� ������ ���۵Ǿ���.
	VOID		LeaveAttackVillageTimeZone();					// ���� ������ ������.
	BOOL		IsAttackVillageTimeZone();						// ���� ���� ���������� �˾ƿ´�.	

//������������������������������������������������������������������������������������������������������������������������������������������������
	VOID		ChangeSatietyByWeather();						// �ʵ忡 �ִ� ��� �������� �������� ���ҽ�Ų��.
	VOID		MakeThunder();									// ��õ ������ �����ϰ� ������ �����Ѵ�.
};



#endif