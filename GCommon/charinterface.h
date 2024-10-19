#ifndef _CHARINTERFACE_H
#define _CHARINTERFACE_H

class cltCharInterface 
{
public:
	cltCharInterface();

	void SaveChar(FILE* fp);
	void LoadChar(FILE* fp);

	void SetID(SI16 siID);
	_NewID GetID(SI16 siID);
	SI16 GetPlayerNumber(SI16 siID);
	SI16 GetOriginalPlayerNumber(SI16 siID);
    SI16 GetKind(SI16 siID);


	void SetX(SI16 siID, SI16 siX);
	//-----------------------------------------------------------------------------
	// 설명 : 특정 유닛의 X 타일 좌표값을 얻어온다. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetX(SI16 siID);

	void SetY(SI16 siID, SI16 siY);
	//-----------------------------------------------------------------------------
	// 설명 : 특정 유닛의 Y 타일 좌표값을 얻어온다. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetY(SI16 siID);

	SI16 GetDX(SI16 siID);
	SI16 GetDY(SI16 siID);

	//-----------------------------------------------------------------------------
	// 설명 : 특정 유닛의 타격 지점을 얻어온다. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetHitPoint(SI16 siID, SI16* siX, SI16* siY);

	//-----------------------------------------------------------------------------
	// 설명 : 특정 유닛의 속성을 알아낸다. 
	// Desc :
	//-----------------------------------------------------------------------------
	BOOL IsAtb(SI16 siID, SI32 siAtb);

	// 귀신탄
	DWORD GetGhostTime(SI16 siID);
	VOID SetGhostTime(SI16 siID, DWORD GhostTime);

	// 풍모술
	DWORD GetCloudWindFrame(SI16 siID);
	VOID SetCloudWindFrame(SI16 siID, DWORD CloudWindFrame);

	//-----------------------------------------------------------------------------
	// 설명 : 특정 유닛을 중독상태로 만든다.
	// Desc :
	//-----------------------------------------------------------------------------
	BOOL SetPoisonSwitch(SI16 siID, BOOL bMode);
		
	SI16 GetDpower(SI16 siID);
	SI16 GetAssistDpower(SI16 siID);

	BOOL IsColl(SI16 siID, SI16 siX, SI16 siY);

	BOOL RideOn(SI16 siID, _NewID clID);

	BOOL IsCover(SI16 siID);

	UI08 GetTeamColor(SI16 siID);

	SI16 GetGroup(SI16 siID);
	void SetGroup(SI16 siID, SI16 siGroup);

	SI16 GetMaxCrewNumber(SI16 siID);
	BOOL GetSeeSwitch(SI16 siID);
	void SetSeeSwitch(SI16 siID, BOOL bMode);
	BOOL SetSelect(SI16 siID, BOOL bMode, BOOL bEffectSwitch);
	BOOL GetGuardSwitch(SI16 siID);
	void SetGuardSwitch(SI16 siID, BOOL bGuardMode);
	void SetGuardXY(SI16 siID, SI16 siX, SI16 siY);

	BOOL ChangePlayer(SI16 siID, SI16 siPlayerNumber);

	BOOL DeleteID(SI16 siID);
	void SetDrawFrame(SI16 siID, SI32 siDrawFrame);

	SI16 GetCurrentOrder(SI16 siID);

	SHORT GetCrewNumber(SI16 siID);

	VOID SetAutoSwitch(SI16 siID, BOOL bMode);
	BOOL GetAutoSwitch(SI16 siID);

	VOID SetHostMode(SI16 siID, CHAR HostMode);
	CHAR GetHostMode(SI16 siID);

	VOID SetHDResType(SI16 siID, SI16 siType);
	SI16 GetHDResType(SI16 siID);

	// 수확하는 종류를 설정한다.
	VOID SetHDResAmount(SI16 siID, SI16 siAmount);
	// 수확하는 종류를 얻어온다.
	SI16 GetHDResAmount(SI16 siID);

	// 이벤트를 설정한다.
	VOID SetEventSwitch(SI16 siID, SI16 siEventSwitch);
	// 이벤트의 설정 여부를 얻어온다.
	SI16 GetEventSwitch(SI16 siID);


	// 공격을 받았는지의 여부를 얻어온다.
	SI16 GetBeAttackedSwitch(SI16 siID);

	// 공격하는 캐릭터의 ID를 얻어온다.
	SHORT GetAttackID(SI16 siID);
	BOOL IsSameAttackID(SI16 siID, _NewID clID);
	// 포복 여부를 알려준다.
	SI08 GetLowMoveSwitch(SI16 siID);

	// 유닛의 방어 속성을 알아온다. 
	SI32 GetDefenceAtb(SI16 siID);

	void SetLife(SI16 siID, SI16 siHealth);
	SI16 GetRDCrewID(SI16 siID, SI16 num);
	BOOL IncreaseLife(SI16 siID, SHORT amount);
	BOOL ReduceLife(SI16 siID, SHORT amount, SHORT unique);

