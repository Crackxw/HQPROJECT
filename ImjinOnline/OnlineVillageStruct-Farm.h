/*********************************************************************************************************
	���ϸ� : OnlineVillageStruct-Farm.h
	�ۼ��� : ������
	����� : ������
	�ۼ��� : 2001.11.20
	������ : 2002.01.28
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
	UI16					m_uiItemIndex;																					//	���°�� �ִ� �������ΰ�?(0~39)
	UI32					m_uiNum;																						//	������ ����
	UI32					m_uiFinalNum;																					//	���� ��ǥ �����.(0�̸� ������ ���������� ����)
};

struct BuildingInfo
{
	_ImageButton			m_pBuilding;
	CHAR					m_szKind[4];																					// �ǹ��� ����
	BOOL					m_bBuilding;																					// �ǹ��� �ִ°�?
	SI16					m_siBuilingName;																				// �ǹ��� �̸�
//	UI08					m_uiLevel[ON_MAX_FACTORY_NUM-1];																// �ǹ��� ���� ��Ȳ
	BOOL					m_bSelect;																						// ����
	SI16					m_siAniFrame;																					// �Ǽ��ܰ�
	UI08					m_uiSlotLevel;																					// ���� ����
	BOOL					m_bNowBuild;																					// ������ ���� �ϴ���
	MONEY					m_uiPay;																						// �ϴ�
	UI32					m_uiWork;

	BuildWorkInfo			*m_pWorkInfo;																					// ���� �ǹ����� �۾� �Ǵ� ������ ������ �ϴ�
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

	SI32					m_siNextStatus;																					// ���� ����
	SI32					m_siPrevStatus;																					//

	BOOL					m_bMouseDown;																					// ���콺 ����
	BOOL					m_bUpGrade;																						// ���� �ǹ����� 4 -> 6����
	BOOL					m_bOpen, m_bRecvOk;
	BOOL					m_bNoBuilding;
	BOOL					m_bInFactory;																					// �ǹ��� ������ üũ
	BOOL					m_bWorkCompleted;																				// �۾��� �ǹ��� �ϼ��Ǿ�����
	SI16					m_siMouseStatus;																				// ���콺 ����
	SI16					m_siTempConunt;

	SI16					m_siShowFont;																					// ���� ���̴� ��������Ʈ ��Ʈ ��ȣ
	SI16					m_siSelect;																						// ������ �ǹ��̳� �ٴ�
	DWORD					m_siScrollX, m_siRight, m_siScrollY;															// ��ũ��
	SI16					m_siXpos[ ON_MAX_FACTORY_NUM ];																	// �ǹ��� X ��ǥ
	SI16					m_siYpos[ ON_MAX_FACTORY_NUM ];																	// �ǹ��� Y ��ǥ
//	SI32					m_siMsgResult;
	BOOL					m_bExtension;																					// ���� Ȯ�� �÷���

	SI16					m_siBuildSelect;																				// ���� �� �ִ� �ǹ� ����
	CHAR					m_szBuildCode[FACILITY_CODE_NUM][ON_MAKE_BUILD];
	CHAR					m_szKind[ON_MAKE_BUILD];

	DWORD					m_dwStart;																						// ��ũ�ѿ�
	UI32					m_uiItemID;																						// �巡�� ���� ������ ���̵�
	SI16					m_siSelectLB;																					// ���� ������ �ɸ���
	SI16					m_siPos;																						// ������ ��ġ( �κ��丮���� )
	UI16					m_uiQuantity;																					// �ƾ��� ����
	SI16					m_siYetPos;																						// �� ��ġ
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

	DWORD					m_dwDelayTime;																					// ���콺 �Է� ������
	CHAR					*m_pTitleName;
	RECT					m_rtNpcRect, m_rtPlayerRect;

public:
	SI32					m_siStatus;																						// ��ü���� ���¸� üũ�ϴ� ����
	SI32					m_siBuildStatus;

	BOOL					m_bExitOk;

public:
	OnlineVillageStructFarm();
	virtual ~OnlineVillageStructFarm();

	virtual VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);	// �ʱ�ȭ
	virtual VOID			Free();																							// ����.
	virtual BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);															// �׼�.	
	virtual BOOL			Action();

	UI16	FarmWorkCompleted(BYTE *pMsg);					// ����ü� ���� �۾��� �Ϸ�Ǿ���
	UI16	FarmSlotInfo(BYTE *pMsg);						// ����ü� ����(�ǹ�) ����
	UI16	FarmSlotExpand(BYTE *pMsg);						// ����ü� ���� Ȯ��
	UI16	FarmExit(BYTE *pMsg);							// ����ü� ������
	UI16	FarmStructBuild(BYTE *pMsg);					// ����ü� �ǹ� �Ǽ�
	UI16	FarmStructDestroy(BYTE *pMsg);					// ����ü� �ǹ� ö��
	UI16	FarmStructUpgrade(BYTE *pMsg);					// ����ü� �ǹ� ���׷��̵�
	UI16	FarmStructExit(BYTE *pMsg);						// ����ü� �ǹ� ������
	UI16	FarmStorageInfo(BYTE *pMsg);					// ����ü� â�� ����
	UI16	FarmStorageItemInput(BYTE *pMsg);				// ����ü� â�� ������ �ֱ�
	UI16	FarmStorageItemOutput(BYTE *pMsg);				// ����ü� â�� ������ ��������
	UI16	FarmProductInfo(BYTE *pMsg);					// ����ü� ������ ���� ����
	UI16	FarmProduct(BYTE *pMsg);						// ����ü� ������ �����ϱ�
	UI16	FarmProductCancel(BYTE *pMsg);					// ����ü� ������ ���� ���
	UI16	FarmProductPayChange(BYTE *pMsg);				// ����ü� ������ ���� �ӱ� ����
	UI16	FarmWorkStart(BYTE *pMsg);						// ����ü� ���ϱ� ����(���Ҽ��ִ��� üũ)
	UI16	FarmWorkResult(BYTE *pMsg);						// ����ü� ������ ���ޱ�

private:
	VOID					CreateFarm(OnResFarmSlotInfoMsg	*pOnResFarmSlotInfoMsg);																	// �̹��� ��ư�� ������ �Ѵ�.( �� �ǹ��� �ִ°� ���°��� ���� ũ��� ��ǥ�� �޶� ����.)
	VOID					ScrollDraw(LPDIRECTDRAWSURFACE7 pSurface, SI16 x, SI16 y, SI16 z);								// ��ũ�ѿ� ��ο�

	VOID					BuildAction();																					// �Ǽ� �׼�
	VOID					BuildDraw(LPDIRECTDRAWSURFACE7 pSurface);														// �Ǽ� ��ο�
	VOID					StoreAction();
	VOID					StorehouseDraw(LPDIRECTDRAWSURFACE7 pSurface);													// â�� ��ο�
	VOID					ItemMakeAction();																				// ������ ����� �׼�
	VOID					ItemMakeDraw(LPDIRECTDRAWSURFACE7 pSurface);													// ������ ����� ��ο�
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
