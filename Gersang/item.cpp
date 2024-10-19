#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <clGame.h>

#include "maindata.h"
#include <etc.h>
#include <item.h>
#include "screenpoint.h"
#include "music.h"
#include <chat.h>
#include <OnlineWorld.h>

#include "help.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <MapInterface.h>
#include <CharOrg.h>
#include <charinterface.h>
#include "text.h"
#include <gamebutton.h>
#include <MyNet-Game.h>

extern	_clGame					*pGame;
extern	_Item 				Item[];
extern	_MainData			GD;									// 게임의 주데이터 
extern	_Text 				Text;								// 텍스트 
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	BOOL				bGameAllyWindowSwitch;
extern	SHORT 				MenuOfMouse;						// 마우스 커서가 위치하고 있는 메뉴의 번호 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	cltMapInterface		clifMap;							// 맵 관련 인터페이스 
extern	int 				MenuMode;							// 메뉴의 상태를 저장 
extern	_Order 				Order[];
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern  _Chat 				Chat;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;				// 마우스 왼쪽 버튼
extern	int 				ControlKeyPressSwitch;				// 컨트롤키 상태
extern	int 				ShiftKeyPressSwitch;				// 시프트키 상태
extern	SHORT 				CrewMenuOfMouse;
extern	_Help 				Help;
extern _InputDevice			IpD;
extern	BOOL 				IsMosueInControlSwitch;				// 마우스가 게임 콘트롤위에 위치하고 있다.
extern	clock_t				CurrentClock;						// 시간 관련 
extern	_NewID 				ZeroID;
extern	int 				GeneralList[];						// 국가별 장수의 리스트 
extern SHORT				GameRightPressSwitch;

extern	unsigned char		TeamLightTable[];
extern	unsigned char		GrayTable[];

static	BOOL				bOnlineAllySwitch = FALSE;
extern	_MyNetGame			MyNet;

// robypark 2004/10/13 11:43
// 디버그 모드에서만 전투 중 여러 캐릭터 선택시 캐릭터들의 초상화, 체력, 마나 등을 채팅 리스트 박스에 보여진다.
#ifdef _DEBUG
#define MULTI_SELECT_PORTRAIT_WIDTH		50		// 여러 캐릭터 선택시 보여지는 초상 이미지의 가로 크기
#define MULTI_SELECT_PORTRAIT_HEIGHT	32		// 여러 캐릭터 선택시 보여지는 초상 이미지의 세로 크기
#define MULTI_SELECT_START_POSITION_X	218		// 여러 캐릭터 선택시 보여지는 초상 이미지들 중에 첫번째 캐릭터 초상 이미지 위치(X)
#define MULTI_SELECT_START_POSITION_Y	508		// 여러 캐릭터 선택시 보여지는 초상 이미지들 중에 첫번째 캐릭터 초상 이미지 위치(Y)
#define MULTI_SELECT_GAP_WIDTH_SIZE		2		// 여러 캐릭터 선택시 보여지는 초상 이미지들 간의 띄어진 거리(가로)
#define MULTI_SELECT_GAP_HEIGHT_SIZE	2		// 여러 캐릭터 선택시 보여지는 초상 이미지들 간의 띄어진 거리(세로)
#define MULTI_SELECT_GROUP_ROW			2		// 그룹 선택 그려질 때 보여지는 초상화 세로 개수
#define MULTI_SELECT_GROUP_COLUMN		7		// 그룹 선택 그려질 때 보여지는 초상화 가로 개수
#define MULTI_SELECT_MAX_COUNT			(MULTI_SELECT_GROUP_ROW * MULTI_SELECT_GROUP_COLUMN)		// 한 그룹으로 선택가능한 캐릭터 개수

#define MULTI_SELECT_BKGND_IMAGE_POSITION_X			201			// 그룹 선택 배경 이미지 위치(X)
#define MULTI_SELECT_BKGND_IMAGE_POSITION_Y			502			// 그룹 선택 배경 이미지 위치(Y)
#define MULTI_SELECT_BKGND_IMAGE_WIDTH				398			// 그룹 선택 배경 이미지 위치(WIDTH)
#define MULTI_SELECT_BKGND_IMAGE_HEIGHT				72			// 그룹 선택 배경 이미지 위치(HEIGHT)
#endif // _DEBUG

// 마우스나 키보드에 의해서 선택된 메뉴의 인덱스 값 
SHORT	SelectedMenu = -1;
// 선택된 메뉴가 있는지 여부를 알려주는 변수 
SHORT	MenuSelectedSwitch = FALSE;

int		SelectedCrewMenu = -1;
SHORT	CrewMenuSelectedSwitch = FALSE;

SHORT	LeftPressSelectedSwitch = FALSE;
SHORT	RightPressSelectedSwitch = FALSE;
SHORT	CrewMenuLeftPressSelectedSwitch = FALSE;


void _Item::SetItem(SHORT order, SHORT font, SHORT multiswitch, char hotkey1, char hotkey2, char* text, char*text2)
{
	Order=order;
	Font=font;
	MultiSwitch=multiswitch;
	HotKeyCode[0]=hotkey1;
	HotKeyCode[1]=hotkey2;
	Text=text;
	Text2=text2;
}

SHORT _Item::GetOrder()
{
	return Order;
}

SHORT _Item::GetFont()
{
	return Font;
}

SHORT _Item::GetMultiSwitch()
{
	return MultiSwitch;
}

unsigned char _Item::GetHotKeyCode(int vary)
{
	return HotKeyCode[vary];
}

char* _Item::GetText()
{
	return Text;
}

char* _Item::GetText2()
{
	return Text2;
}

DWORD _Item::GetUnique()
{
	return Order + Font + MultiSwitch + HotKeyCode[0] + HotKeyCode[1];
}

void ItemUniqueIdenty()
{
	int i;
	DWORD unique = 0;

	for(i = 0 ;i < MAX_ITEM_NUMBER;i++)
	{
		unique += Item[i].GetUnique(); 
	}

	if(unique != 99505)
	{
		clGrp.Error("Fdsf", "frame:%d unqiue:%d", GD.Frame, unique);
	}
}

