//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 게임 텍스트 메시지 출력부
//	File Name		: Text.cpp
//	Birth Date		: 2003. 06.
//	Creator			: 거 상 팀
//	Editer			: 거 상 팀, 조 현 준 (actdoll@joyon.com), 박 상 원 (robypark@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 전투 게임 내에서 사용되는 텍스트 일람입니다.
//		추론컨대 옛날 임진록 시절에 쓰던 텍스트 모듈인 듯 합니다.
//
//===================================================================================================

#include <GSL.h>

#include <main.h>
#include <etc.h>


#include "text.h"
#include <DebugTool.h>	// hojae_ddd
#include <clGame.h>

#include "bindJXFile.h"

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : 공통 파서기


#define MAX_STAFF_LINE   180

extern	_clGame* pGame;
extern	_Text 				Text;								// 텍스트 
extern	char 				SavePath[];
 
static char NoText[10]="NOTEXT";

#ifdef _IMJIN2PLUS
#ifdef	_ENGLISH
char szStaffString[MAX_STAFF_LINE][32]={
	TITLE_STRING, "Producer",
	NAME_STRING,  "Kim  T.G",
	NAME_STRING,  "", 


	TITLE_STRING, "Scenario",
	NAME_STRING,  "Kim  N.T",
	NAME_STRING,  "",

	TITLE_STRING, "Graphic",
	NAME_STRING,  "Jung J.P", 
	NAME_STRING,  "Jung S.H",
	NAME_STRING,  "Shin K.Y",
	NAME_STRING,  "Lee  J.H",
	NAME_STRING,  "Jung S.K",
	NAME_STRING,  "Jeun H.S",
	NAME_STRING,  "Park J.H",
	NAME_STRING,  "Kim  N.H",
	NAME_STRING,  "",

	TITLE_STRING, "Programming",
	NAME_STRING,  "Kim  T.G",
	NAME_STRING,  "Yang J.M",
	NAME_STRING,  "Lee	J.H",
	NAME_STRING,  "Park Y.S",
	NAME_STRING,  "Han	D.H",
	NAME_STRING,  "Park J.M",
	NAME_STRING,  "",


	TITLE_STRING, "Animation",
	NAME_STRING,  "S.E Multimedia Dept",
	NAME_STRING,  "",

	TITLE_STRING, "Music&Sound",
	NAME_STRING,  "Lee H.J",
    NAME_STRING,  "Lee J.Y",
	NAME_STRING,  "Kang D.I",
	NAME_STRING,  "",

	TITLE_STRING, "Dubbing Artist",
	NAME_STRING,  "Kim S.J",
	NAME_STRING,  "Lee J.Y",
	NAME_STRING,  "Seo K.J",
	NAME_STRING,  "Kim K.J",
	NAME_STRING,  "Mun K.I",
	NAME_STRING,  "",

	TITLE_STRING, "Game Test",
	NAME_STRING,  "Shin H.C",
	NAME_STRING,  "Lee  S.H",
	NAME_STRING,  "Ryu  B.J",
	NAME_STRING,  "Kang J.M",
	NAME_STRING,  "Jung J.H",
	NAME_STRING,  "Han  J.H",
	NAME_STRING,  "O    J.W",
	NAME_STRING,  "Kim  Y.M",
	NAME_STRING,  "Lee  S.M",
	NAME_STRING,  "Yeum K.R",
	NAME_STRING,  "Ryu  M.H",
	NAME_STRING,  "",

	TITLE_STRING, "Special Thanks",	
	NAME_STRING,  "Lee  M.H  Kim J.T",
	NAME_STRING,  "Kang S.Y  Lee J.Y",
	NAME_STRING,  "Kang D.I  Kim Y.J",
	NAME_STRING,  "Park S.J  Kang K.W",
	NAME_STRING,  "Lee  J.Y  Lee H.J",
	NAME_STRING,  "Lee  H.M",
	NAME_STRING,  "",

	TITLE_STRING, "Production",
	NAME_STRING,  "JOYON Entertainment",
	END_STRING 

};
#else
char szStaffString[MAX_STAFF_LINE][32]={
	TITLE_STRING, "기획",
	NAME_STRING,  "최환준 김태곤",
	NAME_STRING,  "정지응",
	NAME_STRING,  "",

	TITLE_STRING, "시나리오",
	NAME_STRING,  "최환준",
	NAME_STRING,  "안형인",
	NAME_STRING,  "",

	TITLE_STRING, "메인 그래픽 디자인",
	NAME_STRING,  "정종필", 
	NAME_STRING,  "정성훈",
	NAME_STRING,  "서상원",
	NAME_STRING,  "최용준",
	NAME_STRING,  "김지현",
	NAME_STRING,  "박기영",
	NAME_STRING,  "윤정민",
	NAME_STRING,  "신동욱",
	NAME_STRING,  "",

	TITLE_STRING, "효과,에니메이션 디자인",
	NAME_STRING,  "김대현",
	NAME_STRING,  "",

	TITLE_STRING, "2D 그래픽 디자인",
	NAME_STRING,  "한상헌",
	NAME_STRING,  "신관용",
	NAME_STRING,  "",

	TITLE_STRING, "타이틀 디자인",
	NAME_STRING,  "김남희",
	NAME_STRING,  "",

	TITLE_STRING, "메인 동영상 디자인",
	NAME_STRING,  "정종필",
	NAME_STRING,  "최혁재",
	NAME_STRING,  "",

	TITLE_STRING, "보조 동영상 디자인",
	NAME_STRING,  "남민수",
	NAME_STRING,  "양경애",
	NAME_STRING,  "",

	TITLE_STRING, "모션 캡쳐 시스템",
	NAME_STRING,  "김대현",
	NAME_STRING,  "",
 
	TITLE_STRING, "프로그래밍",
	NAME_STRING,  "김태곤",
	NAME_STRING,  "양정모",
	NAME_STRING,  "이정환",
	NAME_STRING,  "박연숙",
	NAME_STRING,  "한동희",
	NAME_STRING,  "이문균",
	NAME_STRING,  "박정만",
	NAME_STRING,  "이성재",
	NAME_STRING,  "",

	TITLE_STRING, "음악&효과",
	NAME_STRING,  "리옹뮤직",
	NAME_STRING,  "이형진",
	NAME_STRING,  "",

	TITLE_STRING, "메뉴얼&패키지",
	NAME_STRING,  "RED Communications",
	NAME_STRING,  "",

	TITLE_STRING, "성우",
	NAME_STRING,  "김수중",
	NAME_STRING,  "이재용",
	NAME_STRING,  "서광재",
	NAME_STRING,  "김관진",
	NAME_STRING,  "문관일",
	NAME_STRING,  "김희선",
	NAME_STRING,  "양정애",
	NAME_STRING,  "",

	TITLE_STRING, "녹음",
	NAME_STRING,  "소리네트워크",
	NAME_STRING,  "",

	TITLE_STRING, "도움 주신 분들",	
	NAME_STRING,  "조성삼 조성용",
	NAME_STRING,  "최준영 신승한",
	NAME_STRING,  "조연수 조은희",
	NAME_STRING,  "한석준 이준혁",
	NAME_STRING,  "opsoft 3dartisan",
	NAME_STRING,  "김지영 박미라",
	NAME_STRING,  "선미외식산업 영스넥",
	NAME_STRING,  "유미순 김민석",
	NAME_STRING,  "이정림 여대경",
	NAME_STRING,  "김희정 김정진",
	NAME_STRING,  "박성준 조경희",
	NAME_STRING,  "정민경 ",
	NAME_STRING,  "",

	TITLE_STRING, "제작",
	NAME_STRING,  "JOYON Entertainment",
	END_STRING 
};
#endif

