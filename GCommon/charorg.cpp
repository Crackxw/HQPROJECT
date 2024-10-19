//--------------------------------------------------------------------
//  ��� : ���°�
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

extern	_MainData				GD;									// ������ �ֵ����� 
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
extern	SHORT 					WaterBlock;
extern	SHORT 					BasicBlock;
extern	_Item 					Item[];
extern	_Order 					Order[];
extern	SI32					UserPlayerNumber;					// ����� ���� 
extern	_Text 					Text;								// �ؽ�Ʈ 
extern	_Map 					Map;
extern	SHORT 					MapEditMode;						// ���� ���� ����ΰ�?
extern	clock_t					CurrentClock;						// �ð� ���� 
extern	_Char					CR[];								// ĳ������ ������ 
extern	_MyNetGame				MyNet;
extern	cltMapInterface			clifMap;							// �ʰ��� �������̽� 
extern	_NewID 					ZeroID;
extern	cltCharInterface		clifChar;							// ĳ���� ���� �������̽� 
extern	CCharFileLoadManager	g_FileLoadManager;				// ������ ĳ����
extern	_OnlineBattleMsgQue		OBMQue;
extern	CItem					*g_pItem;
extern	int 					FlyVibrateData[];
extern	int 					MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
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

	m_cltCharDrawInfo.SetDrawFrame(-1);                         // ������ �׸��� �׸� ������ 

	// robypark 2004/6/14 15:35
	// �ױ� ���� �� ĳ���Ͱ� ������ �Ǿ� �־����� ��Ÿ���� �÷���
	m_bAlreadySelectedBeforeDead = FALSE;

	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		m_scSeeMode[i]	=	TRUE;           // ���̴� �����ΰ� ������� �����ΰ�
	}

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);

	//////////////////////////////////////////////////
	//char-wait.cpp�� if(GD.Player[GetPlayerNumber()].PI.Host != BY_PLAYER) �κ� �Բ� ����.
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

	//��ȣ��
	m_bGuardianChar		= FALSE;
	m_bUseGuardian		= false;
	m_siCount			= 0;
	m_dwGuardianTime	= 0;

	m_dwCawTimer		= 0;
	m_siCawNumber		= 0;

	m_siGuardianMaxLife		= 0;
	m_siGuardianLife		= 0;

	// ����
	m_dwPigAttackDelay = 0;
	m_bPigAttack = m_bPigDefence = FALSE;

	// ��
	m_ChickenID[0].Init();
	m_ChickenID[1].Init();
	m_ChickenID[2].Init();
	m_siChickPrevKind = 0;

	m_ssX		=	x;		
	m_ssY		=	y;
	m_scXsize	=	1; 
	m_scYsize	=	1;
	m_scIDXsize = m_scIDYsize = 1;


	Group = -1;                                // ���õ� ���� ����. 


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

	// ���� �ִϸ��̼� ������ ����
	m_siMagicAniStep = 0;

	// ��� ���� 
	GuardX=-1;
	GuardY=-1;

	// ���� ����
	CarcassDelay=100;
	CarcassDelayor=0;

	// �ǰݰ��� 
	AttackMePlayer=-1;
	AttackMeID.Init();

	FireID.Init();

	// ������ �⺻ ������ 1����. 
	MaxWaitType = 1;
	WaitType = ANI_WAIT0;

////////////////////////////////////////////////////////////	
	m_cltCharStatus.SetKind(kind);
	m_cltCharStatus.SetPlayerNumber(pnumber);
	m_cltCharStatus.SetOriginalPlayerNumber(pnumber);

	// �߿� �Ķ��Ÿ ������ ���´�. 
	if(pip)
	{
		m_ParmeterBox.SetImportantParameta(*pip);
	}
	else 
	{
		clGrp.Error("No Info", "test:0001");
	}

	// ���� ���¸� ���´�. 

	HeroInit();

	if(pmw)
	{
	
		SI16			siEquip;
		CItemHeader		*pItemHeader;

		MW = *pmw;

		// ����						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_ARMOR_ITEM), siEquip );
		SetEquip(TRUE, ON_WEAR_ARMOR_ITEM, (SI32)siEquip, FALSE);
		// ���						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_HELMET_ITEM), siEquip );
		SetEquip(TRUE, ON_WEAR_HELMET_ITEM, (SI32)siEquip, FALSE);
		// ����						
		g_pItem->GetItemInfo( MW.GetWearItem(ON_WEAR_WEAPON_ITEM), siEquip );				
		SetEquip(TRUE, ON_WEAR_WEAPON_ITEM, (SI32)siEquip, FALSE);
		m_ssWeaponID = MW.GetWearItem(ON_WEAR_WEAPON_ITEM);
		//��ȣ��
		m_siGuardianID = MW.GetWearItem(ON_WEAR_GUARDIAN_ITEM);
		
		if(m_siGuardianID)
		{
			pItemHeader = g_pItem->GetItem(m_siGuardianID);

			m_siGuardianKind		 = pItemHeader->m_pGuardian->m_siKind;
			m_siGuardianCondition	 = pItemHeader->m_pGuardian->m_siStartCondition;
			m_siGuardianSkillNum     = pItemHeader->m_pGuardian->m_siSkillNum;

			//����Ҽ� �ִ� ����� 2����� ���� �Ѱ��� ����.
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

			//����ð��� ������ �ִ� ���� ����̶��.
			if(m_siGuardianAliveTime > 0)
				m_siGuardianTimeOut  = TRUE;
			else 
				m_siGuardianTimeOut = FALSE;

				//��ȣ�θ� �����ϰ� �ִ�.
			m_bGuardianEquip = true;

			SetGuardianStartImage(m_siGuardianKind);
		}
	}
	

	m_ParmeterBox.Action(g_pItem, MW);

	// m_ParmeterBox.Action()�Լ� ���Ŀ� ó���Ǿ�� �Ѵ�. 
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

	// �����ϰ� ���ĺ��δ�.
	if(IsAtb(ATB_TRANSPARENT)==TRUE)
	   m_cltCharDrawInfo.SetTransparentSwitch(TRUE);

	if(IsAtb(ATB_FLY))
	{
		m_cltCharStatus.SetArea(AREA_AIR);
	}
	else
	    m_cltCharStatus.SetArea(AREA_LAND);


	// ����Ʈ �ִϸ��̼��� �����Ѵ�. 
	m_cltCharDrawInfo.SetAnimation(ANI_WAIT0);

	m_siAttackRange1 = KI[m_cltCharStatus.GetKind()].GetAttackRange(0);
	m_siAttackRange2 = KI[m_cltCharStatus.GetKind()].GetAttackRange(1);

	// robypark 2004/10/18 12:36 - �� ���� ���� �Ӽ��� ���� ��� �ΰ����� ó�� ���� ���� ����
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

	// �ٸ� ����ڿ��� ó������ ������Ʈ �� �ʿ�� ����. 
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

	// ������ ����� ���� ������ ���� ���� ����� ��������� ���Ѵ�. 
	
	// ���ο� ����� �켱������ �� ���ϰ�
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



	// ������ ��쿡 
	if(Order[order].Atb&ORDERATB_MAGIC)
	{
		// �������� ���ڶ��, 
		if(m_ParmeterBox.IP.GetMana() < Order[order].Para1)
		{
			mode=FALSE;
		}
		else mode=TRUE;
	}

	switch(menu)
	{
		// ������ ���� ������ ä������ Ȱ��ȭ�ȴ�. 
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
			// �� ���� ���̸� ����� �� ����. 
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

	// ������ ����� ���� ������ ���� ���� ����� ��������� ���Ѵ�. 
	
	// ���ο� ����� �켱������ �� ���ϰ�
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
// ����� ���ۿ� �ִ´�. 
// ����ڰ� �� ����� �̹� ���۾ȿ� �ִ� ��쿡�� ��ǻ�Ͱ� �� ����� ���� �ʴ´�. 
BOOL cltCharOrg::PushOrder(_OrderDataInfo order, SHORT mode, SHORT debuginfo)
{

//////////////////////////////////////////////////////////////////////////////
//  ������� ����� �� ������ ���� �ԷµǾ� ��� ���¿� �ִµ� ���� ���� �������� 
// �۾��� �����ϰ� ���� ����� ������ ����ڰ� ���� ����� ���õǴ� ������ �ִ�. 
// �̷� ��쿡�� ���� ���� ����� �����ؾ� �Ѵ�. 
/////////////////////////////////////////////////////////////////////////////

	// �̹� ������� ����� �ְ� 
	// ���� ����� ���ٸ�, 
	// ���� �� ����� �����Ѵ�. 
	if(BufferOrder.Order!=ORDER_WAIT 
	&& BufferOrder.HostMode!=BY_COMPUTER
	&& order.HostMode==BY_COMPUTER)
		return FALSE;
		
	BufferOrder=order;
	PushOrderMode=mode;    // �ڱ��� ���̴°�?


	return TRUE;
}

// ���� ����� �����Ѵ�. 
void cltCharOrg::ClearReserveOrder()
{
	ReserveOrderIndex=0;
}

// ���� ����� ������ ���� ���۷� �ű��. 
void cltCharOrg::PopReserveOrder()
{
	SHORT i;

	// ���� ���۰� ��� �־�� �Ѵ�. 
	if(BufferOrder.Order!=ORDER_WAIT)return ;

	// ����� ����� ������ �������� �ʴ´�. 
   if(ReserveOrderIndex<=0)return ;  

   PushOrder(ReserveOrder[0], TRUE, 10);
   
   for(i=0;i<(ReserveOrderIndex-1);i++)
   {
	   ReserveOrder[i]=ReserveOrder[i+1];
   }

   ReserveOrderIndex--;
}

// ����� ���� ���ۿ� �ִ´�. 
BOOL cltCharOrg::PushReserveOrder(_OrderDataInfo order)
{
	// �ִ밪�� ������ ������ ������ۿ� ��� �ִ´�. 
	if(ReserveOrderIndex>=MAX_BUFFER_ORDER_NUMBER)
		ReserveOrderIndex=MAX_BUFFER_ORDER_NUMBER-1;

	ReserveOrder[ReserveOrderIndex]=order;
	ReserveOrderIndex++;

	return TRUE;
}

// �־��� ����� ���� ���ۿ� �̹� ��� �ִ��� Ȯ���Ѵ�. 
BOOL cltCharOrg::HaveReserveOrder(SHORT order)
{

	SHORT i;

	for(i=0;i<ReserveOrderIndex;i++)
	{
		if(ReserveOrder[i].Order==order)return TRUE;
	}
	return FALSE;
}


// ���� �������� ����� �����̾����Ŀ� ���� �������� ������ �����ش�. 
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

// ������� ���θ� �˷��ش�. 
BOOL cltCharOrg::IsGeneral()const 
{
		if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL) && m_siTimerDelay ==0)return TRUE;
		return FALSE;
}


