#ifndef _TEXT_H
#define _TEXT_H

#define TITLE_STRING "TITLE"
#define NAME_STRING  "NAME"
#define END_STRING   "END"


class _TextField {
public:
	SI32	siIntData;
	CHAR*	pStringData;

	VOID	SetIntData(SI32 IntData){siIntData = IntData;}
	CHAR*	Get();
};

class _Text {
public:
//--------------------------------------------------------------------------------------
//           게임 메시지 
//--------------------------------------------------------------------------------------

	// 기본 텍스트
	_TextField Ok;
	_TextField Cancel;
	_TextField InsufficientMana;
	_TextField Button1;
	_TextField Button2;
	_TextField ScrollSlow;
	_TextField ScrollNormal;
	_TextField ScrollFast;
	_TextField ButtonDecide;
	_TextField ButtonReturn;
    _TextField ButtonExit;
    _TextField ButtonSearch;
	_TextField ButtonMakeRoom;
	_TextField ButtonJoin;
	_TextField ButtonReview;
	_TextField ButtonGame;
	_TextField Level;
	_TextField MoreNeed; 
	_TextField Need;
	_TextField Scenario; 
    _TextField OptionVolume;
	_TextField OptionMusic;
	_TextField OptionBright;
	_TextField OptionMouse;
	_TextField Load;
	_TextField Save;

	_TextField Korea;
	_TextField Japan;
	_TextField China;
	_TextField Random;
	_TextField NoCountry;
	_TextField Spectator;

	_TextField Rice;
	_TextField Potato;
	_TextField Tree;

	_TextField Player;
	_TextField Computer;

	_TextField TempleEvent;
	_TextField Sign;
	_TextField Ally;
	_TextField Enemy;
	_TextField AllyPlayer;     // 동맹 
	_TextField AllPlayer;      // 전체 
	_TextField PlayerStart;


//--------------------------------------------------------------------------------------
//       번역 되면 안됨. 
//--------------------------------------------------------------------------------------
	
	// 장수 식별 코드 
	_TextField General1KIDName;
	_TextField General2KIDName;
	_TextField General3KIDName;
	_TextField General4KIDName;
	_TextField General5KIDName;
	_TextField General6KIDName;
	_TextField General7KIDName;
	_TextField General8KIDName;
	_TextField General1JIDName;			
	_TextField General2JIDName;
	_TextField General3JIDName;
	_TextField General4JIDName;
	_TextField General5JIDName;
	_TextField General6JIDName;
	_TextField General7JIDName;
	_TextField General8JIDName;
	_TextField General1CIDName;
	_TextField General2CIDName;
	_TextField General3CIDName;
	_TextField General4CIDName;
	_TextField General5CIDName;
	_TextField General6CIDName;
	_TextField General7CIDName;
	_TextField KingIDName;

//--------------------------------------------------------------------------------------
//   명령. 
//--------------------------------------------------------------------------------------
	_TextField Stop;
	_TextField Move;
	_TextField Attack;
	_TextField Harvest;
	_TextField Build;
	_TextField Sell;
	_TextField Repair;
	_TextField Build1;
	_TextField Build2;
	_TextField TrainSpot;
	_TextField RideOut;
	_TextField TakeIn;
    _TextField CaptureBuild;
	_TextField Patrol;
	_TextField Convoy;
	_TextField Heal;
	_TextField MassHeal;
	_TextField Hold;
	_TextField Betray;
	_TextField SelfExp;
	_TextField Warp;
	_TextField UseBuda;
	_TextField AutoMagic;
	_TextField NoAutoMagic;
	_TextField GeneralFirst;
	_TextField NoGeneralFirst;
	_TextField FarmerToBull;

	_TextField Earthquake;
	_TextField Tornado;
	_TextField Absorb;
	_TextField Lightning;
	_TextField Shield;
	_TextField Clon;
	_TextField StealTech;
	_TextField RainMake;
	_TextField StealExp;
	_TextField GroundExp;

	
//--------------------------------------------------------------------------------------
// 캐릭터 이름. 
//--------------------------------------------------------------------------------------

	_TextField HorseArcherK;
	_TextField TShipK;

	_TextField PorterJ;
	_TextField HorseSwordJ;
	_TextField AdvBudaJ;
	_TextField NinjaJ;

