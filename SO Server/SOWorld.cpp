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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// SOWorld 儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOWorld::SOWorld(SOServer *clTempServer) : 
clSOFindPath(MAX_ONLINEMAP_XSIZE, MAX_ONLINEMAP_YSIZE), Timer(g_GameServerInfoParser.IsTestServer()),
m_bGameGuard(g_GameServerInfoParser.IsGameGuard())
{
	SI32			i;	
//	HQFileManager	hqFile;
	
	srand( (unsigned int)time( NULL ) );

	// 煎斜 衙棲盪蒂 蟾晦�� 衛鑑棻.
	m_LogMgr.Init();


	// 臢蝶お蒂 煎萄и棻.

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

	
	// 衙濰脹 嬴檜蠱 薑爾蒂 だ橾縑憮 檗橫螞棻 
	if( !m_buriedItemParser.readFile("Online\\Item\\ItemforField.txt") )
	{
		printf(" ItemForField.txt Error! \n ");
		getch();
	}

	// 憮幗 贗溯蝶蒂 翱唸и棻.
	clServer		= clTempServer;

	// clWorld諦 clServer 贗溯蝶陛 clDatabase 薑爾蒂 奢嶸и棻.
	clDatabase		= clServer->clDatabase;
	clLoginDatabase = clServer->clLoginDatabase;
	clItemDatabase	= clServer->clItemDatabase;

	// 樣棻遴 囀萄
	siErrCode		= 2;

	siWeatherChangeTime = 5;
	bNowWeather         = FALSE;
	m_bQuestEventUpdate = FALSE;
	m_bWarEnd			= FALSE;
	m_siEventItemCount  = 0;

	

	dwLastThunderCheckTime	=	timeGetTime();

	// 衛除 撲薑 
	//------------------------------------------------------------
	time(&stClock);                         //Day Timer蒂 撲薑и棻 
	pRealTime = localtime(&stClock);        //base 衛除擊 撲薑и棻
	//------------------------------------------------------------	

//	try{
//		m_pQuestManager = new CQuestManager;
//	}
//	catch(std::bad_alloc){
//		assert(0);
//	}
//	// 絮紫 蠡蝶お蒂 諫猿и 餌辨濠 薑爾蒂 DB縑憮 橢橫螞棻.
	m_uiDokdoQuestCompletedUserCount = 0;
	//SI16	siQuestCode = DOKDO_QUEST_CODE2;
	//clDatabase->GetDBInfo(SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST, &m_uiDokdoQuestCompletedUserCount, &siQuestCode);


	// 唳я纂 薑爾蒂 煎萄и棻.
	LoadExpData( g_GameServerInfoParser.IsTestServer() );

	// Text蒂 蟾晦�倆挬�.
	clOnlineText.Init(NULL, szNationName,NULL);

	// 議葛攪曖 World蒂 撲薑и棻.
	BaseChar::InitStaticVariable(this);		

	// Profit 檣攪む檜蝶蒂 蟾晦�� и棻.
	printf( "--- Loading Profit Interface... " );	
	if( clIProfit.Init("Online\\Village\\Profit.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// GuildClass曖 檣攪む檜蝶蒂 蟾晦�� и棻.
	printf( "--- Loading GuildClass Interface... " );	
	if( clIGuildClass.Init("Online\\Village\\Grade.txt") == TRUE )
		printf( "[Completed]\n" );
	else
	{
		printf( "[Failed]\n" );
		exit(1);
	}
	
	// Guild曖 World蒂 撲薑и棻.
	// 奩萄衛 clIGuildClass.Init() 壽縑 氈橫撿 и棻.
	cltIGuild::InitStaticVariable(this);		


	////////////////////////////////////////////////
	// Ы溯檜橫蒂 儅撩и棻.
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
	

	// 寡曖 薑爾蒂 轎溘и棻.
	printf( "--- Analyzing Ships' Info... " );
	clIOnlineShipKI.Init();
	printf( "[Completed]\n" );

	// 寡曖 檜翕縑 婦溼脹 偌羹菟擊 蟾晦�� 衛鑑棻.
	printf( "--- Analyzing Ships' course... " );
	clOnlineSMI.Init();
	printf( "[Completed]\n" );
	

	printf( "--- Loading Credit Info... " );
	clCredit.Init("Online\\Add_Credit.txt");
	printf( "[Completed]\n" );

	////////////////////////////////////////////////
	// ん驍 �倣狨� 翱棻.
	////////////////////////////////////////////////	
	printf( "--- Creating Portals... " );
	if( Portal.Init("Online\\Map\\MapPortal.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	////////////////////////////////////////////////
	// 裘擊 儅撩и棻.
	////////////////////////////////////////////////	
	printf( "--- Creating Maps... " );
	ISOMap::InitStaticVariable(this);
	if(clISOMap.Init() == TRUE)	
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );	


	////////////////////////////////////////////////
	// 啗薑縑 婦溼脹 薑爾蒂 煎萄и棻.
	////////////////////////////////////////////////	
	
	clOnlineEconomy.Init("Online\\Village\\Economy.txt");
	
	
	////////////////////////////////////////////////
	// 儅骯 勒僭縑 婦溼脹 薑爾蒂 煎萄и棻.
	////////////////////////////////////////////////
	printf( "--- Loading Manufacturing Plant... " );
	if( clBuilding.Init() == FALSE )
		printf( " [Failed]\n" );
	else
		printf( " [Completed]\n" );

	////////////////////////////////////////////////
	// 嬴檜蠱擊 煎萄и棻.
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

	// TradeParser蒂 檗橫螞棻.
	pTradeParser	=	new	OnlineTradeParser;	

	// IFolower 偌羹蒂 蟾晦�� и棻.
	// 奩萄衛 clItem婁 pTradeParser陛 儅撩脹 �醴� п撿 и棻.
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

	//QuestEventItem Parser縑 婦п 盪濰.
	m_QuestEventItemMgr = new CQuestEventItemMgr;
	m_QuestEventItemMgr->Init(clQuestParser.m_siEventQuestCount);

	//DB縑憮 橢橫螞 嬴檜蠱 偎熱蒂 盪濰.
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
	// DB煎睡攪 в萄縑 雲橫螳 氈朝 嬴檜蠱曖 薑爾蒂 橢橫諦憮 裘縑 撲薑и棻.
	///////////////////////////////////////////////////////////////////////
	clISOMap.SetItemFromDB();
	
	// ⑷營 衛除擊 DB煎 睡攪 橢橫螞棻.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GETWORLDTIME, NULL, &CurrentTime);
	
	// DB縑憮 橢橫螞 衛除擊 撲薑и棻.
	Timer.Create(CurrentTime);
	PrevSaveTimeToDBTime = PrevCurrentTime	=	CurrentTime;

	printf( "Current Time : %d/%d/%d %d:00\n", CurrentTime.Month, CurrentTime.Day, CurrentTime.Year, CurrentTime.Hour);
	// 葆擊擊 煎萄и棻.
	// 奩萄衛 InitGuildFromDB() 嬴楚縑 氈橫撿 и棻.
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

	// 遴艙濠曖 薑爾蒂 橢橫螞棻.
	clDatabase->GetDBInfo( SO_DB_INFO_KIND_GET_GM_LIST, NULL, &m_clGMManager );

	GetItemDBPrice();
	SetItemDBPrice();

	// 葆擊擊 煎萄ц戲嘎煎 陝 顫橾縑 陛梱遴 葆擊擊 撲薑п遽棻.  
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


	// NPC曖 謙盟 薑爾蒂 �倣狤□� 檗橫螞棻.
	printf( "--- Loading NPC KI Interface... " );
	if( clSOCharKI.Init(&clOnlineText, "Online\\CharInfo\\Character List.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// 跨蝶攪蒂 蟾晦�� и棻.
	printf( "--- Loading Monsters... " );	
	clMonster.Init(this);
	printf( " [Completed]\n" );

	// в萄 跨蝶攪 婦葬濠蒂 蟾晦�� и棻.
	printf( "--- Loading Field Monster Interface... " );	
	clFieldMonster.Init(&clMonster, &clISOMap);
	printf( "[Completed]\n" );

	//檜漸お 蠡蝶お 薑爾蒂 橢橫螞棻.

	// Text �倣狨� п薯и棻.
//	hqFile.Free();

	BindJXFile::GetInstance()->destroy();
	

	// 寡蒂 蟾晦�� и棻.
	clSOShip.Init();

	// Find Path蒂 薑瞳 滲熱蒂 蟾晦�� и棻.
	clSOFindPath.InitStaticVariable(&clISOMap);

	// Command蒂 蟾晦�倆挬�.
	clCommand.Init(this);	

	dwLastSendPayingBusinessTime	=	0;
	uiLastSendPayingBusinessItem	=	0;

	bUpdateTime		=	FALSE;

	// dwtimeGetTime擊 蟾晦�� и棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// SOWorld 模資濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOWorld::~SOWorld()
{
	SI32	i;

	if( m_pAuctionMarket ) delete m_pAuctionMarket;
	if( m_pSmith ) delete m_pSmith;

	//-------------------------------------------------------------------------------
	// 葆擊擊 薯剪и棻.
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
	// 蠡蝶お 薑爾蒂 薯剪и棻.
	//-------------------------------------------------------------------------------
//	if(m_pQuestManager != NULL)
//	{
//		delete m_pQuestManager;
//		m_pQuestManager = NULL;
//	}

	//-------------------------------------------------------------------------------
	// 裘擊 薯剪и棻.
	//-------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------
	// Ы溯檜橫蒂 薯剪и棻.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Player Instances... " );
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		delete clPlayer[i];

		clPlayer[i] = NULL;
	}
	printf( "[Completed]\n" );


	//-------------------------------------------------------------------------------
	// 跨蝶攪蒂 薯剪и棻.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Monsters... " );
	clMonster.Free();
	printf( "[Completed]\n" );

	printf( "--- Deleting Ships... " );
	clSOShip.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// 裘擊 薯剪и棻.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Maps... " );
	clISOMap.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// 嬴檜蠱擊 薯剪и棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫諦 婦溼脹 賅萇 翱骯 籀葬 (嘐蕾樓檣陛?, 蕾樓醞檣陛?, 渠晦褒檣陛?, 渠�食磍帡�?, 啪歜醞檣陛?)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32 SOWorld::Poll()
{
	SI32	i;
	char	InfoBuf[1024];
	
	dwtimeGetTime	=	timeGetTime();	

	time(&stClock);                         // 褒薯 ⑷營 衛除擊 橢橫螞棻.
	pRealTime = localtime(&stClock); 
	GetRealTime(&RealWorldTime);

	CurrentTime		=		Timer.GetNowTime();	
	
	bHourChanged = bDayChanged = bMonthChanged = bYearChanged = FALSE;

	//衛除 濠薑檜 剩擊 陽 釭醞縑 熱薑п還匙.
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
			//鼻欽 婦啗 營撲薑.
			
			clIGuild.SetRelationReLoad();
			m_bQuestEventUpdate = FALSE;	
		}
	}

	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))
	{    //奢撩ж朝 陳, 衛除縑 憮幗蒂 籀擠 褒чж賊
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour)) // 8 ~ 10
		{	//奢撩ん晦 薑爾蒂 陛螳螞棻.		
			if(!m_bGiveUp)
			{
				clIGuild.DBGetGiveUpInfo();
				m_bGiveUp = TRUE;
			}						
		}
		else 
		{   //奢撩ж朝 衛除檜 嬴棋唳辦 奢撩嶸棉 纔檜綰蒂 餉薯 и棻.
			if(!m_bFlag)
			{
				clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
				m_bFlag = TRUE;
			}
		}
	}

	// 衛除檜 滲唳腎歷朝雖 羹觼и棻.
	if(CurrentTime.Hour != PrevCurrentTime.Hour)
	{
		// 衛除檜 滲唳腎歷棻.
		bHourChanged	=	TRUE;
	
		// 陳噢 滲唳瞪曖 1衛除翕寰 換檜釭 綠陛 頂溜棻賊 ん虜馬擊 馬模衛鑑棻.
		// 綠陛 斜纂朝 衛薄縑 ん虜馬擊 馬模衛酈晦 嬪п 橾睡楝 陳噢 籀葬 檜瞪縑 嬪纂衛儷棻.
		//if( bNowWeather )		ChangeSatietyByWeather();

		siWeatherChangeTime--;
		if(siWeatherChangeTime < 0)
		{
			BOOL		fEvent;

			// Event煎 12錯 24橾婁 25擎 о鼻 換檜 螃啪 л
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
			// 陳噢 Change
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

			// 瞪 User縑啪 陳噢 滲唳 詭撮雖
			for(int PlayerCount = 1; PlayerCount < ON_MAX_CONNECTION + 1; PlayerCount++)
			{
				if(pPlayerOnAccount[PlayerCount] != NULL)
				{
					pPlayerOnAccount[PlayerCount]->SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(bNowWeather));
				}
			}
		}	// if(siWeatherChangeTime < 0)

		// 蹂橾檜 滲唳腎歷朝雖 羹觼и棻.
		if(CurrentTime.Day != PrevCurrentTime.Day)
		{			
			// 蹂橾檜 滲唳腎歷棻.
			bDayChanged	=	TRUE;
			
			// ж瑞陛 雖陬擊衛 ん驍縑憮 п輿橫撿 й 濛機蛔擊 п遽棻.
			Portal.DayChanged();

			sprintf(InfoBuf,"[Date : %4d-%2d-%2d]   Max[%4d] Current[%4d]",CurrentTime.Year,CurrentTime.Month,CurrentTime.Day,
				clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer);
//			clServer->WriteInfo("..\\ConnectStatus.txt",InfoBuf);
			writeInfoToFile("ConnectStatus.txt",InfoBuf);

			// 錯檜 滲唳腎歷朝雖 羹觼и棻.
			if(CurrentTime.Month != PrevCurrentTime.Month)
			{
				// 錯檜 滲唳腎歷棻.
				bMonthChanged	=	TRUE;

				// 喇檜 滲唳腎歷朝雖 羹觼и棻.
				if(CurrentTime.Year != PrevCurrentTime.Year)
				{
					// 喇檜 滲唳腎歷棻.
					bYearChanged	=	TRUE;
					printf("Year Changed!!\n");		

					// 葆擊 轎殮濠熱蒂 蟾晦�� и棻.
					for(i = 0; i < siTotalVillageNum; i++)
					{
						pclVillage[i].PrepareVillageInOutPlayer();
					}
				}

				printf("Month Changed!!\n");		
			}

			printf("Day Changed!!\n");		
		}
	}	// end of if( 衛除檜 滲唳腎歷朝雖 羹觼и棻 )

	// 廓偃蒂 儅撩й 匙檣雖 1蟾縑 и廓噶 羹觼и棻.
	if( dwtimeGetTime - dwLastThunderCheckTime >= 1000 )
	{
		dwLastThunderCheckTime	=	dwtimeGetTime;
		// 綠 傳朝 換檜 頂葬堅 氈朝 鼻鷓罹撿 и棻.
		if( bNowWeather && rand() % 60 == 0 )		MakeThunder();
	}


	///////////////////////////////////////////////////////////////////////////////////////
	// 褒薯衛除 11衛縑 憮幗縑憮 檣掘蒂 機等檜お и棻. 
	// 褒薯 檣掘機等檜お朝 SoVillage縑憮 給萼棻.SetDBPopulation();
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

				// 億漁 4衛縑 ж朝 濛機  - 嬴檜蠱 陛問 滲翕
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
	// 奢撩 衛除擊 匐餌и棻. 纔蝶お 辨擊 嬪п 奢撩衛除擊 PM:8 ~ 10衛煎 и棻.
	///////////////////////////////////////////////////////////////////////////////////////