// 모든 메뉴 아이템을 초기화 한다. 
void ItemInit()
{

	// 일반 아이템 
	Item[ITEM_STOP].SetItem       (ORDER_STOP,        43, 
		TRUE, 'S', 's', Text.Stop.Get(), NULL);
	Item[ITEM_MOVE].SetItem       (ORDER_MOVE,        6, 
		TRUE, 'M', 'm', Text.Move.Get(), NULL);
	Item[ITEM_ATTACK].SetItem     (ORDER_ATTACK,      4, 
		TRUE, 'A', 'a', Text.Attack.Get(), NULL);
	Item[ITEM_ATTACKGROUND].SetItem     (ORDER_ATTACKGROUND,      21, 
		TRUE, 'G', 'g', Text.AttackGround.Get(), NULL);
	Item[ITEM_HARVEST].SetItem    (ORDER_HARVEST,     41, 
		TRUE, 'V', 'v', Text.Harvest.Get(), NULL);
	Item[ITEM_EXTINGUISH].SetItem    (ORDER_EXTINGUISH,     5, 
		FALSE, 'W', 'w', Text.Extinguish.Get(), NULL);
	Item[ITEM_BUILD].SetItem      (ORDER_BUILD,       16, 
		FALSE, 'B', 'b', Text.Build.Get(), NULL);
	Item[ITEM_SELL].SetItem      (ORDER_SELL,         13, 
		FALSE, 'D', 'd', Text.Sell.Get(), NULL);
	Item[ITEM_REPAIR].SetItem    (ORDER_REPAIR,       12, 
		TRUE, 'R', 'r', Text.Repair.Get(), NULL);
	Item[ITEM_BUILD1].SetItem    (ORDER_BUILD1,       18, 
		FALSE, 'Z', 'z', Text.Build1.Get(), NULL);
	Item[ITEM_BUILD2].SetItem    (ORDER_BUILD2,       19, 
		FALSE, 'X', 'x', Text.Build2.Get(), NULL);
	Item[ITEM_INITMENU].SetItem  (ORDER_INITMENU,     45, 
		FALSE, 'C', 'c', Text.Cancel.Get(), NULL);
	Item[ITEM_SELLMENU].SetItem  (ORDER_SELLMENU,     13, 
		FALSE, 'D', 'd', Text.Sell.Get(), Text.HelpSellMessage.Get());
	Item[ITEM_BUILDCANCEL].SetItem(ORDER_SELL,        45, 
		FALSE, 'S', 's', Text.Cancel.Get(), NULL);
	Item[ITEM_TRAINSPOT].SetItem (ORDER_TRAINSPOT,    11, 
		FALSE, 'T', 't', Text.TrainSpot.Get(), NULL);
	Item[ITEM_RIDEOUT].SetItem   (ORDER_MOVERIDEOUT,  48, 
		TRUE, 'D', 'd', Text.RideOut.Get(), NULL);
	Item[ITEM_TAKEIN].SetItem    (ORDER_TAKEIN,       47, 
		TRUE, 'T', 't', Text.TakeIn.Get(), NULL);
	Item[ITEM_CAPTUREBUILD].SetItem(ORDER_CAPTUREBUILD,42, 
		FALSE, 'C', 'c', Text.CaptureBuild.Get(), NULL);
	Item[ITEM_PATROL].SetItem    (ORDER_PATROL,        10, 
		TRUE, 'P', 'p', Text.Patrol.Get(), NULL);
	Item[ITEM_CONVOY].SetItem    (ORDER_CONVOY,        37, 
		FALSE, 'C', 'c', Text.Convoy.Get(), NULL);
	Item[ITEM_MAGIC_HEAL].SetItem (ORDER_HEAL,         46, 
		TRUE, 'F', 'f', Text.Heal.Get(), NULL);
	Item[ITEM_MAGIC_MANA].SetItem (ORDER_MANA,         46, 
		TRUE, 'M', 'M', Text.ManaHeal.Get(), NULL);
	Item[ITEM_MAGIC_MASSHEAL].SetItem(ORDER_MASSHEAL,  25, 
		TRUE, 'N', 'n', Text.MassHeal.Get(), NULL);
	Item[ITEM_HOLD].SetItem      (ORDER_HOLD,          39, 
		TRUE, 'H', 'h', Text.Hold.Get(), NULL);
	Item[ITEM_BETRAY].SetItem    (ORDER_BETRAY,        49, 
		FALSE, 'B', 'b', Text.Betray.Get(), Text.HelpBetrayMessage.Get());
	Item[ITEM_SELFEXP].SetItem   (ORDER_SELFEXP,       82, 
		TRUE, 'E', 'e', Text.SelfExp.Get(), NULL);
	Item[ITEM_WARP].SetItem      (ORDER_WARP,          81, 
		TRUE, 'W', 'w', Text.Warp.Get(), Text.HelpWarpMessage2.Get());
	Item[ITEM_MAGIC_USEBUDA].SetItem  (ORDER_USEBUDA,  74, 
		FALSE, 'U', 'u', Text.UseBuda.Get(), Text.HelpUseBudaMessage.Get());
	Item[ITEM_AUTOMAGIC].SetItem  (ORDER_AUTOMAGIC,  27, 
		FALSE, 0, 0, Text.AutoMagic.Get(), Text.HelpAutoMagicMessage.Get());
	Item[ITEM_NOAUTOMAGIC].SetItem(ORDER_NOAUTOMAGIC,  26, 
		FALSE, 0, 0, Text.NoAutoMagic.Get(), Text.HelpNoAutoMagicMessage.Get());
	Item[ITEM_GENERALFIRST].SetItem  (ORDER_GENERALFIRST,  28, 
		FALSE, 0, 0, Text.GeneralFirst.Get(), Text.HelpGeneralFirstMessage.Get());
	Item[ITEM_NOGENERALFIRST].SetItem(ORDER_NOGENERALFIRST,  29, 
		FALSE, 0, 0, Text.NoGeneralFirst.Get(), Text.HelpNoGeneralFirstMessage.Get());
	Item[ITEM_MINE].SetItem(ORDER_MINE,  1, 
		FALSE, 'E', 'e', Text.Mine.Get(), Text.HelpMineMessage.Get());
	Item[ITEM_AIRMAGIC].SetItem (ORDER_AIRMAGIC,     38, 
		FALSE, 'E', 'e', Text.AirMagic.Get(), Text.HelpAirMagicMessage.Get());
	Item[ITEM_KIMBOMB].SetItem	(ORDER_KIMBOMB,     7, 
		FALSE, 'I', 'i', Text.KimBomb.Get(), NULL);
	Item[ITEM_MAGIC5J].SetItem	(ORDER_MAGIC5J,     16, 
		FALSE, 'I', 'i', Text.Magic5j.Get(), NULL);
	Item[ITEM_FARMERTOBULL].SetItem (ORDER_FARMERTOBULL, 54, 
		FALSE, 'B', 'b', Text.FarmerToBull.Get(), Text.HelpFarmerToBullMessage.Get());
	Item[ITEM_SHOWSTORAGE].SetItem (ORDER_SHOWSTORAGE, 15, 
		FALSE, 'A', 'a', Text.ShowStorage.Get(), NULL);

	Item[ITEM_USEPOTION].SetItem (ORDER_USEPOTION, 8, 
		FALSE, 'O', 'o', Text.UsePotion.Get(), NULL);
	
	Item[ITEM_AMBUSH].SetItem	(ORDER_AMBUSH,	   23, 
		TRUE, 'O', 'o', Text.Ambush.Get(), NULL);
	Item[ITEM_DETECTMINE].SetItem	(ORDER_DETECTMINE,	   89, 
		TRUE, 'E', 'e', Text.DetectMine.Get(), NULL);
	Item[ITEM_MAGIC_EARTHQUAKE].SetItem(ORDER_EARTHQUAKE,    72, 
		FALSE, 'E', 'e', Text.Earthquake.Get(), NULL);
	Item[ITEM_MAGIC_TORNADO].SetItem(ORDER_TORNADO,         114, 
		FALSE, 'E', 'e', Text.Tornado.Get(), Text.HelpTornadoMessage.Get());
	Item[ITEM_MAGIC_ABSORB].SetItem(ORDER_ABSORB,         75, 
		FALSE, 'R', 'r', Text.Absorb.Get(), NULL);
	Item[ITEM_MAGIC_MAGIC8J].SetItem(ORDER_MAGIC8J,         9, 
		FALSE, 'R', 'r', Text.Magic8j.Get(), NULL);
	Item[ITEM_MAGIC_LIGHTNING].SetItem(ORDER_LIGHTNING,    73, 
		FALSE, 'T', 't', Text.Lightning.Get(), NULL);
	Item[ITEM_MAGIC_SHIELD].SetItem(ORDER_SHIELD,          83, 
		FALSE, 'C', 'c', Text.Shield.Get(), NULL);
	Item[ITEM_MAGIC_CLON].SetItem   (ORDER_CLON,           69, 
		FALSE, 'C', 'c', Text.Clon.Get(), NULL);
	Item[ITEM_MAGIC_STEALTECH].SetItem(ORDER_STEALTECH,    71, 
		FALSE, 'T', 't', Text.StealTech.Get(), NULL);
	Item[ITEM_MAGIC_RAINMAKE].SetItem (ORDER_RAINMAKE,     24, 
		FALSE, 'R', 'r', Text.RainMake.Get(), NULL);
	Item[ITEM_MAGIC_STEALEXP].SetItem (ORDER_STEALEXP,     70, 
		TRUE, 'E', 'e',  Text.StealExp.Get(), NULL);
    Item[ITEM_MAGIC_GROUNDEXP].SetItem(ORDER_GROUNDEXP,    153, 
		FALSE, 'E', 'e', Text.GroundExp.Get(), NULL);
    Item[ITEM_MAGIC_MAGIC4C].SetItem(ORDER_MAGIC4C,    17, 
		FALSE, 'F', 'f', Text.Magic4c.Get(), NULL);
    Item[ITEM_MAGIC_MAGIC6C].SetItem(ORDER_MAGIC6C,    14, 
		FALSE, 'F', 'f', Text.Magic6c.Get(), NULL);
	Item[ITEM_MAGIC_MAGIC4T].SetItem(ORDER_MAGIC4T,    216, 
		FALSE, 'B', 'b', Text.Magic4T.Get(), NULL);
	Item[ITEM_MAGIC_MAGIC2T].SetItem(ORDER_MAGIC2T,    217, 
		FALSE, 'C', 'c', Text.Magic2T.Get(), NULL);
	//////////////////////////////////////////////////////////////
	Item[ITEM_MAGIC_MAGICTEST].SetItem(ORDER_MAGICTEST, 76,
		FALSE,'T', 't',Text.MagicTest.Get(),NULL);
	//////////////////////////////////////////////////////////////
	Item[ITEM_MAGIC_RECALL_PANDA].SetItem(ORDER_RECALL_PANDA,77,
		FALSE,'D', 'd',Text.MagicRecallPanda.Get(),NULL);
	Item[ITEM_MAGIC_RECALL_MONKEY].SetItem(ORDER_RECALL_MONKEY,2,
		FALSE,'M', 'm',Text.Monkey.Get(),NULL);
	Item[ITEM_MAGIC_ICE].SetItem(ORDER_ICE,80,
		FALSE,'D', 'd',NULL,NULL);
	Item[ITEM_MAGIC_SOUL].SetItem(ORDER_SOUL,84,
		FALSE,'Q', 'q',Text.WangSoul.Get(),NULL);
//	Item[ITEM_MAGIC_GHOST].SetItem(ORDER_GHOST,85,
//		FALSE,'G', 'g',Text.MagicRecallGoldDragon.Get(),NULL);
	Item[ITEM_MAGIC_DARKFIRE].SetItem(ORDER_DARKFIRE,86,
		FALSE,'T', 't',Text.DarkFire.Get(),NULL);
	Item[ITEM_MAGIC_YELLOWSAND].SetItem(ORDER_YELLOWSAND,86,
		FALSE,'E', 'e',Text.DarkFire.Get(),NULL);
	Item[ITEM_MAGIC_IVORY].SetItem(ORDER_IVORY,86,
		FALSE,'V', 'v',Text.Ivory.Get(),NULL);
	Item[ITEM_MAGIC_BLUEDRAGON].SetItem(ORDER_BLUEDRAGON,86,
		FALSE,'B', 'b',Text.BlueDragon.Get(),NULL);
	Item[ITEM_MAGIC_RUNNINGFIRE].SetItem(ORDER_RUNNINGFIRE,91,
		FALSE,'R', 'r',Text.RunningFire.Get(),NULL);
	Item[ITEM_MAGIC_ENERGYPA].SetItem(ORDER_ENERGYPA,94,
		FALSE,'E', 'e',Text.EnergyPa.Get(),NULL);
	Item[ITEM_MAGIC_SNOWATTACK].SetItem(ORDER_SNOWATTACK,86,
		FALSE,'W', 'w',Text.SnowAttack.Get(),NULL);
	Item[ITEM_MAGIC_FIREATTACK].SetItem(ORDER_FIREATTACK,92,
		FALSE,'F', 'f',Text.FireAttack.Get(),NULL);
	Item[ITEM_MAGIC_FIREENERGYPA].SetItem(ORDER_FIREENERGYPA,93,
		FALSE,'F', 'f',Text.FireEnergyPa.Get(),NULL);
	Item[ITEM_MAGIC_DOUBLE_SWORD].SetItem(ORDER_DOUBLE_SWORD,31,
		FALSE,'E', 'e',Text.DoubleSword.Get(),NULL);
	Item[ITEM_MAGIC_LONG_SPEAR].SetItem(ORDER_LONG_SPEAR,30,
		FALSE,'F', 'f',Text.LongSpear.Get(),NULL);
	Item[ITEM_MAGIC_ICE_BARRIER].SetItem(ORDER_ICE_BARRIER,34,
		FALSE,'R', 'r',Text.IceBarrier.Get(),NULL);
	Item[ITEM_MAGIC_FLOWER_NEEDLE_RAIN].SetItem(ORDER_FLOWER_NEEDLE_RAIN,35,
		FALSE,'E', 'e',Text.FlowerNeedleRain.Get(),NULL);
	Item[ITEM_MAGIC_SOUL_CHANGE].SetItem(ORDER_SOUL_CHANGE,32,
		FALSE,'C', 'c',Text.SoulChange.Get(),NULL);
	Item[ITEM_MAGIC_STONE_MAGIC].SetItem(ORDER_STONE_MAGIC,33,
		FALSE,'T', 't',Text.StoneMagic.Get(),NULL);
	Item[ITEM_MAGIC_MANABURN].SetItem(ORDER_MANABURN,33,
		FALSE,'B', 'B',"마나번",NULL);

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:17) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	Item[ITEM_MAGIC_FIRE_ROUND_DANCE].SetItem(ORDER_FIRE_ROUND_DANCE, 19,
								FALSE,'F', 'f',Text.Skill_FIRE_ROUND_DANCE.Get(), NULL);
	
	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	Item[ITEM_MAGIC_NOVA].SetItem(ORDER_NOVA, 10,
								FALSE,'N', 'n',Text.Skill_NOVA.Get(),NULL);
	
	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	Item[ITEM_MAGIC_FURY_OF_VOLCANO].SetItem(ORDER_FURY_OF_VOLCANO, 12,
								FALSE,'V', 'v',Text.Skill_FURY_OF_VOLCANO.Get(), NULL);
	
	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	Item[ITEM_MAGIC_POISONING_HEAVENS].SetItem(ORDER_POISONING_HEAVENS, 18,
								FALSE,'P', 'p',Text.Skill_POISONING_HEAVENS.Get(), NULL);
	
	// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
	Item[ITEM_MAGIC_BOMB_OF_HEAVEN].SetItem(ORDER_BOMB_OF_HEAVEN, 11,
								FALSE,'B', 'b',Text.Skill_BOMB_OF_HEAVEN.Get(), NULL);
	
	// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	Item[ITEM_MAGIC_SIEGE_MODE].SetItem(ORDER_SIEGE_MODE, 41,
								FALSE,'I', 'i',Text.Skill_SIEGE_MODE.Get(), NULL);
	
	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	Item[ITEM_MAGIC_ONSLAUGHT_FIREBALL].SetItem(ORDER_ONSLAUGHT_FIREBALL, 22, 
								FALSE,'B', 'b',Text.Skill_ONSLAUGHT_FIREBALL.Get(), NULL);

	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
	Item[ITEM_MAGIC_FIRERAIN_OF_GOD].SetItem(ORDER_FIRERAIN_OF_GOD, 20, 
								FALSE,'R', 'r',Text.Skill_FIRERAIN_OF_GOD.Get(), NULL);

	// robypark 2004/6/4 14:5
	// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	Item[ITEM_MAGIC_SIEGE_MODE_UNDO].SetItem(ORDER_SIEGE_MODE_UNDO, 41,
								FALSE,'I', 'i',Text.Skill_SIEGE_MODE_UNDO.Get(), NULL);
}


