//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include <CharOrg.h>

#include <charinterface.h>

extern _MainData GD;
extern _Char CR[];
extern SHORT GameRealX, GameRealY;

cltCharInterface::cltCharInterface()
{
	
}


void cltCharInterface::SaveChar(FILE* fp)
{
	int i;

	for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		if(GD.CharInfo[i])
		   fwrite(&CR[i],      sizeof(_Char),   1, fp);
    }

}

void cltCharInterface::LoadChar(FILE* fp)
{
	int i;

    for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		if(GD.CharInfo[i])
			fread(&CR[i],      sizeof(_Char),   1, fp);
		else
			ZeroMemory(&CR[i], sizeof(_Char));
    }
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���̵� ���Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
_NewID cltCharInterface::GetID(SI16 siID)
{
	return CR[siID].m_NewID;
}

void cltCharInterface::SetID(SI16 siID)
{
	CR[siID].SetID();
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ �÷��̾� ��ȣ��  ���Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetPlayerNumber(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetPlayerNumber();
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ �÷��̾� ��ȣ��  ���Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetOriginalPlayerNumber(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetOriginalPlayerNumber();
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ Ÿ����   ���Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetKind(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetKind();
}


//-----------------------------------------------------------------------------
// ���� : Ư�� ������ X Ÿ�� ��ǥ���� ���´�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetX(SI16 siID)
{
	return CR[siID].m_ssX;
}

void cltCharInterface::SetX(SI16 siID, SI16 siX)
{
	CR[siID].m_ssX = siX;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ Y Ÿ�� ��ǥ���� ���´�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetY(SI16 siID)
{
	return CR[siID].m_ssY;
}

void cltCharInterface::SetY(SI16 siID, SI16 siY)
{
	CR[siID].m_ssY = siY;
}

SI16 cltCharInterface::GetDX(SI16 siID)
{
	return CR[siID].DX;
}

SI16 cltCharInterface::GetDY(SI16 siID)
{
	return CR[siID].DY;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ Ÿ�� ������ ���´�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetHitPoint(SI16 siID, SI16* siX, SI16* siY)
{
	CR[siID].GetHitPoint(*siX, *siY);

	return 0;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ �Ӽ��� �˾Ƴ���. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsAtb(SI16 siID, SI32 siAtb)
{
	return CR[siID].IsAtb(siAtb);
}


//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���ݷ��� ���´�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetDpower(SI16 siID)
{
	return CR[siID].GetDpower();
}


//-----------------------------------------------------------------------------
// ���� : Ư�� ������ Ư�� ��ġ�� �� �� �ִ��� �˷��ش�.
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsColl(SI16 siID, SI16 siX, SI16 siY)
{
	return CR[siID].IsColl(siX, siY);
}


//-----------------------------------------------------------------------------
// ���� : ���� ������ ���θ� �˷��ش�. (õ���� ��ȭ�� ��쿡�� �ǹ̰� �ִ�. )
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsCover(SI16 siID)
{
	return  FALSE;
   
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ĳ������ ��ǥ����  ���Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
UI08 cltCharInterface::GetTeamColor(SI16 siID)
{
   return CR[siID].GetTeamColor();
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���� �׷��� �˷��ش�. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetGroup(SI16 siID)
{
	return CR[siID].Group;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���� �׷��� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGroup(SI16 siID, SI16 siGroup)
{
	CR[siID].Group = siGroup;
}



//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���̴� �������� �˷��ش�. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetSeeSwitch(SI16 siID)
{
    return CR[siID].m_scSeeSwitch;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���̴� ���¿��θ� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetSeeSwitch(SI16 siID, BOOL bMode)
{
    CR[siID].m_scSeeSwitch = bMode;
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���� ���¸� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::SetSelect(SI16 siID, BOOL bMode, BOOL bEffectSwitch)
{
    return CR[siID].SetSelect(bMode, bEffectSwitch);
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ DrawFrame�� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetDrawFrame(SI16 siID, SI32 siDrawFrame)
{
	CR[siID].m_cltCharDrawInfo.SetDrawFrame(siDrawFrame);
}


//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���̵� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::DeleteID(SI16 siID)
{
	return CR[siID].DeleteID();
}

//-----------------------------------------------------------------------------
// ���� : ��� ��带 ���´�. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetGuardSwitch(SI16 siID)
{
	return CR[siID].GuardSwitch;
}

//-----------------------------------------------------------------------------
// ���� : ��� ��带 �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGuardSwitch(SI16 siID, BOOL bGuardMode)
{
	CR[siID].GuardSwitch = bGuardMode;
}

//-----------------------------------------------------------------------------
// ���� : ��� ������ x, y��ǥ�� �����Ѵ�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGuardXY(SI16 siID, SI16 siX, SI16 siY)
{
	CR[siID].GuardX = siX;
	CR[siID].GuardY = siY;
}


//-----------------------------------------------------------------------------
// ���� : Ư�� ������ �÷��̾� ��ȣ�� �����Ѵ�.  
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::ChangePlayer(SI16 siID, SI16 siPlayerNumber)
{
	return CR[siID].ChangePlayer(siPlayerNumber);
}

//-----------------------------------------------------------------------------
// ���� : Ư�� ������ ���� ��� ���¸� ���´�.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetCurrentOrder(SI16 siID)
{
	return CR[siID].CurrentOrder;
}



//-----------------------------------------------------------------------------
// ���� : �ڵ����� ��带 �����Ѵ�.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetAutoSwitch(SI16 siID, BOOL bMode)
{
	CR[siID].AutoSwitch = bMode;	
}

//-----------------------------------------------------------------------------
// ���� : �ڵ����� ��带 ���´�.
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetAutoSwitch(SI16 siID)
{
	return CR[siID].AutoSwitch;
}

//-----------------------------------------------------------------------------
// ���� : Host Mode�� �����Ѵ�.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetHostMode(SI16 siID, CHAR HostMode)
{
	CR[siID].HostMode	=	HostMode;
}

//-----------------------------------------------------------------------------
// ���� : Host Mode�� ���´�.
// Desc :
//-----------------------------------------------------------------------------
CHAR cltCharInterface::GetHostMode(SI16 siID)
{
	return CR[siID].HostMode;	
}


//-----------------------------------------------------------------------------
// ���� : �̺�Ʈ�� �����Ѵ�.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetEventSwitch(SI16 siID, SI16 siEventSwitch)
{
	CR[siID].EventSwitch	=	siEventSwitch;
}

//-----------------------------------------------------------------------------
// ���� : �̺�Ʈ�� ���� ���θ� ���´�.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetEventSwitch(SI16 siID)
{
	return CR[siID].EventSwitch;
}


//-----------------------------------------------------------------------------
// ���� : ������ �޾Ҵ����� ���θ� ���´�.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetBeAttackedSwitch(SI16 siID)
{
	return CR[siID].BeAttackedSwitch;
}
	
//-----------------------------------------------------------------------------
// ���� : �����ϴ� ĳ������ ID�� ���´�.
// Desc :
//-----------------------------------------------------------------------------
SHORT cltCharInterface::GetAttackID(SI16 siID)
{
	return CR[siID].AttackID.ID;
}

BOOL cltCharInterface::IsSameAttackID(SI16 siID, _NewID clID)
{
	return CR[siID].AttackID.IsSame(clID);
}

//-----------------------------------------------------------------------------
// ���� : ���� ���θ� �˷��ش�.
// Desc :
//-----------------------------------------------------------------------------
SI08 cltCharInterface::GetLowMoveSwitch(SI16 siID)
{
	return CR[siID].LowMoveSwitch;
}


//-----------------------------------------------------------------------------
// ���� : ������ ��� �Ӽ��� �˾ƿ´�. 
// Desc :
//-----------------------------------------------------------------------------
SI32 cltCharInterface::GetDefenceAtb(SI16 siID)
{
	return CR[siID].m_uiDefenceAtb;
}

void cltCharInterface::SetLife(SI16 siID, SI16 siHealth)
{
	CR[siID].m_ParmeterBox.IP.SetLife(siHealth);
}

BOOL cltCharInterface::IncreaseLife(SI16 siID, SHORT amount)
{
	return CR[siID].IncreaseLife(amount);
}

BOOL cltCharInterface::ReduceLife(SI16 siID, SHORT amount, SHORT unique)
{
	return CR[siID].ReduceLife(amount, unique);
}

SI16 cltCharInterface::GetCenterX(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetCenterX();
}

SI16 cltCharInterface::GetCenterY(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetCenterY();
}

// ������ ���ݹް� ������ ȣ���ϴ� �Լ� 
void cltCharInterface::SetBeAttackedSwitch(SI16 siID, SHORT enemyid, SHORT attackmeplayer)
{
	CR[siID].SetBeAttackedSwitch(enemyid, attackmeplayer);
}


BOOL cltCharInterface::UpdateMenu(SI16 siID, SHORT order)
{
	return CR[siID].UpdateMenu(order);
}

// idĳ���͸� ������ �� �ִ��� ���θ� �˷��ش�. 
BOOL cltCharInterface::CanAttack(SI16 siID, _NewID id)
{
	return CR[siID].CanAttack( id );
}

void cltCharInterface::SetNewSelectedSwitch(SI16 siID, SHORT mode)
{
	CR[siID].SetNewSelectedSwitch(mode);
}

UI32 cltCharInterface::GetWorkStep(SI16 siID)
{
	return CR[siID].WorkStep;
}

void cltCharInterface::SetWorkStep(SI16 siID, UI32 uiWorkStep)
{
	CR[siID].WorkStep = uiWorkStep;
}

UI16 cltCharInterface::GetPara1(SI16 siID)
{
	return CR[siID].Para1;
}

SI16 cltCharInterface::GetLife(SI16 siID)
{
	return CR[siID].m_ParmeterBox.IP.GetLife();
}

SI16 cltCharInterface::GetMana(SI16 siID)
{
	return CR[siID].m_ParmeterBox.IP.GetMana();
}

BOOL cltCharInterface::Action(SI16 siID)
{
	return CR[siID].Action();
}


// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� (���� ��ǥ)
// length: �߽ɰ��� �Ÿ� 
BOOL cltCharInterface::DrawSwitch(SI16 siID, SHORT x, SHORT y, SHORT& length)
{
	return CR[siID].IsAbsPtInObj(GameRealX, GameRealY, length);
}

// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� (���� ��ǥ)
// length: �߽ɰ��� �Ÿ� 
BOOL cltCharInterface::IsAbsPtInObj(SI16 siID, SHORT x, SHORT y, SHORT& length)
{
	return CR[siID].IsAbsPtInObj(x, y, length);
}

// � rect���� ������ �����ϴ°�?
BOOL cltCharInterface::IsAbsPtInObj(SI16 siID, SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	return CR[siID].IsAbsPtInObj(x1, y1, x2, y2);
}

clock_t cltCharInterface::GetSelectedClock(SI16 siID)
{
	return CR[siID].SelectedClock;
}

void cltCharInterface::SelectOperation(SI16 siID)
{
	CR[siID].SelectOperation();
}

BOOL cltCharInterface::CanWork(SI16 siID)
{
	return CR[siID].CanWork();
}

// ĳ���͸� �̵��ϰ� �����. 
BOOL cltCharInterface::SetMove(SI16 siID, SHORT dx, SHORT dy)
{
	return CR[siID].SetMove(dx, dy);
}

void cltCharInterface::SetAttackGiveUpDelayor(SI16 siID, SI16 AttackGiveUpDelayor)
{
	CR[siID].AttackGiveUpDelayor=0;
}

int	cltCharInterface::Move(SI16 siID, SHORT moveblockcount, SHORT range)
{
	if(CR[siID].IsAtb(ATB_FLY)==TRUE)
	{
		return CR[siID].MoveFly(moveblockcount, range);
	}
	else
	{
		return CR[siID].MoveNormal(moveblockcount, range);
	}
}

int	cltCharInterface::GetMovePercent(SI16 siID)
{
	return CR[siID].MovePercent;
}

void cltCharInterface::SetBusySwitch(SI16 siID, BOOL bBusySwitch)
{
	CR[siID].m_scBusySwitch = bBusySwitch;
}

// �Ӽ��� ���Ѵ�. 
BOOL cltCharInterface::IsAtb2(SI16 siID, DWORD atb)
{
	return CR[siID].IsAtb2(atb);
}

void cltCharInterface::SetDirection(SI16 siID, SI16 siDirection)
{
	CR[siID].Direction = siDirection;
}

SI16 cltCharInterface::GetDirection(SI16 siID)
{
	return CR[siID].Direction;
}

void cltCharInterface::SetStep(SI16 siID, SI16 siStep)
{
	CR[siID].Step = siStep;
}

SI16 cltCharInterface::GetStep(SI16 siID)
{
	return CR[siID].Step;
}

SI08 cltCharInterface::GetWorkDelayor(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetWorkDelayor();
}

void cltCharInterface::SetWorkDelayor(SI16 siID, SI08 siWorkDelayor)
{
	CR[siID].m_cltCharStatus.SetWorkDelayor(siWorkDelayor);
}

SI08 cltCharInterface::GetWorkDelay(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetWorkDelay();
}

void cltCharInterface::IncreaseWorkDelayor(SI16 siID, SI08 siWorkDelayor)
{
	CR[siID].m_cltCharStatus.SetWorkDelayor(CR[siID].m_cltCharStatus.GetWorkDelayor()+siWorkDelayor);
}


void cltCharInterface::IncreaseAttackGiveUpDelayor(SI16 siID, SI16 siAttackGiveUpDelayor)
{
	CR[siID].AttackGiveUpDelayor += siAttackGiveUpDelayor;
}

SI16 cltCharInterface::GetAttackGiveUpDelayor(SI16 siID)
{
	return CR[siID].AttackGiveUpDelayor;
}

SI16 cltCharInterface::GetAttackGiveUpDelay(SI16 siID)
{
	return CR[siID].AttackGiveUpDelay;
}

SI16 cltCharInterface::GetMenuItem(SI16 siID, SI16 siIndex)
{
	return CR[siID].MenuItem[siIndex];
}

//-----------------------------------------------------------------------------
// ���� : ������ ����� ����� ���ش�. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::ClearReserveOrder(SI16 siID)
{
	CR[siID].ClearReserveOrder();
}


SI16 cltCharInterface::GetMenuMode(SI16 siID, SI16 siIndex)
{
	return CR[siID].MenuMode[siIndex];
}

SI16 cltCharInterface::GetMenuNumber(SI16 siID)
{
	return CR[siID].MenuNumber;
}

// ĳ���� ������ �����ش�. 
void cltCharInterface::ShowCharInfo(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface)
{
	CR[siID].ShowCharInfo(lpSurface);
}

// robypark 2004/10/6 15:28
// ĳ������ ������ ������ �����ش�.
// �ʻ�, ü��, ����
void cltCharInterface::ShowCharInfoSimple(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
	CR[siID].ShowCharInfoSimple(lpSurface, siX, siY, siWidth, siHeight);
}

// �þ߸� ó���Ѵ� - �ϱ��� ��� ����
void cltCharInterface::BlackCheckHeroMode(SI16 siID)
{
	CR[siID].BlackCheckHeroMode();
}

CHAR cltCharInterface::GetArea(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetArea();
}

// id�� ġ���� �� �ִ°�?
// id��ü�� �������� ������ Ȯ���Ѵ�. 
BOOL cltCharInterface::CallCanHealGlobal(_NewID id)
{
	return CanHealGlobal(id);
}

BOOL cltCharInterface::CallCanManaGlobal(_NewID id)
{
	return CanManaGlobal(id);
}

// ����� ���ۿ� �ִ´�. 
// ����ڰ� �� ����� �̹� ���۾ȿ� �ִ� ��쿡�� ��ǻ�Ͱ� �� ����� ���� �ʴ´�. 
BOOL cltCharInterface::PushOrder(SI16 siID, _OrderDataInfo order, SHORT mode, SHORT debuginfo)
{
	return CR[siID].PushOrder(order, mode, debuginfo);
}

// ����� ���� ���ۿ� �ִ´�. 
BOOL cltCharInterface::PushReserveOrder(SI16 siID, _OrderDataInfo order)
{
	return CR[siID].PushReserveOrder(order);
}

SI32 cltCharInterface::GetOnlineID(SI16 siID)
{
	return CR[siID].m_siOnlineID;
}

// �ͽ�ź
DWORD cltCharInterface::GetGhostTime(SI16 siID)
{
	return CR[siID].GhostTime;
}

VOID cltCharInterface::SetGhostTime(SI16 siID, DWORD GhostTime)
{
	CR[siID].GhostTime = GhostTime;
}

// ǳ���
DWORD cltCharInterface::GetCloudWindFrame(SI16 siID)
{
	return CR[siID].m_CloudWindFrame;
}

VOID cltCharInterface::SetCloudWindFrame(SI16 siID, DWORD CloudWindFrame)
{
	CR[siID].m_CloudWindFrame = CloudWindFrame;
}

