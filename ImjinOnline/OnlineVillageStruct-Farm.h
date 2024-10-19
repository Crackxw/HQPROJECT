/*********************************************************************************************************
	파일명 : OnlineVillageStruct-Farm.h
	작성자 : 정진욱
	담당자 : 정진욱
	작성일 : 2001.11.20
	수정일 : 2002.01.28
*********************************************************************************************************/

#ifndef __VILLAGE_FARM_H
#define __VILLAGE_FARM_H



#define	ON_MAKE_BUILD						4


class	OnlineVillageBaseStructure;

struct CheckInterface
{
	SI16					m_siX, m_siY;
	BOOL					m_bActive;
//	OnlineEditControl		*m_pEdit01, *m_pEdit02;
	HOEC					m_hOecEdit01,m_hOecEdit02;
	CHAR					m_szEdit01[100], m_szEdit02[100];
	UI08					m_uiKind;
	_Button					m_BOk, m_BCancel, m_BStop, m_BModify;
};

struct BuildWorkInfo
{
	UI16					m_uiItemIndex;																					//	몇번째에 있는 아이템인가?(0~39)
	UI32					m_uiNum;																						//	생산한 개수
	UI32					m_uiFinalNum;																					//	최종 목표 생산수.(0이면 아이템 생산중이지 않음)
};

struct BuildingInfo
{
	_ImageButton			m_pBuilding;
	CHAR					m_szKind[4];																					// 건물의 종류
	BOOL					m_bBuilding;																					// 건물이 있는가?
	SI16					m_siBuilingName;																				// 건물의 이름
//	UI08					m_uiLevel[ON_MAX_FACTORY_NUM-1];																// 건물의 업글 상황
	BOOL					m_bSelect;																						// 선택
	SI16					m_siAniFrame;																					// 건설단계
	UI08					m_uiSlotLevel;																					// 슬롯 레벨
	BOOL					m_bNowBuild;																					// 슬롯을 업글 하는중
	MONEY					m_uiPay;																						// 일당
	UI32					m_uiWork;

	BuildWorkInfo			*m_pWorkInfo;																					// 현재 건물에서 작업 되는 아이템 정보와 일당
};

class	OnlineSelectLB;
class   OnlineVillageStructMake;
class	OnlineListControl;
class	OnlineVillageStructFarm : public OnlineVillageBaseStructure
{
private:
	VillageStructManager	*m_pMyVillage;
	cltOnlineWorld			*m_pMyOnlineWorld;		
	BuildingInfo			m_Building[ ON_MAX_FACTORY_NUM ];
	_Button					m_BExit, m_BMain01, m_BMain02, m_BMain03,m_BBack, m_BOk, m_BStore, m_BStop, m_BModify;
	OnlineSelectLB			m_SelectBuild, m_SelectControl, m_SelectMakeMaterial, m_SelectWorkMaterial,m_SelectItemWorkMaterial;
	OnlineListControl		*m_NpcList, *m_pTempDragList;	
	OnlineVillageStructMake	*m_pMakeList;
	CheckInterface			m_CheckInterface;

	UI08					*m_pCheckButton[ON_MAX_FACTORY_NUM], *m_pBuildButton[ON_MAX_FACTORY_NUM];

	XSPR					m_MainButtonSpr, m_OkSpr, m_CheckSpr, m_CheckLineSpr;
	XSPR					m_TitleSpr, m_BuidingSpr, m_InterfaceSpr,m_ItemInterfaceSpr;
	XSPR					m_BackSpr, m_NpcSpr, m_ItemPannelSpr;
	XSPR					*m_pButton3Spr,m_ItemScrollBase;	
	XSPR					m_ButtonBaseSpr;
	XSPR					m_ButtonCheckBttnSpr;
	XSPR					m_ExitImage, m_ExitBackImage;
	XSPR					m_ScrollUpImage,m_ScrollDnImage;
	_Button					m_BDestroy, m_BSubExit;
	_Button					m_BScrUp,   m_BScrDn;
	_Button					m_BScr2Up,	m_BScr2Dn;
	_Button					m_BScrBuildUp,m_BScrBuildDn;

	SI16					m_siX, m_siY;

	SI32					m_siNextStatus;																					// 다음 갈곳
	SI32					m_siPrevStatus;																					//

	BOOL					m_bMouseDown;																					// 마우스 상태
	BOOL					m_bUpGrade;																						// 생산 건물수를 4 -> 6개로
	BOOL					m_bOpen, m_bRecvOk;
	BOOL					m_bNoBuilding;
	BOOL					m_bInFactory;																					// 건물에 들어갔는지 체크
	BOOL					m_bWorkCompleted;																				// 작업중 건물이 완성되었는지
	SI16					m_siMouseStatus;																				// 마우스 상태
	SI16					m_siTempConunt;

	SI16					m_siShowFont;																					// 현재 보이는 스프라이트 폰트 번호
	SI16					m_siSelect;																						// 선택한 건물이나 바닥
	DWORD					m_siScrollX, m_siRight, m_siScrollY;															// 스크롤
	SI16					m_siXpos[ ON_MAX_FACTORY_NUM ];																	// 건물의 X 좌표
	SI16					m_siYpos[ ON_MAX_FACTORY_NUM ];																	// 건물의 Y 좌표
//	SI32					m_siMsgResult;
	BOOL					m_bExtension;																					// 슬롯 확장 플래그