BOOL IsMouseInMenu(SHORT number)
{
	if(MapDragSwitch==TRUE)return FALSE;

	// 존재하지 않는 메뉴는 확인할 수 없다. 
	if(GD.GetMenu(number, 138)==0)return FALSE;

    int order = Item[GD.GetMenu(number, 10)].GetOrder();

	if(GameMouseX>SP.MenuStartX+(number%SP.MaxMenuXCount)*(SP.MenuButtonXsize+SP.MenuMarginX)
	&& GameMouseX<SP.MenuStartX+(number%SP.MaxMenuXCount)*(SP.MenuButtonXsize+SP.MenuMarginX)+SP.MenuButtonXsize
	&& GameMouseY>SP.MenuStartY+(number/SP.MaxMenuXCount)*(SP.MenuButtonYsize+SP.MenuMarginY)
	&& GameMouseY<SP.MenuStartY+(number/SP.MaxMenuXCount)*(SP.MenuButtonYsize+SP.MenuMarginY)+SP.MenuButtonYsize)
	   return TRUE;

	return FALSE;
	
}



// 전역 변수에 메뉴를 넣는다. 
BOOL PushGMenu(SHORT index, SHORT menu, SHORT reservenumber, SHORT mode, SHORT menupercent)
{
	int i;
	int alreadyswitch=FALSE;

	// 이미 있는 명령이면 
	for(i=0;i<MAX_MENU_NUMBER;i++)
	{
		if(GD.GetMenu(i, 20)==menu)
		{
			// 기존에 FALSE, 새로넣은 것이 TRUE이면, 
			if(mode==TRUE && GD.MenuMode[i]==FALSE)
			{
				GD.MenuMode[i]=mode;
				return FALSE;
			}
			
			return FALSE;
		}
	}

    GD.SetMenu(index, menu, reservenumber, mode, menupercent);

    return TRUE;

}