#else
#ifdef	_ENGLISH
char szStaffString[MAX_STAFF_LINE][32]={
	TITLE_STRING, "Producer",
	NAME_STRING,  "Kim  T.G",
	NAME_STRING,  "",


	TITLE_STRING, "Scenario",
	NAME_STRING,  "Kim  N.T",
	NAME_STRING,  "",

	TITLE_STRING, "Graphic",
	NAME_STRING,  "Jung J.P", 
	NAME_STRING,  "Jung S.H",
	NAME_STRING,  "Shin K.Y",
	NAME_STRING,  "Lee  J.H",
	NAME_STRING,  "Jung S.K",
	NAME_STRING,  "Jeun H.S",
	NAME_STRING,  "Park J.H",
	NAME_STRING,  "Kim  N.H",
	NAME_STRING,  "",

	TITLE_STRING, "Programming",
	NAME_STRING,  "Kim  T.G",
	NAME_STRING,  "Yang J.M",
	NAME_STRING,  "Lee	J.H",
	NAME_STRING,  "Park Y.S",
	NAME_STRING,  "Han	D.H",
	NAME_STRING,  "Park J.M",
	NAME_STRING,  "",


	TITLE_STRING, "Animation",
	NAME_STRING,  "S.E Multimedia Dept",
	NAME_STRING,  "",

	TITLE_STRING, "Music&Sound",
	NAME_STRING,  "Lee H.J",
    NAME_STRING,  "Lee J.Y",
	NAME_STRING,  "Kang D.I",
	NAME_STRING,  "",

	TITLE_STRING, "Dubbing Artist",
	NAME_STRING,  "Kim S.J",
	NAME_STRING,  "Lee J.Y",
	NAME_STRING,  "Seo K.J",
	NAME_STRING,  "Kim K.J",
	NAME_STRING,  "Mun K.I",
	NAME_STRING,  "",

	TITLE_STRING, "Game Test",
	NAME_STRING,  "Shin H.C",
	NAME_STRING,  "Lee  S.H",
	NAME_STRING,  "Ryu  B.J",
	NAME_STRING,  "Kang J.M",
	NAME_STRING,  "Jung J.H",
	NAME_STRING,  "Han  J.H",
	NAME_STRING,  "O    J.W",
	NAME_STRING,  "Kim  Y.M",
	NAME_STRING,  "Lee  S.M",
	NAME_STRING,  "Yeum K.R",
	NAME_STRING,  "Ryu  M.H",
	NAME_STRING,  "",

	TITLE_STRING, "Special Thanks",	
	NAME_STRING,  "Lee  M.H  Kim J.T",
	NAME_STRING,  "Kang S.Y  Lee J.Y",
	NAME_STRING,  "Kang D.I  Kim Y.J",
	NAME_STRING,  "Park S.J  Kang K.W",
	NAME_STRING,  "Lee  J.Y  Lee H.J",
	NAME_STRING,  "Lee  H.M",
	NAME_STRING,  "",

	TITLE_STRING, "Production",
	NAME_STRING,  "JOYON Entertainment",
	END_STRING 

};
#else
char szStaffString[MAX_STAFF_LINE][32]={
	TITLE_STRING, "기획",
	NAME_STRING,  "김태곤",
	NAME_STRING,  "",


	TITLE_STRING, "시나리오",
	NAME_STRING,  "김남태 ",
	NAME_STRING,  "",

	TITLE_STRING, "그래픽",
	NAME_STRING,  "정종필", 
	NAME_STRING,  "정성훈",
	NAME_STRING,  "신관용",
	NAME_STRING,  "이재훈",
	NAME_STRING,  "정승교",
	NAME_STRING,  "전현수",
	NAME_STRING,  "박정훈",
	NAME_STRING,  "김남희",
	NAME_STRING,  "",

	TITLE_STRING, "프로그래밍",
	NAME_STRING,  "김태곤",
	NAME_STRING,  "양정모",
	NAME_STRING,  "이정환",
	NAME_STRING,  "박연숙",
	NAME_STRING,  "한동희",
	NAME_STRING,  "박정만",
	NAME_STRING,  "",


	TITLE_STRING, "동영상",
	NAME_STRING,  "삼성전자 멀티미디어 사업부",
	NAME_STRING,  "",

	TITLE_STRING, "음악&효과",
	NAME_STRING,  "이형진",
    NAME_STRING,  "이주연",
	NAME_STRING,  "강대일",
	NAME_STRING,  "",

	TITLE_STRING, "성우",
	NAME_STRING,  "김수중",
	NAME_STRING,  "이재용",
	NAME_STRING,  "서광재",
	NAME_STRING,  "김관진",
	NAME_STRING,  "문관일",
	NAME_STRING,  "",

	TITLE_STRING, "테스트",
	NAME_STRING,  "신현철",
	NAME_STRING,  "이상헌",
	NAME_STRING,  "류봉재",
	NAME_STRING,  "강지만",
	NAME_STRING,  "정재훈",
	NAME_STRING,  "한제희",
	NAME_STRING,  "오종원",
	NAME_STRING,  "김윤명",
	NAME_STRING,  "이상민",
	NAME_STRING,  "염기로",
	NAME_STRING,  "유명한",
	NAME_STRING,  "",

	TITLE_STRING, "도움 주신 분들",	
	NAME_STRING,  "이민호  김정태",
	NAME_STRING,  "강성욱  이주연",
	NAME_STRING,  "강대일  김윤정",
	NAME_STRING,  "박성준  강기원",
	NAME_STRING,  "이주연  이형주",
	NAME_STRING,  "이형모",
	NAME_STRING,  "",

	TITLE_STRING, "제작",
	NAME_STRING,  "JOYON Entertainment",
	END_STRING 

};
#endif
#endif

