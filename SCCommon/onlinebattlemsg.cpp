//-----------------------------------------------------------------------------------
//    2001.11.10 
//    V0.2
//    김태곤
//-----------------------------------------------------------------------------------

#include <GSL.h>
#include <onlinebattlemsg.h>
#include <OnlineImjin2i.h>
#include <assert.h>

//extern OnlineBattleInfo OBInfo;

static SI32 OnlineCharKindList[]=
{
	60+7,		//KIND_ON_KEON,			
	300+23,		//KIND_ON_MYUNGHWA,		
	9000+27,	//KIND_ON_HANAHINE,		
	8000+771,	//KIND_ON_RYO,			
	10000+307,	//KIND_ON_SORYUNG,		
	10000+563,	//KIND_ON_MIRYUNG,		// 10000+563
	80+5,		//KIND_ON_PORTERK,		// 80+5
	300+41,		//KIND_ON_SWORDK,			// 300+41
	12000+117,	//KIND_ON_SWORDUPK,		// 12000+117
	500+97,		//KIND_ON_ARCHERK,		// 500+97
	11000+349,	//KIND_ON_ARCHERUPK,		// 11000+349
	800+53,		//KIND_ON_BUDAK,			// 800+53
	12000+373,	//KIND_ON_BUDAUPK,		// 12000+373
	1000+109,	//KIND_CANNON2K,			// 1000+109
	1000+365,	//KIND_ON_HORSEARCHERK,	// 1000+365
	11000+605,	//KIND_ON_HORSEARCHERUPK, // 11000+605
	2000+133,	//KIND_ON_JUSTICEK,		// 2000+133
	11000+861,	//KIND_ON_JUSTICEUPK,		// 11000+861
	2000+389,	//KIND_ON_SWORDJ,			// 2000+389
	12000+629,	//KIND_ON_SWORDUPJ,		// 12000+629
	2000+645,	//KIND_ON_GUNJ,			// 2000+645
	12000+885,	//KIND_ON_GUNUPJ,			// 12000+885
	2000+901,	//KIND_ON_HORSESWORDJ,	// 2000+901
	13000+141,	//KIND_ON_HORSESWORDUPJ,	// 13000+141
	3000+157,	//KIND_ON_ADVBUDAJ,		// 3000+157
	13000+397,	//KIND_ON_ADVBUDAUPJ,		// 13000+397
	3000+413,	//KIND_ON_NINJAJ,
	13000+653,	//KIND_ON_NINJAUPJ,
	70+7,		//KIND_ON_HUNGRYDEMON,
	800+45,		//KIND_ON_EATERDEMON,
	300+33,		//KIND_ON_TIGER,
	500+89,		//KIND_ON_WEIRDPLANT,
	1300+57,	//KIND_ON_DOGDEMON,
	1100+1,		//KIND_ON_ONGHWA,
	1600+13,	//KIND_ON_BANDIT1,
	1800+69,	//KIND_ON_BANDIT2,
	2100+25,	//KIND_ON_BANDIT3,
	4600+85,	//KIND_ON_BANDIT4,
	2600+37,	//KIND_ON_GACKDAGUI,
	2300+81,	//KIND_ON_GATPHA,
	2800+93,	//KIND_ON_BOGY,
	3100+49,	//KIND_ON_FOX,
	3400+5,		//KIND_ON_STARFISH,
	15189,		//KIND_ON_SWORDT,
	3600+61,	//KIND_ON_ONI,
	4100+73,	//KIND_ON_SNOWDEMON,
	4400+29,	//KIND_ON_FURHEADDEMON,
	3900+17,	//KIND_ON_FIREDRAGON,
	4900+41,	//KIND_ON_GOLDDRAGON,
	5100+97,	//KIND_ON_BAT,
	5400+53,	//KIND_ON_TOAD,
	5700+9,		//KIND_ON_CENTIPEDE,
	5900+65,	//KIND_ON_DUEOKSINI,
	6400+77,	//KIND_ON_RACCOONDOG,
	6200+21,	//KIND_ON_DEER,
	6700+33,	//KIND_ON_SWORDPIRATE,
	6900+89,	//KIND_ON_GUNPIRATE,
	6200+29,	//KIND_ON_PORTERJ,
	7200+45,	//KIND_ON_SNOWWOMAN,
	7500+1,		//KIND_ON_GANGSI,
	7700+57,	//KIND_ON_HARUBANG,
	8000+13,	//KIND_ON_CRAB,
	8200+69,	//KIND_GHOSTCAT,
	23000+107,	//KIND_MANAGER1,
	8500+25,	//KIND_WHITETIGER,
	9000+37,	//KIND_BUTTERFLY,
	9200+93,	//KIND_WIDOW,
	8700+81,	//KIND_TUTTLE,
	9500+49,	//KIND_INSECT,
	10000+61,	//KIND_WORRIORGHOST,
	10000+317,	//KIND_SINGERMGHOST,
	9800+5,		//KIND_MAMMOTH,
	10000+573,	//KIND_WILDBOAR,
	9800+13,	//KIND_GENERAL1J,
	11000+93,	//KIND_GENERAL4J,
	9500+57,	//KIND_GENERAL6J,
	8200+77,	//KIND_GENERAL5K,
	9000+45,	//KIND_GENERAL4K,
	8500+33,	//KIND_GENERAL8K,
	11000+85,	//KIND_ON_SEAHORSE,
	10000+829,	//KIND_ON_LEECH,
	11000+341,	//KIND_ON_THIEF,
	11000+597,	//KIND_ON_SEAKING,
	80+3,		//KIND_ON_SHIP1,
	300+39,		//KIND_ON_SHIP1,
	500+95,		//KIND_ON_SHIP1,
	800+51,		//KIND_ON_SHIP1,
	1100+7,		//KIND_ON_SHIP1,
	1300+63,	//KIND_ON_SHIP1,
	1600+19,	//KIND_ON_SHIP1,
	1800+75,	//KIND_ON_SHIP1,
	11000+853,  //KIND_ON_BEAR,
	10000+325,	//KIND_GENERAL2J,
	10000+69,	//KIND_GENERAL3J,
	10000+837,	//KIND_GENERAL8J,
	9300+1,		//KIND_GENERAL3K,
	7700+65,	//KIND_GENERAL2K,
	12000+877,	//KIND_ON_HORNET,
	12000+621,	//KIND_ON_CAT,
	12000+365,  //KIND_ON_DUCK,
	13000+909,	//KIND_ON_PORTERT,
	14000+677,	//KIND_ON_BUDAT,
	14000+421,	//KIND_ON_GUNT,
	14000+933,	//KIND_ON_AMAZONT,
	14000+165,	//KIND_ON_AXET,
	13000+133,	//KIND_ON_BADMERCHANT,
	15000+957,	//KIND_ON_SWORDUPT,
	15000+701,	//KIND_ON_BUDAUPT,
	15000+445,	//KIND_ON_GUNUPT,
	16000+469,	//KIND_ON_AXEUPT,
	16000+213,	//KIND_ON_AMAZONUPT,
	13000+645,	//KIND_ON_MBANDIT2,
	13000+901,	//KIND_ON_MBANDIT3,
	14000+925,	//KIND_ON_CHAWOO,
	14000+669,	//KIND_ON_PIRATES,
	13000+389,	//KIND_ON_MBANDIT1,
	15000+437,	//KIND_ON_PANDER,
	15000+949,	//KIND_ON_CROCODILE,
	15000+693,	//KIND_ON_SNAKE,
	16000+205,	//KIND_ON_HOOK,
	12000+109,	//KIND_ON_HORSESPEAR,
	14000+413,	//KIND_ON_BABARIANSHAMAN,
	14000+157,	//KIND_ON_BABARIAN, 
	12000+867,	//KIND_ON_DANG,
	13000+123,	//KIND_ON_SUYOUN,
	16000+461,	//KIND_ON_HORSEMAN,
	17000+229,	//KIND_ON_BADDEER, 
	16000+973,	//KIND_ON_SCORPION, 
	15000+181,  //KIND_ON_CACTUS, 
	16000+717,  //KIND_ON_SMALLGANG,
	18000 +5,	//KIND_ON_GENERAL1T, 
	18000+261,	//KIND_ON_GENERAL4T,
	17000+485,  //KIND_ON_SKYNINE,
	17000+997,  //KIND_ON_ICETREE,
	18000+253,	//KIND_ON_SNOWLIZARD,
	18000+509,	//KIND_ON_MADBULL, 
	18000+765,	//KIND_ON_BIGTREE,
	17000+741,	//KIND_ON_MAGICIAN, 
	18000+517,  //KIND_ON_GENERAL2T,
	18000+773,  //KIND_ON_GENERAL3T, 
	19000+29,	//KIND_ON_GENERAL5T,
	6000+485,	//KIND_ON_PORTER_C,
	17000+237,	//KIND_ON_FANC,
	17000+493,	//KIND_ON_BUDAK_C,
	16000+981,	//KIND_ON_MUDOGA_C,
	16000+725,	//KIND_ON_SWORD_C,
	17000+749,	//KIND_ON_GUN_C,
	19000+797,  //KIND_ON_FANUP_C,
	20000+53,   //KIND_ON_BUDAKUP_C,
	19000+541,  //KIND_ON_MUDOGAUP_C,
	19000+285,	//KIND_ON_SWORDUP_C,
	20000+309,  //KIND_ON_GUNUP_C,
	20000+565,	//KIND_ON_GENERAL4_C		//조승훈
	20000+821,	//KIND_ON_GENERAL2_C		//이령
	21000+77,	//KIND_ON_GENERAL1_C		//동방불패
	21000+333,  //KIND_ON_GENERAL5_C		//축융
	21000+589,  //KIND_ON_GENERAL3_C		//제갈공명
	19000+21,	//KIND_ON_RUNNERHAND,		//괴물덩굴손
	19000+277,  //KIND_ON_SANDGOD,			//모래의 신
	19000+533,	//KIND_ON_SEAENVOY,			//바다의 사신
	19000+789,	//KIND_ON_VIPER,			//살모사.
	20000+45,   //KIND_ON_AMAKUSA,			//아마쿠사.
	20000+301,  //KIND_ON_LAVAMONSTER,		//용암괴물.
	20000+557,  //KIND_ON_EVIL,				//이사의 사념.
	20000+813,	//KIND_ON_BIRD,				//인면조.
	21000+69,	//KIND_ON_WEASEL,			//족제비.
	21000+581,	//KIND_ON_COBRA,			//코브라.
	21000+325,	//KIND_ON_SOUL,				//징기스칸의 혼.
	21000+837,	//KIND_ON_VOLCANOFLAME,		//화산의 불꽃.
	22000+93,   //KIND_ON_REDMADBULL,		//붉은광우.			
	22000+349,	//KIND_ON_MAGICOFFICER,		//요술사령.
	22000+605,  //KIND_ON_CAPBEAR,			//대장반달곰.
	22000+861,  //KIND_ON_KOREANGHOST,		//처녀귀신.
	23000+117,  //KIND_ON_SKELETON,			//해골.
	23000+373,  //KIND_ON_SKELETONSOLDIER,	//해골병사.
	23000+629,  //KIND_ON_PIRATECAPTAIN,	//해적선장.
	23000+885,	//KIND_ON_TAIWANGHOST,		//대만귀신.
	24000+141,	//KIND_ON_JAPANGHOST,		//일본귀신.
	24000+397,	//KIND_ON_JUCK,				//적고적.
	24000+653,	//KIND_ON_FEMALESHAMAN,		//조선무녀.
	24000+909,  //KIND_ON_QUEEN,			//진성여왕.
	25000+165,  //KIND_ON_AUTA,             //아우타.
	21000+845,  //KIND_ON_DONKEY,			//당나귀.
	22000+101,  //KIND_ON_BUTCHER,			//칼잡이.
	22000+357,  //KIND_GENERAL1K,			//권율.
	22000+613,  //KIND_ON_DSWORD,			//기마전사.
	22000+869,  //KIND_GENERAL1J,			//고니시.
	23000+637,	//KIND_ON_BEASTWARRIOR,		//야수전사.
	23000+893,	//KIND_ON_GENERAL6T,		//손유창.
	23000+125,  //KIND_ON_MAGIC_C,			//도술사.
	23000+381,  //KIND_ON_GENERAL6_C,		//순비연.
	25000+421,	//KIND_ON_AGILETIGER,		//비호
	25000+677,	//KIND_ON_PAWNTIGER,		//졸개호랑이.
	27000+213,	//KIND_ON_OWL,				//올빼미
	27000+469,	//KIND_ON_LEOPARD,			//표범
	28000+237,	//KIND_ON_HUNSWARRIOR,		//흉노전사
	27000+981,	//KIND_ON_HUNSHORSE,		//흉노기마병
	27000+725,	//KIND_ON_HUNSHORSEARCHER,	//흉노기마궁수.
	25000+933,	//KIND_ON_GATEKEEPER,		//문지기
	26000+445,	//KIND_ON_CLAYFIGURE,		//기마병마용
	26000+701,	//KIND_ON_PHOENIX,			//불사조
	26000+957,	//KIND_ON_JINEMPEROR,		//진시황의혼
	24000+149,	//KIND_ON_MEDICALMAN,		//의술사.
	24000+405,	//KIND_ON_HEOJUN,			//허준.
	24000+661,	//KIND_ON_COSMIC,			//음양사.
	24000+917,  //KIND_ON_SEIMEI,			//세이메이.
	25000+173,  //KIND_ON_YUMRUCK,			//염력사.
	25000+429,  //KIND_ON_JANGSUNHWA,		//장선화.
	25000+685,  //KIND_ON_LONGSPEAR,		//장창무사.
	25000+941,	//KIND_ON_KOAKHU,			//곽후.	
	28000+493,  //KIND_ON_SNOWMAN1,			//눈사람1
	28000+749,  //KIND_ON_SNOWMAN2,         //눈사람2
	29000+5,	//KIND_ON_SANTA1,		    //가짜산타1
	29000+261,  //KIND_ON_SANTA2,			//가짜산타2
	29000+517,  //KIND_ON_SANTA3,			//가짜산타3
	29000+773,  //KIND_ON_MOTHER_DEER1,		//엄마루돌이1
	30000+29,	//KIND_ON_MOTHER_DEER2,		//엄마루돌이2
	30000+285,	//KIND_ON_MOTHER_DEER3,		//엄마루돌이3
	30000+541,	//KIND_ON_MOTHER_DEER4,		//엄마루돌이4
	30000+797,	//KIND_ON_SANTA4,			//가짜산타4
	31000+53,	//KIND_ON_SANTA_TW,			//대만 가짜 산타.
	31000+309,	//KIND_ON_MOTHER_DEER_TW,		//대만 가짜 루돌이.
	31000+565,  //KIND_ON_FLYDRAGON,			//익룡
	31000+821,	//KIND_ON_RAPTOR,				//랩터
	32000+77,   //KIND_ON_TIRANO,				//티라노
	32000+333,	//KIND_ON_MONKEY,				//원숭이

	// actdoll (2004/02/26 11:12) : 추가된 몬스터 - 유명계 맵용
	32589,		//KIND_ON_HELLKING,								//250		// 염라대왕
	32845,		//KIND_ON_HELLMESSENGER,						//251		// 저승사자
	33101,		//KIND_ON_GHOSTSAMURAI,							//252		// 유령 사무라이
	33357,		//KIND_ON_PYTHON,								//253		// 이무기
	33613,		//KIND_ON_GHOSTELEPHANT,						//254		// 유령 코끼리
	33869,		//KING_ON_GHOSTSEAKING,							//255		// 유령 해저왕
	34125,		//KIND_ON_GHOSTPIRATECAPTAIN,					//256		// 유령 해적선장
	34381,		//KIND_ON_GHOSTSKELETON,						//257		// 유령 해골
	34637,		//KIND_ON_GHOSTSKELETONSOLDIER,					//258		// 유령 해골병사
	34893,		//KIND_ON_GHOSTFEMALESHAMAN,					//259		// 유령 무녀
	35149,		//KIND_ON_GHOSTVOLCANOFLAME,					//260		// 유령 불꽃

	35405,		//KIND_ON_GUARDIAN_MOUSE,						//261		// 수호부(쥐)
	35661,		//KIND_ON_GUARDIAN_CAW1,						//262		// 수호부(소1)
	35917,		//KIND_ON_GUARDIAN_CAW2,						//263		// 수호부(소2)
	36173,		//KIND_ON_GUARDIAN_CAW3,						//264		// 수호부(소3)

	36429,		//KIND_ON_GUARDIAN_CHICK,						//265		// 수호부(병아리)
	36685,		//KIND_ON_GUARDIAN_PIG,							//266		// 수호부(돼지)
	36941,		//KIND_ON_GUARDIAN_SNAKE,						//267		// 수호부(뱀)
	37197,		//KIND_ON_GUARDIAN_MONKEY,						//268		// 수호부(원숭이)

	// robypark 2004/8/30 18:21
	// #define		CONVERT_CHARCODE_TO_DBCHARNO( character, number )					( number << 8 | character )
	28245,		// KIND_ON_MAGIC_C_UP							// 279		// 중국 용병 - 고급 도술사(U110)
	28501,		// KIND_ON_BEASTWARRIOR_UP						// 280		// 대만 용병 - 고급 야수전사(U111)
	28757,		// KIND_ON_YUMRUCK_UP							// 281		// 대만 용병 - 고급 염력사(U112)
	29013,		// KIND_ON_COSMIC_UP							// 282		// 일본 용병 - 고급 음양사(U113)
	29269,		// KIND_ON_MEDICALMAN_UP						// 283		// 조선 용병 - 고급 의술사(U114)
	29525,		// KIND_ON_BUTCHER_UP							// 284		// 조선 용병 - 고급 칼잡이(U115)
	29781,		// KIND_ON_LONGSPEAR_UP							// 285		// 중국 용병 - 고급 장창무사(U116)

	-1
};