// ĳ���Ͱ� x, y��ġ�� floor���� ���� �� �ִ°� ���°�?
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

// ĳ���Ͱ� x, y��ġ�� �� ���� �� �ִ°� ���°�?
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


// ���� ��ġ x,y���� �̷������ ��ġ�� ���Ѵ�. 
void cltCharOrg::OperateMapPosition(SHORT x, SHORT y)
{

	  // �̵��ϴ� ��ġ�� �ڿ��� �ִ��� Ȯ���Ѵ�. 
	  if(Map.GetTreasureMap(m_ssX,m_ssY))
	  {
	 	 GetTreasure(Map.GetTreasureMap(m_ssX, m_ssY));
	 	 Map.SetTreasureMap(m_ssX, m_ssY, 0);

		 if(GetPlayerNumber() == UserPlayerNumber)
			PushEffect(EFFECT_GETITEM, m_ssX, m_ssY);
	  }

	  // �̵��ϴ� ��ġ�� ���ڰ� �ִ��� Ȯ���Ѵ�. 
	  // �Ʊ��� ��ġ�� ���ڴ� �������. 
	  /*
	  if(IsEnemyMine(X, Y, GetPlayerNumber()))
	  {
		  // ���ڸ� ��������� ó���� �Ѵ�. 
		  StepEnemyMine(X, Y, GetPlayerNumber());
	  }
	  */
}

