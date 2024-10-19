

#ifndef	__ONLINEPRODUCTSTRUCTURE_H_
#define	__ONLINEPRODUCTSTRUCTURE_H_

#include "GSCDefine.h"

struct	stEquipment
{
	union
	{
		CHAR			Code[4];						//	�ǹ��ڵ� (FNC:��Ÿ��)
		UI32			uiNumCode;						//	�ڵ带 ���ڷ� �Ѱ�.
	};
	SI32			siTextCode;						//  �ؽ�Ʈ
	UI32			uiCost;							//	����
	UI08			Repeat;							//	���׷��̵� �ݺ� ��
	UI08			uiResist[RESIST_KIND_NUM];		//	���ؿ� ���� ��ȣ����
};

struct stFacility
{
	union
	{
		CHAR			Code[4];						//	�ڵ� (STG:â��, FRU:������)
		UI32			uiNumCode;						//	�ڵ带 ���ڷ� ��ȯ�� ��
	};
	CHAR			Name[8];
	SI32			siTextCode;						//  �ؽ�Ʈ
	SI16			siSpriteFont;					//  ��������Ʈ ��Ʈ
	// �����ϴ� ��ǰ ������ ��ȣ
	UI16			uiPr[4][PRODUCT_CODE_NUM];			//	������ �� �ִ� ������ �ڵ� (00011)
	//	���׷��̵� �ϴ� ����,�ü��� �� �ڵ�
	SI16			siUpCodeNum;					//  ��ġ�� �� �ִ� �ü� ����
	union
	{
		CHAR			UpCode[UPGRADE_CODE_NUM][4];	//	��ġ�� �� �ִ� �ü� �ڵ� (FNC:��Ÿ��)
		UI32			UpNumCode[UPGRADE_CODE_NUM];	//	���ڷ�.
	};
	UI32			UpLabor[MAX_UPGRADESLOT_NUM];
};

struct stPlants
{
	SI32				DBIndex;
	union
	{
		CHAR			Code[4];						//	�ǹ��ڵ� (FAM:����)
		UI32			uiNumCode;						//	�ڵ带 ���ڷ� ��ȯ�� ��
	};
	UI32			uiCost;							//	����
	SI32			siTextCode;						//	Text
	UI08			uiMaxSlot;						//	�ִ뽽�Լ� (6)
	UI08			uiInitSlot;						//	�ʱ⽽�Լ� (4)
	SI16			siBuildNum;						//  ������ �ִ� �ǹ� ��
	union
	{
		CHAR			FCode[FACILITY_CODE_NUM][4];	//	������ �ִ� �ǹ� (STG:â��)(FRU:������)
		UI32			FNumCode[FACILITY_CODE_NUM];	//	������ ���ڷ� ��ȯ.
	};
	UI32			Up2Labor;
	UI32			Up3Labor;
	SI32			siReqCredit;					//	�ʿ� �ſ뵵
};

 
class	OnlineBuilding
{
private:
	stEquipment			*pEquipment;
	stFacility			*pFacility;
	stPlants			*pPlants;
	SI32				EquipNum;		//	����
	SI32				FacilNum;
	SI32				PlantsNum;

public:
	OnlineBuilding();
	~OnlineBuilding();

	stPlants		*GetPlantsFromID( UI08 uiPlantID );	//0:����, ...

	stEquipment		*GetEquipment( SI32 siIndex );
	stFacility		*GetFacility( SI32 siIndex );
	stPlants		*GetPlants( SI32 siIndex );

	stEquipment		*GetEquipment( CHAR	*pCode );
	stFacility		*GetFacility( CHAR *pCode);
	stPlants		*GetPlants( CHAR *pCode);

	SI32			GetIndex(CHAR *FaCode, UI16	uiItem, UI08 Level=3, UI08 uiNation = ON_COUNTRY_KOREA);
	UI32			CodeToNum(CHAR	*pCode);
	BOOL			Init();

	SI32			GetEquipNum()			{	return EquipNum;	};
	SI32			GetFacilNum()			{	return FacilNum;	};
	SI32			GetPlantsNum()			{	return PlantsNum;	};
};

#endif

