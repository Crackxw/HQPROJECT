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
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	_ScreenPoint 		SP;									// ȭ�鿡 ���� ���� ���� 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	BOOL				bGameAllyWindowSwitch;
extern	SHORT 				MenuOfMouse;						// ���콺 Ŀ���� ��ġ�ϰ� �ִ� �޴��� ��ȣ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	cltMapInterface		clifMap;							// �� ���� �������̽� 
extern	int 				MenuMode;							// �޴��� ���¸� ���� 
extern	_Order 				Order[];
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Char				CR[];								// ĳ������ ������ 
extern  _Chat 				Chat;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;				// ���콺 ���� ��ư
extern	int 				ControlKeyPressSwitch;				// ��Ʈ��Ű ����
extern	int 				ShiftKeyPressSwitch;				// ����ƮŰ ����
extern	SHORT 				CrewMenuOfMouse;
extern	_Help 				Help;
extern _InputDevice			IpD;
extern	BOOL 				IsMosueInControlSwitch;				// ���콺�� ���� ��Ʈ������ ��ġ�ϰ� �ִ�.
extern	clock_t				CurrentClock;						// �ð� ���� 
extern	_NewID 				ZeroID;
extern	int 				GeneralList[];						// ������ ����� ����Ʈ 
extern SHORT				GameRightPressSwitch;

extern	unsigned char		TeamLightTable[];
extern	unsigned char		GrayTable[];

static	BOOL				bOnlineAllySwitch = FALSE;
extern	_MyNetGame			MyNet;

// robypark 2004/10/13 11:43
// ����� ��忡���� ���� �� ���� ĳ���� ���ý� ĳ���͵��� �ʻ�ȭ, ü��, ���� ���� ä�� ����Ʈ �ڽ��� ��������.
#ifdef _DEBUG
#define MULTI_SELECT_PORTRAIT_WIDTH		50		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� ���� ũ��
#define MULTI_SELECT_PORTRAIT_HEIGHT	32		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� ���� ũ��
#define MULTI_SELECT_START_POSITION_X	218		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� �߿� ù��° ĳ���� �ʻ� �̹��� ��ġ(X)
#define MULTI_SELECT_START_POSITION_Y	508		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� �߿� ù��° ĳ���� �ʻ� �̹��� ��ġ(Y)
#define MULTI_SELECT_GAP_WIDTH_SIZE		2		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� ���� ����� �Ÿ�(����)
#define MULTI_SELECT_GAP_HEIGHT_SIZE	2		// ���� ĳ���� ���ý� �������� �ʻ� �̹����� ���� ����� �Ÿ�(����)
#define MULTI_SELECT_GROUP_ROW			2		// �׷� ���� �׷��� �� �������� �ʻ�ȭ ���� ����
#define MULTI_SELECT_GROUP_COLUMN		7		// �׷� ���� �׷��� �� �������� �ʻ�ȭ ���� ����
#define MULTI_SELECT_MAX_COUNT			(MULTI_SELECT_GROUP_ROW * MULTI_SELECT_GROUP_COLUMN)		// �� �׷����� ���ð����� ĳ���� ����

#define MULTI_SELECT_BKGND_IMAGE_POSITION_X			201			// �׷� ���� ��� �̹��� ��ġ(X)
#define MULTI_SELECT_BKGND_IMAGE_POSITION_Y			502			// �׷� ���� ��� �̹��� ��ġ(Y)
#define MULTI_SELECT_BKGND_IMAGE_WIDTH				398			// �׷� ���� ��� �̹��� ��ġ(WIDTH)
#define MULTI_SELECT_BKGND_IMAGE_HEIGHT				72			// �׷� ���� ��� �̹��� ��ġ(HEIGHT)
#endif // _DEBUG

// ���콺�� Ű���忡 ���ؼ� ���õ� �޴��� �ε��� �� 
SHORT	SelectedMenu = -1;
// ���õ� �޴��� �ִ��� ���θ� �˷��ִ� ���� 
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