CHAR* _TextField ::Get()
{
	if(pStringData)
		return pStringData;
	
	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	pStringData	= new char[12];
	sprintf( pStringData, "GA%08d", siIntData );

	return pStringData;	
}

//--------------------------------------------------------------------------
// Description		: 메모리 해제
//--------------------------------------------------------------------------
void _Text::Free()
{
	int			i;
	char*		text;
	_TextField* textfield;
	int			fieldTotalNum;

	text = (char*)&Text;
	fieldTotalNum = sizeof(Text) / sizeof(_TextField);

	for( i=0; i<fieldTotalNum; i++ )
	{
		textfield = (_TextField*)&text[i* sizeof(_TextField)];
		if( textfield->pStringData )
		{
			delete [] textfield->pStringData;
			textfield->pStringData = NULL;
		}
	}
}

//--------------------------------------------------------------------------
// Name: Init()
// Desc: 게임안에서 사용될 글들의 초기화.
//--------------------------------------------------------------------------
void _Text::Init()
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//------------------------------------
	// 게임 시작시 에러 문자열들 
	//------------------------------------

	// actdoll (2004/11/11 0:01) : 대대적으로 이 부분을 손질하도록 한다.
	Ok.SetIntData							(	900			);
	Cancel.SetIntData						(	1000		);
	InsufficientMana.SetIntData				(	1100		);
	Button1.SetIntData						(	1400		);
	Button2.SetIntData						(	1500		);
	ScrollSlow.SetIntData					(	1600		);
	ScrollNormal.SetIntData					(	1700		);
	ScrollFast.SetIntData					(	1800		);
	ButtonDecide.SetIntData					(	1900		);
	ButtonReturn.SetIntData					(	2000		);
	ButtonExit.SetIntData					(	2100		);
	ButtonSearch.SetIntData					(	2200		);
	ButtonMakeRoom.SetIntData				(	2300		);
	ButtonJoin.SetIntData					(	2400		);
	ButtonReview.SetIntData					(	2500		);
	ButtonGame.SetIntData					(	2600		);
	Level.SetIntData						(	2700		);
	MoreNeed.SetIntData						(	2800		);
	Need.SetIntData							(	2900		);
	Scenario.SetIntData						(	3200		);
	OptionVolume.SetIntData					(	4100		);
	OptionMusic.SetIntData					(	4200		);
	OptionBright.SetIntData					(	4400		);
	OptionMouse.SetIntData					(	4500		);
	Load.SetIntData							(	4600		);
	Save.SetIntData							(	4700		);

	//------------------------------------
	// 국가 이름 문자열 
	//------------------------------------
	Korea.SetIntData						(	6300		);
	Japan.SetIntData						(	6400		);
	China.SetIntData						(	6500		);
	Random.SetIntData						(	6600		);
	NoCountry.SetIntData					(	6700		);
	Spectator.SetIntData					(	6800		);


	//------------------------------------
	// 자원용 문자열 
	//------------------------------------
	Rice.SetIntData							(	6900		);
	Potato.SetIntData						(	7000		);
	Tree.SetIntData							(	7100		);

	//------------------------------------
	// 사용자 변경에서 사용될 문자열 
	//------------------------------------
	Player.SetIntData						(	7300		);
	Computer.SetIntData						(	7400		);

	//------------------------------------
	// 게임 상에서 사용될 문자열 
	//------------------------------------
	TempleEvent.SetIntData					(	8100		);
	Sign.SetIntData							(	8200		);
	Ally.SetIntData							(	8300		);
	Enemy.SetIntData						(	8400		);
	AllyPlayer.SetIntData					(	8500		);
	AllPlayer.SetIntData					(	8600		);
	PlayerStart.SetIntData					(	8700		);

	//------------------------------------
	// 장군들의 ID Name
	//------------------------------------
	General1KIDName.SetIntData				(	8900		);
	General2KIDName.SetIntData				(	9000		);
	General3KIDName.SetIntData				(	9100		);
	General4KIDName.SetIntData				(	9200		);
	General5KIDName.SetIntData				(	9300		);
	General6KIDName.SetIntData				(	9400		);
	General7KIDName.SetIntData				(	9500		);
	General8KIDName.SetIntData				(	9600		);
	General1JIDName.SetIntData				(	9700		);
	General2JIDName.SetIntData				(	9800		);
	General3JIDName.SetIntData				(	9900		);
	General4JIDName.SetIntData				(	10000		);
	General5JIDName.SetIntData				(	10100		);
	General6JIDName.SetIntData				(	10200		);
	General7JIDName.SetIntData				(	10250		);
	General8JIDName.SetIntData				(	10270		);
	General1CIDName.SetIntData				(	10300		);
	General2CIDName.SetIntData				(	10400		);
	General3CIDName.SetIntData				(	10500		);
	General4CIDName.SetIntData				(	10600		);
	General5CIDName.SetIntData				(	10700		);
	General6CIDName.SetIntData				(	10750		);
	General7CIDName.SetIntData				(	10770		);
	KingIDName.SetIntData					(	10800		);

	//------------------------------------
	// 게임의 명령 문자열 
	//------------------------------------
	Stop.SetIntData							(	10900		);
	Move.SetIntData							(	11000		);
	Attack.SetIntData						(	11100		);
	Harvest.SetIntData						(	11200		);
	Build.SetIntData						(	11300		);
	Sell.SetIntData							(	11400		);
	Repair.SetIntData						(	11500		);
	Build1.SetIntData						(	11600		);
	Build2.SetIntData						(	11700		);
	TrainSpot.SetIntData					(	11800		);
	RideOut.SetIntData						(	11900		);
	TakeIn.SetIntData						(	12000		);
	CaptureBuild.SetIntData					(	12100		);
	Patrol.SetIntData						(	12200		);
	Convoy.SetIntData						(	12300		);
	Heal.SetIntData							(	12400		);
	MassHeal.SetIntData						(	12500		);
	Hold.SetIntData							(	12600		);
	Betray.SetIntData						(	12700		);
	SelfExp.SetIntData						(	12800		);
	Warp.SetIntData							(	12900		);
	UseBuda.SetIntData						(	13200		);
	AutoMagic.SetIntData					(	13300		);
	NoAutoMagic.SetIntData					(	13400		);
	GeneralFirst.SetIntData					(	13500		);
	NoGeneralFirst.SetIntData				(	13600		);
	FarmerToBull.SetIntData					(	13900		);

	Earthquake.SetIntData					(	14000		);
	Tornado.SetIntData						(	14100		);
	Absorb.SetIntData						(	14200		);
	Lightning.SetIntData					(	14300		);
	Shield.SetIntData						(	14400		);
	Clon.SetIntData							(	14500		);
	StealTech.SetIntData					(	14600		);
	RainMake.SetIntData						(	14700		);
	StealExp.SetIntData						(	14800		);
	GroundExp.SetIntData					(	14900		);

	//------------------------------------
	// 등장 캐릭터 이름 문자열 
	//------------------------------------
	HorseArcherK.SetIntData					(	15500		);
	TShipK.SetIntData						(	16300		);

	PorterJ.SetIntData						(	17500		);
	HorseSwordJ.SetIntData					(	17900		);
	AdvBudaJ.SetIntData						(	18200		);
	NinjaJ.SetIntData						(	18300		);

	// 장군들 
    General2K.SetIntData					(	19100		);
    General3K.SetIntData					(	19200		);
	General4K.SetIntData					(	19300		);
	General5K.SetIntData					(	19400		);
	General2J.SetIntData					(	20300		);
	General3J.SetIntData					(	20400		);
	General4J.SetIntData					(	20500		);
	General5J.SetIntData					(	20600		);


	// 건물들 
	// 물품 이름 
	Mine.SetIntData							(	24600		);
	RainMaker.SetIntData					(	24700		);
	SamArrow.SetIntData						(	24800		);
	Elephant.SetIntData						(	24900		);
	AirMagic.SetIntData						(	25000		);
	Bull.SetIntData							(	25100		);
	DragonBall.SetIntData					(	25200		);
	GinSeng.SetIntData						(	25300		);

	NeedBuildingMessage.SetIntData			(	32100		);
	MineStockMessage.SetIntData				(	32200		);
	AirMagicStockMessage.SetIntData			(	32500		);
	BullStockMessage.SetIntData				(	32600		);

	// 게임에서 사용되는 메시지 
	CannotMakeThereMessage.SetIntData		(	32800		);
	QuitGameMessage.SetIntData				(	33500		);
	MerchantComeMessage.SetIntData			(	33700		);
	MerchantLeaveMessage.SetIntData			(	33800		);
    MineExplodeMessage.SetIntData			(	34900		);
	ReturnMenuMessage.SetIntData			(	35000		);
	RestartMessage.SetIntData				(	35100		);
	QuitMessage.SetIntData					(	35200		);
	NotProperSoundCardMessage.SetIntData	(	35700		);
	CheatMessage.SetIntData					(	35900		);

	// 상세 도움말 
	HelpSellMessage.SetIntData				(	36100		);
	HelpAirMagicMessage.SetIntData			(	36200		);
	HelpFarmerToBullMessage.SetIntData		(	36300		);
	HelpMineMessage.SetIntData				(	36400		);
	HelpNoGeneralFirstMessage.SetIntData	(	36500		);
	HelpGeneralFirstMessage.SetIntData		(	36600		);
	HelpNoAutoMagicMessage.SetIntData		(	36700		);
	HelpAutoMagicMessage.SetIntData			(	36800		);
	HelpUseBudaMessage.SetIntData			(	36900		);
	HelpBetrayMessage.SetIntData			(	37300		);
	HelpTornadoMessage.SetIntData			(	37800		);

	CheatedGame.SetIntData					(	42900		);	


	//------------------------------------------------------------------------
	// 추가 텍스트
	//------------------------------------------------------------------------
	General7K.SetIntData					(	55600		);
	General8K.SetIntData					(	55800		);
	General6J.SetIntData					(	56000		);	// 도쿠가와 이에야스
	General7J.SetIntData					(	56200		);
	General8J.SetIntData					(	56400		);

	Cannon2K.SetIntData						(	57000		);
	MerchantKoreaIDName.SetIntData			(	57900		);
	MerchantJapanIDName.SetIntData			(	58000		);
	MerchantChinaIDName.SetIntData			(	58100		);

	Bow1.SetIntData							(	59100		);
	Bow2.SetIntData							(	59200		);
	Rifle1.SetIntData						(	59300		);
	Gun1.SetIntData							(	59400		);
	Cannon1.SetIntData						(	59500		);
	Sword1.SetIntData						(	59600		);
	Sword2.SetIntData						(	59700		);
	Sword3.SetIntData						(	59800		);
	Sword4.SetIntData						(	59900		);
	Sword5.SetIntData						(	60000		);
	Sword6.SetIntData						(	60100		);
	Sword7.SetIntData						(	60110		);
	Spear1.SetIntData						(	60200		);
	Spear2.SetIntData						(	60300		);
	Axe1.SetIntData							(	60400		);
	Book1.SetIntData						(	60500		);
	Book2.SetIntData						(	60600		);
	Book3.SetIntData						(	60700		);
	Necklace1.SetIntData					(	60800		);
	Armour1.SetIntData						(	60900		);
	Belt1.SetIntData						(	61000		);
	Belt2.SetIntData						(	61100		);
	Med1.SetIntData							(	61200		);
	Med2.SetIntData							(	61300		);
	Med3.SetIntData							(	61400		);
	Med4.SetIntData							(	61500		);


	KimBomb.SetIntData						(	66100		);
	Bamboo.SetIntData						(	66300		);

	Crop.SetIntData							(	67400		);
	Scores.SetIntData						(	67700		);
	Prologue.SetIntData						(	68400		);
	CustomGame.SetIntData					(	68500		);
	ChangeUser.SetIntData					(	68600		);	
	WinGame.SetIntData						(	68700		);
	LoseGame.SetIntData						(	68800		);
	Continue.SetIntData						(	68900		);
	Objectives.SetIntData					(	69000		);
	Optoin.SetIntData						(	69100		);
	InitMenu.SetIntData						(	69200		);
	Replay.SetIntData						(	69300		);
	On.SetIntData							(	69600		);
	Off.SetIntData							(	69700		);
	Number_1.SetIntData						(	70200		);
	Number_2.SetIntData						(	70300		);	
	Number_3.SetIntData						(	70400		);
	Number_4.SetIntData						(	70500		);
	Number_5.SetIntData						(	70600		);
	Number_6.SetIntData						(	70700		);
	Number_7.SetIntData						(	70800		);
	Number_8.SetIntData						(	70900		);

	JusticeK.SetIntData						(	72500		);
	Menu.SetIntData							(	72600		);

	Magic8j.SetIntData						(	72800		);

	Armour2.SetIntData						(	73100		);
	Armour3.SetIntData						(	73300		);

	Magic6c.SetIntData						(	73500		);

	Magic4c.SetIntData						(	74000		);

	Extinguish.SetIntData					(	74600		);

	ShowStorage.SetIntData					(	77500		);

	Spear3.SetIntData						(	77600		);
	Stick1.SetIntData						(	77700		);
	Stick2.SetIntData						(	77800		);
	Mace1.SetIntData						(	77900		);
	Claw1.SetIntData						(	78000		);
	Armour5.SetIntData						(	78100		);
	Med5.SetIntData							(	78200		);
	GoodsSpear3.SetIntData					(	78500		);
	GoodsStick1.SetIntData					(	78600		);
	GoodsStick2.SetIntData					(	78700		);

	Armour4.SetIntData						(	79600		);

	UsePotion.SetIntData					(	83000		);
	HelpWarpMessage2.SetIntData				(	83100		);

	Ambush.SetIntData						(	83500		);
	AttackGround.SetIntData					(	83800		);

	ErrorMagic8jNoApower.SetIntData			(	84100		);
	ErrorBetrayHealth.SetIntData			(	84200		);
	Magic5j.SetIntData						(	84400		);
	ImjinOnline.SetIntData					(	85400		);
	DetectMine.SetIntData					(	85700		);

	Keon.SetIntData							(	90300		);
	EaterDemon.SetIntData					(	90400		);
	WeirdPlant.SetIntData					(	90500		);
	HungryDemon.SetIntData					(	90600		);
	Tiger.SetIntData						(	90700		);
	DogDemon.SetIntData						(	90800		);
	Onghwa.SetIntData						(	90900		);
	Bandit1.SetIntData						(	91000		);
	Bandit2.SetIntData						(	91100		);
	Bandit3.SetIntData						(	91200		);
	PorterK.SetIntData						(	91300		);
	Gackdagui.SetIntData					(	91400		);
	Gatpha.SetIntData						(	91500		);
	Life.SetIntData							(	91600		);
	Mana.SetIntData							(	91700		);
	Bogy.SetIntData							(	91800		);
	Fox.SetIntData							(	91900		);
	Starfish.SetIntData						(	92000		);
	Myunghwa.SetIntData						(	92100		);
	Ryo.SetIntData							(	92200		);
	Hanahine.SetIntData						(	92300		);
	Soryung.SetIntData						(	92400		);
	Miryung.SetIntData						(	92500		);
	Apower.SetIntData						(	93300		);
	Dpower.SetIntData						(	93400		);
	Bandit4.SetIntData						(	93500		);
	BrightTile.SetIntData					(	93600		);
	Oni.SetIntData							(	93700		);
	MoveSpeed.SetIntData					(	93800		);
	SnowDemon.SetIntData					(	93900		);
	FurHeadDemon.SetIntData					(	94000		);
	FireDragon.SetIntData					(	94100		);
	GoldDragon.SetIntData					(	94200		);
	Bat.SetIntData							(	94300		);
	Toad.SetIntData							(	94400		);
	Centipede.SetIntData					(	94500		);
	Dueoksini.SetIntData					(	94600		);
	Gangsi.SetIntData						(	94700		);
	RaccoonDog.SetIntData					(	94800		);
	Deer.SetIntData							(	94900		);
	OnSwordK.SetIntData						(	95000		);
	OnArcherK.SetIntData					(	95100		);
	OnSwordJ.SetIntData						(	95200		);
	OnGunJ.SetIntData						(	95300		);
	SwordPirate.SetIntData					(	95400		);
	GunPirate.SetIntData					(	95500		);
	SnowWoman.SetIntData					(	95600		);
	OnBudaK.SetIntData						(	95700		);
	HaruBang.SetIntData						(	95800		);
	Crab.SetIntData							(	95900		);
	GhostCat.SetIntData						(	96000		);
	Manager1.SetIntData						(	96100		);
	WhiteTiger.SetIntData					(	96200		);
	Tuttle.SetIntData						(	96300		);
	Butterfly.SetIntData					(	96400		);
	Widow.SetIntData						(	96500		);
	Insect.SetIntData						(	96600		);
	WorriorGhost.SetIntData					(	96700		);
	SingermGhost.SetIntData					(	96800		);
	Mammoth.SetIntData						(	96900		);
	Wildboar.SetIntData						(	97000		);
	Leech.SetIntData						(	97100		);
	SeaHorse.SetIntData						(	97200		);
	Thief.SetIntData						(	97300		);
	SeaKing.SetIntData						(	97400		);
	Bear.SetIntData							(	97500		);
	HorseSpear.SetIntData					(	97600		);
	Cat.SetIntData							(	97700		);
	Duck.SetIntData							(	97800		);
	Hornet.SetIntData						(	97900		);
	Work.SetIntData							(	98000		);
	Difficulty.SetIntData					(	98100		);
	GotFruit.SetIntData						(	98200		);
	RemainFruit.SetIntData					(	98300		);
	PorterT.SetIntData						(	98400		);
	BudaT.SetIntData						(	98500		);
	GunT.SetIntData							(	98600		);
	SwordT.SetIntData						(	98700		);
	AmazonT.SetIntData						(	98800		);
	AxeT.SetIntData							(	98900		);
	GetItemMessage.SetIntData				(	99000		);
	BadMerchant.SetIntData					(	100000		);
	MBandit1.SetIntData						(	100100		);
	MBandit2.SetIntData						(	100200		);
	MBandit3.SetIntData						(	100300		);
	Chawoo.SetIntData						(	100400		);
	Pirates.SetIntData						(	100500		);
	LostExpMessage.SetIntData				(	100600		);
	LostMoneyMessage.SetIntData				(	100700		);
	Pander.SetIntData						(	100900		);
	Snake.SetIntData						(	101000		);
	Crocodile.SetIntData					(	101100		);
	Hook.SetIntData							(	101200		);
	RetreatWarningInBattleArea.SetIntData	(	101300		);
	RetreatWarningInPeaceArea.SetIntData	(	101400		);
	SmallGang.SetIntData					(	101500		);
	HorseMan.SetIntData						(	101600		);
	BabarianShaman.SetIntData				(	101700		);
	Babarian.SetIntData						(	101800		);
	Dang.SetIntData							(	101900		);
	Suyoun.SetIntData						(	102000		);
	BadDeer.SetIntData						(	102100		);
	Cactus.SetIntData						(	102200		);
	Scorpion.SetIntData						(	102300		);
	General1T.SetIntData					(	102400		);
	General4T.SetIntData					(	102500		);
	SkyNine.SetIntData						(	102600		);
	IceTree.SetIntData						(	102700		);
	SnowLizard.SetIntData					(	102800		);
	MadBull.SetIntData						(	102900		);
	BigTree.SetIntData						(	103000		);
	Magician.SetIntData						(	103100		);
	General2T.SetIntData					(	103200		);
	General5T.SetIntData					(	103300		);
	General3T.SetIntData					(	103400		);
	Magic4T.SetIntData						(	103500		);
	Magic2T.SetIntData						(	103600		);
	MagicTest.SetIntData					(	104000		);
	BigSwordC.SetIntData					(	104001		);
	PorterC.SetIntData						(	104002		);
	FanC.SetIntData							(	104003		);
	BudakC.SetIntData						(	104004		);
	MudogaC.SetIntData						(	104005		);
	FireGunC.SetIntData						(	104006		);
	FanUpC.SetIntData						(	104007		);
	BudakUpC.SetIntData						(	104008		);
	MudogaUpC.SetIntData					(	104009		);
	BigSwordUpC.SetIntData					(	104010		);
	FireGunUpC.SetIntData					(	104011		);
	MagicRecallPanda.SetIntData				(	104012		);
	Monkey.SetIntData						(	104013		);
	General2_c.SetIntData					(	104014		);
	General3_c.SetIntData					(	104015		);
	General4_c.SetIntData					(	104016		);
	General5_c.SetIntData					(	104017		);
	General1_c.SetIntData					(	104018		);
	RunnerHand.SetIntData					(	104019		);
	SandGod.SetIntData						(	104020		);
	SeaEnvoy.SetIntData						(	104021		);
	Viper.SetIntData						(	104022		);
	Amakusa.SetIntData						(	104023		);
	LavaMonster.SetIntData					(	104024		);
	Evil.SetIntData							(	104025		);
	Bird.SetIntData							(	104026		);
	Weasel.SetIntData						(	104027		);	
	Soul.SetIntData							(	104028		);	
	Cobra.SetIntData						(	104029		);
	VolcanoFlame.SetIntData					(	104030		);
	ExpLost.SetIntData						(	104031		);
	MagicOfficer.SetIntData					(	104033		);
	CapBear.SetIntData						(	104034		);
	KoreanGhost.SetIntData					(	104040		);
	WangSoul.SetIntData						(	104041		);
	DarkFire.SetIntData						(	104043		);
	Skeleton.SetIntData						(	104044		);
	SkeletonSoldier.SetIntData				(	104045		);
	PirateCaptain.SetIntData				(	104046		);
	TaiwanGhost.SetIntData					(	104047		);
	JapanGhost.SetIntData					(	104048		);
	Auta.SetIntData							(	104049		);
	Juck.SetIntData							(	104050		);
	FemaleShaman.SetIntData					(	104051		);
	Queen.SetIntData						(	104052		);
	Ivory.SetIntData						(	104053		);
	BlueDragon.SetIntData					(	104054		);
	RunningFire.SetIntData					(	104055		);
	EnergyPa.SetIntData						(	104056		);
	SnowAttack.SetIntData					(	104057		);
	FireAttack.SetIntData					(	104058		);
	Donkey.SetIntData						(	104059		);
	FireEnergyPa.SetIntData					(	104060		);	
	Butcher.SetIntData						(	104061		);
	Kwunyul.SetIntData						(	104062		);
	Dsword.SetIntData						(	104063		);
	Gonisi.SetIntData						(	104064		);
	BeastWorrior.SetIntData					(	104065		);
	General6T.SetIntData					(	104066		);
	MagicC.SetIntData						(	104067		);
	Sunbiyoun.SetIntData					(	104068		);
	AgileTiger.SetIntData					(	104069		);
	PawnTiger.SetIntData					(	104070		);
	ManaHeal.SetIntData						(	104071		);
	DoubleSword.SetIntData					(	104072		);
	LongSpear.SetIntData					(	104073		);
	IceBarrier.SetIntData					(	104074		);
	Owl.SetIntData							(	104075		);
	LeoPard.SetIntData						(	104076		);
	HunsWarrior.SetIntData					(	104077		);
	HunsHorse.SetIntData					(	104078		);
	HunsHorseArcher.SetIntData				(	104079		);
	MedicalMan.SetIntData					(	104080		);	//의술사
	HeoJun.SetIntData						(	104081		);	//허준
	Cosmic.SetIntData						(	104082		);	//음양사
	Seimei.SetIntData						(	104083		);	//세이메이
	Yumruck.SetIntData						(	104084		);	//염력사
	JangSunHwa.SetIntData					(	104085		);	//장선화
	LongSpearM.SetIntData					(	104086		);	//장창무사
	KoakHu.SetIntData						(	104087		);	//곽후
	GateKeeper.SetIntData					(	104088		);	//문지기
	Clayfigure.SetIntData					(	104089		);	//기마병마용
	Phoenix.SetIntData						(	104090		);	//피닉스
	Jinemperor.SetIntData					(	104091		);	//진시황
	FlowerNeedleRain.SetIntData				(	104092		);	//만천화우
	SoulChange.SetIntData					(	104093		);	//빙의술
	StoneMagic.SetIntData					(	104094		);	//석괴술
	Snowman.SetIntData						(	104095		);	//눈사람
	Santa.SetIntData						(	104096		);	//석괴술
	MotherDeer.SetIntData					(	104097		);	//엄마루돌이
	Raptor.SetIntData						(	104098		);	//랩터
	Tirano.SetIntData						(	104099		);	//랩터
	FlyDragon.SetIntData					(	104100		);	//익룡.
	AllySelect.SetIntData					(	104101		);

