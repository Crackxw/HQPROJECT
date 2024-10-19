#ifndef		_SOPLANT_H_
#define		_SOPLANT_H_

#include	"OnlineProductStructure.h"
//#include	"Stack.h"
//#include	"OnlineCommonStruct.h"

struct	stFarmGroup;
struct	stFarmData;

struct	stFarmEquipment
{
	UI08			uiLevel;
	stEquipment		*pEquipment;
};


struct	stFarmStructure
{
	UI32			uiNum;				//	���� ������ ����
	UI32			uiFinalNum;			//	���� ��ǥ �����.(0�̸� ������ ���������� ����)
	SI08			siRate;				//	���� ���� (-30% ~ 30%)
	stFarmEquipment	Equipment[UPGRADE_CODE_NUM];

	VOID	Init()
	{
		uiNum		=	0;
		uiFinalNum	=	0;
		siRate		=	0;
		for(int i=0; i<UPGRADE_CODE_NUM; i++)
		{
			Equipment[i].uiLevel	=	0;
		}
	}

	SI32	GetRemainNum()
	{
		return (uiFinalNum - uiNum);
	}
};


struct stStorage						//	â��	-	�� ũ�� 44����Ʈ
{
	STORAGE_ITEM		item[ON_MAX_STORAGEITEM];

	VOID	Init()
	{
		ZeroMemory(item, sizeof(STORAGE_ITEM)*ON_MAX_STORAGEITEM);
	}

	UI08	Search(UI16	ItemCode)
	{
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiItemCode==ItemCode)
			{
				if(item[i].uiQuantity > 0 )			return i;
			}
		}
		return UI08_ERROR;
	}

	UI08	GetEmptySlot()
	{		
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiQuantity == 0 )	return i;
		}
		return UI08_ERROR;
	}

	UI08	GetStorageItemIndex(UI16 ItemCode)
	{
		UI08	uiReturn;
		uiReturn	=	Search(ItemCode);
		if( uiReturn != UI08_ERROR )	return uiReturn;
		return GetEmptySlot();
	}

	UI08	CheckItemExist(UI16 uiItemCode, UI16 uiITemQnt=1)
	{
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiItemCode == uiItemCode)
			{
				if(item[i].uiQuantity >= uiITemQnt)
					return i;
				else continue;
			}
		}
		return UI08_ERROR;
	}
};


struct	stFarmSlot
{
	SI32				nSlotIndex;
	BOOL				bWorkable;					//	�� �� �� �ִ°�?
	BOOL				bStore;						//	������ �� �ִ°�?
	UI08				uiLevel;					//  0~3 ���� ������ �ִ�.
	SI16				siPay;						//  ����.
	UI16				ProductItem;				//	�������� ������ �ڵ�
	UI32				uiRemainLabor;
	UI32				uiLabor;					//	���� �뵿��.
	stFacility			*pFacility;					//  NULL �̸� �ǹ��� �������� ����
	stFarmData			*pFarm;
	union
	{
		stFarmStructure		Structure;				//	����ǹ��� ���
		stStorage			Storage;				//	â���� ���
	};

	// �Լ�
	VOID	Init()
	{
		bWorkable		=	FALSE;
		uiLevel			=	0;
		siPay			=	0;
		uiLabor			=	0;
		uiRemainLabor	=	0;
		ProductItem		=	0;
		pFacility		=	NULL;
		Structure.Init();
		Storage.Init();
	}

	VOID	EndProduct()
	{
		if( uiLevel == 0 )
		{
			Init();
		}
		else
		{
			bWorkable		=	FALSE;
			siPay			=	0;
			uiLabor			=	0;
			uiRemainLabor	=	0;
			ProductItem		=	0;
			if( bStore == FALSE )
			{
				Structure.uiFinalNum=	0;
				Structure.uiNum		=	0;
			}
		}
	}

	VOID	LevelUp()
	{
		uiLevel++;
		bWorkable		=	FALSE;
		siPay			=	0;
		uiLabor			=	0;
		uiRemainLabor	=	0;
		ProductItem		=	0;
	}

	BOOL	bOnSchedule()
	{
		if( siPay > 0 )
		{
			if( uiRemainLabor > 0 )		return TRUE;
			else	siPay = 0;
		}
		return FALSE;
	}

	BOOL	bStorage()
	{
		if(pFacility)
		{
			if(bStore)	return TRUE;
		}
		return FALSE;
	}
/*
	stFarmEquipment*	GetEquip(UI32 EquipID)
	{
		if( bStore == FALSE )
		{
			if( pFacility )
			{
				if( uiEquipID	< UPGRADE_CODE_NUM)
				{
					pFacility->UpCode[uiEquipID]
				}
			}
		}
		return NULL;
	}*/
};

/*
struct	stFarmData					// 6���� �������� �� ����, �Ѹ����� ������ �ִ�. (�ϴ� 10���� �迭)
{
	SI32			nIndex;							//	���° �����ΰ�?
	SI32			siOwnerDBAcnt;					//	�������� : 0 �̸� ���� �� �ȸ� ����
	UI08			uiOwnerSlot;
	SStack<UI16>	UserManager;					//  �ϲ۵��� �����ϴ� ��ü
	SI32			siWorkDay;						//	�ֱ� ���� ���κ��� �� �� �����°�? , �����ð����� �������� ���� ó������
	OnFarmList		FarmInfo;						//  ������ ���� ����ü.(uiAvailNum,szFarmName,szOwner,Labor)
	stFarmGroup		*pGroup;
	stPlants		*pPlant;
	stFarmSlot		FarmSlot[ON_FARM_SLOT_NUM];		// ���� 6��


	stFarmData()
	{
		UserManager.Create(ON_MAX_PLANT_PLAYER_NUM);
	}

	VOID Init()
	{
		siOwnerDBAcnt		=	0;
		uiOwnerSlot			=	0;
		siWorkDay			=	0;
		FarmInfo.Init();
		FarmInfo.uiAvailSlot=	pPlant->uiInitSlot;
		UserManager.DeleteAll();
		for(int i=0; i<ON_FARM_SLOT_NUM; i++)
		{
			FarmSlot[i].Init();
			FarmSlot[i].nSlotIndex	=	i;
			FarmSlot[i].pFarm		=	this;
		}
	}

	~stFarmData()
	{
		UserManager.Release();
	}

	stFarmSlot	*GetFactory( SI32 siFactoryIndex )
	{
		if( siFactoryIndex < FarmInfo.uiAvailSlot && siFactoryIndex >= 0)
			return &FarmSlot[ siFactoryIndex ];
		
		return NULL;
	}

};

*/

/*
struct stFarmGroup
{
	stFarmData		Farm[ON_VILLAGE_FARM_NUM];		// �� ������ ��� �ǹ��� �ִ°�?
	stPlants		*pPlant;
	SOVillage		*pVillage;

	stFarmGroup(SOVillage *pV, stPlants *pP)
	{
		pPlant		=	pP;
		pVillage	=	pV;
		Init();
	}

	~stFarmGroup()
	{
	}

	VOID	Init()
	{
		for(int i=0; i<ON_VILLAGE_FARM_NUM; i++)
		{
			Farm[i].nIndex	=	i;
			Farm[i].pGroup	=	this;
			Farm[i].pPlant	=	pPlant;
			Farm[i].Init();
		}
	}

	stFarmData *GetFarm(SI32 Index)
	{
		if( Index < ON_VILLAGE_FARM_NUM && Index >= 0)
			return &Farm[Index];
		return NULL;
	}
};
*/

#endif	// _SOPLANT_H_