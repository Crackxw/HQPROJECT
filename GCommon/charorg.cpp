//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "..\Gersang\maindata.h"
#include <etc.h>
#include <item.h>
#include "..\Gersang\music.h"
#include "..\Gersang\calcattack.h"
#include "..\Gersang\mine.h"
#include <CharOrg.h>
#include "..\Gersang\text.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include "../ImjinOnline/onlineworld.h"
#include <onlinebattlemsg.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <MapInterface.h>
#include <CharInterface.h>
#include <Map.h>
#include <yprg2.h>
#include <MyNet-Game.h>
#include <findpath5.h>
#include <colortable.h>
#include "..\Gersang\cheat.h"
#include "OnlineItem.h"
#include <smoke.h>
#include <Magic.h>
#include <DebugTool.h>
#include <setchar.h>
#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb


extern _clGame* pGame;

extern	_MainData				GD;									// 게임의 주데이터 
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 
extern	SHORT 					WaterBlock;
extern	SHORT 					BasicBlock;
extern	_Item 					Item[];
extern	_Order 					Order[];
extern	SI32					UserPlayerNumber;					// 사용자 관련 
extern	_Text 					Text;								// 텍스트 
extern	_Map 					Map;
extern	SHORT 					MapEditMode;						// 지도 편집 모드인가?
extern	clock_t					CurrentClock;						// 시간 관련 
extern	_Char					CR[];								// 캐릭터의 포인터 
extern	_MyNetGame				MyNet;
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 
extern	_NewID 					ZeroID;
extern	cltCharInterface		clifChar;							// 캐릭터 관련 인터페이스 
extern	CCharFileLoadManager	g_FileLoadManager;				// 조합형 캐릭터
extern	_OnlineBattleMsgQue		OBMQue;
extern	CItem					*g_pItem;
extern	int 					FlyVibrateData[];
extern	int 					MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 					MapCenterY[][MAX_MAP_YSIZE];
extern	unsigned char			TeamColor[];
extern SI32 AttackAtbInfo[];
extern	_Cheat 					Cheat;
extern SHORT					GameStatus;

cltCharOrg::cltCharOrg()
{


}

void cltCharOrg::Init(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber,_ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid)
{

	int i;

	m_cltCharDrawInfo.SetDrawSwitch(TRUE);

	m_cltCharDrawInfo.SetRealImageXPercent(100);
	m_cltCharDrawInfo.SetRealImageYPercent(100);

	m_cltCharDrawInfo.SetDrawFrame(-1);                         // 이전에 그림을 그린 프레임 

	// robypark 2004/6/14 15:35
	// 죽기 전에 이 캐릭터가 선택이 되어 있었나를 나타내는 플래그
	m_bAlreadySelectedBeforeDead = FALSE;

	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		m_scSeeMode[i]	=	TRUE;           // 보이는 방향인가 사라지는 방향인가
	}

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);

	//////////////////////////////////////////////////
	//char-wait.cpp에 if(GD.Player[GetPlayerNumber()].PI.Host != BY_PLAYER) 부분 함께 수정.
	//////////////////////////////////////////////////

	m_cltCharStatus.SetArea(AREA_LAND);

	Direction		=	dir;
	TempDirection	=	dir;
	TargetDirection	=	dir;
	m_bChangePlayer	= FALSE;
	

	m_scSeeSwitch=TRUE;

	Para2.Init();

	CurrentOrder=ORDER_WAIT;

	m_NewID.ID		=	id;
	m_NewID.Unique	=	unique;

	m_siOnlineID		= onlineid;

	//수호부
	m_bGuardianChar		= FALSE;
	m_bUseGuardian		= false;
	m_siCount			= 0;
	m_dwGuardianTime	= 0;

	m_dwCawTimer		= 0;
	m_siCawNumber		= 0;

	m_siGuardianMaxLife		= 0;
	m_siGuardianLife		= 0;

	// 돼지
	m_dwPigAttackDelay = 0;
	m_bPigAttack = m_bPigDefence = FALSE;

	// 닭
	m_ChickenID[0].Init();
	m_ChickenID[1].Init();
	m_ChickenID[2].Init();
	m_siChickPrevKind = 0;

	m_ssX		=	x;		
	m_ssY		=	y;
	m_scXsize	=	1; 
	m_scYsize	=	1;
	m_scIDXsize = m_scIDYsize = 1;


	Group = -1;                                // 선택된 팀이 없다. 


	ZeroMemory(&BufferOrder, sizeof(_OrderDataInfo));
	BufferOrder.Order=ORDER_WAIT;

	ReserveOrderIndex=0;
	for(i=0;i<MAX_BUFFER_ORDER_NUMBER;i++)
	{
		ZeroMemory(&ReserveOrder[i], sizeof(_OrderDataInfo));
	}
	
	ZeroMemory(&OldOrder, sizeof(_OrderDataInfo));

	OtherCollSwitch=FALSE;
	OtherCollPoint.x=0;
	OtherCollPoint.y=0;
	m_siFontCount   =0;

	MenuNumber=0;
	for(i=0;i<MAX_MENU_NUMBER;i++)
	   MenuItem[i]=0;

	UpdateMenuOrder=ORDER_INITMENU;

	// 마법 애니메이션 프레임 순서
	m_siMagicAniStep = 0;

	// 경계 관련 
	GuardX=-1;
	GuardY=-1;

	// 죽음 관련
	CarcassDelay=100;
	CarcassDelayor=0;

	// 피격관련 
	AttackMePlayer=-1;
	AttackMeID.Init();

	FireID.Init();

	// 정지의 기본 갯수는 1개다. 
	MaxWaitType = 1;
	WaitType = ANI_WAIT0;

////////////////////////////////////////////////////////////	
	m_cltCharStatus.SetKind(kind);
	m_cltCharStatus.SetPlayerNumber(pnumber);
	m_cltCharStatus.SetOriginalPlayerNumber(pnumber);

	// 중요 파라메타 정보를 얻어온다. 
	if(pip)
	{
		m_ParmeterBox.SetImportantParameta(*pip);
	}
	else 
	{
		clGrp.Error("No Info", "test:0001");
	}

	// 무장 상태를 얻어온다. 

	HeroInit();

	if(pmw)
	{
	
		SI16			siEquip;
		CItemHeader		*pItemHeader;

		MW = *pmw;

		// 갑옷						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_ARMOR_ITEM), siEquip );
		SetEquip(TRUE, ON_WEAR_ARMOR_ITEM, (SI32)siEquip, FALSE);
		// 헬멧						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_HELMET_ITEM), siEquip );
		SetEquip(TRUE, ON_WEAR_HELMET_ITEM, (SI32)siEquip, FALSE);
		// 무기						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_WEAPON_ITEM), siEquip );				
		SetEquip(TRUE, ON_WEAR_WEAPON_ITEM, (SI32)siEquip, FALSE);
		m_ssWeaponID = MW.GetWearItem(ON_WEAR_WEAPON_ITEM);
		//수호부
		m_siGuardianID = MW.GetWearItem(ON_WEAR_GUARDIAN_ITEM);
		
		if(m_siGuardianID)
		{
			pItemHeader = g_pItem->GetItem(m_siGuardianID);

			m_siGuardianKind		 = pItemHeader->m_pGuardian->m_siKind;
			m_siGuardianCondition	 = pItemHeader->m_pGuardian->m_siStartCondition;
			m_siGuardianSkillNum     = pItemHeader->m_pGuardian->m_siSkillNum;

			//사용할수 있는 기술이 2개라면 그중 한개를 선택.
			if(m_siGuardianSkillNum == 2)
				m_siGuardianSkillNum = (GD.Random() % 2);
			else
				m_siGuardianSkillNum = 0;

			m_uiGuardianRecallKind	 = pItemHeader->m_pGuardian->m_uiRecallKind[m_siGuardianSkillNum];
			m_siGuardianRecallNum	 = pItemHeader->m_pGuardian->m_siRecallNum[m_siGuardianSkillNum];
			m_siGuardianNHitrate	 = pItemHeader->m_pGuardian->m_siNHitrate[m_siGuardianSkillNum];
			m_siGuardianBHitrate	 = pItemHeader->m_pGuardian->m_siBHitrate[m_siGuardianSkillNum];
			m_siGuardianSHitrate	 = pItemHeader->m_pGuardian->m_siSBHitrate[m_siGuardianSkillNum];
			m_siGuardianNoHaltHitrate= pItemHeader->m_pGuardian->m_siNoHaltSBHitrate[m_siGuardianSkillNum];
			m_siGuardianAliveTime	 = pItemHeader->m_pGuardian->m_siAliveTime[m_siGuardianSkillNum];

			//종료시간이 정해져 있는 않은 기술이라면.
			if(m_siGuardianAliveTime > 0)
				m_siGuardianTimeOut  = TRUE;
			else 
				m_siGuardianTimeOut = FALSE;

				//수호부를 장착하고 있다.
			m_bGuardianEquip = true;

			SetGuardianStartImage(m_siGuardianKind);
		}
	}
	

	m_ParmeterBox.Action(g_pItem, MW);

	// m_ParmeterBox.Action()함수 이후에 처리되어야 한다. 
	if(pip == NULL)
	{
		m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
		m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
	}


	m_uiAtb		= KI[m_cltCharStatus.GetKind()].m_uiAtb;
	m_uiAtb2	= KI[m_cltCharStatus.GetKind()].m_uiAtb2;
	m_uiAtb3	= KI[m_cltCharStatus.GetKind()].m_uiAtb3;
	m_uiAtb4	= KI[m_cltCharStatus.GetKind()].m_uiAtb4;
	m_uiAtb5	= KI[m_cltCharStatus.GetKind()].m_uiAtb5;
	DyingAtb=(SHORT)KI[m_cltCharStatus.GetKind()].m_uiDyingAtb;
	
	m_siAttackAtb1 = KI[m_cltCharStatus.GetKind()].GetAttackAtb(0);
	m_siAttackAtb2 = KI[m_cltCharStatus.GetKind()].GetAttackAtb(1);

	m_uiDefenceAtb=KI[m_cltCharStatus.GetKind()].m_uiDefenceAtb;

	m_scIDXsize=(char)KI[m_cltCharStatus.GetKind()].m_ssIDXsize;
	m_scIDYsize=(char)KI[m_cltCharStatus.GetKind()].m_ssIDYsize;

	// 투명하게 비쳐보인다.
	if(IsAtb(ATB_TRANSPARENT)==TRUE)
	   m_cltCharDrawInfo.SetTransparentSwitch(TRUE);

	if(IsAtb(ATB_FLY))
	{
		m_cltCharStatus.SetArea(AREA_AIR);
	}
	else
	    m_cltCharStatus.SetArea(AREA_LAND);


	// 디폴트 애니메이션을 설정한다. 
	m_cltCharDrawInfo.SetAnimation(ANI_WAIT0);

	m_siAttackRange1 = KI[m_cltCharStatus.GetKind()].GetAttackRange(0);
	m_siAttackRange2 = KI[m_cltCharStatus.GetKind()].GetAttackRange(1);

	// robypark 2004/10/18 12:36 - 두 가지 공격 속성이 있을 경우 인공지능 처리 관련 변수 설정
	m_ssPriority_Attack_Type = KI[m_cltCharStatus.GetKind()].GetPriority_Attack_Type();

	m_siSearchRange=(unsigned char)KI[m_cltCharStatus.GetKind()].m_siSearchRange;
	MaxFireRepeatNumber=(char)KI[m_cltCharStatus.GetKind()].m_ssMaxFireRepeatNumber;
	FireRepeatDelay=KI[m_cltCharStatus.GetKind()].m_ssFireRepeatDelay;

	m_cltCharStatus.SetWorkDelay( (char)KI[m_cltCharStatus.GetKind()].m_ssWorkDelay );
	DeadDelay=KI[m_cltCharStatus.GetKind()].m_ssDeadDelay;
	MoveSpeed=KI[m_cltCharStatus.GetKind()].m_ssMoveSpeed;

    ChangeDirectionDelay=(unsigned char)KI[m_cltCharStatus.GetKind()].m_ssChangeDirectionDelay;
	AttackGiveUpDelay=KI[m_cltCharStatus.GetKind()].m_ssAttackGiveUpDelay;

	m_cltCharDrawInfo.SetVibrateSwitch(KI[m_cltCharStatus.GetKind()].m_ssVibrateSwitch);

	if(IsAtb(ATB_SHIP))
	{
		Block=WaterBlock;
		SelfBlock|=BLOCK_CHAR;
	}
	else if(IsAtb(ATB_FLY))
	{
		Block=0;
		SelfBlock|=BLOCK_AIRCHAR;
	}
	else if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_SWIM))
	{
		Block = BasicBlock;
		if(Block & BLOCK_DEEPWATER)
			Block -= BLOCK_DEEPWATER;

		SelfBlock |= BLOCK_CHAR;
	}

	else 
	{
	   Block = BasicBlock;
	   SelfBlock |= BLOCK_CHAR;
	}

//	m_siGuardianKind    = 0;

	m_MoveDelay			= 1.0;
	m_AttackDelay		= 1.0;
	m_bRabbitAttack		= false;
	m_RabbitShooterId	= 0;

	m_bHorseAttack		= false;

	m_bGuardianLifeUp		  = false;
	m_bMagicResist	  = false;
	m_bDamageResist  = false;


	m_siDeadCawX		=   0;
	m_siDeadCawY		=   0;

	m_siPreGuardianID	=   0;

	m_siAttackX_ForGuardianDragon =	0;
	m_siAttackY_ForGuardianDragon =	0;

	GhostTime = 0;

	m_CloudWindFrame = 0;

	if(m_siGuardianCondition == ON_GUARDIAN_START)
	{
		m_bUseGuardian = true;
		m_dwGuardianTime = timeGetTime();
	}

	// 다른 사용자에게 처음부터 업데이트 할 필요는 없다. 
	SetUpdateInfo();
	SetOldUpdateInfo();

}

BOOL cltCharOrg::GetSeeStep(SI32 pnumber)const 
{
	return m_scSeeStep[pnumber];
}

SI32 cltCharOrg::GetBlackByPlayerNumber(SI32 pnumber, SI32 x, SI32 y)const 
{
	return GD.BlackMap[ pnumber ][x][y];
}

BOOL cltCharOrg::GetSeeModeByPlayerNumber(SI32 pnumber)const 
{
	return m_scSeeMode[ pnumber ];
}


void cltCharOrg::PushMenu(SHORT number, int menu, SHORT needupgrade, SHORT level, SHORT kind, SHORT needkind, SHORT money, SHORT tree)
{
	SHORT mode=TRUE;
	SHORT percent=0;
	SHORT order = Item[menu].GetOrder();

	if(number>=MAX_MENU_NUMBER)return ;

	// 기존의 명령의 수행 순위와 새로 받은 명령의 수행순위를 비교한다. 
	
	// 새로운 명령의 우선순위가 더 급하고
	if(Order[order].Priority<Order[CurrentOrder].Priority)
	{
		mode=TRUE;
	}
	else if(Order[order].Priority==Order[CurrentOrder].Priority 
	&& Order[order].Info&ORDERINFO_SAMEPRIORITY)
	{
		mode=TRUE;
	}
	else if(Order[order].Priority==Order[CurrentOrder].Priority 
	&& Order[order].Info&ORDERINFO_LOWPRIORITY)
	{
		mode=FALSE;
	}



	// 마법인 경우에 
	if(Order[order].Atb&ORDERATB_MAGIC)
	{
		// 마법력이 모자라면, 
		if(m_ParmeterBox.IP.GetMana() < Order[order].Para1)
		{
			mode=FALSE;
		}
		else mode=TRUE;
	}

	switch(menu)
	{
		// 자폭은 자폭 변수가 채워져야 활성화된다. 
		case ITEM_SELFEXP: 
			if(SelfExpPercent==100)
			{
				if(mode==TRUE)
				   mode=TRUE;
			}
			else
			{
			   mode=FALSE;

				percent=SelfExpPercent;
			}
			break;

		case ITEM_MAGIC_RAINMAKE:
			// 비가 오는 중이면 사용할 수 없다. 
			if(GD.Rain.RainSwitch==TRUE)
			{
				mode=FALSE;
			}

			break;

		default:
			break;
	}


	MenuItem[number]=menu;
	MenuMode[number]=(unsigned char)mode;
	MenuNumber++;

}

void cltCharOrg::PushMenuByOrder(SHORT number, int menu)
{
	SHORT mode=TRUE;
	SHORT percent=0;
	SHORT order = Item[menu].GetOrder();

	if(number>=MAX_MENU_NUMBER)return ;

	// 기존의 명령의 수행 순위와 새로 받은 명령의 수행순위를 비교한다. 
	
	// 새로운 명령의 우선순위가 더 급하고
	if(Order[order].Priority<Order[CurrentOrder].Priority)
	{
		mode=TRUE;
	}
	else if(Order[order].Priority==Order[CurrentOrder].Priority 
	&& Order[order].Info&ORDERINFO_SAMEPRIORITY)
	{
		mode=TRUE;
	}
	else if(Order[order].Priority==Order[CurrentOrder].Priority 
	&& Order[order].Info&ORDERINFO_LOWPRIORITY)
	{
		mode=FALSE;
	}

	SI32 needkind = Order[order].siNeedKind;



	MenuItem[number]=menu;
	MenuMode[number]=(unsigned char)mode;
	MenuNumber++;

}


/////// char-order ////////////////////////////////////////////////////////////////
// 명령을 버퍼에 넣는다. 
// 사용자가 준 명령이 이미 버퍼안에 있는 경우에는 컴퓨터가 준 명령은 넣지 않는다. 
BOOL cltCharOrg::PushOrder(_OrderDataInfo order, SHORT mode, SHORT debuginfo)
{

//////////////////////////////////////////////////////////////////////////////
//  사용자의 명령이 몇 프레임 전에 입력되어 대기 상태에 있는데 컴이 현재 수행중인 
// 작업을 종료하고 정지 명령을 내리면 사용자가 내린 명령이 무시되는 문제가 있다. 
// 이런 경우에는 컴이 내린 명령을 무시해야 한다. 
/////////////////////////////////////////////////////////////////////////////

	// 이미 사용자의 명령이 있고 
	// 컴의 명령이 들어간다면, 
	// 새로 들어간 명령은 무시한다. 
	if(BufferOrder.Order!=ORDER_WAIT 
	&& BufferOrder.HostMode!=BY_COMPUTER
	&& order.HostMode==BY_COMPUTER)
		return FALSE;
		
	BufferOrder=order;
	PushOrderMode=mode;    // 자금을 줄이는가?


	return TRUE;
}

// 예약 명령을 삭제한다. 
void cltCharOrg::ClearReserveOrder()
{
	ReserveOrderIndex=0;
}

// 예약 명령을 꺼내서 실행 버퍼로 옮긴다. 
void cltCharOrg::PopReserveOrder()
{
	SHORT i;

	// 실행 버퍼가 비어 있어야 한다. 
	if(BufferOrder.Order!=ORDER_WAIT)return ;

	// 예약된 명령이 없으면 수행하지 않는다. 
   if(ReserveOrderIndex<=0)return ;  

   PushOrder(ReserveOrder[0], TRUE, 10);
   
   for(i=0;i<(ReserveOrderIndex-1);i++)
   {
	   ReserveOrder[i]=ReserveOrder[i+1];
   }

   ReserveOrderIndex--;
}

// 명령을 예약 버퍼에 넣는다. 
BOOL cltCharOrg::PushReserveOrder(_OrderDataInfo order)
{
	// 최대값을 넘으면 마직막 예약버퍼에 계속 넣는다. 
	if(ReserveOrderIndex>=MAX_BUFFER_ORDER_NUMBER)
		ReserveOrderIndex=MAX_BUFFER_ORDER_NUMBER-1;

	ReserveOrder[ReserveOrderIndex]=order;
	ReserveOrderIndex++;

	return TRUE;
}

// 주어진 명령이 예약 버퍼에 이미 들어 있는지 확인한다. 
BOOL cltCharOrg::HaveReserveOrder(SHORT order)
{

	SHORT i;

	for(i=0;i<ReserveOrderIndex;i++)
	{
		if(ReserveOrder[i].Order==order)return TRUE;
	}
	return FALSE;
}


// 현재 수행중인 명령이 무엇이었느냐에 따라 여러가지 복구를 시켜준다. 
void cltCharOrg::RecoverPrevOrder()
{

}