//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 START
//----------------------------------------------------------------------------
	HellKing.SetIntData						(	104102		);	// 염라대왕
	HellMessenger.SetIntData				(	104103		);	// 저승사자
	GhostSamurai.SetIntData					(	104104		);	// 유령 사무라이
	Python.SetIntData						(	104105		);	// 이무기
	GhostElephant.SetIntData				(	104106		);	// 유령 코끼리
	GhostSeaking.SetIntData					(	104107		);	// 유령 해저왕
	GhostPirateCaptain.SetIntData			(	104108		);	// 유령 해적선장
	GhostSkeleton.SetIntData				(	104109		);	// 유령 해골
	GhostSkeletonSoldier.SetIntData			(	104110		);	// 유령 해골병사
	GhostFemaleShaman.SetIntData			(	104111		);	// 유령 무녀
	GhostVolcanoFlame.SetIntData			(	104112		);	// 유령 불꽃
//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 END
//----------------------------------------------------------------------------

	GuardianMouse.SetIntData				(	104113		);	// 수호부(쥐)
	GuardianCaw.SetIntData					(	104114		);	// 수호부(소)
	GuardianChick.SetIntData				(	104115		);	// 수호부(병아리)
	GuardianPig.SetIntData					(	104116		);	// 수호부(돼지)
	GuardianSnake.SetIntData				(	104117		);	// 수호부(뱀)
	GuardianMonkey.SetIntData				(	104118		);	// 수호부(원숭이)

