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
//           ���� �޽��� 
//--------------------------------------------------------------------------------------

	// �⺻ �ؽ�Ʈ
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
	_TextField AllyPlayer;     // ���� 
	_TextField AllPlayer;      // ��ü 
	_TextField PlayerStart;


//--------------------------------------------------------------------------------------
//       ���� �Ǹ� �ȵ�. 
//--------------------------------------------------------------------------------------
	
	// ��� �ĺ� �ڵ� 
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
//   ���. 
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
// ĳ���� �̸�. 
//--------------------------------------------------------------------------------------

	_TextField HorseArcherK;
	_TextField TShipK;

	_TextField PorterJ;
	_TextField HorseSwordJ;
	_TextField AdvBudaJ;
	_TextField NinjaJ;

	// ĳ������ �̸� 
	_TextField General2K;
	_TextField General3K;
	_TextField General4K;
	_TextField General5K;
	_TextField General2J;
	_TextField General3J;
	_TextField General4J;			// �Ϻ� ���̼��
	_TextField General5J;



//--------------------------------------------------------------------------------------
//     ������ ����. 
//--------------------------------------------------------------------------------------
	// ��ǰ �̸� 
	_TextField Mine;
	_TextField RainMaker;
	_TextField SamArrow;
	_TextField Elephant;
	_TextField AirMagic;
	_TextField Bull;
	_TextField DragonBall;
	_TextField GinSeng;

	// ��ǰ ���� 