void cltCharOrg::SetSeeSwitch(SHORT mode)
{
   switch(mode)
   {
	  case TRUE:
		 m_scSeeSwitch=TRUE;
		 break;
	  case FALSE:
		 m_scSeeSwitch=FALSE;
		 if(m_scSelectedSwitch==TRUE)
		     SetSelect(FALSE, FALSE);

		 break;
   }
}

// 장수인지 여부를 알려준다. 
BOOL cltCharOrg::IsGeneral()const 
{
		if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL) && m_siTimerDelay ==0)return TRUE;
		return FALSE;
}


// 캐릭터가 x, y위치의 floor층에 있을 수 있는가 없는가?
BOOL cltCharOrg::IsColl(SHORT x, SHORT y)
{
	int i, j;

	 for(i=0;i<m_scIDYsize;i++)
		 for(j=0;j<m_scIDXsize;j++)
		 {
			if(Map.IsColl(x-m_scIDXsize+j+1, y-m_scIDYsize+i+1, Block))
				return TRUE;
		 }

	return FALSE;	
}

// 캐릭터가 x, y위치의 에 있을 수 있는가 없는가?
BOOL cltCharOrg::IsColl(SHORT x, SHORT y, SHORT block)
{
	int i, j;
	
	for(i=0;i<m_scIDYsize;i++)
		for(j=0;j<m_scIDXsize;j++)
		{
			if(Map.IsColl(x-m_scIDXsize+j+1,y-m_scIDYsize+i+1, block))
			{
				return TRUE;
			}
		}
		
	return FALSE;	
}


// 지도 위치 x,y에서 이루어지는 조치를 취한다. 
void cltCharOrg::OperateMapPosition(SHORT x, SHORT y)
{

	  // 이동하는 위치에 자원이 있는지 확인한다. 
	  if(Map.GetTreasureMap(m_ssX,m_ssY))
	  {
	 	 GetTreasure(Map.GetTreasureMap(m_ssX, m_ssY));
	 	 Map.SetTreasureMap(m_ssX, m_ssY, 0);

		 if(GetPlayerNumber() == UserPlayerNumber)
			PushEffect(EFFECT_GETITEM, m_ssX, m_ssY);
	  }

	  // 이동하는 위치에 지뢰가 있는지 확인한다. 
	  // 아군이 설치한 지뢰는 상관없다. 
	  /*
	  if(IsEnemyMine(X, Y, GetPlayerNumber()))
	  {
		  // 지뢰를 밟았을떄의 처리를 한다. 
		  StepEnemyMine(X, Y, GetPlayerNumber());
	  }
	  */
}

BOOL cltCharOrg::SetSelect(int mode, int effectswitch)
{
	int i;
	int number=-1;

	// 지도 편집 모드에서는 캐릭터를 선택하거나 삭제할 수 없다. 
	if(MapEditMode==TRUE)return FALSE;

	// 이미 선택되어 있는 것이 아닌지 확인 한다. 
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		
		if(GD.SelectedID[i].IsSame(m_NewID))
		{
			number=i;
		}
	}

	if(mode==TRUE)
	{
		// 이미 죽은 캐릭터는 선택할 수 없다. 
		if(m_ParmeterBox.IP.GetLife() <= 0)return FALSE;

	   // 선택된 시간 
	   SelectedClock=CurrentClock;

		// 이미 선택된 캐릭터이면, 
       if(number>=0)return FALSE;


		UpdateMenu(ORDER_INITMENU);

	   // 선택시에 효과를 나타내야 하는 경우 
	   if(effectswitch==TRUE)
	   {
          SelectOperation();
	   }

		for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
		{
			if(GD.SelectedID[i].IsEmpty()==TRUE)
			{
				GD.SelectedID[i]=m_NewID;
				GD.SelectedNumber++;
				break;
			}
		}

		m_scSelectedSwitch=mode;
	}
	else
	{
		// 선택되어 있지 않은 캐릭터라면 
       if(number<0)return FALSE;

	   for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	   {
			if(GD.SelectedID[i].IsSame(m_NewID))
			{
				GD.SelectedID[i].Init();
				GD.SelectedNumber--;
				
				// 더이상 선택된 것이 없을때에는, 
				if(GD.SelectedNumber==0)
				{
		           GD.ObjectBufferIndex=0;
		           GD.ObjectNeedSwitch=FALSE;
		           GD.SelectedOrder=0;
				}
				break;
			}
		}

	   m_scSelectedSwitch=mode;

	}

	

	return TRUE;
}

// 인근의 빈 자리에 보물을 내려 놓는다. 
void cltCharOrg::PutTreasureNear(SHORT type, SI32 amount)
{
	SI16 x, y;

	if(FindEmptyAreaTreasure(x, y) == TRUE)
	{
		Map.PutTreasure(x, y, type, amount);
	}

}


void cltCharOrg::GetTreasure(SHORT treasure)
{

	// 얻은 보물을 확인한다. 
	SI32 siType = (treasure & 0xff00) >> 8;

	// 양을 분석한다. 
	SI32 siAmount = treasure & 0x00ff;

}

// 죽은 후 자리에 아이템을 남겨 놓는다.
BOOL cltCharOrg::ReleaseItem()
{

	return TRUE;
}

BOOL cltCharOrg::SelectOperation()
{
	// 관전자이거나 유닛의 주인만이 유닛의 효과음을 들을 수 있다.
	if( GD.Player[UserPlayerNumber].PI.Country == COUNTRY_NEUTRAL ){}
	else if( GetPlayerNumber() != UserPlayerNumber )	return FALSE;

	   
   if(KI[m_cltCharStatus.GetKind()].m_siSelectEffect)
   {
#ifndef _USE_HASHCODE_EFFECTSOUND	  
	   PushEffect(KI[m_cltCharStatus.GetKind()].m_siSelectEffect, m_ssX, m_ssY);
#else
	   HashCodePushEffect(KI[m_cltCharStatus.GetKind()].m_siSelectEffect, m_ssX, m_ssY);
#endif
   }

   return TRUE;
}



BOOL cltCharOrg::IncreaseLife(SHORT amount)
{

	m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife() + amount);
	if(m_ParmeterBox.IP.GetLife() > m_ParmeterBox.GetMaxLife())
	{
		m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
	}


	// 체력이 올랐다는 사실을 서버에 알려준다. (온라인 전투인 경우. )
	if(GetPlayerNumber() == UserPlayerNumber)							// 사용자 유닛의 체력이 변동 되었을 때.
	{
		// 해킹 우려가 없을때에만 전송한다. 
		if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
		{
			OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetLife());
		}
	}
	
	return TRUE;
}

BOOL cltCharOrg::ReduceLife(SHORT amount, SHORT unique)
{

	// 방어막이 설치되어 있다면, 
	// 방어막이 먼저 손상된 후 체력이 저하된다. 
	if(m_siGuardianLife)
	{
		if(m_siGuardianLife >= (SHORT)((float)amount/2 + 0.5))
		{
			float tempLife;

			tempLife = (SHORT)((float)amount/2 + 0.5);
			m_siGuardianLife -= tempLife;
			amount = (SHORT)((float)amount/2 - 0.5);
		}
		else
		{
			amount-=m_siGuardianLife;
			m_siGuardianLife = 0;
		}

	}

	if(ShieldDelay)
	{

		if(ShieldDelay>=amount)
		{
			ShieldDelay-=amount;
			return TRUE;
		}
		else 
		{
			ShieldDelay=0;
			amount-=ShieldDelay;
		}
	}

	SI32 life = m_ParmeterBox.IP.GetLife();
	if(life >= amount)
	{
		m_ParmeterBox.IP.SetLife(life - amount);
	}
	else
		m_ParmeterBox.IP.SetLife(0);

	

	// 체력이 떨어졌다는 사실을 서버에 알려준다. (온라인 전투인 경우. )
	if(GetPlayerNumber() == UserPlayerNumber)							// 사용자 유닛의 체력이 변동 되었을 때.
	{
		// 무도장이 아니어야 한다. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			// 해킹 우려가 없을때에만 전송한다. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
				
				// 2차 장수가 죽는 경우 원래 장수가 등장해야 한다.
				if(m_ParmeterBox.IP.GetLife() == 0 && siCharType == 3)
				{
					// robypark 2004/6/10
					// 앙천대소인 상태에서 죽을 경우 애니메이션을 불랑기포차의 죽는 애니메이션을 사용해야 한다.
					if (m_cltCharStatus.GetKind() == KIND_ON_ROAR_OF_LAUGHTER)
					{
						// robypark 2004/6/14 15:37
						// 2차 장수가 죽고 난 뒤에 등장하는 1차 장수를 선택한 것으로 설정해야하는지 저장
						// 2차 장수를 사용자가 선택한 상태일 경우 1차 장수도 선택된 것으로 설정하기 위함.
						m_bAlreadySelectedBeforeDead = m_scSelectedSwitch;

						// 불랑기포차로 즉시 변신한다.
						CR[m_NewID.ID].ChangeModeFarangi_Immediately();
					}

					// robypark 2004/6/14 15:37
					// 2차 장수가 죽고 난 뒤에 등장하는 1차 장수를 선택한 것으로 설정해야하는지 저장
					// 2차 장수를 사용자가 선택한 상태일 경우 1차 장수도 선택된 것으로 설정하기 위함.
					m_bAlreadySelectedBeforeDead = m_scSelectedSwitch;

					m_scBusySwitch=TRUE;
					CurrentOrder = ORDER_DIE_GENERALEX;
					return FALSE;
				}
				else
					OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetLife());
				
			}
		}
	}

	if(m_ParmeterBox.IP.GetLife() == 0)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL cltCharOrg::IncreaseMana(SHORT amount)
{

	m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() + amount);

	if(m_ParmeterBox.IP.GetMana() > m_ParmeterBox.GetMaxMana())
	{
		m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
	}

	// 마법력이 떨어졌다는 사실을 서버에 알려준다. (온라인 전투인 경우. )
	if(GetPlayerNumber() == UserPlayerNumber)							// 사용자 유닛의 체력이 변동 되었을 때.
	{
		// 해킹 우려가 없을때에만 전송한다. 
		if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
		{
			OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetMana());
		}
	}

	return TRUE;
}

BOOL cltCharOrg::ReduceMana(SHORT amount)
{

	SI32 vary = m_ParmeterBox.IP.GetMana() - amount;

	if(vary < 0)
		return FALSE;

	m_ParmeterBox.IP.SetMana(vary);

	// 마법력이 떨어졌다는 사실을 서버에 알려준다. (온라인 전투인 경우. )
	if(GetPlayerNumber() == UserPlayerNumber)							// 사용자 유닛의 체력이 변동 되었을 때.
	{
		// 무도장이 아니어야 한다. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			// 해킹 우려가 없을때에만 전송한다. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetMana());
			}
		}
	}
	return TRUE;
}

// 캐릭터의 방향을 전환한다. 
BOOL cltCharOrg::ChangeDirectionNormal(SHORT destdir)
{
	Direction=destdir;
	TempDirection=destdir;

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);

	return TRUE;
}

// 캐릭터의 방향을 전환한다. 
BOOL cltCharOrg::ChangeDirection16(SHORT destdir)
{
	SHORT index=0;
	SHORT source_index=0;
	SHORT rotate_dir;
	SHORT dir[]={SOUTH, SSW, SOUTH|WEST, WWS, WEST, WWN, NORTH|WEST, NNW, 
		         NORTH, NNE, NORTH|EAST, EEN, EAST, EES, SOUTH|EAST, SSE };

	if(TempDirection==destdir)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}

	// 지연을 준다. 
	ChangeDirectionDelayor++;
	if(ChangeDirectionDelayor<ChangeDirectionDelay)
		return FALSE;
	else
		ChangeDirectionDelayor=0;

	m_scBusySwitch=TRUE;

	// 방향을 결정한다. 
	index=0;
	while(1)
	{
		if(dir[index]==TempDirection)
		{
			source_index=index;
			break;
		}
		index++;
	}

	index=0;
	while(1)
	{
		if(dir[(source_index+index)%16]==destdir)
		{
			break;
		}
		index++;
	}

	// 반시계 방향으로 회전 
	if(index>=8)
	{
        rotate_dir=FALSE;
	}
	else 
	{
		rotate_dir=TRUE;
	}


	if(rotate_dir==TRUE)
	{
	  TempDirection=dir[(source_index+1)%16];
	}
	else 
	{
		if(source_index-1==-1)
	       TempDirection=dir[15];
		else 
			TempDirection=dir[source_index-1];
	}
	
	if(TempDirection<1000)
	{
		Direction=TempDirection;
	}
	else
	{

	}

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);

	return FALSE;
}

BOOL cltCharOrg::ChangeDirection(SHORT destdir)
{
   return ChangeDirectionNormal(destdir);
}


// 유닛과 인접한 육지 섹터를 구한다. 
SHORT cltCharOrg::GetNearLandSector()
{
/*	SHORT i, j;

	SHORT startx=X-IDXsize;
	SHORT starty=Y-IDYsize;

	SHORT endx=startx+IDXsize-1;
	SHORT endy=starty+IDYsize-1;

	for(i=starty-1;i<=(endy+2);i++)
		for(j=startx-1;j<=(endx+2);j++)
		{

			if(clifMap.IsInMapArea(j, i)==FALSE)continue;

			if(clifMap.IsLand(j, i)==TRUE)
			{
			   return clifMap.GetSector(j, i);
			}
		}
*/
	return  0;

}

//지금 좌표와 다음 좌표간의 백분율 비 변위를 구한다. 
void cltCharOrg::GetMoveRatio(int startx, int starty, int endx, int endy, int movepercent, 
								SHORT& rx, SHORT& ry)
{

	endx-=startx;
	endy-=starty;
	startx=0;
	starty=0;

	SHORT index=0;
	POINT point[100];

	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	delta_x=endx-startx;
	delta_y=endy-starty;

	if(delta_x>0)       incx=1;
	else if(delta_x==0) incx=0;
	else               incx=-1;

	if(delta_y>0)       incy=1;
	else if(delta_y==0) incy=0;
	else               incy=-1;

	delta_x=abs(delta_x);
	delta_y=abs(delta_y);
	if(delta_x>delta_y)distance=delta_x;
	else distance=delta_y;


	for(t=0;t<=distance+1;t++)
	{
		if(index<100)
		{
			point[index].x=startx;
			point[index].y=starty;
			index++;
		}

		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
		   xerr-=distance;
		   startx+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			starty+=incy;
		}
	}

	int temp;
	if(index>0)
	{
	   temp=((index-1)*movepercent)/100;
	   rx=(SHORT)point[temp].x;
	   ry=(SHORT)point[temp].y;
	}
	else
	{
		rx=ry=0;
	}

}


// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
// length: 중심과의 거리 
BOOL cltCharOrg::IsAbsPtInObj(SHORT x, SHORT y, SHORT& length)
{

	if(RealRect.left <x && x<RealRect.right
	&& RealRect.top  <y && y<RealRect.bottom)
	{
		length=max(abs(m_cltCharStatus.GetCenterX()-x),abs(m_cltCharStatus.GetCenterY()-y));
		return TRUE;
	}

	return FALSE;
}


// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 
BOOL cltCharOrg::IsPtInObj(SHORT x, SHORT y)
{
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(m_ssDrawStartX+rect.left<x && x<m_ssDrawStartX+rect.right
	&& m_ssDrawStartY+rect.top<y && y<m_ssDrawStartY+rect.bottom)
		return TRUE;

	return FALSE;
}

// 어떤 rect와의 교점이 존재하는가?
BOOL cltCharOrg::IsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	RECT rect1, rect2, destrect;
	RECT realrect = m_cltCharDrawInfo.GetRealImageRect();
	
	rect1.left=x1;
	rect1.right=x2;
	rect1.top=y1;
	rect1.bottom=y2;


	rect2.left		=	m_ssDrawStartX	+	realrect.left;
	rect2.right		=	m_ssDrawStartX	+	realrect.right;
	rect2.top		=	m_ssDrawStartY	+	realrect.top;
	rect2.bottom	=	m_ssDrawStartY	+	realrect.bottom;

	if(IntersectRect(&destrect, &rect1, &rect2) )
		return TRUE;
	else return FALSE;

}

// 어떤 rect와의 교점이 존재하는가?
BOOL cltCharOrg::IsAbsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	RECT rect1, rect2, destrect;
	
	rect1.left=x1;
	rect1.right=x2;
	rect1.top=y1;
	rect1.bottom=y2;

	rect2=RealRect;
	
	if(IntersectRect(&destrect, &rect1, &rect2) )
		return TRUE;
	else return FALSE;

}

//-----------------------------------------------------------------------------------
// 출력 관련 
//-----------------------------------------------------------------------------------
// 야수의 혼 시간 설정 함수.

// 마비 설정 함수. 
void cltCharOrg::SetHalt(SI32 haltmode, SI32 haltdelay)
{
	//수호부[돼지]로 먹힌 캐릭터의 경우 포박이 걸리지 않는다.
	if(m_bPigDefence)
		return;

	SI16 siKind;
	
	siKind = TransKindImjinToOnline(CR[m_NewID.ID].m_cltCharStatus.GetKind());

	int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;


	//siMonsterType == 4 보스 몹이면서 포박술이 되지 않는 경우.
	if(siMonsterType == ON_MONSTER_HALT)
		return ;

	if(m_siHaltDelay == 0)
		m_siHaltStep	= 0;

	if(haltdelay > m_siHaltDelay)
	{
		m_siHaltDelay  = haltdelay;
		m_siHaltReason = haltmode;
	}
}

// 포박된 상태인 경우 이 장면을 보여준다. 
void cltCharOrg::DrawHalt()
{

	SHORT file;
	SHORT font;
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(m_siHaltDelay == 0)
		return ;	

	switch(m_siHaltReason)
	{
	case HALT_REASON_ICE:
		file=FILE_ON_FURHEADDEMON_SMOKE;
		font= m_siHaltStep;
		break;
	case HALT_REASON_IVY:
		file=FILE_MAGIC6CSTICK;
		font= (GD.Frame / 3 + m_NewID.ID) % 9;
		break;

	// robypark 2004-6-3 16:15
	// 천지독살 냉기에 의한 마비 smoke, 흑룡차 기본 공격에 의한 마비 smoke
	case HALT_REASON_FOG_OF_ICE:
		file = FILE_ON_FOG_OF_ICE_HIT;
		font = (GD.Frame / 3 + m_NewID.ID) % 9;
		break;

	default:
		SetMessage("3213767662");
		return ;
	}

	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);

	SHORT startx=(m_ssDrawStartX+rect.left+(rect.right-rect.left)/2)-xsize/2;
	SHORT starty=(m_ssDrawStartY+rect.top +(rect.bottom-rect.top)/2)-ysize/2;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);
	}

}



//-----------------------------------------------------------------------------
//  빈 자리 찾기 함수 
//-----------------------------------------------------------------------------
// 빈자리를 찾아 x, y에 넣어 준다.
BOOL cltCharOrg::FindEmptyArea(SHORT& x, SHORT &y)
{
   int i, j;
   SHORT tempx, tempy;

	  for(i=-1;i<=m_scIDYsize;i++)
		 for(j=-1;j<=m_scIDXsize;j++)
		 {
			 if(j==-1 || j==m_scIDXsize || i==-1 || i==m_scIDYsize)
			 {
				 tempx=m_ssX+j-m_scIDXsize/2;
				 tempy=m_ssY+i-m_scIDYsize/2;

				 // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			     if(Map.IsInMapArea(tempx, tempy)==TRUE 
				 && Map.IsEmpty(tempx, tempy))
				{
				   x=tempx;
				   y=tempy;
				   return TRUE;
				}
			 }
		 }
   return FALSE;
}