// ��� �޴� �������� �ʱ�ȭ �Ѵ�. 
void ItemInit()
{

	// �Ϲ� ������ 
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
		FALSE,'B', 'B',"������",NULL);

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:17) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	// ����������(����-�ź���, �̼��� 2�� ���� ���)
	Item[ITEM_MAGIC_FIRE_ROUND_DANCE].SetItem(ORDER_FIRE_ROUND_DANCE, 19,
								FALSE,'F', 'f',Text.Skill_FIRE_ROUND_DANCE.Get(), NULL);
	
	// ����(����-������, ���� 2�� ���� ���)
	Item[ITEM_MAGIC_NOVA].SetItem(ORDER_NOVA, 10,
								FALSE,'N', 'n',Text.Skill_NOVA.Get(),NULL);
	
	// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	Item[ITEM_MAGIC_FURY_OF_VOLCANO].SetItem(ORDER_FURY_OF_VOLCANO, 12,
								FALSE,'V', 'v',Text.Skill_FURY_OF_VOLCANO.Get(), NULL);
	
	// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	Item[ITEM_MAGIC_POISONING_HEAVENS].SetItem(ORDER_POISONING_HEAVENS, 18,
								FALSE,'P', 'p',Text.Skill_POISONING_HEAVENS.Get(), NULL);
	
	// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
	Item[ITEM_MAGIC_BOMB_OF_HEAVEN].SetItem(ORDER_BOMB_OF_HEAVEN, 11,
								FALSE,'B', 'b',Text.Skill_BOMB_OF_HEAVEN.Get(), NULL);
	
	// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	Item[ITEM_MAGIC_SIEGE_MODE].SetItem(ORDER_SIEGE_MODE, 41,
								FALSE,'I', 'i',Text.Skill_SIEGE_MODE.Get(), NULL);
	
	// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	Item[ITEM_MAGIC_ONSLAUGHT_FIREBALL].SetItem(ORDER_ONSLAUGHT_FIREBALL, 22, 
								FALSE,'B', 'b',Text.Skill_ONSLAUGHT_FIREBALL.Get(), NULL);

	// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
	Item[ITEM_MAGIC_FIRERAIN_OF_GOD].SetItem(ORDER_FIRERAIN_OF_GOD, 20, 
								FALSE,'R', 'r',Text.Skill_FIRERAIN_OF_GOD.Get(), NULL);

	// robypark 2004/6/4 14:5
	// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	Item[ITEM_MAGIC_SIEGE_MODE_UNDO].SetItem(ORDER_SIEGE_MODE_UNDO, 41,
								FALSE,'I', 'i',Text.Skill_SIEGE_MODE_UNDO.Get(), NULL);
}


BOOL IsMouseInMenu(SHORT number)
{
	if(MapDragSwitch==TRUE)return FALSE;

	// �������� �ʴ� �޴��� Ȯ���� �� ����. 
	if(GD.GetMenu(number, 138)==0)return FALSE;

    int order = Item[GD.GetMenu(number, 10)].GetOrder();

	if(GameMouseX>SP.MenuStartX+(number%SP.MaxMenuXCount)*(SP.MenuButtonXsize+SP.MenuMarginX)
	&& GameMouseX<SP.MenuStartX+(number%SP.MaxMenuXCount)*(SP.MenuButtonXsize+SP.MenuMarginX)+SP.MenuButtonXsize
	&& GameMouseY>SP.MenuStartY+(number/SP.MaxMenuXCount)*(SP.MenuButtonYsize+SP.MenuMarginY)
	&& GameMouseY<SP.MenuStartY+(number/SP.MaxMenuXCount)*(SP.MenuButtonYsize+SP.MenuMarginY)+SP.MenuButtonYsize)
	   return TRUE;

	return FALSE;
	
}