//----------------------------------------------------------------------------
// robypark (2004/05/7 18:42) : 추가 캐릭터 - 침략왜구 (독도)
//----------------------------------------------------------------------------
	JapInvaders.SetIntData					(	104119		);	// 침략왜구(M146, (game text)104119, (online text)8000689)

//----------------------------------------------------------------------------
// robypark (2004/05/11 13:15) : 추가 캐릭터 - 장수 2차 전직
//----------------------------------------------------------------------------
	General_TURTLE_VEHICLE.SetIntData		(	104120		);	// 270	U102	조선-거북차
	General_THUNDER_VEHICLE.SetIntData		(	104121		);	// 271	U103	조선-뇌전차
	General_QUAKE_VEHICLE.SetIntData		(	104122		);	// 272	U104	일본-지진차
	General_BLACK_DRAGON_VEHICLE.SetIntData	(	104123		);	// 273	U105	일본-흑룡차
	General_FARANGI_VEHICLE.SetIntData		(	104124		);	// 275	U106	중국-불랑기포차
	General_CATAPULT.SetIntData				(	104125		);	// 274	U107	중국-발석거
	General_FIRE_DRAGON_VEHICLE.SetIntData	(	104126		);	// 276	U108	대만-화룡차
	General_CHINESEPHOENIX.SetIntData		(	104127		);	// 277	U109	대만-봉황비조

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:10) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	Skill_FIRE_ROUND_DANCE.SetIntData		(	104128		);	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	Skill_NOVA.SetIntData					(	104129		);	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	Skill_FURY_OF_VOLCANO.SetIntData		(	104130		);	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	Skill_POISONING_HEAVENS.SetIntData		(	104131		);	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	Skill_BOMB_OF_HEAVEN.SetIntData			(	104132		);	// 승천포(중국-발석거, 순비연 2차 전직 기술)
	Skill_SIEGE_MODE.SetIntData				(	104133		);	// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
	Skill_ONSLAUGHT_FIREBALL.SetIntData		(	104134		);	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	Skill_FIRERAIN_OF_GOD.SetIntData		(	104135		);	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

	// robypark 2004/6/4 14:1
	Skill_SIEGE_MODE_UNDO.SetIntData		(	104136		);	// 앙천대소에서 일반 불랑기포차로 변신(중국-불랑기포차, 이령 2차 전직 기술)
	
	// actdoll (2004/09/17 11:02) : 몇가지 하드코딩된 텍스트들
	Msg_About_Destroy_Alliance.SetIntData	(	104137		);	// %s 님이 %s 님과의 동맹을 해제했습니다.
	Msg_About_Request_Alliance.SetIntData	(	104138		);	// %s 님이 %s 님에게 동맹을 요청했습니다.
	Msg_About_Winner_Of_Battle.SetIntData	(	104139		);	// %s 님이 전투에서 승리했습니다.
	Msg_About_Loser_Of_Battle.SetIntData	(	104140		);	// %s 님이 전투에서 패배했습니다.

	// robypark 2004/9/17 18:32
	// 추가된 몬스터 이름
	Monster_RedEye_SeabedMan.SetIntData		(	104141		);		// 붉은 눈 해저인
	Monster_HunsWarrior_Fake.SetIntData		(	104142		);		// 흉노전사

	// robypark 2004/11/4 17:23
	// 11월 문화 유적 탐사 이벤트 몬스터 이름
	Monster_Jeopalge.SetIntData				(	104143		);		// 저팔계

	// actdoll (2004/02/23 15:55) : 
	//	다른 방식으로 해당 정보를 로딩한다.
	char buffer[128];
	strcpy( buffer, pGame->m_pszHQNation );		// 앞에 해당 국가의 폴더명 붙이고
	strcat( buffer, "\\GameText.dat" );			// 해당 파일명을 붙인다.

	LoadData( (char*)pGame->m_pszHQNation, ( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) );
}