// 온라인 캐릭터 코드가 유효한 것인지 확인한다. 
SI32 _OnlineBattleMsgQue::IsValidKind(SI32 onlinekind)
{
	SI32 index = 0;

	 while(1)
	 {

		 if(OnlineCharKindList[index] == -1)
		 {
			 return FALSE;
		 }

		 if(onlinekind == OnlineCharKindList[index])
		 {
			 return TRUE;
		 }

		 index++;
	 }

	 return FALSE;
}

SI32 _OnlineBattleMsgQue::Encode(_OnlineBattleMsg* pobm)
{
	SI32 i;
	char* ptemp;
	SI32 checksum = 0;

	ptemp = (char*)pobm;

	for(i = 0;i < sizeof(_OnlineBattleMsg);i++)
	{
		ptemp[i] = ptemp[i] + 1;
		checksum += ptemp[i]; 
	}

	return checksum;
}

SI32 _OnlineBattleMsgQue::Decode(_OnlineBattleMsg* pobm)
{
	SI32 i;
	char* ptemp;
	SI32 checksum = 0;

	ptemp = (char*)pobm;

	for(i = 0;i < sizeof(_OnlineBattleMsg);i++)
	{
		checksum += ptemp[i]; 
		ptemp[i] = ptemp[i] - 1;
	}

	return checksum;
}