	// 캐릭터의 이름 
	_TextField General2K;
	_TextField General3K;
	_TextField General4K;
	_TextField General5K;
	_TextField General2J;
	_TextField General3J;
	_TextField General4J;			// 일본 세이쇼오
	_TextField General5J;



//--------------------------------------------------------------------------------------
//     아이템 관련. 
//--------------------------------------------------------------------------------------
	// 물품 이름 
	_TextField Mine;
	_TextField RainMaker;
	_TextField SamArrow;
	_TextField Elephant;
	_TextField AirMagic;
	_TextField Bull;
	_TextField DragonBall;
	_TextField GinSeng;

	// 물품 구입 
//--------------------------------------------------------------------------------------
	// 단문의  게임 일반 메시지. 
//--------------------------------------------------------------------------------------
	_TextField NeedBuildingMessage;
	_TextField MineStockMessage;
	_TextField AirMagicStockMessage;
	_TextField BullStockMessage;

	_TextField CannotMakeThereMessage;
	_TextField QuitGameMessage;
	_TextField MerchantComeMessage;
	_TextField MerchantLeaveMessage;
	_TextField MineExplodeMessage;
	_TextField ReturnMenuMessage;
	_TextField RestartMessage;
	_TextField QuitMessage;
	_TextField NotProperSoundCardMessage;
	_TextField CheatMessage;

	_TextField HelpSellMessage;
	_TextField HelpAirMagicMessage;
	_TextField HelpFarmerToBullMessage;
	_TextField HelpMineMessage;
	_TextField HelpNoGeneralFirstMessage;
	_TextField HelpGeneralFirstMessage;
	_TextField HelpNoAutoMagicMessage;
	_TextField HelpAutoMagicMessage;
	_TextField HelpUseBudaMessage;
	_TextField HelpBetrayMessage;
	_TextField HelpTornadoMessage;

	_TextField CheatedGame;
//--------------------------------------------------------------------------------------
//       추가 텍스트
//--------------------------------------------------------------------------------------
	_TextField General7K;				// 조선 권율
	_TextField General8K;	
	_TextField General6J;				// 일본 도쿠가와
	_TextField General7J;	
	_TextField General8J;				// 일본 아사코	

	_TextField Cannon2K;
	_TextField MerchantKoreaIDName;
	_TextField MerchantJapanIDName;
	_TextField MerchantChinaIDName;

	_TextField Bow1;					// 대궁
	_TextField Bow2;					// 철력궁 
	_TextField Rifle1;					// 용쇄총
	_TextField Gun1;					// 드래군파이슨
	_TextField Cannon1;					// 철릉포
	_TextField Sword1;					// 청홍검
	_TextField Sword2;					// 폭뢰검
	_TextField Sword3;      			// 쿠사나기의 검
	_TextField Sword4;      			// 패왕도
	_TextField Sword5;      			// 칠성검
	_TextField Sword6;      			// 부동명왕검
	_TextField Sword7;      			// 용비늘검
	_TextField Spear1;      			// 방천화극
	_TextField Spear2;      			// 흑철창
	_TextField Axe1;					// 참마부도
	_TextField Book1;					// 육도삼략
	_TextField Book2;					// 손자병법
	_TextField Book3;					// 화타의책
	_TextField Necklace1;    			// 달마의 염주
	_TextField Armour1;     			// 등갑옷
	_TextField Belt1;					// 적토마의 갈기
	_TextField Belt2;					// 화룡천마의 허리띠
	_TextField Med1;					// 백설산삼
	_TextField Med2;					// 백사탕
	_TextField Med3;					// 용왕탕
	_TextField Med4;					// 웅족탕

	_TextField KimBomb;
	_TextField Bamboo;

	_TextField Crop;
	_TextField Scores;
	_TextField Prologue;
	_TextField CustomGame;
	_TextField ChangeUser;
	_TextField WinGame;
	_TextField LoseGame;
	_TextField Continue;
	_TextField Objectives;
	_TextField Optoin;
	_TextField InitMenu;
	_TextField Replay;
	_TextField On;
	_TextField Off;
	_TextField Number_1;
	_TextField Number_2;
	_TextField Number_3;
	_TextField Number_4;
	_TextField Number_5;
	_TextField Number_6;
	_TextField Number_7;
	_TextField Number_8;

	_TextField JusticeK;					// 조선 의병
	_TextField Menu;						// 설정

	_TextField Magic8j;

	_TextField Armour2;     				// 용기린갑옷
	_TextField Armour3;     				// 현무갑옷

	_TextField Magic6c;						// 송응창의 포박술
	
