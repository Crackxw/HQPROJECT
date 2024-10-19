/************************************************************************************************
	���ϸ� : IOnlineChar.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_ICHAR_H
#define _ONLINE_ICHAR_H

#include "OnlineChar.h"
#include "OnlineGameMsg.h"


class	IOnlineChar
{
protected:
	cltOnlineWorld		*pMyOnlineWorld;		// �¶��� ����.
	cltOnlineChar		*pclOnlineChar;			// ĳ����.

public:
	IOnlineChar();		// ������.	
	~IOnlineChar();		// �Ҹ���.
	
	BOOL	Init(cltOnlineWorld *pWorld);		// �¶��� ĳ������ �������̽��� �ʱ�ȭ �Ѵ�.	
	VOID	Free();								// �¶��� ĳ������ �������̽��� �����Ѵ�.
	
	BOOL	Init(UI16 uiUniqueID,  CHAR *pszID, SI32 siKind, SI16 siX, SI16 siY, DWORD ip, float Ranking, SI32 FightPower, cltOnlineWorld *pWorld, UI16* lpuiEquipWeapon);	// ĳ���͸� �ʱ�ȭ �Ѵ�.		
	BOOL	SetEquipItem(UI16 uiUniqueID, UI16* lpuiEquipWeapon);											// ���� Item ����
	VOID	Free(UI16 uiUniqueID);																			// ĳ���͸� ���ش�.
	VOID	AllFree();																						// ��� ĳ���͸� ���ش�.	
	VOID	Draw(SI16 siID, BOOL fWater);																	// ĳ���� �Ѹ��� �׸���.
	VOID	DrawByVMercenary(SI16 siID, BOOL fWater);														// ĳ���� �Ѹ��� �׸���.(����ٴϴ� �뺴��)	
	VOID	DrawTransparent(SI16 siID, BOOL fWater);														// ��� ĳ���͸� �������ؼ� ����ش�.
	VOID	DrawName(UI16 uiUniqueID, HDC hdc);																// Draw Name
	VOID	DrawGuildFlag(UI16 uiUniqueID, SI16 ScreenX = -1000, SI16 ScreenY = -1000);						// ĳ���� �̸� �տ� Guild ����� ��´�.(���� ��ǥ�� -1�̸� ID ��� �κ� �տ� ��´�.)
	VOID	AllDrawName(HDC hdc);																			// �̸��� ��´�.(hdc�� ����ϴ� ��ο�)
	VOID	AllDrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface);													// ��縦 ��´�.(hdc�� ����ϴ� ��ο�)
	VOID	Action();																						// �׼�
	VOID	ActionByVMercenary();																			// ����ٴϴ� �뺴�� �׼�

	VOID	SetAttackMode(UI16 uiUniqueID, BOOL b);
	BOOL	IsAttackMode(UI16 uiUniqueID);


		
	BOOL	GetPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// Ÿ�� ��ǥ�� ���´�.	
	VOID	GetRPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// Ÿ���� ���� ��ǥ�� ���´�.
	VOID	GetDPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// ���� �̵����� ��ǥ ��ġ�� ���´�.
	VOID	GetNPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// ���� �̵����� ���� ��ġ�� ���´�.	
	VOID	GetNPosByVMercenary(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);									// ���� �̵����� ���� ��ġ�� ���´�.(����ٴϴ� �뺴��)	
	VOID	GetMovePersent(UI16 uiUniqueID, SI16 *ps);														// ���� �̵����� ���� ��ġ�� ������ ���´�.	
	VOID	GetMovePersentByVMercenary(UI16 uiUniqueID, SI16 *ps);											// ���� �̵����� ���� ��ġ�� ������ ���´�.(����ٴϴ� �뺴��)

	void	ClearOrder(UI16 uiUniqueID);
	BOOL	AddOrder(UI16 uiUniqueID, SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2);				// ĳ���� ����� ���ۿ� �ִ´�.
	BOOL	AddOrder(UI16 uiUniqueID, OnOrder Order);														// ĳ���� ����� ���ۿ� �ִ´�.

	VOID	SetID(UI16 uiUniqueID, CHAR *pszID);															// UniqueID�� ����Ͽ� ID(���ڿ�)�� �����Ѵ�.
	SI32	GetID(UI16 uiUniqueID);																			// UniqueID�� ����Ͽ� ID�� ���´�.
	UI16	GetUniqueID(SI16 siID);																			// ĳ������ UniqueID�� ���´�.
	SI32	FindEmptyID();																					// ����ִ� ���̵� ã�´�.
	UI32	GetIP(UI16 uiUniqueID);																			// Get IP

	SI32	GetMovingDirection(UI16 uiUniqueID);															// ���� �̵����� ������ ���Ѵ�.
	BOOL	IsExistUniqueID(UI16 uiUniqueID);																// �� ����ũ ���̵� ���� ĳ���Ͱ� �����ϴ��� �˾ƿ´�.
	UI16	GetIPToUniqueID(DWORD IP);																		// ����ũ ID �˻�

	BOOL	IsMoving(UI16 uiUniqueID);
	VOID	SetMove(UI16 uiUniqueID, SI32 siDX, SI32 siDY);													// ĳ���͸� �̵� ��Ų��.
	VOID	SetMoveByDirection(UI16 uiUniqueID, SI32 siDirection);											// ĳ���͸� �� ������ �� Ÿ�� �̵� ��Ų��.
	VOID	SetWalkingFlag(UI16 uiUniqueID, UI16 Flag);														// ĳ���͸� �̵� Flag

	VOID	SetSpeech(UI16 uiUniqueID, CHAR *pString, BOOL fNotDel = FALSE, CHAR* pID = NULL);				// ĳ������ ��縦 �����Ѵ�.
	BOOL	CheckSpeech(UI16 uiUniqueID);																	// ��簡 ���ִ��� �˻�
	CHAR*	GetCharName(UI16 uiUniqueID);																	// ������� �̸��� ���´�.
	BOOL	IsPointToChar(SI16 siID, LPPOINT pptMouse, SI32 *psiDistance);									// ���� ���콺�� ����Ű�� ĳ���ʹ��� ���´�.

	SI32	GetFightPower(UI16 uiUniqueID);																	// ĳ������ �������� ��´�.
	BOOL	SetFightPower(UI16 uiUniqueID, SI32 FightPower);												// ĳ������ �������� �����Ѵ�.

	BOOL	IsWarp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY,SI16 sidistance);											// ������ �ؾ��ϴ� ��Ȳ�ΰ�?
	VOID	Warp(UI16 uiUniqueID, SI16 siWaprX, SI16 siWarpY);												// ������ �Ѵ�.

	OnOrder	GetCurrentOrder(UI16 uiUniqueID);																// ���� �������� ����� ���´�.

	SI32	GetClass(UI16 uiUniqueID);																		// ĳ������ Ŭ������ ���´�.

	VOID	RecvAroundChars(OnResAroundCharInfoMsg *AroundCharsInfoMsg);									// �� ������ �ִ� ĳ���͵��� ���� ����.
	VOID	SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo);	// �÷��̾��� ������ �������ش�.
	VOID	SetPlayerInfo(UI16 uiUniqueID, OnCharPlayerDetailInfo* lpPlayerInfo);							// �÷��̾��� ������ �������ش�.
	BOOL	SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, UI16 uiUniqueID, BOOL bEffectSound=FALSE);	// ĳ���Ϳ��� �������� ������Ų��.
	SI16	GetWeaponDrawID(UI16 uiUniqueID);																// (��ȭ/���� ���) ���� �����ϰ� �ִ� ���� ID�� ���´�.
	VOID	SetWeaponDrawID(UI16 uiUniqueID, SI16 siWeaponDrawID);											// (��ȭ/���� ���) ���� �����ϰ� �ִ� ���� ID�� �����Ѵ�.

	VOID	SetBattleFlag(UI16 uiUniqueID, UI08 Flag);														// Set Battle Flag
	UI08	GetBattleFlag(UI16 uiUniqueID);																	// Get Battle Flag
	VOID	SetImoticon(UI16 uiUniqueID, UI16 uiImoticon);													// Imoticon ���
	VOID	SetFindTargetFlag(UI16 uiUniqueID, UI08 Flag);													// Set Find Target Flag
	SI32	GetKind(UI16 uiUniqueID);																		// Kind�� ������
	SI32	GetImjinKind(UI16 uiUniqueID);																	// Imjin Kind�� ������. Client���� ���Ǵ� ĳ���� ���� �����

	VOID	SetGuildData(UI16 uiUniqueID, char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag);		// ��� Data ����
	char*	GetGuildName(UI16 uiUniqueID);																	// ��� Name ���
	SI32	GetGuildInGrade(UI16 uiUniqueID);																// ��� ���� ���
	SI32	GetGuildFlag(UI16 uiUniqueID);																	// ��� ��� ���

	BOOL	SetIsHostilityGuild(UI16 uiUniqueID, UI08 uiIsHostilityGuild);									// ���� ������� �˻�
	UI08	GetIsHostilityGuild(UI16 uiUniqueID);															// ���� ������� �˻�

	void	SetNewSelectedFlag(UI16 uiUniqueID, BOOL flag);													// ���� ���õ� ĳ���Ϳ��θ� ���� 
	BOOL	GetNewSelectedFlag(UI16 uiUniqueID);															//���� ���õ� ĳ���Ϳ��θ� ����.

	BOOL	Show(UI16 uiUniqueID, BOOL fShow);																// �������� �˻�
	BOOL	GetShow(UI16 uiUniqueID);																		// �������� �˻�

	BOOL	SetChangeDirection(UI16 uiUniqueID,SI16 siDirection);

	SellItemInfo	*GetBoothItem(UI16 uiUniqueID, UI08 uiSlot);											// ���λ��� ������ ���� ���
	VOID			SetBoothItem(UI16 uiUniqueID, UI08 uiSlot, SellItemInfo ItemInfo);						// ���λ��� ������ ���� ����

	BOOL	IsBoothOpen(UI16 uiUniqueID);																	// ���λ��� ���� ���� ���
	VOID	SetBoothOpen(UI16 uiUniqueID, BOOL bOpenBooth);													// ���λ��� ���� ���� ����

	SI08	GetDataType(UI16 uiUniqueID);
	void	SetDataType(UI16 uiUniqueID, SI08 fDataType);

	BOOL	SetMercenary(UI16 uiUniqueID, SI32 siKind);
	SI32	GetMercenary(UI16 uiUniqueID);

	void	GetEquip(UI16 uiUniqueID, UI16* lpuiEquipWeapon);												// ��� �����͸� ���´�.
	void	SetChangeEffect(UI16 uiUniqueID);																// Set Change Effect

	int		GetScore(UI16 uiUniqueID);
	int		GetRank(UI16 uiUniqueID);

	void	SetRank(UI16 uiUniqueID, UI16 Rank);
	void	SetScore(UI16 uiUniqueID, int Score);
	BOOL	IsInvalidBattle(UI16 uiUniqueID);

	// linus
	// ������ ���� ü�°��� �Լ�
	//ü��.(������ �ִ� �� ü�� ����.)
	void	SetSiegeWarfare_Hp(UI16 uiUniqueID,SI16 siFieldHp);
	//������.(������ �ִ� ü�� - ������ ����)
	void	SetSiegeWarfare_Damage(UI16 uiUniqueID,SI16 siDamage);
	//������ �ִ� ü���� ���´�.
	SI16	GetSiegeWarfare_Hp(UI16 uiUniqueID);

	BOOL	IsFieldAlive(UI16 uiUniqueID);


	// ���� /////////////////////////////////////////
	// robypark 2004/10/27 15:20
	// ������ ���� ü��, ����ǰ, �������� �ð� �Լ�
	// ������ ���� �ִ� ü��ġ ����
	void	SetSiegeWarfare_MaximumHP(UI16 uiUniqueID, SI16 siMaximumHP);
	// ������ ���� �ִ� ü��ġ ���
	SI16	GetSiegeWarfare_MaximumHP(UI16 uiUniqueID);
	// ������ ���� ����ǰ ���緮 ����
	void	SetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID, SI32 siCurrentSupplyGoodsAmount);
	// ������ ���� ����ǰ ���緮 ���
	SI32	GetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID);
	// ������ ���� ����ǰ �ִ뷮 ����
	void	SetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID, SI32 siSupplyGoodsAmount);
	// ������ ���� ����ǰ �ִ뷮 ���
	SI32	GetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID);

	// ������ ������ ���� ���� �ð� ����
	void	SetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID, DWORD dwLastAttackDelay);
	// ������ ������ ����� ���� ���� �ð� ����
	void	SetSiegeWarfareUnit_PastAttackDelay(UI16 uiUniqueID, DWORD dwPastAttackDelay);
	// ������ ������ ���� ���� �ð� ���
	DWORD	GetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID);
	// �� /////////////////////////////////////////	

	// robypark 2004/11/2 17:57
	// ������ �������� ���� ȿ�� ���
	void	SetSiegeWarfare_TransformEffect(UI16 uiUniqueID, SI32 siSiegeWarfareTransformType);

	// robypark 2004/12/2 19:2
	// ������ ������ ����ǰ ���� ȿ�� ����. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	void	SetSiegeWarfare_EffectSupplyGoods(UI16 uiUniqueID, SI32 siEffectSupplyGoodsType);

	// robypark 2004/11/2 18:6
	// ������ ���� ����ǰ �ڵ� �Ҹ�(1%)
	void	WasteSiegeWarfareSupplyGoods(UI16 uiUniqueID);

	// robypark 2004/11/3 15:24
	// ������ ������ ������ ���� ���ݹ޴� ȿ�� ����
	void	SetSiegeWarfare_VillageCountAttack(UI16 uiUniqueID, SI32 siEffectVillageConterAttackType);

	// robypark 2004/11/24 18:31
	// ������ �߰��� ��� ȿ�� ���
	void	SetSiegeWarfare_Evasion(UI16 uiUniqueID);

	// robypark 2004/11/24 18:31
	// ������ ġ���� ���� ȿ�� ���
	void	SetSiegeWarfare_DeathBlow(UI16 uiUniqueID);

	void	SetCaptain(UI16 uiUniqueID,BOOL bCaptain);
	BOOL	GetCaptain(UI16 uiUniqueID);

	// robypark 2004/12/16 14:33
	// ������ �����ΰ�?
	BOOL	IsSiegeWarfareUnit(UI16 uiUniqueID);

	// robypark 2004/12/17 17:20
	// ����ڰ� ���� ���� �������̽� �� �ִ°�?
	BOOL	IsInVillageDefenceInterface(UI16 uiUniqueID);

	// robypark 2004/12/17 17:20
	// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�? ����
	void	SetInVillageDefenceInterface(UI16 uiUniqueID, BOOL bIsInVillageDefenceInterface);

/*
	void	SetSupplyCharge(UI16 uiUniqueID, SI16 siSupply);
	SI16	GetSupplyCharge(UI16 uiUniqueID);
*/
	// robypark 2004/11/12 21:27
	// ������ ������  ü�� �� ����ǰ ��ġ(%) �׸���
	void	DrawSiegeWarfareUnit_StatusBar(UI16 uiUniqueID);

	// robypark 2004/11/13 16:41
	// ��� ������ ������  ü�� �� ����ǰ ��ġ(%) �׸���
	void	AllDrawSiegeWarfareUnit_StatusBar(void);

	// robypark 2004/11/22 10:56
	// ��� ������ ������ ��� ��� �׸���
	void	AllDrawSiegeWarfareUnit_GuildFlag(void);

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷����� �����ϴ� ��ġ ���
	void	GetCharacterDrawCenterPos(UI16 uiUniqueID, SI16 &siCenterX, SI16 &siCenterY);

	// robypark 2005/1/14 19:8
	// ��� ���ΰ� ĳ���͸� �ǳ��� Ȥ�� �Ϲ� ���ΰ� ĳ���ͷ� ���Ž�Ų��.
	void	AllHeroCharacterTransformTo(void);

	// robypark 2005/2/17 21:44
	// ���콺 Ŀ�� �Ʒ� ĳ���������� �����Ѵ�.
	void	SetCursorHoverCharacter(UI16 uiUniqueID, BOOL bHover = FALSE);

	cltOnlineChar	*GetOnlineChar()	{ return pclOnlineChar; }
};


#endif