BOOL cltCharOrg::FindEmptyArea(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, BOOL diff_level_mode)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x, min_y;
   SHORT tempx, tempy;
   BOOL skipswitch ;

	  for(i=-4;i<=4;i++)
		  for(j=-4;j<=4;j++)
		  {
			  tempx=m_ssX+j;
			  tempy=m_ssY+i;
			  skipswitch = FALSE;
			  
			  // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			  if(Map.IsEmpty(tempx, tempy, xsize, ysize, m_ssX, m_ssY, m_scIDXsize, m_scIDYsize)==TRUE)
			  {
				  // 다른 층을 허용하지 않으면, 
				  if(diff_level_mode == FALSE)
				  {
					  // 건물과 같은 층의 지역이어야 한다. 
					  if(GD.LastLevel[m_ssX][m_ssY] != GD.LastLevel[tempx][tempy])
					  {
						  skipswitch = TRUE;
					  }
				  }
				  
				  if(skipswitch == FALSE)
				  {
					  length=(tempx-m_ssX)*(tempx-m_ssX)+(tempy-m_ssY)*(tempy-m_ssY);
					  
					  if(length < min_length)
					  {
						  min_x=tempx;
						  min_y=tempy;
						  min_length=length;
					  }
				  }
			  }
		  }

		 // 빈자리가 없으면, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // 빈자리가 있으면, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   return FALSE;
}

BOOL cltCharOrg::FindEmptyAreaTreasure(SHORT& x, SHORT &y)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x, min_y;
   SHORT tempx, tempy;

   for(i=-4;i<=4;i++)
	   for(j=-4;j<=4;j++)
	   {
		   tempx=m_ssX+j;
		   tempy=m_ssY+i;
		   
		   // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
		   // 이미 보물이 배치된 곳이어도 안된다.
		   if(Map.IsEmpty(tempx, tempy, 1, 1, m_ssX, m_ssY, m_scIDXsize, m_scIDYsize)==TRUE
			   && Map.GetTreasureMap(tempx,tempy) == 0)
		   {
			   
			   length=(tempx-m_ssX)*(tempx-m_ssX)+(tempy-m_ssY)*(tempy-m_ssY);
			   
			   if(length < min_length)
			   {
				   min_x=tempx;
				   min_y=tempy;
				   min_length=length;
			   }
		   }
	   }
	   
	   // 빈자리가 없으면, 
	   if(min_length==10000)
	   {
		   return FALSE;
	   }
	   else  // 빈자리가 있으면, 
	   {
		   x=min_x; y=min_y;
		   return TRUE;
	   }
   return FALSE;
}


// 불 붙은 자리에 있을때의 대처 방안. 
BOOL cltCharOrg::FireOperation(int x,int y)
{

	// 방어력에 비례하여 피해를 준다. 
	// 아무리 방어력이 높은 캐릭터라도 데미지가 상당하다. 

	// 가장 가까운 적에 의해 죽임을 당한 것으로 처리한다. 
	SI32 min_length = 10000;
	SI32 min_enemy_id = -1;
	SI16 siPlayerID;
	_NewID enemyid;
	
	/*
	for(i = 1;i<MAX_CHAR_NUMBER;i++)
	{
		if(IsAlive(i) 
		&& IsFriendToByPlayerNumber(CR[i].GetPlayerNumber(), GetPlayerNumber()) == FALSE)
		{
			length = max(abs(CR[i].GetX() - X), abs(CR[i].GetY() - Y));
			if(length < min_length)
			{
				min_length = length;
				min_enemy_id = i;
			}
		}
	}

	if(min_enemy_id > 0)
	{
		enemyid = CR[min_enemy_id].m_NewID;
	}
	else
	{		enemyid = ZeroID;
	}*/

	enemyid =  Map.GetFireCharMap(x,y);
	
	siPlayerID = Map.GetFirePlayerMap(x,y);

//	if((IsFriendToByPlayerNumber(siPlayerID, GetPlayerNumber()) == TRUE) || Map.m_bFire == FALSE 
	// 플레이어는 바닥 불에 의해 데미지를 전혀 입지않는다. 심의상 문제
	if((IsFriendToByPlayerNumber(siPlayerID, GetPlayerNumber()) == TRUE)  
		|| ISPLAYER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)
		return TRUE;
	
	
	SI32 apower = CalcApowerByAtb(max(40, m_ParmeterBox.GetTotalDpower() * 3 / 10), ATTACKATB_FIRE, m_uiAtb);

	CalculateHit(enemyid, siPlayerID, ATTACKATB_FIRE, 0, AREA_LAND, apower, m_NewID, m_ssX, m_ssY, 70/*확률*/);

	return TRUE;
}


//------------------------------------------------------------------------------
// 인공지능 관련. 
//-----------------------------------------------------------------------------

void cltCharOrg::BlackCheck()
{
	if(IsAtb(ATB_CIRCLEBALCKCHECK))
	{
		BlackCheckNormal();		
	}
	else
	{
		BlackCheckHeroMode();
	}
}

void cltCharOrg::BlackCheckNormal()
{

	// 유효한 지도의 영역에 있는 것이 아니면, 
	if( Map.IsInMapArea(m_ssX, m_ssY) == FALSE )	return;
	
	GlobalBlackCheck(m_ssX, m_ssY, GetTotalSearchRange(), TRUE, GetPlayerNumber());
	
}

// 시야를 처리한다 - 일기토 모드 게임
void cltCharOrg::BlackCheckHeroMode()
{
	DWriteStringToFile("11");
	SI32	x, y, x2, y2, i, j;
	BOOL	bBreak;
	SI32	angle = 45;
	SI32	cosValue, sinValue;

	// 세 점의 좌표
	POINT	basisPos[3] = {{-1, -1}, {-1, -1}, {-1, -1}};
	SHORT	edgePos[MAX_MAP_XSIZE];
	SHORT	tempPos[MAX_MAP_XSIZE * MAX_MAP_YSIZE];
	SI32	tempX, tempY;
	SHORT	edgePosNum1, edgePosNum2;	// basisPos[0]과 [1]사이에 있는 점의 개수, [1]~[2]
	SHORT	tempPosNum;	

	// 유효한 지도의 영역에 있는 것이 아니면, 
	if(Map.IsInMapArea(m_ssX, m_ssY) == FALSE) return;

	cosValue = (SI32)((DOUBLE)GetTotalSearchRange() * cos(angle * (3.14 / 180)));
	sinValue = (SI32)((DOUBLE)GetTotalSearchRange() * sin(angle * (3.14 / 180)));

	DWriteStringToFile("22");
	switch(Direction)
	{
		case SOUTH:		
			tempX = m_ssX, tempY = m_ssY - 1;

			basisPos[0].x = tempX + sinValue;
			basisPos[0].y = tempY + cosValue;

			basisPos[1].x = tempX;
			basisPos[1].y = tempY + GetTotalSearchRange();

			basisPos[2].x = tempX - sinValue;
			basisPos[2].y = tempY + cosValue;
			break;

		case SOUTH | EAST:	
			tempX = m_ssX - 1, tempY = m_ssY - 1;

			basisPos[0].x = tempX + GetTotalSearchRange();
			basisPos[0].y = tempY;

			basisPos[1].x = tempX + cosValue;
			basisPos[1].y = tempY + sinValue;

			basisPos[2].x = tempX;
			basisPos[2].y = tempY + GetTotalSearchRange();
			break;

		case EAST:	
			tempX = m_ssX - 1, tempY = m_ssY;

			basisPos[0].x = tempX + cosValue;
			basisPos[0].y = tempY - sinValue;

			basisPos[1].x = tempX + GetTotalSearchRange();
			basisPos[1].y = tempY;

			basisPos[2].x = tempX + cosValue;
			basisPos[2].y = tempY + sinValue;
			break;

		case NORTH | EAST:	
			tempX = m_ssX - 1, tempY = m_ssY + 1;

			basisPos[0].x = tempX;
			basisPos[0].y = tempY - GetTotalSearchRange();

			basisPos[1].x = tempX + sinValue;
			basisPos[1].y = tempY - cosValue;

			basisPos[2].x = tempX + GetTotalSearchRange();
			basisPos[2].y = tempY;
			break;

		case NORTH:	
			tempX = m_ssX, tempY = m_ssY + 1; 

			basisPos[0].x = tempX - sinValue;
			basisPos[0].y = tempY - cosValue;

			basisPos[1].x = tempX;
			basisPos[1].y = tempY - GetTotalSearchRange();

			basisPos[2].x = tempX + sinValue;
			basisPos[2].y = tempY - cosValue;
			break;

		case NORTH | WEST:	
			tempX = m_ssX + 1, tempY = m_ssY + 1;

			basisPos[0].x = tempX - GetTotalSearchRange();
			basisPos[0].y = tempY;

			basisPos[1].x = tempX - sinValue;
			basisPos[1].y = tempY - cosValue;

			basisPos[2].x = tempX;
			basisPos[2].y = tempY - GetTotalSearchRange();
			break;

		case WEST:	
			tempX = m_ssX + 1, tempY = m_ssY;

			basisPos[0].x = tempX - cosValue;
			basisPos[0].y = tempY + sinValue;

			basisPos[1].x = tempX - GetTotalSearchRange();
			basisPos[1].y = tempY;

			basisPos[2].x = tempX - cosValue;
			basisPos[2].y = tempY - sinValue;
			break;

		case SOUTH | WEST:	
			tempX = m_ssX + 1, tempY = m_ssY - 1;

			basisPos[0].x = tempX;
			basisPos[0].y = tempY + GetTotalSearchRange();

			basisPos[1].x = tempX - sinValue;
			basisPos[1].y = tempY + cosValue;

			basisPos[2].x = tempX - GetTotalSearchRange();
			basisPos[2].y = tempY;
			break;

		default:
			break;
	}
	DWriteStringToFile("33");

	// 목적지까지의 경로를 구한다. 
	FindStraightLine((SHORT)basisPos[0].x, (SHORT)basisPos[0].y, (SHORT)basisPos[1].x, (SHORT)basisPos[1].y, 
		edgePosNum1, edgePos, MAX_MAP_XSIZE * MAX_MAP_YSIZE);
	FindStraightLine((SHORT)basisPos[1].x, (SHORT)basisPos[1].y, (SHORT)basisPos[2].x, (SHORT)basisPos[2].y, 
		edgePosNum2, &edgePos[edgePosNum1 * 2], MAX_MAP_XSIZE * MAX_MAP_YSIZE);
	DWriteStringToFile("44");

	// 시야를 표현하는 모든 직선들
	for(i = 0; i < (edgePosNum1 + edgePosNum2) * 2; i += 2)
	{
		FindStraightLine(tempX, tempY, edgePos[i], edgePos[i + 1], tempPosNum, tempPos, MAX_MAP_XSIZE * MAX_MAP_YSIZE);
		
		bBreak = FALSE;
		for(j = 0; j < (tempPosNum * 2); j += 2)
		{
			// 현재 점의 주변(4개 점들) 모두 검사
			for(y = tempPos[j + 1] - 1; y <= tempPos[j + 1]; y++)
			{
				for(x = tempPos[j] - 1; x <= tempPos[j]; x++)
				{
					if((max(abs(x - m_ssX), abs(y - m_ssY)) > 2) && ((IsColl(x, y, BLOCK_CASTLE | BLOCK_WALL | BLOCK_TREE) == TRUE) || (GD.LastLevel[x][y] >= GD.LastLevel[m_ssX][m_ssY] + 2))) 
					{
						// 장애물 주변을 밝게 한다. (포그 그라데이션에 의해 어둡게 나타나는 현상을 방지하기 위해)
						for(y2 = y - 1; y2 <= y + 1; y2++)
						{
							for(x2 = x - 1; x2 <= x + 1; x2++)
							{
								if(Map.IsInMapArea(x2, y2) == TRUE)
								{
									// 내가 호의적인 플레이어의 포그도 같이 밝힌다.
									for(SI32 temp_player = 0; temp_player < MAX_MULTI_PLAYER; temp_player++)
									{
										if(IsFriendToByPlayerNumber(GetPlayerNumber(), temp_player) == TRUE)
										{
											if(GD.BlackMap[temp_player][x2][y2] != 0)
											{
												GD.BlackMap[temp_player][x2][y2] = 0;
											}
										}
									}
								}
							}
						}

						bBreak = TRUE;
					}
					else
					{
						if(Map.IsInMapArea(x, y) == TRUE)
						{
							// 내가 호의적인 플레이어의 포그도 같이 밝힌다.
							for(SI32 temp_player = 0; temp_player < MAX_MULTI_PLAYER; temp_player++)
							{
								if(IsFriendToByPlayerNumber(GetPlayerNumber(), temp_player) == TRUE)
								{
									if(GD.BlackMap[temp_player][x][y] != 0)
									{
										GD.BlackMap[temp_player][x][y] = 0;
									}
								}
							}
						}
					}
				}
			}

			if(bBreak)
			{
				break;
			}
		}
	}
	DWriteStringToFile("55");
}



// 전방 탐사 :근처에 적이 있는지 여부를 조사한다. 
BOOL cltCharOrg::SearchForward()
{

	SHORT		i;
	SHORT		id=0;
	SHORT		length=0;
	
	
	SHORT		x=0, y=0;

	SHORT		search_range;

	if( CanWork() == FALSE )	return FALSE;

	search_range = GetTotalSearchRange();


	// 위기에 처한 아군을 도울 수 있는 범위. 
	SI32 helprange = 0 ;

	if(GD.Player[GetPlayerNumber()].PI.Host == BY_COMPUTER)
	{
		helprange = search_range; 
	}
	else
	{
		helprange = 4;
	}


	// 지상 수색 
    SHORT minx=max(0, m_ssX-search_range);
    SHORT miny=max(0, m_ssY-search_range);
    SHORT maxx=min(Map.GetXsize()-1, m_ssX+search_range);
    SHORT maxy=min(Map.GetYsize()-1, m_ssY+search_range);

	for(i = 0; i < GD.CharOrderIndex;i++)
	{
		id = GD.CharOrder[i];
		
		if( IsSee(id) )
		{
			x = clifChar.GetX(id);
			y = clifChar.GetY(id);
			
			if( id!=AttackID.ID && GD.BlackMap[GetPlayerNumber()][x][y] == 0 
				&& x>=minx && x<=maxx && y>=miny && y<=maxy )
			{
				// 발견한 놈이 적이고 매복중인 것이 아니면, (적의 발견은 50%의 확률로..)
				if( IsFriendToByID(m_NewID.ID, id)==FALSE && (GD.Random() % 2) )
				{
					// 거리가 가까우면 (바로 옆칸) 보인다. 
					if(max(abs(m_ssX-x) , abs(m_ssY-y)) <= 1 ||  CR[id].AmbushSwitch == FALSE)
					{
						// 적이 나에게 보이게 만든다.
						CR[id].SetDiscover(GetPlayerNumber());
						
						if(OperateFindEnemy(x, y, CR[id].m_NewID)==TRUE)return 0;
					}
				}
				// 도움이 필요한 아군 캐릭터라면, 
				else if(max(abs(m_ssX-x) , abs(m_ssY-y)) <= helprange
					&& abs(CR[id].BeAttackedFrame-GD.Frame) < 200 
					&& IsSee(CR[id].PrevAttackMeID)==TRUE)
				{
					// 아군을 공격하는 놈은 없앤다. 
					if(IsFriendToByID(m_NewID.ID, CR[id].PrevAttackMeID.ID) == FALSE)
					{
						if(OperateFindEnemy(CR[ CR[id].PrevAttackMeID.ID ].m_ssX, 
							CR[ CR[id].PrevAttackMeID.ID ].m_ssY, 
							CR[id].PrevAttackMeID)==TRUE)return 0;
					}
				}
			}
		}
	}

	return 0;
}

// 일을 할 준비가 되어 있는가?
BOOL cltCharOrg::CanWork()
{

   // 사라져 있는 것이라면, 
   if(m_scSeeSwitch==FALSE)return FALSE;

   return TRUE;
}


// 적을 발견했을 경우 어떻게 대처하는지 결정한다. 
// TRUE:적을 발견하고 적절한 조치를 취했다. 
// FALSE:적에게 조치를 취할 수 없는 경우이다. 
BOOL cltCharOrg::OperateFindEnemy(SHORT x, SHORT y, _NewID id)
{

	SHORT length=0;

	if(m_scSeeSwitch==FALSE)return FALSE;

	if(IsSee(id.ID)==FALSE)return FALSE;

	// 발견한 캐리터에게 내가 적대적인 설정이 아니면. 
	if(IsFriendToByID(m_NewID.ID, id.ID) == TRUE)return FALSE;


	// 사수 모드가 설정되어 있다면 사거리 밖의 캐릭터는 공격할 수 없다. 
	if(HoldSwitch==TRUE)
	{
		if(IsInRange(id.ID, GetAttackRange())==FALSE)return FALSE;
	}

	// 현재 대기 상태이면, 
	if(CurrentOrder==ORDER_WAIT)
	{

			if(CanAttack(id)==TRUE)
			{
				if(OldOrder.Order==ORDER_WAIT)
				{
					SetOldOrder(ORDER_MOVE, HostMode, FALSE, m_ssX|(m_ssY<<16), ZeroID);
				}
			   
				OrderAttack(m_NewID, x, y, id, BY_COMPUTER);
			   return TRUE;
			}
	}
 
	// 현재 공격중이고 
	// 컴퓨터 플레이어이거나 컴퓨터 명령이면, 
	if(CurrentOrder==ORDER_ATTACK 
	&& (GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER
	    || HostMode==BY_COMPUTER)  )
	{
		// 새로운 적을 공격할 수 있다면, 		
		if(CanAttack(id)==TRUE)
		{
			// 새로운 적을 공격하는 것이 더 적절하다면 새로운 적을 공격한다. 
			if(IsGoodAttackNewEnemy(id)==TRUE)
			{
			   OrderAttack(m_NewID, x, y, id, BY_COMPUTER);
			   return TRUE;
			}
		}
	}
	// 자체적으로 이동중에 적을 발견하면, 
	// 순찰중에 만나면, 
	else if( (CurrentOrder == ORDER_MOVE && HostMode == BY_COMPUTER)
		   || CurrentOrder == ORDER_PATROL)                         
	{

		if(CanAttack(id)==TRUE)
		{

			if(OldOrder.Order==ORDER_WAIT)
			{
				SetOldOrder(ORDER_MOVE, HostMode, FALSE, DX|(DY<<16), ZeroID);
			}

			 OrderAttack(m_NewID, x, y, id, BY_COMPUTER);
					
			return TRUE;
		}
	}

	return FALSE;
}


BOOL cltCharOrg::IsInRange(SHORT id, SHORT range)
{

	SHORT length;
	
	if(IsSee(id)==FALSE)return FALSE;
	
	
	// 목표의 좌표 
	SHORT dx=CR[id].m_ssX;
	SHORT xsize=CR[id].m_scIDXsize;
	SHORT dy=CR[id].m_ssY;
	SHORT ysize=CR[id].m_scIDYsize;
	
	SHORT lengthx, lengthy;
	
	if( range == 0 )
	{
		if(m_ssX == dx && m_ssY == dy)return TRUE;
		else return FALSE;
	}
	else if(range==1)
	{
		if(m_ssX > dx)
		{
			lengthx=abs(m_ssX- (dx- xsize/2+xsize-1) );
		}
		else if(m_ssX < dx)
		{
			lengthx=abs((dx- xsize/2)- m_ssX );
		}
		else lengthx=0;
		
		if(m_ssY > dy)
		{
			lengthy=abs(m_ssY - (dy- ysize/2+ysize-1) );
		}
		else if(m_ssY < dy)
		{
			lengthy=abs((dy- ysize/2) - m_ssY );
		}
		else lengthy=0;
		
		length=max(lengthx, lengthy);
		
		if(length<=range)
		{
			return TRUE;
		}
	}
	else
	{
		// 45*45=2025  한 타일의 단위를 45도트로 설정했다. 
		
		if((range*range*2025) >  ( (CR[id].m_cltCharStatus.GetCenterX()-m_cltCharStatus.GetCenterX())*(CR[id].m_cltCharStatus.GetCenterX()-m_cltCharStatus.GetCenterX()) + (CR[id].m_cltCharStatus.GetCenterY() - m_cltCharStatus.GetCenterY())*(CR[id].m_cltCharStatus.GetCenterY()-m_cltCharStatus.GetCenterY() )))return TRUE;
		return FALSE;
	}
	
	
	
	return FALSE;
}