SI32 OnlineInventoryFunc(SI32 siPara1, SI32 siPara2)
{
	return 0;
}

SI32 OnlineRegisterFunc(SI32 siPara1, SI32 siPara2)
{
	return 0;
}

SI32 OnlineAllyNumFunc(SI32 siPara1, SI32 siPara2)
{
	// 무도장에서는 동맹을 맺거나 해제하는 것이 가능하다. 
	if(pGame->pOnlineWorld->CheckPKFieldMap() == TRUE)
	{
		// 동맹여부(0: 동맹 없음, 1: 동맹 신청요청, 2: 동맹 신청받음, 3: 동맹 상태)
		MakeOrder(UserPlayerNumber, ORDER_ALLY, siPara1, ZeroID, TRUE); 
	}

	return 0;
}

SI32 GameMenuFunc(SI32 siPara1, SI32 siPara2)
{
	// 게임 메뉴를 보여주도록 한다. 
	GameMenuShowSwitch = TRUE;
	
	return 0;
}

SI32 GameAllyWindowCloseFunc(SI32 siPara1, SI32 siPara2)
{
	bGameAllyWindowSwitch = TRUE;
	return 0;
}

SI32 GameAllyWindowOpenFunc(SI32 siPara1, SI32 siPara2)
{
	bGameAllyWindowSwitch = FALSE;
	return 0;
}

// 상황을 설정하기 전에 모든 변수를 초기화한다.
void InitOperateMenu()
{
	SHORT i;

	// '마우스가 위치한 메뉴는 없다'로 초기화 
	MenuOfMouse=-1;
	
	// 게임에 사용되는 버튼들을 초기화한다. 
	pGame->pBattle->InitGameButton();

	// 메인 메뉴 버퍼를 초기화한다. 
	for(i=0;i<MAX_MENU_NUMBER;i++)
	{
		GD.SetMenu(i, 0, 0, FALSE, 0);
	}

}