void _Text::SetStringData(int number, char* string, int strLength)
{
	int i;
	char* text;
	_TextField* textfield;
	int fieldTotalNum;

	text = (char*)&Text;
	fieldTotalNum = sizeof(Text) / sizeof(_TextField);
	for( i=0; i<fieldTotalNum; i++ )
	{
		textfield = (_TextField*)&text[i* sizeof(_TextField)];

		if( number == textfield->siIntData )
		{
			textfield->pStringData = new char[strLength];
			ZeroMemory(textfield->pStringData, strLength );
			strcpy(textfield->pStringData, string);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: LoadData()
// Code: actdoll (2004-06-21)
// Desc: 스트링이 들어있는 텍스트 파일을 로딩한다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(스트링)
//		filename		- 데이터가 들어있는 스트링 명
//		return			- 성공적으로 로딩됐다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL _Text::LoadData( char* pszHQNation, char* pszLocalPath )
{
	OnlineCommonParser	ocp;
	char	pszFullPath[MAX_PATH];
	char	*pszFileName	= "%s\\GameText.dat";
	FILE*	fp = NULL;

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 QuestText.dat 장착
	//	만약 여기서 g_bOutsideHQText가 false이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )				// 일단 외부 폴더를 쓰라고 명령이 왔다면 확인
	{
		sprintf( pszFullPath, pszFileName, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )						// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !fp )						// 그래도 문제가 있다면 에러발생
	{
		clGrp.Error( "GameText", "%s - File Open Error", pszFullPath );
		return FALSE;;
	}

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	ocp.SetErrFileName( "c:\\ParseErr_GameText.txt" );

	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "GameText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		int		iLineNo, iColNo;
		int		iTextID;
		char	pszString[2048];

		// 컬럼별 데이터 추출
		ocp.GetValue( (int&)iTextID,	iLineNo, iColNo );		// 텍스트 번호
		ocp.GetValue( pszString, 2047,	iLineNo, iColNo );		// 스트링

		// 적재
		// actdoll (2004/11/12 12:53) : 이제 텍스트 스트링에 ""를 붙일 필요가 없게끔 한다.
//		pToken = strtok( pszString, "\"" );											// 받은 스트링에서 ""는 제낀다.
//		if( !pToken )	
//		{
//			ZeroMemory( pszString, sizeof(pszString) );						// NULL이 떨어진다면 공란인 것이다.
//		}
//		else			
//		{
//			strcpy( pszString, pToken );
//			SetStringData( iTextID, pszString, strlen(pszString) + 1 );		// 텍스트 적재
//		}

		ConvertSpecialChar( pszString );
		SetStringData( iTextID, pszString, strlen(pszString) + 1 );		// 텍스트 적재

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "GameText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
	}

	ocp.Release();
	fclose( fp );
	fp = NULL;

	return TRUE;
}

// by kuzimo
void _Text::ConvertSpecialChar(CHAR *pSpecialString)
{
	CHAR *oriString = pSpecialString;
	bool bSpecial;
	CHAR *pWriteString;

	pWriteString = pSpecialString;
	bSpecial = false;

	while( *pSpecialString != '\0' )
	{
		if( bSpecial == true )
		{
			switch(*pSpecialString)
			{
				case '0' :
					*pWriteString++ = '\0';
					return;
				case 'n' :
					*pWriteString++ = '\n';
					*pWriteString++ = '\r';
					break;
				case 't' :
					*pWriteString++ = '\t';
					break;
				default:
					break;
			}
			bSpecial = false;
		}
		else
		{
			if( *pSpecialString == '\\' )
			{
				bSpecial = true;
			}
			else
			{
				*pWriteString++ = *pSpecialString;
			}
		}
		pSpecialString++;
	}
	*pWriteString = '\0';
}


void TextInit()
{
	Text.Init();
}


void TextFree()
{
	Text.Free();
}

