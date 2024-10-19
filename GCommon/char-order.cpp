//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include <team.h>
#include "..\Gersang\message.h"
#include "..\Gersang\rain.h"
#include "..\Gersang\earthquake.h"
#include "..\Gersang\mytime.h"
#include "..\Gersang\maindata.h"
#include <etc.h>
#include "..\Gersang\music.h"
#include <CharOrg.h>
#include "..\Gersang\text.h"

#include <magic.h>


extern	_Order				Order[];
extern	_Char				CR[];
extern	_MainData			GD;
extern	SI32				UserPlayerNumber;
extern	_KindInfo			KI[];
extern	_Text				Text;
extern	_NewID 				ZeroID;


// 명령을 새로 수여하기 전에 해야할 초기화 
void _Char::InitPreGetOrder()
{

	// 내가 공격하는 캐릭터를 초기화한다.
	AttackID.Init(); 
	// 나를공격하는 캐릭터를 초기화한다.
	AttackMeID.Init();
	BeAttackedSwitch = FALSE;

	AmbushSwitch = FALSE;

	AttackGiveUpDelayor=0;
}


// 비축된 명령을 꺼내서 수행한다. 
BOOL _Char::GetOrderBuffer()
{
	int id=0;
	DWORD flag = 0;
	SI32 returnval = 0;

	// 비축된 명령이 없으면 리턴, 
	if(BufferOrder.Order==ORDER_WAIT)return FALSE;
	if(m_scBusySwitch==TRUE)return FALSE;

	// 포박술을 사용중일때에는 정지 명령 이외에는 받지 않는다. 
	if(m_siHaltDelay)
	{
		if(BufferOrder.Order != ORDER_STOP)
		{
			return FALSE;
		}
	}

	// 수호부[돼지] 캐릭터일 경우 정지명령 이외에는 받지 않는다.
	if(m_bPigAttack)
	{
		if(BufferOrder.Order != ORDER_STOP)
		{
			return FALSE;
		}
	}

	// 새로운 명령의 우선순위가 더 급하고
	if(Order[BufferOrder.Order].Priority<Order[CurrentOrder].Priority)
	{
	}
	// 새로운 명령과 기존 명령의 우선 순위가 같고, 
	//  같은 순위의 명령은 수행하는 옵션이면, 
	else if(Order[BufferOrder.Order].Priority==Order[CurrentOrder].Priority 
	&& Order[BufferOrder.Order].Info&ORDERINFO_SAMEPRIORITY)
	{
	}
	// 새로운 명령과 기존 명령의 우선 순위가 같고, 
	//  같은 순위의 명령은 수행하지 않는 옵션이면, 
	else if(Order[BufferOrder.Order].Priority==Order[CurrentOrder].Priority 
	&& Order[BufferOrder.Order].Info&ORDERINFO_LOWPRIORITY)
	{
		BufferOrder.Order=ORDER_WAIT;
		return FALSE;
	}

	// 명령주체가 컴퓨터가 아니면 
	if(BufferOrder.HostMode!=BY_COMPUTER)
	{
	   //oldorder를 초기화해야 한다. 
	   OldOrder.Order=ORDER_WAIT;
	}

	switch(BufferOrder.Order)
	{
	case ORDER_STOP:
		Step=0;
		InitPreGetOrder();
		RecoverPrevOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_PATROL:
		if(SetPatrol()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
	           CurrentOrder= BufferOrder.Order;
			   Para1=        BufferOrder.Para1;
			   Para2.ID=m_ssX;
			   Para2.Unique=m_ssY;
			   HostMode=     BufferOrder.HostMode;
			   SetMove(LOWORD(Para1), HIWORD(Para1));
		}
		break;
		
	case ORDER_MOVE:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		SetMove(LOWORD(Para1), HIWORD(Para1));
		break;
		
	case ORDER_GOINTOHQ:
		if(SetGoIntoHQ(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_HEAL:
		if(SetHeal(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_MANA:
		if(SetMana(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_EARTHQUAKE:
		if(SetEarthQuake(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
		
	case ORDER_LIGHTNING:
		if(SetLightning(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_TORNADO:
		if(SetSpecial(SPECIAL_TORNADO, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_YELLOWSAND:
		if(SetSpecial(SPECIAL_YELLOWSAND, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_SOUL:
		if(SetSpecial(SPECIAL_SOUL ,BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_GHOST:
		if(SetSpecial(SPECIAL_GHOST ,BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_DARKFIRE:
		if(SetSpecial(SPECIAL_DARKFIRE ,BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_MAGIC2T:
		if(SetSpecial(SPECIAL_MAGIC2T, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		//////////////////////////////////////////////////////////////////테스트마법
	case ORDER_MAGICTEST:
		if(SetSpecial(SPECIAL_MAGICTEST, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		//////////////////////////////////////////////////////////////////
	case ORDER_MAGIC4T:
		if(SetSpecial(SPECIAL_MAGIC4T, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_MAGIC4C:
		if(SetSpecial(SPECIAL_MAGIC4C, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_IVORY:
		if(SetSpecialAtb4(SPECIAL_IVORY, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_BLUEDRAGON:
		if(SetSpecialAtb4(SPECIAL_BLUEDRAGON, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_RUNNINGFIRE:
		if(SetSpecialAtb4(SPECIAL_RUNNINGFIRE, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_ENERGYPA:
		if(SetSpecialAtb4(SPECIAL_ENERGYPA, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_SNOWATTACK:
		if(SetSpecialAtb4(SPECIAL_SNOWATTACK, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_FIREATTACK:
		if(SetSpecialAtb4(SPECIAL_FIREATTACK,BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_FIREENERGYPA:
		if(SetSpecialAtb4(SPECIAL_FIREENERGYPA,BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_GROUNDEXP:
		if(SetSpecial(SPECIAL_GROUNDEXP, ZeroID)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_MAGIC6C:
		if(SetSpecial(SPECIAL_MAGIC6C, ZeroID)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_ICE:
		if(SetSpecial(SPECIAL_ICE, ZeroID)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
		break;
		
	case ORDER_MASSHEAL:
		if(CanHeal()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
		
	case ORDER_USEBUDA:
		if(CanUseBuda()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_STEALEXP:
		if(CanStealExp()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_AIRMAGIC:
		if(CanAirMagic()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_KIMBOMB:
		if(SetSpecial(SPECIAL_KIMBOMB, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_MAGIC5J:
		if(SetSpecial(SPECIAL_MAGIC5J, BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
		
	case ORDER_RAINMAKE:
		if(CanRainMake()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_CLON:
	case ORDER_ENEMYCLON:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		
		break;
	case ORDER_RECALL_PANDA:
		Step=0;
		WorkStep=WS_DECIDE_LENGTH_TO_DEST;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_RECALL_DAMDUK:
	case ORDER_RECALL_MONKEY:
	case ORDER_RECALL_AMAKUSA:
	case ORDER_RECALL_TIGER:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_GUARDIAN_RABBIT:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_RECALL_GUARDIAN_MOUSE:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_RECALL_GUARDIAN_CAW:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_GUARDIAN_DRAGON:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_GUARDIAN_TIGER:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_GUARDIAN_HORSE:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_GUARDIAN_SHEEP:
		Step=0;
		InitPreGetOrder();
		CurrentOrder= BufferOrder.Order;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
	case ORDER_HOLD:
		HoldSwitch=TRUE;
		Step=0;
		InitPreGetOrder();
		CurrentOrder= ORDER_STOP;
		Para1=        BufferOrder.Para1;
		Para2=        BufferOrder.Para2;
		HostMode=     BufferOrder.HostMode;
		break;
		
	case ORDER_AMBUSH:
		if(KI[m_cltCharStatus.GetKind()].m_siKindAtb & KINDATB_AMBUSH)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_DETECTMINE:
		if(CanSpecial(SPECIAL_DETECTMINE, ZeroID) == TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
		
	case ORDER_ATTACKGROUND:
		// 지상 공격이 가능한 캐릭터만 공격할 수 있다. 
		// 지상 공격 
		if( IsAtb(ATB_ATTACKGROUND) )
		{
			// 바로 공격할 수 있다면, 
			if( CanAttackGround(LOWORD(BufferOrder.Para1), HIWORD(BufferOrder.Para1)) == TRUE )
			{  
				Step=0;
				InitPreGetOrder();
				
				CurrentOrder= ORDER_ATTACK;
				
				Para1		= BufferOrder.Para1;
				Para2		= BufferOrder.Para2;
				HostMode	= BufferOrder.HostMode;
				WorkStep	= WS_DECIDE_LENGTH_TO_DEST;
				
				SetAttackGround(LOWORD(Para1), HIWORD(Para1));
			}
		}
		break;
		
	case ORDER_ATTACK:
		
		// 공격이 가능한 캐릭터만 공격할 수 있다. 
		// 적을 공격할 수 있다. 
		if(IsAtb(ATB_ATTACK|ATB_ATTACKAIR))
		{
			// 적을 바로 공격할 수 있다면, 
			if(CanAttack(BufferOrder.Para2)==TRUE)
			{  
				
				Step=0;
				InitPreGetOrder();
				
				CurrentOrder= BufferOrder.Order;
				Para1=        BufferOrder.Para1;
				Para2=        BufferOrder.Para2;
				HostMode=     BufferOrder.HostMode;
				WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				
				SetAttack(Para2);
			}
			// 공격 대상이 무효하면 
			else if(IsSee(BufferOrder.Para2.ID)==FALSE)
			{
				SHORT length=0;
				
				// 사용자가 직접 입력한 명령이거나 사수 모드가 아니어야 공격관련 작업을 진행할 수 있다.
				if(BufferOrder.HostMode == BY_PLAYER || HoldSwitch == FALSE)
				{
					// 현재 적을 공격하는 작업중에 있다면, 
					if(CurrentOrder==ORDER_ATTACK && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
					{
						// 현재 작업에 몰두하기 위해 준 명령을 예약한다. 
						PushReserveOrder(BufferOrder);
					}
					else
					{
						Step=0;
						InitPreGetOrder();
						CurrentOrder=ORDER_MOVE;
						HostMode=BY_COMPUTER;
						Para1=        BufferOrder.Para1;
						Para2=        BufferOrder.Para2;
						
						SetMove(LOWORD(Para1), HIWORD(Para1));
					}
				}
				
			}
		}
		
		break;
		
	case ORDER_EXTINGUISH:
		if(SetSpecial(SPECIAL_EXTINGUISH, BufferOrder.Para2, &returnval)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_SELL:
		Step=0;
		InitPreGetOrder();
		//RecoverPrevOrder();
		CurrentOrder=BufferOrder.Order;
		Para1=BufferOrder.Para1; 
		Para2=BufferOrder.Para2; 
		HostMode=BufferOrder.HostMode;
		
		// 새롭게 선택된 것처럼 만든다. 
    		  // 변화를 보여주는 것이다. 
		SetNewSelectedSwitch(TRUE);
		
		
		// 부대에서 제거한다. 
		GD.Player[m_cltCharStatus.GetPlayerNumber()].Team.DeleteTeam(m_NewID);
		//GD.TeamDeleteTeam(NewID,m_cltCharStatus.GetPlayerNumber());
		break;
		
	case ORDER_REPAIR:
		// 수리 가능하고 체력이 떨어져 있을 때에만 
		if(IsAtb(ATB_REPAIR)==TRUE && m_ParmeterBox.IP.GetLife() < m_ParmeterBox.GetMaxLife())
		{
			Step=0;
			InitPreGetOrder();
			RecoverPrevOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1; 
			Para2=BufferOrder.Para2; 
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_MINE:
		if(SetMine(LOWORD(BufferOrder.Para1), HIWORD(BufferOrder.Para1))==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_BETRAY:
		if(SetBetray(BufferOrder.Para2, &returnval)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		else
		{
			if(GetPlayerNumber() == UserPlayerNumber && BufferOrder.HostMode == BY_PLAYER)
			{
				switch(returnval)
				{
				case ERROR_BETRAY_HEALTH:
					SetMessage2(Text.ErrorBetrayHealth.Get(), 0);
					break;
				}
			}
			
		}
		break;
		
	case ORDER_SHIELD:
		if(SetShield(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_WARP:
		if(SetWarp()==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
		
	case ORDER_ABSORB:
		if(SetAbsorb(BufferOrder.Para2)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		break;
	case ORDER_MAGIC8J:
		if(SetSpecial(SPECIAL_MAGIC8J, BufferOrder.Para2, &returnval)==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder=BufferOrder.Order;
			Para1=BufferOrder.Para1;   
			Para2=BufferOrder.Para2;  
			HostMode=BufferOrder.HostMode;
		}
		else
		{
			// 실패 원인을 알려준다. 
			if(GetPlayerNumber() == UserPlayerNumber && BufferOrder.HostMode == BY_PLAYER)
			{
				switch(returnval)
				{
				case ERROR_MAGIC8J_NOAPOWER:
					SetMessage2(Text.ErrorMagic8jNoApower.Get(), 0);
					break;
				}
			}
		}
		break;
		
	case ORDER_SELFEXP:
		if(SetSelfExp( LOWORD(BufferOrder.Para1), HIWORD(BufferOrder.Para1))==TRUE)
		{
			Step=0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_MANABURN:
		if(SetSpecialAtb4(SPECIAL_MANABURN, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_DOUBLE_SWORD:
		if(SetSpecialAtb4(SPECIAL_DOUBLE_SWORD, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_LONG_SPEAR:
		//if(SetSpecialAtb4(SPECIAL_LONG_SPEAR, BufferOrder.Para2)==TRUE)
		if(SetLongSpear(BufferOrder.Para2) == TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_ICE_BARRIER:
		if(SetSpecialAtb4(SPECIAL_ICE_BARRIER, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_FLOWER_NEEDLE_RAIN:
		if(SetSpecialAtb4(SPECIAL_FLOWER_NEEDLE_RAIN, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_SOUL_CHANGE:
		if(SetSpecialAtb4(SPECIAL_SOUL_CHANGE, BufferOrder.Para2)==TRUE)
		//if(SetBetray(BufferOrder.Para2, &returnval)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_STONE_MAGIC:
		if(SetSpecialAtb4(SPECIAL_STONE_MAGIC, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_GOD_SPEAR:
		if(SetSpecialAtb4(SPECIAL_GOD_SPEAR, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;
	case ORDER_CLOUD_WIND:
		if(SetSpecialAtb4(SPECIAL_CLOUD_WIND, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:31) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	case ORDER_FIRE_ROUND_DANCE:			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_FIRE_ROUND_DANCE, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_NOVA:						// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_NOVA, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_FURY_OF_VOLCANO:				// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_FURY_OF_VOLCANO, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_POISONING_HEAVENS:			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_POISONING_HEAVENS, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_BOMB_OF_HEAVEN:				// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_BOMB_OF_HEAVEN, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_SIEGE_MODE:	// 불랑기포차=>앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
//		if(SetSpecialAtb4(SPECIAL_SIEGE_MODE, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	// robypark 2004/6/4 14:21
	case ORDER_SIEGE_MODE_UNDO:	// 앙천대소=>불랑기포차(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
//		if(SetSpecialAtb4(SPECIAL_SIEGE_MODE_UNDO, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_ONSLAUGHT_FIREBALL:			// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
		if(SetSpecialAtb4(SPECIAL_ONSLAUGHT_FIREBALL, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	case ORDER_FIRERAIN_OF_GOD:				// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
		if(SetSpecialAtb4(SPECIAL_FIRERAIN_OF_GOD, BufferOrder.Para2)==TRUE)
		{
			Step = 0;
			InitPreGetOrder();
			CurrentOrder= BufferOrder.Order;
			Para1=        BufferOrder.Para1;
			Para2=        BufferOrder.Para2;
			HostMode=     BufferOrder.HostMode;
		}
		break;

	default:
		clGrp.Error("Fsdafe2r", "%d", BufferOrder.Order);
		break;
	}
		
	BufferOrder.Order=ORDER_WAIT;
		
	return FALSE;
}