// id캐릭터를 공격할 수 있는지 여부를 알려준다. 
BOOL cltCharOrg::CanAttack(_NewID id)
{
	// 내 자신이 볼수 없는 상태라면, 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// 화면에 볼 수 없는 캐릭터라면 공격할 수 없다. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// 자기 자신은 공격할 수 없다.
	if(id.IsSame(m_NewID))return FALSE;

	// 지상 공격 속성이 없고 지상캐릭터라면 공격할 수 없다. 
	if(IsAtb(ATB_ATTACK)==FALSE && CR[id.ID].m_cltCharStatus.GetArea()==AREA_LAND)return FALSE;

	
	// 공중 공격 속성이 없고 공중 캐릭터라면, 
	if(IsAtb(ATB_ATTACKAIR)==FALSE && CR[id.ID].m_cltCharStatus.GetArea()==AREA_AIR)return FALSE;

	// 수영하는 유닛이고 지금 물속에 있다면 공격 불가. 
	if(SwimSwitch == TRUE)return FALSE;


	// 거미요괴 거미줄 공격이고 마법이 부족하면, 공격불가. 
	if(GetAttackAtb() == ATTACKATB_WIDOW && m_ParmeterBox.IP.GetMana() < MAGICPOWER_WIDOW)return  FALSE;

	// 파동공격이고 마법이 부족하면, 공격불가. 
	if(GetAttackAtb() == ATTACKATB_WAVE  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_WAVE)return  FALSE;

	// 마법을 소모하는 불공격이고 마법이 부족하면, 공격불가. 불가사리
	if(GetAttackAtb() == ATTACKATB_FIREUSINGMANA  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_FIRE)return  FALSE;

	// 거대수 나무가지 공격
	if(GetAttackAtb() == ATTACKATB_BIGTREE  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_BIGTREE)return  FALSE;

	// robypark 2004/9/1 15:14
	// 기본 공격속성 중에 폭뢰격 공격을 하는 경우 마나 검사
	// 신검의 혼, 유령 사무라이, 유령 불꽃
	if ((GetAttackAtb() == ATTACKATB_KIMBOMB) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_KIMBOMB))
		return FALSE;

	// robypark 2004/9/1 15:47
	// 기본 공격속성 중에 흑룡대차륜 공격을 하는 경우 마나 검사
	// 징기스칸의 혼, 문지기
	if ((GetAttackAtb() == ATTACKATB_MAGIC4T) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_MAGIC4T))
		return FALSE;

	// robypark 2004/11/11 11:20
	// 앙천대소 공격일 때 마나소모하도록 수정이 되었다. 공격에 필요한 마나가 남았는지 검사
	if ((GetAttackAtb() == ATTACKATB_ROAR_OF_LAUGHTER) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_SIEGE_MODE_FIRE_MISSILE))
		return FALSE;

	//수호부(뱀)의 공격일 때 상대방의 마나가 없으면 다른 상대를 찾는다.
	if( (GetAttackAtb() == ATTACKATB_GUARDIAN_SNAKE) && (CR[id.ID].m_ParmeterBox.IP.GetMana() == 0)) return FALSE;

	if( CR[id.ID].m_bPigDefence == TRUE)	return FALSE;

	return TRUE;
}


void cltCharOrg::SetOldOrder(SHORT order, SHORT hostmode, SHORT clearswitch,
							int para1, _NewID para2)
{
   OldOrder.Order=order;
   OldOrder.HostMode=(char)hostmode;
   OldOrder.ClearSwitch=(char)clearswitch;
   OldOrder.Para1=para1;
   OldOrder.Para2=para2;
}

// 현재 공격중인 적과 id중에서 id를 공격하는 것이 더 좋겠는가?
BOOL cltCharOrg::IsGoodAttackNewEnemy(_NewID id)
{

	if(IsSee(id.ID)==FALSE)return FALSE;
	if(IsSee(AttackID.ID)==FALSE)return TRUE;

	// 지금 공격하는 캐릭터가 나를 공격할 수 없는데 마비되어 있고 새로운 캐릭터가 나를 공격할 수 있는  캐릭터라면
	// 새로운 캐릭터를 공격한다. 
	if(CR[AttackID.ID].CanAttack(m_NewID)==FALSE || CR[AttackID.ID].m_siHaltDelay)
	{
		if(CR[id.ID].CanAttack(m_NewID)==TRUE)
		{
		   return TRUE;
		}
	}
	// 현재 공격하는 캐릭터가 나를 공격할 수 있다면, 
	else
	{
		// 새로운 캐릭터가 나를 공격할 수 없다면, 
		if(CR[id.ID].CanAttack(m_NewID)==FALSE)
		   return FALSE;
	}

	// 현재 공격중인 캐릭터가 비무장인데 새로운 캐릭터가 무장캐릭터라면, 
	if(CR[AttackID.ID].IsAtb(ATB_ATTACK|ATB_ATTACKAIR)==FALSE
	&& CR[id.ID].IsAtb(ATB_ATTACK|ATB_ATTACKAIR)==TRUE)return TRUE;

	// 내가 쫓는 캐릭터보다 새로운 캐릭터가 가까우면, (사거리 안에 있으면,)
	if(IsInRange(AttackID.ID, GetAttackRange())==FALSE && IsInRange(id.ID, GetAttackRange())==TRUE)
	{
		return TRUE;
	}

	// 새로운 캐릭터와의 거리가 기존 캐릭터의 거리보다 작으면 TRUE
	SI32 length1 = max( abs(CR[id.ID].m_ssX-m_ssX), abs(CR[id.ID].m_ssY-m_ssY));
	SI32 length2 = max( abs(CR[AttackID.ID].m_ssX-m_ssX), abs(CR[AttackID.ID].m_ssY-m_ssY));
	if(length1 < length2)
	{
		return TRUE;
	}


	return FALSE;

}

// id캐릭터를 공격할 수 있는지 여부를 알려준다. 
BOOL cltCharOrg::CanAttack(SHORT x, SHORT y)
{
	// 내 자신이 볼수 없는 상태라면, 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// 지상 공격 속성이 없으면 공격할 수 없다. 
	if(IsAtb(ATB_ATTACK)==FALSE)return FALSE;

	
	return TRUE;
}

//--------------------------------------------------------------------------------------------
// 애니메이션 관련. 
//--------------------------------------------------------------------------------------------
void cltCharOrg::SetAnimation(SI32 ani, SI32 *panistep)
{
	m_cltCharDrawInfo.SetAnimation(ani);

	m_cltCharDrawInfo.SetpAniStep(panistep);

	// 조합형 캐릭터일 경우
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		// 캐릭터의 행동 결정(AniAction)
		switch( ani )
		{
		case ANI_WAIT0:		AniAction	= HERO_ACT_N1;		break;
		case ANI_MOVE:		AniAction	= HERO_ACT_WL;		break;
		case ANI_ATTACK:	AniAction	= HERO_ACT_A1;		break;
		case ANI_DYING:		AniAction	= HERO_ACT_DT;		break;
		default:			clGrp.Error("DecideFontHero", "Unknown Animation : [%d]", ani);		break;
		}
	}

	SI32 oldanivary = m_cltCharDrawInfo.GetAniVary();
	switch(ani)
	{
	case ANI_MOVE:
		m_cltCharDrawInfo.SetAniVary(MoveCharAnimationIndex);
		break;
	case ANI_LOWMOVE:
		m_cltCharDrawInfo.SetAniVary(LowMoveCharAnimationIndex);
		break;
	case ANI_SWIM:
		m_cltCharDrawInfo.SetAniVary(SwimCharAnimationIndex);
		break;
	case ANI_READYTOFIRE:
		m_cltCharDrawInfo.SetAniVary(ReadyToFireCharAnimationIndex);
		break;
	case ANI_LOWATTACK:
		m_cltCharDrawInfo.SetAniVary(LowAttackCharAnimationIndex);
		break;
	case ANI_ATTACK:
		m_cltCharDrawInfo.SetAniVary(AttackCharAnimationIndex);
		break;
	case ANI_ATTACK2:
		m_cltCharDrawInfo.SetAniVary(Attack2CharAnimationIndex);
		break;
	case ANI_SWIM_WAIT:
		m_cltCharDrawInfo.SetAniVary (SwimWaitCharAnimationIndex);
		break;
	case ANI_WAIT0:
		m_cltCharDrawInfo.SetAniVary(Wait0CharAnimationIndex);
		break;
	case ANI_WAIT1:
		m_cltCharDrawInfo.SetAniVary(Wait1CharAnimationIndex);
		break;
	case ANI_DYING:
		m_cltCharDrawInfo.SetAniVary(DyingCharAnimationIndex);
		break;
	case ANI_AMBUSH:
		m_cltCharDrawInfo.SetAniVary(AmbushCharAnimationIndex);
		break;
	case ANI_INGROUND:
		m_cltCharDrawInfo.SetAniVary(InGroundCharAnimationIndex);
		break;
	case ANI_OUTGROUND:
		m_cltCharDrawInfo.SetAniVary(OutGroundCharAnimationIndex);
		break;
	case ANI_MAGIC:
		m_cltCharDrawInfo.SetAniVary(MagicCharAnimationIndex);
		break;
	case ANI_MAGIC2:
		m_cltCharDrawInfo.SetAniVary(Magic2CharAnimationIndex);
		break;
		
	default:
		clGrp.Error("Fsdf39", "fsd38y222:%d", ani);
	}

	if(oldanivary != m_cltCharDrawInfo.GetAniVary())
	{
		m_cltCharDrawInfo.SetUpdateAnimationSwitch (TRUE);
	}

}


BOOL cltCharOrg::DoAnimationDelay(BOOL animode, BOOL atb)
{
	SI32	delay;
	SI32	framenumber;

	// 조합형 캐릭터일 경우
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		// 무기에 따라 행동할 수 있는 동작 클래스명을 얻어온다.
		framenumber  = g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalFrame[AniAction][CharBaseWClass][CharWeaponEnum]; 
	}
	else
	{
		framenumber = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].GetFrameNumber();
	}
	delay		= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].siDelay;

	m_cltCharDrawInfo.SetAniDelayor(m_cltCharDrawInfo.GetAniDelayor()+1);

	if(m_cltCharDrawInfo.GetpAniStep())
	{
		m_cltCharDrawInfo.SetAniStep( *m_cltCharDrawInfo.GetpAniStep() );
	}
	
	if(m_cltCharDrawInfo.GetAniDelayor() >= delay)
	{
		m_cltCharDrawInfo.SetAniDelayor (0);


		// 한번만 애니메이션 하게 되어 있는 경우, 
		// 프레임이 끝까지 갔다면, 
		// 더이상 애니메이션을 돌리지 않는다. 
		if((atb & ANI_ATB_ONCE) && m_cltCharDrawInfo.GetAniStep() == (framenumber - 1))
		{
			return TRUE;
		}
		else
		{
			
			if(animode == TRUE)
			{
				m_cltCharDrawInfo.SetAniStep(m_cltCharDrawInfo.GetAniStep()+1);
			}
			else
			{
				m_cltCharDrawInfo.SetAniStep(m_cltCharDrawInfo.GetAniStep()-1);
			}
			
			if(framenumber == 0)
			{
				m_cltCharDrawInfo.SetAniStep( 0);
			}
			else
			{
				m_cltCharDrawInfo.SetAniStep(m_cltCharDrawInfo.GetAniStep() % framenumber);
			}
			
		}

		if(m_cltCharDrawInfo.GetpAniStep())
		{
			*m_cltCharDrawInfo.GetpAniStep() = m_cltCharDrawInfo.GetAniStep();
		}

		
		m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
		return TRUE;
	}

	return FALSE;
}


SI32 cltCharOrg::GetAniInstant()
{
	// 조합형 캐릭터일 경우
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		return g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalFrame[AniAction][CharBaseWClass][CharWeaponEnum] - 2;
	}
	else
		return KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].siInstant;
}


// 공중에서 빈자리를 찾는다. 
BOOL cltCharOrg::FindEmptyAreaAir(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x, min_y;
   SHORT tempx, tempy;

	  for(i=-4;i<=4;i++)
		 for(j=-4;j<=4;j++)
		 {
			 tempx=m_ssX+j;
			 tempy=m_ssY+i;
				 
			 // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			     if(Map.IsInMapArea(tempx, tempy)==TRUE && clifMap.GetSkyMap(tempx, tempy)==0)
				 {
			        length=(tempx-m_ssX)*(tempx-m_ssX)+(tempy-m_ssY)*(tempy-m_ssY);

			        if(length<min_length)
					{
				       min_x=tempx;
				       min_y=tempy;
				       min_length=length;
					}
				 }
		 }

		 // 빈자리가 없으면, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // 빈자리가 있으면, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   return FALSE;
}

BOOL cltCharOrg::FindEmptyAreaWater(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x, min_y;
   SHORT tempx, tempy;

	  for(i=-8;i<=8;i++)
		 for(j=-8;j<=8;j++)
		 {
			 tempx=m_ssX+j;
			 tempy=m_ssY+i;
				 // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			     if(Map.IsEmptyWater(tempx, tempy, xsize, ysize, m_ssX, m_ssY, m_scIDXsize, m_scIDYsize)==TRUE)
				 {
			        length=(tempx-m_ssX)*(tempx-m_ssX)+(tempy-m_ssY)*(tempy-m_ssY);

			        if(length<min_length)
					{
				       min_x=tempx;
				       min_y=tempy;
				       min_length=length;
					}
				 }
		 }

		 // 빈자리가 없으면, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // 빈자리가 있으면, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	SetEquip()
// Description	:	캐릭터에게 아이템을 장착시킨다.
// Parameter	:	bEquip			- FALSE이면 장착해제
//					siEquipKind		- 장착부위
//					siEquipID		- 아이템 아이디, -1일 경우 디폴트 아이템 착용
//					bEffectSound	- 장착시 효과음을 낼 것인가 
BOOL cltCharOrg::SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound)
{
	BOOL bSet = FALSE;

	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE ) return FALSE;
	if( siEquipID == 0 )	return FALSE;


	switch( siEquipKind )
	{
	case ON_WEAR_ARMOR_ITEM:
		bSet = TRUE;
		if( bEquip == FALSE || siEquipID == -1 )
		{
			siEquipID = KI[m_cltCharStatus.GetKind()].m_ssDefaultArmor;
		}

		m_siArmorDrawID		= siEquipID;
		if( m_siArmorDrawID == m_siPrevArmorDrawID )	return FALSE;
		m_siPrevArmorDrawID	= m_siArmorDrawID;	
		
		break;

	case ON_WEAR_HELMET_ITEM:
		bSet = TRUE;
		if( bEquip == FALSE || siEquipID == -1 )
		{
			siEquipID = KI[m_cltCharStatus.GetKind()].m_ssDefaultHelmet;
		}
		m_siHelmetDrawID		= siEquipID;
		if( m_siHelmetDrawID == m_siPrevHelmetDrawID )	return FALSE;
		m_siPrevHelmetDrawID	= m_siHelmetDrawID;	

		break;

	case ON_WEAR_WEAPON_ITEM:

		if( bEquip == FALSE || siEquipID == -1 )
		{
			siEquipID = KI[m_cltCharStatus.GetKind()].m_ssDefaultWeapon;
			bSet = TRUE;
		}
		// 그림이 다 있지 않으므로.
		else
		{
			bSet = TRUE; 	
		}

		m_siWeaponDrawID		= siEquipID;
		if( m_siWeaponDrawID == m_siPrevWeaponDrawID )	return FALSE;
		m_siPrevWeaponDrawID	= m_siWeaponDrawID;	

		break;

	default:	
		clGrp.Error("fsd39743jf", "fsd9734:%d", siEquipKind);
		return FALSE;
	}

	if( bEffectSound == TRUE )
		PushEffect(EFFECT_ON_ITEMONOFF, 0, 0);
	if( bSet )
	{

		g_FileLoadManager.SetChange(siEquipKind, siEquipID, KI[m_cltCharStatus.GetKind()].m_ssHeroID, m_NewID.ID, DefaultBody,TRUE);
		
	}

	// CharBaseWClass 설정
	CharBaseWClass	= g_FileLoadManager.GetHeroClassCode(&g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID]);	
	return TRUE;
}