BOOL _OnlineBattleMsgQue::Push(_OnlineBattleMsg* pobm)
{

	if(QueIndex >= MAX_ONLINE_BATTLE_MSG_QUE_SIZE)return FALSE;

	// 암호화 해서 저장한다. 

	
	SI32 checksum = Encode(pobm);

	OBMCheckSum[QueIndex] = checksum;
	OBM[QueIndex] = *pobm;
	QueIndex++;
	return TRUE;
}

BOOL _OnlineBattleMsgQue::Pop(_OnlineBattleMsg* pobm)
{
	
	SI32 i;
	
	if(QueIndex == 0)return FALSE;

	// 암호화를 해제한다. 
	SI32 checksum = Decode(&OBM[0]);

	if(checksum == OBMCheckSum[0])
	{
		*pobm = OBM[0];
	}
	
	for(i = 1;i < QueIndex;i++)
	{
		OBM[i-1]			= OBM[i];
		OBMCheckSum[i-1]	= OBMCheckSum[i];
	}

	QueIndex--;

	return TRUE;
	
}

// 사용자의 유니크 아이디가 유효한지 확인한다.
BOOL _OnlineBattleMsgQue::IsValidID(SI32 playerid)
{
//	SI32 i;
/*
	// 아이디가 음수이면 FALSE
	if(playerid <= 0 )return FALSE;

	for(i = 0;i<MAX_MULTI_PLAYER;i++)
	{
		// 일치하는 아이디가 있으면 TRUE
		if(playerid == OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			return TRUE;
		}
	}
	
	// 일치하는 아이디가 없으면 FALSE
	return FALSE;
*/

	return TRUE;
}