//	if(pRealTime->tm_wday == 2 || pRealTime->tm_wday == 4) //��, 跡蹂橾檣雖 羹觼
	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))// IsWarCheck(pRealTime->tm_wday, pRealTime->tm_hour))
	{
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour))
		{
			if( IsAttackVillageTimeZone() == FALSE )
			{
				EnterAttackVillageTimeZone();

				printf("滲褐 陛棟 瞪歎 衛濛...");
				clIGuild.SendWarTime(); //奢撩衛除檜 腎歷棻堅 憲溥遽棻.				
				printf("滲褐 陛棟 瞪歎 部\n");			
			}
		}
		else
		{
			if( IsAttackVillageTimeZone() == TRUE )
			{
				LeaveAttackVillageTimeZone();
				/////////////////////////////////////////////////////////////////////////////////
				//奢撩檜 部陪棻賊 奢撩 議葛攪蒂 橾奩 議葛攪煎 夥紱堅 棻艇薑爾菟擊 撢た.
				//SOPlayer::Poll縑憮 議葛攪曖 薑爾蒂 餉薯и棻.
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
			//奢撩檜 部陪棻賊 奢撩 議葛攪蒂 橾奩 議葛攪煎 夥紱堅 棻艇薑爾菟擊 撢た.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
			m_bWarEnd = TRUE;
			/////////////////////////////////////////////////////////////////////////////////

			//奢撩ん晦 婦溼薑爾蒂 蟾晦�� п遽棻.
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
	// 奢撩 ァ癱掏и擊 陳萼棻.嫘 12衛縑 堅薑.
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
			//憮幗曖 癱濠掏и擊 陳溥遽棻.
			clIGuild.SetPowerInvestmentEnd();
			//DB縑 癱濠掏и擊 陳萼棻堅 詭撮雖蒂 爾魚棻.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_POWER_INVESTMENT_END, NULL, 0);
			bPowerInvestmentEnd = FALSE;

			///////////////////////////////////////////////////////////////////////////////
			//鼻欽 婦啗 營撲薑. 12衛陛 腎賊 鼻欽 奢撩 罹睡蒂 營撲薑и棻.
			///////////////////////////////////////////////////////////////////////////////
//			clIGuild.SetRelationReLoad();
			///////////////////////////////////////////////////////////////////////////////
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////


	// 衙 衛除 葆棻 DB縑 盪濰и棻.
	if( PrevSaveTimeToDBTime.Hour != CurrentTime.Hour )
	{
		DBIKWorldTime			DBWorldTime;
		DBWorldTime.Year		=	CurrentTime.Year;
		DBWorldTime.Month		=	CurrentTime.Month;
		DBWorldTime.Day			=	CurrentTime.Day;
		DBWorldTime.Hour		=	CurrentTime.Hour;
	
		// 衛除檜 滲唳腎歷棻. DB縑 盪濰и棻.
		clDatabase->writeDBQuery(SO_DB_INFOKIND_SETWORLDTIME, &DBWorldTime, sizeof(DBWorldTime));

		memcpy(&PrevSaveTimeToDBTime, &CurrentTime, sizeof(SOnlineTime));
	}

	// 檜櫛檜 陴朝 嬴檜蠱曖 薑爾蒂 奢雖煎 嗥橫遽棻
	if(GETTIMEGAP(dwLastSendPayingBusinessTime, dwtimeGetTime) >= 40000)
		SendPayingBusinessAnnoucement();
	
	// 裘曖 籀葬
	clISOMap.Poll();

	// 葆擊曖 籀葬
	for(i = 0; i < siTotalVillageNum; i++)
	{
		pclVillage[i].Poll();
	}
	
	// в萄縑氈朝 跨蝶攪曖 婦葬 Action ��轎.
	clFieldMonster.Action();	

	// 瞪癱 籀葬 
	m_BattleMgr.ActionPoll();
	
	// 摹夢 籀葬
	clSOShip.Poll();	
	
	// 瞪 Щ溯歜曖 衛除擊 盪濰и棻.
	memcpy(&PrevCurrentTime, &CurrentTime, sizeof(SOnlineTime));			

	// 唳衙模 籀葬 
	m_pAuctionMarket->Poll();


	if( IsEvent() )
	{		
		SI32					siDBAccounts[ 3000 ];
		SI32					siRankings[ 3000 ];
		DBIKEvent_ExpoRanking	DBExpoRanking;

		// 30碟葆棻 и廓噶 DB縑 瞪羹 牖嬪蒂 剩啖遽棻.		
		if( ( timeGetTime() - m_dwExpoUpdateRankingTime ) > DWORD( 1800000 ) )
		{
			// 楨韁擊 橢橫螞棻.
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

		// 10碟葆棻 и廓噶 蕾樓п 氈朝 賅萇 Ы溯檜橫曖 牖嬪蒂 爾頂遽棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽婁 葆擊擊 蟾晦�� и棻.
//					鼻欽婁 葆擊擎 憮煎 曖襄瞳檜罹憮 偽擎 л熱縑憮 蟾晦�飛� 熱чп 輿橫撿 и棻
//	熱薑 橾濠	:	2002-04-23 螃�� 3:11:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOWorld::InitGuildAndVillage()
{	
	OnlineVillageParser		VillageParser;
	VillageHeader			*pVillageHeader;
	SI32					i;
	BOOL					bLoadVillageSuccess = TRUE;
	
	// 葆擊曖 薑瞳 滲熱蒂 蟾晦�� и棻.
	SOVillage::InitStaticVariable(this);

	if(VillageParser.ReadText("Online\\Village\\VillageInfo.txt", NULL) == TRUE)
	{		
		siTotalVillageNum	=	VillageParser.GetTotalVillageNum();

		// 鼻欽縑 識 葆擊曖 熱蒂 憲溥遽棻.
		clIGuild.OnceInit(siTotalVillageNum);

		// 鼻欽曖 賅萇 薑爾蒂 DB縑憮 橢橫諦憮 鼻欽 衙棲盪縑 撲薑и棻.
		if (InitGuildFromDB() == FALSE)
		{
			return FALSE;
		}

		pclVillage	=	new	SOVillage[ siTotalVillageNum ];			
		
		// �倣狤□� 檗擎 薑爾蒂 檜辨ж罹 葆擊擊 蟾晦�� и棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 Index Code蒂 餌辨ж罹 葆擊曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 檜葷擊 餌辨ж罹 葆擊曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	か薑 嬪纂縑憮 薯橾 陛梱遴 曖錳擊 陛雖 葆擊擊 瓊朝棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:04:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 蕾樓脹 餌辨濠縑啪 奢雖 餌о擊 憲溥遽棻.
//					翕橾и 詭衛雖蒂 賅萇 餌辨濠縑啪 爾頂晦 嬪ж罹
//					陝 Ы溯檜橫曖 SendFieldMsg蒂 餌辨ж雖 彊朝棻
//	熱薑 橾濠	:	2002-04-23 螃�� 1:37:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		
		// 奢雖餌о擎 譆渠 255濠 夤縑 爾鳥 熱 橈棻
		pTextMsg->dwLength	=	min(strlen(pszText2), 255);

		strncpy(pszText1, pszText2, pTextMsg->dwLength);

		siLength			=	sizeof(OnOATextMsg) + pTextMsg->dwLength;
		break;

	//////////////////////////////////////////////////////////////////////
	// SMS 婦溼
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 望萄蒂 檗橫諦憮 撲薑и棻.
//	熱薑 橾濠	:	2002-02-26 螃�� 1:20:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL		SOWorld::InitGuildFromDB()
{
	// DB縑憮 識 望萄曖 熱蒂 橢橫螞棻.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDLIST, LPVOID(&clIGuild), NULL);

	clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_ITEM, LPVOID(&clIGuild), NULL);


	// DB縑憮 鼻欽錳曖 薑爾蒂 橢橫螞棻.
	if (clIGuild.SetMemberInfoFromDB() == FALSE) return FALSE;

	// DB縑憮 望萄曖 婦啗蒂 撲薑и棻.
	clIGuild.SetGuildRelationFromDB();

	// 陛濰贖 望萄 嬴檜蛤蒂 橢橫螞棻.
	clIGuild.SetMaxGuildID();

	return TRUE;
}

// DB縑憮 檜漸お縑 渠и 晦獄 薑爾蒂 橢橫螞棻.
BOOL		SOWorld::GetEventDataFromDDB()
{
	if( clDatabase->GetDBInfo( SO_DB_INFOKIND_EVENT_EXPO, NULL, LPVOID( &m_ExpoRankingSystem ) ) == FALSE )		return	FALSE;
	
	m_ExpoRankingSystem.sortData();

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫曖 ん檣攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-07 螃�� 3:53:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 滲翕 ж朝 陛問擊 DB煎 睡攪 橢橫螞棻. (衙橾 機等檜お)
//	熱薑 橾濠	:	2002-03-13 螃�� 4:48:04 - 檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
				//printf("雖選溥賊 犒濩п憮 熱翕戲煎 雖錶. 堅溥餌о: mapitem, village_item蛔蛔 \n");
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

	// 嬴檜蠱曖 晦遽 陛問擊 億煎檜 橢橫諮戲嘎煎
	// 賅萇 葆擊曖 嬴檜蠱曖 陛問擊 偵褐п遽棻.
	for(i = 0; i < siTotalVillageNum; i++)
	{
		// 嬴檜蠱曖 陛問擊 棻衛 唸薑и棻.
		pclVillage[i].m_pMarket->UpdateAllItemsPrice();
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 滲翕 ж朝 陛問擊 DB煎 睡攪 橢橫螞棻.(憮幗 睡た衛 TEXT等檜顫諦 DB等檜顫陛 偽擎雖 �挫�)
//	熱薑 橾濠	:	2002-03-13 螃�� 4:48:04 - 檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::SetItemDBPrice()
{
	CItemHeader*	pItemHeader;
	SI32			siTotalItemNum=0;
	SI32			i;

	
	siTotalItemNum		=	clItem.GetCount();
	// 0戲煎 蟾晦�降� 腎歷擊 唳辦.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 譆堅 鼻欽 霜晝擊 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-25 螃瞪 10:18:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOWorld::GetHighestGuildClass(CHAR *pszID)
{
	return	GetHighestGuildClassInVillage(pszID);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑憮 議葛攪曖 譆堅 鼻欽 霜晝擊 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-25 螃瞪 10:02:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOWorld::GetHighestGuildClassInVillage(CHAR *pszID)
{
	SI32	siGuildClassCode, siHighestGuildClassCode;
	SI32	i;
		
	// 陝 葆擊縑憮 п渡 Ы溯檜橫曖 譆堅 霜晝擊 橢橫螞棻.
	siHighestGuildClassCode	=	0;

	for(i = 0; i < siTotalVillageNum; i++)
	{
		siGuildClassCode		=	pclVillage[i].GetHighestGuildClass(pszID);

		// GuildClassCode陛 0檜 嬴棲罹撿 и棻.
		// GuildClassCode陛 0檜塭賊 縑楝檜棻.
		if(siGuildClassCode != 0)
		{
			if(siHighestGuildClassCode == 0) 
			{	
				// 籀擠 и廓虜 菟橫螞棻.
				siHighestGuildClassCode	=	siGuildClassCode;
			}
			else
			{	
				// 舒 霜晝醞 堪擎 霜晝擊 鏃и棻.
				siHighestGuildClassCode	=	clIGuildClass.GetHighClass(siGuildClassCode, siHighestGuildClassCode);							
			}
		}
	}

	return siHighestGuildClassCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪陛 爾嶸и 葆擊曖 偃熱蒂 憲嬴螞棻.
//	熱薑 橾濠	:	2003-03-25 螃�� 8:43:23 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑憮 檜 議葛攪陛 陛雖堅 氈朝 霜晝擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 7:49:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::DeleteGuildClassInVillage(CHAR *pszName)
{
	SI32	i;	
		
	for(i = 0; i < siTotalVillageNum; i++)
	{	
		if(pclVillage[i].DeleteGuildClass(pszName) == TRUE)
		{
			if( pclVillage[i].uiGuildID != 0 )
			{
				// 檜 葆擊擎 鼻欽縑憮 餉薯腎橫撿 и棻.
				clIGuild.DelVillage(pclVillage[i].uiGuildID, &pclVillage[i], FALSE);
			}
		}
	}	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褒薯 撮啗曖 衛除擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-08 螃�� 7:10:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::GetRealTime(OnRealWorldTime *pRealWorldTime)
{
	pRealWorldTime->uiMinute	=	pRealTime->tm_min;
	pRealWorldTime->uiHour		=	pRealTime->tm_hour;
	pRealWorldTime->uiDay		=	pRealTime->tm_mday;
	pRealWorldTime->uiMonth		=	pRealTime->tm_mon + 1;	
	pRealWorldTime->uiYear		=	pRealTime->tm_year + 1900;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п渡 Code蒂 陛雖堅 氈朝 葆擊曖 Village Index蒂 憲嬴螞棻.
//	熱薑 橾濠	:	2002-04-15 螃�� 5:01:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜櫛檜 腎朝 嬴檜蠱曖 薑爾蒂 奢雖п 遽棻
//	熱薑 橾濠	:	2002-04-24 螃瞪 9:42:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::SendPayingBusinessAnnoucement()
{	
	UI16			uiBuyVillageCode, uiSellVillageCode;
	MONEY			mnBuyPrice, mnSellPrice, mnMinBuyPrice, mnMaxSellPrice;
	SI32			i;
	CItemHeader*	pItemHeader;
		
	// 高 蟾晦��.
	mnMinBuyPrice		=	0x7fffffffffffffff;
	mnMaxSellPrice		=	0;
	uiBuyVillageCode	=	0;
	uiSellVillageCode	=	0;

	if((pItemHeader = clItem.GetItemHeader(uiLastSendPayingBusinessItem)) != NULL)
	{
		// 檜 嬴檜蠱擊 薯橾 諄啪 だ朝 葆擊婁 薯橾 綠諄啪 だ朝 葆擊擊 瓊朝棻
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

		// 嬴檜蠱擊 餌堅 だ朝 葆擊檜 氈朝雖 匐餌и棻.
		if(uiBuyVillageCode == 0 || uiSellVillageCode == 0)	goto	END;

		// 譆模 檜彌檜 1000 傖 檜鼻 陴嬴撿 и棻.
		// 傳朝 檜彌檜 10% 檜鼻 陴嬴撿 и棻.		
		if(((mnMinBuyPrice + 2000 ) >= mnMaxSellPrice) &&
			((mnMinBuyPrice + (mnMinBuyPrice * 15 / 100)) >= mnMaxSellPrice))	goto	END;	
			
		// 奢雖蒂 爾頂遽棻.
		OfficialAnnouncement(ON_RET_OA_PAYING_BUSINESS,	
						LPARAM(pItemHeader->m_uiID), 
						LPARAM(UI32(mnMinBuyPrice)), LPARAM(uiBuyVillageCode),
						LPARAM(UI32(mnMaxSellPrice)), LPARAM(uiSellVillageCode));
							

		// 奢雖蒂 爾魚 衛除擊 盪濰и棻.
		dwLastSendPayingBusinessTime	=	dwtimeGetTime;
	}


END:
	uiLastSendPayingBusinessItem++;
	uiLastSendPayingBusinessItem	=	uiLastSendPayingBusinessItem % clItem.GetCount();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪 薑爾曖 ④渦蒂 掘и棻.
//	熱薑 橾濠	:	2002-05-02 螃�� 9:25:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 模嶸ж堅 氈朝 衛撲僭擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 3:51:44 - 檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
stFarmData*	SOWorld::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		//return	pclVillage[i].GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );		
	}

	return NULL;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 輿薄縑憮 渠�食磍� 虜萇 �蝦鷏� 隸陛衛麵遽棻.
//	熱薑 橾濠	:	2002-05-23 螃�� 5:58:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOWorld::IncChatRoomInVillCounter()
{
	dwChatRoomInVillCounter++;

	return	dwChatRoomInVillCounter;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 輿薄縑憮 渠�食磍� 虜萇 �蝦鷏� 隸陛衛麵遽棻.
//	熱薑 橾濠	:	2002-05-23 螃�� 5:58:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫曖 ん檣攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-04 螃瞪 10:30:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOPlayer*	SOWorld::GetPlayer(UI16 uiAccount)
{
	// Ы溯檜橫縑啪 嶸�褲� Account 彰嬪蒂 慇橫釭賊 NULL擊 葬欐衛鑑棻.
	if( !ISPLAYER(uiAccount) )					return NULL;

	if(pPlayerOnAccount[uiAccount] != NULL)		return pPlayerOnAccount[uiAccount];
	else										return	NULL;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	晦獄 議葛攪曖 ん檣攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-27 螃�� 4:12:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BaseChar*	SOWorld::GetBaseChar(UI16 uiAccount)
{
	if(ISPLAYER(uiAccount) == TRUE)				return	(BaseChar*)GetPlayer(uiAccount);	
	else if(ISMONSTER(uiAccount) == TRUE)		return	(BaseChar*)clMonster.GetMonster(uiAccount);	
	else if(ISSHIP(uiAccount) == TRUE)			return	(BaseChar*)clSOShip.GetShip(uiAccount);

	return	NULL;
}	


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB Account蒂 檜辨ж罹 Ы溯檜橫曖 ん檣攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-07 螃�� 7:52:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB Account諦 DBSlot擊 檜辨ж罹 Ы溯檜橫曖 ん檣攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 2:59:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOPlayer*	SOWorld::GetPlayerByDBAccount( DWORD dwDBAccount, UI08 uiDBSlot )
{
	SOPlayer	*pPlayer;

	if( ( pPlayer = GetPlayerByDBAccount( dwDBAccount ) ) == NULL )		return	NULL;
	if( pPlayer->stAccountInfo.uiSlot != uiDBSlot )						return	NULL;

	return	pPlayer;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 骯機 欽雖蒂 陛雖堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 1:27:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOWorld::IsIndustrialComplexOwner( SOPlayer *pPlayer )	
{
	SI32	i;

	
	for( i = 0; i < siTotalVillageNum; i++ )
	{
		if( pclVillage[ i ].IsIndustrialComplexOwner( pPlayer ) == TRUE )		return	TRUE;
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫曖 骯機欽雖蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 1:27:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	// null檜賊 寰脾
	if (pOldOwner == NULL || pNewOwner == NULL) return FALSE;
	// 輿檣檜 奢濰擊 檜嘐 陛雖堅氈戲賊 寰脾
	if (IsIndustrialComplexOwner(pNewOwner)) return FALSE;

	// 蕪輿檣檜 奢濰檜橈戲賊 寰脾.
	pComplex = GetIndustrialComplex(pOldOwner);
	if (!pComplex) return FALSE;


	// 輿檣擊 夥脯邀
	pComplex->ChangeOwner(pNewOwner);
	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 奢撩檜 衛濛腎歷棻.
//	熱薑 橾濠	:	2003-01-14 螃�� 3:34:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::EnterAttackVillageTimeZone()
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		pclVillage[ i ].EnterAttackVillageTimeZone();
	}
	m_IsAttackVillageTimeZone	=	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 奢撩檜 部陬棻.
//	熱薑 橾濠	:	2003-01-14 螃�� 3:35:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 葆擊 奢撩醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2003-01-14 螃�� 3:35:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOWorld::IsAttackVillageTimeZone()
{
	return	m_IsAttackVillageTimeZone;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	в萄縑 氈朝 賅萇 嶸螳菟曖 ん虜馬擊 馬模衛鑑棻.
//	熱薑 橾濠	:	2003-04-09 螃�� 2:13 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::ChangeSatietyByWeather()
{
	// 換檜釭 綠陛 螃朝 唳辦縑 ん虜馬擊 馬模衛鑑棻.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		// 湍蟒縑 氈擊 陽紫 в萄縑 氈朝 鼻鷓煎 除輿脹棻.
		// 換檜釭 綠陛 頂葬雖 彊朝 裘紫 氈戲嘎煎 Ы溯檜橫陛 氈朝 裘檜 換檜陛 綠陛 頂萵 熱 氈朝 裘檣雖 羹觼и棻.
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			if( clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) || clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) )
			{
				pPlayerOnAccount[i]->ChangeSatiety( -10 );
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	綠陛 螃朝 雖羲縑 楠渾ж啪 廓偃蒂 儅撩и棻.
//	熱薑 橾濠	:	2003-04-09 螃�� 4:16 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOWorld::MakeThunder()
{
	UI16		uiMapID;
	POINT		ptPos;
	SI32		siAccount[ON_MAX_CONNECTION];		// 廓偃蒂 蜃擊 陛棟撩擊 陛霞 Ы溯檜橫菟曖 橫蘋遴お 葬蝶お
	SI32		numAdded	=	0;					// 葬蝶お縑 蹺陛脹 Ы溯檜橫曖 熱
	SI32		siSelectedAccount;					// 廓偃蒂 蜃啪 腆 Ы溯檜橫

	// 廓偃蒂 蜃擊 陛棟撩擊 陛霞 賅萇 Ы溯檜橫蒂 葬蝶お縑 厥朝棻.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[i]->GetPos( &uiMapID, &ptPos );		// Ы溯檜橫陛 氈朝 裘婁 謝ル蒂 橢橫螞棻.

			if( clISOMap.CanBeRainy(uiMapID) )	// 綠陛 頂萵 熱 氈朝 雖羲檣陛?
			{
				// 0廓裘曖 唳辦 y謝ル 524蒂 晦遽戲煎 陴薹縑 氈朝 雖羲縑虜 綠陛 頂萼棻. (ж萄囀註-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				siAccount[numAdded++]	=	i;
			}
		}
	}

	if( numAdded == 0 )		return;								// 廓偃曖 渠鼻檜 и貲紫 橈戲賊 葬欐
	siSelectedAccount	=	siAccount[rand() % numAdded];		// 葬蝶お縑憮 廓偃 蜃擊 Ы溯檜橫蒂 и貲 堅艇棻.


	// 廓偃 蜃擊 Ы溯檜橫朝 羹溘擊 馬模衛酈堅 檜 餌褒擊 廓偃 蜃擊 陛棟撩檜 氈朝 賅萇 Ы溯檜橫縑啪 憲萼棻.	
	pPlayerOnAccount[siSelectedAccount]->IFollower.AllCharacterDeleteLifePercent( 5 );			// 賅萇 辨煽曖 羹溘擊 5% 馬模衛霽
	for( i = 0; i < numAdded; i++ )
	{
		if( pPlayerOnAccount[siAccount[i]] != NULL && pPlayerOnAccount[siAccount[i]]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[siAccount[i]]->GetPos( &uiMapID, &ptPos );		// Ы溯檜橫陛 氈朝 裘婁 謝ル蒂 橢橫螞棻.

			if( clISOMap.CanBeRainy(uiMapID) )	// 綠陛 頂萵 熱 氈朝 雖羲檣陛?
			{
				// 0廓裘曖 唳辦 y謝ル 524蒂 晦遽戲煎 陴薹縑 氈朝 雖羲縑虜 綠陛 頂萼棻. (ж萄囀註-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				// 賅舒縑啪 詭衛雖 陳葡
				pPlayerOnAccount[siAccount[i]]->SendFieldMsg( ON_MAKE_THUNDER, SO_SFM_ONLYME, (LPVOID)pPlayerOnAccount[siSelectedAccount]->uiAccount );
			}
		}
	}
}