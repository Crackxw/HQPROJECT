//--------------------------------------------------------------------
//  ��� : ���°�
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


// ����� ���� �����ϱ� ���� �ؾ��� �ʱ�ȭ 
void _Char::InitPreGetOrder()
{

	// ���� �����ϴ� ĳ���͸� �ʱ�ȭ�Ѵ�.
	AttackID.Init(); 
	// ���������ϴ� ĳ���͸� �ʱ�ȭ�Ѵ�.
	AttackMeID.Init();
	BeAttackedSwitch = FALSE;

	AmbushSwitch = FALSE;

	AttackGiveUpDelayor=0;
}


// ����� ����� ������ �����Ѵ�. 
BOOL _Char::GetOrderBuffer()
{
	int id=0;
	DWORD flag = 0;
	SI32 returnval = 0;

	// ����� ����� ������ ����, 
	if(BufferOrder.Order==ORDER_WAIT)return FALSE;
	if(m_scBusySwitch==TRUE)return FALSE;

	// ���ڼ��� ������϶����� ���� ��� �̿ܿ��� ���� �ʴ´�. 
	if(m_siHaltDelay)
	{
		if(BufferOrder.Order != ORDER_STOP)
		{
			return FALSE;
		}
	}

	// ��ȣ��[����] ĳ������ ��� ������� �̿ܿ��� ���� �ʴ´�.
	if(m_bPigAttack)
	{
		if(BufferOrder.Order != ORDER_STOP)
		{
			return FALSE;
		}
	}

	// ���ο� ����� �켱������ �� ���ϰ�
	if(Order[BufferOrder.Order].Priority<Order[CurrentOrder].Priority)
	{
	}
	// ���ο� ��ɰ� ���� ����� �켱 ������ ����, 
	//  ���� ������ ����� �����ϴ� �ɼ��̸�, 
	else if(Order[BufferOrder.Order].Priority==Order[CurrentOrder].Priority 
	&& Order[BufferOrder.Order].Info&ORDERINFO_SAMEPRIORITY)
	{
	}
	// ���ο� ��ɰ� ���� ����� �켱 ������ ����, 
	//  ���� ������ ����� �������� �ʴ� �ɼ��̸�, 
	else if(Order[BufferOrder.Order].Priority==Order[CurrentOrder].Priority 
	&& Order[BufferOrder.Order].Info&ORDERINFO_LOWPRIORITY)
	{
		BufferOrder.Order=ORDER_WAIT;
		return FALSE;
	}

	// �����ü�� ��ǻ�Ͱ� �ƴϸ� 
	if(BufferOrder.HostMode!=BY_COMPUTER)
	{
	   //oldorder�� �ʱ�ȭ�ؾ� �Ѵ�. 
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
		//////////////////////////////////////////////////////////////////�׽�Ʈ����
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
		// ���� ������ ������ ĳ���͸� ������ �� �ִ�. 
		// ���� ���� 
		if( IsAtb(ATB_ATTACKGROUND) )
		{
			// �ٷ� ������ �� �ִٸ�, 
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
		
		// ������ ������ ĳ���͸� ������ �� �ִ�. 
		// ���� ������ �� �ִ�. 
		if(IsAtb(ATB_ATTACK|ATB_ATTACKAIR))
		{
			// ���� �ٷ� ������ �� �ִٸ�, 
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
			// ���� ����� ��ȿ�ϸ� 
			else if(IsSee(BufferOrder.Para2.ID)==FALSE)
			{
				SHORT length=0;
				
				// ����ڰ� ���� �Է��� ����̰ų� ��� ��尡 �ƴϾ�� ���ݰ��� �۾��� ������ �� �ִ�.
				if(BufferOrder.HostMode == BY_PLAYER || HoldSwitch == FALSE)
				{
					// ���� ���� �����ϴ� �۾��߿� �ִٸ�, 
					if(CurrentOrder==ORDER_ATTACK && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
					{
						// ���� �۾��� �����ϱ� ���� �� ����� �����Ѵ�. 
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
		
		// ���Ӱ� ���õ� ��ó�� �����. 
    		  // ��ȭ�� �����ִ� ���̴�. 
		SetNewSelectedSwitch(TRUE);
		
		
		// �δ뿡�� �����Ѵ�. 
		GD.Player[m_cltCharStatus.GetPlayerNumber()].Team.DeleteTeam(m_NewID);
		//GD.TeamDeleteTeam(NewID,m_cltCharStatus.GetPlayerNumber());
		break;
		
	case ORDER_REPAIR:
		// ���� �����ϰ� ü���� ������ ���� ������ 
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
			// ���� ������ �˷��ش�. 
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
// robypark (2004/05/17 15:31) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	case ORDER_FIRE_ROUND_DANCE:			// ����������(����-�ź���, �̼��� 2�� ���� ���)
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

	case ORDER_NOVA:						// ����(����-������, ���� 2�� ���� ���)
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

	case ORDER_FURY_OF_VOLCANO:				// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
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

	case ORDER_POISONING_HEAVENS:			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
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

	case ORDER_BOMB_OF_HEAVEN:				// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
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

	case ORDER_SIEGE_MODE:	// �Ҷ�������=>��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
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
	case ORDER_SIEGE_MODE_UNDO:	// ��õ���=>�Ҷ�������(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
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

	case ORDER_ONSLAUGHT_FIREBALL:			// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
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

	case ORDER_FIRERAIN_OF_GOD:				// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
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