// 메뉴 입력등을 처리한다. 
void OperateMenu()
{
	int i, j, k;
	int id=0;
	int country=GD.Player[UserPlayerNumber].PI.Country;
	int enablemode=TRUE;
	int cancelmode=FALSE;
	char *text2=NULL;
	char textbuffer[128];
	SHORT kind;
	SI32 font;

	// 상황을 설정하기 전에 모든 변수를 초기화한다.
	InitOperateMenu();
	
	SI32	starty;
	
	font = FILE_ON_BUTTMENU;
	
	//  게임 설정 버튼을 만든다. 
	id = pGame->pBattle->FindEmptyGameButton();
	if(id)
	{
		starty = SP.GameMenuButtonStartY;
		// 온라인 전투 모드에서 설정 버튼. 
		pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_GAMEMENU, SP.GameMenuButtonStartX, starty, 
			font,	0, FALSE,
			font,	1, FALSE,
			font,	1, FALSE,
			NULL,
			Text.Menu.Get(),
			EFFECT_CLICK,
			0, 0, 
			GameMenuFunc);
	}

	

	if(bGameAllyWindowSwitch == FALSE)
	{
		font = FILE_ON_ALLY_BUTTON_CLOSE;

		id = pGame->pBattle->FindEmptyGameButton();
		if(id)
		{
			pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_WINDOW,776,394 ,
				font, 0, FALSE,
				font, 1, FALSE,
				font, 1, FALSE,
				NULL,
				NULL,
				EFFECT_CLICK,
				0,0,
				GameAllyWindowCloseFunc);
		}
	}
	if(bGameAllyWindowSwitch == TRUE)
	{
		font = FILE_ON_ALLY_BUTTON_OPEN;

		id = pGame->pBattle->FindEmptyGameButton();
		if(id)
		{
			pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_WINDOW,776,485,
				font, 0, FALSE,
				font, 1, FALSE,
				font, 1, FALSE,
				NULL,
				NULL,
				EFFECT_CLICK,
				0,0,
				GameAllyWindowOpenFunc);
		}
	}


	// 동맹자 선택 체크버튼들
	font = FILE_ON_ALLY_CHECK;
	
	
	k = 0;
	for( i=0; i<MAX_MULTI_PLAYER-1; i++ )
	{
		// 유효한 플레이이어인 경우 , 
		if( MyNet.PlayersdpId.dpId[i] ) 
		{
			if( ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE && i != UserPlayerNumber )
			{
				if(bGameAllyWindowSwitch == FALSE)
				{
					//  동맹 버튼을 만든다. 
					id = pGame->pBattle->FindEmptyGameButton();
					if( id )
					{
						//if( k < 3 )
						//	starty = SP.GameMenuButtonStartY + 20;
						//else 
						//	starty = SP.GameMenuButtonStartY + 25 + GetFileYsize(font);
						
						//startx = SP.GameMenuButtonStartX + 110 + (k%3)*GetFileXsize(font);
						
						// 동맹 중인 경우, 
						if(GD.OnlineAllyNumSwitch[UserPlayerNumber][i] == TRUE && GD.OnlineAllyNumSwitch[i][UserPlayerNumber] == TRUE)
						{
							// 온라인 전투 모드에서 설정 버튼. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 2, FALSE,	// 눌리기 전 상태
								font, 2, FALSE,	// 눌린 상태
								font, 2, FALSE,	// 마우스 갖다댄 상태
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// 내가 다른 사용자에게 동맹을 요청한 경우, 
						else if(GD.OnlineAllyNumSwitch[UserPlayerNumber][i] == TRUE)
						{
							// 온라인 전투 모드에서 설정 버튼. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 1, FALSE,	// 눌리기 전 상태
								font, 1, FALSE,	// 눌린 상태
								font, 1, FALSE,	// 마우스 갖다댄 상태
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// 다른 사용자가 내게 동맹을 요청한 경우, 
						else if(GD.OnlineAllyNumSwitch[i][UserPlayerNumber] == TRUE)
						{
							// 온라인 전투 모드에서 설정 버튼. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 1, FALSE,	// 눌리기 전 상태
								font, 1, FALSE,	// 눌린 상태
								font, 1, FALSE,	// 마우스 갖다댄 상태
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// 아무도 동맹을 요청하지 않은 경우, 
						else
						{
							// 온라인 전투 모드에서 설정 버튼. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 0, FALSE,	// 눌리기 전 상태
								font, 0, FALSE,	// 눌린 상태
								font, 0, FALSE,	// 마우스 갖다댄 상태
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}				
						k++;
					}
				}
			}
		}
	}
	
	
	// 선택된 캐릭터가 없을때 
	if(GD.SelectedNumber==0)
	{
	}
	// 선택된 캐릭터가 하나일때 
	else if(GD.SelectedNumber==1)
	{
	   // 선택된 하나의 아이디를 구한다. 
		id = 0 ;

       for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	   {
		   if( GD.CharInfo[GD.SelectedID[i].ID] )
		   {
			   if(id == 0)
			   {
		            id = GD.SelectedID[i].ID;
			   }
			   else
			   {
				   clGrp.Error("32472", "asdefrt:%d", GD.SelectedID[i].ID);
			   }
		   }
	   }

	   // 하나의 캐릭터가 있다고 해놓고 실제로는 없다면, 
/*	   if(id == 0)
	   {
		   clGrp.Error("1239074", "fsdf3283");
		   return ;
	   }*/

	   // 선택된 캐릭터가 하나일때에는 0번에 넣는다. 
       for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	   {
		   GD.SelectedID[i].Init();
	   }

	   // 선택된 하나의 아이디를 0번에 넣는다.
	   GD.SelectedID[0] = clifChar.GetID(id);


	   // 아군 캐릭터가 아니면 명령을 넣지 않는다. 
	   // 아군 캐릭터라도 자동모드이면 명령을 넣을 수 없다. 
	   if( clifChar.GetPlayerNumber(id) == UserPlayerNumber && clifChar.GetAutoSwitch(id) == FALSE )
	   {


		   // 자신이 완전히 만들어진 상태에서만 가능하다. 
		   MenuMode|=MENUMODE_MOVE;
		   
		   if(clifChar.IsAtb(id, ATB_ATTACK))   MenuMode|=MENUMODE_ATTACK;
		   if(clifChar.IsAtb(id, ATB_ATTACKAIR))MenuMode|=MENUMODE_ATTACKAIR;
		   if(clifChar.IsAtb2(id, ATB2_HEAL))   MenuMode|=MENUMODE_HEAL;
		   if(clifChar.IsAtb2(id, ATB2_MANA))   MenuMode|=MENUMODE_MANA;
//		   if(clifChar.GetKind(id) == KIND_FLAGMAN)    MenuMode|=MENUMODE_GOINTOHQ;

		   // 캐릭터가 가지고 있는 명령을 버퍼에 넣는다. 
	       for(i = 0;i < MAX_MENU_NUMBER;i++)
		   {
			   if( clifChar.GetMenuItem(id, i) == 0 )	continue;

			   SHORT order         = Item[clifChar.GetMenuItem(id, i)].GetOrder();
			   SHORT reservenumber = 0;
			   
		       PushGMenu(i, clifChar.GetMenuItem(id, i), reservenumber, clifChar.GetMenuMode(id, i), 100);
		   }
		   
	   }
	}
	// 여러 캐릭터가 선택되어 있을때 
	else if(GD.SelectedNumber > 1)
	{

		SHORT index = 0;

		for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
		{
			if(GD.SelectedID[i].IsEmpty() == FALSE)
			{
				id = GD.SelectedID[i].ID;
				
				if(GD.CharInfo[id])
				{
				    for(j = 0;j < clifChar.GetMenuNumber(id);j++)
					{
					   // 멀티에서 사용되는 버튼이 아니면 넣지 않는다. 
			  	       if(Item[clifChar.GetMenuItem(id, j)].GetMultiSwitch() == FALSE)
					   {}
					   else
					   {
						   // 자신이 완전히 만들어진 상태에서만 가능하다. 
						   MenuMode|=MENUMODE_MOVE;
						   
						   if(clifChar.IsAtb(id, ATB_ATTACK))   MenuMode|=MENUMODE_ATTACK;
						   if(clifChar.IsAtb(id, ATB_ATTACKAIR))MenuMode|=MENUMODE_ATTACKAIR;
						   if(clifChar.IsAtb2(id, ATB2_HEAL))   MenuMode|=MENUMODE_HEAL;
						   if(clifChar.IsAtb2(id, ATB2_MANA))   MenuMode|=MENUMODE_MANA;
//						   if(clifChar.GetKind(id) == KIND_FLAGMAN)    MenuMode|=MENUMODE_GOINTOHQ;
						   
						   // 메뉴를 정상적으로 넣으면 index를 추가해준다. 
						   if( index < MAX_MENU_NUMBER 
							   && PushGMenu(index, clifChar.GetMenuItem(id, j), 0, clifChar.GetMenuMode(id, j), 100) == TRUE)
						   {
							   index++;
						   }
							   
						}
					}
				}
				else
				{
					clGrp.Error("fds93jdjhal"," school 101"); 
				}
			}
		}
	}


	// 마우스나 키보드 입력을 통해 어떤 메뉴가 선택되었는지 확인한다. 
	for(i = 0;i < MAX_MENU_NUMBER;i++)
	{

		// 유효한 메뉴만 관련된다. 
		if(GD.GetMenu(i, 43)==0)continue;


		// 키보드 처리를 먼저한다. (입력 모드가 아니어야 한다. )
		// ===============================================================================
//		if(pGame->pBattle->m_EditControl.IsFocus() == FALSE
		// ===============================================================================
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
		if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) 
        && (   (Item[GD.GetMenu(i, 2234)].GetHotKeyCode(0) && GameWParam == Item[GD.GetMenu(i, 2234)].GetHotKeyCode(0))
		    || (Item[GD.GetMenu(i, 2235)].GetHotKeyCode(1) && GameWParam == Item[GD.GetMenu(i, 2235)].GetHotKeyCode(1))  ) )
		{

			 // 마우스가 위치한 메뉴의 번호 
		     MenuOfMouse = i;
			 // 마우스로 눌린 메뉴의 번호 
			 SelectedMenu = i;


			 SHORT ableswitch = TRUE;

			 // 건물 생산 명령이라면, 
			 SHORT order = Item[GD.GetMenu(i, 234)].GetOrder();

			 if(ableswitch==TRUE)
			 {

				 // 작동가능한 모드여야 한다. 
	    	     if(GD.MenuMode[i] == FALSE)continue;
			
				 PushEffect(EFFECT_CLICK, 0, 0);
			 
				 GD.SelectedOrder        = Item[GD.GetMenu(i, 234)].GetOrder();
			     GD.SelectedOrderOption |= SELECTEDORDEROPTION_KEYPRESS;
		         GD.ObjectNeedType       = Order[Item[GD.GetMenu(i, 549)].GetOrder()].Object;

		         switch(GD.ObjectNeedType)
				 {
		             case NOOBJECT:
		                 GD.ObjectNeedSwitch = FALSE;
		                 break;
		             default:
		                 GD.ObjectNeedSwitch = TRUE;
					     break;
				 }
			}
		}
		// 마우스가 메뉴 위에 있고 
		else if(IsMouseInMenu(i))
		{
			// 마우스가 위치한 메뉴의 번호 
		    MenuOfMouse = i;

      		// 이미 선택된 메뉴라면, 
			if(SelectedMenu == i)
			{
				// 최종 선택 
				if(LeftPressSelectedSwitch == TRUE)
				{

				   if(GameLeftPressSwitch == FALSE)
				   {

					  // 선택된 상태를 초기화한다. 
				      SelectedMenu       = -1;
				      MenuSelectedSwitch = FALSE;


					   // control 키가 눌리면, 
					   if(ControlKeyPressSwitch == TRUE)
					   {

						   // 선택된 캐릭터가 1마리이고 ,
		                   // 메뉴 버튼위에서 눌리면 
		                   // 병사를 생산하는 명령만 받는다. 
		                   if(GD.SelectedNumber == 1 
		                   && Order[Item[GD.Menu[i]].GetOrder()].Atb == ORDERATB_TRAIN)
						   { 

			                  _NewID new_id;
				              new_id.Init() ;

			                  for(j = 0;j < MAX_SELECTED_CHAR_NUMBER;j++)
							  {
		                          if(GD.SelectedID[j].IsEmpty() == FALSE)
								  {
			                          new_id = GD.SelectedID[j];
				                      break;
								  }
							  }
		                  
						   }
					   }
					   else
					   {
					      SHORT ableswitch=TRUE;

					      // 건물 생산 명령이라면, 
					      SHORT order = Item[GD.GetMenu(i, 234)].GetOrder();

						  if(ableswitch == TRUE)
						  {

							  // 작동가능한 모드여야 한다. 
    		                  if(GD.MenuMode[i]==FALSE)continue;
					   
						      GD.SelectedOrder        = order;
					          GD.SelectedOrderOption |= SELECTEDORDEROPTION_LEFTPRESS;
		                      GD.ObjectNeedType       = Order[order].Object;
	
							  switch(GD.ObjectNeedType)
							  {  
		                         case NOOBJECT:
		                             GD.ObjectNeedSwitch = FALSE;
		                             break;
		                         default:
		                             GD.ObjectNeedSwitch = TRUE;
		                             break;
							  }
						  } 

					   }
				   }
				}
				else if(RightPressSelectedSwitch==TRUE)
				{
				   if(GameRightPressSwitch==FALSE)
				   {
					   SelectedMenu=-1;
					   MenuSelectedSwitch=FALSE;

					   // 이미 예약된 명령이면 예약을 취소하게 만든다. 

					   // 대상물을 필요로 하지 않는 경우에만 오른쪽 마우스 버튼을 입력 받는다.
					   if(Order[Item[GD.GetMenu(i, 549)].GetOrder()].Object==NOOBJECT)
					   {
					      GD.SelectedOrder=Item[GD.GetMenu(i, 234)].GetOrder();
					      GD.SelectedOrderOption|=SELECTEDORDEROPTION_RIGHTPRESS;
		                  GD.ObjectNeedType=Order[Item[GD.GetMenu(i, 549)].GetOrder()].Object;

		                  switch(GD.ObjectNeedType)
						  {  
		                     case NOOBJECT:
		                         GD.ObjectNeedSwitch=FALSE;
		                         break;
		                     default:
		                         GD.ObjectNeedSwitch=TRUE;
		                         break;
						  }
					   }


				   }

				}
			}
			// 선택되지 않은 다른 메뉴위라면, 
			else
			{
			   // 왼쪽 버튼이 눌린 것이라면, 
			   if(GameLeftPressSwitch==TRUE)
			   {

				   PushEffect(EFFECT_CLICK, 0, 0);
				   LeftPressSelectedSwitch=TRUE;
				   RightPressSelectedSwitch=FALSE;
			 	   SelectedMenu=i;
				   MenuSelectedSwitch=TRUE;
			   }
			   else if(GameRightPressSwitch==TRUE)
			   {
				   PushEffect(EFFECT_CLICK, 0, 0);
				   LeftPressSelectedSwitch=FALSE;
				   RightPressSelectedSwitch=TRUE;
				   SelectedMenu=i;
				   MenuSelectedSwitch=TRUE;
			   }

			}
		}
	}


	IsMosueInControlSwitch = FALSE;

	if(pGame->pBattle->OperateGameButton() == TRUE)
	{
		// 마우스가 다른 콘트롤 상에 존재한다. 
		IsMosueInControlSwitch = TRUE;
	}



	// 마우스가 메뉴 위에 없으면 
	 if(MenuOfMouse==-1)
	 {
		 SelectedMenu=-1;
		 MenuSelectedSwitch=FALSE;
		 LeftPressSelectedSwitch=FALSE;
		 RightPressSelectedSwitch=FALSE;

	 }
	 // 메뉴 위에 있지만 선택된 메뉴가 아니라면, 
	 else if(SelectedMenu!=MenuOfMouse)
	 {
		 SelectedMenu=-1;
		 MenuSelectedSwitch=FALSE;
		 LeftPressSelectedSwitch=FALSE;
		 RightPressSelectedSwitch=FALSE;
	 }


	 // 마우스가 메뉴 위에 없으면 
	 if(CrewMenuOfMouse==-1)
	 {
		 SelectedCrewMenu=-1;
		 CrewMenuSelectedSwitch=FALSE;
		 CrewMenuLeftPressSelectedSwitch=FALSE;

	 }
	 // 메뉴 위에 있지만 선택된 메뉴가 아니라면, 
	 else if(SelectedCrewMenu!=CrewMenuOfMouse)
	 {
		 SelectedCrewMenu=-1;
		 CrewMenuSelectedSwitch=FALSE;
		 CrewMenuLeftPressSelectedSwitch=FALSE;
	 }


	 // 메뉴 위에 마우스가 위치해 있다면 도움말을 표시한다.
	 if(MenuOfMouse>=0 && GD.GetMenu(MenuOfMouse, 345))
	 {

		 // 마우스가 위치한 메뉴의 위치가 변한것이면, 
		 if(GD.HelpMenuOfMouse!=MenuOfMouse)
		 {
		    GD.HelpMenuOfMouse =MenuOfMouse;
		    GD.HelpMenuClock   =CurrentClock;
		 }
		 // 위치한지 600clock이 지난것이면, 
		 else if(abs(CurrentClock-GD.HelpMenuClock)>600)
		 {

			 SHORT item=GD.GetMenu(MenuOfMouse, 130);
		     SHORT money=0, tree=0, mancost=0, magic=0;
			 SI32 needgoods = 0;
			 SI32 needkind = 0;
			 SI32 buyunit  = 0;
			 char* pDetailHelpString = Item[GD.GetMenu(MenuOfMouse, 132)].GetText2();

			
			 SI32 order = Item[item].GetOrder();

			 needkind = Order[order].siNeedKind;

			 if(needkind)
			 {
/*				 if(GD.Player[UserPlayerNumber].KindSwitch[needkind]==FALSE)
				 {
					 // 건물이면, 
					 if(KI[needkind].Atb & ATB_BUILDING)
						wsprintf(textbuffer, "%s:[%s]", Text.NeedBuildingMessage.Get(), KI[needkind].Name);
					 // 일반 유닛이면, 
					 else
						wsprintf(textbuffer, "[%s] %s", KI[needkind].Name, Text.Need.Get());
					 text2=textbuffer;
				 }
				 */
			 }
			 // 마법 속성이면, 
			 else if(Order[Item[item].GetOrder()].IsAtb(ORDERATB_MAGIC) )
			 {
				 // 필요한 마법력 
				 magic=Order[Item[item].GetOrder()].Para1;
				 
				 if(magic > CR[GD.SelectedID[0].ID].m_ParmeterBox.IP.GetMana())
				 {
					 wsprintf(textbuffer, "%s", Text.InsufficientMana.Get());
					 text2=textbuffer;
				 }

			 }
			 // 여여문의 지뢰라면, 
			 else if(Item[item].GetOrder() ==ORDER_MINE)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.MineStockMessage.Get(), 0);
				 text2=textbuffer;
			 }
			 // 유성룡의 공중공격이면, 
			 else if(Item[item].GetOrder()==ORDER_AIRMAGIC)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.AirMagicStockMessage.Get(), 0);
				 text2=textbuffer;
			 }

			 // 농부를 황소로 바꾸는 것이라면, 
			 else if(Item[item].GetOrder() ==ORDER_FARMERTOBULL)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.BullStockMessage.Get(), 0);
				 text2=textbuffer;
			 }

			 // 마법력 보조이면, 
			 else if(Item[item].GetOrder() ==ORDER_USEBUDA)
			 {
				 kind=clifChar.GetKind(GD.SelectedID[0].ID);

/*				 if(GD.Player[UserPlayerNumber].GeneralLevel[kind]<NEEDLEVEL_USEBUDA)
				 {
				    wsprintf(textbuffer, "%s[%d]%s", Text.Level.Get(), NEEDLEVEL_USEBUDA+1, Text.MoreNeed.Get());
   				    text2=textbuffer;
				 }
				 */
			 }

			 		 
		     Help.SetHelp(IpD.Mouse_X, IpD.Mouse_Y, 
				          Item[GD.GetMenu(MenuOfMouse, 130)].GetText(),
						  pDetailHelpString,
						  money, 
						  tree, 
						  mancost,
						  magic, 
						  Item[GD.GetMenu(MenuOfMouse, 130)].GetHotKeyCode(0),
						  text2);
		 }


	 }
	 else
	 {
	      GD.HelpMenuOfMouse=-1;
	 }

