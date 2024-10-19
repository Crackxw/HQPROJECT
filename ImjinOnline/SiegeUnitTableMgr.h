// ������ ���� ���� ���̺� ������
//
// SiegeUnitTableMgr.h: interface for the CSiegeUnitTableMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEUNITTABLEMGR_H__8B023FB6_5D29_461E_B1A2_D86A0B3C9235__INCLUDED_)
#define AFX_SIEGEUNITTABLEMGR_H__8B023FB6_5D29_461E_B1A2_D86A0B3C9235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning ( disable : 4786 )

#include	<GSL.h>
#include	"GSCDefine.h"
#include	<DDraw.h>
#include	"button.h"
#include	<graphics16.h>

class cltOnlineWorld;

#include	<vector>

using namespace std;

#define ANIBTN_WAIT				0
#define ANIBTN_MOVE			1

struct SAniamtionInfo
{
	char		m_szAnimationFilename[128];	// �ִϸ��̼� ���ϸ�
	SI32		m_siAnimationDelay;			// �ִϸ��̼� ������
	SI32		m_siAniamtionStartFont;		// �ִϸ��̼� ���� ��Ʈ ��ȣ
	SI32		m_siAniamtionFontNumber;	// �ִϸ��̼� ��Ʈ ��
};

// �����¿� ���� ü��, ���� ���ʽ� ��ġ
struct SSiegeUnitCalcTable
{
	SI32		m_siMinBattlePower;		// �ּ� ������
	SI32		m_siMaxBattlePower;		// �ִ� ������
	SI32		m_siIncreaseValue;		// ���ġ
};

// ������ ���� ���� ����ü
typedef struct _SSiegeUnitTable
{	
	SI08	m_siID;										// ������ ����. �迭 �ε���
	UI16	m_uiKind;									// ���ϴ� ĳ���� ���̵�.
	SI32	m_siUnitNameTextIdx;						// ������ ���� �̸� �ؽ�Ʈ �ε���
	SI32	m_siArmyKindDescTextID;						// �ش� ������ ���� ���� �ؽ�Ʈ ID
	SI16	m_siPopulation;								// ������ ���Ž� ���Ǵ� �α���.
	SI16	m_siPrice;									// �α��� 1��� ������ ����µ� �ʿ��� ���.
														// ���� ������ ������ m_siPopulation * m_siPrice �̴�.
	SI08	m_scTransformOption;						// robypark 2004/11/10 14:13 - �ش� �����ǿ� ��õ� ĳ���ͷ� ���� ���ɿ� ���� �ɼ����� OR(|) ����� ���̴�.
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE(1)				// ��õ� ĳ���ͷ� ���� ����
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL(2)	// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
														// ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE(4)		// �� ������ ������ ���� �����������̽��� �̿��� �� �ִ�.

	// robypark 2005/1/24 17:44
	SI08	m_scTransformLevelLimit;					// �ش� ������ �������� �����ϱ� ���� ���� ����
	SI32	m_siTransformCost;							// �ش� ������ �������� �����ϱ� ���� �ݾ�

	SI32	m_siBasicHP;								// �⺻ ü��
	SI32	m_siBasicMP;								// �⺻ ����


	// robypark 2005/1/27 11:1
	SI32	m_siAnimationResourceIdx_Blue;				// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	SI32	m_siAnimationResourceIdx_Red;				// �ش� ������ ���� �̹��� ����. ���� ������ ��

	// ���� �������̽�(�ִϸ��̼� ���� 0 = wait, 1 = attack)
	SAniamtionInfo	m_sAniamtionInfo[2];

	// robypark 2005/2/15 16:32
	SI08	m_scMaxMoveActionSkip;						// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��

	char	m_szPortraitFileName[256];					// �ʻ�ȭ �̹��� ���ϸ�

} SSiegeUnitTable, *LPSSiegeUnitTable;

class CSiegeUnitTableMgr  
{
	public:
		CSiegeUnitTableMgr();
		virtual ~CSiegeUnitTableMgr();

	private:
		vector<SSiegeUnitTable>	m_vecSiegeUnitTableMgr;					// �������� ���� ����Ʈ

		vector<SSiegeUnitCalcTable>	m_vecSiegeUnitCalcHPTableMgr;		// �����¿� ���� ü�� ��� �� ����Ʈ

		vector<SSiegeUnitCalcTable>	m_vecSiegeUnitCalcMPTableMgr;		// �����¿� ���� ���� ��� �� ����Ʈ

	public:
		// �޸� ����
		void			Free(void);

		// HQ�� SiegeUnitTable.txt ���� �б�
		BOOL			LoadSiegeUnitTable(void);

		// HQ�� SiegeUnitTableCalcHP.txt ���� �б�
		BOOL			LoadSiegeUnitTableCalcHP(void);

		// HQ�� SiegeUnitTableCalcMP.txt ���� �б�
		BOOL			LoadSiegeUnitTableCalcMP(void);

		// ĳ���� Online Kind�� �´� LPSSiegeUnitTable���
		LPSSiegeUnitTable	GetSiegeUnitTable(SI32 siOnlineCharKind);

		// �迭 �ε����� �´� LPSSiegeUnitTable���
		LPSSiegeUnitTable	GetSiegeUnitTableByArrayIdx(SI32 siArrayIdx);

		// �����¿� ���� ü�� ���ġ ���
		SI32			GetIncreaseHP(SI32 siBattlePower);

		// �����¿� ���� ���� ���ġ ���
		SI32			GetIncreaseMP(SI32 siBattlePower);

		// ĳ���� ���� ���� ���
		SI32			GetCount(void);
};

#endif // !defined(AFX_SIEGEUNITTABLEMGR_H__8B023FB6_5D29_461E_B1A2_D86A0B3C9235__INCLUDED_)
