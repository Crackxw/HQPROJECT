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
	// ���� : Ư�� ������ X Ÿ�� ��ǥ���� ���´�. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetX(SI16 siID);

	void SetY(SI16 siID, SI16 siY);
	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ Y Ÿ�� ��ǥ���� ���´�. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetY(SI16 siID);

	SI16 GetDX(SI16 siID);
	SI16 GetDY(SI16 siID);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ Ÿ�� ������ ���´�. 
	// Desc :
	//-----------------------------------------------------------------------------
	SI16 GetHitPoint(SI16 siID, SI16* siX, SI16* siY);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ �Ӽ��� �˾Ƴ���. 
	// Desc :
	//-----------------------------------------------------------------------------
	BOOL IsAtb(SI16 siID, SI32 siAtb);

	// �ͽ�ź
	DWORD GetGhostTime(SI16 siID);
	VOID SetGhostTime(SI16 siID, DWORD GhostTime);

	// ǳ���
	DWORD GetCloudWindFrame(SI16 siID);
	VOID SetCloudWindFrame(SI16 siID, DWORD CloudWindFrame);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ �ߵ����·� �����.
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

	// ��Ȯ�ϴ� ������ �����Ѵ�.
	VOID SetHDResAmount(SI16 siID, SI16 siAmount);
	// ��Ȯ�ϴ� ������ ���´�.
	SI16 GetHDResAmount(SI16 siID);

	// �̺�Ʈ�� �����Ѵ�.
	VOID SetEventSwitch(SI16 siID, SI16 siEventSwitch);
	// �̺�Ʈ�� ���� ���θ� ���´�.
	SI16 GetEventSwitch(SI16 siID);


	// ������ �޾Ҵ����� ���θ� ���´�.
	SI16 GetBeAttackedSwitch(SI16 siID);

	// �����ϴ� ĳ������ ID�� ���´�.
	SHORT GetAttackID(SI16 siID);
	BOOL IsSameAttackID(SI16 siID, _NewID clID);
	// ���� ���θ� �˷��ش�.
	SI08 GetLowMoveSwitch(SI16 siID);

	// ������ ��� �Ӽ��� �˾ƿ´�. 
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

	// ������ ���ݹް� ������ ȣ���ϴ� �Լ� 
	void SetBeAttackedSwitch(SI16 siID, SHORT enemyid, SHORT attackmeplayer);
	// �����ǹ��� �Էµ� ��ġ(x,y)�� ���� ���� ��ǥ�� ���Ѵ�. 
	BOOL GetBuildAddOnXY(SI16 siID, SHORT kind, SHORT x, SHORT y, SHORT &dx, SHORT &dy);
	BOOL UpdateMenu(SI16 siID, SHORT order);
	// idĳ���͸� ������ �� �ִ��� ���θ� �˷��ش�. 
	BOOL CanAttack(SI16 siID, _NewID id);
	void SetNewSelectedSwitch(SI16 siID, SHORT mode);
	UI32 GetWorkStep(SI16 siID);
	BOOL Action(SI16 siID);
	BOOL DrawSwitch(SI16 siID, SHORT x, SHORT y, SHORT& length);
	// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� (���� ��ǥ)
	BOOL IsAbsPtInObj(SI16 siID, SHORT x, SHORT y, SHORT& length);
	// � rect���� ������ �����ϴ°�?
	BOOL IsAbsPtInObj(SI16 siID, SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	clock_t GetSelectedClock(SI16 siID);
	void SelectOperation(SI16 siID);
	BOOL CanWork(SI16 siID);
	// ĳ���͸� �̵��ϰ� �����. 
	BOOL SetMove(SI16 siID, SHORT dx, SHORT dy);
	void SetWorkStep(SI16 siID, UI32 uiWorkStep);
	void SetAttackGiveUpDelayor(SI16 siID, SI16 AttackGiveUpDelayor);
	int Move(SI16 siID, SHORT moveblockcount, SHORT range=0);
	int GetMovePercent(SI16 siID);
	void SetBusySwitch(SI16 siID, BOOL bBusySwitch);
	// �Ӽ��� ���Ѵ�. 
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
	//������ ����� ����� ���ش�. 
	void ClearReserveOrder(SI16 siID);
	// ����Ǿ��ִ� ����� ������ ���Ѵ�.
	SI16 GetReserveTrainNumber(SI16 siID, SI16 siOrder);
	SI16 GetMenuMode(SI16 siID, SI16 siIndex);
	SI16 IsRDCrewEmpty(SI16 siID, SI16 siIndex);
	SI16 GetMenuNumber(SI16 siID);
	// ĳ���� ������ �����ش�. 
	void ShowCharInfo(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface);

	// robypark 2004/10/6 15:28
	// ĳ������ ������ ������ �����ش�.
	// �ʻ�, ü��, ����
	void ShowCharInfoSimple(SI16 siID, LPDIRECTDRAWSURFACE7 lpSurface,  SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// �þ߸� ó���Ѵ� - �Ϲ� ����
	void BlackCheckNormal(SI16 siID);
	// �þ߸� ó���Ѵ� - �ϱ��� ��� ����
	void BlackCheckHeroMode(SI16 siID);
	CHAR GetArea(SI16 siID);
	// id�� ġ���� �� �ִ°�?
	BOOL CallCanHealGlobal(_NewID id);
	// ������ ġ���� �� �ִ°�?
	BOOL CallCanManaGlobal(_NewID is);
	// ����� ���ۿ� �ִ´�. 
	BOOL PushOrder(SI16 siID, _OrderDataInfo order, SHORT mode, SHORT debuginfo);
	// ����� ���� ���ۿ� �ִ´�. 
	BOOL PushReserveOrder(SI16 siID, _OrderDataInfo order);
	BOOL IsConvoyIDEmpty(SI16 siID);
	void SetPoisonDelay(SI16 siID, SI32 delay);


	SI32 GetOnlineID(SI16 siID);
};
#endif
