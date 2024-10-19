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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// SOWorld 생성자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOWorld::SOWorld(SOServer *clTempServer) : 
clSOFindPath(MAX_ONLINEMAP_XSIZE, MAX_ONLINEMAP_YSIZE), Timer(g_GameServerInfoParser.IsTestServer()),
m_bGameGuard(g_GameServerInfoParser.IsGameGuard())
{
	SI32			i;	
//	HQFileManager	hqFile;
	
	srand( (unsigned int)time( NULL ) );

	// 로그 매니저를 초기화 시킨다.
	m_LogMgr.Init();


	// 텍스트를 로드한다.

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

	
	// 매장된 아이템 정보를 파일에서 읽어온다 
	if( !m_buriedItemParser.readFile("Online\\Item\\ItemforField.txt") )
	{
		printf(" ItemForField.txt Error! \n ");
		getch();
	}

	// 서버 클레스를 연결한다.
	clServer		= clTempServer;

	// clWorld와 clServer 클레스가 clDatabase 정보를 공유한다.
	clDatabase		= clServer->clDatabase;
	clLoginDatabase = clServer->clLoginDatabase;
	clItemDatabase	= clServer->clItemDatabase;

	// 셧다운 코드
	siErrCode		= 2;

	siWeatherChangeTime = 5;
	bNowWeather         = FALSE;
	m_bQuestEventUpdate = FALSE;
	m_bWarEnd			= FALSE;
	m_siEventItemCount  = 0;

	

	dwLastThunderCheckTime	=	timeGetTime();

	// 시간 설정 
	//------------------------------------------------------------
	time(&stClock);                         //Day Timer를 설정한다 
	pRealTime = localtime(&stClock);        //base 시간을 설정한다
	//------------------------------------------------------------	

//	try{
//		m_pQuestManager = new CQuestManager;
//	}
//	catch(std::bad_alloc){
//		assert(0);
//	}
//	// 독도 퀘스트를 완료한 사용자 정보를 DB에서 얻어온다.
	m_uiDokdoQuestCompletedUserCount = 0;
	//SI16	siQuestCode = DOKDO_QUEST_CODE2;
	//clDatabase->GetDBInfo(SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST, &m_uiDokdoQuestCompletedUserCount, &siQuestCode);


	// 경험치 정보를 로드한다.
	LoadExpData( g_GameServerInfoParser.IsTestServer() );

	// Text를 초기화한다.
	clOnlineText.Init(NULL, szNationName,NULL);

	// 캐릭터의 World를 설정한다.
	BaseChar::InitStaticVariable(this);		

	// Profit 인터페이스를 초기화 한다.
	printf( "--- Loading Profit Interface... " );	
	if( clIProfit.Init("Online\\Village\\Profit.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// GuildClass의 인터페이스를 초기화 한다.
	printf( "--- Loading GuildClass Interface... " );	
	if( clIGuildClass.Init("Online\\Village\\Grade.txt") == TRUE )
		printf( "[Completed]\n" );
	else
	{
		printf( "[Failed]\n" );
		exit(1);
	}
	
	// Guild의 World를 설정한다.
	// 반드시 clIGuildClass.Init() 밑에 있어야 한다.
	cltIGuild::InitStaticVariable(this);		


	////////////////////////////////////////////////
	// 플레이어를 생성한다.
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
	

	// 배의 정보를 출력한다.
	printf( "--- Analyzing Ships' Info... " );
	clIOnlineShipKI.Init();
	printf( "[Completed]\n" );

	// 배의 이동에 관련된 객체들을 초기화 시킨다.
	printf( "--- Analyzing Ships' course... " );
	clOnlineSMI.Init();
	printf( "[Completed]\n" );
	

	printf( "--- Loading Credit Info... " );
	clCredit.Init("Online\\Add_Credit.txt");
	printf( "[Completed]\n" );

	////////////////////////////////////////////////
	// 포탈 화일을 연다.
	////////////////////////////////////////////////	
	printf( "--- Creating Portals... " );
	if( Portal.Init("Online\\Map\\MapPortal.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	////////////////////////////////////////////////
	// 맵을 생성한다.
	////////////////////////////////////////////////	
	printf( "--- Creating Maps... " );
	ISOMap::InitStaticVariable(this);
	if(clISOMap.Init() == TRUE)	
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );	


	////////////////////////////////////////////////
	// 계정에 관련된 정보를 로드한다.
	////////////////////////////////////////////////	
	
	clOnlineEconomy.Init("Online\\Village\\Economy.txt");
	
	
	////////////////////////////////////////////////
	// 생산 건물에 관련된 정보를 로드한다.
	////////////////////////////////////////////////
	printf( "--- Loading Manufacturing Plant... " );
	if( clBuilding.Init() == FALSE )
		printf( " [Failed]\n" );
	else
		printf( " [Completed]\n" );

	////////////////////////////////////////////////
	// 아이템을 로드한다.
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

	// TradeParser를 읽어온다.
	pTradeParser	=	new	OnlineTradeParser;	

	// IFolower 객체를 초기화 한다.
	// 반드시 clItem과 pTradeParser가 생성된 후에 해야 한다.
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

	//QuestEventItem Parser에 관해 저장.
	m_QuestEventItemMgr = new CQuestEventItemMgr;
	m_QuestEventItemMgr->Init(clQuestParser.m_siEventQuestCount);

	//DB에서 얻어온 아이템 갯수를 저장.
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
	// DB로부터 필드에 떨어져 있는 아이템의 정보를 얻어와서 맵에 설정한다.
	///////////////////////////////////////////////////////////////////////
	clISOMap.SetItemFromDB();
	
	// 현재 시간을 DB로 부터 얻어온다.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GETWORLDTIME, NULL, &CurrentTime);
	
	// DB에서 얻어온 시간을 설정한다.
	Timer.Create(CurrentTime);
	PrevSaveTimeToDBTime = PrevCurrentTime	=	CurrentTime;

	printf( "Current Time : %d/%d/%d %d:00\n", CurrentTime.Month, CurrentTime.Day, CurrentTime.Year, CurrentTime.Hour);
	// 마을을 로드한다.
	// 반드시 InitGuildFromDB() 아래에 있어야 한다.
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

	// 운영자의 정보를 얻어온다.
	clDatabase->GetDBInfo( SO_DB_INFO_KIND_GET_GM_LIST, NULL, &m_clGMManager );

	GetItemDBPrice();
	SetItemDBPrice();

	// 마을을 로드했으므로 각 타일에 가까운 마을을 설정해준다.  
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


	// NPC의 종류 정보를 화일에서 읽어온다.
	printf( "--- Loading NPC KI Interface... " );
	if( clSOCharKI.Init(&clOnlineText, "Online\\CharInfo\\Character List.txt") == TRUE )
		printf( "[Completed]\n" );
	else
		printf( "[Failed]\n" );

	// 몬스터를 초기화 한다.
	printf( "--- Loading Monsters... " );	
	clMonster.Init(this);
	printf( " [Completed]\n" );

	// 필드 몬스터 관리자를 초기화 한다.
	printf( "--- Loading Field Monster Interface... " );	
	clFieldMonster.Init(&clMonster, &clISOMap);
	printf( "[Completed]\n" );

	//이벤트 퀘스트 정보를 얻어온다.

	// Text 화일을 해제한다.
//	hqFile.Free();

	BindJXFile::GetInstance()->destroy();
	

	// 배를 초기화 한다.
	clSOShip.Init();

	// Find Path를 정적 변수를 초기화 한다.
	clSOFindPath.InitStaticVariable(&clISOMap);

	// Command를 초기화한다.
	clCommand.Init(this);	

	dwLastSendPayingBusinessTime	=	0;
	uiLastSendPayingBusinessItem	=	0;

	bUpdateTime		=	FALSE;

	// dwtimeGetTime을 초기화 한다.
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// SOWorld 소멸자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOWorld::~SOWorld()
{
	SI32	i;

	if( m_pAuctionMarket ) delete m_pAuctionMarket;
	if( m_pSmith ) delete m_pSmith;

	//-------------------------------------------------------------------------------
	// 마을을 제거한다.
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
	// 퀘스트 정보를 제거한다.
	//-------------------------------------------------------------------------------
//	if(m_pQuestManager != NULL)
//	{
//		delete m_pQuestManager;
//		m_pQuestManager = NULL;
//	}

	//-------------------------------------------------------------------------------
	// 맵을 제거한다.
	//-------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------
	// 플레이어를 제거한다.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Player Instances... " );
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		delete clPlayer[i];

		clPlayer[i] = NULL;
	}
	printf( "[Completed]\n" );


	//-------------------------------------------------------------------------------
	// 몬스터를 제거한다.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Monsters... " );
	clMonster.Free();
	printf( "[Completed]\n" );

	printf( "--- Deleting Ships... " );
	clSOShip.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// 맵을 제거한다.
	//-------------------------------------------------------------------------------
	printf( "--- Deleting Maps... " );
	clISOMap.Free();
	printf( "[Completed]\n" );
	//-------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------
	// 아이템을 제거한다.
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어와 관련된 모든 연산 처리 (미접속인가?, 접속중인가?, 대기실인가?, 대화방인가?, 게임중인가?)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32 SOWorld::Poll()
{
	SI32	i;
	char	InfoBuf[1024];
	
	dwtimeGetTime	=	timeGetTime();	

	time(&stClock);                         // 실제 현재 시간을 얻어온다.
	pRealTime = localtime(&stClock); 
	GetRealTime(&RealWorldTime);

	CurrentTime		=		Timer.GetNowTime();	
	
	bHourChanged = bDayChanged = bMonthChanged = bYearChanged = FALSE;

	//시간 자정이 넘을 때 나중에 수정해줄것.
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
			//상단 관계 재설정.
			
			clIGuild.SetRelationReLoad();
			m_bQuestEventUpdate = FALSE;	
		}
	}

	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))
	{    //공성하는 날, 시간에 서버를 처음 실행하면
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour)) // 8 ~ 10
		{	//공성포기 정보를 가져온다.		
			if(!m_bGiveUp)
			{
				clIGuild.DBGetGiveUpInfo();
				m_bGiveUp = TRUE;
			}						
		}
		else 
		{   //공성하는 시간이 아닌경우 공성유닛 테이블를 삭제 한다.
			if(!m_bFlag)
			{
				clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
				m_bFlag = TRUE;
			}
		}
	}

	// 시간이 변경되었는지 체크한다.
	if(CurrentTime.Hour != PrevCurrentTime.Hour)
	{
		// 시간이 변경되었다.
		bHourChanged	=	TRUE;
	
		// 날씨 변경전의 1시간동안 눈이나 비가 내렸다면 포만감을 감소시킨다.
		// 비가 그치는 시점에 포만감을 감소시키기 위해 일부러 날씨 처리 이전에 위치시켰다.
		//if( bNowWeather )		ChangeSatietyByWeather();

		siWeatherChangeTime--;
		if(siWeatherChangeTime < 0)
		{
			BOOL		fEvent;

			// Event로 12월 24일과 25은 항상 눈이 오게 함
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
			// 날씨 Change
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

			// 전 User에게 날씨 변경 메세지
			for(int PlayerCount = 1; PlayerCount < ON_MAX_CONNECTION + 1; PlayerCount++)
			{
				if(pPlayerOnAccount[PlayerCount] != NULL)
				{
					pPlayerOnAccount[PlayerCount]->SendFieldMsg(ON_CHANGE_WEATHER, SO_SFM_ONLYME, LPVOID(bNowWeather));
				}
			}
		}	// if(siWeatherChangeTime < 0)

		// 요일이 변경되었는지 체크한다.
		if(CurrentTime.Day != PrevCurrentTime.Day)
		{			
			// 요일이 변경되었다.
			bDayChanged	=	TRUE;
			
			// 하루가 지났을시 포탈에서 해주어야 할 작업등을 해준다.
			Portal.DayChanged();

			sprintf(InfoBuf,"[Date : %4d-%2d-%2d]   Max[%4d] Current[%4d]",CurrentTime.Year,CurrentTime.Month,CurrentTime.Day,
				clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer);
//			clServer->WriteInfo("..\\ConnectStatus.txt",InfoBuf);
			writeInfoToFile("ConnectStatus.txt",InfoBuf);

			// 월이 변경되었는지 체크한다.
			if(CurrentTime.Month != PrevCurrentTime.Month)
			{
				// 월이 변경되었다.
				bMonthChanged	=	TRUE;

				// 년이 변경되었는지 체크한다.
				if(CurrentTime.Year != PrevCurrentTime.Year)
				{
					// 년이 변경되었다.
					bYearChanged	=	TRUE;
					printf("Year Changed!!\n");		

					// 마을 출입자수를 초기화 한다.
					for(i = 0; i < siTotalVillageNum; i++)
					{
						pclVillage[i].PrepareVillageInOutPlayer();
					}
				}

				printf("Month Changed!!\n");		
			}

			printf("Day Changed!!\n");		
		}
	}	// end of if( 시간이 변경되었는지 체크한다 )

	// 번개를 생성할 것인지 1초에 한번씩 체크한다.
	if( dwtimeGetTime - dwLastThunderCheckTime >= 1000 )
	{
		dwLastThunderCheckTime	=	dwtimeGetTime;
		// 비 또는 눈이 내리고 있는 상태여야 한다.
		if( bNowWeather && rand() % 60 == 0 )		MakeThunder();
	}


	///////////////////////////////////////////////////////////////////////////////////////
	// 실제시간 11시에 서버에서 인구를 업데이트 한다. 
	// 실제 인구업데이트는 SoVillage에서 돌린다.SetDBPopulation();
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

				// 새벽 4시에 하는 작업  - 아이템 가격 변동
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
	// 공성 시간을 검사한다. 테스트 용을 위해 공성시간을 PM:8 ~ 10시로 한다.
	///////////////////////////////////////////////////////////////////////////////////////
