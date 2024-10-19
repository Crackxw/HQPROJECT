/************************************************************************************************
	파일명 : IOnlineChar.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_ICHAR_H
#define _ONLINE_ICHAR_H

#include "OnlineChar.h"
#include "OnlineGameMsg.h"


class	IOnlineChar
{
protected:
	cltOnlineWorld		*pMyOnlineWorld;		// 온라인 월드.
	cltOnlineChar		*pclOnlineChar;			// 캐릭터.

public:
	IOnlineChar();		// 생성자.	
	~IOnlineChar();		// 소멸자.
	
	BOOL	Init(cltOnlineWorld *pWorld);		// 온라인 캐릭터의 인터페이스를 초기화 한다.	
	VOID	Free();								// 온라인 캐릭터의 인터페이스를 해제한다.
	
	BOOL	Init(UI16 uiUniqueID,  CHAR *pszID, SI32 siKind, SI16 siX, SI16 siY, DWORD ip, float Ranking, SI32 FightPower, cltOnlineWorld *pWorld, UI16* lpuiEquipWeapon);	// 캐릭터를 초기화 한다.		
	BOOL	SetEquipItem(UI16 uiUniqueID, UI16* lpuiEquipWeapon);											// 장착 Item 지정
	VOID	Free(UI16 uiUniqueID);																			// 캐릭터를 없앤다.
	VOID	AllFree();																						// 모든 캐릭터를 없앤다.	
	VOID	Draw(SI16 siID, BOOL fWater);																	// 캐릭터 한명을 그린다.
	VOID	DrawByVMercenary(SI16 siID, BOOL fWater);														// 캐릭터 한명을 그린다.(따라다니는 용병용)	
	VOID	DrawTransparent(SI16 siID, BOOL fWater);														// 모든 캐릭터를 반투명해서 찍어준다.
	VOID	DrawName(UI16 uiUniqueID, HDC hdc);																// Draw Name
	VOID	DrawGuildFlag(UI16 uiUniqueID, SI16 ScreenX = -1000, SI16 ScreenY = -1000);						// 캐릭터 이름 앞에 Guild 깃발을 찍는다.(만약 좌표가 -1이면 ID 찍는 부분 앞에 찍는다.)
	VOID	AllDrawName(HDC hdc);																			// 이름을 찍는다.(hdc를 사용하는 드로우)
	VOID	AllDrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface);													// 대사를 찍는다.(hdc를 사용하는 드로우)
	VOID	Action();																						// 액션
	VOID	ActionByVMercenary();																			// 따라다니는 용병들 액션

	VOID	SetAttackMode(UI16 uiUniqueID, BOOL b);
	BOOL	IsAttackMode(UI16 uiUniqueID);


		
	BOOL	GetPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// 타일 좌표를 얻어온다.	
	VOID	GetRPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// 타일의 변위 좌표를 얻어온다.
	VOID	GetDPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// 현재 이동중인 목표 위치를 얻어온다.
	VOID	GetNPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);												// 현재 이동중인 다음 위치를 얻어온다.	
	VOID	GetNPosByVMercenary(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY);									// 현재 이동중인 다음 위치를 얻어온다.(따라다니는 용병용)	
	VOID	GetMovePersent(UI16 uiUniqueID, SI16 *ps);														// 현재 이동중인 다음 위치의 비율을 얻어온다.	
	VOID	GetMovePersentByVMercenary(UI16 uiUniqueID, SI16 *ps);											// 현재 이동중인 다음 위치의 비율을 얻어온다.(따라다니는 용병용)

	void	ClearOrder(UI16 uiUniqueID);
	BOOL	AddOrder(UI16 uiUniqueID, SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2);				// 캐릭터 명령을 버퍼에 넣는다.
	BOOL	AddOrder(UI16 uiUniqueID, OnOrder Order);														// 캐릭터 명령을 버퍼에 넣는다.

	VOID	SetID(UI16 uiUniqueID, CHAR *pszID);															// UniqueID를 사용하여 ID(문자열)을 설정한다.
	SI32	GetID(UI16 uiUniqueID);																			// UniqueID를 사용하여 ID를 얻어온다.
	UI16	GetUniqueID(SI16 siID);																			// 캐릭터의 UniqueID를 얻어온다.
	SI32	FindEmptyID();																					// 비어있는 아이디를 찾는다.
	UI32	GetIP(UI16 uiUniqueID);																			// Get IP

	SI32	GetMovingDirection(UI16 uiUniqueID);															// 현재 이동중인 방향을 구한다.
	BOOL	IsExistUniqueID(UI16 uiUniqueID);																// 이 유니크 아이디를 가진 캐릭터가 존재하는지 알아온다.
	UI16	GetIPToUniqueID(DWORD IP);																		// 유니크 ID 검사

	BOOL	IsMoving(UI16 uiUniqueID);
	VOID	SetMove(UI16 uiUniqueID, SI32 siDX, SI32 siDY);													// 캐릭터를 이동 시킨다.
	VOID	SetMoveByDirection(UI16 uiUniqueID, SI32 siDirection);											// 캐릭터를 그 방향을 한 타일 이동 시킨다.
	VOID	SetWalkingFlag(UI16 uiUniqueID, UI16 Flag);														// 캐릭터를 이동 Flag

	VOID	SetSpeech(UI16 uiUniqueID, CHAR *pString, BOOL fNotDel = FALSE, CHAR* pID = NULL);				// 캐릭터의 대사를 설정한다.
	BOOL	CheckSpeech(UI16 uiUniqueID);																	// 대사가 떠있는지 검사
	CHAR*	GetCharName(UI16 uiUniqueID);																	// 사용자의 이름을 얻어온다.
	BOOL	IsPointToChar(SI16 siID, LPPOINT pptMouse, SI32 *psiDistance);									// 현재 마우스가 가리키는 캐릭터는지 얻어온다.

	SI32	GetFightPower(UI16 uiUniqueID);																	// 캐릭터의 전투력을 얻는다.
	BOOL	SetFightPower(UI16 uiUniqueID, SI32 FightPower);												// 캐릭터의 전투력을 지정한다.

	BOOL	IsWarp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY,SI16 sidistance);											// 워프를 해야하는 상황인가?
	VOID	Warp(UI16 uiUniqueID, SI16 siWaprX, SI16 siWarpY);												// 워프를 한다.

	OnOrder	GetCurrentOrder(UI16 uiUniqueID);																// 현재 수행중인 명령을 얻어온다.

	SI32	GetClass(UI16 uiUniqueID);																		// 캐릭터의 클래스를 얻어온다.

	VOID	RecvAroundChars(OnResAroundCharInfoMsg *AroundCharsInfoMsg);									// 내 주위에 있는 캐릭터들의 대한 정보.
	VOID	SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo);	// 플레이어의 정보를 설정해준다.
	VOID	SetPlayerInfo(UI16 uiUniqueID, OnCharPlayerDetailInfo* lpPlayerInfo);							// 플레이어의 정보를 설정해준다.
	BOOL	SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, UI16 uiUniqueID, BOOL bEffectSound=FALSE);	// 캐릭터에게 아이템을 장착시킨다.
	SI16	GetWeaponDrawID(UI16 uiUniqueID);																// (평화/전투 모드) 현재 장착하고 있는 무기 ID를 얻어온다.
	VOID	SetWeaponDrawID(UI16 uiUniqueID, SI16 siWeaponDrawID);											// (평화/전투 모드) 현재 장착하고 있는 무기 ID를 설정한다.

	VOID	SetBattleFlag(UI16 uiUniqueID, UI08 Flag);														// Set Battle Flag
	UI08	GetBattleFlag(UI16 uiUniqueID);																	// Get Battle Flag
	VOID	SetImoticon(UI16 uiUniqueID, UI16 uiImoticon);													// Imoticon 출력
	VOID	SetFindTargetFlag(UI16 uiUniqueID, UI08 Flag);													// Set Find Target Flag
	SI32	GetKind(UI16 uiUniqueID);																		// Kind를 리턴함
	SI32	GetImjinKind(UI16 uiUniqueID);																	// Imjin Kind를 리턴함. Client에서 사용되는 캐릭터 종류 상수값

	VOID	SetGuildData(UI16 uiUniqueID, char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag);		// 길드 Data 지정
	char*	GetGuildName(UI16 uiUniqueID);																	// 길드 Name 얻기
	SI32	GetGuildInGrade(UI16 uiUniqueID);																// 길드 직급 얻기
	SI32	GetGuildFlag(UI16 uiUniqueID);																	// 길드 깃발 얻기

	BOOL	SetIsHostilityGuild(UI16 uiUniqueID, UI08 uiIsHostilityGuild);									// 적대 길드인지 검사
	UI08	GetIsHostilityGuild(UI16 uiUniqueID);															// 적대 길드인지 검사

	void	SetNewSelectedFlag(UI16 uiUniqueID, BOOL flag);													// 새로 선택된 캐릭터여부를 설정 
	BOOL	GetNewSelectedFlag(UI16 uiUniqueID);															//새로 선택된 캐릭터여부를 얻어옴.

	BOOL	Show(UI16 uiUniqueID, BOOL fShow);																// 보여줄지 검사
	BOOL	GetShow(UI16 uiUniqueID);																		// 보여줄지 검사

	BOOL	SetChangeDirection(UI16 uiUniqueID,SI16 siDirection);

	SellItemInfo	*GetBoothItem(UI16 uiUniqueID, UI08 uiSlot);											// 개인상점 아이템 정보 얻기
	VOID			SetBoothItem(UI16 uiUniqueID, UI08 uiSlot, SellItemInfo ItemInfo);						// 개인상점 아이템 정보 지정

	BOOL	IsBoothOpen(UI16 uiUniqueID);																	// 개인상점 개설 여부 얻기
	VOID	SetBoothOpen(UI16 uiUniqueID, BOOL bOpenBooth);													// 개인상점 개설 여부 지정

	SI08	GetDataType(UI16 uiUniqueID);
	void	SetDataType(UI16 uiUniqueID, SI08 fDataType);

	BOOL	SetMercenary(UI16 uiUniqueID, SI32 siKind);
	SI32	GetMercenary(UI16 uiUniqueID);

	void	GetEquip(UI16 uiUniqueID, UI16* lpuiEquipWeapon);												// 장비 데이터를 얻어온다.
	void	SetChangeEffect(UI16 uiUniqueID);																// Set Change Effect

	int		GetScore(UI16 uiUniqueID);
	int		GetRank(UI16 uiUniqueID);

	void	SetRank(UI16 uiUniqueID, UI16 Rank);
	void	SetScore(UI16 uiUniqueID, int Score);
	BOOL	IsInvalidBattle(UI16 uiUniqueID);

	// linus
	// 공성전 유닛 체력관련 함수
	//체력.(가지고 있는 총 체력 설정.)
	void	SetSiegeWarfare_Hp(UI16 uiUniqueID,SI16 siFieldHp);
	//데미지.(가지고 있는 체력 - 데미지 설정)
	void	SetSiegeWarfare_Damage(UI16 uiUniqueID,SI16 siDamage);
	//가지고 있는 체력을 얻어온다.
	SI16	GetSiegeWarfare_Hp(UI16 uiUniqueID);

	BOOL	IsFieldAlive(UI16 uiUniqueID);


	// 시작 /////////////////////////////////////////
	// robypark 2004/10/27 15:20
	// 공성전 유닛 체력, 보급품, 공격지연 시간 함수
	// 공성전 유닛 최대 체력치 설정
	void	SetSiegeWarfare_MaximumHP(UI16 uiUniqueID, SI16 siMaximumHP);
	// 공성전 유닛 최대 체력치 얻기
	SI16	GetSiegeWarfare_MaximumHP(UI16 uiUniqueID);
	// 공성전 유닛 보급품 현재량 설정
	void	SetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID, SI32 siCurrentSupplyGoodsAmount);
	// 공성전 유닛 보급품 현재량 얻기
	SI32	GetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID);
	// 공성전 유닛 보급품 최대량 설정
	void	SetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID, SI32 siSupplyGoodsAmount);
	// 공성전 유닛 보급품 최대량 얻기
	SI32	GetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID);

	// 공성전 유닛의 공격 지연 시간 설정
	void	SetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID, DWORD dwLastAttackDelay);
	// 공성전 유닛의 경과된 공격 지연 시간 설정
	void	SetSiegeWarfareUnit_PastAttackDelay(UI16 uiUniqueID, DWORD dwPastAttackDelay);
	// 공성전 유닛의 공격 지연 시간 얻기
	DWORD	GetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID);
	// 끝 /////////////////////////////////////////	

	// robypark 2004/11/2 17:57
	// 공성전 유닛으로 변신 효과 사용
	void	SetSiegeWarfare_TransformEffect(UI16 uiUniqueID, SI32 siSiegeWarfareTransformType);

	// robypark 2004/12/2 19:2
	// 공성전 유닛이 보급품 관련 효과 설정. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	void	SetSiegeWarfare_EffectSupplyGoods(UI16 uiUniqueID, SI32 siEffectSupplyGoodsType);

	// robypark 2004/11/2 18:6
	// 공성전 유닛 보급품 자동 소모(1%)
	void	WasteSiegeWarfareSupplyGoods(UI16 uiUniqueID);

	// robypark 2004/11/3 15:24
	// 공성전 유닛이 마을로 부터 공격받는 효과 설정
	void	SetSiegeWarfare_VillageCountAttack(UI16 uiUniqueID, SI32 siEffectVillageConterAttackType);

	// robypark 2004/11/24 18:31
	// 공성전 견고한 방어 효과 사용
	void	SetSiegeWarfare_Evasion(UI16 uiUniqueID);

	// robypark 2004/11/24 18:31
	// 공성전 치명적 공격 효과 사용
	void	SetSiegeWarfare_DeathBlow(UI16 uiUniqueID);

	void	SetCaptain(UI16 uiUniqueID,BOOL bCaptain);
	BOOL	GetCaptain(UI16 uiUniqueID);

	// robypark 2004/12/16 14:33
	// 공성전 유닛인가?
	BOOL	IsSiegeWarfareUnit(UI16 uiUniqueID);

	// robypark 2004/12/17 17:20
	// 사용자가 현재 수성 인터페이스 상에 있는가?
	BOOL	IsInVillageDefenceInterface(UI16 uiUniqueID);

	// robypark 2004/12/17 17:20
	// 캐릭터가 현재 수성 인터페이스 상에 있는가? 설정
	void	SetInVillageDefenceInterface(UI16 uiUniqueID, BOOL bIsInVillageDefenceInterface);

/*
	void	SetSupplyCharge(UI16 uiUniqueID, SI16 siSupply);
	SI16	GetSupplyCharge(UI16 uiUniqueID);
*/
	// robypark 2004/11/12 21:27
	// 공성전 유닛의  체력 및 보급품 수치(%) 그리기
	void	DrawSiegeWarfareUnit_StatusBar(UI16 uiUniqueID);

	// robypark 2004/11/13 16:41
	// 모든 공성전 유닛의  체력 및 보급품 수치(%) 그리기
	void	AllDrawSiegeWarfareUnit_StatusBar(void);

	// robypark 2004/11/22 10:56
	// 모든 공성전 유닛의 상단 깃발 그리기
	void	AllDrawSiegeWarfareUnit_GuildFlag(void);

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지기 시작하는 위치 얻기
	void	GetCharacterDrawCenterPos(UI16 uiUniqueID, SI16 &siCenterX, SI16 &siCenterY);

	// robypark 2005/1/14 19:8
	// 모든 주인공 캐릭터를 피난민 혹은 일반 주인공 캐릭터로 변신시킨다.
	void	AllHeroCharacterTransformTo(void);

	// robypark 2005/2/17 21:44
	// 마우스 커서 아래 캐릭터인지를 설정한다.
	void	SetCursorHoverCharacter(UI16 uiUniqueID, BOOL bHover = FALSE);

	cltOnlineChar	*GetOnlineChar()	{ return pclOnlineChar; }
};


#endif
