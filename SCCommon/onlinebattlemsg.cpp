//-----------------------------------------------------------------------------------
//    2001.11.10 
//    V0.2
//    ���°�
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
	20000+565,	//KIND_ON_GENERAL4_C		//������
	20000+821,	//KIND_ON_GENERAL2_C		//�̷�
	21000+77,	//KIND_ON_GENERAL1_C		//�������
	21000+333,  //KIND_ON_GENERAL5_C		//����
	21000+589,  //KIND_ON_GENERAL3_C		//��������
	19000+21,	//KIND_ON_RUNNERHAND,		//����������
	19000+277,  //KIND_ON_SANDGOD,			//���� ��
	19000+533,	//KIND_ON_SEAENVOY,			//�ٴ��� ���
	19000+789,	//KIND_ON_VIPER,			//����.
	20000+45,   //KIND_ON_AMAKUSA,			//�Ƹ����.
	20000+301,  //KIND_ON_LAVAMONSTER,		//��ϱ���.
	20000+557,  //KIND_ON_EVIL,				//�̻��� ���.
	20000+813,	//KIND_ON_BIRD,				//�θ���.
	21000+69,	//KIND_ON_WEASEL,			//������.
	21000+581,	//KIND_ON_COBRA,			//�ں��.
	21000+325,	//KIND_ON_SOUL,				//¡�⽺ĭ�� ȥ.
	21000+837,	//KIND_ON_VOLCANOFLAME,		//ȭ���� �Ҳ�.
	22000+93,   //KIND_ON_REDMADBULL,		//��������.			
	22000+349,	//KIND_ON_MAGICOFFICER,		//������.
	22000+605,  //KIND_ON_CAPBEAR,			//����ݴް�.
	22000+861,  //KIND_ON_KOREANGHOST,		//ó��ͽ�.
	23000+117,  //KIND_ON_SKELETON,			//�ذ�.
	23000+373,  //KIND_ON_SKELETONSOLDIER,	//�ذ񺴻�.
	23000+629,  //KIND_ON_PIRATECAPTAIN,	//��������.
	23000+885,	//KIND_ON_TAIWANGHOST,		//�븸�ͽ�.
	24000+141,	//KIND_ON_JAPANGHOST,		//�Ϻ��ͽ�.
	24000+397,	//KIND_ON_JUCK,				//������.
	24000+653,	//KIND_ON_FEMALESHAMAN,		//��������.
	24000+909,  //KIND_ON_QUEEN,			//��������.
	25000+165,  //KIND_ON_AUTA,             //�ƿ�Ÿ.
	21000+845,  //KIND_ON_DONKEY,			//�糪��.
	22000+101,  //KIND_ON_BUTCHER,			//Į����.
	22000+357,  //KIND_GENERAL1K,			//����.
	22000+613,  //KIND_ON_DSWORD,			//�⸶����.
	22000+869,  //KIND_GENERAL1J,			//��Ͻ�.
	23000+637,	//KIND_ON_BEASTWARRIOR,		//�߼�����.
	23000+893,	//KIND_ON_GENERAL6T,		//����â.
	23000+125,  //KIND_ON_MAGIC_C,			//������.
	23000+381,  //KIND_ON_GENERAL6_C,		//����.
	25000+421,	//KIND_ON_AGILETIGER,		//��ȣ
	25000+677,	//KIND_ON_PAWNTIGER,		//����ȣ����.
	27000+213,	//KIND_ON_OWL,				//�û���
	27000+469,	//KIND_ON_LEOPARD,			//ǥ��
	28000+237,	//KIND_ON_HUNSWARRIOR,		//�������
	27000+981,	//KIND_ON_HUNSHORSE,		//���⸶��
	27000+725,	//KIND_ON_HUNSHORSEARCHER,	//���⸶�ü�.
	25000+933,	//KIND_ON_GATEKEEPER,		//������
	26000+445,	//KIND_ON_CLAYFIGURE,		//�⸶������
	26000+701,	//KIND_ON_PHOENIX,			//�һ���
	26000+957,	//KIND_ON_JINEMPEROR,		//����Ȳ��ȥ
	24000+149,	//KIND_ON_MEDICALMAN,		//�Ǽ���.
	24000+405,	//KIND_ON_HEOJUN,			//����.
	24000+661,	//KIND_ON_COSMIC,			//�����.
	24000+917,  //KIND_ON_SEIMEI,			//���̸���.
	25000+173,  //KIND_ON_YUMRUCK,			//���»�.
	25000+429,  //KIND_ON_JANGSUNHWA,		//�弱ȭ.
	25000+685,  //KIND_ON_LONGSPEAR,		//��â����.
	25000+941,	//KIND_ON_KOAKHU,			//����.	
	28000+493,  //KIND_ON_SNOWMAN1,			//�����1
	28000+749,  //KIND_ON_SNOWMAN2,         //�����2
	29000+5,	//KIND_ON_SANTA1,		    //��¥��Ÿ1
	29000+261,  //KIND_ON_SANTA2,			//��¥��Ÿ2
	29000+517,  //KIND_ON_SANTA3,			//��¥��Ÿ3
	29000+773,  //KIND_ON_MOTHER_DEER1,		//�����絹��1
	30000+29,	//KIND_ON_MOTHER_DEER2,		//�����絹��2
	30000+285,	//KIND_ON_MOTHER_DEER3,		//�����絹��3
	30000+541,	//KIND_ON_MOTHER_DEER4,		//�����絹��4
	30000+797,	//KIND_ON_SANTA4,			//��¥��Ÿ4
	31000+53,	//KIND_ON_SANTA_TW,			//�븸 ��¥ ��Ÿ.
	31000+309,	//KIND_ON_MOTHER_DEER_TW,		//�븸 ��¥ �絹��.
	31000+565,  //KIND_ON_FLYDRAGON,			//�ͷ�
	31000+821,	//KIND_ON_RAPTOR,				//����
	32000+77,   //KIND_ON_TIRANO,				//Ƽ���
	32000+333,	//KIND_ON_MONKEY,				//������

	// actdoll (2004/02/26 11:12) : �߰��� ���� - ����� �ʿ�
	32589,		//KIND_ON_HELLKING,								//250		// ������
	32845,		//KIND_ON_HELLMESSENGER,						//251		// ���»���
	33101,		//KIND_ON_GHOSTSAMURAI,							//252		// ���� �繫����
	33357,		//KIND_ON_PYTHON,								//253		// �̹���
	33613,		//KIND_ON_GHOSTELEPHANT,						//254		// ���� �ڳ���
	33869,		//KING_ON_GHOSTSEAKING,							//255		// ���� ������
	34125,		//KIND_ON_GHOSTPIRATECAPTAIN,					//256		// ���� ��������
	34381,		//KIND_ON_GHOSTSKELETON,						//257		// ���� �ذ�
	34637,		//KIND_ON_GHOSTSKELETONSOLDIER,					//258		// ���� �ذ񺴻�
	34893,		//KIND_ON_GHOSTFEMALESHAMAN,					//259		// ���� ����
	35149,		//KIND_ON_GHOSTVOLCANOFLAME,					//260		// ���� �Ҳ�

	35405,		//KIND_ON_GUARDIAN_MOUSE,						//261		// ��ȣ��(��)
	35661,		//KIND_ON_GUARDIAN_CAW1,						//262		// ��ȣ��(��1)
	35917,		//KIND_ON_GUARDIAN_CAW2,						//263		// ��ȣ��(��2)
	36173,		//KIND_ON_GUARDIAN_CAW3,						//264		// ��ȣ��(��3)

	36429,		//KIND_ON_GUARDIAN_CHICK,						//265		// ��ȣ��(���Ƹ�)
	36685,		//KIND_ON_GUARDIAN_PIG,							//266		// ��ȣ��(����)
	36941,		//KIND_ON_GUARDIAN_SNAKE,						//267		// ��ȣ��(��)
	37197,		//KIND_ON_GUARDIAN_MONKEY,						//268		// ��ȣ��(������)

	// robypark 2004/8/30 18:21
	// #define		CONVERT_CHARCODE_TO_DBCHARNO( character, number )					( number << 8 | character )
	28245,		// KIND_ON_MAGIC_C_UP							// 279		// �߱� �뺴 - ��� ������(U110)
	28501,		// KIND_ON_BEASTWARRIOR_UP						// 280		// �븸 �뺴 - ��� �߼�����(U111)
	28757,		// KIND_ON_YUMRUCK_UP							// 281		// �븸 �뺴 - ��� ���»�(U112)
	29013,		// KIND_ON_COSMIC_UP							// 282		// �Ϻ� �뺴 - ��� �����(U113)
	29269,		// KIND_ON_MEDICALMAN_UP						// 283		// ���� �뺴 - ��� �Ǽ���(U114)
	29525,		// KIND_ON_BUTCHER_UP							// 284		// ���� �뺴 - ��� Į����(U115)
	29781,		// KIND_ON_LONGSPEAR_UP							// 285		// �߱� �뺴 - ��� ��â����(U116)

	-1
};

// �¶��� ĳ���� �ڵ尡 ��ȿ�� ������ Ȯ���Ѵ�. 
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

	// ��ȣȭ �ؼ� �����Ѵ�. 

	
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

	// ��ȣȭ�� �����Ѵ�. 
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

// ������� ����ũ ���̵� ��ȿ���� Ȯ���Ѵ�.
BOOL _OnlineBattleMsgQue::IsValidID(SI32 playerid)
{
//	SI32 i;
/*
	// ���̵� �����̸� FALSE
	if(playerid <= 0 )return FALSE;

	for(i = 0;i<MAX_MULTI_PLAYER;i++)
	{
		// ��ġ�ϴ� ���̵� ������ TRUE
		if(playerid == OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			return TRUE;
		}
	}
	
	// ��ġ�ϴ� ���̵� ������ FALSE
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




// �÷��̾ �α׾ƿ��ߴٴ� �޽����� ������.
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

// ��ŷ�Ǿ����� ������ �뺸�Ѵ�. 
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