//	if(pRealTime->tm_wday == 2 || pRealTime->tm_wday == 4) //화, 목요일인지 체크
	if(m_WarTimeParser.IsWarDay(pRealTime->tm_wday))// IsWarCheck(pRealTime->tm_wday, pRealTime->tm_hour))
	{
		if(m_WarTimeParser.IsWarTime(pRealTime->tm_hour))
		{
			if( IsAttackVillageTimeZone() == FALSE )
			{
				EnterAttackVillageTimeZone();

				printf("변신 가능 전송 시작...");
				clIGuild.SendWarTime(); //공성시간이 되었다고 알려준다.				
				printf("변신 가능 전송 끝\n");			
			}
		}
		else
		{
			if( IsAttackVillageTimeZone() == TRUE )
			{
				LeaveAttackVillageTimeZone();
				/////////////////////////////////////////////////////////////////////////////////
				//공성이 끝난다면 공성 캐릭터를 일반 캐릭터로 바꾸고 다른정보들을 셋팅.
				//SOPlayer::Poll에서 캐릭터의 정보를 삭제한다.
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
			//공성이 끝난다면 공성 캐릭터를 일반 캐릭터로 바꾸고 다른정보들을 셋팅.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_WAR_END, NULL, 0);
			m_bWarEnd = TRUE;
			/////////////////////////////////////////////////////////////////////////////////

			//공성포기 관련정보를 초기화 해준다.
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
	// 공성 폭투권한을 날린다.밤 12시에 고정.
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
			//서버의 투자권한을 날려준다.
			clIGuild.SetPowerInvestmentEnd();
			//DB에 투자권한을 날린다고 메세지를 보낸다.
			clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_POWER_INVESTMENT_END, NULL, 0);
			bPowerInvestmentEnd = FALSE;

			///////////////////////////////////////////////////////////////////////////////
			//상단 관계 재설정. 12시가 되면 상단 공성 여부를 재설정한다.
			///////////////////////////////////////////////////////////////////////////////
//			clIGuild.SetRelationReLoad();
			///////////////////////////////////////////////////////////////////////////////
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////


	// 매 시간 마다 DB에 저장한다.
	if( PrevSaveTimeToDBTime.Hour != CurrentTime.Hour )
	{
		DBIKWorldTime			DBWorldTime;
		DBWorldTime.Year		=	CurrentTime.Year;
		DBWorldTime.Month		=	CurrentTime.Month;
		DBWorldTime.Day			=	CurrentTime.Day;
		DBWorldTime.Hour		=	CurrentTime.Hour;
	
		// 시간이 변경되었다. DB에 저장한다.
		clDatabase->writeDBQuery(SO_DB_INFOKIND_SETWORLDTIME, &DBWorldTime, sizeof(DBWorldTime));

		memcpy(&PrevSaveTimeToDBTime, &CurrentTime, sizeof(SOnlineTime));
	}

	// 이익이 남는 아이템의 정보를 공지로 띄어준다
	if(GETTIMEGAP(dwLastSendPayingBusinessTime, dwtimeGetTime) >= 40000)
		SendPayingBusinessAnnoucement();
	
	// 맵의 처리
	clISOMap.Poll();

	// 마을의 처리
	for(i = 0; i < siTotalVillageNum; i++)
	{
		pclVillage[i].Poll();
	}
	
	// 필드에있는 몬스터의 관리 Action 호출.
	clFieldMonster.Action();	

	// 전투 처리 
	m_BattleMgr.ActionPoll();
	
	// 선박 처리
	clSOShip.Poll();	
	
	// 전 프레임의 시간을 저장한다.
	memcpy(&PrevCurrentTime, &CurrentTime, sizeof(SOnlineTime));			

	// 경매소 처리 
	m_pAuctionMarket->Poll();


	if( IsEvent() )
	{		
		SI32					siDBAccounts[ 3000 ];
		SI32					siRankings[ 3000 ];
		DBIKEvent_ExpoRanking	DBExpoRanking;

		// 30분마다 한번씩 DB에 전체 순위를 넘겨준다.		
		if( ( timeGetTime() - m_dwExpoUpdateRankingTime ) > DWORD( 1800000 ) )
		{
			// 랭킹을 얻어온다.
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

		// 10분마다 한번씩 접속해 있는 모든 플레이어의 순위를 보내준다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단과 마을을 초기화 한다.
//					상단과 마을은 서로 의존적이여서 같은 함수에서 초기화를 수행해 주어야 한다
//	수정 일자	:	2002-04-23 오후 3:11:11 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOWorld::InitGuildAndVillage()
{	
	OnlineVillageParser		VillageParser;
	VillageHeader			*pVillageHeader;
	SI32					i;
	BOOL					bLoadVillageSuccess = TRUE;
	
	// 마을의 정적 변수를 초기화 한다.
	SOVillage::InitStaticVariable(this);

	if(VillageParser.ReadText("Online\\Village\\VillageInfo.txt", NULL) == TRUE)
	{		
		siTotalVillageNum	=	VillageParser.GetTotalVillageNum();

		// 상단에 총 마을의 수를 알려준다.
		clIGuild.OnceInit(siTotalVillageNum);

		// 상단의 모든 정보를 DB에서 얻어와서 상단 매니저에 설정한다.
		if (InitGuildFromDB() == FALSE)
		{
			return FALSE;
		}

		pclVillage	=	new	SOVillage[ siTotalVillageNum ];			
		
		// 화일에서 읽은 정보를 이용하여 마을을 초기화 한다.
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 Index Code를 사용하여 마을의 포인터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 이름을 사용하여 마을의 포인터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	특정 위치에서 제일 가까운 의원을 가지 마을을 찾는다.
//	수정 일자	:	2002-04-22 오전 11:04:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 접속된 사용자에게 공지 사항을 알려준다.
//					동일한 메시지를 모든 사용자에게 보내기 위하여
//					각 플레이어의 SendFieldMsg를 사용하지 않는다
//	수정 일자	:	2002-04-23 오후 1:37:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		
		// 공지사항은 최대 255자 밖에 보낼 수 없다
		pTextMsg->dwLength	=	min(strlen(pszText2), 255);

		strncpy(pszText1, pszText2, pTextMsg->dwLength);

		siLength			=	sizeof(OnOATextMsg) + pTextMsg->dwLength;
		break;

	//////////////////////////////////////////////////////////////////////
	// SMS 관련
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 길드를 읽어와서 설정한다.
//	수정 일자	:	2002-02-26 오후 1:20:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL		SOWorld::InitGuildFromDB()
{
	// DB에서 총 길드의 수를 얻어온다.
	clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDLIST, LPVOID(&clIGuild), NULL);

	clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_ITEM, LPVOID(&clIGuild), NULL);


	// DB에서 상단원의 정보를 얻어온다.
	if (clIGuild.SetMemberInfoFromDB() == FALSE) return FALSE;

	// DB에서 길드의 관계를 설정한다.
	clIGuild.SetGuildRelationFromDB();

	// 가장큰 길드 아이디를 얻어온다.
	clIGuild.SetMaxGuildID();

	return TRUE;
}

// DB에서 이벤트에 대한 기본 정보를 얻어온다.
BOOL		SOWorld::GetEventDataFromDDB()
{
	if( clDatabase->GetDBInfo( SO_DB_INFOKIND_EVENT_EXPO, NULL, LPVOID( &m_ExpoRankingSystem ) ) == FALSE )		return	FALSE;
	
	m_ExpoRankingSystem.sortData();

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어의 포인터를 얻어온다.
//	수정 일자	:	2002-03-07 오후 3:53:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 변동 하는 가격을 DB로 부터 얻어온다. (매일 업데이트)
//	수정 일자	:	2002-03-13 오후 4:48:04 - 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				//printf("지울려면 복잡해서 수동으로 지워. 고려사항: mapitem, village_item등등 \n");
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

	// 아이템의 기준 가격을 새로이 얻어왔으므로
	// 모든 마을의 아이템의 가격을 갱신해준다.
	for(i = 0; i < siTotalVillageNum; i++)
	{
		// 아이템의 가격을 다시 결정한다.
		pclVillage[i].m_pMarket->UpdateAllItemsPrice();
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 변동 하는 가격을 DB로 부터 얻어온다.(서버 부팅시 TEXT데이타와 DB데이타가 같은지 확인)
//	수정 일자	:	2002-03-13 오후 4:48:04 - 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::SetItemDBPrice()
{
	CItemHeader*	pItemHeader;
	SI32			siTotalItemNum=0;
	SI32			i;

	
	siTotalItemNum		=	clItem.GetCount();
	// 0으로 초기화가 되었을 경우.
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 최고 상단 직급을 알아온다.
//	수정 일자	:	2002-03-25 오전 10:18:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOWorld::GetHighestGuildClass(CHAR *pszID)
{
	return	GetHighestGuildClassInVillage(pszID);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에서 캐릭터의 최고 상단 직급을 알아온다.
//	수정 일자	:	2002-03-25 오전 10:02:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOWorld::GetHighestGuildClassInVillage(CHAR *pszID)
{
	SI32	siGuildClassCode, siHighestGuildClassCode;
	SI32	i;
		
	// 각 마을에서 해당 플레이어의 최고 직급을 얻어온다.
	siHighestGuildClassCode	=	0;

	for(i = 0; i < siTotalVillageNum; i++)
	{
		siGuildClassCode		=	pclVillage[i].GetHighestGuildClass(pszID);

		// GuildClassCode가 0이 아니여야 한다.
		// GuildClassCode가 0이라면 에러이다.
		if(siGuildClassCode != 0)
		{
			if(siHighestGuildClassCode == 0) 
			{	
				// 처음 한번만 들어온다.
				siHighestGuildClassCode	=	siGuildClassCode;
			}
			else
			{	
				// 두 직급중 높은 직급을 취한다.
				siHighestGuildClassCode	=	clIGuildClass.GetHighClass(siGuildClassCode, siHighestGuildClassCode);							
			}
		}
	}

	return siHighestGuildClassCode;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터가 보유한 마을의 개수를 알아온다.
//	수정 일자	:	2003-03-25 오후 8:43:23 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에서 이 캐릭터가 가지고 있는 직급을 초기화 한다.
//	수정 일자	:	2002-05-09 오후 7:49:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::DeleteGuildClassInVillage(CHAR *pszName)
{
	SI32	i;	
		
	for(i = 0; i < siTotalVillageNum; i++)
	{	
		if(pclVillage[i].DeleteGuildClass(pszName) == TRUE)
		{
			if( pclVillage[i].uiGuildID != 0 )
			{
				// 이 마을은 상단에서 삭제되어야 한다.
				clIGuild.DelVillage(pclVillage[i].uiGuildID, &pclVillage[i], FALSE);
			}
		}
	}	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	실제 세계의 시간을 얻어온다.
//	수정 일자	:	2002-04-08 오후 7:10:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::GetRealTime(OnRealWorldTime *pRealWorldTime)
{
	pRealWorldTime->uiMinute	=	pRealTime->tm_min;
	pRealWorldTime->uiHour		=	pRealTime->tm_hour;
	pRealWorldTime->uiDay		=	pRealTime->tm_mday;
	pRealWorldTime->uiMonth		=	pRealTime->tm_mon + 1;	
	pRealWorldTime->uiYear		=	pRealTime->tm_year + 1900;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해당 Code를 가지고 있는 마을의 Village Index를 알아온다.
//	수정 일자	:	2002-04-15 오후 5:01:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이익이 되는 아이템의 정보를 공지해 준다
//	수정 일자	:	2002-04-24 오전 9:42:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::SendPayingBusinessAnnoucement()
{	
	UI16			uiBuyVillageCode, uiSellVillageCode;
	MONEY			mnBuyPrice, mnSellPrice, mnMinBuyPrice, mnMaxSellPrice;
	SI32			i;
	CItemHeader*	pItemHeader;
		
	// 값 초기화.
	mnMinBuyPrice		=	0x7fffffffffffffff;
	mnMaxSellPrice		=	0;
	uiBuyVillageCode	=	0;
	uiSellVillageCode	=	0;

	if((pItemHeader = clItem.GetItemHeader(uiLastSendPayingBusinessItem)) != NULL)
	{
		// 이 아이템을 제일 싸게 파는 마을과 제일 비싸게 파는 마을을 찾는다
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

		// 아이템을 사고 파는 마을이 있는지 검사한다.
		if(uiBuyVillageCode == 0 || uiSellVillageCode == 0)	goto	END;

		// 최소 이윤이 1000 냥 이상 남아야 한다.
		// 또는 이윤이 10% 이상 남아야 한다.		
		if(((mnMinBuyPrice + 2000 ) >= mnMaxSellPrice) &&
			((mnMinBuyPrice + (mnMinBuyPrice * 15 / 100)) >= mnMaxSellPrice))	goto	END;	
			
		// 공지를 보내준다.
		OfficialAnnouncement(ON_RET_OA_PAYING_BUSINESS,	
						LPARAM(pItemHeader->m_uiID), 
						LPARAM(UI32(mnMinBuyPrice)), LPARAM(uiBuyVillageCode),
						LPARAM(UI32(mnMaxSellPrice)), LPARAM(uiSellVillageCode));
							

		// 공지를 보낸 시간을 저장한다.
		dwLastSendPayingBusinessTime	=	dwtimeGetTime;
	}


END:
	uiLastSendPayingBusinessItem++;
	uiLastSendPayingBusinessItem	=	uiLastSendPayingBusinessItem % clItem.GetCount();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터 정보의 헤더를 구한다.
//	수정 일자	:	2002-05-02 오후 9:25:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 소유하고 있는 시설물을 얻어온다.
//	수정 일자	:	2002-05-09 오후 3:51:44 - 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stFarmData*	SOWorld::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		//return	pclVillage[i].GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );		
	}

	return NULL;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 주점에서 대화방을 만든 횟수를 증가시켜준다.
//	수정 일자	:	2002-05-23 오후 5:58:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOWorld::IncChatRoomInVillCounter()
{
	dwChatRoomInVillCounter++;

	return	dwChatRoomInVillCounter;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 주점에서 대화방을 만든 횟수를 증가시켜준다.
//	수정 일자	:	2002-05-23 오후 5:58:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어의 포인터를 얻어온다.
//	수정 일자	:	2002-07-04 오전 10:30:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOPlayer*	SOWorld::GetPlayer(UI16 uiAccount)
{
	// 플레이어에게 유효한 Account 범위를 벗어나면 NULL을 리턴시킨다.
	if( !ISPLAYER(uiAccount) )					return NULL;

	if(pPlayerOnAccount[uiAccount] != NULL)		return pPlayerOnAccount[uiAccount];
	else										return	NULL;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	기본 캐릭터의 포인터를 얻어온다.
//	수정 일자	:	2002-07-27 오후 4:12:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BaseChar*	SOWorld::GetBaseChar(UI16 uiAccount)
{
	if(ISPLAYER(uiAccount) == TRUE)				return	(BaseChar*)GetPlayer(uiAccount);	
	else if(ISMONSTER(uiAccount) == TRUE)		return	(BaseChar*)clMonster.GetMonster(uiAccount);	
	else if(ISSHIP(uiAccount) == TRUE)			return	(BaseChar*)clSOShip.GetShip(uiAccount);

	return	NULL;
}	


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB Account를 이용하여 플레이어의 포인터를 얻어온다.
//	수정 일자	:	2002-08-07 오후 7:52:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB Account와 DBSlot을 이용하여 플레이어의 포인터를 얻어온다.
//	수정 일자	:	2002-10-15 오후 2:59:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOPlayer*	SOWorld::GetPlayerByDBAccount( DWORD dwDBAccount, UI08 uiDBSlot )
{
	SOPlayer	*pPlayer;

	if( ( pPlayer = GetPlayerByDBAccount( dwDBAccount ) ) == NULL )		return	NULL;
	if( pPlayer->stAccountInfo.uiSlot != uiDBSlot )						return	NULL;

	return	pPlayer;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 산업 단지를 가지고 있는지 알아온다.
//	수정 일자	:	2002-10-15 오후 1:27:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOWorld::IsIndustrialComplexOwner( SOPlayer *pPlayer )	
{
	SI32	i;

	
	for( i = 0; i < siTotalVillageNum; i++ )
	{
		if( pclVillage[ i ].IsIndustrialComplexOwner( pPlayer ) == TRUE )		return	TRUE;
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어의 산업단지를 얻어온다.
//	수정 일자	:	2002-10-15 오후 1:27:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	// null이면 안됨
	if (pOldOwner == NULL || pNewOwner == NULL) return FALSE;
	// 주인이 공장을 이미 가지고있으면 안됨
	if (IsIndustrialComplexOwner(pNewOwner)) return FALSE;

	// 옛주인이 공장이없으면 안됨.
	pComplex = GetIndustrialComplex(pOldOwner);
	if (!pComplex) return FALSE;


	// 주인을 바꿔줌
	pComplex->ChangeOwner(pNewOwner);
	return TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 공성이 시작되었다.
//	수정 일자	:	2003-01-14 오후 3:34:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::EnterAttackVillageTimeZone()
{
	SI32	i;

	for( i = 0; i < siTotalVillageNum; i++ )
	{
		pclVillage[ i ].EnterAttackVillageTimeZone();
	}
	m_IsAttackVillageTimeZone	=	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 공성이 끝났다.
//	수정 일자	:	2003-01-14 오후 3:35:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 마을 공성중인지 알아온다.
//	수정 일자	:	2003-01-14 오후 3:35:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOWorld::IsAttackVillageTimeZone()
{
	return	m_IsAttackVillageTimeZone;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	필드에 있는 모든 유져들의 포만감을 감소시킨다.
//	수정 일자	:	2003-04-09 오후 2:13 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::ChangeSatietyByWeather()
{
	// 눈이나 비가 오는 경우에 포만감을 감소시킨다.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		// 던젼에 있을 때도 필드에 있는 상태로 간주된다.
		// 눈이나 비가 내리지 않는 맵도 있으므로 플레이어가 있는 맵이 눈이가 비가 내릴 수 있는 맵인지 체크한다.
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			if( clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) || clISOMap.CanBeRainy(pPlayerOnAccount[i]->uiMapID) )
			{
				pPlayerOnAccount[i]->ChangeSatiety( -10 );
			}
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	비가 오는 지역에 랜덤하게 번개를 생성한다.
//	수정 일자	:	2003-04-09 오후 4:16 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOWorld::MakeThunder()
{
	UI16		uiMapID;
	POINT		ptPos;
	SI32		siAccount[ON_MAX_CONNECTION];		// 번개를 맞을 가능성을 가진 플레이어들의 어카운트 리스트
	SI32		numAdded	=	0;					// 리스트에 추가된 플레이어의 수
	SI32		siSelectedAccount;					// 번개를 맞게 될 플레이어

	// 번개를 맞을 가능성을 가진 모든 플레이어를 리스트에 넣는다.
	for( int i = 1; i < ON_MAX_CONNECTION + 1; i++ )
	{
		if( pPlayerOnAccount[i] != NULL && pPlayerOnAccount[i]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[i]->GetPos( &uiMapID, &ptPos );		// 플레이어가 있는 맵과 좌표를 얻어온다.

			if( clISOMap.CanBeRainy(uiMapID) )	// 비가 내릴 수 있는 지역인가?
			{
				// 0번맵의 경우 y좌표 524를 기준으로 남쪽에 있는 지역에만 비가 내린다. (하드코딩-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				siAccount[numAdded++]	=	i;
			}
		}
	}

	if( numAdded == 0 )		return;								// 번개의 대상이 한명도 없으면 리턴
	siSelectedAccount	=	siAccount[rand() % numAdded];		// 리스트에서 번개 맞을 플레이어를 한명 고른다.


	// 번개 맞을 플레이어는 체력을 감소시키고 이 사실을 번개 맞을 가능성이 있는 모든 플레이어에게 알린다.	
	pPlayerOnAccount[siSelectedAccount]->IFollower.AllCharacterDeleteLifePercent( 5 );			// 모든 용병의 체력을 5% 감소시킴
	for( i = 0; i < numAdded; i++ )
	{
		if( pPlayerOnAccount[siAccount[i]] != NULL && pPlayerOnAccount[siAccount[i]]->GetStatus() == ON_PLAYER_STATUS_INFIELD )
		{
			pPlayerOnAccount[siAccount[i]]->GetPos( &uiMapID, &ptPos );		// 플레이어가 있는 맵과 좌표를 얻어온다.

			if( clISOMap.CanBeRainy(uiMapID) )	// 비가 내릴 수 있는 지역인가?
			{
				// 0번맵의 경우 y좌표 524를 기준으로 남쪽에 있는 지역에만 비가 내린다. (하드코딩-_-;)
				if( uiMapID == 0 && ptPos.y < 524 )			continue;
				// 모두에게 메시지 날림
				pPlayerOnAccount[siAccount[i]]->SendFieldMsg( ON_MAKE_THUNDER, SO_SFM_ONLYME, (LPVOID)pPlayerOnAccount[siSelectedAccount]->uiAccount );
			}
		}
	}
}