BOOL cltCharOrg::SetSelect(int mode, int effectswitch)
{
	int i;
	int number=-1;

	// ���� ���� ��忡���� ĳ���͸� �����ϰų� ������ �� ����. 
	if(MapEditMode==TRUE)return FALSE;

	// �̹� ���õǾ� �ִ� ���� �ƴ��� Ȯ�� �Ѵ�. 
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		
		if(GD.SelectedID[i].IsSame(m_NewID))
		{
			number=i;
		}
	}

	if(mode==TRUE)
	{
		// �̹� ���� ĳ���ʹ� ������ �� ����. 
		if(m_ParmeterBox.IP.GetLife() <= 0)return FALSE;

	   // ���õ� �ð� 
	   SelectedClock=CurrentClock;

		// �̹� ���õ� ĳ�����̸�, 
       if(number>=0)return FALSE;


		UpdateMenu(ORDER_INITMENU);

	   // ���ýÿ� ȿ���� ��Ÿ���� �ϴ� ��� 
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
		// ���õǾ� ���� ���� ĳ���Ͷ�� 
       if(number<0)return FALSE;

	   for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	   {
			if(GD.SelectedID[i].IsSame(m_NewID))
			{
				GD.SelectedID[i].Init();
				GD.SelectedNumber--;
				
				// ���̻� ���õ� ���� ����������, 
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

// �α��� �� �ڸ��� ������ ���� ���´�. 
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

	// ���� ������ Ȯ���Ѵ�. 
	SI32 siType = (treasure & 0xff00) >> 8;

	// ���� �м��Ѵ�. 
	SI32 siAmount = treasure & 0x00ff;

}

// ���� �� �ڸ��� �������� ���� ���´�.
BOOL cltCharOrg::ReleaseItem()
{

	return TRUE;
}

BOOL cltCharOrg::SelectOperation()
{
	// �������̰ų� ������ ���θ��� ������ ȿ������ ���� �� �ִ�.
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


	// ü���� �ö��ٴ� ����� ������ �˷��ش�. (�¶��� ������ ���. )
	if(GetPlayerNumber() == UserPlayerNumber)							// ����� ������ ü���� ���� �Ǿ��� ��.
	{
		// ��ŷ ����� ���������� �����Ѵ�. 
		if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
		{
			OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetLife());
		}
	}
	
	return TRUE;
}

BOOL cltCharOrg::ReduceLife(SHORT amount, SHORT unique)
{

	// ���� ��ġ�Ǿ� �ִٸ�, 
	// ���� ���� �ջ�� �� ü���� ���ϵȴ�. 
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

	

	// ü���� �������ٴ� ����� ������ �˷��ش�. (�¶��� ������ ���. )
	if(GetPlayerNumber() == UserPlayerNumber)							// ����� ������ ü���� ���� �Ǿ��� ��.
	{
		// �������� �ƴϾ�� �Ѵ�. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			// ��ŷ ����� ���������� �����Ѵ�. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
				
				// 2�� ����� �״� ��� ���� ����� �����ؾ� �Ѵ�.
				if(m_ParmeterBox.IP.GetLife() == 0 && siCharType == 3)
				{
					// robypark 2004/6/10
					// ��õ����� ���¿��� ���� ��� �ִϸ��̼��� �Ҷ��������� �״� �ִϸ��̼��� ����ؾ� �Ѵ�.
					if (m_cltCharStatus.GetKind() == KIND_ON_ROAR_OF_LAUGHTER)
					{
						// robypark 2004/6/14 15:37
						// 2�� ����� �װ� �� �ڿ� �����ϴ� 1�� ����� ������ ������ �����ؾ��ϴ��� ����
						// 2�� ����� ����ڰ� ������ ������ ��� 1�� ����� ���õ� ������ �����ϱ� ����.
						m_bAlreadySelectedBeforeDead = m_scSelectedSwitch;

						// �Ҷ��������� ��� �����Ѵ�.
						CR[m_NewID.ID].ChangeModeFarangi_Immediately();
					}

					// robypark 2004/6/14 15:37
					// 2�� ����� �װ� �� �ڿ� �����ϴ� 1�� ����� ������ ������ �����ؾ��ϴ��� ����
					// 2�� ����� ����ڰ� ������ ������ ��� 1�� ����� ���õ� ������ �����ϱ� ����.
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

	// �������� �������ٴ� ����� ������ �˷��ش�. (�¶��� ������ ���. )
	if(GetPlayerNumber() == UserPlayerNumber)							// ����� ������ ü���� ���� �Ǿ��� ��.
	{
		// ��ŷ ����� ���������� �����Ѵ�. 
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

	// �������� �������ٴ� ����� ������ �˷��ش�. (�¶��� ������ ���. )
	if(GetPlayerNumber() == UserPlayerNumber)							// ����� ������ ü���� ���� �Ǿ��� ��.
	{
		// �������� �ƴϾ�� �Ѵ�. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			// ��ŷ ����� ���������� �����Ѵ�. 
			if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
			{
				OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(MyNet.PlayersdpId.dpId[ GetPlayerNumber() ], m_siOnlineID, m_ParmeterBox.IP.GetMana());
			}
		}
	}
	return TRUE;
}

// ĳ������ ������ ��ȯ�Ѵ�. 
BOOL cltCharOrg::ChangeDirectionNormal(SHORT destdir)
{
	Direction=destdir;
	TempDirection=destdir;

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);

	return TRUE;
}

// ĳ������ ������ ��ȯ�Ѵ�. 
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

	// ������ �ش�. 
	ChangeDirectionDelayor++;
	if(ChangeDirectionDelayor<ChangeDirectionDelay)
		return FALSE;
	else
		ChangeDirectionDelayor=0;

	m_scBusySwitch=TRUE;

	// ������ �����Ѵ�. 
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

	// �ݽð� �������� ȸ�� 
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


// ���ְ� ������ ���� ���͸� ���Ѵ�. 
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

//���� ��ǥ�� ���� ��ǥ���� ����� �� ������ ���Ѵ�. 
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


// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� (���� ��ǥ)
// length: �߽ɰ��� �Ÿ� 
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


// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� 
BOOL cltCharOrg::IsPtInObj(SHORT x, SHORT y)
{
	RECT rect = m_cltCharDrawInfo.GetRealImageRect();

	if(m_ssDrawStartX+rect.left<x && x<m_ssDrawStartX+rect.right
	&& m_ssDrawStartY+rect.top<y && y<m_ssDrawStartY+rect.bottom)
		return TRUE;

	return FALSE;
}

// � rect���� ������ �����ϴ°�?
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

// � rect���� ������ �����ϴ°�?
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
// ��� ���� 
//-----------------------------------------------------------------------------------
// �߼��� ȥ �ð� ���� �Լ�.

// ���� ���� �Լ�. 
void cltCharOrg::SetHalt(SI32 haltmode, SI32 haltdelay)
{
	//��ȣ��[����]�� ���� ĳ������ ��� ������ �ɸ��� �ʴ´�.
	if(m_bPigDefence)
		return;

	SI16 siKind;
	
	siKind = TransKindImjinToOnline(CR[m_NewID.ID].m_cltCharStatus.GetKind());

	int siMonsterType = pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->siMonsterType;


	//siMonsterType == 4 ���� ���̸鼭 ���ڼ��� ���� �ʴ� ���.
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

// ���ڵ� ������ ��� �� ����� �����ش�. 
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
	// õ������ �ñ⿡ ���� ���� smoke, ����� �⺻ ���ݿ� ���� ���� smoke
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
//  �� �ڸ� ã�� �Լ� 
//-----------------------------------------------------------------------------
// ���ڸ��� ã�� x, y�� �־� �ش�.
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

				 // ĳ���͸� ��ġ�� �� �ִ� ������ �ִ� �������� Ȯ���Ѵ�. 
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
			  
			  // ĳ���͸� ��ġ�� �� �ִ� ������ �ִ� �������� Ȯ���Ѵ�. 
			  if(Map.IsEmpty(tempx, tempy, xsize, ysize, m_ssX, m_ssY, m_scIDXsize, m_scIDYsize)==TRUE)
			  {
				  // �ٸ� ���� ������� ������, 
				  if(diff_level_mode == FALSE)
				  {
					  // �ǹ��� ���� ���� �����̾�� �Ѵ�. 
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

		 // ���ڸ��� ������, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // ���ڸ��� ������, 
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
		   
		   // ĳ���͸� ��ġ�� �� �ִ� ������ �ִ� �������� Ȯ���Ѵ�. 
		   // �̹� ������ ��ġ�� ���̾ �ȵȴ�.
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
	   
	   // ���ڸ��� ������, 
	   if(min_length==10000)
	   {
		   return FALSE;
	   }
	   else  // ���ڸ��� ������, 
	   {
		   x=min_x; y=min_y;
		   return TRUE;
	   }
   return FALSE;
}


// �� ���� �ڸ��� �������� ��ó ���. 
BOOL cltCharOrg::FireOperation(int x,int y)
{

	// ���¿� ����Ͽ� ���ظ� �ش�. 
	// �ƹ��� ������ ���� ĳ���Ͷ� �������� ����ϴ�. 

	// ���� ����� ���� ���� ������ ���� ������ ó���Ѵ�. 
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
	// �÷��̾�� �ٴ� �ҿ� ���� �������� ���� �����ʴ´�. ���ǻ� ����
	if((IsFriendToByPlayerNumber(siPlayerID, GetPlayerNumber()) == TRUE)  
		|| ISPLAYER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)
		return TRUE;
	
	
	SI32 apower = CalcApowerByAtb(max(40, m_ParmeterBox.GetTotalDpower() * 3 / 10), ATTACKATB_FIRE, m_uiAtb);

	CalculateHit(enemyid, siPlayerID, ATTACKATB_FIRE, 0, AREA_LAND, apower, m_NewID, m_ssX, m_ssY, 70/*Ȯ��*/);

	return TRUE;
}


//------------------------------------------------------------------------------
// �ΰ����� ����. 
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

	// ��ȿ�� ������ ������ �ִ� ���� �ƴϸ�, 
	if( Map.IsInMapArea(m_ssX, m_ssY) == FALSE )	return;
	
	GlobalBlackCheck(m_ssX, m_ssY, GetTotalSearchRange(), TRUE, GetPlayerNumber());
	
}

// �þ߸� ó���Ѵ� - �ϱ��� ��� ����
void cltCharOrg::BlackCheckHeroMode()
{
	DWriteStringToFile("11");
	SI32	x, y, x2, y2, i, j;
	BOOL	bBreak;
	SI32	angle = 45;
	SI32	cosValue, sinValue;

	// �� ���� ��ǥ
	POINT	basisPos[3] = {{-1, -1}, {-1, -1}, {-1, -1}};
	SHORT	edgePos[MAX_MAP_XSIZE];
	SHORT	tempPos[MAX_MAP_XSIZE * MAX_MAP_YSIZE];
	SI32	tempX, tempY;
	SHORT	edgePosNum1, edgePosNum2;	// basisPos[0]�� [1]���̿� �ִ� ���� ����, [1]~[2]
	SHORT	tempPosNum;	

	// ��ȿ�� ������ ������ �ִ� ���� �ƴϸ�, 
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

	// ������������ ��θ� ���Ѵ�. 
	FindStraightLine((SHORT)basisPos[0].x, (SHORT)basisPos[0].y, (SHORT)basisPos[1].x, (SHORT)basisPos[1].y, 
		edgePosNum1, edgePos, MAX_MAP_XSIZE * MAX_MAP_YSIZE);
	FindStraightLine((SHORT)basisPos[1].x, (SHORT)basisPos[1].y, (SHORT)basisPos[2].x, (SHORT)basisPos[2].y, 
		edgePosNum2, &edgePos[edgePosNum1 * 2], MAX_MAP_XSIZE * MAX_MAP_YSIZE);
	DWriteStringToFile("44");

	// �þ߸� ǥ���ϴ� ��� ������
	for(i = 0; i < (edgePosNum1 + edgePosNum2) * 2; i += 2)
	{
		FindStraightLine(tempX, tempY, edgePos[i], edgePos[i + 1], tempPosNum, tempPos, MAX_MAP_XSIZE * MAX_MAP_YSIZE);
		
		bBreak = FALSE;
		for(j = 0; j < (tempPosNum * 2); j += 2)
		{
			// ���� ���� �ֺ�(4�� ����) ��� �˻�
			for(y = tempPos[j + 1] - 1; y <= tempPos[j + 1]; y++)
			{
				for(x = tempPos[j] - 1; x <= tempPos[j]; x++)
				{
					if((max(abs(x - m_ssX), abs(y - m_ssY)) > 2) && ((IsColl(x, y, BLOCK_CASTLE | BLOCK_WALL | BLOCK_TREE) == TRUE) || (GD.LastLevel[x][y] >= GD.LastLevel[m_ssX][m_ssY] + 2))) 
					{
						// ��ֹ� �ֺ��� ��� �Ѵ�. (���� �׶��̼ǿ� ���� ��Ӱ� ��Ÿ���� ������ �����ϱ� ����)
						for(y2 = y - 1; y2 <= y + 1; y2++)
						{
							for(x2 = x - 1; x2 <= x + 1; x2++)
							{
								if(Map.IsInMapArea(x2, y2) == TRUE)
								{
									// ���� ȣ������ �÷��̾��� ���׵� ���� ������.
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
							// ���� ȣ������ �÷��̾��� ���׵� ���� ������.
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



// ���� Ž�� :��ó�� ���� �ִ��� ���θ� �����Ѵ�. 
BOOL cltCharOrg::SearchForward()
{

	SHORT		i;
	SHORT		id=0;
	SHORT		length=0;
	
	
	SHORT		x=0, y=0;

	SHORT		search_range;

	if( CanWork() == FALSE )	return FALSE;

	search_range = GetTotalSearchRange();


	// ���⿡ ó�� �Ʊ��� ���� �� �ִ� ����. 
	SI32 helprange = 0 ;

	if(GD.Player[GetPlayerNumber()].PI.Host == BY_COMPUTER)
	{
		helprange = search_range; 
	}
	else
	{
		helprange = 4;
	}


	// ���� ���� 
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
				// �߰��� ���� ���̰� �ź����� ���� �ƴϸ�, (���� �߰��� 50%�� Ȯ����..)
				if( IsFriendToByID(m_NewID.ID, id)==FALSE && (GD.Random() % 2) )
				{
					// �Ÿ��� ������ (�ٷ� ��ĭ) ���δ�. 
					if(max(abs(m_ssX-x) , abs(m_ssY-y)) <= 1 ||  CR[id].AmbushSwitch == FALSE)
					{
						// ���� ������ ���̰� �����.
						CR[id].SetDiscover(GetPlayerNumber());
						
						if(OperateFindEnemy(x, y, CR[id].m_NewID)==TRUE)return 0;
					}
				}
				// ������ �ʿ��� �Ʊ� ĳ���Ͷ��, 
				else if(max(abs(m_ssX-x) , abs(m_ssY-y)) <= helprange
					&& abs(CR[id].BeAttackedFrame-GD.Frame) < 200 
					&& IsSee(CR[id].PrevAttackMeID)==TRUE)
				{
					// �Ʊ��� �����ϴ� ���� ���ش�. 
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

// ���� �� �غ� �Ǿ� �ִ°�?
BOOL cltCharOrg::CanWork()
{

   // ����� �ִ� ���̶��, 
   if(m_scSeeSwitch==FALSE)return FALSE;

   return TRUE;
}


// ���� �߰����� ��� ��� ��ó�ϴ��� �����Ѵ�. 
// TRUE:���� �߰��ϰ� ������ ��ġ�� ���ߴ�. 
// FALSE:������ ��ġ�� ���� �� ���� ����̴�. 
BOOL cltCharOrg::OperateFindEnemy(SHORT x, SHORT y, _NewID id)
{

	SHORT length=0;

	if(m_scSeeSwitch==FALSE)return FALSE;

	if(IsSee(id.ID)==FALSE)return FALSE;

	// �߰��� ĳ���Ϳ��� ���� �������� ������ �ƴϸ�. 
	if(IsFriendToByID(m_NewID.ID, id.ID) == TRUE)return FALSE;


	// ��� ��尡 �����Ǿ� �ִٸ� ��Ÿ� ���� ĳ���ʹ� ������ �� ����. 
	if(HoldSwitch==TRUE)
	{
		if(IsInRange(id.ID, GetAttackRange())==FALSE)return FALSE;
	}

	// ���� ��� �����̸�, 
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
 
	// ���� �������̰� 
	// ��ǻ�� �÷��̾��̰ų� ��ǻ�� ����̸�, 
	if(CurrentOrder==ORDER_ATTACK 
	&& (GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER
	    || HostMode==BY_COMPUTER)  )
	{
		// ���ο� ���� ������ �� �ִٸ�, 		
		if(CanAttack(id)==TRUE)
		{
			// ���ο� ���� �����ϴ� ���� �� �����ϴٸ� ���ο� ���� �����Ѵ�. 
			if(IsGoodAttackNewEnemy(id)==TRUE)
			{
			   OrderAttack(m_NewID, x, y, id, BY_COMPUTER);
			   return TRUE;
			}
		}
	}
	// ��ü������ �̵��߿� ���� �߰��ϸ�, 
	// �����߿� ������, 
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
	
	
	// ��ǥ�� ��ǥ 
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
		// 45*45=2025  �� Ÿ���� ������ 45��Ʈ�� �����ߴ�. 
		
		if((range*range*2025) >  ( (CR[id].m_cltCharStatus.GetCenterX()-m_cltCharStatus.GetCenterX())*(CR[id].m_cltCharStatus.GetCenterX()-m_cltCharStatus.GetCenterX()) + (CR[id].m_cltCharStatus.GetCenterY() - m_cltCharStatus.GetCenterY())*(CR[id].m_cltCharStatus.GetCenterY()-m_cltCharStatus.GetCenterY() )))return TRUE;
		return FALSE;
	}
	
	
	
	return FALSE;
}


// idĳ���͸� ������ �� �ִ��� ���θ� �˷��ش�. 
BOOL cltCharOrg::CanAttack(_NewID id)
{
	// �� �ڽ��� ���� ���� ���¶��, 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// ȭ�鿡 �� �� ���� ĳ���Ͷ�� ������ �� ����. 
	if(IsSee(id.ID)==FALSE)return FALSE;

	// �ڱ� �ڽ��� ������ �� ����.
	if(id.IsSame(m_NewID))return FALSE;

	// ���� ���� �Ӽ��� ���� ����ĳ���Ͷ�� ������ �� ����. 
	if(IsAtb(ATB_ATTACK)==FALSE && CR[id.ID].m_cltCharStatus.GetArea()==AREA_LAND)return FALSE;

	
	// ���� ���� �Ӽ��� ���� ���� ĳ���Ͷ��, 
	if(IsAtb(ATB_ATTACKAIR)==FALSE && CR[id.ID].m_cltCharStatus.GetArea()==AREA_AIR)return FALSE;

	// �����ϴ� �����̰� ���� ���ӿ� �ִٸ� ���� �Ұ�. 
	if(SwimSwitch == TRUE)return FALSE;


	// �Ź̿䱫 �Ź��� �����̰� ������ �����ϸ�, ���ݺҰ�. 
	if(GetAttackAtb() == ATTACKATB_WIDOW && m_ParmeterBox.IP.GetMana() < MAGICPOWER_WIDOW)return  FALSE;

	// �ĵ������̰� ������ �����ϸ�, ���ݺҰ�. 
	if(GetAttackAtb() == ATTACKATB_WAVE  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_WAVE)return  FALSE;

	// ������ �Ҹ��ϴ� �Ұ����̰� ������ �����ϸ�, ���ݺҰ�. �Ұ��縮
	if(GetAttackAtb() == ATTACKATB_FIREUSINGMANA  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_FIRE)return  FALSE;

	// �Ŵ�� �������� ����
	if(GetAttackAtb() == ATTACKATB_BIGTREE  && m_ParmeterBox.IP.GetMana() < MAGICPOWER_BIGTREE)return  FALSE;

	// robypark 2004/9/1 15:14
	// �⺻ ���ݼӼ� �߿� ���ڰ� ������ �ϴ� ��� ���� �˻�
	// �Ű��� ȥ, ���� �繫����, ���� �Ҳ�
	if ((GetAttackAtb() == ATTACKATB_KIMBOMB) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_KIMBOMB))
		return FALSE;

	// robypark 2004/9/1 15:47
	// �⺻ ���ݼӼ� �߿� �������� ������ �ϴ� ��� ���� �˻�
	// ¡�⽺ĭ�� ȥ, ������
	if ((GetAttackAtb() == ATTACKATB_MAGIC4T) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_MAGIC4T))
		return FALSE;

	// robypark 2004/11/11 11:20
	// ��õ��� ������ �� �����Ҹ��ϵ��� ������ �Ǿ���. ���ݿ� �ʿ��� ������ ���Ҵ��� �˻�
	if ((GetAttackAtb() == ATTACKATB_ROAR_OF_LAUGHTER) && (m_ParmeterBox.IP.GetMana() < MAGICPOWER_SIEGE_MODE_FIRE_MISSILE))
		return FALSE;

	//��ȣ��(��)�� ������ �� ������ ������ ������ �ٸ� ��븦 ã�´�.
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

// ���� �������� ���� id�߿��� id�� �����ϴ� ���� �� ���ڴ°�?
BOOL cltCharOrg::IsGoodAttackNewEnemy(_NewID id)
{

	if(IsSee(id.ID)==FALSE)return FALSE;
	if(IsSee(AttackID.ID)==FALSE)return TRUE;

	// ���� �����ϴ� ĳ���Ͱ� ���� ������ �� ���µ� ����Ǿ� �ְ� ���ο� ĳ���Ͱ� ���� ������ �� �ִ�  ĳ���Ͷ��
	// ���ο� ĳ���͸� �����Ѵ�. 
	if(CR[AttackID.ID].CanAttack(m_NewID)==FALSE || CR[AttackID.ID].m_siHaltDelay)
	{
		if(CR[id.ID].CanAttack(m_NewID)==TRUE)
		{
		   return TRUE;
		}
	}
	// ���� �����ϴ� ĳ���Ͱ� ���� ������ �� �ִٸ�, 
	else
	{
		// ���ο� ĳ���Ͱ� ���� ������ �� ���ٸ�, 
		if(CR[id.ID].CanAttack(m_NewID)==FALSE)
		   return FALSE;
	}

	// ���� �������� ĳ���Ͱ� �����ε� ���ο� ĳ���Ͱ� ����ĳ���Ͷ��, 
	if(CR[AttackID.ID].IsAtb(ATB_ATTACK|ATB_ATTACKAIR)==FALSE
	&& CR[id.ID].IsAtb(ATB_ATTACK|ATB_ATTACKAIR)==TRUE)return TRUE;

	// ���� �Ѵ� ĳ���ͺ��� ���ο� ĳ���Ͱ� ������, (��Ÿ� �ȿ� ������,)
	if(IsInRange(AttackID.ID, GetAttackRange())==FALSE && IsInRange(id.ID, GetAttackRange())==TRUE)
	{
		return TRUE;
	}

	// ���ο� ĳ���Ϳ��� �Ÿ��� ���� ĳ������ �Ÿ����� ������ TRUE
	SI32 length1 = max( abs(CR[id.ID].m_ssX-m_ssX), abs(CR[id.ID].m_ssY-m_ssY));
	SI32 length2 = max( abs(CR[AttackID.ID].m_ssX-m_ssX), abs(CR[AttackID.ID].m_ssY-m_ssY));
	if(length1 < length2)
	{
		return TRUE;
	}


	return FALSE;

}

// idĳ���͸� ������ �� �ִ��� ���θ� �˷��ش�. 
BOOL cltCharOrg::CanAttack(SHORT x, SHORT y)
{
	// �� �ڽ��� ���� ���� ���¶��, 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// ���� ���� �Ӽ��� ������ ������ �� ����. 
	if(IsAtb(ATB_ATTACK)==FALSE)return FALSE;

	
	return TRUE;
}

//--------------------------------------------------------------------------------------------
// �ִϸ��̼� ����. 
//--------------------------------------------------------------------------------------------
void cltCharOrg::SetAnimation(SI32 ani, SI32 *panistep)
{
	m_cltCharDrawInfo.SetAnimation(ani);

	m_cltCharDrawInfo.SetpAniStep(panistep);

	// ������ ĳ������ ���
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		// ĳ������ �ൿ ����(AniAction)
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

	// ������ ĳ������ ���
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		// ���⿡ ���� �ൿ�� �� �ִ� ���� Ŭ�������� ���´�.
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


		// �ѹ��� �ִϸ��̼� �ϰ� �Ǿ� �ִ� ���, 
		// �������� ������ ���ٸ�, 
		// ���̻� �ִϸ��̼��� ������ �ʴ´�. 
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
	// ������ ĳ������ ���
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		return g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalFrame[AniAction][CharBaseWClass][CharWeaponEnum] - 2;
	}
	else
		return KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].siInstant;
}


// ���߿��� ���ڸ��� ã�´�. 
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
				 
			 // ĳ���͸� ��ġ�� �� �ִ� ������ �ִ� �������� Ȯ���Ѵ�. 
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

		 // ���ڸ��� ������, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // ���ڸ��� ������, 
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
				 // ĳ���͸� ��ġ�� �� �ִ� ������ �ִ� �������� Ȯ���Ѵ�. 
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

		 // ���ڸ��� ������, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // ���ڸ��� ������, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	SetEquip()
// Description	:	ĳ���Ϳ��� �������� ������Ų��.
// Parameter	:	bEquip			- FALSE�̸� ��������
//					siEquipKind		- ��������
//					siEquipID		- ������ ���̵�, -1�� ��� ����Ʈ ������ ����
//					bEffectSound	- ������ ȿ������ �� ���ΰ� 
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
		// �׸��� �� ���� �����Ƿ�.
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

	// CharBaseWClass ����
	CharBaseWClass	= g_FileLoadManager.GetHeroClassCode(&g_FileLoadManager.m_HeroCurrEquip[m_NewID.ID]);	
	return TRUE;
}



// ĳ������ ����ġ�� �÷��ְ� ���� ������ ����� �뺸�Ѵ�. 
void cltCharOrg::IncreaseExp(SI32 exp, BOOL serversendswitch, BOOL showresultswitch)
{
	// ��� �÷��̾��ΰ�쿡�� ����ġ�� ������ �ȴ�. 
	if(ISPLAYER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) == TRUE)
	{

		
		// ������ ����ġ�� �����ϴ� ��쿡��, 
		if(serversendswitch == TRUE)
		{
			// ����ڿ� ���� ĳ������ ��쿡��, 
			if(GetPlayerNumber() == UserPlayerNumber)
			{
				// ��ŷ ����� ���������� �����Ѵ�. 
				if(m_ParmeterBox.IP.CheckCheckSum() == TRUE)
				{
//					for(int i = 0 ; i < 100 ; i++)
//					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[GetPlayerNumber() ], m_siOnlineID, exp);
//					}
				}
				else
				{
					SetMessage("��ŷ�� ���ɼ��� �ִ� �������Դϴ�.(Find Possibility of Cracking Data!)");
					m_ParmeterBox.IP.SetLife(0);
				}
			}
		}

		// ����ġ ����� ��� �������� �̷������, 
		if(m_ParmeterBox.IP.IncreaseExp(exp) == TRUE)
		{
			if(GetPlayerNumber() == UserPlayerNumber)
			{
				// ȿ����
				PushEffect(EFFECT_LEVELUP, m_ssX, m_ssY);
				
				CR[m_NewID.ID].SetLevelUpDraw(TRUE);
			}

			// ������ �÷��ش�. 
			m_ParmeterBox.IP.SetLevel(m_ParmeterBox.IP.GetLevel() + 1);

			//ü�°� �������� �ְ�� ä���ش�. 
			m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
		}

		// ����kind�� �¶��� kind�� ��ȯ�Ѵ�.
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
			// �߱� �뺴 - ������ => ��� �����簡 �ǵ��� �߰�
			case KIND_ON_MAGIC_C:		kind = TransKindImjinToOnline(KIND_ON_MAGIC_C_UP);	break;
			// robypark 2004/8/26 17:52
			// ��� �뺴 �߰�
			// �븸 �뺴 - �߼����� => ��� �߼�����
			case KIND_ON_BEASTWARRIOR:	kind = TransKindImjinToOnline(KIND_ON_BEASTWARRIOR_UP); break;
			// �븸 �뺴 - ���»�   => ��� ���»�
			case KIND_ON_YUMRUCK:		kind = TransKindImjinToOnline(KIND_ON_YUMRUCK_UP); break;
			// �Ϻ� �뺴 - �����   => ��� �����
			case KIND_ON_COSMIC:		kind = TransKindImjinToOnline(KIND_ON_COSMIC_UP); break;
			// ���� �뺴 - �Ǽ���   => ��� �Ǽ���
			case KIND_ON_MEDICALMAN:	kind = TransKindImjinToOnline(KIND_ON_MEDICALMAN_UP); break;
			// ���� �뺴 - Į����   => ��� Į����
			case KIND_ON_BUTCHER:		kind = TransKindImjinToOnline(KIND_ON_BUTCHER_UP); break;
			// �߱� �뺴 - ��â���� => ��� ��â����
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
								
	// �ι��ڸ� �־��ش�. 
	m_szName[namelength] = NULL;
}


// ĳ������ �̸��� ���´�. 
char* cltCharOrg::GetName()const
{
	// �÷��̾� �̰� ù���� ĳ�����̸� ���ΰ��̱� ������ ���ΰ��� �̸��� ǥ���Ѵ�. 
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

// Ư�� �������� ĳ���͸� �о��. 
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

	// �̵������� ��� �ִٸ�,
	if(IsColl(x, y, Block) == FALSE)
	{
		m_ssX = x;
		m_ssY = y;

		// �� �������� �з�����.
		return TRUE;
	}

	// �� ������ �ƴ϶� �з����� �ʾҴ�.
	return FALSE;
}

// robypark 2004/6/1 17:11
// Ư�� �������� ĳ���͸� �о��. 
// SI32 dir[in]: ����
// SI32 siDistance[in]: �о �Ÿ�
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

	// �̵������� ��� �ִٸ�,
	if(IsColl(x, y, Block) == FALSE)
	{
		m_ssX = x;
		m_ssY = y;

		// �� �������� �з�����.
		return TRUE;
	}

	// �� ������ �ƴ϶� �з����� �ʾҴ�.
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// ������ ĳ���� ���� �ʱ�ȭ
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
	Direction				= SOUTH|EAST;				// �⺻ ������ ������ (�� �Ʒ���)


	m_siPrevArmorDrawID		= -1;
	m_siPrevHelmetDrawID	= -1;
	m_siPrevWeaponDrawID	= -1;

	// ����Ʈ �� �ڵ带 �������� �� ���ΰ� ����
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
	// �̹��� ���� ����� �̸� �����Ѵ�.
	siTotalPiece	= g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siTotalPiece[AniAction][CharBaseWClass];
	bExist = FALSE;
	for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
	{
		// ���� Bodypart�� ���´�.
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



// ��ü���� ���� ���� �ð��� ���Ѵ�. 
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
// ĳ���� ���� �ʱ�ȭ
void _Char::CharInit(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid)
{
	ZeroMemory(this, sizeof(_Char));
	m_ssPreWeaponID = -1;



	Init(kind, id, unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid);

	// ������ ĳ���Ͱ� �ƴ� �Ϲ� ĳ������ ��쿡 �̹��� ����� ���Ѵ�. ������ ĳ���ʹ� �Ʒ� HeroInit() �Լ����� ���Ѵ�.
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE )
	{
		ImageXsize = GetFileXsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
		ImageYsize = GetFileYsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
	}

	m_cltCharDrawInfo.SetLastAniFrame(100);


	// �� ĳ������ Ư¡�� ���� �ٸ� ������ �ο��Ѵ�. 

	// �����̸� ������� ������ �����Ѵ�.
	if(m_siOnlineID == 0 && GetPlayerNumber()==UserPlayerNumber)
	{
		GD.HomeX=m_ssX;
		GD.HomeY=m_ssY;
	}

	UpdateMenu(ORDER_INITMENU);



	SetFileNFont();
	SetInfo();

	// ��ġ�� ���� ������ �Ѵ�. 
	CalculateRealXY();

	Action();
}

// robypark 2004/6/9 12:32
// ĳ���Ͱ� ������ �� ���
// �� �Լ��� CharInit(...)�Լ��� �����ϴ�. ���� SetInfo()�Լ��� ȣ������ �ʴ´�.
//	SHORT CharOrderNumber: ������ �迭���� ��� ��ġ�� �ִ°�?
void _Char::CharInit_DISGUISE(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID)
{
	ZeroMemory(this, sizeof(_Char));
	m_ssPreWeaponID = siPrevWeaponID;

	CharOrderNumber = usCharOrderNumber; // ������ �迭���� ��� ��ġ�� �ִ°�?

	Init(kind, id, unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid);

	// ������ ĳ���Ͱ� �ƴ� �Ϲ� ĳ������ ��쿡 �̹��� ����� ���Ѵ�. ������ ĳ���ʹ� �Ʒ� HeroInit() �Լ����� ���Ѵ�.
	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) == FALSE )
	{
		ImageXsize = GetFileXsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
		ImageYsize = GetFileYsize(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[1].siFile);
	}

	m_cltCharDrawInfo.SetLastAniFrame(100);


	// �� ĳ������ Ư¡�� ���� �ٸ� ������ �ο��Ѵ�. 

	// �����̸� ������� ������ �����Ѵ�.
	if(m_siOnlineID == 0 && GetPlayerNumber()==UserPlayerNumber)
	{
		GD.HomeX=m_ssX;
		GD.HomeY=m_ssY;
	}

	UpdateMenu(ORDER_INITMENU);

	SetFileNFont();
//	SetInfo();	// => �� �Լ� ������ ĳ���� ������ �����Ǿ� ������ ĳ���Ϳ� ���� ������ �ߺ����� ���Եȴ�.
				//   �̷� ���Ͽ� �̵��ӵ��� ���ݼӵ��� �����Ǵ� ���װ� �߻��Ѵ�.

///////////////////////////////////////////////////////////////////////////

	// robypark 2004/07/09 11:29
	// 2�� ����� �װ� �����ϴ� 1�� ����� ������ ���ΰ� ĳ���Ͱ� ������
	// �ȵǴ� ������ �߻��Ѵ�.
	// SetInfo() �Լ����� ó���ϴ� �� �߿� 
	// �ڵ� GD.CharOrder[ GD.CharOrderIndex ]=m_NewID.ID;
	// �� ���Ͽ� ���ݼӵ��� �̵��ӵ� ���������� �߻��Ͽ�����,
	// �ش� �ڵ忡�� ���� ���ŵǴ� ĳ���� ������ �������� �ʾƼ�
	// ���ΰ� ĳ���� ������ �ȵǴ� ������ �߻��Ѵ�.
	// SetInfo() �Լ��� ����� �����ϸ� ���� ���� �ڵ常 �ٸ���.
    GD.IncreaseCharNumber(GetPlayerNumber(), m_cltCharStatus.GetKind());

	GD.CharInfo[m_NewID.ID]=m_cltCharStatus.GetKind();

	GD.CharOrder[ CharOrderNumber ]=m_NewID.ID;	// ���ο� ĳ���� ������ �ƴ϶� ������ ĳ���Ϳ��� �����̹Ƿ�
												// ����Ʈ�� �߰��� �ʿ����.

	// �� ���� ������Ű�� ������, 2�� ����� �׵� �����ϴ� 1�� ����� ������, �� ĳ���͵��� �������� �ʰԵǴ� ���װ� �߻��Ѵ�.
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

	// ��ġ�� ���� ������ �Ѵ�. 
	CalculateRealXY();

	Action();
}


// ���� ��ġ�� ����Ѵ�. 
void _Char::CalculateRealXY()
{
	SHORT x=m_ssX-m_scIDXsize/2+m_scIDXsize-1;
	SHORT y=m_ssY-m_scIDYsize/2+m_scIDYsize-1;
	
	//��鸲�� �ִ´�. 
	if(m_cltCharDrawInfo.GetVibrateSwitch() ==TRUE)
	{
		SI32 data = FlyVibrateData[((GD.Frame+m_NewID.ID)/4)%MAX_FLY_VIBRATE];
	    m_cltCharDrawInfo.SetVibrateY( data );
	}

	// ĳ������ �߽��� ���Ѵ�
	m_cltCharStatus.SetCenterX(MapCenterX[m_ssX][m_ssY]+RX);

	// ����ü�� �߽��� �����. 
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
// Description	:	ĳ���͸� ���� ��ǥ(m_ssDrawStartX, m_ssDrawStartY)�� ����Ѵ�. 
void _Char::CalculateDrawStartXY()
{
	SHORT centerx = 0, centery = 0;

	SHORT x = m_ssX - m_scIDXsize/2 + m_scIDXsize - 1;
	SHORT y = m_ssY - m_scIDYsize/2 + m_scIDYsize - 1;


	// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (ȭ����ǥ)
	Map.GetCenterXYByMapCenterXY(x, y, centerx, centery);
	
	m_ssDrawStartX = centerx - ImageXsize/2 + RX;
	
	// ����ü�� �߽��� �����. 
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
// ���� ��ġ�� ����Ѵ�. 
void _Char::CalculateDrawStartXYInMapCaptureMode()
{
	SHORT centerx=0, centery=0;

	SHORT x=m_ssX-m_scIDXsize/2+m_scIDXsize-1;
	SHORT y=m_ssY-m_scIDYsize/2+m_scIDYsize-1;
	

	// ����ü�� �߽��� �����. 
	if(IsAtb(ATB_FLY))
	{
		centerx=MapCenterX[x][y];
		centery=MapCenterY[x][y];

	    m_ssDrawStartX=centerx-ImageXsize/2+RX;
	    m_ssDrawStartY=centery-ImageYsize/2+RY + m_cltCharDrawInfo.GetVibrateY();
	}
	else
	{
		// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (ȭ����ǥ)
		centerx=MapCenterX[x][y];
		centery=MapCenterY[x][y];

	    m_ssDrawStartX=centerx-ImageXsize/2+RX;
		m_ssDrawStartY=centery-ImageYsize+KI[m_cltCharStatus.GetKind()].m_ssBottomIndent+RY + m_cltCharDrawInfo.GetVibrateY();
	}
}


// id�� ���� �ּ� �����ȿ� �ִ��� Ȯ���Ѵ�. 
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




// ���÷��� �˷��ش�.
unsigned char _Char::GetTeamColor()const 
{
	return TeamColor[GetPlayerNumber()];
}

// pnumber�� �÷��̾�� ���� ���̰� �Ѵ�. 
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

// ������ ���ݹް� ������ ȣ���ϴ� �Լ� 
void _Char::SetBeAttackedSwitch(SHORT enemyid, SHORT attackmeplayer)
{
	if(enemyid<=0)return ;

	// ��ȿ���� ���� �÷��̾� ��ȣ�� �Ѿ���� ����. 
	if(IsValidPlayerNumber(attackmeplayer) == FALSE)
	{
		return ;
	}

	AttackMePlayer=attackmeplayer;

    if(IsSee(enemyid)==FALSE)return ;
	AttackMeID=CR[enemyid].m_NewID;
	BeAttackedSwitch=TRUE;

    // ������ �ǰݽ� �Ʊ��� ������ ��û�Ѵ�. 
    //GD.SetNeedHelpSwitch(GetPlayerNumber(), ID);

	NeedHelpSwitch=TRUE;
    NeedHelpDelay=MAX_NEED_HELP_DELAY;

}

// ������ ���ݹ������� ��ġ 
void _Char::BeAttackedOperation()
{
	int  x=0,  y=0;
	int dx=0, dy=0;
	_NewID attackmeid;

	// �ٸ� �Ϸ� �ٻ� ���¸� ó���� �� ����. 
	if(m_scBusySwitch==TRUE) return ;

	// ���� �޴� ���� �ߴܵǸ�, 
	if(BeAttackedSwitch==FALSE)return ;

	if(GD.Frame % 15)return ;

	attackmeid=AttackMeID;
	AttackMeID.Init();
	BeAttackedSwitch=FALSE;



	// ���� �����ϴ� ���� ��ȿȭ�Ǹ�,
	if(IsSee(attackmeid.ID)==FALSE)
	{
		return ;
	}
	// ���� �����ϴ� ���� �Ʊ��̸�, 
	if(IsFriendToByID(m_NewID.ID, attackmeid.ID))
	{
		return ;
	}

	// ���� �����ϴ� ���� ��Ҽӿ� ������. 
	x= CR[attackmeid.ID].GetX();
	y= CR[attackmeid.ID].GetY();
	if(GD.BlackMap[GetPlayerNumber()][x][y] != 0)
	{
		// ������ �� �ִ� ĳ���Ͷ��, 
		// ���� �������� �̵��� ����. 
		if(CanAttack(attackmeid) == TRUE)
		{
			OrderMove(m_NewID, x, y, BY_COMPUTER);
		}
		else
		{
			return ;
		}
	}


	// ���� ���� �������� �����Ѵ�.
	BeAttackedFrame=GD.Frame;
	// ���� �����ϴ� ������ �����Ѵ�. 
	PrevAttackMeID=attackmeid;



	// ���� �۾��� ������ �� ���� ���� �ƴ϶��, 
	
	
	// �������� ���� 
    if((Order[CurrentOrder].Info&ORDERINFO_NORETREAT)==0
	&& CurrentOrder==ORDER_WAIT                          // ���� Ư���� ����� �������� �ƴ϶��, 
	&& HoldSwitch==FALSE                                 // ��� ��尡 �ƴϾ�� �Ѵ�.
	&& CanAttack(attackmeid)==FALSE                      // ���� ������ �� ���� ĳ���Ͷ�� 
	&& CR[attackmeid.ID].AttackID.IsSame(m_NewID))
	{
		// ���ݹ���  �������� �����Ѵ�. 
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

	// ���� �����ϴ� ���� �����Ѵ�. 
	OperateFindEnemy(CR[attackmeid.ID].GetX(), CR[attackmeid.ID].GetY(), attackmeid);

}


void _Char::OldOperation()
{

	if(CurrentOrder==ORDER_WAIT)
	{
	   // ������ �ϴ� ������ �ִٸ� 
	   if(OldOrder.Order!=ORDER_WAIT)
	   {
	    	// ���� ������ �����̶��,
	        if(OldOrder.Order==ORDER_ATTACK)
	        {
			   OldOrder.Order=ORDER_WAIT;
			   OrderAttack(m_NewID, LOWORD(OldOrder.Para1), HIWORD(OldOrder.Para1), OldOrder.Para2, BY_COMPUTER);
	        }
	        // ���� ������ �̵��̶��, 
	        else if(OldOrder.Order==ORDER_MOVE)
	        {
		        OldOrder.Order=ORDER_WAIT;

				// ��� ��尡 �����Ǿ� �����ʾƾ� �̵��� �� �ִ�. 
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





// ĳ���Ͱ� �����Ǿ����� �˸���. 
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





// ĳ������ ������ �����. 
BOOL _Char::SetID()
{
	 int i, j;


	 if(m_scSeeSwitch==FALSE)return FALSE;


	 // �����ÿ��� �ڽ��� �� ��ġ���� �浹 ǥ�ø� �Ѵ�. 
	 if(MovePercent==0)
	 {
		 // ����ĳ���͸� �浹�� üũ�Ѵ�. 
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
				   // �������  ������ �ִ´�. 
				   if(clifMap.GetSkyMap(m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i)==0)
				      Map.SetSkyMap(m_NewID.ID, m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i);
				}

		 }
	 }
	 // �̵��� 
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
				   // �������  ������ �ִ´�. 
				   if(clifMap.GetSkyMap(m_ssX-m_scIDXsize/2+j, m_ssY-m_scIDYsize/2+i)==0)
				      Map.SetSkyMap(m_NewID.ID, m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
				}

		 }
	 }
	 
	SHORT darkstep;
	 
	// ���� ������ ǥ�õ��� �ʵ��� �����.
	m_cltCharDrawInfo.SetCharSmallMapDrawSwitch(FALSE);
	
	
	for(i=0;i<m_scIDYsize;i++)
		for(j=0;j<m_scIDXsize;j++)
		{

			// ���� �������� �׷��־�� �ϴ��� Ȯ���Ѵ�.
			
			// ��� ��ο� �ܰ迡 �ִ°� ?
			darkstep=GD.BlackMap[UserPlayerNumber][m_ssX][m_ssY];

			// ���� ���� ������ ĳ���͸� �׷��־�� �Ѵ�. 
			if(darkstep < FOG2_DARK_STEP)
			{

				// ���� �������� seemode�� �����Ǿ� �־�� �Ѵ�. 
			    if(m_scSeeMode[ UserPlayerNumber ]==TRUE)
				{
				    m_cltCharDrawInfo.SetCharSmallMapDrawSwitch(TRUE);
				}
			}
		}







	SetIDSwitch=TRUE;

	return TRUE;
}


// ĳ������ ������ ���ش�. 
BOOL _Char::DeleteID()
{
	int i, j;

	if(m_scSeeSwitch==FALSE)return FALSE;

	// ����ĳ�����̸�, 
	if(m_cltCharStatus.GetArea() == AREA_LAND)
	{
	 for(i=0;i<m_scIDYsize;i++)
		for(j=0;j<m_scIDXsize;j++)
		{
			Map.ClearIDMap(m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
			Map.DeleteCharCollMap(m_ssX+j-m_scIDXsize/2, m_ssY+i-m_scIDYsize/2);
		}
	}
	// ����ĳ�����̸�, 
	else
	{
		// �ڽ��� ������ ����Ǿ� �ִٸ�, 
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




//  ���� �Ķ��Ÿ�� �ʱ�ȭ�Ѵ�. 
BOOL _Char::ParametaAction()
{
	SHORT			i;
	SHORT			apower = 0;
	SHORT			dpower = 0;
	CItemHeader		*pItemHeader;

#ifndef _SKILL_
	SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
#endif

	// ���⿡ ���� ���� �Ӽ��� �ٲ۴�.
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
			// 2�� ����� ���⸦ �����ص� �ش� ���� �Ӽ����� �ٲ��� �ʴ´�.
			if (siCharType != 3)
				SetAttackAtb(pItemHeader->pWeaponHeader->siAttackAtb);
#endif

		// 2�� ��� ����� �߰��Ǿ��� �� ���
		#ifdef _SKILL_
			SetAttackAtb(pItemHeader->pWeaponHeader->siAttackAtb);

			// robypark 2004/6/16 10:27
			// 2�� ����� ���⸦ �����Ͽ��� ���, 2�� ����� ���� ����� ������� ���Ѵ�.
			// ��) �ź���-����������, ������-�ݳ뿰��, �Ҷ�������-��õ���, ȭ����-�Ͱ�ȭ����
			SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));
			if (siCharType == 3)
			{
				Atb4 = 0;	// 2�� ��� ����� ��� Atb4 �Ӽ��� �����Ǿ� �ִ�.
							// 0���� �����Ͽ� ����� �����Ѵ�.

				// ȭ������ ��� 2���� ���ݼӼ��� �ִ�. ���� â ���ݰ� ���Ÿ� ȭ����� ������
				// ������ �ִµ�, ���������� ���Ÿ� ���ݵ� ������ ��찡 �߻��Ѵ�.
				// �̸� �������ؼ� ���Ÿ� ���ݼӼ��� AttackAtb2�� 0���� �������ش�.
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


		// �����ۿ� ���� ��Ÿ��� �����Ѵ� 
#ifndef _SKILL_
		// 2�� ����� ������ ���⿡ ���� ��Ÿ��� ������� �ʴ´�.
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
			// Ư����� �Ӽ� ����
			switch(pItemHeader->pWeaponHeader->uiMagic)
			{
			case ON_ITEM_MAGICATTACK_KILLSHOT:		m_uiAtb3 |= ATB3_SPECIALATTACK;		break;		// �ϰ��ʻ�
			case ON_ITEM_MAGICATTACK_SEISMIC:		m_uiAtb3 |= ATB3_EARTHQUAKE;		break;		// ������
//			case ON_ITEM_MAGICATTACK_HIDDEN:		m_uiAtb3 |= ATB2_HIDE;				break;		// ���ż�
			case ON_ITEM_MAGICATTACK_VITALSTEEL:	m_uiAtb3 |= ATB3_ABSORBLIFE;		break;		// �����
			case ON_ITEM_MAGICATTACK_MOONAXE:		m_uiAtb3 |= ATB3_MAGIC5J;			break;		// ����������
			case ON_ITEM_MAGICATTACK_FIREBLOW:		m_uiAtb3 |= ATB3_KIMBOMB;			break;		// ���ڰ�
			case ON_ITEM_MAGICATTACK_ASTRAFIRE:		m_uiAtb3 |= ATB3_MAGIC4;			break;		// �������
			case ON_ITEM_MAGICATTACK_FIREARROW:		m_uiAtb3 |= ATB3_FIREARROW;			break;		// ��ȭ��
			case ON_ITEM_MAGICATTACK_MAGICARROW:	m_uiAtb3 |= ATB3_FIREARROW2;		break;		// ������ȭ��
			case ON_ITEM_MAGICATTACK_WEDGEATTACK:	m_uiAtb3 |= ATB3_MAGIC8K;			break;		// �������
			case ON_ITEM_MAGICATTACK_BOLT:			m_uiAtb3 |= ATB3_LIGHTNING;			break;		// ������
			case ON_ITEM_MAGICATTACK_ASTRALFIRE:	m_uiAtb3 |= ATB3_MAGIC4;			break;		// �������
			case ON_ITEM_MAGICATTACK_TORNADO:		m_uiAtb3 |= ATB3_TORNADO;			break;		// ǳ���
			case ON_ITEM_MAGICATTACK_HOLD:			m_uiAtb3 |= ATB3_MAGIC6C;			break;		// ���ڼ�
			case ON_ITEM_MAGICATTACK_RUNNINGFIRE:	m_uiAtb4 |= ATB4_RUNNINGFIRE;		break;		// ������ź.
			case ON_ITEM_MAGICATTACK_ENERGYPA:		m_uiAtb4 |= ATB4_ENERGYPA;			break;		// ��ǳ.
			case ON_ITEM_MAGICATTACK_LONG_SPEAR:	m_uiAtb4 |= ATB4_LONG_SPEAR;		break;		// �ְ˳���
			case ON_ITEM_MAGICATTACK_DOUBLE_SWORD:	m_uiAtb4 |= ATB4_DOUBLE_SWORD;		break;		// ��â��ȯ.
			case ON_ITEM_MAGICATTACK_ICE_BARRIER:	m_uiAtb4 |= ATB4_ICE_BARRIER;		break;		// ������.

			case ON_ITEM_MAGIC_FLOWER_NEEDLE_RAIN:	m_uiAtb4 |= ATB4_FLOWER_NEEDLE_RAIN;break;		// ��õȭ��
			case ON_ITEM_MAGIC_SOUL_CHANGE:			m_uiAtb4 |= ATB4_SOUL_CHANGE;		break;		// ���Ǽ�
			case ON_ITEM_MAGIC_STONE_MAGIC:			m_uiAtb4 |= ATB4_STONE_MAGIC;		break;		// ������
			case ON_ITEM_MAGIC_GOD_SPEAR:			m_uiAtb4 |= ATB4_GOD_SPEAR;			break;		// ��â�뵵
			case ON_ITEM_MAGICATTACK_MONKEYSTICK:	m_uiAtb3 |= ATB3_MAGIC_RECALL_MONKEY;		break;		// ���Ǻ�
			}

			// ��������� ����
/*			if(pItemHeader->pWeaponHeader->uiMagic)
			{
				if(IsGeneral())		// ���
				{
					if(MagicApplyRate < pItemHeader->pWeaponHeader->siMagicAttack)
						MagicApplyRate = pItemHeader->pWeaponHeader->siMagicAttack;
				}
				else	MagicApplyRate = pItemHeader->pWeaponHeader->siMagicAttack;
			}*/
		}
	}

	// ������������ ���,  
	if(IsAtb3(ATB3_MAGIC8K)== TRUE) 
	{
		// �������� �����ϸ� ���⿡ ���� �����Ÿ��� 1�̴�. 
		if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_MAGIC8K)
			m_ParmeterBox.SetItemAttackRange(1);
		else 
			m_ParmeterBox.SetItemAttackRange(5);
	}
	// ��ŷ�� ����� �ִ� ���, 
	if(m_ParmeterBox.Action(g_pItem, MW) == FALSE)
	{
		// ������ ��ŷ ����� �ִ� ����ڶ�� ����� �˸���. 
		// ������ �����Ų��. 
		GameStatus = GS_INITQUIT;

		// ��ŷ���� ���� ������ ������� �˸���. 
		clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
	}


	// Ÿ�̸Ӱ� ��ġ�� ������ ���� �׿�����.
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


	// ���⸦ �����ϸ� ���⿡ ���� ���� ������ �����ȴ�. 
	SI32 attackrange;
#ifndef _SKILL_
	// 2�� ��� ���ݹ���(��Ÿ�)�� ���� �����ۿ� ������ ���� �ʴ´�.
	if ((m_ParmeterBox.GetItemAttackRange()) && (siCharType != 3))
#endif
#ifdef _SKILL_
	if(m_ParmeterBox.GetItemAttackRange())
#endif
		attackrange   = m_ParmeterBox.GetItemAttackRange();
	// ���Ⱑ ������ ����Ʈ ���� ������ �����Ѵ�. 
	else attackrange   = KI[m_cltCharStatus.GetKind()].GetAttackRange(m_siCurrentAttack);

	SHORT searchrange   = KI[m_cltCharStatus.GetKind()].m_siSearchRange		;//+ m_ParmeterBox.GetItemAttackRange();


	//�ӵ��� �����̸� �ش�.
	//m_siMoveDelay�� ��ȭ �� ������ movespeed�� ����Ѵ�.
	SHORT movespeed     = (SHORT)((float)KI[m_cltCharStatus.GetKind()].m_ssMoveSpeed*m_MoveDelay);//+ siEtcMoveSpeed;


	if(m_ParmeterBox.IP.GetMana() > m_ParmeterBox.GetMaxMana())	m_ParmeterBox.IP.SetMana(m_ParmeterBox.GetMaxMana());
	if(m_ParmeterBox.IP.GetLife() > m_ParmeterBox.GetMaxLife())	m_ParmeterBox.IP.SetLife(m_ParmeterBox.GetMaxLife());
	SetAttackRange(attackrange);
	m_siSearchRange		= searchrange;
	MoveSpeed		= movespeed;

	// �������� ��� ���縦 �Ѹ���.
	if(m_cltCharStatus.GetKind() == KIND_BUTTERFLY)
	{
		// ����ִ� ���ȿ���. 
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

	// ���ż��� ����ϴ� �����̰� ���ż��� ���ߵǾ� �ִ� ���.  
	if((KI[m_cltCharStatus.GetKind()].m_uiAtb2 & ATB2_HIDE) )
	{
		m_uiAtb2|=ATB2_HIDE;
	}
	else
	{
		// ���ȿ���� ���� ���ż��� �����ϸ�, 
		if(bAssistHideSwitch)
		{
			m_uiAtb2 |= ATB2_HIDE;
		}
		else
		{
			// �׿ܿ��� ���� �Ӽ��� ����. 
			if(m_uiAtb2 & ATB2_HIDE)
			{
				m_uiAtb2-=ATB2_HIDE;
			}
		}
	}

	// �������̸� ���� ����ɵ��� �����. 
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
		
		// ���� ���ݷ��� ���ش�.
	    if(m_ParmeterBox.AssistApowerByGEffect)
		{
		    if((GD.Frame-m_ParmeterBox.AssistApowerFrame) > 40)
			{
			   m_ParmeterBox.AssistApowerByGEffect = 0;
			}
		}
	    // ���� ������ ���ش�.
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

		// �̵� ĳ���ʹ� �ٽ� ��Ӱ� ����⵵ �Ѵ�. 
		
		// �����ϴ� �Ӽ��� ĳ���ʹ�,  
		if((IsAtb2(ATB2_HIDE) || AmbushSwitch == TRUE) && MapEditMode == FALSE)
		{
			
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				
				// ���� ���Դ� ��Ӱ� ������ �ʴ´�. 
				if(GD.IsFriendTo(GetPlayerNumber(), i) == TRUE)
				{
					m_scSeeMode[i] = TRUE;
					continue;
				}
				
				// ��ο� �������� ��Ӱ� �����. 
				if(GD.BlackMap[i][m_ssX][m_ssY] >= FOG2_DARK_STEP)
				{
					DiscoverDelay[i]=0;
				}
				
				// �߰ߵǾ� �ִ� �ð��� �ִٸ� �ٿ�������. 
				if(DiscoverDelay[i])
				{
					m_scSeeMode[i]=TRUE;
					DiscoverDelay[i]--;
				}
				
				// ��ȿ�Ⱓ�� ������ �Ⱥ��̰� �����.
				if(DiscoverDelay[i]==0)
					m_scSeeMode[i]=FALSE;
			}
		}
		else
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// ���� ���Դ� ��Ӱ� ������ �ʴ´�. 
				if(GD.IsFriendTo(GetPlayerNumber(), i) == TRUE)
				{
					m_scSeeMode[i] = TRUE;
					continue;
				}
				
				// ��ο� �������� ��Ӱ� �����. 
				if(GD.BlackMap[i][m_ssX][m_ssY] >= FOG2_DARK_STEP)
				{
					m_scSeeMode[i] = FALSE;
				}
				// ���� ������ ���̰� �����.
				else
				{
					m_scSeeMode[i] = TRUE;
				}
			}
		}
		
		

		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// ���̴� �����̰� 
			if(m_scSeeMode[i]==TRUE)
			{
				// ������ ���̸�, 
				if(m_scSeeStep[i]==0)
				{
				}
				else
				{
					m_scSeeStep[i]--;
				}

			}
			// ������� �����̰� 
			else
			{
				// ������ �������, 
				if(m_scSeeStep[i]==(MAX_LIGHT_STEP-1))
				{
				}
				else
				{
					m_scSeeStep[i]++;
				}

			}
		}

		// �÷��̾�� �Ⱥ��̴� ����̸�, 
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
		// ġ��޴� �׸��� �����Ѵ�. 
	    if(HealDrawSwitch==TRUE)
		{
			HealDrawStep++;
			if(HealDrawStep==10)
			{
				HealDrawSwitch=FALSE;
				HealDrawStep=0;
			}
		}

		// (�Ż�) �������ϴ� �׸��� �����Ѵ�. 
	    if( LevelUpDrawSwitch == TRUE )
		{
			LevelUpDrawStep++;
			if( LevelUpDrawStep == 20 )
			{
				LevelUpDrawSwitch	= FALSE;
				LevelUpDrawStep		= 0;
			}
		}

	    // ����ġ ���ѱ� �׸��� �����Ѵ�. 
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

		// ���ڰ� ����. 
		if(m_siHaltDelay)
		{
			m_siHaltDelay -- ;

			//if((GD.Frame / 3 == 0 )
			{
				if(m_siHaltStep < (13 - 1))
				{
					// ȿ���� ����. 
					switch(m_siHaltReason)
					{
					// robypark 2004/6/10 18:33
					// õ������ �ñ⿡ ���� ����, ����� �⺻ ���ݿ� ���� ���� ȿ���� ����
					case HALT_REASON_FOG_OF_ICE:
					// �ñ� ���ݿ� ���� ���� ȿ���� ����
					case HALT_REASON_ICE:
						if(m_siHaltStep == 0)
						{
							// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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

		// �ߵ���ġ. 
		SI32 power = m_cltPoisonEffect.Action();
		if(power)
		{
			// robypark 2004/8/9 13:44
			// �ߵ� ������ ĳ���� ID�� �߰��Ͽ� ���� ���� ����� ����ġ �ֵ��� ����
			if(m_ParmeterBox.IP.GetLife() > 0)	// ��ȿ�� ���¿��� 
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

	// �ǰݽ� ǥ����. 
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


	// ���� ���ִٸ�, �ð��� �ٿ��ش�. 
	if(ShieldDelay)
	{
		ProtectorDrawSwitch=TRUE;
	}

	//��ȣ�� (��) ���� �ð� ī��Ʈ
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

	// �ϴ� ���̴� ���¿��� �Ѵ�.
	// �ڽ��� ��ġ�� �ľ��Ѵ�.
	// ���� �� ���� �ִ��� Ȯ���Ѵ�. 
	BOOL oldshallowwaterswitch = ShallowWaterSwitch;
	ShallowWaterSwitch  = FALSE;
	if(m_scSeeSwitch==TRUE && IsAtb(ATB_FLY)==FALSE)
	{
		// ���� ���� �ְ� �谡 �ƴϸ�, 
		if(Map.GetShallowMap(m_ssX, m_ssY) && IsAtb(ATB_SHIP)==FALSE)
		{
			
			// ���� ���̸� ����Ѵ�. (������ ������ ���¶��. )
			if((GD.Rain.WaterLevel==(MAX_WATER_LEVEL_STEP-1)) )
			{
				// ������ �� �� ���� �����̸� �״´�. 
				if(KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_SWIM) == FALSE)
				{
					m_ParmeterBox.IP.SetLife(0);
					DyingAtb|=DYINGATB_SHIP;
				}
			}
			else
			{
				// ������ ���� ���� �ִ� ���°� �ƴ϶��, 
				if(oldshallowwaterswitch==FALSE)
				{
					// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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

	    // ������� �۵� 
	    // ���׷��̵尡 �Ǿ� �־�� �Ѵ�. 
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

	   // �н��� �ڽ��� ������ �Ǵ� ĳ������ �������� ����ؼ� ���� �Դ´�. 

	   if(m_bGuardianChar == FALSE)
	   {
		   if(HostID.IsEmpty()==FALSE)
		   {
			   //���Ǽ��� ������� ĳ�����ΰ�.
			   if(m_bChangePlayer == FALSE )
			   {
				   // �ڽ��� ü���� ������. 
				   if(m_ParmeterBox.IP.GetLife() > 3)
					   m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife()-3);
				   else
					   m_ParmeterBox.IP.SetLife(m_ParmeterBox.IP.GetLife()-1);
			   }
		   }
	   }
	}

	//��ȣ�� �������� ���� �Ұ��.
	//m_siGuardianID �ڽ��� ������ �ִ� ��ȣ�� �������� ���̵� 
	if(m_siGuardianID != 0)
	{
		//���� �����ϰų� ������ �´´�.
		//���� ��ȣ�� �������� �ߵ�.
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

	//��ȣ�θ� ����� ��ü�Ѵ�.
	if(m_bUseGuardian == true)
	{
		//�ð� ������ �ִ� ��ȣ�ζ��.
		if(m_siGuardianTimeOut == true)
		{
			DWORD difGuardianTime;
			difGuardianTime = ((timeGetTime() - m_dwGuardianTime)/1000);
			if(difGuardianTime > m_siGuardianAliveTime)
			{	
				GuardianBreakless(m_siGuardianKind);
				m_bUseGuardian = false;		//��ȣ�ιߵ��� �Ҹ�Ǿ���.
				
			}
		}
		//�ð� ������ ���� ��ȣ�ζ��.
		else 
		{
			//�ش� ��ȣ���� ����� ����Ͽ���.
			if(m_siGuardianNoTimeOutSkillUse == true)
			{
//				m_bGuardianEquip = false;	//��ȣ���� ������ �ֳ�.
				m_bUseGuardian = false;		//��ȣ�ιߵ��� �Ҹ�Ǿ���.
			}
		}	
	}
	return TRUE;
}


// �ǹ��̳� ĳ������ ��� �ൿ�� �����ϴ� ��ƾ 
// ���ϰ�:  FALSE  .  Ư�� ���� ���� 
//          TRUE   ->  ĳ���͸� ������ 
BOOL _Char::Action()
{
	// ��ȣ��(����) : TRUE�̸� �ٸ� ������ �������� �ʰ� ��ٸ���.
	if(ActionPig())		return TRUE;

	// ġƮŰ�� ����� ĳ���� ���� ǥ�� 
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

	// ����ȿ�� ���� ó��. 
     GeneralEffect();

	// ���õ� ĳ���ʹ� 
	// �޴��� ������Ʈ�Ѵ�. 
	if(m_scSelectedSwitch==TRUE)
	    UpdateMenu(UpdateMenuOrder);

	// ȣ��Ʈ�� �����Ǿ� �ְ�, 
	if(HostID.IsEmpty()==FALSE)
	{
		// �׾��ٸ�, 
		if(IsAlive(HostID.ID)==FALSE)
		{
			m_ParmeterBox.IP.SetLife(0);
		}
	}

	if(MapEditMode==FALSE)
	{
		// ��ȿ�� ���¿��� 
		if(m_ParmeterBox.IP.GetLife() > 0)
		{
		   // ���� �Ӽ��� �ִ� ĳ���͸� ������ �����Ѵ�. 
			if( (GD.Frame + m_NewID.ID) % 10 ==0)
			{
			  SearchForward();
			}
			  
    	   // ������ ���ݹ������� ��ġ 
	       BeAttackedOperation();
    	   // ����� ����� �����Ѵ�. 
    	   GetOrderBuffer();
		}
	    else
		{
		    if(m_scSelectedSwitch==TRUE)
			{
			   SetSelect(FALSE, FALSE);
			}

	        // ü���� �������� �״´�. 
	       if(CurrentOrder!=ORDER_DIE && CurrentOrder!=ORDER_DEAD && CurrentOrder!=ORDER_CARCASS)
		   {
			   SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

			   if(siCharType != 3)
			   {
			   // ���̴� ���¸�, 
				   if(m_scSeeSwitch==TRUE)
				   {
					  CurrentOrder=ORDER_DIE;

					  Step=0;
				   }
				   // ������ �ʴ� ���¶��, 
				   else
				   {
					   Dead();
					   return FALSE;
				   }
			   }
		   }
		}
	}

	// ���� ������ �缳���Ѵ�. 
    ParametaAction();

	// ���ۿ� ������� �ִϰ� ���ӵǴ� ��� 
	// �̷� ��쿡�� �Ǽ��̳� ��ü���� ������� �ʴ´�.
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
	//  �ƹ� �۾��� ���� �ʴ� ��� ���� 
	case ORDER_WAIT:
	    DeleteID();
        OperateWait();

		SetID();
    	break;

	// ���� �ϰ� �ִ� ������ ����
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
			// �ﰢ �״� ���̸� ��ü�� ������ �ʴ´�. 
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
	////////////////////////////////�ؼ�����///////////////////////
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

    // ĳ���͸� �̵� ��Ŵ 
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

		// ���� ���� �ؾ� �� ó���� �Ϸ��ؾ� ���ݿ� ����. 
		if(PreAttack()==TRUE)
		{
			// ������ ����Ǹ�,
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
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_FIRE_ROUND_DANCE:			// ����������(����-�ź���, �̼��� 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_FIRE_ROUND_DANCE);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_NOVA:						// ����(����-������, ���� 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_NOVA);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_FURY_OF_VOLCANO:				// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_FURY_OF_VOLCANO);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_POISONING_HEAVENS:			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_POISONING_HEAVENS);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_BOMB_OF_HEAVEN:				// ��õ��(�߱�-�߼���, �̷� 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_BOMB_OF_HEAVEN);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_SIEGE_MODE:	// �Ҷ�������=>��õ��� ����(�߱�-�Ҷ�������, ���� 2�� ���� ���)
//		DeleteID();
//		SpecialAtb4(SPECIAL_SIEGE_MODE);
//		SetID();
		ChangeModeRoarOfLaughter();
		break;

	// robypark (2004/06/4 14:13)
	case ORDER_SIEGE_MODE_UNDO:	// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, ���� 2�� ���� ���)
//		DeleteID();
//		SpecialAtb4(SPECIAL_SIEGE_MODE);
//		SetID();
		ChangeModeFarangi();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_ONSLAUGHT_FIREBALL:			// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
		DeleteID();
		SpecialAtb4(SPECIAL_ONSLAUGHT_FIREBALL);
		SetID();
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 19:08) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_FIRERAIN_OF_GOD:				// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
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

	// ȭ�鿡 ���� ��ǥ�� ���Ѵ�. 
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
	// ��ȣ��(����)�� ������ ������ 3�ʵ��� �ƹ��� ���۵� ���Ѵ�.
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

	// ��ȣ��(����) ������ ���� ���ʹ� 3�ʵ��� �ƹ��� ���۵� ���Ѵ�.
	if(m_bPigDefence)
	{
		if((timeGetTime() - m_dwPigAttackDelay) < 3000)		return TRUE;

		// ������ ������ ������� �������´�.
		m_bPigDefence = FALSE;
		m_cltCharDrawInfo.SetDrawSwitch(TRUE);
		
		// Ȯ���� ���� ���״� ���͸� ������ ����������.
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

		// �������� �ƴϾ�� �Ѵ�. 
		if(pGame->pOnlineWorld->CheckPKFieldMap() == TRUE)	return FALSE;

		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
			MyNet.PlayersdpId.dpId[CR[RecallID.ID].GetPlayerNumber()], CR[RecallID.ID].m_siOnlineID, 
			MyNet.PlayersdpId.dpId[CR[m_NewID.ID].GetPlayerNumber()], CR[m_NewID.ID].m_siOnlineID);
		
		// ų������ �մ��� ����ġ���� �����Ѵ�. 
		pGame->pBattle->ExpOperate(RecallID.ID, m_NewID.ID);

		// ĳ���� ����
		DeleteChar(m_NewID.ID);
		
		return FALSE;
	}
	return FALSE;
}

#ifdef _DEBUG

// robypark 2004/8/4 18:42
// ġƮŰ ���� �÷��� ���� ����
bool g_bShowGrid		= false;	// �ʿ� GRID �׸���
bool g_bShowID			= false;	// ĳ���� ID �׸���
bool g_bShowMapIdx		= false;	// ���� �� ���� ĳ���Ͱ� ������ ��� ���� �ε��� ��ǥ�� �׷������� �� �������� ����
bool g_bShowArrayIdx	= false;	// ĳ���Ͱ� ��ġ�ϴ� ���� ���� �迭 �ε����� ���̵��� �������ش�.

// robypark 2004/8/4 18:42
// ���� �� ġƮŰ ���� ó��
void ProcessCheatMessage(char * szCheatMessage)
{
	CHAR			cSep[]	= " ,";
	CHAR			*pToken = NULL;
	SI32			siParameter1 = 0;
	SI32			siParameter2 = 0;
	SI32			siParameter3 = 0;

	if (szCheatMessage[0] != '/')
		return;

	// ��ɾ ���´�.
	if ((pToken = strtok(szCheatMessage, cSep)) == NULL)
		return;		

	// ���� ���� GRID�� ���̵��� �� �������� ���� ���ó��
	if (stricmp(pToken, "/SHOWGRID") == 0)
	{
		g_bShowGrid = !g_bShowGrid;
	}
	// ���� �� ĳ������ ID�� �׷������� �� �������� ���� ���ó��
	else if (stricmp(pToken, "/SHOWID") == 0)
	{
		g_bShowID = !g_bShowID;
	}
	// ���� �� ���� ĳ���Ͱ� ������ ��� ���� �ε��� ��ǥ�� �׷������� �� �������� ���� ���ó��
	else if (stricmp(pToken, "/SHOWMAPIDX") == 0)
	{
		g_bShowMapIdx = !g_bShowMapIdx;
	}
	// ĳ���Ͱ� ��ġ�ϴ� ���� ���� �迭 �ε����� ���̵��� �������ش�.
	else if (stricmp(pToken, "/SHOWARRAYIDX") == 0)
	{
		g_bShowArrayIdx = !g_bShowArrayIdx;
	}
	// ���� ���� ĳ���͸� ����� ������ ó���ϵ��� �Ѵ�. ���� ���� ó������ ���ܵ� �� ������ ���� ���� �ƴϴ�.
	// 2�� ������Դ� ������ �ʴ´�. �ٸ� �׷����⸸ �ϸ� ������ ���� �ʴ´�.
	else if (stricmp(pToken, "/KILL") == 0)
	{
		// ������ ĳ������ ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// ü�� ���� 0���� �����Ͽ� ���� ������ ���δ�.
		CR[siParameter1].m_ParmeterBox.IP.SetLife(0);
	}
	// ĳ������ ü�°��� �������ش�.
	// ���� �������� �Ͻ������� ����ȴ�.
	else if (stricmp(pToken, "/SETLIFE") == 0)
	{
		// ������ ĳ������ ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// ���� ������ ü�� ��ġ
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter2 = atoi(pToken);

		// ���� ������ ü�� ��ġ
		// �ִ밪���� ū ������ ������ ��� �ִ� ü�°��� �����ȴ�.
		CR[siParameter1].m_ParmeterBox.IP.SetLife(siParameter2);
	}
	// ĳ������ �������� �������ش�.
	// ���� �������� �Ͻ������� ����ȴ�.
	else if (stricmp(pToken, "/SETMANA") == 0)
	{
		// ������ ĳ������ ID
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// ���� ������ ���� ��ġ
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter2 = atoi(pToken);

		// �ִ밪���� ū ������ ������ ��� �ִ� �������� �����ȴ�.
		CR[siParameter1].m_ParmeterBox.IP.SetMana(siParameter2);
	}
	// ���� Ư�� ��ġ�� ���� ����.
	else if (stricmp(pToken, "/MAKEFIRE") == 0)
	{
		// ���� X��ǥ
		if ((pToken = strtok(NULL, cSep)) == NULL)
			return;

		siParameter1 = atoi(pToken);

		// ���� Y��ǥ
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