// 캐릭터의 경험치를 올려주고 전투 서버에 결과를 통보한다. 
void cltCharOrg::IncreaseExp(SI32 exp, BOOL serversendswitch, BOOL showresultswitch)
{
	// 사람 플레이어인경우에만 경험치가 오르게 된다. 
	if(ISPLAYER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)
	{

		
		// 서버로 경험치를 전송하는 경우에만, 
		if(serversendswitch == TRUE)
		{
			// 사용자와 같은 캐릭터인 경우에만, 
			if(GetPlayerNumber() == UserPlayerNumber)
			{
				// 해킹 우려가 없을때에만 전송한다. 
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
//					for(int i = 0 ; i < 100 ; i++)
//					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, exp);
//					}
				}
				else
				{
					SetMessage("해킹의 가능성이 있는 데이터입니다.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}
			}
		}

		// 경험치 상승의 결과 레벨업이 이루어지면, 
		if(m_ParmeterBox.IP.IncreaseExp(exp) == TRUE)
		{
			if(GetPlayerNumber() == UserPlayerNumber)
			{
				// 효과음
				PushEffect(EFFECT_LEVELUP, m_ssX, m_ssY);
				
				CR[m_NewID.ID].SetLevelUpDraw(TRUE);
			}

			// 레벨을 올려준다. 
			m_ParmeterBox.IP.SetLevel(m_ParmeterBox.IP.GetLevel() + 1);

			//체력과 마법력을 최고로 채워준다. 
			m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
		}

		// 전투kind를 온라인 kind로 변환한다.
		if(m_ParmeterBox.IP.GetLevel() >= 20)
		{
			SI32 kind = 0;
			switch(m_cltCharStatus.GetKind())
			{
			case KIND_ON_SWORDK:	kind = TransKindImjinToOnline(KIND_ON_SWORDUPK);	break;
			case KIND_ON_ARCHERK:	kind = TransKindImjinToOnline(KIND_ON_ARCHERUPK);	break;
			case KIND_ON_HORSEARCHERK:kind = TransKindImjinToOnline(KIND_ON_HORSEARCHERUPK);break;
			case KIND_ON_BUDAK:		kind = TransKindImjinToOnline(KIND_ON_BUDAUPK);		break;
			case KIND_ON_JUSTICEK:	kind = TransKindImjinToOnline(KIND_ON_JUSTICEUPK);	break;

			case KIND_ON_SWORDJ:	kind = TransKindImjinToOnline(KIND_ON_SWORDUPJ);	break;
			case KIND_ON_HORSESWORDJ:	kind = TransKindImjinToOnline(KIND_ON_HORSESWORDUPJ);	break;
			case KIND_ON_ADVBUDAJ:	kind = TransKindImjinToOnline(KIND_ON_ADVBUDAUPJ);	break;
			case KIND_ON_GUNJ:		kind = TransKindImjinToOnline(KIND_ON_GUNUPJ);		break;
			case KIND_ON_NINJAJ:	kind = TransKindImjinToOnline(KIND_ON_NINJAUPJ);	break;

			case KIND_ON_SWORDT:	kind = TransKindImjinToOnline(KIND_ON_SWORDUPT);	break;
			case KIND_ON_BUDAT:		kind = TransKindImjinToOnline(KIND_ON_BUDAUPT);		break;
			case KIND_ON_GUNT:		kind = TransKindImjinToOnline(KIND_ON_GUNUPT);		break;
			case KIND_ON_AXET:		kind = TransKindImjinToOnline(KIND_ON_AXEUPT);		break;
			case KIND_ON_AMAZONT:	kind = TransKindImjinToOnline(KIND_ON_AMAZONUPT);	break;

			case KIND_ON_FAN_C:			kind = TransKindImjinToOnline(KIND_ON_FANUP_C);	break;
			case KIND_ON_BUDAK_C:		kind = TransKindImjinToOnline(KIND_ON_BUDAKUP_C);	break;
			case KIND_ON_MUDOGA_C:		kind = TransKindImjinToOnline(KIND_ON_MUDOGAUP_C);	break;
			case KIND_ON_SWORD_C:		kind = TransKindImjinToOnline(KIND_ON_SWORDUP_C);	break;
			case KIND_ON_FIREGUN_C:		kind = TransKindImjinToOnline(KIND_ON_FIREGUNUP_C);	break;
//#ifdef _SET_CHARACTER_LIST
			// 2004/7/22 robypark
			// 중국 용병 - 도술사 => 고급 도술사가 되도록 추가
			case KIND_ON_MAGIC_C:		kind = TransKindImjinToOnline(KIND_ON_MAGIC_C_UP);	break;
			// robypark 2004/8/26 17:52
			// 고급 용병 추가
			// 대만 용병 - 야수전사 => 고급 야수전사
			case KIND_ON_BEASTWARRIOR:	kind = TransKindImjinToOnline(KIND_ON_BEASTWARRIOR_UP); break;
			// 대만 용병 - 염력사   => 고급 염력사
			case KIND_ON_YUMRUCK:		kind = TransKindImjinToOnline(KIND_ON_YUMRUCK_UP); break;
			// 일본 용병 - 음양사   => 고급 음양사
			case KIND_ON_COSMIC:		kind = TransKindImjinToOnline(KIND_ON_COSMIC_UP); break;
			// 조선 용병 - 의술사   => 고급 의술사
			case KIND_ON_MEDICALMAN:	kind = TransKindImjinToOnline(KIND_ON_MEDICALMAN_UP); break;
			// 조선 용병 - 칼잡이   => 고급 칼잡이
			case KIND_ON_BUTCHER:		kind = TransKindImjinToOnline(KIND_ON_BUTCHER_UP); break;
			// 중국 용병 - 장창무사 => 고급 장창무사
			case KIND_ON_LONGSPEAR:		kind = TransKindImjinToOnline(KIND_ON_LONGSPEAR_UP); break;
//#endif
			}

			if(kind)
			{
				OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(
					MyNet.PlayersdpId.dpId[GetPlayerNumber()], m_siOnlineID,
					kind);
			}
		}

	}

}


SI32 cltCharOrg::GetDpower()
{
	return m_ParmeterBox.GetDpower();
}



void cltCharOrg::SetName(char* name, SI32 namelength)
{
	strncpy(m_szName, name, namelength);
								
	// 널문자를 넣어준다. 
	m_szName[namelength] = NULL;
}


// 캐릭터의 이름을 얻어온다. 
char* cltCharOrg::GetName()const
{
	// 플레이어 이고 첫번쨰 캐릭터이면 주인공이기 때문에 주인공의 이름을 표시한다. 
	if(ISPLAYER( MyNet.PlayersdpId.dpId[GetPlayerNumber()] ) == TRUE)
	{
		return (char*)m_szName;
	}
	else
	{
		return KI[m_cltCharStatus.GetKind()].m_pszName;
	}

	return NULL;

}

// 특정 방향으로 캐릭터를 밀어낸다. 
BOOL cltCharOrg::Pull(SI32 dir)
{
	SI32 x, y;

	switch(dir)
	{
	case SOUTH:			x = m_ssX ;  y = m_ssY+1;	break;
	case SOUTH|WEST:	x = m_ssX-1; y = m_ssY+1;	break;
	case WEST:			x = m_ssX-1; y = m_ssY;		break;
	case WEST|NORTH:    x = m_ssX-1; y = m_ssY-1;	break;
	case NORTH:         x = m_ssX ;  y = m_ssY-1;	break;
	case NORTH|EAST:    x = m_ssX+1; y = m_ssY-1;	break;
	case EAST:          x = m_ssX+1; y = m_ssY;		break;
	case EAST|SOUTH:    x = m_ssX+1; y = m_ssY+1;	break;
	default:
		clGrp.Error("fd09273", "kind:%d dir:%d", m_cltCharStatus.GetKind(), dir);
	}

	// 이동지역이 비어 있다면,
	if(IsColl(x, y, Block) == FALSE)
	{
		m_ssX = x;
		m_ssY = y;

		// 빈 공간으로 밀려났다.
		return TRUE;
	}

	// 빈 공간이 아니라서 밀려나지 않았다.
	return FALSE;
}

// robypark 2004/6/1 17:11
// 특정 방향으로 캐릭터를 밀어낸다. 
// SI32 dir[in]: 방향
// SI32 siDistance[in]: 밀어낼 거리
BOOL cltCharOrg::Pull(SI32 dir, SI32 siDistance)
{
	SI32 x, y;

	switch(dir)
	{
		case SOUTH:
			x = m_ssX ;
			y = m_ssY + siDistance;
			break;
	
		case SOUTH|WEST:
			x = m_ssX - siDistance;
			y = m_ssY + siDistance;
			break;

		case WEST:
			x = m_ssX - siDistance;
			y = m_ssY;
			break;

		case WEST|NORTH:
			x = m_ssX - siDistance;
			y = m_ssY - siDistance;
			break;

		case NORTH:
			x = m_ssX ;
			y = m_ssY - siDistance;
			break;

		case NORTH|EAST:
			x = m_ssX + siDistance;
			y = m_ssY - siDistance;
			break;

		case EAST:
			x = m_ssX + siDistance;
			y = m_ssY;
			break;

		case EAST|SOUTH:
			x = m_ssX + siDistance;
			y = m_ssY + siDistance;
			break;

		default:
			clGrp.Error("fd09273", "kind:%d dir:%d", m_cltCharStatus.GetKind(), dir);
	}

	// 이동지역이 비어 있다면,
	if(IsColl(x, y, Block) == FALSE)
	{
		m_ssX = x;
		m_ssY = y;

		// 빈 공간으로 밀려났다.
		return TRUE;
	}

	// 빈 공간이 아니라서 밀려나지 않았다.
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 조합형 캐릭터 정보 초기화
void cltCharOrg::HeroInit()
{
	BOOL bExist;
	SI32 siCountA, siFileID;
	SI32 siBodyPart, siTotalPiece;


	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE ) return;

	AniStartFont			= 0;
	AniAction				= HERO_ACT_N1;
	AniStep_Hero			= 0;
	
	CharBaseWClass			= HERO_CLASS_HTH;			
	CharWeaponEnum			= CHAR_NORMALSPEED;
	Direction				= SOUTH|EAST;				// 기본 방향이 남동쪽 (즉 아래쪽)


	m_siPrevArmorDrawID		= -1;
	m_siPrevHelmetDrawID	= -1;
	m_siPrevWeaponDrawID	= -1;

	// 디폴트 몸 코드를 무엇으로 할 것인가 결정
	switch( KI[m_cltCharStatus.GetKind()].m_ssNation )
	{
	case COUNTRY_KOREA:		strcpy(DefaultBody, "KD01");		break;
	case COUNTRY_JAPAN:		strcpy(DefaultBody, "JD01");		break;
	case COUNTRY_CHINA:		strcpy(DefaultBody, "CD01");		break;
	case COUNTRY_TAIWAN:	strcpy(DefaultBody, "TD01");		break;
	default:				clGrp.Error("No Default Body:100", "%d", KI[m_cltCharStatus.GetKind()].m_ssNation);break;
	}


	g_FileLoadManager.SetAllChange(KI[m_cltCharStatus.GetKind()].m_ssDefaultArmor, KI[m_cltCharStatus.GetKind()].m_ssDefaultHelmet, KI[m_cltCharStatus.GetKind()].m_ssDefaultWeapon, KI[m_cltCharStatus.GetKind()].m_ssHeroID, m_NewID.ID, DefaultBody,TRUE,TRUE);

	
	




	//---------------------------------------------------------------------------------------------
	// 이미지 파일 사이즈를 미리 결정한다.
	siTotalPiece	= g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalPiece[AniAction][CharBaseWClass];
	bExist = FALSE;
	for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
	{
		// 찍을 Bodypart를 얻어온다.
		siBodyPart = g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siPartDrawSort[AniAction][CharBaseWClass][Direction][siCountA];
		
		if( g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].bPart[AniAction][CharBaseWClass][siBodyPart] == TRUE )
		{
			siFileID		=  g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID].siFileID[AniAction][siBodyPart];
			if( siFileID <= 0 )	continue;

			ImageXsize	= g_FileLoadManager.m_XSpr[siFileID].Header.Xsize;
			ImageYsize	= g_FileLoadManager.m_XSpr[siFileID].Header.Ysize;

			bExist = TRUE;
			break;
		}
	}
	if( bExist == FALSE )
	{
		clGrp.Error("SetFileNFont", "Unknown ImageFile Size");
	}
}



// 전체적인 공격 지연 시간을 구한다. 
SI32 cltCharOrg::GetTotalAttackDelay()
{
	if(m_siWeaponAttackDelay)
		return (SI32)((float)m_siWeaponAttackDelay * m_AttackDelay);
	else
		return (SI32)((float)KI[m_cltCharStatus.GetKind()].GetAttackDelay(m_siCurrentAttack) * m_AttackDelay);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

_Char::_Char() : cltCharOrg()
{
}

///////////////////////////////////////////////////////////////////////////////
// 캐릭터 정보 초기화
void _Char::CharInit(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid)
{
	ZeroMemory(this, sizeof(_Char));
	m_ssPreWeaponID = -1;



	Init(kind, id, unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid);

	// 조합형 캐릭터가 아닌 일반 캐릭터일 경우에 이미지 사이즈를 구한다. 조합형 캐릭터는 아래 HeroInit() 함수에서 구한다.
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE )
	{
		ImageXsize = GetFileXsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
		ImageYsize = GetFileYsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
	}

	m_cltCharDrawInfo.SetLastAniFrame(100);


	// 각 캐릭터의 특징에 따라 다른 성격을 부여한다. 

	// 본영이면 사용자의 기지로 설정한다.
	if(m_siOnlineID == 0 && GetPlayerNumber()==UserPlayerNumber)
	{
		GD.HomeX=m_ssX;
		GD.HomeY=m_ssY;
	}

	UpdateMenu(ORDER_INITMENU);



	SetFileNFont();
	SetInfo();

	// 위치에 대한 보정을 한다. 
	CalculateRealXY();

	Action();
}

// robypark 2004/6/9 12:32
// 캐릭터가 변신할 때 사용
// 이 함수는 CharInit(...)함수와 동일하다. 단지 SetInfo()함수를 호출하지 않는다.
//	SHORT CharOrderNumber: 유닛이 배열에서 어느 위치에 있는가?
void _Char::CharInit_DISGUISE(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID)
{
	ZeroMemory(this, sizeof(_Char));
	m_ssPreWeaponID = siPrevWeaponID;

	CharOrderNumber = usCharOrderNumber; // 유닛이 배열에서 어느 위치에 있는가?

	Init(kind, id, unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid);

	// 조합형 캐릭터가 아닌 일반 캐릭터일 경우에 이미지 사이즈를 구한다. 조합형 캐릭터는 아래 HeroInit() 함수에서 구한다.
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE )
	{
		ImageXsize = GetFileXsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
		ImageYsize = GetFileYsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
	}

	m_cltCharDrawInfo.SetLastAniFrame(100);


	// 각 캐릭터의 특징에 따라 다른 성격을 부여한다. 

	// 본영이면 사용자의 기지로 설정한다.
	if(m_siOnlineID == 0 && GetPlayerNumber()==UserPlayerNumber)
	{
		GD.HomeX=m_ssX;
		GD.HomeY=m_ssY;
	}

	UpdateMenu(ORDER_INITMENU);

	SetFileNFont();
//	SetInfo();	// => 이 함수 내에서 캐릭터 개수가 증가되어 동일한 캐릭터에 관한 정보를 중복으로 갖게된다.
				//   이로 인하여 이동속도와 공격속도가 증가되는 버그가 발생한다.

///////////////////////////////////////////////////////////////////////////

	// robypark 2004/07/09 11:29
	// 2차 장수가 죽고 등장하는 1차 장수가 죽으면 주인공 캐릭터가 조종이
	// 안되는 문제가 발생한다.
	// SetInfo() 함수에서 처리하더 일 중에 
	// 코드 GD.CharOrder[ GD.CharOrderIndex ]=m_NewID.ID;
	// 에 의하여 공격속도와 이동속도 증가문제가 발생하였으며,
	// 해당 코드에서 새로 변신되는 캐릭터 정보를 설정하지 않아서
	// 주인공 캐릭터 조종이 안되는 문제가 발생한다.
	// SetInfo() 함수의 내용과 동일하며 위의 문제 코드만 다르다.
    GD.IncreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());

	GD.CharInfo[m_NewID.ID]=m_cltCharStatus.GetKind();

	GD.CharOrder[ CharOrderNumber ]=m_NewID.ID;	// 새로운 캐릭터 생성이 아니라 기존의 캐릭터에서 변신이므로
												// 리스트에 추가할 필요없다.

	// 이 값을 증가시키지 않으면, 2차 장수가 죽도 등장하는 1차 장수가 죽으면, 적 캐릭터들이 움직이지 않게되는 버그가 발생한다.
	GD.CharOrderIndex++;
/*
	if(m_cltCharStatus.GetArea()==AREA_AIR)
	{
		GD.AirCharID[GD.AirCharNumber]=m_NewID;
		GD.AirCharNumber++;
	}
	else
	{
		GD.LandCharID[GD.LandCharNumber]=m_NewID;
		GD.LandCharNumber++;
	}
*/
///////////////////////////////////////////////////////////////////////////

	// 위치에 대한 보정을 한다. 
	CalculateRealXY();

	Action();
}


// 찍을 위치를 계산한다. 
void _Char::CalculateRealXY()
{
	SHORT x=m_ssX-m_scIDXsize/2+m_scIDXsize-1;
	SHORT y=m_ssY-m_scIDYsize/2+m_scIDYsize-1;
	
	//흔들림을 넣는다. 
	if(m_cltCharDrawInfo.GetVibrateSwitch() ==TRUE)
	{
		SI32 data = FlyVibrateData[((GD.Frame+m_NewID.ID)/4)%MAX_FLY_VIBRATE];
	    m_cltCharDrawInfo.SetVibrateY( data );
	}

	// 캐릭터의 중심을 구한다
	m_cltCharStatus.SetCenterX(MapCenterX[m_ssX][m_ssY]+RX);

	// 비행체는 중심을 맞춘다. 
	if(IsAtb(ATB_FLY))
	{
		m_cltCharStatus.SetCenterY(MapCenterY[m_ssX][m_ssY]+RY);


		RECT rect = m_cltCharDrawInfo.GetRealImageRect();

		RealRect.left=  m_cltCharStatus.GetCenterX()-(rect.right - rect.left)/2;
		RealRect.right= m_cltCharStatus.GetCenterX()+(rect.right - rect.left)/2;
		RealRect.top=   m_cltCharStatus.GetCenterY()-(rect.bottom - rect.top)/2;
		RealRect.bottom=m_cltCharStatus.GetCenterY()+(rect.bottom - rect.top)/2;

	}
	else
	{
		RECT rect = m_cltCharDrawInfo.GetRealImageRect();

		m_cltCharStatus.SetCenterY(MapCenterY[m_ssX][m_ssY]+5+RY-(rect.bottom - rect.top)/2);

		RealRect.left=  MapCenterX[x][y]+RX-(rect.right - rect.left)/2;
		RealRect.right= RealRect.left+(rect.right - rect.left);
		RealRect.bottom=MapCenterY[x][y]+RY;//-KI[Kind].BottomIndent;
		RealRect.top=   RealRect.bottom-(rect.bottom-rect.top); 
	}
}


///////////////////////////////////////////////////////////////////////////////
// Description	:	캐릭터를 찍을 좌표(m_ssDrawStartX, m_ssDrawStartY)를 계산한다. 
void _Char::CalculateDrawStartXY()
{
	SHORT centerx = 0, centery = 0;

	SHORT x = m_ssX - m_scIDXsize/2 + m_scIDXsize - 1;
	SHORT y = m_ssY - m_scIDYsize/2 + m_scIDYsize - 1;


	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
	Map.GetCenterXYByMapCenterXY(x, y, centerx, centery);
	
	m_ssDrawStartX = centerx - ImageXsize/2 + RX;
	
	// 비행체는 중심을 맞춘다. 
	if(IsAtb(ATB_FLY))
	{
		m_ssDrawStartY = centery - ImageYsize/2 + RY + m_cltCharDrawInfo.GetVibrateY();
	}
	else
	{			
		m_ssDrawStartY = centery - ImageYsize + KI[m_cltCharStatus.GetKind()].m_ssBottomIndent + RY + m_cltCharDrawInfo.GetVibrateY();
	}
}


///////////////////////////////////////////////////////////////////////////////
// 찍을 위치를 계산한다. 
void _Char::CalculateDrawStartXYInMapCaptureMode()
{
	SHORT centerx=0, centery=0;

	SHORT x=m_ssX-m_scIDXsize/2+m_scIDXsize-1;
	SHORT y=m_ssY-m_scIDYsize/2+m_scIDYsize-1;
	

	// 비행체는 중심을 맞춘다. 
	if(IsAtb(ATB_FLY))
	{
		centerx=MapCenterX[x][y];
		centery=MapCenterY[x][y];

	    m_ssDrawStartX=centerx-ImageXsize/2+RX;
	    m_ssDrawStartY=centery-ImageYsize/2+RY + m_cltCharDrawInfo.GetVibrateY();
	}
	else
	{
		// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
		centerx=MapCenterX[x][y];
		centery=MapCenterY[x][y];

	    m_ssDrawStartX=centerx-ImageXsize/2+RX;
		m_ssDrawStartY=centery-ImageYsize+KI[m_cltCharStatus.GetKind()].m_ssBottomIndent+RY + m_cltCharDrawInfo.GetVibrateY();
	}
}


// id가 나의 최소 범위안에 있는지 확인한다. 
BOOL _Char::IsInMinRange(SHORT id, SHORT minrange)
{

	if(max( abs(m_ssX-CR[id].m_ssX), abs(m_ssY-CR[id].m_ssY) )<=minrange)return TRUE;

	return FALSE;
}


void _Char::GetHitPoint(SHORT & x, SHORT &y)
{
	x=m_cltCharStatus.GetCenterX();
	y=m_cltCharStatus.GetCenterY();
}




// 팀컬러를 알려준다.
unsigned char _Char::GetTeamColor()const 
{
	return TeamColor[GetPlayerNumber()];
}

// pnumber인 플레이어에게 나를 보이게 한다. 
void _Char::SetDiscover(SHORT pnumber)
{

    Discover[pnumber]		=	TRUE;
    DiscoverDelay[pnumber]	=	MAX_DISCOVER_DELAY;

	if(AmbushSwitch == TRUE)
	{
		AmbushSwitch = FALSE;
		AmbushDelay  = 100;
	}
}

// 적에게 공격받고 있을때 호출하는 함수 
void _Char::SetBeAttackedSwitch(SHORT enemyid, SHORT attackmeplayer)
{
	if(enemyid<=0)return ;

	// 유효하지 않은 플레이어 번호가 넘어오면 리턴. 
	if(IsValidPlayerNumber(attackmeplayer) == FALSE)
	{
		return ;
	}

	AttackMePlayer=attackmeplayer;

    if(IsSee(enemyid)==FALSE)return ;
	AttackMeID=CR[enemyid].m_NewID;
	BeAttackedSwitch=TRUE;

    // 적에게 피격시 아군의 지원을 요청한다. 
    //GD.SetNeedHelpSwitch(GetPlayerNumber(), ID);

	NeedHelpSwitch=TRUE;
    NeedHelpDelay=MAX_NEED_HELP_DELAY;

}