	SI16					m_siBuildSelect;																				// 지을 수 있는 건물 건택
	CHAR					m_szBuildCode[FACILITY_CODE_NUM][ON_MAKE_BUILD];
	CHAR					m_szKind[ON_MAKE_BUILD];

	DWORD					m_dwStart;																						// 스크롤용
	UI32					m_uiItemID;																						// 드래그 중인 아이템 아이디
	SI16					m_siSelectLB;																					// 현재 선택한 케릭터
	SI16					m_siPos;																						// 아이템 위치( 인벤토리에서 )
	UI16					m_uiQuantity;																					// 아아템 수량
	SI16					m_siYetPos;																						// 전 위치
	SI16					m_siMiniGameScore;
	SI32					m_siMiniGameType;
	SI16					m_siQuantity;
	MONEY					m_siPay;
	BOOL					m_bMiniGame;
	CHAR					m_szTalk[ 1024 ];
	SI32					m_siTalk;
	UI16					*m_pItemID;
	SI16					m_siBuildIndex;

	BOOL					bNowScroll;
	SI16					m_siTempX, m_siTempY, m_siTempZ;

	DWORD					m_dwDelayTime;																					// 마우스 입력 딜레이
	CHAR					*m_pTitleName;
	RECT					m_rtNpcRect, m_rtPlayerRect;

public:
	SI32					m_siStatus;																						// 전체적인 상태를 체크하는 변수
	SI32					m_siBuildStatus;

	BOOL					m_bExitOk;

public:
	OnlineVillageStructFarm();
	virtual ~OnlineVillageStructFarm();

	virtual VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);	// 초기화
	virtual VOID			Free();																							// 해제.
	virtual BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);															// 액션.	
	virtual BOOL			Action();

	UI16	FarmWorkCompleted(BYTE *pMsg);					// 생산시설 현재 작업이 완료되었다
	UI16	FarmSlotInfo(BYTE *pMsg);						// 생산시설 슬롯(건물) 정보
	UI16	FarmSlotExpand(BYTE *pMsg);						// 생산시설 슬롯 확장
	UI16	FarmExit(BYTE *pMsg);							// 생산시설 나가기
	UI16	FarmStructBuild(BYTE *pMsg);					// 생산시설 건물 건설
	UI16	FarmStructDestroy(BYTE *pMsg);					// 생산시설 건물 철거
	UI16	FarmStructUpgrade(BYTE *pMsg);					// 생산시설 건물 업그레이드
	UI16	FarmStructExit(BYTE *pMsg);						// 생산시설 건물 나가기
	UI16	FarmStorageInfo(BYTE *pMsg);					// 생산시설 창고 정보
	UI16	FarmStorageItemInput(BYTE *pMsg);				// 생산시설 창고 아이템 넣기
	UI16	FarmStorageItemOutput(BYTE *pMsg);				// 생산시설 창고 아이템 가져오기
	UI16	FarmProductInfo(BYTE *pMsg);					// 생산시설 아이템 생산 정보
	UI16	FarmProduct(BYTE *pMsg);						// 생산시설 아이템 생산하기
	UI16	FarmProductCancel(BYTE *pMsg);					// 생산시설 아이템 생산 취소
	UI16	FarmProductPayChange(BYTE *pMsg);				// 생산시설 아이템 생산 임금 변경
	UI16	FarmWorkStart(BYTE *pMsg);						// 생산시설 일하기 시작(일할수있는지 체크)
	UI16	FarmWorkResult(BYTE *pMsg);						// 생산시설 일한후 돈받기

private:
	VOID					CreateFarm(OnResFarmSlotInfoMsg	*pOnResFarmSlotInfoMsg);																	// 이미지 버튼을 생성을 한다.( 각 건물이 있는가 없는가에 따라서 크기와 좌표가 달라 진다.)
	VOID					ScrollDraw(LPDIRECTDRAWSURFACE7 pSurface, SI16 x, SI16 y, SI16 z);								// 스크롤용 드로우

	VOID					BuildAction();																					// 건설 액션
	VOID					BuildDraw(LPDIRECTDRAWSURFACE7 pSurface);														// 건설 드로우
	VOID					StoreAction();
	VOID					StorehouseDraw(LPDIRECTDRAWSURFACE7 pSurface);													// 창고 드로우
	VOID					ItemMakeAction();																				// 아이템 만들기 액션
	VOID					ItemMakeDraw(LPDIRECTDRAWSURFACE7 pSurface);													// 아이템 만들기 드로우
	VOID					WorkAction();
	VOID					WorkDraw(LPDIRECTDRAWSURFACE7 pSurface);


	VOID					SetShowCheckBox(UI08 uiKind = 0);
	SI16					CheckAction();
	VOID					CheckDraw(LPDIRECTDRAWSURFACE7 pSurface);
	VOID					CheckDraw2(LPDIRECTDRAWSURFACE7 pSurface);
	VOID					ModifyDraw(LPDIRECTDRAWSURFACE7 pSurface);
	VOID					Initialize();
};


#endif