#ifdef _DEBUG
	// robypark 2004/10/14 15:51
	// 선택된 캐릭터가 여러개 일 경우 보여지는 캐릭터들의 처리.
	if (GD.SelectedNumber > 1)
	{
		RECT rtSelectCharacter;								// 선택 캐릭터 박스
		POINT ptMousePos = {IpD.Mouse_X, IpD.Mouse_Y};		// 마우스 좌표
		SI32 siSelectedID = -1;								// 선택된 캐릭터 아이디
		SI32 siIndex = 0;									// 인덱스

		for (SI32 siI = 0; siI < MULTI_SELECT_GROUP_ROW; siI++)
		{
			for (SI32 siJ = 0; siJ < MULTI_SELECT_GROUP_COLUMN; siJ++)
			{
				rtSelectCharacter.left = MULTI_SELECT_START_POSITION_X + ((siJ + 1) * MULTI_SELECT_GAP_WIDTH_SIZE) + (siJ * MULTI_SELECT_PORTRAIT_WIDTH);
				rtSelectCharacter.top = MULTI_SELECT_START_POSITION_Y + (siI * MULTI_SELECT_PORTRAIT_HEIGHT) + (siI * MULTI_SELECT_GAP_HEIGHT_SIZE);
				rtSelectCharacter.right = rtSelectCharacter.left + MULTI_SELECT_PORTRAIT_WIDTH;
				rtSelectCharacter.bottom = rtSelectCharacter.top + MULTI_SELECT_PORTRAIT_HEIGHT;

				// 마우스가 가리키고 있는 위치 좌표 얻기
				if (PtInRect(&rtSelectCharacter, ptMousePos) == TRUE)
				{
					// 마우스 아래에 있는 캐릭터 찾기
					SI32 siTempIndex = 0;
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						SI32 siTempID = GD.SelectedID[i].ID;
						if(siTempID)
						{
							if (siTempIndex == siIndex)
							{
								siSelectedID = siTempID;			// 마우스 아래에 있는 캐릭터 인덱스
								goto FIND_CHARACTER_INDEX_UNDER_CURSOR;	// 해당 처리루틴으로 이동
							}

							siTempIndex++;
						}
					}
					return;
				}

				siIndex++;
			}
		}

		return;
