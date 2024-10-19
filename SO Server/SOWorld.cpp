#include "SOMain.h"
#include <Process.h>
#include "SOWorld.h"
#include "HqFileManager.h"
#include "ReadIP.h"

#include "AuctionList.h"
#include "AuctionMarket.h"
#include "bindjxfile.h"

#include "QuestManager.h"
#include "CSiegeUniteTable.h"
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// SOWorld ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOWorld::SOWorld(SOServer *clTempServer) : 
clSOFindPath(MAX_ONLINEMAP_XSIZE, MAX_ONLINEMAP_YSIZE), Timer(g_GameServerInfoParser.IsTestServer()),
m_bGameGuard(g_GameServerInfoParser.IsGameGuard())
{
	SI32			i;	
//	HQFileManager	hqFile;
	
	srand( (unsigned int)time( NULL ) );

	// �α� �Ŵ����� �ʱ�ȭ ��Ų��.
	m_LogMgr.Init();


	// �ؽ�Ʈ�� �ε��Ѵ�.

	char* szNationName;

	if( !g_GameServerInfoParser.IsTestServer() )
	{
		switch(g_GameServerInfoParser.GetLanguageCode())
		{
			case 'K':	case 'k':
				BindJXFile::GetInstance()->openJX( "hq" );
				szNationName = "korea";
				break;

			case 'E':	case 'e':
				BindJXFile::GetInstance()->openJX( "hq" );				
				szNationName = "English";
				break;

			case 'C':	case 'c':
				BindJXFile::GetInstance()->openJX( "hq" );				
				szNationName = "China";
				break;

			case 'J':	case 'j':
				BindJXFile::GetInstance()->openJX( "hq" );				
				szNationName = "Japan";
				break;

			case 'T':	case 't':
				BindJXFile::GetInstance()->openJX( "hq" );
				szNationName = "Taiwan";
				break;

			case 'I':	case 'i':
				BindJXFile::GetInstance()->openJX( "hq" );
				szNationName = "Indonesia";
				break;

			case 'V':	case 'v':
				BindJXFile::GetInstance()->openJX( "hq" );
				szNationName = "Event";
				break;
		}
	}
	else
	{
		BindJXFile::GetInstance()->openJX( "hq" );	
		szNationName = "korea";
	}

	
	// ����� ������ ������ ���Ͽ��� �о�´� 
	if( !m_buriedItemParser.readFile("Online\\Item\\ItemforField.txt") )
	{
		printf(" ItemForField.txt Error! \n ");
		getch();
	}

	// ���� Ŭ������ �����Ѵ�.
	clServer		= clTempServer;

	// clWorld�� clServer Ŭ������ clDatabase ������ �����Ѵ�.
	clDatabase		= clServer->clDatabase;
	clLoginDatabase = clServer->clLoginDatabase;
	clItemDatabase	= clServer->clItemDatabase;

	// �˴ٿ� �ڵ�
	siErrCode		= 2;

	siWeatherChangeTime = 5;
	bNowWeather         = FALSE;
	m_bQuestEventUpdate = FALSE;
	m_bWarEnd			= FALSE;
	m_siEventItemCount  = 0;

	

	dwLastThunderCheckTime	=	timeGetTime();

	// �ð� ���� 
	//------------------------------------------------------------
	time(&stClock);                         //Day Timer�� �����Ѵ� 
	pRealTime = localtime(&stClock);        //base �ð��� �����Ѵ�
	//------------------------------------------------------------	

//	try{
//		m_pQuestManager = new CQuestManager;
//	}
//	catch(std::bad_alloc){
//		assert(0);
//	}
//	// ���� ����Ʈ�� �Ϸ��� ����� ������ DB���� ���´�.
	m_uiDokdoQuestCompletedUserCount = 0;
	//SI16	siQuestCode = DOKDO_QUEST_CODE2;
	//clDatabase->GetDBInfo(SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST, &m_uiDokdoQuestCompletedUserCount, &siQuestCode);


	// ����ġ ������ �ε��Ѵ�.
	LoadExpData( g_GameServerInfoParser.IsTestServer() );

	// Text�� �ʱ�ȭ�Ѵ�.
	clOnlineText.Init(NULL, szNationName,NULL);

	// ĳ������ World�� �����Ѵ�.
	BaseChar::InitStaticVariable(this);		

	// Profit �������̽��� �ʱ�ȭ �Ѵ�.
	printf( "--- Loading Profit Interface... " );	
	if( clIProfit.Init("Online\\Village\\Profit.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// GuildClass�� �������̽��� �ʱ�ȭ �Ѵ�.
	printf( "--- Loading GuildClass Interface... " );	
	if( clIGuildClass.Init("Online\\Village\\Grade.txt") == TRUE )
		printf( "[Completed]\n" );
	else
	{
		printf( "[Failed]\n" );
		exit(1);
	}
	
	// Guild�� World�� �����Ѵ�.
	// �ݵ�� clIGuildClass.Init() �ؿ� �־�� �Ѵ�.
	cltIGuild::InitStaticVariable(this);		


	////////////////////////////////////////////////
	// �÷��̾ �����Ѵ�.
	////////////////////////////////////////////////
	printf( "--- Creating Player Instances... " );

	pPlayerOnAccount[ 0 ] = NULL;

	for( i = 0; i < ON_MAX_CONNECTION; ++i )
	{
		try{
			clPlayer[ i ] = new SOPlayer();
		}
		catch(std::bad_alloc){
			assert(0);
		}
		clPlayer[ i ]->uiAccount = i + 1;

		pPlayerOnAccount[ i + 1 ] = NULL;
	}
	printf( "[Completed]\n" );
	

	// ���� ������ ����Ѵ�.
	printf( "--- Analyzing Ships' Info... " );
	clIOnlineShipKI.Init();
	printf( "[Completed]\n" );

	// ���� �̵��� ���õ� ��ü���� �ʱ�ȭ ��Ų��.
	printf( "--- Analyzing Ships' course... " );
	clOnlineSMI.Init();
	printf( "[Completed]\n" );
	

	printf( "--- Loading Credit Info... " );
	clCredit.Init("Online\\Add_Credit.txt");
	printf( "[Completed]\n" );

	////////////////////////////////////////////////
	// ��Ż ȭ���� ����.
	////////////////////////////////////////////////	
	printf( "--- Creating Portals... " );
	if( Portal.Init("Online\\Map\\MapPortal.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	////////////////////////////////////////////////
	// ���� �����Ѵ�.
	////////////////////////////////////////////////	
	printf( "--- Creating Maps... " );
	ISOMap::InitStaticVariable(this);
	if(clISOMap.Init() == TRUE)	
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );	


	////////////////////////////////////////////////
	// ������ ���õ� ������ �ε��Ѵ�.
	////////////////////////////////////////////////	
	
	clOnlineEconomy.Init("Online\\Village\\Economy.txt");
	
	
	////////////////////////////////////////////////
	// ���� �ǹ��� ���õ� ������ �ε��Ѵ�.
	////////////////////////////////////////////////
	printf( "--- Loading Manufacturing Plant... " );
	if( clBuilding.Init() == FALSE )
		printf( " [Failed]\n" );
	else
		printf( " [Completed]\n" );

	////////////////////////////////////////////////
	// �������� �ε��Ѵ�.
	////////////////////////////////////////////////	
	printf( "--- Loading Items... " );
	if(clItem.ItemLoad(	&clOnlineText,
						"Online\\Item\\weapon.txt", "Online\\Item\\armor.txt", "Online\\Item\\accessary.txt", "Online\\Item\\Animal.txt"
					   ,"Online\\Item\\Dress.txt", "Online\\Item\\Element.txt", "Online\\Item\\Food.txt", "Online\\Item\\Liquor.txt"
					   ,"Online\\Item\\Medicine.txt", "Online\\Item\\Tool.txt", "Online\\Item\\Trade.txt","Online\\Item\\Specific.txt"
					   ,"Online\\Item\\Imperium.txt") == TRUE)
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// TradeParser�� �о�´�.
	pTradeParser	=	new	OnlineTradeParser;	

	// IFolower ��ü�� �ʱ�ȭ �Ѵ�.
	// �ݵ�� clItem�� pTradeParser�� ������ �Ŀ� �ؾ� �Ѵ�.
	IOnlineFollower::InitStaticVariable(&clItem);	
	
	printf( "--- Loading TicketParser Interface... " );	
	if( clTicketParser.ReadText("Online\\Village\\TicketInfo.txt") == TRUE )	printf( "[Completed]\n" );
	else																		printf( "[Failed]\n" );

	printf( "--- Loading QuestParser Interface... " );	
	if( clQuestParser.ReadText("Online\\Quest\\Quest.txt",szNationName,(SI16)pRealTime->tm_year + 1900,(SI16)pRealTime->tm_mon + 1,(SI16)pRealTime->tm_mday) == TRUE )	printf( "[Completed]\n" );
	else																printf( "[Failed]\n" );

	printf( "--- Loading GeneralParser Interface... " );	
	if( clGeneralParser.ReadText("Online\\CharInfo\\General_List.txt") == TRUE )	printf( "[Completed]\n" );
	else																			printf( "[Failed]\n");

	printf( "--- Loading GeneralExParser Interface... " );	
	if(clGeneralExParser.ReadText("Online\\CharInfo\\GeneralListEx.txt") == TRUE )  printf( "[Completed]\n" );
	else																			printf( "[Failed]\n");

	printf( "--- Loading MercenaryParser Interface... " );	
	if( clMercenaryParser.ReadText("Online\\Village\\Mercenary.txt") == TRUE )	printf( "[Completed]\n" );
	else																		printf( "[Failed]\n");

	printf( "--- Loading SolderListParser Interface... " );	
	if( clSolderListParser.ReadText("Soldier.txt") == TRUE )	printf( "[Completed]\n" );
	else														printf( "[Failed]\n" );

	printf( "--- Loading SmithParser Interface... " );	
	if( clSmithParser.ReadText("Online\\Item\\Weapon_Process.txt") == TRUE )	printf( "[Completed]\n" );
	else																		printf( "[Failed]\n" );

	printf( "--- Loading WarpParser Interface... ") ;	
	if( m_clWarpParser.Init("Online\\Village\\Warp.txt", &clOnlineText ) == TRUE )	printf( "[Completed]\n" );
	else																			printf( "[Failed]\n" );	

	printf( "--- Loading EventQuest Item File... ") ;
	//m_siEventItemCount = m_QuestEventItemParser.Load("QuestEventfile.txt");
	//m_pQuestEventItemParser		=	new CQuestEventItemParser[clQuestParser.m_siEventQuestCount];

	//QuestEventItem Parser�� ���� ����.
	m_QuestEventItemMgr = new CQuestEventItemMgr;
	m_QuestEventItemMgr->Init(clQuestParser.m_siEventQuestCount);

	//DB���� ���� ������ ������ ����.
	m_pstQuestEventItemInfoMgr	=	new DBIKQuestEventItemInfoMgr[clQuestParser.m_siEventQuestCount];
	
	char szTempDate[32];
	int	 siCount = 0;
	sprintf(szTempDate,"%d-%d-%d",pRealTime->tm_year + 1900,pRealTime->tm_mon + 1,pRealTime->tm_mday);

	for(i = 0 ; i < clQuestParser.m_QuestDataCount;i++)
	{
		if(clQuestParser.m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_PLAY)
		{
			char szTemp[32];
			ZeroMemory(szTemp,sizeof(szTemp));
			sprintf(szTemp,"QuestEvent%d.txt",clQuestParser.m_lpQuestData[i].siQuestCode);
			m_QuestEventItemMgr->Load(szTemp,clQuestParser.m_lpQuestData[i].siQuestCode);
			//m_pQuestEventItemParser[siCount].Load(szTemp,(SI32)clQuestParser.m_lpQuestData[i].siQuestCode);
			
			ZeroMemory(&m_pstQuestEventItemInfoMgr[siCount],sizeof(DBIKQuestEventItemInfoMgr));
			m_pstQuestEventItemInfoMgr[siCount].siQuestID	= clQuestParser.m_lpQuestData[i].siQuestCode;
			clDatabase->GetDBInfo(SO_DB_INFO_GETEVENTITEM,&szTempDate,&m_pstQuestEventItemInfoMgr[siCount]);
			siCount++;
		}
	}

	printf( "%d EventQuest Item [Completed]\n",siCount);

//	m_FieldBattleParser.Load("FieldBattle.txt");

	CSiegeUnitTable::GetInstance()->Init();
	
	m_AttackPowerParser.Load("AttackPower.txt");

	m_WarpParser.Load("WarWarp.txt");
	m_WarTimeParser.Load("WarTime.txt");
	
	///////////////////////////////////////////////////////////////////////
	// DB�κ��� �ʵ忡 ������ �ִ� �������� ������ ���ͼ� �ʿ� �����Ѵ�.
	///////////////////////////////////////////////////////////////////////
	clISOMap.SetItemFromDB();
	
	// ���� �ð��� DB�� ���� ���´�.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GETWORLDTIME, NULL, &CurrentTime);
	
	// DB���� ���� �ð��� �����Ѵ�.
	Timer.Create(CurrentTime);
	PrevSaveTimeToDBTime = PrevCurrentTime	=	CurrentTime;

	printf( "Current Time : %d/%d/%d %d:00\n", CurrentTime.Month, CurrentTime.Day, CurrentTime.Year, CurrentTime.Hour);
	// ������ �ε��Ѵ�.
	// �ݵ�� InitGuildFromDB() �Ʒ��� �־�� �Ѵ�.
	printf( "--- Creating Villages and Guilds...\n");	
	if( InitGuildAndVillage() == TRUE )		printf( "Creating Villages and Guilds Completed.\n" );
	else
	{
		printf( "Creating Villages and Guilds Failed.\n" );
//		clServer->WriteInfo( "..\\StartServerError.txt", "Creating Villages Failure!" );
		writeInfoToFile( "StartServerError.txt", "Creating Villages Failure!" );
	}
	
	if( IsEvent() )
	{
		GetEventDataFromDDB();
	}

	// ����� ������ ���´�.
	clDatabase->GetDBInfo( SO_DB_INFO_KIND_GET_GM_LIST, NULL, &m_clGMManager );

	GetItemDBPrice();
	SetItemDBPrice();

	// ������ �ε������Ƿ� �� Ÿ�Ͽ� ����� ������ �������ش�.  
	printf( "--- Loading CharParser Interface... " );	
	if( clCharParser.Init("Online\\CharInfo\\Character List.txt") == TRUE )	printf( "[Completed]\n" );
	else																	printf( "[Failed]\n" );

	for(i = 0 ; i < clCharParser.siTotalCharNum ; i++)
	{	
		DBIKCharacterList	TempCharacterList;
		
		TempCharacterList.uiID = clCharParser.GetCharHeader(i)->uiID;	
		strcpy(TempCharacterList.szChar,clOnlineText.Get(clCharParser.GetCharHeader(i)->uiCharName));
		TempCharacterList.siLv = clCharParser.GetCharHeader(i)->siLv;
		TempCharacterList.uiExp = clCharParser.GetCharHeader(i)->uiMyExp;
		TempCharacterList.siNation = clCharParser.GetCharHeader(i)->siNation;
		TempCharacterList.siStr = clCharParser.GetCharHeader(i)->Parameta.GetStr();
		TempCharacterList.siDex = clCharParser.GetCharHeader(i)->Parameta.GetDex();
		TempCharacterList.siVit = clCharParser.GetCharHeader(i)->Parameta.GetVit();
		TempCharacterList.siInt = clCharParser.GetCharHeader(i)->Parameta.GetInt();
		TempCharacterList.siAc  = clCharParser.GetCharHeader(i)->Parameta.GetAC();
		TempCharacterList.siDamageResist = clCharParser.GetCharHeader(i)->Parameta.DamageResist;
		TempCharacterList.siMagicResist = clCharParser.GetCharHeader(i)->Parameta.MagicResist;
		TempCharacterList.siMinEquip = clCharParser.GetCharHeader(i)->Parameta.EquipMinDamage;
		TempCharacterList.siMaxEquip = clCharParser.GetCharHeader(i)->Parameta.EquipMaxDamage;
		TempCharacterList.siMoveSpeed = clCharParser.GetCharHeader(i)->siMoveSpeed;
		TempCharacterList.siLife = clCharParser.GetCharHeader(i)->Parameta.GetLife();
		TempCharacterList.siMana = clCharParser.GetCharHeader(i)->Parameta.GetMana();
		TempCharacterList.siCost = clCharParser.GetCharHeader(i)->siCost;
		TempCharacterList.siBonus = clCharParser.GetCharHeader(i)->Parameta.GetBonus();

		clDatabase->writeDBQuery(SO_DB_INFOKIND_CHARACTER_LIST, &TempCharacterList, sizeof(DBIKCharacterList));
	}


	// NPC�� ���� ������ ȭ�Ͽ��� �о�´�.
	printf( "--- Loading NPC KI Interface... " );
	if( clSOCharKI.Init(&clOnlineText, "Online\\CharInfo\\Character List.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// ���͸� �ʱ�ȭ �Ѵ�.
	printf( "--- Loading Monsters... " );	
	clMonster.Init(this);
	printf( " [Completed]\n" );

	// �ʵ� ���� �����ڸ� �ʱ�ȭ �Ѵ�.
	printf( "--- Loading Field Monster Interface... " );	
	clFieldMonster.Init(&clMonster, &clISOMap);
	printf( "[Completed]\n" );

	//�̺�Ʈ ����Ʈ ������ ���´�.

	// Text ȭ���� �����Ѵ�.
//	hqFile.Free();

	BindJXFile::GetInstance()->destroy();
	

	// �踦 �ʱ�ȭ �Ѵ�.
	clSOShip.Init();

	// Find Path�� ���� ������ �ʱ�ȭ �Ѵ�.
	clSOFindPath.InitStaticVariable(&clISOMap);

	// Command�� �ʱ�ȭ�Ѵ�.
	clCommand.Init(this);	

	dwLastSendPayingBusinessTime	=	0;
	uiLastSendPayingBusinessItem	=	0;

	bUpdateTime		=	FALSE;

	// dwtimeGetTime�� �ʱ�ȭ �Ѵ�.
	dwtimeGetTime	=	timeGetTime();


	try{	
		m_pAuctionMarket = new CAuctionMarket(this);
	}
	catch(std::bad_alloc){
		assert(0);
	}
	clDatabase->GetItemAuctions( m_pAuctionMarket );

	try{	
		m_pSmith = new CSmith(this);
	}
	catch(std::bad_alloc){
		assert(0);
	}

	m_IsAttackVillageTimeZone	=	FALSE;
	bPowerInvestmentEnd			=	FALSE;
//	m_bVillageAttack			=	FALSE;
//	m_dwVillageDamageUpdateTime	=	0;

	m_dwExpoUpdateRankingTime	=	timeGetTime();
	m_dwExpoSendRankingMsgTime	=	timeGetTime();

	m_bFlag                     =  FALSE;
	m_bGiveUp                   =  FALSE;
//	bChangeTime                 =  FALSE;
//	pChangeTime                 =  pRealTime;
	return;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// SOWorld �Ҹ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOWorld::~SOWorld()
{
	SI32	i;

	if( m_pAuctionMarket ) delete m_pAuctionMarket;
	if( m_pSmith ) delete m_pSmith;

	//-------------------------------------------------------------------------------
	// ������ �����Ѵ�.
	//-------------------------------------------------------------------------------
	if(pclVillage)
	{
		delete [] pclVillage;
		pclVillage	=	NULL;
	}

/*	if(m_pQuestEventItemParser)
	{
		delete[] m_pQuestEventItemParser;
		m_pQuestEventItemParser = NULL;
	}
*/
	//-------------------------------------------------------------------------------
	// ����Ʈ ������ �����Ѵ�.
	//-------------------------------------------------------------------------------
//	if(m_pQuestManager != NULL)
//	{
//		delete m_pQuestManager;
//		m_pQuestManager = NULL;
//	}

	//-------------------------------------------------------------------------------
	// ���� �����Ѵ�.
	//-------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------
	// �÷��̾ �����Ѵ�.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Player Instances... " );
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		delete clPlayer[i];

		clPlayer[i] = NULL;
	}
	printf( "[Completed]\n" );


	//-------------------------------------------------------------------------------
	// ���͸� �����Ѵ�.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Monsters... " );
	clMonster.Free();
	printf( "[Completed]\n" );

	printf( "--- Deleting Ships... " );
	clSOShip.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// ���� �����Ѵ�.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Maps... " );
	clISOMap.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// �������� �����Ѵ�.
	//-------------------------------------------------------------------------------
	clItem.ReMoveAll();
	clOnlineSMI.Free();
	clIProfit.Free();
	clIGuildClass.Free();

	if(pTradeParser)
	{
		delete pTradeParser;
		pTradeParser	=	NULL;
	}

	if(CQuestEventItemMgr)
	{
		delete m_QuestEventItemMgr;
		m_QuestEventItemMgr = NULL;
	}

	siWeatherChangeTime = 0;
	bNowWeather         = FALSE;

	return;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾�� ���õ� ��� ���� ó�� (�������ΰ�?, �������ΰ�?, �����ΰ�?, ��ȭ���ΰ�?, �������ΰ�?)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 SOWorld::Poll()
{
	SI32	i;
	char	InfoBuf[1024];
	
	dwtimeGetTime	=	timeGetTime();	

	time(&stClock);                         // ���� ���� �ð��� ���´�.
	pRealTime = localtime(&stClock); 
	GetRealTime(&RealWorldTime);

	CurrentTime		=		Timer.GetNowTime();	
	
	bHourChanged = bDayChanged = bMonthChanged = bYearChanged = FALSE;

	//�ð� ������ ���� �� ���߿� �������ٰ�.
	if(pRealTime->tm_hour == 23)
	{
		if(pRealTime->tm_min == 59)
		{
			if(pRealTime->tm_sec >= 50)
			if(m_bQuestEventUpdate == FALSE)
				m_bQuestEventUpdate = TRUE;
		}
	}

	if(RealWorldTime.uiHour == 0)
	{
		if(m_bQuestEventUpdate == TRUE)
		{
			char szTemp[32];
			SI16 siEventDay = 0;
			sprintf(szTemp,"%d-%d-%d",pRealTime->tm_year + 1900,pRealTime->tm_mon + 1,pRealTime->tm_mday);

			for(int i = 0 ; i < clQuestParser.m_QuestDataCount ; i++)
			{
				if(clQuestParser.m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_PLAY)
				{
					siEventDay = clQuestParser.GetComPareDate(pRealTime->tm_year + 1900,pRealTime->tm_mon + 1,pRealTime->tm_mday,
							clQuestParser.m_lpQuestData[i].m_stEndDate.siYear,clQuestParser.m_lpQuestData[i].m_stEndDate.siMon,
							clQuestParser.m_lpQuestData[i].m_stEndDate.siDay);
					 if(siEventDay >= 0 )
						 clQuestParser.m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_PLAY;
					 else
						 clQuestParser.m_lpQuestData[i].m_siQuestEvent				= QUEST_EVENT_CANCEL;
				}
				
				for(int j = 0; j < clQuestParser.m_siEventQuestCount ; j++)
				{
					if(m_pstQuestEventItemInfoMgr[j].siQuestID == clQuestParser.m_lpQuestData[i].siQuestCode)
					{
						ZeroMemory(&m_pstQuestEventItemInfoMgr[j],sizeof(DBIKQuestEventItemInfoMgr));
						if(clQuestParser.m_lpQuestData[i].m_siQuestEvent == QUEST_EVENT_PLAY)
							clDatabase->GetDBInfo(SO_DB_INFO_GETEVENTITEM,&szTemp,&m_pstQuestEventItemInfoMgr[j]);
					}
				}
			}
			for(int PlayerCount = 1; PlayerCount < ON_MAX_CONNECTION + 1; PlayerCount++)
			{
				if(pPlayerOnAccount[PlayerCount] != NULL)
				{
					pPlayerOnAccount[PlayerCount]->SendFieldMsg(ON_RESOPNSE_REAL_TIME, SO_SFM_ONLYME, LPVOID(pRealTime->tm_year + 1900),LPVOID(pRealTime->tm_mon + 1),LPVOID(pRealTime->tm_mday));
				}
			}
			//��� ���� �缳��.
			
			clIGuild.SetRelationReLoad();
			m_bQuestEventUpdate = FALSE;	
		}
	}

	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))
	{    //�����ϴ� ��, �ð��� ������ ó�� �����ϸ�
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour)) // 8 ~ 10
		{	//�������� ������ �����´�.		
			if(!m_bGiveUp)
			{
				clIGuild.DBGetGiveUpInfo();
				m_bGiveUp = TRUE;
			}						
		}
		else 
		{   //�����ϴ� �ð��� �ƴѰ�� �������� ���̺� ���� �Ѵ�.
			if(!m_bFlag)
			{
				clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
				m_bFlag = TRUE;
			}
		}
	}

	// �ð��� ����Ǿ����� üũ�Ѵ�.
	if(CurrentTime.Hour != PrevCurrentTime.Hour)
	{
		// �ð��� ����Ǿ���.
		bHourChanged	=	TRUE;
	
		// ���� �������� 1�ð����� ���̳� �� ���ȴٸ� �������� ���ҽ�Ų��.
		// �� ��ġ�� ������ �������� ���ҽ�Ű�� ���� �Ϻη� ���� ó�� ������ ��ġ���״�.
		//if( bNowWeather )		ChangeSatietyByWeather();

		siWeatherChangeTime--;
		if(siWeatherChangeTime < 0)
		{
			BOOL		fEvent;

			// Event�� 12�� 24�ϰ� 25�� �׻� ���� ���� ��
			fEvent = FALSE;
			if(RealWorldTime.uiMonth == 12)
			{
				//////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////
				if(RealWorldTime.uiDay == 24)
				{
					bNowWeather = TRUE;
					fEvent      = TRUE;
				}
				if(RealWorldTime.uiDay == 25)
				{
					bNowWeather = TRUE;
					fEvent      = TRUE;
				}
				//////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////
			}
			// ���� Change
			if(fEvent == FALSE)
			{
				if((rand() % 2) == 0)
				{
					siWeatherChangeTime = 2 + (rand() % 5);
					bNowWeather         = TRUE;
				}
				else
				{
					siWeatherChangeTime = 10 + (rand() % 25);
					bNowWeather         = FALSE;
				}
			}

			// �� User���� ���� ���� �޼���
			for(int PlayerCount = 1; PlayerCount < ON_MAX_CONNECTION + 1; PlayerCount++)
			{
				if(pPlayerOnAccount[PlayerCount] != NULL)
				{
					pPlayerOnAccount[PlayerCount]->SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(bNowWeather));
				}
			}
		}	// if(siWeatherChangeTime < 0)

		// ������ ����Ǿ����� üũ�Ѵ�.
		if(CurrentTime.Day != PrevCurrentTime.Day)
		{			
			// ������ ����Ǿ���.
			bDayChanged	=	TRUE;
			
			// �Ϸ簡 �������� ��Ż���� ���־�� �� �۾����� ���ش�.
			Portal.DayChanged();

			sprintf(InfoBuf,"[Date : %4d-%2d-%2d]   Max[%4d] Current[%4d]",CurrentTime.Year,CurrentTime.Month,CurrentTime.Day,
				clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer);
//			clServer->WriteInfo("..\\ConnectStatus.txt",InfoBuf);
			writeInfoToFile("ConnectStatus.txt",InfoBuf);

			// ���� ����Ǿ����� üũ�Ѵ�.
			if(CurrentTime.Month != PrevCurrentTime.Month)
			{
				// ���� ����Ǿ���.
				bMonthChanged	=	TRUE;

				// ���� ����Ǿ����� üũ�Ѵ�.
				if(CurrentTime.Year != PrevCurrentTime.Year)
				{
					// ���� ����Ǿ���.
					bYearChanged	=	TRUE;
					printf("Year Changed!!\n");		

					// ���� �����ڼ��� �ʱ�ȭ �Ѵ�.
					for(i = 0; i < siTotalVillageNum; i++)
					{
						pclVillage[i].PrepareVillageInOutPlayer();
					}
				}

				printf("Month Changed!!\n");		
			}

			printf("Day Changed!!\n");		
		}
	}	// end of if( �ð��� ����Ǿ����� üũ�Ѵ� )

	// ������ ������ ������ 1�ʿ� �ѹ��� üũ�Ѵ�.
	if( dwtimeGetTime - dwLastThunderCheckTime >= 1000 )
	{
		dwLastThunderCheckTime	=	dwtimeGetTime;
		// �� �Ǵ� ���� ������ �ִ� ���¿��� �Ѵ�.
		if( bNowWeather && rand() % 60 == 0 )		MakeThunder();
	}


	///////////////////////////////////////////////////////////////////////////////////////
	// �����ð� 11�ÿ� �������� �α��� ������Ʈ �Ѵ�. 
	// ���� �α�������Ʈ�� SoVillage���� ������.SetDBPopulation();
	///////////////////////////////////////////////////////////////////////////////////////
	if(pRealTime->tm_hour	==	m_WarTimeParser.GetPopulationStart()) 
	{
		if( pRealTime->tm_min	==	58 )
		{
			bPopulationUpdate	=	TRUE;
		}
		else if(pRealTime->tm_min	==	59 )
		{
			if(bPopulationUpdate == TRUE)
			{
				bUpdateTime		=	TRUE;

				// ���� 4�ÿ� �ϴ� �۾�  - ������ ���� ����
				GetItemDBPrice();

				bPopulationUpdate	= FALSE;
			}
			else
				bUpdateTime		=	FALSE;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	// ���� �ð��� �˻��Ѵ�. �׽�Ʈ ���� ���� �����ð��� PM:8 ~ 10�÷� �Ѵ�.
	///////////////////////////////////////////////////////////////////////////////////////
//	if(pRealTime->tm_wday == 2 || pRealTime->tm_wday == 4) //ȭ, ��������� üũ
	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))// IsWarCheck(pRealTime->tm_wday, pRealTime->tm_hour))
	{
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour))
		{
			if( IsAttackVillageTimeZone() == FALSE )
			{
				EnterAttackVillageTimeZone();

				printf("���� ���� ���� ����...");
				clIGuild.SendWarTime(); //�����ð��� �Ǿ��ٰ� �˷��ش�.				
				printf("���� ���� ���� ��\n");			
			}
		}
		else
		{
			if( IsAttackVillageTimeZone() == TRUE )
			{
				LeaveAttackVillageTimeZone();
				/////////////////////////////////////////////////////////////////////////////////
				//������ �����ٸ� ���� ĳ���͸� �Ϲ� ĳ���ͷ� �ٲٰ� �ٸ��������� ����.
				//SOPlayer::Poll���� ĳ������ ������ �����Ѵ�.
				//clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
				//m_bWarEnd = TRUE;
				/////////////////////////////////////////////////////////////////////////////////
			}
		}
	}
	
	if(( pRealTime->tm_hour >= m_WarTimeParser.GetWarUpdate() ) && (pRealTime->tm_min == 5))
	{
		if(m_bWarEnd == FALSE)
		{
			/////////////////////////////////////////////////////////////////////////////////
			//������ �����ٸ� ���� ĳ���͸� �Ϲ� ĳ���ͷ� �ٲٰ� �ٸ��������� ����.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
			m_bWarEnd = TRUE;
			/////////////////////////////////////////////////////////////////////////////////

			//�������� ���������� �ʱ�ȭ ���ش�.
			clIGuild.InitGiveUpInfo();
		}
	}


	if(( pRealTime->tm_hour >= m_WarTimeParser.GetWarUpdate() ) && (pRealTime->tm_hour == 10))
	{
		if(m_bWarEnd == TRUE)
		{
			clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_ITEM, LPVOID(&clIGuild), NULL);
			m_bWarEnd = FALSE;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////
	// ���� ���������� ������.�� 12�ÿ� ����.
	///////////////////////////////////////////////////////////////////////////////////////
	if(pRealTime->tm_hour == m_WarTimeParser.GetInvestUpdateStart())
	{
		if(pRealTime->tm_min	==	59 )
		{
			if(bPowerInvestmentEnd == FALSE)
			{
				bPowerInvestmentEnd = TRUE;
			}
		}
	}
	else if(pRealTime->tm_hour == m_WarTimeParser.GetInvestUpdateEnd())
	{
		if(bPowerInvestmentEnd == TRUE)
		{	
			//������ ���ڱ����� �����ش�.
			clIGuild.SetPowerInvestmentEnd();
			//DB�� ���ڱ����� �����ٰ� �޼����� ������.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_POWER_INVESTMENT_END, NULL, 0);
			bPowerInvestmentEnd = FALSE;

			///////////////////////////////////////////////////////////////////////////////
			//��� ���� �缳��. 12�ð� �Ǹ� ��� ���� ���θ� �缳���Ѵ�.
			///////////////////////////////////////////////////////////////////////////////
//			clIGuild.SetRelationReLoad();
			///////////////////////////////////////////////////////////////////////////////
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////


	// �� �ð� ���� DB�� �����Ѵ�.
	if( PrevSaveTimeToDBTime.Hour != CurrentTime.Hour )
	{
		DBIKWorldTime			DBWorldTime;
		DBWorldTime.Year		=	CurrentTime.Year;
		DBWorldTime.Month		=	CurrentTime.Month;
		DBWorldTime.Day			=	CurrentTime.Day;
		DBWorldTime.Hour		=	CurrentTime.Hour;
	
		// �ð��� ����Ǿ���. DB�� �����Ѵ�.
		clDatabase->writeDBQuery(SO_DB_INFOKIND_SETWORLDTIME, &DBWorldTime, sizeof(DBWorldTime));

		memcpy(&PrevSaveTimeToDBTime, &CurrentTime, sizeof(SOnlineTime));
	}

	// ������ ���� �������� ������ ������ ����ش�
	if(GETTIMEGAP(dwLastSendPayingBusinessTime, dwtimeGetTime) >= 40000)
		SendPayingBusinessAnnoucement();
	
	// ���� ó��
	clISOMap.Poll();

	// ������ ó��
	for(i = 0; i < siTotalVillageNum; i++)
	{
		pclVillage[i].Poll();
	}
	
	// �ʵ忡�ִ� ������ ���� Action ȣ��.
	clFieldMonster.Action();	

	// ���� ó�� 
	m_BattleMgr.ActionPoll();
	
	// ���� ó��
	clSOShip.Poll();	
	
	// �� �������� �ð��� �����Ѵ�.
	memcpy(&PrevCurrentTime, &CurrentTime, sizeof(SOnlineTime));			

	// ��ż� ó�� 
	m_pAuctionMarket->Poll();


	if( IsEvent() )
	{		
		SI32					siDBAccounts[ 3000 ];
		SI32					siRankings[ 3000 ];
		DBIKEvent_ExpoRanking	DBExpoRanking;

		// 30�и��� �ѹ��� DB�� ��ü ������ �Ѱ��ش�.		
		if( ( timeGetTime() - m_dwExpoUpdateRankingTime ) > DWORD( 1800000 ) )
		{
			// ��ŷ�� ���´�.
			m_ExpoRankingSystem.GetAllRank( siDBAccounts, siRankings );

			for( i = 0; i < 3000; i++ )
			{
				DBExpoRanking.siDBAccount	=	siDBAccounts[ i ];
				DBExpoRanking.siRanking		=	siRankings[ i ];

				clDatabase->writeDBQuery( SO_DB_INFOKIND_EVENT_EXPORANKING, &DBExpoRanking, sizeof( DBExpoRanking ) );
			}

			m_dwExpoUpdateRankingTime	=	timeGetTime();
		}	



		UI16		uiRankingData[ 6000 ];
		SI32		siRankingCounter = 0;
		SOPlayer	*pTempPlayer;

		ZeroMemory( uiRankingData, sizeof( uiRankingData ) );

		// 10�и��� �ѹ��� ������ �ִ� ��� �÷��̾��� ������ �����ش�.
		if( ( timeGetTime() - m_dwExpoSendRankingMsgTime ) > DWORD( 600000 ) )
		{
			for(int PlayerCount = 1; PlayerCount < ON_MAX_CONNECTION + 1; PlayerCount++)
			{
				pTempPlayer	=	pPlayerOnAccount[ PlayerCount ];

				if( ( pTempPlayer != NULL ) && 
					( pTempPlayer->GetStatus() == ON_PLAYER_STATUS_INFIELD ) && 
					( pTempPlayer->m_mainState.match( USER_IN_GAME ) ) )
				{
					uiRankingData[ siRankingCounter * 2 ]		= pTempPlayer->uiAccount;
					uiRankingData[ siRankingCounter * 2 + 1 ]	= m_ExpoRankingSystem.GetRank( pTempPlayer->stAccountInfo.siDBAccount );

					siRankingCounter++;
				}			
			}			

			for( i = 0; i < siRankingCounter; i++ )
			{
				pTempPlayer		=	pPlayerOnAccount[ uiRankingData[ i * 2 ] ];

				pTempPlayer->SendFieldMsg( ON_EVENT_USERRANKINGS, SO_SFM_ONLYME, LPVOID( siRankingCounter ), LPVOID( uiRankingData ) );

			}
			
			m_dwExpoSendRankingMsgTime	=	timeGetTime();

		}
	}
	

	return siErrCode;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܰ� ������ �ʱ�ȭ �Ѵ�.
//					��ܰ� ������ ���� �������̿��� ���� �Լ����� �ʱ�ȭ�� ������ �־�� �Ѵ�
//	���� ����	:	2002-04-23 ���� 3:11:11 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOWorld::InitGuildAndVillage()
{	
	OnlineVillageParser		VillageParser;
	VillageHeader			*pVillageHeader;
	SI32					i;
	BOOL					bLoadVillageSuccess = TRUE;
	
	// ������ ���� ������ �ʱ�ȭ �Ѵ�.
	SOVillage::InitStaticVariable(this);

	if(VillageParser.ReadText("Online\\Village\\VillageInfo.txt", NULL) == TRUE)
	{		
		siTotalVillageNum	=	VillageParser.GetTotalVillageNum();

		// ��ܿ� �� ������ ���� �˷��ش�.
		clIGuild.OnceInit(siTotalVillageNum);

		// ����� ��� ������ DB���� ���ͼ� ��� �Ŵ����� �����Ѵ�.
		if (InitGuildFromDB() == FALSE)
		{
			return FALSE;
		}

		pclVillage	=	new	SOVillage[ siTotalVillageNum ];			
		
		// ȭ�Ͽ��� ���� ������ �̿��Ͽ� ������ �ʱ�ȭ �Ѵ�.
		for(i = 0; i < siTotalVillageNum; i++)
		{	
			if( ( pVillageHeader	= VillageParser.GetVillageHeaderIndex( i ) ) == NULL )		return	FALSE;

			if( pclVillage[ i ].Init( pVillageHeader ) == FALSE )
			{
				bLoadVillageSuccess = FALSE;
				break;
			}				
		}

		VillageParser.Free();				
	}	

	return bLoadVillageSuccess;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ Index Code�� ����Ͽ� ������ �����͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage*	SOWorld::GetVillageByCode(UI16 uiVillageCode)
{
	UI16	i;

	if(uiVillageCode > 100)
	{
		for(i = 0; i < siTotalVillageNum; i++)
		{
			if(pclVillage[i].uiVillageCode == uiVillageCode)
				return &pclVillage[i];
		}
	}
	return NULL;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �̸��� ����Ͽ� ������ �����͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage*	SOWorld::GetVillage(CHAR *pszVillageName)
{
	SI32	i;

	for(i = 0; i < siTotalVillageNum; i++)
	{
		if(strcmp(pclVillage[i].GetVillageName(), pszVillageName) == 0)
			return &pclVillage[i];
	}
	
	return NULL;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Ư�� ��ġ���� ���� ����� �ǿ��� ���� ������ ã�´�.
//	���� ����	:	2002-04-22 ���� 11:04:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOWorld::GetNearestHospitalVillageIndex(UI16 uiMapID, SI32 siX, SI32 siY)
{
	SI32	i;
	SI32	siNearestHospitalVillageIndex;
	SI32	siNearestDistance, siTempNearestDistance;
	
	siNearestHospitalVillageIndex	=	-1;
	for(i = 0; i < siTotalVillageNum; i++)
	{
		if(pclVillage[i].GetMapID() == uiMapID && 
			pclVillage[i].DoesStructureExist(ON_VIL_ST_KIND_HOSPITAL) == TRUE)
		{
			if(siNearestHospitalVillageIndex == -1)
			{
				siNearestHospitalVillageIndex	=	i;		
				siNearestDistance		=	abs(pclVillage[i].siVillageX - siX) + abs(pclVillage[i].siVillageY - siY);
			}
			else
			{
				siTempNearestDistance	=	abs(pclVillage[i].siVillageX - siX) + abs(pclVillage[i].siVillageY - siY);
				if(siNearestDistance > siTempNearestDistance)
				{
					siNearestHospitalVillageIndex		=	i;
					siNearestDistance					=	siTempNearestDistance;
				}
			}
		}
	}

	return siNearestHospitalVillageIndex;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���ӵ� ����ڿ��� ���� ������ �˷��ش�.
//					������ �޽����� ��� ����ڿ��� ������ ���Ͽ�
//					�� �÷��̾��� SendFieldMsg�� ������� �ʴ´�
//	���� ����	:	2002-04-23 ���� 1:37:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOWorld::OfficialAnnouncement(DWORD dwResponse,	LPARAM lParam1,	LPARAM lParam2,	LPARAM lParam3, LPARAM lParam4,	LPARAM lParam5,	LPARAM lParam6,	LPARAM lParam7)
{
	SI32						i;
	CHAR						cLocalBuffer[ON_MAX_LOCALBUFFER];
	OnResponseMsg				*pResponseMsg;
	OnOATextMsg					*pTextMsg;
	OnOAVillAttackedMsg			*pVillAttackedMsg;
	OnOAVillOccupiedMsg			*pVillOccupiedMsg;
	OnOAMaxInvChangedMsg		*pMaxInvChangedMsg;
	OnOAGuildCreatedMsg			*pGuildCreatedMsg;
	OnOAGuildDeletedMsg			*pGuildDeletedMsg;	
	OnOAGuildPayingBusinessMsg	*pGuildPayingBusinessMsg;
	OnOASMSMsg					*pSMSMsg;
	CHAR						*pszGuildID1;	
	SI32						siLength;	
	CHAR						*pszText1, *pszText2;
	UI16						uiVillageCode;
	

	ZeroMemory(cLocalBuffer, sizeof(cLocalBuffer));

	pResponseMsg				=	(OnResponseMsg*)cLocalBuffer;
	pResponseMsg->dwMsgType		=	ON_OFFICIAL_ANNOUNCEMENT;
	pResponseMsg->dwResponse	=	dwResponse;

	switch(pResponseMsg->dwResponse)
	{
	case ON_RET_OA_TEXT:
		pTextMsg			=	(OnOATextMsg*)pResponseMsg;
		pszText1			=	(CHAR*)((CHAR*)pTextMsg + sizeof(OnOATextMsg));
		pszText2			=	(CHAR*)lParam1;
		
		// ���������� �ִ� 255�� �ۿ� ���� �� ����
		pTextMsg->dwLength	=	min(strlen(pszText2), 255);

		strncpy(pszText1, pszText2, pTextMsg->dwLength);

		siLength			=	sizeof(OnOATextMsg) + pTextMsg->dwLength;
		break;

	//////////////////////////////////////////////////////////////////////
	// SMS ����
	case ON_RET_OA_SMS_MSG : 
		{
			pSMSMsg	=	(OnOASMSMsg*)cLocalBuffer;

			pSMSMsg->dwCallCount	=	DWORD(lParam1);
			strncpy(pSMSMsg->szUserCode, (char*)lParam2, 32);
			strncpy(pSMSMsg->szOrgPhone, (char*)lParam3, 32);
			strncpy(pSMSMsg->szDstPhone, (char*)lParam4, 32);
			strncpy(pSMSMsg->szCallMessage, (char*)lParam5, 200);
			strncpy(pSMSMsg->szRecvTime, (char*)lParam6, 16);
			strncpy(pSMSMsg->szReserved, (char*)lParam7, 22);
			
			siLength	=	sizeof(OnOASMSMsg);
		}
		break;
	//////////////////////////////////////////////////////////////////////
	case ON_RET_OA_VILL_ATTACKED:
		pVillAttackedMsg	=	(OnOAVillAttackedMsg*)pResponseMsg;		
		
		uiVillageCode		=	UI16(lParam1);
		pVillAttackedMsg->uiVillageCode		=	uiVillageCode;

		siLength	=	sizeof(OnOAVillAttackedMsg);
		break;
		
	case ON_RET_ON_GUILD_GIVEUP:
	case ON_RET_OA_VILL_SURRENDER:
	case ON_RET_OA_VILL_OCCUPIED:
		pVillOccupiedMsg	=	(OnOAVillOccupiedMsg*)pResponseMsg;		

		pszGuildID1			=	(CHAR*)lParam1;		
		uiVillageCode		=	UI16(lParam2);

		strncpy(pVillOccupiedMsg->szGuildID, pszGuildID1, ON_GUILDNAME_LENGTH);
		pVillOccupiedMsg->uiVillageCode		=	uiVillageCode;

		siLength	=	sizeof(OnOAVillOccupiedMsg);
		break;
	
	case ON_RET_OA_MAXINVESTMENT_CHANGED:
		pMaxInvChangedMsg	=	(OnOAMaxInvChangedMsg*)pResponseMsg;
		
		pszText1			=	(CHAR*)lParam1;
		uiVillageCode		=	UI16(lParam2);

		strncpy(pMaxInvChangedMsg->szID, pszText1, ON_ID_LENGTH);
		pMaxInvChangedMsg->uiVillageCode	=	uiVillageCode;

		siLength	=	sizeof(OnOAMaxInvChangedMsg);
		break;
	case ON_RET_OA_GUILD_CREATED:
		pGuildCreatedMsg	=	(OnOAGuildCreatedMsg*)pResponseMsg;		

		pszText1			=	(CHAR*)lParam1;

		strncpy(pGuildCreatedMsg->szGuildID, pszText1, ON_GUILDNAME_LENGTH);

		siLength	=	sizeof(OnOAGuildCreatedMsg);
		break;
	case ON_RET_OA_GUILD_DELETED:
		pGuildDeletedMsg	=	(OnOAGuildDeletedMsg*)pResponseMsg;		

		pszText1			=	(CHAR*)lParam1;

		strncpy(pGuildDeletedMsg->szGuildID, pszText1, ON_GUILDNAME_LENGTH);

		siLength	=	sizeof(OnOAGuildDeletedMsg);
		break;
	case ON_RET_OA_PAYING_BUSINESS:
		pGuildPayingBusinessMsg	=	(OnOAGuildPayingBusinessMsg*)cLocalBuffer;		

		pGuildPayingBusinessMsg->uiItemCode			=	UI16(lParam1);
		pGuildPayingBusinessMsg->uiBuyPrice			=	UI32(lParam2);
		pGuildPayingBusinessMsg->uiBuyVillageCode	=	UI16(lParam3);
		pGuildPayingBusinessMsg->uiSellPrice		=	UI32(lParam4);
		pGuildPayingBusinessMsg->uiSellVillageCode	=	UI16(lParam5);
		
		siLength	=	sizeof(OnOAGuildPayingBusinessMsg);
		break;
	}
	
	for(i = 1; i < ON_MAX_CONNECTION + 1; i++)
	{
		if(pPlayerOnAccount[i] != NULL)
		{			
			pPlayerOnAccount[i]->SendFieldMsg(ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ALLCHAR, LPVOID(cLocalBuffer), LPVOID(siLength));
			break;
		}
	}

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ��带 �о�ͼ� �����Ѵ�.
//	���� ����	:	2002-02-26 ���� 1:20:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL		SOWorld::InitGuildFromDB()
{
	// DB���� �� ����� ���� ���´�.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDLIST, LPVOID(&clIGuild), NULL);

	clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_ITEM, LPVOID(&clIGuild), NULL);


	// DB���� ��ܿ��� ������ ���´�.
	if (clIGuild.SetMemberInfoFromDB() == FALSE) return FALSE;

	// DB���� ����� ���踦 �����Ѵ�.
	clIGuild.SetGuildRelationFromDB();

	// ����ū ��� ���̵� ���´�.
	clIGuild.SetMaxGuildID();

	return TRUE;
}

// DB���� �̺�Ʈ�� ���� �⺻ ������ ���´�.
BOOL		SOWorld::GetEventDataFromDDB()
{
	if( clDatabase->GetDBInfo( SO_DB_INFOKIND_EVENT_EXPO, NULL, LPVOID( &m_ExpoRankingSystem ) ) == FALSE )		return	FALSE;
	
	m_ExpoRankingSystem.sortData();

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾��� �����͸� ���´�.
//	���� ����	:	2002-03-07 ���� 3:53:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer*	SOWorld::GetPlayer(CHAR* pszName)
{
	SI32	i;

	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			if(strcmp(pszName, clPlayer[i]->szID) == 0)			
				return clPlayer[i];					
		}
	}

	return	NULL;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� �ϴ� ������ DB�� ���� ���´�. (���� ������Ʈ)
//	���� ����	:	2002-03-13 ���� 4:48:04 - �̼���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::GetItemDBPrice()
{
	CItemHeader*	pItemHeader;
	SI32			*pItemID;
	SI32			*pDBCost;
	SI32			i;
	SI32			siDBItemNum;

	clDatabase->GetDBInfo(SO_DB_INFOKIND_ITEM_TOTALNUM, &siDBItemNum, NULL);

	pItemID				=	new SI32[siDBItemNum];
	pDBCost				=	new SI32[siDBItemNum];

	clDatabase->GetDBInfo(SO_DB_INFOKIND_ITEM_DBPRICE, pItemID, pDBCost);

	for(i=0; i<siDBItemNum; i++)
	{
		pItemHeader = clItem.GetItem(pItemID[i]);
		if(pItemHeader)
		{
			pItemHeader->m_siDBPrice	=	pDBCost[i];
		}
		else
		{
			if( pItemID[i] !=0)
			{
				printf("Item[%d] exists only in DB. - Delete it from DB!\n", pItemID[i]);
				//printf("������� �����ؼ� �������� ����. �������: mapitem, village_item��� \n");
			}
		}

	}
	if(pItemID)
	{
		delete[]	pItemID;
		pItemID	=	NULL;
	}
	if(pDBCost)
	{
		delete[]	pDBCost;
		pDBCost	=	NULL;
	}	

	// �������� ���� ������ ������ �������Ƿ�
	// ��� ������ �������� ������ �������ش�.
	for(i = 0; i < siTotalVillageNum; i++)
	{
		// �������� ������ �ٽ� �����Ѵ�.
		pclVillage[i].m_pMarket->UpdateAllItemsPrice();
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� �ϴ� ������ DB�� ���� ���´�.(���� ���ý� TEXT����Ÿ�� DB����Ÿ�� ������ Ȯ��)
//	���� ����	:	2002-03-13 ���� 4:48:04 - �̼���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::SetItemDBPrice()
{
	CItemHeader*	pItemHeader;
	SI32			siTotalItemNum=0;
	SI32			i;

	
	siTotalItemNum		=	clItem.GetCount();
	// 0���� �ʱ�ȭ�� �Ǿ��� ���.
	for(i=0; i<siTotalItemNum; i++)
	{
		pItemHeader	=	clItem.GetItem(i);
		if(pItemHeader)
		{			
//			if( pItemHeader->m_siDBPrice	==	0 )
			{
				if(pItemHeader->m_uiID != 0)
				{
					DBIKItem		ItemData;
					ZeroMemory(&ItemData, sizeof(DBIKItem));
					ItemData.ItemID		=	pItemHeader->m_uiID;
					ItemData.Price		=	pItemHeader->m_uiValue;
					ItemData.siUseDay	=	clItem.GetItemUsingDay(pItemHeader->m_uiID);
					
					memcpy(ItemData.Name, clOnlineText.Get(pItemHeader->m_siItemName), 32);
					
					ItemData.Name[31]	=	'\0';
					 
					//clDatabase->SetDBInfo(SO_DB_INFOKIND_ITEM_APPEND, &ItemData, NULL);
					clDatabase->writeDBQuery(SO_DB_INFOKIND_ITEM_APPEND, &ItemData, sizeof(ItemData));

					printf( "[Item:%s(ID:%d,Price:%d)] exists only in text data. -> It will be added to DB.\n",ItemData.Name,ItemData.ItemID,ItemData.Price );
				}
			}
		}
		else
		{
			siTotalItemNum++;
			printf( "Item[%d] exists only in DB. Press [ENTER] key to continue.\n", i );
		}
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ �ְ� ��� ������ �˾ƿ´�.
//	���� ����	:	2002-03-25 ���� 10:18:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOWorld::GetHighestGuildClass(CHAR *pszID)
{
	return	GetHighestGuildClassInVillage(pszID);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ĳ������ �ְ� ��� ������ �˾ƿ´�.
//	���� ����	:	2002-03-25 ���� 10:02:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOWorld::GetHighestGuildClassInVillage(CHAR *pszID)
{
	SI32	siGuildClassCode, siHighestGuildClassCode;
	SI32	i;
		
	// �� �������� �ش� �÷��̾��� �ְ� ������ ���´�.
	siHighestGuildClassCode	=	0;

	for(i = 0; i < siTotalVillageNum; i++)
	{
		siGuildClassCode		=	pclVillage[i].GetHighestGuildClass(pszID);

		// GuildClassCode�� 0�� �ƴϿ��� �Ѵ�.
		// GuildClassCode�� 0�̶�� �����̴�.
		if(siGuildClassCode != 0)
		{
			if(siHighestGuildClassCode == 0) 
			{	
				// ó�� �ѹ��� ���´�.
				siHighestGuildClassCode	=	siGuildClassCode;
			}
			else
			{	
				// �� ������ ���� ������ ���Ѵ�.
				siHighestGuildClassCode	=	clIGuildClass.GetHighClass(siGuildClassCode, siHighestGuildClassCode);							
			}
		}
	}

	return siHighestGuildClassCode;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���Ͱ� ������ ������ ������ �˾ƿ´�.
//	���� ����	:	2003-03-25 ���� 8:43:23 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOWorld::GetOwnVillageNum( CHAR *pszID )
{
	SI32		i;
	SI32		nCount	=	0;

	if( pszID == NULL )		return -1;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		if( pclVillage[i].m_Government->GetInvestmentMaxInvestmentorName() == NULL )		continue;
		if( strcmp(pclVillage[i].m_Government->GetInvestmentMaxInvestmentorName(), pszID) == 0 )
		{
			nCount++;
		}
	}

	return nCount;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� �� ĳ���Ͱ� ������ �ִ� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-05-09 ���� 7:49:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::DeleteGuildClassInVillage(CHAR *pszName)
{
	SI32	i;	
		
	for(i = 0; i < siTotalVillageNum; i++)
	{	
		if(pclVillage[i].DeleteGuildClass(pszName) == TRUE)
		{
			if( pclVillage[i].uiGuildID != 0 )
			{
				// �� ������ ��ܿ��� �����Ǿ�� �Ѵ�.
				clIGuild.DelVillage(pclVillage[i].uiGuildID, &pclVillage[i], FALSE);
			}
		}
	}	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ð��� ���´�.
//	���� ����	:	2002-04-08 ���� 7:10:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::GetRealTime(OnRealWorldTime *pRealWorldTime)
{
	pRealWorldTime->uiMinute	=	pRealTime->tm_min;
	pRealWorldTime->uiHour		=	pRealTime->tm_hour;
	pRealWorldTime->uiDay		=	pRealTime->tm_mday;
	pRealWorldTime->uiMonth		=	pRealTime->tm_mon + 1;	
	pRealWorldTime->uiYear		=	pRealTime->tm_year + 1900;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ش� Code�� ������ �ִ� ������ Village Index�� �˾ƿ´�.
//	���� ����	:	2002-04-15 ���� 5:01:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOWorld::GetVillageIndexByCode(UI16 uiVillageCode)
{
	SI32	i;

	for(i = 0; i < siTotalVillageNum; i++)
	{
		if(pclVillage[i].uiVillageCode == uiVillageCode)	
			return	i;
	}

	return	-1;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǵ� �������� ������ ������ �ش�
//	���� ����	:	2002-04-24 ���� 9:42:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::SendPayingBusinessAnnoucement()
{	
	UI16			uiBuyVillageCode, uiSellVillageCode;
	MONEY			mnBuyPrice, mnSellPrice, mnMinBuyPrice, mnMaxSellPrice;
	SI32			i;
	CItemHeader*	pItemHeader;
		
	// �� �ʱ�ȭ.
	mnMinBuyPrice		=	0x7fffffffffffffff;
	mnMaxSellPrice		=	0;
	uiBuyVillageCode	=	0;
	uiSellVillageCode	=	0;

	if((pItemHeader = clItem.GetItemHeader(uiLastSendPayingBusinessItem)) != NULL)
	{
		// �� �������� ���� �ΰ� �Ĵ� ������ ���� ��ΰ� �Ĵ� ������ ã�´�
		for(i = 0; i < siTotalVillageNum; i++)
		{			
			if(pclVillage[i].m_pMarket->GetBuySellItemPrice(pItemHeader->m_uiID, &mnBuyPrice, &mnSellPrice) == TRUE)
			{	
				if(mnBuyPrice < mnMinBuyPrice)
				{
					mnMinBuyPrice		=	mnBuyPrice;
					uiBuyVillageCode	=	pclVillage[i].uiVillageCode;
				}
				else if(mnSellPrice > mnMaxSellPrice)
				{
					mnMaxSellPrice		=	mnSellPrice;
					uiSellVillageCode	=	pclVillage[i].uiVillageCode;
				}
			}
		}

		// �������� ��� �Ĵ� ������ �ִ��� �˻��Ѵ�.
		if(uiBuyVillageCode == 0 || uiSellVillageCode == 0)	goto	END;

		// �ּ� ������ 1000 �� �̻� ���ƾ� �Ѵ�.
		// �Ǵ� ������ 10% �̻� ���ƾ� �Ѵ�.		
		if(((mnMinBuyPrice + 2000 ) >= mnMaxSellPrice) &&
			((mnMinBuyPrice + (mnMinBuyPrice * 15 / 100)) >= mnMaxSellPrice))	goto	END;	
			
		// ������ �����ش�.
		OfficialAnnouncement(ON_RET_OA_PAYING_BUSINESS,	
						LPARAM(pItemHeader->m_uiID), 
						LPARAM(UI32(mnMinBuyPrice)), LPARAM(uiBuyVillageCode),
						LPARAM(UI32(mnMaxSellPrice)), LPARAM(uiSellVillageCode));
							

		// ������ ���� �ð��� �����Ѵ�.
		dwLastSendPayingBusinessTime	=	dwtimeGetTime;
	}


END:
	uiLastSendPayingBusinessItem++;
	uiLastSendPayingBusinessItem	=	uiLastSendPayingBusinessItem % clItem.GetCount();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���� ������ ����� ���Ѵ�.
//	���� ����	:	2002-05-02 ���� 9:25:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
CharHead*	SOWorld::GetCharHeader(UI16 uiCharKind)
{
	SI32		siTotalCharKindNum	=	clCharParser.GetCharTotalNum();
	SI32		i;
	CharHead*	lpTempCharHead;

	for(i = 0; i < siTotalCharKindNum; i++)
	{
		lpTempCharHead = clCharParser.GetCharHeader(i);
		
		if(uiCharKind == lpTempCharHead->uiID)		
			return	lpTempCharHead;													
	}

	return	NULL;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �����ϰ� �ִ� �ü����� ���´�.
//	���� ����	:	2002-05-09 ���� 3:51:44 - �̼���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
stFarmData*	SOWorld::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		//return	pclVillage[i].GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );		
	}

	return NULL;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� ��ȭ���� ���� Ƚ���� ���������ش�.
//	���� ����	:	2002-05-23 ���� 5:58:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOWorld::IncChatRoomInVillCounter()
{
	dwChatRoomInVillCounter++;

	return	dwChatRoomInVillCounter;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������� ��ȭ���� ���� Ƚ���� ���������ش�.
//	���� ����	:	2002-05-23 ���� 5:58:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOWorld::LogOutAllPlayer()
{	
	for(int i = 1; i < ON_MAX_CONNECTION + 1; i++)
	{
		if( pPlayerOnAccount[ i ] ) {
			pPlayerOnAccount[ i ]->LogOut();
		}
	}
	return 0;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾��� �����͸� ���´�.
//	���� ����	:	2002-07-04 ���� 10:30:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer*	SOWorld::GetPlayer(UI16 uiAccount)
{
	// �÷��̾�� ��ȿ�� Account ������ ����� NULL�� ���Ͻ�Ų��.
	if( !ISPLAYER(uiAccount) )					return NULL;

	if(pPlayerOnAccount[uiAccount] != NULL)		return pPlayerOnAccount[uiAccount];
	else										return	NULL;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�⺻ ĳ������ �����͸� ���´�.
//	���� ����	:	2002-07-27 ���� 4:12:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BaseChar*	SOWorld::GetBaseChar(UI16 uiAccount)
{
	if(ISPLAYER(uiAccount) == TRUE)				return	(BaseChar*)GetPlayer(uiAccount);	
	else if(ISMONSTER(uiAccount) == TRUE)		return	(BaseChar*)clMonster.GetMonster(uiAccount);	
	else if(ISSHIP(uiAccount) == TRUE)			return	(BaseChar*)clSOShip.GetShip(uiAccount);

	return	NULL;
}	


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB Account�� �̿��Ͽ� �÷��̾��� �����͸� ���´�.
//	���� ����	:	2002-08-07 ���� 7:52:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer*	SOWorld::GetPlayerByDBAccount(DWORD dwDBAccount)
{
	SI32		i;
	SOPlayer	*pPlayer;

	for(i = 1; i < ON_MAX_CONNECTION + 1; i++)
	{
		if(pPlayerOnAccount[i] != NULL)
		{		
			pPlayer		=	pPlayerOnAccount[i];

			if(pPlayer->stAccountInfo.siDBAccount == dwDBAccount)
				return	pPlayer;
		}
	}	
	
	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB Account�� DBSlot�� �̿��Ͽ� �÷��̾��� �����͸� ���´�.
//	���� ����	:	2002-10-15 ���� 2:59:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOPlayer*	SOWorld::GetPlayerByDBAccount( DWORD dwDBAccount, UI08 uiDBSlot )
{
	SOPlayer	*pPlayer;

	if( ( pPlayer = GetPlayerByDBAccount( dwDBAccount ) ) == NULL )		return	NULL;
	if( pPlayer->stAccountInfo.uiSlot != uiDBSlot )						return	NULL;

	return	pPlayer;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ��� ������ ������ �ִ��� �˾ƿ´�.
//	���� ����	:	2002-10-15 ���� 1:27:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOWorld::IsIndustrialComplexOwner( SOPlayer *pPlayer )	
{
	SI32	i;

	
	for( i = 0; i < siTotalVillageNum; i++ )
	{
		if( pclVillage[ i ].IsIndustrialComplexOwner( pPlayer ) == TRUE )		return	TRUE;
	}

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾��� ��������� ���´�.
//	���� ����	:	2002-10-15 ���� 1:27:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex  *SOWorld::GetIndustrialComplex( SOPlayer *pPlayer )	
{

	SOVillage_IndustrialComplex  *pRet = NULL;
	

	
	for( SI32 i = 0; i < siTotalVillageNum; i++ )
	{
		pRet = pclVillage[ i ].GetIndustrialComplexOwner( pPlayer );
		if (pRet != NULL) break;
	}

	return	pRet;
}

BOOL SOWorld::SellIndustrialComplex(SOPlayer *pOldOwner,SOPlayer *pNewOwner)
{
	SOVillage_IndustrialComplex  *pComplex = NULL;
	// null�̸� �ȵ�
	if (pOldOwner == NULL || pNewOwner == NULL) return FALSE;
	// ������ ������ �̹� ������������ �ȵ�
	if (IsIndustrialComplexOwner(pNewOwner)) return FALSE;

	// �������� �����̾����� �ȵ�.
	pComplex = GetIndustrialComplex(pOldOwner);
	if (!pComplex) return FALSE;


	// ������ �ٲ���
	pComplex->ChangeOwner(pNewOwner);
	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���۵Ǿ���.
//	���� ����	:	2003-01-14 ���� 3:34:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::EnterAttackVillageTimeZone()
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		pclVillage[ i ].EnterAttackVillageTimeZone();
	}
	m_IsAttackVillageTimeZone	=	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ������.
//	���� ����	:	2003-01-14 ���� 3:35:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::LeaveAttackVillageTimeZone()
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		pclVillage[ i ].LeaveAttackVillageTimeZone();
	}
//	m_bVillageAttack			=   FALSE;
	m_IsAttackVillageTimeZone	=	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� ���������� �˾ƿ´�.
//	���� ����	:	2003-01-14 ���� 3:35:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOWorld::IsAttackVillageTimeZone()
{
	return	m_IsAttackVillageTimeZone;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʵ忡 �ִ� ��� �������� �������� ���ҽ�Ų��.
//	���� ����	:	2003-04-09 ���� 2:13 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::ChangeSatietyByWeather()
{
	// ���̳� �� ���� ��쿡 �������� ���ҽ�Ų��.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		// ������ ���� ���� �ʵ忡 �ִ� ���·� ���ֵȴ�.
		// ���̳� �� ������ �ʴ� �ʵ� �����Ƿ� �÷��̾ �ִ� ���� ���̰� �� ���� �� �ִ� ������ üũ�Ѵ�.
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			if( clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) || clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) )
			{
				pPlayerOnAccount[i]->ChangeSatiety( -10 );
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ���� ������ �����ϰ� ������ �����Ѵ�.
//	���� ����	:	2003-04-09 ���� 4:16 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOWorld::MakeThunder()
{
	UI16		uiMapID;
	POINT		ptPos;
	SI32		siAccount[ON_MAX_CONNECTION];		// ������ ���� ���ɼ��� ���� �÷��̾���� ��ī��Ʈ ����Ʈ
	SI32		numAdded	=	0;					// ����Ʈ�� �߰��� �÷��̾��� ��
	SI32		siSelectedAccount;					// ������ �°� �� �÷��̾�

	// ������ ���� ���ɼ��� ���� ��� �÷��̾ ����Ʈ�� �ִ´�.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[i]->GetPos( &uiMapID, &ptPos );		// �÷��̾ �ִ� �ʰ� ��ǥ�� ���´�.

			if( clISOMap.CanBeRainy(uiMapID) )	// �� ���� �� �ִ� �����ΰ�?
			{
				// 0������ ��� y��ǥ 524�� �������� ���ʿ� �ִ� �������� �� ������. (�ϵ��ڵ�-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				siAccount[numAdded++]	=	i;
			}
		}
	}

	if( numAdded == 0 )		return;								// ������ ����� �Ѹ� ������ ����
	siSelectedAccount	=	siAccount[rand() % numAdded];		// ����Ʈ���� ���� ���� �÷��̾ �Ѹ� ����.


	// ���� ���� �÷��̾�� ü���� ���ҽ�Ű�� �� ����� ���� ���� ���ɼ��� �ִ� ��� �÷��̾�� �˸���.	
	pPlayerOnAccount[siSelectedAccount]->IFollower.AllCharacterDeleteLifePercent( 5 );			// ��� �뺴�� ü���� 5% ���ҽ�Ŵ
	for( i = 0; i < numAdded; i++ )
	{
		if( pPlayerOnAccount[siAccount[i]] != NULL && pPlayerOnAccount[siAccount[i]]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[siAccount[i]]->GetPos( &uiMapID, &ptPos );		// �÷��̾ �ִ� �ʰ� ��ǥ�� ���´�.

			if( clISOMap.CanBeRainy(uiMapID) )	// �� ���� �� �ִ� �����ΰ�?
			{
				// 0������ ��� y��ǥ 524�� �������� ���ʿ� �ִ� �������� �� ������. (�ϵ��ڵ�-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				// ��ο��� �޽��� ����
				pPlayerOnAccount[siAccount[i]]->SendFieldMsg( ON_MAKE_THUNDER, SO_SFM_ONLYME, (LPVOID)pPlayerOnAccount[siSelectedAccount]->uiAccount );
			}
		}
	}
}