// ���� ������ �޴��� �ִ´�. 
BOOL PushGMenu(SHORT index, SHORT menu, SHORT reservenumber, SHORT mode, SHORT menupercent)
{
	int i;
	int alreadyswitch=FALSE;

	// �̹� �ִ� ����̸� 
	for(i=0;i<MAX_MENU_NUMBER;i++)
	{
		if(GD.GetMenu(i, 20)==menu)
		{
			// ������ FALSE, ���γ��� ���� TRUE�̸�, 
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
	// �����忡���� ������ �ΰų� �����ϴ� ���� �����ϴ�. 
	if(pGame->pOnlineWorld->CheckPKFieldMap() == TRUE)
	{
		// ���Ϳ���(0: ���� ����, 1: ���� ��û��û, 2: ���� ��û����, 3: ���� ����)
		MakeOrder(UserPlayerNumber, ORDER_ALLY, siPara1, ZeroID, TRUE); 
	}

	return 0;
}

SI32 GameMenuFunc(SI32 siPara1, SI32 siPara2)
{
	// ���� �޴��� �����ֵ��� �Ѵ�. 
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

// ��Ȳ�� �����ϱ� ���� ��� ������ �ʱ�ȭ�Ѵ�.
void InitOperateMenu()
{
	SHORT i;

	// '���콺�� ��ġ�� �޴��� ����'�� �ʱ�ȭ 
	MenuOfMouse=-1;
	
	// ���ӿ� ���Ǵ� ��ư���� �ʱ�ȭ�Ѵ�. 
	pGame->pBattle->InitGameButton();

	// ���� �޴� ���۸� �ʱ�ȭ�Ѵ�. 
	for(i=0;i<MAX_MENU_NUMBER;i++)
	{
		GD.SetMenu(i, 0, 0, FALSE, 0);
	}

}

// �޴� �Էµ��� ó���Ѵ�. 
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

	// ��Ȳ�� �����ϱ� ���� ��� ������ �ʱ�ȭ�Ѵ�.
	InitOperateMenu();
	
	SI32	starty;
	
	font = FILE_ON_BUTTMENU;
	
	//  ���� ���� ��ư�� �����. 
	id = pGame->pBattle->FindEmptyGameButton();
	if(id)
	{
		starty = SP.GameMenuButtonStartY;
		// �¶��� ���� ��忡�� ���� ��ư. 
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


	// ������ ���� üũ��ư��
	font = FILE_ON_ALLY_CHECK;
	
	
	k = 0;
	for( i=0; i<MAX_MULTI_PLAYER-1; i++ )
	{
		// ��ȿ�� �÷����̾��� ��� , 
		if( MyNet.PlayersdpId.dpId[i] ) 
		{
			if( ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE && i != UserPlayerNumber )
			{
				if(bGameAllyWindowSwitch == FALSE)
				{
					//  ���� ��ư�� �����. 
					id = pGame->pBattle->FindEmptyGameButton();
					if( id )
					{
						//if( k < 3 )
						//	starty = SP.GameMenuButtonStartY + 20;
						//else 
						//	starty = SP.GameMenuButtonStartY + 25 + GetFileYsize(font);
						
						//startx = SP.GameMenuButtonStartX + 110 + (k%3)*GetFileXsize(font);
						
						// ���� ���� ���, 
						if(GD.OnlineAllyNumSwitch[UserPlayerNumber][i] == TRUE && GD.OnlineAllyNumSwitch[i][UserPlayerNumber] == TRUE)
						{
							// �¶��� ���� ��忡�� ���� ��ư. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 2, FALSE,	// ������ �� ����
								font, 2, FALSE,	// ���� ����
								font, 2, FALSE,	// ���콺 ���ٴ� ����
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// ���� �ٸ� ����ڿ��� ������ ��û�� ���, 
						else if(GD.OnlineAllyNumSwitch[UserPlayerNumber][i] == TRUE)
						{
							// �¶��� ���� ��忡�� ���� ��ư. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 1, FALSE,	// ������ �� ����
								font, 1, FALSE,	// ���� ����
								font, 1, FALSE,	// ���콺 ���ٴ� ����
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// �ٸ� ����ڰ� ���� ������ ��û�� ���, 
						else if(GD.OnlineAllyNumSwitch[i][UserPlayerNumber] == TRUE)
						{
							// �¶��� ���� ��忡�� ���� ��ư. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 1, FALSE,	// ������ �� ����
								font, 1, FALSE,	// ���� ����
								font, 1, FALSE,	// ���콺 ���ٴ� ����
								NULL,
								NULL,	
								EFFECT_CLICK,
								i, 0, 
								OnlineAllyNumFunc);
						}
						// �ƹ��� ������ ��û���� ���� ���, 
						else
						{
							// �¶��� ���� ��忡�� ���� ��ư. 
							pGame->pBattle->GameButton[id].SetGameButton(BUTTON_TYPE_ON_ALLY_NUM, SP.GameAllyWindowX + 8, SP.GameAllyWindowY + 7 + (15 * k),
								font, 0, FALSE,	// ������ �� ����
								font, 0, FALSE,	// ���� ����
								font, 0, FALSE,	// ���콺 ���ٴ� ����
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
	
	
	// ���õ� ĳ���Ͱ� ������ 
	if(GD.SelectedNumber==0)
	{
	}
	// ���õ� ĳ���Ͱ� �ϳ��϶� 
	else if(GD.SelectedNumber==1)
	{
	   // ���õ� �ϳ��� ���̵� ���Ѵ�. 
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

	   // �ϳ��� ĳ���Ͱ� �ִٰ� �س��� �����δ� ���ٸ�, 
/*	   if(id == 0)
	   {
		   clGrp.Error("1239074", "fsdf3283");
		   return ;
	   }*/

	   // ���õ� ĳ���Ͱ� �ϳ��϶����� 0���� �ִ´�. 
       for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	   {
		   GD.SelectedID[i].Init();
	   }

	   // ���õ� �ϳ��� ���̵� 0���� �ִ´�.
	   GD.SelectedID[0] = clifChar.GetID(id);


	   // �Ʊ� ĳ���Ͱ� �ƴϸ� ����� ���� �ʴ´�. 
	   // �Ʊ� ĳ���Ͷ� �ڵ�����̸� ����� ���� �� ����. 
	   if( clifChar.GetPlayerNumber(id) == UserPlayerNumber && clifChar.GetAutoSwitch(id) == FALSE )
	   {


		   // �ڽ��� ������ ������� ���¿����� �����ϴ�. 
		   MenuMode|=MENUMODE_MOVE;
		   
		   if(clifChar.IsAtb(id, ATB_ATTACK))   MenuMode|=MENUMODE_ATTACK;
		   if(clifChar.IsAtb(id, ATB_ATTACKAIR))MenuMode|=MENUMODE_ATTACKAIR;
		   if(clifChar.IsAtb2(id, ATB2_HEAL))   MenuMode|=MENUMODE_HEAL;
		   if(clifChar.IsAtb2(id, ATB2_MANA))   MenuMode|=MENUMODE_MANA;
//		   if(clifChar.GetKind(id) == KIND_FLAGMAN)    MenuMode|=MENUMODE_GOINTOHQ;

		   // ĳ���Ͱ� ������ �ִ� ����� ���ۿ� �ִ´�. 
	       for(i = 0;i < MAX_MENU_NUMBER;i++)
		   {
			   if( clifChar.GetMenuItem(id, i) == 0 )	continue;

			   SHORT order         = Item[clifChar.GetMenuItem(id, i)].GetOrder();
			   SHORT reservenumber = 0;
			   
		       PushGMenu(i, clifChar.GetMenuItem(id, i), reservenumber, clifChar.GetMenuMode(id, i), 100);
		   }
		   
	   }
	}
	// ���� ĳ���Ͱ� ���õǾ� ������ 
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
					   // ��Ƽ���� ���Ǵ� ��ư�� �ƴϸ� ���� �ʴ´�. 
			  	       if(Item[clifChar.GetMenuItem(id, j)].GetMultiSwitch() == FALSE)
					   {}
					   else
					   {
						   // �ڽ��� ������ ������� ���¿����� �����ϴ�. 
						   MenuMode|=MENUMODE_MOVE;
						   
						   if(clifChar.IsAtb(id, ATB_ATTACK))   MenuMode|=MENUMODE_ATTACK;
						   if(clifChar.IsAtb(id, ATB_ATTACKAIR))MenuMode|=MENUMODE_ATTACKAIR;
						   if(clifChar.IsAtb2(id, ATB2_HEAL))   MenuMode|=MENUMODE_HEAL;
						   if(clifChar.IsAtb2(id, ATB2_MANA))   MenuMode|=MENUMODE_MANA;
//						   if(clifChar.GetKind(id) == KIND_FLAGMAN)    MenuMode|=MENUMODE_GOINTOHQ;
						   
						   // �޴��� ���������� ������ index�� �߰����ش�. 
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


	// ���콺�� Ű���� �Է��� ���� � �޴��� ���õǾ����� Ȯ���Ѵ�. 
	for(i = 0;i < MAX_MENU_NUMBER;i++)
	{

		// ��ȿ�� �޴��� ���õȴ�. 
		if(GD.GetMenu(i, 43)==0)continue;


		// Ű���� ó���� �����Ѵ�. (�Է� ��尡 �ƴϾ�� �Ѵ�. )
		// ===============================================================================
//		if(pGame->pBattle->m_EditControl.IsFocus() == FALSE
		// ===============================================================================
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) 
        && (   (Item[GD.GetMenu(i, 2234)].GetHotKeyCode(0) && GameWParam == Item[GD.GetMenu(i, 2234)].GetHotKeyCode(0))
		    || (Item[GD.GetMenu(i, 2235)].GetHotKeyCode(1) && GameWParam == Item[GD.GetMenu(i, 2235)].GetHotKeyCode(1))  ) )
		{

			 // ���콺�� ��ġ�� �޴��� ��ȣ 
		     MenuOfMouse = i;
			 // ���콺�� ���� �޴��� ��ȣ 
			 SelectedMenu = i;


			 SHORT ableswitch = TRUE;

			 // �ǹ� ���� ����̶��, 
			 SHORT order = Item[GD.GetMenu(i, 234)].GetOrder();

			 if(ableswitch==TRUE)
			 {

				 // �۵������� ��忩�� �Ѵ�. 
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
		// ���콺�� �޴� ���� �ְ� 
		else if(IsMouseInMenu(i))
		{
			// ���콺�� ��ġ�� �޴��� ��ȣ 
		    MenuOfMouse = i;

      		// �̹� ���õ� �޴����, 
			if(SelectedMenu == i)
			{
				// ���� ���� 
				if(LeftPressSelectedSwitch == TRUE)
				{

				   if(GameLeftPressSwitch == FALSE)
				   {

					  // ���õ� ���¸� �ʱ�ȭ�Ѵ�. 
				      SelectedMenu       = -1;
				      MenuSelectedSwitch = FALSE;


					   // control Ű�� ������, 
					   if(ControlKeyPressSwitch == TRUE)
					   {

						   // ���õ� ĳ���Ͱ� 1�����̰� ,
		                   // �޴� ��ư������ ������ 
		                   // ���縦 �����ϴ� ��ɸ� �޴´�. 
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

					      // �ǹ� ���� ����̶��, 
					      SHORT order = Item[GD.GetMenu(i, 234)].GetOrder();

						  if(ableswitch == TRUE)
						  {

							  // �۵������� ��忩�� �Ѵ�. 
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

					   // �̹� ����� ����̸� ������ ����ϰ� �����. 

					   // ����� �ʿ�� ���� �ʴ� ��쿡�� ������ ���콺 ��ư�� �Է� �޴´�.
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
			// ���õ��� ���� �ٸ� �޴������, 
			else
			{
			   // ���� ��ư�� ���� ���̶��, 
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
		// ���콺�� �ٸ� ��Ʈ�� �� �����Ѵ�. 
		IsMosueInControlSwitch = TRUE;
	}



	// ���콺�� �޴� ���� ������ 
	 if(MenuOfMouse==-1)
	 {
		 SelectedMenu=-1;
		 MenuSelectedSwitch=FALSE;
		 LeftPressSelectedSwitch=FALSE;
		 RightPressSelectedSwitch=FALSE;

	 }
	 // �޴� ���� ������ ���õ� �޴��� �ƴ϶��, 
	 else if(SelectedMenu!=MenuOfMouse)
	 {
		 SelectedMenu=-1;
		 MenuSelectedSwitch=FALSE;
		 LeftPressSelectedSwitch=FALSE;
		 RightPressSelectedSwitch=FALSE;
	 }


	 // ���콺�� �޴� ���� ������ 
	 if(CrewMenuOfMouse==-1)
	 {
		 SelectedCrewMenu=-1;
		 CrewMenuSelectedSwitch=FALSE;
		 CrewMenuLeftPressSelectedSwitch=FALSE;

	 }
	 // �޴� ���� ������ ���õ� �޴��� �ƴ϶��, 
	 else if(SelectedCrewMenu!=CrewMenuOfMouse)
	 {
		 SelectedCrewMenu=-1;
		 CrewMenuSelectedSwitch=FALSE;
		 CrewMenuLeftPressSelectedSwitch=FALSE;
	 }


	 // �޴� ���� ���콺�� ��ġ�� �ִٸ� ������ ǥ���Ѵ�.
	 if(MenuOfMouse>=0 && GD.GetMenu(MenuOfMouse, 345))
	 {

		 // ���콺�� ��ġ�� �޴��� ��ġ�� ���Ѱ��̸�, 
		 if(GD.HelpMenuOfMouse!=MenuOfMouse)
		 {
		    GD.HelpMenuOfMouse =MenuOfMouse;
		    GD.HelpMenuClock   =CurrentClock;
		 }
		 // ��ġ���� 600clock�� �������̸�, 
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
					 // �ǹ��̸�, 
					 if(KI[needkind].Atb & ATB_BUILDING)
						wsprintf(textbuffer, "%s:[%s]", Text.NeedBuildingMessage.Get(), KI[needkind].Name);
					 // �Ϲ� �����̸�, 
					 else
						wsprintf(textbuffer, "[%s] %s", KI[needkind].Name, Text.Need.Get());
					 text2=textbuffer;
				 }
				 */
			 }
			 // ���� �Ӽ��̸�, 
			 else if(Order[Item[item].GetOrder()].IsAtb(ORDERATB_MAGIC) )
			 {
				 // �ʿ��� ������ 
				 magic=Order[Item[item].GetOrder()].Para1;
				 
				 if(magic > CR[GD.SelectedID[0].ID].m_ParmeterBox.IP.GetMana())
				 {
					 wsprintf(textbuffer, "%s", Text.InsufficientMana.Get());
					 text2=textbuffer;
				 }

			 }
			 // �������� ���ڶ��, 
			 else if(Item[item].GetOrder() ==ORDER_MINE)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.MineStockMessage.Get(), 0);
				 text2=textbuffer;
			 }
			 // �������� ���߰����̸�, 
			 else if(Item[item].GetOrder()==ORDER_AIRMAGIC)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.AirMagicStockMessage.Get(), 0);
				 text2=textbuffer;
			 }

			 // ��θ� Ȳ�ҷ� �ٲٴ� ���̶��, 
			 else if(Item[item].GetOrder() ==ORDER_FARMERTOBULL)
			 {
				 wsprintf(textbuffer, "%s:%d", Text.BullStockMessage.Get(), 0);
				 text2=textbuffer;
			 }

			 // ������ �����̸�, 
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
	// ���õ� ĳ���Ͱ� ������ �� ��� �������� ĳ���͵��� ó��.
	if (GD.SelectedNumber > 1)
	{
		RECT rtSelectCharacter;								// ���� ĳ���� �ڽ�
		POINT ptMousePos = {IpD.Mouse_X, IpD.Mouse_Y};		// ���콺 ��ǥ
		SI32 siSelectedID = -1;								// ���õ� ĳ���� ���̵�
		SI32 siIndex = 0;									// �ε���

		for (SI32 siI = 0; siI < MULTI_SELECT_GROUP_ROW; siI++)
		{
			for (SI32 siJ = 0; siJ < MULTI_SELECT_GROUP_COLUMN; siJ++)
			{
				rtSelectCharacter.left = MULTI_SELECT_START_POSITION_X + ((siJ + 1) * MULTI_SELECT_GAP_WIDTH_SIZE) + (siJ * MULTI_SELECT_PORTRAIT_WIDTH);
				rtSelectCharacter.top = MULTI_SELECT_START_POSITION_Y + (siI * MULTI_SELECT_PORTRAIT_HEIGHT) + (siI * MULTI_SELECT_GAP_HEIGHT_SIZE);
				rtSelectCharacter.right = rtSelectCharacter.left + MULTI_SELECT_PORTRAIT_WIDTH;
				rtSelectCharacter.bottom = rtSelectCharacter.top + MULTI_SELECT_PORTRAIT_HEIGHT;

				// ���콺�� ����Ű�� �ִ� ��ġ ��ǥ ���
				if (PtInRect(&rtSelectCharacter, ptMousePos) == TRUE)
				{
					// ���콺 �Ʒ��� �ִ� ĳ���� ã��
					SI32 siTempIndex = 0;
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						SI32 siTempID = GD.SelectedID[i].ID;
						if(siTempID)
						{
							if (siTempIndex == siIndex)
							{
								siSelectedID = siTempID;			// ���콺 �Ʒ��� �ִ� ĳ���� �ε���
								goto FIND_CHARACTER_INDEX_UNDER_CURSOR;	// �ش� ó����ƾ���� �̵�
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
// ���콺 Ŀ�� �ؿ� ��ȿ�� ĳ���Ͱ� ���� ��� ó��
FIND_CHARACTER_INDEX_UNDER_CURSOR:

		// ���콺 ���ʹ�ư Ŭ���� �ȵǾ��ٸ�.
		if (FALSE == GameLeftPressSwitch)
		{
			// ���콺 Ŀ�� �Ʒ� ĳ���� �̸� ���
			// ü��, ������ ��ġ ǥ��
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

		GameLeftPressSwitch = FALSE;		// ���콺 ���ʹ�ư Ŭ���ʱ�ȭ
		DragSwitch = FALSE;					// ���콺 �巡�� ����ġ �ʱ�ȭ
		MapDragSwitch = FALSE;				// �� �巡�� ����ġ �ʱ�ȭ

		// ĳ���� �ʻ��� Ŭ���Ͽ��� ���
		if ((FALSE == ControlKeyPressSwitch) && (FALSE == ShiftKeyPressSwitch))
		{
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				id=GD.SelectedID[i].ID;
				if(id)
				{
					// �ش� ĳ���� �̿��� ĳ���͸� �׷쿡�� �����Ѵ�.
					if (id != siSelectedID)
					{
						CR[id].SetSelect(FALSE, FALSE);
					}
				}
			}
			// ������ ĳ���͸� ���õǵ��� ����.
			CR[siSelectedID].SetSelect(TRUE, TRUE);
			return;
		}

		// ���콺 ���� ��ư�� ��Ʈ�� Ű�� ������ ���
		// ���� ������ ĳ���͵�θ� �̷���� ĳ���� �׷��� �����.
		// ��� ���ְ� �Ϲ� ������ �ٸ� ������ ĳ���ͷ� �Ǵ��Ѵ�.
		// (��: �İ�°� ����İ���� �ٸ� ������ ĳ���ͷ� �Ǵ��Ѵ�.)
		if (TRUE == ControlKeyPressSwitch)
		{
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				id=GD.SelectedID[i].ID;
				if(id)
				{
					// ���� ������ ĳ�������� �˻�.
					if (CR[id].m_cltCharStatus.GetKind() != CR[siSelectedID].m_cltCharStatus.GetKind())
					{
						// �ٸ� ������ ĳ�����̹Ƿ� �׷쿡�� ����
						CR[id].SetSelect(FALSE, FALSE);
					}
					else
						// ���� ������ ĳ�����̹Ƿ� ĳ���� ����
						CR[id].SetSelect(TRUE, FALSE);
				}
			}
			ControlKeyPressSwitch = FALSE;		// ��Ʈ��Ű ���� �ʱ�ȭ

			return;
		}

		// ����ƮŰ�� �������� ���콺 ���� ��ư�� Ŭ���Ͽ������ ó��
		// ���� �׷쿡�� �ش� ĳ���͸� �����Ѵ�.
		if (TRUE == ShiftKeyPressSwitch)
		{
			// ĳ���� �׷쿡�� ����
			CR[siSelectedID].SetSelect(FALSE, FALSE);
			ShiftKeyPressSwitch = FALSE;			// ����ƮŰ ���� �ʱ�ȭ
		}
	}
#endif // _DEBUG
}

// �޴��� �׸���. 
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

	// Ŭ���� ������ ����Ѵ�. 
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
		// ��ȿ�� �÷����̾��� ��� , 
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

	// ���õ� ĳ���Ͱ� �Ѱ��϶� �ʻ�ȭ�� �׸���.
	if(GD.SelectedNumber==1)
	{
		for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
		{
			id=GD.SelectedID[i].ID;
			if(id)
			{
				// ĳ���� ������ �����ش�. 
                clifChar.ShowCharInfo(id, surface);
				break;
			}
		}
	}
#ifdef _DEBUG
	// robypark 2004/10/6 15:26
	// ���õ� ĳ���Ͱ� ������ �� ��� ĳ���͵��� �������� �����ش�.
	// �Ϲ� �����ӿ��� �������� ��ó��.
	else if (GD.SelectedNumber > 1)
	{
		POINT ptMassSelected[MULTI_SELECT_MAX_COUNT];		// �׷����� �� ĳ���͵��� �ʻ�ȭ�� �׷����� ��ġ ��ǥ
		SI32 siIndex = 0;				// �迭 �ε���
		for (SI32 siI = 0; siI < MULTI_SELECT_GROUP_ROW; siI++)
		{
			for (int siJ = 0; siJ < MULTI_SELECT_GROUP_COLUMN; siJ++)
			{
				// �׷��� ĳ���͵��� �ʻ�ȭ�� �׷��� ��ġ �� ���
				ptMassSelected[siIndex].x = MULTI_SELECT_START_POSITION_X + ((siJ + 1) * MULTI_SELECT_GAP_WIDTH_SIZE) + (siJ * MULTI_SELECT_PORTRAIT_WIDTH);
				ptMassSelected[siIndex].y = MULTI_SELECT_START_POSITION_Y + (siI * MULTI_SELECT_PORTRAIT_HEIGHT) + (siI * MULTI_SELECT_GAP_HEIGHT_SIZE);
				siIndex++;	// �迭 �ε��� ����
			}
		}

		siIndex = 0;

		// �׷� �� ĳ���͵��� �ʻ�ȭ �׸���
		for (i = 0; i < MAX_SELECTED_CHAR_NUMBER; i++)
		{
			id = GD.SelectedID[i].ID;

			if (id)
			{
				// �׷� ������ ĳ���͵��� ȭ�� �ϴܿ� �׸���
				// ĳ���� ������ �����ش�. 
                clifChar.ShowCharInfoSimple(id, surface, ptMassSelected[siIndex].x, ptMassSelected[siIndex].y, MULTI_SELECT_PORTRAIT_WIDTH, MULTI_SELECT_PORTRAIT_HEIGHT);
				siIndex++;
			}
		}
	}
#endif
	
	buttonFile = FILE_BUTTON_PLUS;

	// �������� �׸���.
	// ������ �޴��� �׸���. 
	
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

				// ��ư�� ���õǾ����� 
				if(SelectedMenu==i && MenuSelectedSwitch==TRUE)
				{
					Draw_PutSpriteT(startx, starty, FILE_ON_BUTTON, 2);
				}


				
				
				// ����Ű�� ǥ���Ѵ�. 
				if(Item[GD.GetMenu(i, 134)].GetHotKeyCode(0))
				{
					font=Item[GD.GetMenu(i, 134)].GetHotKeyCode(0) -'A';
					
					Draw_PutSpriteT(startx+3, starty+3, FILE_SHORTCUTKEY, font);
				}
				
				
				// ����� ���� ����� ������ �����ش�. 
				for(j=0;j<GD.MenuReserve[i];j++)
				{
					clGrp.FillBox((startx+4)+j*5,   (starty+SP.MenuButtonYsize-12), 
						(startx+4)+j*5+3, (starty+SP.MenuButtonYsize-12)+5,  (UI08)COLOR_GREEN);
				}
				
			}
			
		}
		clGrp.UnlockSurface(surface);
	}


	// �����߿� ������ ��ư���� ��Ƽ� �׷��ش�. 
	pGame->pBattle->DrawGameButton(surface);

	// Ŭ���� ������ �� �����Ѵ�. 
	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
	
}

// ���� �޴��� �����. 
void ClearMenu()
{
	int i;

	for(i=0;i<MAX_MENU_NUMBER;i++)
		GD.SetMenu(i, 0, 0, FALSE, 0);
}