// 마우스 커서 밑에 유효한 캐릭터가 있을 경우 처리
FIND_CHARACTER_INDEX_UNDER_CURSOR:

		// 마우스 왼쪽버튼 클릭이 안되었다면.
		if (FALSE == GameLeftPressSwitch)
		{
			// 마우스 커서 아래 캐릭터 이름 출력
			// 체력, 마법력 수치 표시
			char szBufferHP[64], szBufferMP[64];
			wsprintf(szBufferHP,"HP: %4d/%4d", CR[siSelectedID].m_siGuardianLife + CR[siSelectedID].m_ParmeterBox.IP.GetLife(), CR[siSelectedID].m_siGuardianMaxLife + CR[siSelectedID].m_ParmeterBox.GetMaxLife());
			wsprintf(szBufferMP,"MP: %4d/%4d", CR[siSelectedID].m_ParmeterBox.IP.GetMana(), CR[siSelectedID].m_ParmeterBox.GetMaxMana());
			Help.SetHelp(IpD.Mouse_X, IpD.Mouse_Y,			// SHORT drawx, SHORT drawy
				          CR[siSelectedID].GetName(),		// CHAR* text
						  NULL,								// CHAR* extendtext
						  0,								// SHORT money
						  0,								// SHORT tree
						  0,								// SHORT maintenance
						  0,								// SHORT magicpower
						  0,								// CHAR hotKey
						  szBufferHP,						// CHAR* lpInfo1
						  szBufferMP,						// CHAR* lpInfo2
						  0);								// CHAR* lpInfo3
			return;
		}

		GameLeftPressSwitch = FALSE;		// 마우스 왼쪽버튼 클릭초기화
		DragSwitch = FALSE;					// 마우스 드래그 스위치 초기화
		MapDragSwitch = FALSE;				// 맵 드래그 스위치 초기화

		// 캐릭터 초상을 클릭하였을 경우
		if ((FALSE == ControlKeyPressSwitch) && (FALSE == ShiftKeyPressSwitch))
		{
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				id=GD.SelectedID[i].ID;
				if(id)
				{
					// 해당 캐릭터 이외의 캐릭터를 그룹에서 제거한다.
					if (id != siSelectedID)
					{
						CR[id].SetSelect(FALSE, FALSE);
					}
				}
			}
			// 선택한 캐릭터만 선택되도록 설정.
			CR[siSelectedID].SetSelect(TRUE, TRUE);
			return;
		}

		// 마우스 왼쪽 버튼과 컨트롤 키가 눌렸을 경우
		// 같은 종류의 캐릭터들로만 이루어진 캐릭터 그룹을 만든다.
		// 고급 유닛과 일반 유닛은 다른 종류의 캐릭터러 판단한다.
		// (예: 파계승과 고급파계승은 다른 종류의 캐릭터로 판단한다.)
		if (TRUE == ControlKeyPressSwitch)
		{
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				id=GD.SelectedID[i].ID;
				if(id)
				{
					// 같은 종류의 캐릭터인지 검사.
					if (CR[id].m_cltCharStatus.GetKind() != CR[siSelectedID].m_cltCharStatus.GetKind())
					{
						// 다른 종류의 캐릭터이므로 그룹에서 제거
						CR[id].SetSelect(FALSE, FALSE);
					}
					else
						// 같은 종류의 캐릭터이므로 캐릭터 선택
						CR[id].SetSelect(TRUE, FALSE);
				}
			}
			ControlKeyPressSwitch = FALSE;		// 컨트롤키 상태 초기화

			return;
		}

		// 쉬프트키가 눌려지고 마우스 왼쪽 버튼이 클릭하였을경우 처리
		// 현재 그룹에서 해당 캐릭터를 제거한다.
		if (TRUE == ShiftKeyPressSwitch)
		{
			// 캐릭터 그룹에서 제거
			CR[siSelectedID].SetSelect(FALSE, FALSE);
			ShiftKeyPressSwitch = FALSE;			// 쉬프트키 상태 초기화
		}
	}
#endif // _DEBUG
}