	_TextField Magic4c;		
	
	_TextField Extinguish;

	_TextField ShowStorage;

	_TextField Spear3;
	_TextField Stick1;
	_TextField Stick2;
	_TextField Mace1;
	_TextField Claw1;
	_TextField Armour5;
	_TextField Med5;
	_TextField GoodsSpear3;
	_TextField GoodsStick1;
	_TextField GoodsStick2;

	_TextField Armour4;     				// 수룡왕 갑옷. 

	_TextField UsePotion;
	_TextField HelpWarpMessage2;

	_TextField Ambush;
	_TextField AttackGround;
	
	_TextField ErrorMagic8jNoApower;
	_TextField ErrorBetrayHealth;
	_TextField Magic5j;
	_TextField ImjinOnline;
	_TextField DetectMine;

	_TextField Keon;
	_TextField EaterDemon;
	_TextField WeirdPlant;
	_TextField HungryDemon;
	_TextField Tiger;
	_TextField DogDemon;
	_TextField Onghwa;
	_TextField Bandit1;
	_TextField Bandit2;
	_TextField Bandit3;
	_TextField PorterK;
	_TextField Gackdagui;
	_TextField Gatpha;
	_TextField Life;
	_TextField Mana;
	_TextField Bogy;
	_TextField Fox;
	_TextField Starfish;
	_TextField Myunghwa;
	_TextField Ryo;
	_TextField Hanahine;
	_TextField Soryung;
	_TextField Miryung;
	_TextField Apower;
	_TextField Dpower;
	_TextField Bandit4;
	_TextField BrightTile;
	_TextField Oni;
	_TextField MoveSpeed;
	_TextField SnowDemon;
	_TextField FurHeadDemon;
	_TextField FireDragon;
	_TextField GoldDragon;
	_TextField Bat;
	_TextField Toad;
	_TextField Centipede;
	_TextField Dueoksini;
	_TextField Gangsi;
	_TextField RaccoonDog;
	_TextField Deer;
	_TextField OnSwordK;
	_TextField OnArcherK;
	_TextField OnSwordJ;
	_TextField OnGunJ;
	_TextField SwordPirate;
	_TextField GunPirate;
	_TextField SnowWoman;
	_TextField OnBudaK;
	_TextField HaruBang;
	_TextField Crab;
	_TextField GhostCat;
	_TextField Manager1;
	_TextField WhiteTiger;
	_TextField Tuttle;
	_TextField Butterfly;
	_TextField Widow;
	_TextField Insect;
	_TextField WorriorGhost;
	_TextField SingermGhost;
	_TextField Mammoth;
	_TextField Wildboar;
	_TextField Leech;
	_TextField SeaHorse;
	_TextField Thief;
	_TextField SeaKing;
	_TextField Bear;
	_TextField HorseSpear;
	_TextField Cat;
	_TextField Duck;
	_TextField Hornet;
	_TextField Work;
	_TextField Difficulty;
	_TextField GotFruit;
	_TextField RemainFruit;
	_TextField PorterT;
	_TextField BudaT;
	_TextField GunT;
	_TextField SwordT;
	_TextField AmazonT;
	_TextField AxeT;
	_TextField GetItemMessage;
	_TextField BadMerchant;
	_TextField MBandit1;
	_TextField MBandit2;
	_TextField MBandit3;
	_TextField Chawoo;
	_TextField Pirates;
	_TextField LostExpMessage;
	_TextField LostMoneyMessage;
	_TextField Pander;
	_TextField Snake;
	_TextField Crocodile;
	_TextField Hook;
	_TextField RetreatWarningInBattleArea;
	_TextField RetreatWarningInPeaceArea;
	_TextField SmallGang;
	_TextField HorseMan;
	_TextField BabarianShaman;
	_TextField Babarian;
	_TextField Dang;
	_TextField Suyoun;
	_TextField BadDeer;
	_TextField Cactus;
	_TextField Scorpion;
	_TextField General1T;
	_TextField General4T;
	_TextField SkyNine;
	_TextField IceTree;
	_TextField SnowLizard;
	_TextField MadBull;
	_TextField BigTree;
	_TextField Magician;
	_TextField General2T;
	_TextField General5T;
	_TextField General3T;
	_TextField Magic4T;
	_TextField Magic2T;
	//////////////////////////////
	_TextField MagicTest;
	//////////////////////////////
	_TextField MagicRecallPanda;
	_TextField Monkey;
	_TextField PorterC;
	_TextField FanC;
	_TextField BudakC;
	_TextField MudogaC;
	_TextField BigSwordC;
	_TextField FireGunC;
	_TextField FanUpC;
	_TextField BudakUpC;
	_TextField MudogaUpC;
	_TextField BigSwordUpC;
	_TextField FireGunUpC;
	_TextField General2_c;				//이령.
	_TextField General3_c;				//제갈공명.
	_TextField General4_c;				//조승훈.
	_TextField General5_c;				//축융.
	_TextField General1_c;				//동방불패.
	