// 적에게 공격받을때의 조치 
void _Char::BeAttackedOperation()
{
	int  x=0,  y=0;
	int dx=0, dy=0;
	_NewID attackmeid;

	// 다른 일로 바쁜 상태면 처리할 수 없다. 
	if(m_scBusySwitch==TRUE) return ;

	// 공격 받는 일이 중단되면, 
	if(BeAttackedSwitch==FALSE)return ;

	if(GD.Frame % 15)return ;

	attackmeid=AttackMeID;
	AttackMeID.Init();
	BeAttackedSwitch=FALSE;



	// 나를 공격하는 놈이 무효화되면,
	if(IsSee(attackmeid.ID)==FALSE)
	{
		return ;
	}
	// 나를 공격하는 놈이 아군이면, 
	if(IsFriendToByID(m_NewID.ID, attackmeid.ID))
	{
		return ;
	}

	// 나를 공격하는 놈이 어둠속에 있으면. 
	x= CR[attackmeid.ID].GetX();
	y= CR[attackmeid.ID].GetY();
	if(GD.BlackMap[GetPlayerNumber()][x][y] != 0)
	{
		// 공격할 수 있는 캐릭터라면, 
		// 공격 지점으로 이동해 간다. 
		if(CanAttack(attackmeid) == TRUE)
		{
			OrderMove(m_NewID, x, y, BY_COMPUTER);
		}
		else
		{
			return ;
		}
	}


	// 공격 받은 프레임을 설정한다.
	BeAttackedFrame=GD.Frame;
	// 나를 공격하던 유닛을 저장한다. 
	PrevAttackMeID=attackmeid;



	// 현재 작업이 도망갈 수 없는 것이 아니라면, 
	
	
	// 도망가야 정상 
    if((Order[CurrentOrder].Info&ORDERINFO_NORETREAT)==0
	&& CurrentOrder==ORDER_WAIT                          // 현재 특별한 명령을 수행중이 아니라면, 
	&& HoldSwitch==FALSE                                 // 사수 모드가 아니어야 한다.
	&& CanAttack(attackmeid)==FALSE                      // 내가 공격할 수 없는 캐릭터라면 
	&& CR[attackmeid.ID].AttackID.IsSame(m_NewID))
	{
		// 공격받은  프레임을 저장한다. 
	    BeAttackedFrame=GD.Frame;

			SHORT dir=DecideDirection8(CR[attackmeid.ID].GetX(), CR[attackmeid.ID].GetY(), m_ssX, m_ssY);
			SHORT tx=m_ssX, ty=m_ssY;
			
			if(dir&NORTH)
			{
				ty=max(0, m_ssY-5);
			}
			if(dir&SOUTH)
			{
				ty=min(Map.GetYsize()-1, m_ssY+5);
			}
			if(dir&EAST)
			{
				tx=min(Map.GetXsize()-1, m_ssX+5);
			}
			if(dir&WEST)
			{ 
				tx=max(0, m_ssX-5);
			}
			
			OrderMove(m_NewID, tx, ty, BY_COMPUTER);
			
			return ;
	}

	// 나를 공격하는 적을 공격한다. 
	OperateFindEnemy(CR[attackmeid.ID].GetX(), CR[attackmeid.ID].GetY(), attackmeid);

}


void _Char::OldOperation()
{

	if(CurrentOrder==ORDER_WAIT)
	{
	   // 이전에 하던 동작이 있다면 
	   if(OldOrder.Order!=ORDER_WAIT)
	   {
	    	// 이전 동작이 공격이라면,
	        if(OldOrder.Order==ORDER_ATTACK)
	        {
			   OldOrder.Order=ORDER_WAIT;
			   OrderAttack(m_NewID, LOWORD(OldOrder.Para1), HIWORD(OldOrder.Para1), OldOrder.Para2, BY_COMPUTER);
	        }
	        // 이전 동작이 이동이라면, 
	        else if(OldOrder.Order==ORDER_MOVE)
	        {
		        OldOrder.Order=ORDER_WAIT;

				// 사수 모드가 설정되어 있지않아야 이동할 수 있다. 
				if(HoldSwitch == FALSE)
		          OrderMove(m_NewID, LOWORD(OldOrder.Para1), HIWORD(OldOrder.Para1), BY_COMPUTER);
	        }
			else OldOrder.Order=ORDER_WAIT;
	    }
	}
}


void _Char::SetNewSelectedSwitch(SHORT mode)
{

	switch(mode)
	{
	case TRUE:
		NewSelectedSwitch=TRUE;
		NewSelectedDelay=NEW_SELECTED_DELAY;
		break;
	case FALSE:
		NewSelectedSwitch=FALSE;
		break;
	default:
		clGrp.Error("dfsj89", "fds923");
	}
}





// 캐릭터가 생성되었음을 알린다. 
void _Char::SetInfo()
{

    GD.IncreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());

	GD.CharInfo[m_NewID.ID]=m_cltCharStatus.GetKind();

	GD.CharOrder[ GD.CharOrderIndex ]=m_NewID.ID;
	CharOrderNumber=GD.CharOrderIndex;
	GD.CharOrderIndex++;

	if(m_cltCharStatus.GetArea()==AREA_AIR)
	{
		GD.AirCharID[GD.AirCharNumber]=m_NewID;
		GD.AirCharNumber++;
	}
	else
	{
		GD.LandCharID[GD.LandCharNumber]=m_NewID;
		GD.LandCharNumber++;

	}

}





// 캐릭터의 흔적을 남긴다. 
BOOL _Char::SetID()
{
	 int i, j;


	 if(m_scSeeSwitch==FALSE)return FALSE;


	 // 정지시에는 자신의 현 위치에만 충돌 표시를 한다. 
	 if(MovePercent==0)
	 {
		 // 지상캐릭터만 충돌을 체크한다. 
		 if(m_cltCharStatus.GetArea()==AREA_LAND)
		 {
		     for(i=0;i<m_scIDYsize;i++)
			    for(j=0;j<m_scIDXsize;j++)
				{
				    Map.SetCollMap(m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i, SelfBlock);
					Map.SetIDMap(m_NewID.ID, m_ssX+j-(int)(m_scIDXsize/2), m_ssY+i-(int)(m_scIDYsize/2));
				}
		 }
		 else
		 {
		     for(i=0;i<m_scIDYsize;i++)
			    for(j=0;j<m_scIDXsize;j++)
				{
				   // 비어있을  때에만 넣는다. 
				   if(clifMap.GetSkyMap(m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i)==0)
				      Map.SetSkyMap(m_NewID.ID, m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i);
				}

		 }
	 }
	 // 이동시 
	 else 
	 {
		 if(m_cltCharStatus.GetArea()==AREA_LAND)
		 {

			 for(i=0;i<m_scIDYsize;i++)
			    for(j=0;j<m_scIDXsize;j++)
				{
			       Map.SetCollMap(m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2,BLOCK_MOVE);
				   Map.SetIDMap(m_NewID.ID, m_ssX+j-(int)(m_scIDXsize/2), m_ssY+i-(int)(m_scIDYsize/2));
				}
				if(NX!=m_ssX || NY!=m_ssY)
				{
    	           for(i=0;i<m_scIDYsize;i++)
			          for(j=0;j<m_scIDXsize;j++)
					  {
			              Map.SetCollMap(NX+j-m_scIDXsize/2, NY+i-m_scIDYsize/2, BLOCK_MOVE);
					  }

		             OtherCollSwitch=TRUE;
		             OtherCollPoint.x=NX;
		             OtherCollPoint.y=NY;
				}


		 }
		 else
		 {
		     for(i=0;i<m_scIDYsize;i++)
			    for(j=0;j<m_scIDXsize;j++)
				{
				   // 비어있을  때에만 넣는다. 
				   if(clifMap.GetSkyMap(m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i)==0)
				      Map.SetSkyMap(m_NewID.ID, m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
				}

		 }
	 }
	 
	SHORT darkstep;
	 
	// 작은 지도에 표시되지 않도록 만든다.
	m_cltCharDrawInfo.SetCharSmallMapDrawSwitch(FALSE);
	
	
	for(i=0;i<m_scIDYsize;i++)
		for(j=0;j<m_scIDXsize;j++)
		{

			// 작은 지도에서 그려주어야 하는지 확인한다.
			
			// 어느 어두운 단계에 있는가 ?
			darkstep=GD.BlackMap[UserPlayerNumber][m_ssX][m_ssY];

			// 밝은 곳에 있으면 캐릭터를 그려주어야 한다. 
			if(darkstep < FOG2_DARK_STEP)
			{

				// 밝은 곳에서도 seemode가 설정되어 있어야 한다. 
			    if(m_scSeeMode[ UserPlayerNumber ]==TRUE)
				{
				    m_cltCharDrawInfo.SetCharSmallMapDrawSwitch(TRUE);
				}
			}
		}







	SetIDSwitch=TRUE;

	return TRUE;
}


// 캐릭터의 흔적을 없앤다. 
BOOL _Char::DeleteID()
{
	int i, j;

	if(m_scSeeSwitch==FALSE)return FALSE;

	// 지상캐릭터이면, 
	if(m_cltCharStatus.GetArea() == AREA_LAND)
	{
	 for(i=0;i<m_scIDYsize;i++)
		for(j=0;j<m_scIDXsize;j++)
		{
			Map.ClearIDMap(m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
			Map.DeleteCharCollMap(m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
		}
	}
	// 공중캐릭터이면, 
	else
	{
		// 자신의 정보가 저장되어 있다면, 
		if(clifMap.GetSkyMap(m_ssX, m_ssY)==m_NewID.ID)
			Map.ClearSkyMap(m_ssX, m_ssY);
	}


   if(OtherCollSwitch==TRUE)
   {
	 OtherCollSwitch=FALSE;

	 for(i=0;i<m_scIDYsize;i++)
		 for(j=0;j<m_scIDXsize;j++)
		 {
			 if(m_cltCharStatus.GetArea()==AREA_LAND)
			 {
	            Map.DeleteCharCollMap(OtherCollPoint.x+j-m_scIDXsize/2, 
		                              OtherCollPoint.y+i-m_scIDYsize/2);
			 }
		 }
   }

   SetIDSwitch=FALSE;

	return TRUE;
}




//  각종 파라메타를 초기화한다. 
BOOL _Char::ParametaAction()
{
	SHORT			i;
	SHORT			apower = 0;
	SHORT			dpower = 0;
	CItemHeader		*pItemHeader;

#ifndef _SKILL_
	SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
#endif

	// 무기에 따라 공격 속성을 바꾼다.
	if ( m_ssWeaponID != m_ssPreWeaponID )
	{

		m_uiAtb				= KI[m_cltCharStatus.GetKind()].m_uiAtb;
		m_uiAtb2			= KI[m_cltCharStatus.GetKind()].m_uiAtb2;
		m_uiAtb3			= KI[m_cltCharStatus.GetKind()].m_uiAtb3;
		m_uiAtb4			= KI[m_cltCharStatus.GetKind()].m_uiAtb4;
		m_uiAtb5			= KI[m_cltCharStatus.GetKind()].m_uiAtb5;
		m_scMagicApplyRate	= KI[m_cltCharStatus.GetKind()].GetMagicApplyRate();

		pItemHeader = g_pItem->GetItem(m_ssWeaponID);
		if(pItemHeader)
		{
#ifndef _SKILL_
			// 2차 장수는 무기를 착용해도 해당 무기 속성으로 바뀌지 않는다.
			if (siCharType != 3)
				SetAttackAtb(pItemHeader->pWeaponHeader->siAttackAtb);
#endif

		// 2차 장수 기술이 추가되었을 때 사용
		#ifdef _SKILL_
			SetAttackAtb(pItemHeader->pWeaponHeader->siAttackAtb);

			// robypark 2004/6/16 10:27
			// 2차 장수가 무기를 장착하였을 경우, 2차 장수의 고유 기술을 사용하지 못한다.
			// 예) 거북차-강강수월래, 지진차-격노염폭, 불랑기포차-앙천대소, 화룡차-맹격화룡파
			SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
			if (siCharType == 3)
			{
				Atb4 = 0;	// 2차 장수 기술은 모두 Atb4 속성에 설정되어 있다.
							// 0으로 설정하여 기술을 삭제한다.

				// 화룡차의 경우 2가지 공격속성이 있다. 근접 창 공격과 원거리 화룡출수 공격을
				// 가지고 있는데, 무기장착시 원거리 공격도 나가는 경우가 발생한다.
				// 이를 막기위해서 원거리 공격속성인 AttackAtb2를 0으로 설정해준다.
				AttackAtb2 = 0;
			}
		#endif
		}

		m_siWeaponAttackDelay = DecideAttackDelayByAttackAtb(GetAttackAtb());

		if(AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_AIR)
		{
			m_uiAtb |= ATB_ATTACKAIR;
		}
		if(AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_MINRANGE)
		{
			m_uiAtb |= ATB_MINRANGE;
		}


		// 아이템에 따른 사거리를 설정한다 
#ifndef _SKILL_
		// 2차 장수가 장착한 무기에 따라 사거리가 변경되지 않는다.
		if ((pItemHeader) && (siCharType != 3))
#endif
#ifdef _SKILL_
		if(pItemHeader)
#endif
		{
			m_ParmeterBox.SetItemAttackRange(pItemHeader->pWeaponHeader->siAttackRange);
		}
		m_ssPreWeaponID = m_ssWeaponID;

#ifndef _SKILL_
		if ((pItemHeader) && (siCharType != 3))
#endif
#ifdef _SKILL_
		if(pItemHeader)
#endif
		{
			// 특수기술 속성 갱신
			switch(pItemHeader->pWeaponHeader->uiMagic)
			{
			case ON_ITEM_MAGICATTACK_KILLSHOT:		m_uiAtb3 |= ATB3_SPECIALATTACK;		break;		// 일격필살
			case ON_ITEM_MAGICATTACK_SEISMIC:		m_uiAtb3 |= ATB3_EARTHQUAKE;		break;		// 지진술
//			case ON_ITEM_MAGICATTACK_HIDDEN:		m_uiAtb3 |= ATB2_HIDE;				break;		// 은신술
			case ON_ITEM_MAGICATTACK_VITALSTEEL:	m_uiAtb3 |= ATB3_ABSORBLIFE;		break;		// 흡수술
			case ON_ITEM_MAGICATTACK_MOONAXE:		m_uiAtb3 |= ATB3_MAGIC5J;			break;		// 만월도끼춤
			case ON_ITEM_MAGICATTACK_FIREBLOW:		m_uiAtb3 |= ATB3_KIMBOMB;			break;		// 폭뢰격
			case ON_ITEM_MAGICATTACK_ASTRAFIRE:		m_uiAtb3 |= ATB3_MAGIC4;			break;		// 기공신포
			case ON_ITEM_MAGICATTACK_FIREARROW:		m_uiAtb3 |= ATB3_FIREARROW;			break;		// 불화살
			case ON_ITEM_MAGICATTACK_MAGICARROW:	m_uiAtb3 |= ATB3_FIREARROW2;		break;		// 마법불화살
			case ON_ITEM_MAGICATTACK_WEDGEATTACK:	m_uiAtb3 |= ATB3_MAGIC8K;			break;		// 쐐기찌르기
			case ON_ITEM_MAGICATTACK_BOLT:			m_uiAtb3 |= ATB3_LIGHTNING;			break;		// 번개술
			case ON_ITEM_MAGICATTACK_ASTRALFIRE:	m_uiAtb3 |= ATB3_MAGIC4;			break;		// 기공신포
			case ON_ITEM_MAGICATTACK_TORNADO:		m_uiAtb3 |= ATB3_TORNADO;			break;		// 풍백술
			case ON_ITEM_MAGICATTACK_HOLD:			m_uiAtb3 |= ATB3_MAGIC6C;			break;		// 포박술
			case ON_ITEM_MAGICATTACK_RUNNINGFIRE:	m_uiAtb4 |= ATB4_RUNNINGFIRE;		break;		// 연발포탄.
			case ON_ITEM_MAGICATTACK_ENERGYPA:		m_uiAtb4 |= ATB4_ENERGYPA;			break;		// 장풍.
			case ON_ITEM_MAGICATTACK_LONG_SPEAR:	m_uiAtb4 |= ATB4_LONG_SPEAR;		break;		// 쌍검난무
			case ON_ITEM_MAGICATTACK_DOUBLE_SWORD:	m_uiAtb4 |= ATB4_DOUBLE_SWORD;		break;		// 장창소환.
			case ON_ITEM_MAGICATTACK_ICE_BARRIER:	m_uiAtb4 |= ATB4_ICE_BARRIER;		break;		// 빙석술.

			case ON_ITEM_MAGIC_FLOWER_NEEDLE_RAIN:	m_uiAtb4 |= ATB4_FLOWER_NEEDLE_RAIN;break;		// 만천화우
			case ON_ITEM_MAGIC_SOUL_CHANGE:			m_uiAtb4 |= ATB4_SOUL_CHANGE;		break;		// 빙의술
			case ON_ITEM_MAGIC_STONE_MAGIC:			m_uiAtb4 |= ATB4_STONE_MAGIC;		break;		// 석괴술
			case ON_ITEM_MAGIC_GOD_SPEAR:			m_uiAtb4 |= ATB4_GOD_SPEAR;			break;		// 신창노도
			case ON_ITEM_MAGICATTACK_MONKEYSTICK:	m_uiAtb3 |= ATB3_MAGIC_RECALL_MONKEY;		break;		// 여의봉
			}

			// 마법적용률 갱신
/*			if(pItemHeader->pWeaponHeader->uiMagic)
			{
				if(IsGeneral())		// 장수
				{
					if(MagicApplyRate < pItemHeader->pWeaponHeader->siMagicAttack)
						MagicApplyRate = pItemHeader->pWeaponHeader->siMagicAttack;
				}
				else	MagicApplyRate = pItemHeader->pWeaponHeader->siMagicAttack;
			}*/
		}
	}

	// 쐐기지르기의 경우,  
	if(IsAtb3(ATB3_MAGIC8K)== TRUE) 
	{
		// 마법력이 부족하면 무기에 의한 사정거리가 1이다. 
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_MAGIC8K)
			m_ParmeterBox.SetItemAttackRange(1);
		else 
			m_ParmeterBox.SetItemAttackRange(5);
	}
	// 해킹의 우려가 있는 경우, 
	if(m_ParmeterBox.Action(g_pItem, MW) == FALSE)
	{
		// 서버에 해킹 우려가 있는 사용자라는 사실을 알린다. 
		// 게임을 종료시킨다. 
		GameStatus = GS_INITQUIT;

		// 해킹으로 인해 게임이 종료됨을 알린다. 
		clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
	}


	// 타이머가 설치된 유닛은 점차 죽여간다.
	if(m_siTimerDelay)
	{
		if(GD.Frame % m_siTimerDelay == 0)
		{
			m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife()-2);
		}
	}
	//
	if(m_bSoulUse)
	{
		if(m_siSoulDelay == 0)
		{
			m_ParmeterBox.IP.SetStr(m_ParmeterBox.IP.GetStr()/3);
			m_ParmeterBox.IP.SetDex(m_ParmeterBox.IP.GetDex()/2);
			m_bSoulUse = false;
		}
	}


	// 무기를 장착하면 무기에 의해 공격 범위가 결정된다. 
	SI32 attackrange;
#ifndef _SKILL_
	// 2차 장수 공격범위(사거리)는 무기 아이템에 영향을 받지 않는다.
	if ((m_ParmeterBox.GetItemAttackRange()) && (siCharType != 3))
#endif
#ifdef _SKILL_
	if(m_ParmeterBox.GetItemAttackRange())
#endif
		attackrange   = m_ParmeterBox.GetItemAttackRange();
	// 무기가 없으면 디폴트 공격 범위를 선택한다. 
	else attackrange   = KI[m_cltCharStatus.GetKind()].GetAttackRange(m_siCurrentAttack);

	SHORT searchrange   = KI[m_cltCharStatus.GetKind()].m_siSearchRange		;//+ m_ParmeterBox.GetItemAttackRange();


	//속도의 딜레이를 준다.
	//m_siMoveDelay의 변화 된 값으로 movespeed를 계산한다.
	SHORT movespeed     = (SHORT)((float)KI[m_cltCharStatus.GetKind()].m_ssMoveSpeed*m_MoveDelay);//+ siEtcMoveSpeed;


	if(m_ParmeterBox.IP.GetMana() > m_ParmeterBox.GetMaxMana())	m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
	if(m_ParmeterBox.IP.GetLife() > m_ParmeterBox.GetMaxLife())	m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
	SetAttackRange(attackrange);
	m_siSearchRange		= searchrange;
	MoveSpeed		= movespeed;

	// 독나방인 경우 가루를 뿌린다.
	if(m_cltCharStatus.GetKind() == KIND_BUTTERFLY)
	{
		// 살아있는 동안에만. 
		if(IsAlive(m_NewID) == TRUE)
		{
			if(GD.Frame % 3 == 0)
			{
				SI32 smokeid = FindEmptySmoke();
				if(smokeid)
				{
					SetSmoke(KIND_SMOKE_DUST, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AREA_LAND);
					
					SI32 hitrate = m_ParmeterBox.GetHitRate( CR[AttackID.ID].m_ParmeterBox.GetDR() );
					CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_BUTTERFLY, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssX, m_ssY, hitrate);
				}
			}
		}
	}

	// 은신술을 사용하는 유닛이고 은신술이 개발되어 있는 경우.  
	if((KI[m_cltCharStatus.GetKind()].m_uiAtb2 & ATB2_HIDE) )
	{
		m_uiAtb2|=ATB2_HIDE;
	}
	else
	{
		// 장수효과에 의한 은신술이 가능하면, 
		if(bAssistHideSwitch)
		{
			m_uiAtb2 |= ATB2_HIDE;
		}
		else
		{
			// 그외에는 은신 속성을 뺀다. 
			if(m_uiAtb2 & ATB2_HIDE)
			{
				m_uiAtb2-=ATB2_HIDE;
			}
		}
	}

	// 수영중이면 물에 가라앉도록 만든다. 
	if( Map.IsInMapArea(m_ssX, m_ssY) && Map.GetSeaMode(m_ssX, m_ssY)
	&& (Map.GetShallowMap(m_ssX, m_ssY) == FALSE || GD.Rain.WaterLevel==(MAX_WATER_LEVEL_STEP-1))
	&& SwimCharAnimationIndex )
	{
		SwimSwitch = TRUE;
		DyingAtb = DYINGATB_SHIP|DYINGATB_IMMEDIATE;
	}
	else 
	{
		SwimSwitch = FALSE;
		DyingAtb = KI[m_cltCharStatus.GetKind()].m_uiDyingAtb;
	}


	if(GD.Frame % 2 == 0)
	{
		
		// 보조 공격력을 없앤다.
	    if(m_ParmeterBox.AssistApowerByGEffect)
		{
		    if((GD.Frame-m_ParmeterBox.AssistApowerFrame) > 40)
			{
			   m_ParmeterBox.AssistApowerByGEffect = 0;
			}
		}
	    // 보조 방어력을 없앤다.
	    if(m_ParmeterBox.AssistDpowerByGEffect)
		{
		    if((GD.Frame - m_ParmeterBox.AssistDpowerFrame) > 40)
			   m_ParmeterBox.AssistDpowerByGEffect = 0;
		}


		if(bAssistHideSwitch)
		{
		    if((GD.Frame - AssistHideFrame) > 40)
			{
			   bAssistHideSwitch = FALSE;
			}
		}

		// 이동 캐릭터는 다시 어둡게 만들기도 한다. 
		
		// 은신하는 속성의 캐릭터는,  
		if((IsAtb2(ATB2_HIDE) || AmbushSwitch == TRUE) && MapEditMode == FALSE)
		{
			
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				
				// 같은 편에게는 어둡게 만들지 않는다. 
				if(GD.IsFriendTo(GetPlayerNumber(), i) == TRUE)
				{
					m_scSeeMode[i] = TRUE;
					continue;
				}
				
				// 어두운 곳에서는 어둡게 만든다. 
				if(GD.BlackMap[i][m_ssX][m_ssY] >= FOG2_DARK_STEP)
				{
					DiscoverDelay[i]=0;
				}
				
				// 발견되어 있는 시간이 있다면 줄여나간다. 
				if(DiscoverDelay[i])
				{
					m_scSeeMode[i]=TRUE;
					DiscoverDelay[i]--;
				}
				
				// 유효기간이 지나면 안보이게 만든다.
				if(DiscoverDelay[i]==0)
					m_scSeeMode[i]=FALSE;
			}
		}
		else
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// 같은 편에게는 어둡게 만들지 않는다. 
				if(GD.IsFriendTo(GetPlayerNumber(), i) == TRUE)
				{
					m_scSeeMode[i] = TRUE;
					continue;
				}
				
				// 어두운 곳에서는 어둡게 만든다. 
				if(GD.BlackMap[i][m_ssX][m_ssY] >= FOG2_DARK_STEP)
				{
					m_scSeeMode[i] = FALSE;
				}
				// 밝은 곳에는 보이게 만든다.
				else
				{
					m_scSeeMode[i] = TRUE;
				}
			}
		}
		
		

		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// 보이는 방향이고 
			if(m_scSeeMode[i]==TRUE)
			{
				// 완전히 보이면, 
				if(m_scSeeStep[i]==0)
				{
				}
				else
				{
					m_scSeeStep[i]--;
				}

			}
			// 사라지는 방향이고 
			else
			{
				// 완전히 사라지면, 
				if(m_scSeeStep[i]==(MAX_LIGHT_STEP-1))
				{
				}
				else
				{
					m_scSeeStep[i]++;
				}

			}
		}

		// 플레이어에게 안보이는 모드이면, 
		if(m_scSeeMode[ UserPlayerNumber ] == FALSE
		&& m_scSeeStep[ UserPlayerNumber ] == (MAX_LIGHT_STEP-1))
		{
			m_cltCharDrawInfo.SetDrawSwitch(FALSE);
		}
		else
			m_cltCharDrawInfo.SetDrawSwitch(TRUE);

	}

	if( (GD.Frame % 2) == 0 )
	{	
		// 치료받는 그림을 수정한다. 
	    if(HealDrawSwitch==TRUE)
		{
			HealDrawStep++;
			if(HealDrawStep==10)
			{
				HealDrawSwitch=FALSE;
				HealDrawStep=0;
			}
		}

		// (거상) 레벨업하는 그림을 수정한다. 
	    if( LevelUpDrawSwitch == TRUE )
		{
			LevelUpDrawStep++;
			if( LevelUpDrawStep == 20 )
			{
				LevelUpDrawSwitch	= FALSE;
				LevelUpDrawStep		= 0;
			}
		}

	    // 경험치 빼앗기 그림을 수정한다. 
	    if(StealExpDrawSwitch==TRUE)
		{
			StealExpDrawStep++;
			if(StealExpDrawStep==11)
			{
				StealExpDrawStep=0;
			    StealExpDrawSwitch=FALSE;
			}
		}

		m_cltElectronShock.Action();

		// 포박과 관련. 
		if(m_siHaltDelay)
		{
			m_siHaltDelay -- ;

			//if((GD.Frame / 3 == 0 )
			{
				if(m_siHaltStep < (13 - 1))
				{
					// 효과음 연주. 
					switch(m_siHaltReason)
					{
					// robypark 2004/6/10 18:33
					// 천지독살 냉기에 의한 마비, 흑룡차 기본 공격에 의한 마비 효과음 연주
					case HALT_REASON_FOG_OF_ICE:
					// 냉기 공격에 의한 마미 효과음 연주
					case HALT_REASON_ICE:
						if(m_siHaltStep == 0)
						{
							// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
							if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
								PushEffect(EFFECT_ICEPALSY, m_ssX, m_ssY);
						}
						break;
					}
					
					m_siHaltStep++;
				}
			}
		}
		if(m_siSoulDelay)
		{
			m_siSoulDelay--;
			
			m_siFontCount++;
			if(m_siFontCount == 120)
				m_siFontCount = 0;
			
		}

		// 중독조치. 
		SI32 power = m_cltPoisonEffect.Action();
		if(power)
		{
			// robypark 2004/8/9 13:44
			// 중독 공격한 캐릭터 ID를 추가하여 독에 의한 사망시 경험치 주도록 수정
			if(m_ParmeterBox.IP.GetLife() > 0)	// 유효한 상태에서 
				HitNOperateAfterHit(m_cltPoisonEffect.GetPoisonAttacker(), m_NewID, CR[m_cltPoisonEffect.GetPoisonAttacker().ID].GetPlayerNumber(), ATTACKATB_TOAD, power);

//			HitNOperateAfterHit(ZeroID, m_NewID, -1, ATTACKATB_TOAD, power);
		}
	}



	if(NewSelectedSwitch==TRUE)
	{
		if(GD.Frame % 2 ==0)
		{
			if(NewSelectedDelay>0)
			{
				NewSelectedDelay--;
			}
			else
			{
				SetNewSelectedSwitch(FALSE);
			}
		}
	}

	// 피격시 표현용. 
	if(m_bBeAttackedDrawSwitch)
	{
		if(GD.Frame % 3 == 0)
			m_siBeAttackedDrawStep++;

		if(m_siBeAttackedDrawStep >= 4 )
		{
			SetBeAttackedDrawSwitch(FALSE);
			SetBeAttackedDrawStep(0);
		}
	}


	// 방어막이 쳐있다면, 시간을 줄여준다. 
	if(ShieldDelay)
	{
		ProtectorDrawSwitch=TRUE;
	}

	//수호부 (소) 변신 시간 카운트
	if(IsRecallGuardianCaw())
	{
		SI16	dif;
		switch(m_siCawNumber)
		{
			case 1:
			{
					RecallGuardianCaw();
					m_siCawNumber = 2;		
			}
			break;
			case 2:
			{
				if((dif = timeGetTime() - m_dwCawTimer) > 5000)
				{
					RecallGuardianCaw();
					m_siCawNumber = 3;
				}
			}
			break;
			case 3:
			{
				if((dif = timeGetTime() - m_dwCawTimer) > 10000)
				{
					RecallGuardianCaw();
					m_siCawNumber = 0;
				}
			}
			break;
		}
	}

	// 일단 보이는 상태여야 한다.
	// 자신의 위치를 파악한다.
	// 얕은 물 위에 있는지 확인한다. 
	BOOL oldshallowwaterswitch = ShallowWaterSwitch;
	ShallowWaterSwitch  = FALSE;
	if(m_scSeeSwitch==TRUE && IsAtb(ATB_FLY)==FALSE)
	{
		// 얕은 물에 있고 배가 아니면, 
		if(Map.GetShallowMap(m_ssX, m_ssY) && IsAtb(ATB_SHIP)==FALSE)
		{
			
			// 깊은 물이면 사망한다. (수위가 높아진 상태라면. )
			if((GD.Rain.WaterLevel==(MAX_WATER_LEVEL_STEP-1)) )
			{
				// 수영을 할 수 없는 유닛이면 죽는다. 
				if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_SWIM) == FALSE)
				{
					m_ParmeterBox.IP.SetLife(0);
					DyingAtb|=DYINGATB_SHIP;
				}
			}
			else
			{
				// 이전에 얕은 물에 있던 상태가 아니라면, 
				if(oldshallowwaterswitch==FALSE)
				{
					// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_SHALLOW, m_ssX, m_ssY);
				}
				
				ShallowWaterSwitch=TRUE;
			}
		}
	}

	if(GD.Frame % 2==0)
	{
   	     if(GetTotalAttackDelay() > m_siAttackDelayor)
		 {
	         m_siAttackDelayor++;
		 }
	     else
		 {
   	         if(FireRepeatDelay > FireRepeatDelayor)
			 {
	            FireRepeatDelayor++;
			 }
		 }
	}

	if(GD.Frame % 10 == 0 && CanWork()==TRUE)
	{

	    // 자폭기능 작동 
	    // 업그레이드가 되어 있어야 한다. 
	    if( IsAtb(ATB_SELFEXP) == TRUE)
		{
		    if(SelfExpPercent < 100)
			{
			   if(GD.Frame % 2 == 0)
			      SelfExpPercent++;
			}
		}
	
	
		if(FireSwitch==TRUE)
		{
		    if(FireDelay)
			{
			   FireDelay--;
			}
		    else
			{
			   FireSwitch=FALSE;
			   FireID.Init();
			}
		}

		if(NeedHelpSwitch==TRUE)
		{
            if(NeedHelpDelay>0)
			{
		        NeedHelpDelay--;
			}

	        if(NeedHelpSwitch==0)
			{
		       NeedHelpSwitch=FALSE;
			}
		}

	   // 분신은 자신의 주인이 되는 캐릭터의 마법력을 계속해서 갉아 먹는다. 

	   if(m_bGuardianChar == FALSE)
	   {
		   if(HostID.IsEmpty()==FALSE)
		   {
			   //빙의술로 만들어진 캐릭터인가.
			   if(m_bChangePlayer == FALSE )
			   {
				   // 자신의 체력을 떨군다. 
				   if(m_ParmeterBox.IP.GetLife() > 3)
					   m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife()-3);
				   else
					   m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife()-1);
			   }
		   }
	   }
	}

	//수호부 아이템이 존재 할경우.
	//m_siGuardianID 자신이 가지고 있는 수호부 아이템의 아이디 
	if(m_siGuardianID != 0)
	{
		//적을 공격하거나 적에게 맞는다.
		//만약 수호부 아이템이 발동.
		if(m_bUseGuardian == true)
		{
			switch(m_siGuardianKind)
			{
				case ON_GUARDIAN_MOUSE: 
					ReCallGuardianChar();
					break;
				case ON_GUARDIAN_CAW:
					StartRecallGurdianCaw(m_NewID,BY_PLAYER);
					break;
				case ON_GUARDIAN_DRAGON:
					GroundExpOperation(AttackID,m_siAttackX_ForGuardianDragon, m_siAttackY_ForGuardianDragon);
					break;
				case ON_GUARDIAN_TIGER:
					AttackMagicGuardianTiger();
					break;
				case ON_GUARDIAN_HORSE:
					MagicGuardianHorse();
					break;
				case ON_GUARDIAN_SHEEP:
					MagicGuardianLifeUp();
				break;
				case ON_GUARDIAN_RABBIT:
					AttackMagicGuardianRabbit();
					break;
				case ON_GUARDIAN_SNAKE:
					ReCallGuardianChar();
					break;
				case ON_GUARDIAN_PIG:
					ReCallGuardianChar();
					break;
				case ON_GUARDIAN_CHICKEN:
				{
					if(m_siGuardianSkillNum == 0)
						AttackMagicGuardianChicken();						
					else
						ReCallGuardianChar();
				}
				break;
				case ON_GUARDIAN_MONKEY:
					ReCallGuardianChar();
					break;
				case ON_GUARDIAN_DOG:
				{
					MagicGuardianDamageResistUp();
				}
					break;
			}
			m_siPreGuardianID = m_siGuardianID;
			m_siGuardianID = 0;
		}			
	}

	//수호부를 기능을 해체한다.
	if(m_bUseGuardian == true)
	{
		//시간 제한이 있는 수호부라면.
		if(m_siGuardianTimeOut == true)
		{
			DWORD difGuardianTime;
			difGuardianTime = ((timeGetTime() - m_dwGuardianTime)/1000);
			if(difGuardianTime > m_siGuardianAliveTime)
			{	
				GuardianBreakless(m_siGuardianKind);
				m_bUseGuardian = false;		//수호부발동이 소멸되었나.
				
			}
		}
		//시간 제한이 없는 수호부라면.
		else 
		{
			//해당 수호부의 기술을 사용하였나.
			if(m_siGuardianNoTimeOutSkillUse == true)
			{
//				m_bGuardianEquip = false;	//수호부을 가지고 있나.
				m_bUseGuardian = false;		//수호부발동이 소멸되었나.
			}
		}	
	}
	return TRUE;
}