// 메뉴를 그린다. 
void DrawMenu(LPDIRECTDRAWSURFACE7 surface)
{
	int		i, j, k;
	SHORT	startx, starty;
	int		id=0;
	SHORT	lightswitch=FALSE;
	SI32	buttonFile;
	SI32    siAllyWindowFile;
	SI32	siAllyTitleFile;
	HDC		hdc;

	// 클리핑 영역을 기억한다. 
	SI32 clipx1, clipx2, clipy1, clipy2;
	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);
	
	buttonFile = FILE_ON_ALLY_PANNEL;
	siAllyTitleFile = FILE_ON_ALLY_TITLE;
	siAllyWindowFile = FILE_ON_ALLY_WINDOW;

	if(clGrp.LockSurface(surface) == TRUE)
	{
		if(bGameAllyWindowSwitch == FALSE)
		{
			Draw_PutSpriteLightT(SP.GameAllyTitleX,SP.GameAllyTitleY,siAllyTitleFile,0);
			Draw_PutSpriteLightT(SP.GameAllyWindowX,SP.GameAllyWindowY,siAllyWindowFile,0);
		}
		else if(bGameAllyWindowSwitch == TRUE)
		{
			Draw_PutSpriteLightT(680,482,siAllyTitleFile,0);
		}

		clGrp.UnlockSurface(surface);
	}

	if(surface->GetDC(&hdc) == DD_OK)
	{
		SelectObject(hdc, pGame->pBattle->VersionInfoFont);
						
		SetTextColor( hdc, RGB( 250, 250, 250 ) );
		SetBkMode(hdc, TRANSPARENT);

		if(bGameAllyWindowSwitch == FALSE)
		{
			TextOut(hdc,SP.GameAllyTitleX + 10,SP.GameAllyTitleY + 2,Text.AllySelect.Get(),strlen(Text.AllySelect.Get()));
		}
		else if(bGameAllyWindowSwitch == TRUE)
		{
			TextOut(hdc,680 + 10,482 + 2,Text.AllySelect.Get(),strlen(Text.AllySelect.Get()));
		}

		surface->ReleaseDC(hdc);
	}

	
	k = 0;
	for( i=0; i<MAX_MULTI_PLAYER-1; i++ )
	{
		// 유효한 플레이이어인 경우 , 
		if( MyNet.PlayersdpId.dpId[i] ) 
		{

			if( ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE && i != UserPlayerNumber )
			{
				if(bGameAllyWindowSwitch == FALSE)
				{
					//if( k < 3 )
					//	starty = SP.GameMenuButtonStartY + 20;
					//else 
					//	starty = SP.GameMenuButtonStartY + 25 + GetFileYsize(buttonFile);
					
					//startx = SP.GameMenuButtonStartX + 33 + (k%3)*GetFileXsize(buttonFile);

					if(clGrp.LockSurface(surface)==TRUE)
					{

						//Draw_PutSpriteT(startx, starty, buttonFile, 0);
						clGrp.UnlockSurface(surface);
					}
					if(surface->GetDC(&hdc) == DD_OK)
					{
						SelectObject(hdc, pGame->pBattle->VersionInfoFont);
						
						SetTextColor( hdc, RGB( 250, 250, 250 ) );
						SetBkMode(hdc, TRANSPARENT);
						TextOut(hdc, SP.GameAllyWindowX + 30, SP.GameAllyWindowY + 5 + (15 * k)  , MyNet.PlayersName.Name[i], strlen(MyNet.PlayersName.Name[i]));
						surface->ReleaseDC(hdc);
					}
					k++;
				}
			}
		}
	}
	
#ifdef _DEBUG
	XSPR *pXsprChatBoard = pGame->pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHATTING_BACK);
	if( pXsprChatBoard )
	{
		SI32 siX, siY, siXsize, siYsize;
		siX = MULTI_SELECT_BKGND_IMAGE_POSITION_X;
		siY = MULTI_SELECT_BKGND_IMAGE_POSITION_Y;
		siXsize = MULTI_SELECT_BKGND_IMAGE_WIDTH;
		siYsize = MULTI_SELECT_BKGND_IMAGE_HEIGHT;

		if(siX < 0)		siX = 0;
		if(siY < 0)		siY = 0;

		if((siX + siXsize) > ON_SCREEN_XSIZE)		siXsize = ON_SCREEN_XSIZE - siX;
		if((siY + siYsize) > ON_SCREEN_YSIZE)		siYsize = ON_SCREEN_YSIZE - siY;

		clGrp.PutSpriteJin(siX, siY, siXsize, siYsize, pXsprChatBoard->Image);
	}

#endif // _DEBUG

	// 선택된 캐릭터가 한개일때 초상화를 그린다.
	if(GD.SelectedNumber==1)
	{
		for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
		{
			id=GD.SelectedID[i].ID;
			if(id)
			{
				// 캐릭터 정보를 보여준다. 
                clifChar.ShowCharInfo(id, surface);
				break;
			}
		}
	}
#ifdef _DEBUG
	// robypark 2004/10/6 15:26
	// 선택된 캐릭터가 여러개 일 경우 캐릭터들의 정보들을 보여준다.
	// 일반 워게임에서 보여지는 것처럼.
	else if (GD.SelectedNumber > 1)
	{
		POINT ptMassSelected[MULTI_SELECT_MAX_COUNT];		// 그룹으로 된 캐릭터들의 초상화가 그려지는 위치 좌표
		SI32 siIndex = 0;				// 배열 인덱스
		for (SI32 siI = 0; siI < MULTI_SELECT_GROUP_ROW; siI++)
		{
			for (int siJ = 0; siJ < MULTI_SELECT_GROUP_COLUMN; siJ++)
			{
				// 그룹의 캐릭터들의 초상화가 그려질 위치 값 계산
				ptMassSelected[siIndex].x = MULTI_SELECT_START_POSITION_X + ((siJ + 1) * MULTI_SELECT_GAP_WIDTH_SIZE) + (siJ * MULTI_SELECT_PORTRAIT_WIDTH);
				ptMassSelected[siIndex].y = MULTI_SELECT_START_POSITION_Y + (siI * MULTI_SELECT_PORTRAIT_HEIGHT) + (siI * MULTI_SELECT_GAP_HEIGHT_SIZE);
				siIndex++;	// 배열 인덱스 증가
			}
		}

		siIndex = 0;

		// 그룹 내 캐릭터들의 초상화 그리기
		for (i = 0; i < MAX_SELECTED_CHAR_NUMBER; i++)
		{
			id = GD.SelectedID[i].ID;

			if (id)
			{
				// 그룹 지정된 캐릭터들을 화면 하단에 그리기
				// 캐릭터 정보를 보여준다. 
                clifChar.ShowCharInfoSimple(id, surface, ptMassSelected[siIndex].x, ptMassSelected[siIndex].y, MULTI_SELECT_PORTRAIT_WIDTH, MULTI_SELECT_PORTRAIT_HEIGHT);
				siIndex++;
			}
		}
	}
#endif
	
	buttonFile = FILE_BUTTON_PLUS;

	// 아이콘을 그린다.
	// 실제로 메뉴를 그린다. 
	
	if(clGrp.LockSurface(surface)==TRUE)
	{
		
		for(i=0;i<MAX_MENU_NUMBER;i++)
		{
			if(GD.GetMenu(i, 120))
			{
				int font;
				
				font=Item[GD.GetMenu(i, 130)].GetFont();
				
				if(MenuOfMouse==i)
					lightswitch=TRUE;
				else
					lightswitch=FALSE;
				
				startx=SP.MenuStartX+(i%SP.MaxMenuXCount)*(SP.MenuButtonXsize+SP.MenuMarginX);
				starty=SP.MenuStartY+(i/SP.MaxMenuXCount)*(SP.MenuButtonYsize+SP.MenuMarginY);
				
				if(GD.MenuMode[i]==TRUE)
				{
					if(lightswitch==TRUE)
					{
						Draw_PutSpriteT(startx, starty, FILE_ON_BUTTON, 1);
					}
					
					Draw_PutSpriteT(startx, starty, buttonFile, font);
				}
				else
				{
					Draw_PutSpriteT(startx, starty, buttonFile, font, GrayTable);
					
				}

				// 버튼이 선택되었을때 
				if(SelectedMenu==i && MenuSelectedSwitch==TRUE)
				{
					Draw_PutSpriteT(startx, starty, FILE_ON_BUTTON, 2);
				}


				
				
				// 단축키를 표시한다. 
				if(Item[GD.GetMenu(i, 134)].GetHotKeyCode(0))
				{
					font=Item[GD.GetMenu(i, 134)].GetHotKeyCode(0) -'A';
					
					Draw_PutSpriteT(startx+3, starty+3, FILE_SHORTCUTKEY, font);
				}
				
				
				// 예약된 생산 명령의 갯수를 보여준다. 
				for(j=0;j<GD.MenuReserve[i];j++)
				{
					clGrp.FillBox((startx+4)+j*5,   (starty+SP.MenuButtonYsize-12), 
						(startx+4)+j*5+3, (starty+SP.MenuButtonYsize-12)+5,  (UI08)COLOR_GREEN);
				}
				
			}
			
		}
		clGrp.UnlockSurface(surface);
	}


	// 게임중에 나오는 버튼들을 모아서 그려준다. 
	pGame->pBattle->DrawGameButton(surface);

	// 클리핑 영역을 재 설정한다. 
	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
	
}

// 현재 메뉴를 지운다. 
void ClearMenu()
{
	int i;

	for(i=0;i<MAX_MENU_NUMBER;i++)
		GD.SetMenu(i, 0, 0, FALSE, 0);
}


