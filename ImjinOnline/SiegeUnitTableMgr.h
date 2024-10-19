// 공성전 유닛 정보 테이블 관리자
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
	char		m_szAnimationFilename[128];	// 애니메이션 파일명
	SI32		m_siAnimationDelay;			// 애니메이션 딜레이
	SI32		m_siAniamtionStartFont;		// 애니메이션 시작 폰트 번호
	SI32		m_siAniamtionFontNumber;	// 애니메이션 폰트 수
};

// 전투력에 따른 체력, 마력 보너스 수치
struct SSiegeUnitCalcTable
{
	SI32		m_siMinBattlePower;		// 최소 전투력
	SI32		m_siMaxBattlePower;		// 최대 전투력
	SI32		m_siIncreaseValue;		// 상승치
};

// 공성전 유닛 정보 구조체
typedef struct _SSiegeUnitTable
{	
	SI08	m_siID;										// 소집권 종류. 배열 인덱스
	UI16	m_uiKind;									// 변하는 캐릭터 아이디.
	SI32	m_siUnitNameTextIdx;						// 공성전 유닛 이름 텍스트 인덱스
	SI32	m_siArmyKindDescTextID;						// 해당 공성전 유닛 설명 텍스트 ID
	SI16	m_siPopulation;								// 소집권 구매시 사용되는 인구수.
	SI16	m_siPrice;									// 인구수 1명당 소집권 만드는데 필요한 비용.
														// 실제 소집권 가격은 m_siPopulation * m_siPrice 이다.
	SI08	m_scTransformOption;						// robypark 2004/11/10 14:13 - 해당 소집권에 명시된 캐릭터로 변신 가능에 대한 옵션으로 OR(|) 연산된 값이다.
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE(1)				// 명시된 캐릭터로 변신 가능
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL(2)	// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
														// ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE(4)		// 이 공성전 유닛은 마을 수성인터페이스를 이용할 수 있다.

	// robypark 2005/1/24 17:44
	SI08	m_scTransformLevelLimit;					// 해당 공성전 유닛으로 변신하기 위한 레벨 제한
	SI32	m_siTransformCost;							// 해당 공성전 유닛으로 변신하기 위한 금액

	SI32	m_siBasicHP;								// 기본 체력
	SI32	m_siBasicMP;								// 기본 마력


	// robypark 2005/1/27 11:1
	SI32	m_siAnimationResourceIdx_Blue;				// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	SI32	m_siAnimationResourceIdx_Red;				// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때

	// 변신 인터페이스(애니메이션 정보 0 = wait, 1 = attack)
	SAniamtionInfo	m_sAniamtionInfo[2];

	// robypark 2005/2/15 16:32
	SI08	m_scMaxMoveActionSkip;						// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과

	char	m_szPortraitFileName[256];					// 초상화 이미지 파일명

} SSiegeUnitTable, *LPSSiegeUnitTable;

class CSiegeUnitTableMgr  
{
	public:
		CSiegeUnitTableMgr();
		virtual ~CSiegeUnitTableMgr();

	private:
		vector<SSiegeUnitTable>	m_vecSiegeUnitTableMgr;					// 공성유닛 정보 리스트

		vector<SSiegeUnitCalcTable>	m_vecSiegeUnitCalcHPTableMgr;		// 전투력에 따른 체력 상승 값 리스트

		vector<SSiegeUnitCalcTable>	m_vecSiegeUnitCalcMPTableMgr;		// 전투력에 따른 마력 상승 값 리스트

	public:
		// 메모리 해제
		void			Free(void);

		// HQ의 SiegeUnitTable.txt 파일 읽기
		BOOL			LoadSiegeUnitTable(void);

		// HQ의 SiegeUnitTableCalcHP.txt 파일 읽기
		BOOL			LoadSiegeUnitTableCalcHP(void);

		// HQ의 SiegeUnitTableCalcMP.txt 파일 읽기
		BOOL			LoadSiegeUnitTableCalcMP(void);

		// 캐릭터 Online Kind에 맞는 LPSSiegeUnitTable얻기
		LPSSiegeUnitTable	GetSiegeUnitTable(SI32 siOnlineCharKind);

		// 배열 인덱스에 맞는 LPSSiegeUnitTable얻기
		LPSSiegeUnitTable	GetSiegeUnitTableByArrayIdx(SI32 siArrayIdx);

		// 전투력에 따른 체력 상승치 얻기
		SI32			GetIncreaseHP(SI32 siBattlePower);

		// 전투력에 따른 마력 상승치 얻기
		SI32			GetIncreaseMP(SI32 siBattlePower);

		// 캐릭터 정보 개수 얻기
		SI32			GetCount(void);
};

#endif // !defined(AFX_SIEGEUNITTABLEMGR_H__8B023FB6_5D29_461E_B1A2_D86A0B3C9235__INCLUDED_)