void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(SI32 playerid, SI32 charid)
{

	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_DEAD);
	obmsg.SetPlayerID(playerid);
	
	obmsg.SetCharID(charid);

	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid)
{
	UI08 data = 0;

	*playerid	= pmsg->GetPlayerID();
	data		= pmsg->Data[0];
	*charid		= data;

	return TRUE;
}



void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(SI32 playerid, SI32 charid, SI32 health)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_LIFE);
	obmsg.SetPlayerID(playerid);
	
	obmsg.SetCharID(charid);

	obmsg.SetBigData(health);
	
	Push(&obmsg);
	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *health)
{
	UI08 data = 0;

	*playerid	= pmsg->GetPlayerID();
	data		= pmsg->Data[0];
	*charid		= data;

	*health		= pmsg->GetBigData();

	return TRUE;
}


void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(SI32 playerid, SI32 charid, SI32 mana)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_MANA);
	obmsg.SetPlayerID(playerid);
	
	obmsg.SetCharID(charid);

	obmsg.SetBigData(mana);
	
	Push(&obmsg);
	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *mana)
{
	UI08 data = 0;

	*playerid	= pmsg->GetPlayerID();
	data		= pmsg->Data[0];
	*charid		= data;

	*mana		= pmsg->GetBigData();

	return TRUE;
}