// 건물이나 캐릭터의 모든 행동을 제어하는 루틴 
// 리턴값:  FALSE  .  특이 사항 없음 
//          TRUE   ->  캐릭터를 삭제함 
BOOL _Char::Action()
{
	// 수호부(돼지) : TRUE이면 다른 동작은 수행하지 않고 기다린다.
	if(ActionPig())		return TRUE;

	// 치트키를 사용한 캐릭터 정보 표시 
	if(Cheat.CheatStatusSwitch==TRUE)
	{
		if(m_scSelectedSwitch==TRUE)
		{
		   char buffer[ FILE_NAME_SIZE ];
		   wsprintf(buffer, "pn:%d ID:%d kind:%d o:%d ws:%d aid:%d (x:%d y:%d) (dx:%d dy:%d) sws:%d agd:%d", 
			   GetPlayerNumber(), m_NewID.ID, m_cltCharStatus.GetKind(), CurrentOrder, WorkStep, AttackID.ID, m_ssX, m_ssY, DX, DY, ShallowWaterSwitch, AttackGiveUpDelayor);

		   SetMessage(buffer);
		 }
	}

	// 지휘효과 관련 처리. 
     GeneralEffect();

	// 선택된 캐릭터는 
	// 메뉴를 업데이트한다. 
	if(m_scSelectedSwitch==TRUE)
	    UpdateMenu(UpdateMenuOrder);

	// 호스트가 설정되어 있고, 
	if(HostID.IsEmpty()==FALSE)
	{
		// 죽었다면, 
		if(IsAlive(HostID.ID)==FALSE)
		{
			m_ParmeterBox.IP.SetLife(0);
		}
	}

	if(MapEditMode==FALSE)
	{
		// 유효한 상태에서 
		if(m_ParmeterBox.IP.GetLife() > 0)
		{
		   // 공격 속성이 있는 캐릭터만 전방을 수색한다. 
			if( (GD.Frame + m_NewID.ID) % 10 ==0)
			{
			  SearchForward();
			}
			  
    	   // 적에게 공격받을때의 조치 
	       BeAttackedOperation();
    	   // 비축된 명령을 수행한다. 
    	   GetOrderBuffer();
		}
	    else
		{
		    if(m_scSelectedSwitch==TRUE)
			{
			   SetSelect(FALSE, FALSE);
			}

	        // 체력이 떨어지면 죽는다. 
	       if(CurrentOrder!=ORDER_DIE && CurrentOrder!=ORDER_DEAD && CurrentOrder!=ORDER_CARCASS)
		   {
			   SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

			   if(siCharType != 3)
			   {
			   // 보이는 상태면, 
				   if(m_scSeeSwitch==TRUE)
				   {
					  CurrentOrder=ORDER_DIE;

					  Step=0;
				   }
				   // 보이지 않는 상태라면, 
				   else
				   {
					   Dead();
					   return FALSE;
				   }
			   }
		   }
		}
	}

	// 각종 변수를 재설정한다. 
    ParametaAction();

	// 동작에 관계없이 애니가 지속되는 경우 
	// 이런 경우에도 건설이나 해체등은 영향받지 않는다.
	if( IsAtb(ATB_ABSANIWAIT) && CurrentOrder!=ORDER_BUILD && CurrentOrder!=ORDER_SELL )
	{
		if(abs(GD.Frame - m_cltCharDrawInfo.GetLastAniFrame()) >= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Wait0CharAnimationIndex].siDelay )
		{
			m_cltCharDrawInfo.SetLastAniFrame(GD.Frame);

			SI32 index  = 0;
			
			m_cltCharDrawInfo.SetAnimation(ANI_WAIT0);
			index = Wait0CharAnimationIndex;

			Step++;


			if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].GetFrameNumber())
				Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[index].GetFrameNumber();
			else
				Step = 0;
			
			m_cltCharDrawInfo.SetAniStep(Step);
			
			m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
		}
	}

	switch(CurrentOrder)
	{
	//  아무 작업도 하지 않는 대기 상태 
	case ORDER_WAIT:
	    DeleteID();
        OperateWait();

		SetID();
    	break;

	// 현재 하고 있는 동작을 멈춤
	case ORDER_STOP:
		DeleteID();
		
		if(IsPreMoveOk() == TRUE)
		{
			CurrentOrder=ORDER_WAIT;
		}
		else
		{
		}

		SetID();
		break;

	case ORDER_AMBUSH:
		DeleteID();
		Special(SPECIAL_AMBUSH);
		SetID();
		break;

	case ORDER_DETECTMINE:
		DeleteID();
		Special(SPECIAL_DETECTMINE);
		SetID();
		break;

	case ORDER_CLON:
	case ORDER_ENEMYCLON:
		DeleteID();
		Clon();
		SetID();
		break;
	case ORDER_RECALL_DAMDUK:
		DeleteID();
		ReCallDamduk();
		SetID();
		break;
	case ORDER_RECALL_MONKEY:
	case ORDER_RECALL_AMAKUSA:
	case ORDER_RECALL_TIGER:
		DeleteID();
		ReCall();
		SetID();
		break;
	case ORDER_RECALL_PANDA:
		ReCallPanda();
		break;
	case ORDER_GUARDIAN_RABBIT:
		DeleteID();
		//MagicGuardianRabbit();
		AttackMagicGuardianRabbit();
		SetID();
		break;
	case ORDER_RECALL_GUARDIAN_MOUSE:
		DeleteID();
		ReCallGuardianChar();
		SetID();
		break;
	case ORDER_RECALL_GUARDIAN_CAW:
		DeleteID();
		RecallGuardianCaw();
		SetID();
		break;
	case ORDER_GUARDIAN_TIGER:
		DeleteID();
		AttackMagicGuardianTiger();
		SetID();
		break;
	case ORDER_GUARDIAN_HORSE:
		DeleteID();
		MagicGuardianHorse();
		SetID();
		break;
	case ORDER_GUARDIAN_SHEEP:
		DeleteID();
		SetID();
		break;
	case ORDER_GUARDIAN_DRAGON:
		DeleteID();
		//GroundExp();
		SetID();
		break;
	case ORDER_DIE:
	{
		SI16 siDying;
		ReleaseItem();
		DeleteID();

		if (CR[HostID.ID].m_siReCallCount == 1)
			CR[HostID.ID].m_siReCallCount = 0;

		siDying = Dying();
		if(siDying == CHAR_DEAD)
		{
			{
				//BusySwitch=FALSE;
			}
			// 즉각 죽는 것이면 시체를 남기지 않는다. 
			if(IsDyingAtb(DYINGATB_EXP | DYINGATB_SHIP | DYINGATB_NOCARCASS)==TRUE)
			{
				 CurrentOrder=ORDER_DEAD;
			}
			else
			{
				CurrentOrder=ORDER_CARCASS;
			}
		}
		else if(siDying == CHAR_GENERALEX)
		{
			m_scBusySwitch=FALSE;
			CurrentOrder=ORDER_WAIT;
		}
		else SetID();
	
	}
	break;
	case ORDER_DIE_GENERALEX:
	{
		GeneralExDying();
	}
	break;
	case ORDER_CARCASS:
		if(CarcassDelayor>=CarcassDelay)
			CurrentOrder=ORDER_DEAD;
		else
			CarcassDelayor++;
		break;

	case ORDER_DEAD:
		Dead();
		
		return FALSE; 
		break;

	case ORDER_PATROL:
	    DeleteID();
		Patrol();
		SetID();
		break;

	case ORDER_HEAL:
	    DeleteID();
		Heal();
		SetID();
		break;

	case ORDER_MANA:
		DeleteID();
		Mana();
		SetID();
		break;

	case ORDER_EARTHQUAKE: 
	    DeleteID();
		EarthQuake();
		SetID();
		break;

	case ORDER_TORNADO:
	    DeleteID();
		Special(SPECIAL_TORNADO);
		SetID();
		break;
	case ORDER_YELLOWSAND:
		DeleteID();
		Special(SPECIAL_YELLOWSAND);
		SetID();
		break;
	case ORDER_SOUL:
		DeleteID();
		Soul();
//		Special(SPECIAL_SOUL);
		SetID();
		break;
	////////////////////////////////준석이형///////////////////////
	case ORDER_GHOST:
		break;
	case ORDER_DARKFIRE:
	    DeleteID();
		Special(SPECIAL_DARKFIRE);
		SetID();
		break;
	///////////////////////////////////////////////////////////////
	case ORDER_MAGIC2T:
	    DeleteID();
		Special(SPECIAL_MAGIC2T);
		SetID();
		break;
	/////////////////////////////////////
	case ORDER_MAGICTEST:
		DeleteID();
		Special(SPECIAL_MAGICTEST);
		SetID();
		break;
	/////////////////////////////////////
	case ORDER_MAGIC4T:
	    DeleteID();
		Special(SPECIAL_MAGIC4T);
		SetID();
		break;

	case ORDER_LIGHTNING:
	    DeleteID();
		Lightning();
		SetID();
		break;

	case ORDER_KIMBOMB:
	    DeleteID();
		Special(SPECIAL_KIMBOMB);
		SetID();
		break;

	case ORDER_GROUNDEXP:
	    DeleteID();
		Special(SPECIAL_GROUNDEXP);
		SetID();
		break;

	case ORDER_MAGIC4C:
	    DeleteID();
		Special(SPECIAL_MAGIC4C);
		SetID();
		break;
	case ORDER_IVORY:
		DeleteID();
		SpecialAtb4(SPECIAL_IVORY);
		SetID();
		break;
	case ORDER_BLUEDRAGON:
		DeleteID();
		SpecialAtb4(SPECIAL_BLUEDRAGON);
		SetID();
		break;
	case ORDER_RUNNINGFIRE:
		DeleteID();
		SpecialAtb4(SPECIAL_RUNNINGFIRE);
		SetID();
		break;	
	case ORDER_ENERGYPA:
		DeleteID();
		SpecialAtb4(SPECIAL_ENERGYPA);
		SetID();
		break;
	case ORDER_SNOWATTACK:
		DeleteID();
		SpecialAtb4(SPECIAL_SNOWATTACK);
		SetID();
		break; 
	case ORDER_FIREATTACK:
		DeleteID();
		SpecialAtb4(SPECIAL_FIREATTACK);
		SetID();
		break;
	case ORDER_FIREENERGYPA:
		DeleteID();
		SpecialAtb4(SPECIAL_FIREENERGYPA);
		SetID();
		break;
	case ORDER_MAGIC6C:
	    DeleteID();
		Special(SPECIAL_MAGIC6C);
		SetID();
		break;

	case ORDER_ICE:
		DeleteID();
		Special(SPECIAL_ICE);
		SetID();
		break;

	case ORDER_USEBUDA:
	    DeleteID();
		UseBuda();
		SetID();
		break;

	case ORDER_AIRMAGIC:
	    DeleteID();
		AirMagic();
		SetID();
		break;

	case ORDER_MAGIC5J:
	    DeleteID();
		Special(SPECIAL_MAGIC5J);
		SetID();
		break;

	case ORDER_MASSHEAL:
		DeleteID();
		MassHeal();
		SetID();
		break;

	case ORDER_STEALEXP:
		DeleteID();
		StealExp();
		SetID();
		break;

	case ORDER_RAINMAKE:
		DeleteID();
		RainMake();
		SetID();
		break;

    // 캐릭터를 이동 시킴 
	case ORDER_MOVE:
		DeleteID();
		CharMove();
		SetID();
		break;

	case ORDER_RIDE:
		DeleteID();
		SetID();
		break;

	case ORDER_TAKEIN:
		DeleteID();
		SetID();
		break;

	case ORDER_RIDEOUT:
		DeleteID();
		SetID();
		break;

	case ORDER_MOVERIDEOUT:
		DeleteID();
		SetID();
		break;


	case ORDER_MOVENRIDEOUTNATTACK:
		DeleteID();
		SetID();
		break;

	case ORDER_EXTINGUISH:
		DeleteID();
		Special(SPECIAL_EXTINGUISH);
		SetID();
		break;

	case ORDER_ATTACK:
		
		DeleteID();

		// 공격 전에 해야 할 처리를 완료해야 공격에 들어간다. 
		if(PreAttack()==TRUE)
		{
			// 공격이 종료되면,
			if(Attack() == FALSE)
			{


				OrderStop(m_NewID, BY_COMPUTER);
			}
		}

		SetID();
		break;

	case ORDER_BETRAY:
		DeleteID();
		Betray();
		SetID();
		break;

	case ORDER_MINE:
		DeleteID();
		Mine();
		SetID();
		break;

	case ORDER_WARP:
		DeleteID();
		Warp();
		SetID();
		break;

	case ORDER_ABSORB:
		DeleteID();
		Absorb();
		SetID();
		break;

	case ORDER_MAGIC8J:
		DeleteID();
		Special(SPECIAL_MAGIC8J);
		SetID();
		break;

	case ORDER_SHIELD:
		DeleteID();
		Shield();
		SetID();
		break;


	case ORDER_SELFEXP:
		DeleteID();
		if(SelfExp()==TRUE)
		{
			Dead();
			return FALSE;
		}
		SetID();
		break;

	case ORDER_GOINTOHQ:
		DeleteID();
		
		if(GoIntoHQ()==TRUE)
		{
			   Dead();
			   return FALSE;
		}

		SetID();
		break;
	case ORDER_MANABURN:
		DeleteID();
		SpecialAtb4(SPECIAL_MANABURN);
		SetID();
		break;

	case ORDER_DOUBLE_SWORD:
		DeleteID();
		SpecialAtb4(SPECIAL_DOUBLE_SWORD);
		SetID();
		break;
	case ORDER_LONG_SPEAR:
		DeleteID();
		//SpecialAtb4(SPECIAL_LONG_SPEAR);
		LongSpear();
		SetID();
		break;
	case ORDER_ICE_BARRIER:
		DeleteID();
		SpecialAtb4(SPECIAL_ICE_BARRIER);
		SetID();
		break;
	case ORDER_FLOWER_NEEDLE_RAIN:
		DeleteID();
		SpecialAtb4(SPECIAL_FLOWER_NEEDLE_RAIN);
		SetID();
		break;
	case ORDER_SOUL_CHANGE:
		DeleteID();
		SpecialAtb4(SPECIAL_SOUL_CHANGE);
		//Betray();
		SetID();
		break;
	case ORDER_STONE_MAGIC:
		DeleteID();
		SpecialAtb4(SPECIAL_STONE_MAGIC);
		SetID();
		break;
	case ORDER_GOD_SPEAR:
		DeleteID();
		SpecialAtb4(SPECIAL_GOD_SPEAR);
		SetID();
		break;
	case ORDER_CLOUD_WIND:
		DeleteID();
		SpecialAtb4(SPECIAL_CLOUD_WIND);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_FIRE_ROUND_DANCE:			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_FIRE_ROUND_DANCE);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_NOVA:						// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_NOVA);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_FURY_OF_VOLCANO:				// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_FURY_OF_VOLCANO);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_POISONING_HEAVENS:			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_POISONING_HEAVENS);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_BOMB_OF_HEAVEN:				// 승천포(중국-발석거, 이령 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_BOMB_OF_HEAVEN);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_SIEGE_MODE:	// 불랑기포차=>앙천대소 변신(중국-불랑기포차, 순비연 2차 전직 기술)