	//몬스터 추가.
	_TextField KoreanGhost;				//처녀귀신.
	_TextField RunnerHand;				//괴물덩굴손.
	_TextField SandGod;					//모래의 신.
	_TextField SeaEnvoy;				//바다의 사신.
	_TextField Viper;					//살무사.
	_TextField Amakusa;					//아마쿠사.
	_TextField LavaMonster;				//용암괴물.
	_TextField Evil;					//이사의 사념.
	_TextField Bird;					//인면조.
	_TextField Weasel;					//족제비.
	_TextField Soul;					//징기스칸의 혼.
	_TextField Cobra;					//코브라.
	_TextField VolcanoFlame;			//화산의 불꽃.
	_TextField ExpLost;					//경험치 잃어버렸다는 메세지
	_TextField MagicOfficer;			//요술사령.
	_TextField CapBear;					//붉은반달곰.
	_TextField WangSoul;				//야수의 혼
	_TextField DarkFire;				//암흑화염폭풍.
	_TextField Skeleton;				//해골.
	_TextField SkeletonSoldier;			//해골병사.
	_TextField PirateCaptain;			//해적선장.
	_TextField TaiwanGhost;				//대만귀신.
	_TextField JapanGhost;				//일본귀신.
	_TextField Auta;					//아우타
	_TextField Juck;					//적고적
	_TextField FemaleShaman;			//조선무녀
	_TextField Queen;					//진성여왕
	_TextField Ivory;					//상아폭격
	_TextField BlueDragon;				//비격청룡도
	_TextField RunningFire;				//연발포탄
	_TextField EnergyPa;				//장풍
	_TextField SnowAttack;				//설공신포
	_TextField FireAttack;				//화공신포
	_TextField Donkey;					//당나귀
	_TextField FireEnergyPa;			//열화장풍(비격황룡권)
	_TextField Butcher;					//칼잡이
	_TextField Kwunyul;					//권율
	_TextField Dsword;					//쌍검낭인
	_TextField Gonisi;					//고니시
	_TextField BeastWorrior;			//야수전사
	_TextField General6T;				//손유창
	_TextField MagicC;					//도술사
	_TextField Sunbiyoun;				//순비연
	_TextField AgileTiger;				//비호
	_TextField PawnTiger;				//졸개호랑이
	_TextField ManaHeal;				//마나회복
	_TextField DoubleSword;				//고니시 쌍검난무
	_TextField LongSpear;				//손유창 장창소환
	_TextField IceBarrier;				//순비연 빙석술
	_TextField Owl;						//올빼미
	_TextField LeoPard;					//표범
	_TextField HunsWarrior;				//흉노전사
	_TextField HunsHorse;				//흉노기마병
	_TextField HunsHorseArcher;			//흉노기마궁수
	_TextField MedicalMan;				//의술사
	_TextField HeoJun;					//허준
	_TextField Cosmic;					//음양사
	_TextField Seimei;					//세이메이
	_TextField Yumruck;					//염력사
	_TextField JangSunHwa;				//장선화
	_TextField LongSpearM;				//장창무사
	_TextField KoakHu;					//곽후
	_TextField GateKeeper;				//문지기
	_TextField Clayfigure;				//기마병마용
	_TextField Phoenix;					//피닉스
	_TextField Jinemperor;				//진시황
	_TextField FlowerNeedleRain;		//만천화우
	_TextField SoulChange;				//빙의술
	_TextField StoneMagic;				//석괴술
	_TextField Santa;					//가짜산타.
	_TextField Snowman;					//눈사람
	_TextField MotherDeer;				//엄마루돌이.
	_TextField Raptor;					//랩터.
	_TextField Tirano;					//티라노.
	_TextField FlyDragon;				//익룡.
	_TextField AllySelect;				//동맹 선택.

//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 START
//----------------------------------------------------------------------------
	_TextField	HellKing;				// 염라대왕
	_TextField	HellMessenger;			// 저승사자
	_TextField	GhostSamurai;			// 유령 사무라이
	_TextField	Python;					// 이무기
	_TextField	GhostElephant;			// 유령 코끼리
	_TextField	GhostSeaking;			// 유령 해저왕
	_TextField	GhostPirateCaptain;		// 유령 해적선장
	_TextField	GhostSkeleton;			// 유령 해골
	_TextField	GhostSkeletonSoldier;	// 유령 해골병사
	_TextField	GhostFemaleShaman;		// 유령 무녀
	_TextField	GhostVolcanoFlame;		// 유령 불꽃

//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 END
//----------------------------------------------------------------------------
	_TextField GuardianMouse;			// 수호부(쥐)
	_TextField GuardianCaw;				// 수호부(소)
	_TextField GuardianChick;			// 수호부(병아리)
	_TextField GuardianPig;				// 수호부(돼지)
	_TextField GuardianSnake;			// 수호부(뱀)
	_TextField GuardianMonkey;			// 수호부(원숭이)

//----------------------------------------------------------------------------
// robypark (2004/05/7 18:42) : 추가 캐릭터 - 침략왜구 (독도)
//----------------------------------------------------------------------------
	_TextField JapInvaders;				// 침략왜구(M146, (game text)104119, (online text)8000689)

//----------------------------------------------------------------------------
// robypark (2004/05/11 13:15) : 추가 캐릭터 - 장수 2차 전직
//----------------------------------------------------------------------------
	_TextField General_TURTLE_VEHICLE;			// 270	U102	조선-거북차
	_TextField General_THUNDER_VEHICLE;			// 271	U103	조선-뇌전차
	_TextField General_QUAKE_VEHICLE;			// 272	U104	일본-지진차
	_TextField General_BLACK_DRAGON_VEHICLE;	// 273	U105	일본-흑룡차
	_TextField General_FARANGI_VEHICLE;			// 275	U106	중국-불랑기포차
	_TextField General_CATAPULT;				// 274	U107	중국-발석거
	_TextField General_FIRE_DRAGON_VEHICLE;		// 276	U108	대만-화룡차
	_TextField General_CHINESEPHOENIX;			// 277	U109	대만-봉황비조

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:10) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	_TextField Skill_FIRE_ROUND_DANCE;			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	_TextField Skill_NOVA;						// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	_TextField Skill_FURY_OF_VOLCANO;			// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	_TextField Skill_POISONING_HEAVENS;			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	_TextField Skill_BOMB_OF_HEAVEN;			// 승천포(중국-발석거, 순비연 2차 전직 기술)
	_TextField Skill_SIEGE_MODE;				// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
	_TextField Skill_ONSLAUGHT_FIREBALL;		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	_TextField Skill_FIRERAIN_OF_GOD;			// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

	// robypark 2004/6/4 14:1
	_TextField Skill_SIEGE_MODE_UNDO;			// 앙천대소에서 일반 불랑기포차로 변신(중국-불랑기포차, 이령 2차 전직 기술)
	
	// actdoll (2004/09/17 11:02) : 몇가지 하드코딩된 텍스트들
	_TextField Msg_About_Destroy_Alliance;		// %s 님이 %s 님과의 동맹을 해제했습니다.
	_TextField Msg_About_Request_Alliance;		// %s 님이 %s 님에게 동맹을 요청했습니다. 
	_TextField Msg_About_Winner_Of_Battle;		// %s 님이 전투에서 승리했습니다.
	_TextField Msg_About_Loser_Of_Battle;		// %s 님이 전투에서 패배했습니다. 

	// robypark 2004/9/17 18:29
	// 추가된 몬스터 이름
	_TextField Monster_RedEye_SeabedMan;		// 붉은 눈 해저인
	_TextField Monster_HunsWarrior_Fake;		// 흉노전사

	// robypark 2004/11/4 17:11
	// 11월 문화 유적 탐사 이벤트 몬스터 이름
	_TextField Monster_Jeopalge;				// 저팔계

	void Free();
	void Init();
	void SetText(int number, char* string);
	void SetStringData(int number, char* string, int strLength);
	BOOL LoadData( char* pszHQNation, char* pszLocalPath );
	
	private:
	void	ConvertSpecialChar(CHAR *pSpecialString);
};

void TextInit();
void TextFree();

#endif