//------------------------

void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(SI32 playerid, SI32 charid, SI32 exp)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;

	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_EXP);
	obmsg.SetPlayerID(playerid);
	
	obmsg.SetCharID(charid);

	obmsg.SetBigData(exp);
	
	Push(&obmsg);
	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *exp)
{
	UI08 data = 0;
	SI32 temp_health = 0;

	*playerid	= pmsg->GetPlayerID();
	data		= pmsg->Data[0];
	*charid		= data;

	*exp		= pmsg->GetBigData();

	return TRUE;
}




// 플레이어가 로그아웃했다는 메시지를 보낸다.
void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LOGOUT(SI32 playerid)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_LOGOUT);
	obmsg.SetPlayerID(playerid);
	
	Push(&obmsg);
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LOGOUT(_OnlineBattleMsg * pmsg, SI32 *playerid)
{
	*playerid		= pmsg->GetPlayerID();

	return TRUE;
}


void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(SI32 killerplayerid, SI32 killercharid, SI32 victimplayerid, SI32 victimcharid)
{
	if(IsValidID(killerplayerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", killerplayerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_KILL);
	obmsg.SetPlayerID(killerplayerid);
	obmsg.SetBigData(killercharid);

	UI16 data = victimplayerid;
	memcpy(&obmsg.Data[0], &data, sizeof(UI16));

	data = victimcharid;
	memcpy(&obmsg.Data[2], &data, sizeof(UI16));


	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(_OnlineBattleMsg * pmsg, SI32 *killerplayerid, SI32 *killercharid, SI32 *victimplayerid, SI32 *victimcharid)
{
	UI16 data = 0;

	*killerplayerid	= pmsg->GetPlayerID();
	*killercharid	= pmsg->GetBigData();
	
	memcpy(&data, &pmsg->Data[0], sizeof(UI16));
	*victimplayerid	= data;

	memcpy(&data, &pmsg->Data[2], sizeof(UI16));
	*victimcharid	= data;

	return TRUE;
}


void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(SI32 playerid)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_HEARTBEAT);
	obmsg.SetPlayerID(playerid);

	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(_OnlineBattleMsg * pmsg, SI32 *playerid)
{
	UI08 data = 0;

	*playerid	= pmsg->GetPlayerID();

	return TRUE;
}



void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(SI32 playerid, SI32 charid, SI32 kind)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_CHANGEKIND);
	obmsg.SetPlayerID(playerid);
	obmsg.SetBigData(kind);
	obmsg.Data[0]	=	charid;

	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32* pcharid, SI32* pkind)
{
	UI08 data = 0;

	*playerid	= pmsg->GetPlayerID();
	*pkind		= pmsg->GetBigData();
	
	data		= pmsg->Data[0];
	*pcharid		= data;

	if(IsValidKind(*pkind) == FALSE)
	{
		return FALSE;
	}
	

	return TRUE;
}

void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(SI32 playerid, SI32 exp)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_EXPALL);
	obmsg.SetPlayerID(playerid);
	obmsg.SetBigData(exp);

	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32* pexp)
{
	*playerid	= pmsg->GetPlayerID();
	*pexp		= pmsg->GetBigData();

	return TRUE;
}

// 해킹되었음을 서버에 통보한다. 
void _OnlineBattleMsgQue::SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(SI32 playerid)
{
	if(IsValidID(playerid) == FALSE)
	{
//		clGrp.Error("Not proper ID", "%d", playerid);
		return ;
	}

	_OnlineBattleMsg obmsg;
	obmsg.Init();
	obmsg.SetType(ONLINE_BATTLE_MSG_HACK);
	obmsg.SetPlayerID(playerid);

	Push(&obmsg);	
}

BOOL _OnlineBattleMsgQue::InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(_OnlineBattleMsg * pmsg, SI32 *playerid)
{
	*playerid	= pmsg->GetPlayerID();

	return TRUE;
}