//		DeleteID();
//		SpecialAtb4(SPECIAL_SIEGE_MODE);
//		SetID();
		ChangeModeRoarOfLaughter();
		break;

	// robypark (2004/06/4 14:13)
	case ORDER_SIEGE_MODE_UNDO:	// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 순비연 2차 전직 기술)
//		DeleteID();
//		SpecialAtb4(SPECIAL_SIEGE_MODE);
//		SetID();
		ChangeModeFarangi();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_ONSLAUGHT_FIREBALL:			// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
		DeleteID();
		SpecialAtb4(SPECIAL_ONSLAUGHT_FIREBALL);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_FIRERAIN_OF_GOD:				// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
		DeleteID();
		SpecialAtb4(SPECIAL_FIRERAIN_OF_GOD);
		SetID();
		break;

	default:
		{
			char buffer[FILE_NAME_SIZE];
			wsprintf(buffer, "There is No order. kind:%d order:%d", m_cltCharStatus.GetKind(), CurrentOrder);
		    SetMessage(buffer);

		}
		break;
		
	}

	// 화면에 찍을 좌표를 구한다. 
	CalculateRealXY();

	return TRUE;

}

BOOL _Char::StartRecallGurdianCaw(_NewID id,char host)
{
	m_dwCawTimer  = timeGetTime();		
	m_siCawNumber = 1;	
	
	m_idCaw.ID     = id.ID;
	m_idCaw.Unique = id.Unique;
	m_hostCaw      = host;

	return TRUE;
}

void cltCharOrg::SetGuardianStartImage(SI16 m_siGuardianKind)
{
	switch(m_siGuardianKind)
	{
		case ON_GUARDIAN_MOUSE:			m_GuardianStartFile	= FILE_ON_GUARDIAN_MOUSE_START;		break;
		case ON_GUARDIAN_CAW:			m_GuardianStartFile	= FILE_ON_GUARDIAN_CAW_START;		break;
		case ON_GUARDIAN_TIGER:			m_GuardianStartFile	= FILE_ON_GUARDIAN_TIGER_START;		break;
		case ON_GUARDIAN_RABBIT:		m_GuardianStartFile	= FILE_ON_GUARDIAN_RABBIT_START;	break;
		case ON_GUARDIAN_DRAGON:		m_GuardianStartFile	= FILE_ON_GUARDIAN_DRAGON_START;	break;
		case ON_GUARDIAN_SNAKE:			m_GuardianStartFile	= FILE_ON_GUARDIAN_SNAKE_START;		break;
		case ON_GUARDIAN_HORSE:			m_GuardianStartFile	= FILE_ON_GUARDIAN_HORSE_START;		break;
		case ON_GUARDIAN_SHEEP:			m_GuardianStartFile	= FILE_ON_GUARDIAN_SHEEP_START;		break;
		case ON_GUARDIAN_MONKEY:		m_GuardianStartFile	= FILE_ON_GUARDIAN_MONKEY_START;	break;
		case ON_GUARDIAN_CHICKEN:		m_GuardianStartFile	= FILE_ON_GUARDIAN_CHICKEN_START;   break;
		case ON_GUARDIAN_DOG:			m_GuardianStartFile	= FILE_ON_GUARDIAN_DOG_START;		break;
		case ON_GUARDIAN_PIG:			m_GuardianStartFile	= FILE_ON_GUARDIAN_PIG_START;		break;
	}
}

SI16 cltCharOrg::GetGuardianStartImage()
{
	return m_GuardianStartFile;
}

void _Char::GuardianBreakless(SI16 siKind)
{
	switch(siKind)
	{
		case ON_GUARDIAN_MOUSE:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_RABBIT:
			DeleteGuardianRabbit();
			break;
		case ON_GUARDIAN_CAW:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_HORSE:
			DeleteGuardianHorse();
			break;
		case ON_GUARDIAN_SNAKE:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_SHEEP:
			DeleteGuardianLifeUp();		
			break;
		case ON_GUARDIAN_PIG:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_CHICKEN:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_MONKEY:
			DeleteRecallGuardianChar();
			break;
		case ON_GUARDIAN_DOG:
			DeleteGuardianDamageResistUp();	
			break;
	}
}

BOOL _Char::ActionPig()
{
	// 수호부(돼지)가 공격을 가한후 3초동안 아무런 동작도 안한다.
	if(m_bPigAttack)
	{
//		OrderStop(m_NewID, BY_COMPUTER);
		//CharDrawInfo.SetAnimation(ANI_WAIT0);
		//m_MoveDelay		=	0;
		//m_AttackDelay	=	0;
		if((timeGetTime() - m_dwPigAttackDelay) >= 3000)
		{
			m_bPigAttack = FALSE;
		}
		return FALSE;
	}

	// 수호부(돼지) 공격을 받은 몬스터는 3초동안 아무런 동작도 안한다.
	if(m_bPigDefence)
	{
		if((timeGetTime() - m_dwPigAttackDelay) < 3000)		return TRUE;

		// 이전의 세팅을 원래대로 돌려놓는다.
		m_bPigDefence = FALSE;
		m_cltCharDrawInfo.SetDrawSwitch(TRUE);
		
		// 확률에 따라 삼켰던 몬스터를 영원히 지워버린다.
		_NewID RecallID = CR[AttackID.ID].HostID;
		SI16 siKind = TransKindImjinToOnline(m_cltCharStatus.GetKind());
		int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;
		int siHitrate = (GD.Random()%100);
		
		BOOL bKillMonster = FALSE;
		if(siMonsterType == ON_MONSTER_TYPE1)
		{
			if(siHitrate < CR[RecallID.ID].m_siGuardianNHitrate)
				bKillMonster = TRUE;
		}
		else if(siMonsterType == ON_MONSTER_TYPE2)
		{
			if(siHitrate < CR[RecallID.ID].m_siGuardianBHitrate)
				bKillMonster = TRUE;
		}
		else if(siMonsterType == ON_MONSTER_TYPE3)
		{
			if(siHitrate < CR[RecallID.ID].m_siGuardianSHitrate)
				bKillMonster = TRUE;
		}
		else
		{
			if(siHitrate < CR[RecallID.ID].m_siGuardianNoHaltHitrate)
				bKillMonster = TRUE;
		}

		if(!bKillMonster)	return FALSE;	

		if(!IsAlive(RecallID))	return FALSE;

		// 무도장이 아니어야 한다. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == TRUE)	return FALSE;

		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
			MyNet.PlayersdpId.dpId[CR[RecallID.ID].GetPlayerNumber()], CR[RecallID.ID].m_siOnlineID, 
			MyNet.PlayersdpId.dpId[CR[m_NewID.ID].GetPlayerNumber()], CR[m_NewID.ID].m_siOnlineID);
		
		// 킬러에게 합당한 경험치등을 제공한다. 
		pGame->pBattle->ExpOperate(RecallID.ID, m_NewID.ID);

		// 캐릭터 삭제
		DeleteChar(m_NewID.ID);
		
		return FALSE;
	}
	return FALSE;
}

#ifdef _DEBUG

// robypark 2004/8/4 18:42
// 치트키 관련 플래그 변수 선언
bool g_bShowGrid		= false;	// 맵에 GRID 그리기
bool g_bShowID			= false;	// 캐릭터 ID 그리기
bool g_bShowMapIdx		= false;	// 전투 맵 위에 캐릭터가 존재할 경우 맵의 인덱스 좌표를 그려지도록 할 것인지에 대한
bool g_bShowArrayIdx	= false;	// 캐릭터가 위치하는 변수 내부 배열 인덱스를 보이도록 설정해준다.

// robypark 2004/8/4 18:42
// 전투 중 치트키 관련 처리
void ProcessCheatMessage(char * szCheatMessage)
{
	CHAR			cSep[]	= " ,";
	CHAR			*pToken = NULL;
	SI32			siParameter1 = 0;
	SI32			siParameter2 = 0;
	SI32			siParameter3 = 0;

	if (szCheatMessage[0] != '/')
		return;

	// 명령어를 얻어온다.
	if ((pToken = strtok(szCheatMessage, cSep)) == NULL)
		return;		

	// 전투 맵의 GRID를 보이도록 할 것인지에 대한 명령처리
	if (stricmp(pToken, "/SHOWGRID") == 0)
	{
		g_bShowGrid = !g_bShowGrid;
	}
	// 전투 중 캐릭터의 ID를 그려지도록 할 것인지에 대한 명령처리
	else if (stricmp(pToken, "/SHOWID") == 0)
	{
		g_bShowID = !g_bShowID;
	}
	// 전투 맵 위에 캐릭터가 존재할 경우 맵의 인덱스 좌표를 그려지도록 할 것인지에 대한 명령처리
	else if (stricmp(pToken, "/SHOWMAPIDX") == 0)
	{
		g_bShowMapIdx = !g_bShowMapIdx;
	}
	// 캐릭터가 위치하는 변수 내부 배열 인덱스를 보이도록 설정해준다.
	else if (stricmp(pToken, "/SHOWARRAYIDX") == 0)
	{
		g_bShowArrayIdx = !g_bShowArrayIdx;
	}
	// 전투 중의 캐릭터를 사망한 것으로 처리하도록 한다. 단지 전투 처리에서 제외될 뿐 실제로 죽은 것은 아니다.
	// 2차 장수에게는 통하지 않는다. 다만 그려지기만 하며 조종이 되지 않는다.
	else if (stricmp(pToken, "/KILL") == 0)
	{
		// 변경할 캐릭터의 ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// 체력 값을 0으로 설정하여 죽은 것으로 속인다.
		CR[siParameter1].m_ParmeterBox.IP.SetLife(0);
	}
	// 캐릭터의 체력값을 설정해준다.
	// 전투 내에서만 일시적으로 적용된다.
	else if (stricmp(pToken, "/SETLIFE") == 0)
	{
		// 변경할 캐릭터의 ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// 새로 설정할 체력 수치
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter2 = atoi(pToken);

		// 새로 설정할 체력 수치
		// 최대값보다 큰 값으로 설정할 경우 최대 체력값이 설정된다.
		CR[siParameter1].m_ParmeterBox.IP.SetLife(siParameter2);
	}
	// 캐릭터의 마나값을 설정해준다.
	// 전투 내에서만 일시적으로 적용된다.
	else if (stricmp(pToken, "/SETMANA") == 0)
	{
		// 변경할 캐릭터의 ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// 새로 설정할 마나 수치
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter2 = atoi(pToken);

		// 최대값보다 큰 값으로 설정할 경우 최대 마나값이 설정된다.
		CR[siParameter1].m_ParmeterBox.IP.SetMana(siParameter2);
	}
	// 맵의 특정 위치에 불을 낸다.
	else if (stricmp(pToken, "/MAKEFIRE") == 0)
	{
		// 맵의 X좌표
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// 맵의 Y좌표
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter2 = atoi(pToken);

		_NewID zeroID;
		zeroID.ID = -1;
		zeroID.Unique = 0;

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				Map.PutFire(siParameter1 + i, siParameter2 + j, 1, GROUNDFIRE_LIFETIME, zeroID, -1);
			}
		}

//		Map.PutFire(siParameter1, siParameter2, 1, GROUNDFIRE_LIFETIME, zeroID, -1);
	}
}

#endif