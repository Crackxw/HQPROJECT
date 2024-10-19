//--------------------------------------------------------------------
//  담당 : 김태곤
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
// 설명 : 특정 유닛의 아이디를 구한다. 
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
// 설명 : 특정 유닛의 플레이어 번호를  구한다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetPlayerNumber(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetPlayerNumber();
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 플레이어 번호를  구한다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetOriginalPlayerNumber(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetOriginalPlayerNumber();
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 타입을   구한다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetKind(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetKind();
}


//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 X 타일 좌표값을 얻어온다. 
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
// 설명 : 특정 유닛의 Y 타일 좌표값을 얻어온다. 
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
// 설명 : 특정 유닛의 타격 지점을 얻어온다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetHitPoint(SI16 siID, SI16* siX, SI16* siY)
{
	CR[siID].GetHitPoint(*siX, *siY);

	return 0;
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 속성을 알아낸다. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsAtb(SI16 siID, SI32 siAtb)
{
	return CR[siID].IsAtb(siAtb);
}


//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 공격력을 얻어온다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetDpower(SI16 siID)
{
	return CR[siID].GetDpower();
}


//-----------------------------------------------------------------------------
// 설명 : 특정 유닛이 특정 위치로 갈 수 있는지 알려준다.
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsColl(SI16 siID, SI16 siX, SI16 siY)
{
	return CR[siID].IsColl(siX, siY);
}


//-----------------------------------------------------------------------------
// 설명 : 위장 중인지 여부를 알려준다. (천년의 신화인 경우에만 의미가 있다. )
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::IsCover(SI16 siID)
{
	return  FALSE;
   
}

//-----------------------------------------------------------------------------
// 설명 : 특정 캐릭터의 대표색을  구한다. 
// Desc :
//-----------------------------------------------------------------------------
UI08 cltCharInterface::GetTeamColor(SI16 siID)
{
   return CR[siID].GetTeamColor();
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛이 속한 그룹을 알려준다. 
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetGroup(SI16 siID)
{
	return CR[siID].Group;
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛이 속한 그룹을 설정한다. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGroup(SI16 siID, SI16 siGroup)
{
	CR[siID].Group = siGroup;
}



//-----------------------------------------------------------------------------
// 설명 : 특정 유닛이 보이는 상태인지 알려준다. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetSeeSwitch(SI16 siID)
{
    return CR[siID].m_scSeeSwitch;
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 보이는 상태여부를 설정한다. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetSeeSwitch(SI16 siID, BOOL bMode)
{
    CR[siID].m_scSeeSwitch = bMode;
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 선택 상태를 변경한다. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::SetSelect(SI16 siID, BOOL bMode, BOOL bEffectSwitch)
{
    return CR[siID].SetSelect(bMode, bEffectSwitch);
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 DrawFrame을 설정한다. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetDrawFrame(SI16 siID, SI32 siDrawFrame)
{
	CR[siID].m_cltCharDrawInfo.SetDrawFrame(siDrawFrame);
}


//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 아이디를 삭제한다. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::DeleteID(SI16 siID)
{
	return CR[siID].DeleteID();
}

//-----------------------------------------------------------------------------
// 설명 : 경계 모드를 얻어온다. 
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetGuardSwitch(SI16 siID)
{
	return CR[siID].GuardSwitch;
}

//-----------------------------------------------------------------------------
// 설명 : 경계 모드를 설정한다. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGuardSwitch(SI16 siID, BOOL bGuardMode)
{
	CR[siID].GuardSwitch = bGuardMode;
}

//-----------------------------------------------------------------------------
// 설명 : 경계 지역의 x, y좌표를 설정한다. 
// Desc :
//-----------------------------------------------------------------------------
void cltCharInterface::SetGuardXY(SI16 siID, SI16 siX, SI16 siY)
{
	CR[siID].GuardX = siX;
	CR[siID].GuardY = siY;
}


//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 플레이어 번호를 변경한다.  
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::ChangePlayer(SI16 siID, SI16 siPlayerNumber)
{
	return CR[siID].ChangePlayer(siPlayerNumber);
}

//-----------------------------------------------------------------------------
// 설명 : 특정 유닛의 현재 명령 상태를 얻어온다.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetCurrentOrder(SI16 siID)
{
	return CR[siID].CurrentOrder;
}



//-----------------------------------------------------------------------------
// 설명 : 자동수행 모드를 설정한다.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetAutoSwitch(SI16 siID, BOOL bMode)
{
	CR[siID].AutoSwitch = bMode;	
}

//-----------------------------------------------------------------------------
// 설명 : 자동수행 모드를 얻어온다.
// Desc :
//-----------------------------------------------------------------------------
BOOL cltCharInterface::GetAutoSwitch(SI16 siID)
{
	return CR[siID].AutoSwitch;
}

//-----------------------------------------------------------------------------
// 설명 : Host Mode를 설정한다.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetHostMode(SI16 siID, CHAR HostMode)
{
	CR[siID].HostMode	=	HostMode;
}

//-----------------------------------------------------------------------------
// 설명 : Host Mode를 얻어온다.
// Desc :
//-----------------------------------------------------------------------------
CHAR cltCharInterface::GetHostMode(SI16 siID)
{
	return CR[siID].HostMode;	
}


//-----------------------------------------------------------------------------
// 설명 : 이벤트를 설정한다.
// Desc :
//-----------------------------------------------------------------------------
VOID cltCharInterface::SetEventSwitch(SI16 siID, SI16 siEventSwitch)
{
	CR[siID].EventSwitch	=	siEventSwitch;
}

//-----------------------------------------------------------------------------
// 설명 : 이벤트의 설정 여부를 얻어온다.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetEventSwitch(SI16 siID)
{
	return CR[siID].EventSwitch;
}


//-----------------------------------------------------------------------------
// 설명 : 공격을 받았는지의 여부를 얻어온다.
// Desc :
//-----------------------------------------------------------------------------
SI16 cltCharInterface::GetBeAttackedSwitch(SI16 siID)
{
	return CR[siID].BeAttackedSwitch;
}
	
//-----------------------------------------------------------------------------
// 설명 : 공격하는 캐릭터의 ID를 얻어온다.
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
// 설명 : 포복 여부를 알려준다.
// Desc :
//-----------------------------------------------------------------------------
SI08 cltCharInterface::GetLowMoveSwitch(SI16 siID)
{
	return CR[siID].LowMoveSwitch;
}


//-----------------------------------------------------------------------------
// 설명 : 유닛의 방어 속성을 알아온다. 
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

// 적에게 공격받고 있을때 호출하는 함수 
void cltCharInterface::SetBeAttackedSwitch(SI16 siID, SHORT enemyid, SHORT attackmeplayer)
{
	CR[siID].SetBeAttackedSwitch(enemyid, attackmeplayer);
}


BOOL cltCharInterface::UpdateMenu(SI16 siID, SHORT order)
{
	return CR[siID].UpdateMenu(order);
}

// id캐릭터를 공격할 수 있는지 여부를 알려준다. 
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


// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
// length: 중심과의 거리 
BOOL cltCharInterface::DrawSwitch(SI16 siID, SHORT x, SHORT y, SHORT& length)
{
	return CR[siID].IsAbsPtInObj(GameRealX, GameRealY, length);
}

// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
// length: 중심과의 거리 
BOOL cltCharInterface::IsAbsPtInObj(SI16 siID, SHORT x, SHORT y, SHORT& length)
{
	return CR[siID].IsAbsPtInObj(x, y, length);
}

// 어떤 rect와의 교점이 존재하는가?
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

// 캐릭터를 이동하게 만든다. 
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

// 속성을 구한다. 
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
// 설명 : 유닛의 예약된 명령을 없앤다. 
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

// 캐릭터 정보를 보여준다. 
void cltCharInterface::ShowCharInfo(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface)
{
	CR[siID].ShowCharInfo(lpSurface);
}

// robypark 2004/10/6 15:28
// 캐릭터의 간략한 정보를 보여준다.
// 초상, 체력, 마법
void cltCharInterface::ShowCharInfoSimple(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight)
{
	CR[siID].ShowCharInfoSimple(lpSurface, siX, siY, siWidth, siHeight);
}

// 시야를 처리한다 - 일기토 모드 게임
void cltCharInterface::BlackCheckHeroMode(SI16 siID)
{
	CR[siID].BlackCheckHeroMode();
}

CHAR cltCharInterface::GetArea(SI16 siID)
{
	return CR[siID].m_cltCharStatus.GetArea();
}

// id를 치료할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL cltCharInterface::CallCanHealGlobal(_NewID id)
{
	return CanHealGlobal(id);
}

BOOL cltCharInterface::CallCanManaGlobal(_NewID id)
{
	return CanManaGlobal(id);
}

// 명령을 버퍼에 넣는다. 
// 사용자가 준 명령이 이미 버퍼안에 있는 경우에는 컴퓨터가 준 명령은 넣지 않는다. 
BOOL cltCharInterface::PushOrder(SI16 siID, _OrderDataInfo order, SHORT mode, SHORT debuginfo)
{
	return CR[siID].PushOrder(order, mode, debuginfo);
}

// 명령을 예약 버퍼에 넣는다. 
BOOL cltCharInterface::PushReserveOrder(SI16 siID, _OrderDataInfo order)
{
	return CR[siID].PushReserveOrder(order);
}

SI32 cltCharInterface::GetOnlineID(SI16 siID)
{
	return CR[siID].m_siOnlineID;
}

// 귀신탄
DWORD cltCharInterface::GetGhostTime(SI16 siID)
{
	return CR[siID].GhostTime;
}

VOID cltCharInterface::SetGhostTime(SI16 siID, DWORD GhostTime)
{
	CR[siID].GhostTime = GhostTime;
}

// 풍모술
DWORD cltCharInterface::GetCloudWindFrame(SI16 siID)
{
	return CR[siID].m_CloudWindFrame;
}

VOID cltCharInterface::SetCloudWindFrame(SI16 siID, DWORD CloudWindFrame)
{
	CR[siID].m_CloudWindFrame = CloudWindFrame;
}