	SI16 GetCenterX(SI16 siID);
	SI16 GetCenterY(SI16 siID);
	UI16 GetPara1(SI16 siID);
	SI16 GetApower(SI16 siID);
	SI16 GetLife(SI16 siID);
	SI16 GetMana(SI16 siID);

	// 적에게 공격받고 있을때 호출하는 함수 
	void SetBeAttackedSwitch(SI16 siID, SHORT enemyid, SHORT attackmeplayer);
	// 보조건물을 입력된 위치(x,y)에 따라 지을 좌표를 구한다. 
	BOOL GetBuildAddOnXY(SI16 siID, SHORT kind, SHORT x, SHORT y, SHORT &dx, SHORT &dy);
	BOOL UpdateMenu(SI16 siID, SHORT order);
	// id캐릭터를 공격할 수 있는지 여부를 알려준다. 
	BOOL CanAttack(SI16 siID, _NewID id);
	void SetNewSelectedSwitch(SI16 siID, SHORT mode);
	UI32 GetWorkStep(SI16 siID);
	BOOL Action(SI16 siID);
	BOOL DrawSwitch(SI16 siID, SHORT x, SHORT y, SHORT& length);
	// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
	BOOL IsAbsPtInObj(SI16 siID, SHORT x, SHORT y, SHORT& length);
	// 어떤 rect와의 교점이 존재하는가?
	BOOL IsAbsPtInObj(SI16 siID, SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	clock_t GetSelectedClock(SI16 siID);
	void SelectOperation(SI16 siID);
	BOOL CanWork(SI16 siID);
	// 캐릭터를 이동하게 만든다. 
	BOOL SetMove(SI16 siID, SHORT dx, SHORT dy);
	void SetWorkStep(SI16 siID, UI32 uiWorkStep);
	void SetAttackGiveUpDelayor(SI16 siID, SI16 AttackGiveUpDelayor);
	int Move(SI16 siID, SHORT moveblockcount, SHORT range=0);
	int GetMovePercent(SI16 siID);
	void SetBusySwitch(SI16 siID, BOOL bBusySwitch);
	// 속성을 구한다. 
	BOOL IsAtb2(SI16 siID, DWORD atb);
	void SetDirection(SI16 siID, SI16 siDirection);
	SI16 GetDirection(SI16 siID);
	void SetStep(SI16 siID, SI16 siStep);
	SI16 GetStep(SI16 siID);
	void SetWorkDelayor(SI16 siID, SI08 siWorkDelayor);
	SI08 GetWorkDelayor(SI16 siID);
	SI08 GetWorkDelay(SI16 siID);
	void IncreaseWorkDelayor(SI16 siID, SI08 siWorkDelayor);
	void SetAnimation(SI16 siID, SI08 siAnimation);
	void SetAniStep(SI16 siID, SI16 siAniStep);
	void SetUpdateAnimationSwitch(SI16 siID, BOOL bUpdateAnimationSwitch);
	void IncreaseAttackGiveUpDelayor(SI16 siID, SI16 siAttackGiveUpDelayor);
	SI16 GetAttackGiveUpDelayor(SI16 siID);
	SI16 GetAttackGiveUpDelay(SI16 siID);
	SI16 GetMenuItem(SI16 siID, SI16 siIndex);
	//유닛의 예약된 명령을 없앤다. 
	void ClearReserveOrder(SI16 siID);
	// 예약되어있는 명령의 갯수를 구한다.
	SI16 GetReserveTrainNumber(SI16 siID, SI16 siOrder);
	SI16 GetMenuMode(SI16 siID, SI16 siIndex);
	SI16 IsRDCrewEmpty(SI16 siID, SI16 siIndex);
	SI16 GetMenuNumber(SI16 siID);
	// 캐릭터 정보를 보여준다. 
	void ShowCharInfo(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface);

	// robypark 2004/10/6 15:28
	// 캐릭터의 간략한 정보를 보여준다.
	// 초상, 체력, 마법
	void ShowCharInfoSimple(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface,  SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// 시야를 처리한다 - 일반 게임
	void BlackCheckNormal(SI16 siID);
	// 시야를 처리한다 - 일기토 모드 게임
	void BlackCheckHeroMode(SI16 siID);
	CHAR GetArea(SI16 siID);
	// id를 치료할 수 있는가?
	BOOL CallCanHealGlobal(_NewID id);
	// 마나를 치료할 수 있는가?
	BOOL CallCanManaGlobal(_NewID is);
	// 명령을 버퍼에 넣는다. 
	BOOL PushOrder(SI16 siID, _OrderDataInfo order, SHORT mode, SHORT debuginfo);
	// 명령을 예약 버퍼에 넣는다. 
	BOOL PushReserveOrder(SI16 siID, _OrderDataInfo order);
	BOOL IsConvoyIDEmpty(SI16 siID);
	void SetPoisonDelay(SI16 siID, SI32 delay);


	SI32 GetOnlineID(SI16 siID);
};
#endif