//--------------------------------------------------------------------------------------
	// �ܹ���  ���� �Ϲ� �޽���. 
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
//       �߰� �ؽ�Ʈ
//--------------------------------------------------------------------------------------
	_TextField General7K;				// ���� ����
	_TextField General8K;	
	_TextField General6J;				// �Ϻ� ������
	_TextField General7J;	
	_TextField General8J;				// �Ϻ� �ƻ���	

	_TextField Cannon2K;
	_TextField MerchantKoreaIDName;
	_TextField MerchantJapanIDName;
	_TextField MerchantChinaIDName;

	_TextField Bow1;					// ���
	_TextField Bow2;					// ö�±� 
	_TextField Rifle1;					// �����
	_TextField Gun1;					// �巡�����̽�
	_TextField Cannon1;					// ö����
	_TextField Sword1;					// ûȫ��
	_TextField Sword2;					// ���ڰ�
	_TextField Sword3;      			// ��糪���� ��
	_TextField Sword4;      			// �пյ�
	_TextField Sword5;      			// ĥ����
	_TextField Sword6;      			// �ε���հ�
	_TextField Sword7;      			// ���ð�
	_TextField Spear1;      			// ��õȭ��
	_TextField Spear2;      			// ��öâ
	_TextField Axe1;					// �����ε�
	_TextField Book1;					// �����﷫
	_TextField Book2;					// ���ں���
	_TextField Book3;					// ȭŸ��å
	_TextField Necklace1;    			// �޸��� ����
	_TextField Armour1;     			// ���
	_TextField Belt1;					// ���丶�� ����
	_TextField Belt2;					// ȭ��õ���� �㸮��
	_TextField Med1;					// �鼳���
	_TextField Med2;					// �����
	_TextField Med3;					// �����
	_TextField Med4;					// ������

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

	_TextField JusticeK;					// ���� �Ǻ�
	_TextField Menu;						// ����

	_TextField Magic8j;

	_TextField Armour2;     				// ��⸰����
	_TextField Armour3;     				// ��������

	_TextField Magic6c;						// ����â�� ���ڼ�
	
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

	_TextField Armour4;     				// ����� ����. 

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
	_TextField General2_c;				//�̷�.
	_TextField General3_c;				//��������.
	_TextField General4_c;				//������.
	_TextField General5_c;				//����.
	_TextField General1_c;				//�������.
	
	//���� �߰�.
	_TextField KoreanGhost;				//ó��ͽ�.
	_TextField RunnerHand;				//����������.
	_TextField SandGod;					//���� ��.
	_TextField SeaEnvoy;				//�ٴ��� ���.
	_TextField Viper;					//�칫��.
	_TextField Amakusa;					//�Ƹ����.
	_TextField LavaMonster;				//��ϱ���.
	_TextField Evil;					//�̻��� ���.
	_TextField Bird;					//�θ���.
	_TextField Weasel;					//������.
	_TextField Soul;					//¡�⽺ĭ�� ȥ.
	_TextField Cobra;					//�ں��.
	_TextField VolcanoFlame;			//ȭ���� �Ҳ�.
	_TextField ExpLost;					//����ġ �Ҿ���ȴٴ� �޼���
	_TextField MagicOfficer;			//������.
	_TextField CapBear;					//�����ݴް�.
	_TextField WangSoul;				//�߼��� ȥ
	_TextField DarkFire;				//����ȭ����ǳ.
	_TextField Skeleton;				//�ذ�.
	_TextField SkeletonSoldier;			//�ذ񺴻�.
	_TextField PirateCaptain;			//��������.
	_TextField TaiwanGhost;				//�븸�ͽ�.
	_TextField JapanGhost;				//�Ϻ��ͽ�.
	_TextField Auta;					//�ƿ�Ÿ
	_TextField Juck;					//������
	_TextField FemaleShaman;			//��������
	_TextField Queen;					//��������
	_TextField Ivory;					//�������
	_TextField BlueDragon;				//���û�浵
	_TextField RunningFire;				//������ź
	_TextField EnergyPa;				//��ǳ
	_TextField SnowAttack;				//��������
	_TextField FireAttack;				//ȭ������
	_TextField Donkey;					//�糪��
	_TextField FireEnergyPa;			//��ȭ��ǳ(���Ȳ���)
	_TextField Butcher;					//Į����
	_TextField Kwunyul;					//����
	_TextField Dsword;					//�ְ˳���
	_TextField Gonisi;					//��Ͻ�
	_TextField BeastWorrior;			//�߼�����
	_TextField General6T;				//����â
	_TextField MagicC;					//������
	_TextField Sunbiyoun;				//����
	_TextField AgileTiger;				//��ȣ
	_TextField PawnTiger;				//����ȣ����
	_TextField ManaHeal;				//����ȸ��
	_TextField DoubleSword;				//��Ͻ� �ְ˳���
	_TextField LongSpear;				//����â ��â��ȯ
	_TextField IceBarrier;				//���� ������
	_TextField Owl;						//�û���
	_TextField LeoPard;					//ǥ��
	_TextField HunsWarrior;				//�������
	_TextField HunsHorse;				//���⸶��
	_TextField HunsHorseArcher;			//���⸶�ü�
	_TextField MedicalMan;				//�Ǽ���
	_TextField HeoJun;					//����
	_TextField Cosmic;					//�����
	_TextField Seimei;					//���̸���
	_TextField Yumruck;					//���»�
	_TextField JangSunHwa;				//�弱ȭ
	_TextField LongSpearM;				//��â����
	_TextField KoakHu;					//����
	_TextField GateKeeper;				//������
	_TextField Clayfigure;				//�⸶������
	_TextField Phoenix;					//�Ǵн�
	_TextField Jinemperor;				//����Ȳ
	_TextField FlowerNeedleRain;		//��õȭ��
	_TextField SoulChange;				//���Ǽ�
	_TextField StoneMagic;				//������
	_TextField Santa;					//��¥��Ÿ.
	_TextField Snowman;					//�����
	_TextField MotherDeer;				//�����絹��.
	_TextField Raptor;					//����.
	_TextField Tirano;					//Ƽ���.
	_TextField FlyDragon;				//�ͷ�.
	_TextField AllySelect;				//���� ����.

//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� START
//----------------------------------------------------------------------------
	_TextField	HellKing;				// ������
	_TextField	HellMessenger;			// ���»���
	_TextField	GhostSamurai;			// ���� �繫����
	_TextField	Python;					// �̹���
	_TextField	GhostElephant;			// ���� �ڳ���
	_TextField	GhostSeaking;			// ���� ������
	_TextField	GhostPirateCaptain;		// ���� ��������
	_TextField	GhostSkeleton;			// ���� �ذ�
	_TextField	GhostSkeletonSoldier;	// ���� �ذ񺴻�
	_TextField	GhostFemaleShaman;		// ���� ����
	_TextField	GhostVolcanoFlame;		// ���� �Ҳ�

//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� END
//----------------------------------------------------------------------------
	_TextField GuardianMouse;			// ��ȣ��(��)
	_TextField GuardianCaw;				// ��ȣ��(��)
	_TextField GuardianChick;			// ��ȣ��(���Ƹ�)
	_TextField GuardianPig;				// ��ȣ��(����)
	_TextField GuardianSnake;			// ��ȣ��(��)
	_TextField GuardianMonkey;			// ��ȣ��(������)

//----------------------------------------------------------------------------
// robypark (2004/05/7 18:42) : �߰� ĳ���� - ħ���ֱ� (����)
//----------------------------------------------------------------------------
	_TextField JapInvaders;				// ħ���ֱ�(M146, (game text)104119, (online text)8000689)

//----------------------------------------------------------------------------
// robypark (2004/05/11 13:15) : �߰� ĳ���� - ��� 2�� ����
//----------------------------------------------------------------------------
	_TextField General_TURTLE_VEHICLE;			// 270	U102	����-�ź���
	_TextField General_THUNDER_VEHICLE;			// 271	U103	����-������
	_TextField General_QUAKE_VEHICLE;			// 272	U104	�Ϻ�-������
	_TextField General_BLACK_DRAGON_VEHICLE;	// 273	U105	�Ϻ�-�����
	_TextField General_FARANGI_VEHICLE;			// 275	U106	�߱�-�Ҷ�������
	_TextField General_CATAPULT;				// 274	U107	�߱�-�߼���
	_TextField General_FIRE_DRAGON_VEHICLE;		// 276	U108	�븸-ȭ����
	_TextField General_CHINESEPHOENIX;			// 277	U109	�븸-��Ȳ����

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:10) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	_TextField Skill_FIRE_ROUND_DANCE;			// ����������(����-�ź���, �̼��� 2�� ���� ���)
	_TextField Skill_NOVA;						// ����(����-������, ���� 2�� ���� ���)
	_TextField Skill_FURY_OF_VOLCANO;			// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	_TextField Skill_POISONING_HEAVENS;			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	_TextField Skill_BOMB_OF_HEAVEN;			// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
	_TextField Skill_SIEGE_MODE;				// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
	_TextField Skill_ONSLAUGHT_FIREBALL;		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	_TextField Skill_FIRERAIN_OF_GOD;			// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 

	// robypark 2004/6/4 14:1
	_TextField Skill_SIEGE_MODE_UNDO;			// ��õ��ҿ��� �Ϲ� �Ҷ��������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
	
	// actdoll (2004/09/17 11:02) : ��� �ϵ��ڵ��� �ؽ�Ʈ��
	_TextField Msg_About_Destroy_Alliance;		// %s ���� %s �԰��� ������ �����߽��ϴ�.
	_TextField Msg_About_Request_Alliance;		// %s ���� %s �Կ��� ������ ��û�߽��ϴ�. 
	_TextField Msg_About_Winner_Of_Battle;		// %s ���� �������� �¸��߽��ϴ�.
	_TextField Msg_About_Loser_Of_Battle;		// %s ���� �������� �й��߽��ϴ�. 

	// robypark 2004/9/17 18:29
	// �߰��� ���� �̸�
	_TextField Monster_RedEye_SeabedMan;		// ���� �� ������
	_TextField Monster_HunsWarrior_Fake;		// �������

	// robypark 2004/11/4 17:11
	// 11�� ��ȭ ���� Ž�� �̺�Ʈ ���� �̸�
	_TextField Monster_Jeopalge;				// ���Ȱ�

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
