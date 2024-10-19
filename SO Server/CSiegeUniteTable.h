#ifndef __CSIEGEUNITETABLE_H__
#define __CSIEGEUNITETABLE_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tag_Unite_table
{
	
	int     uiSpeed;       //이동 속도
	int     uiDelay;       //공격 딜레이
	int     uiMinDis;      //최소 사정거리
	int     uiMaxDis;      //최대 사정거리
	int     uiDamage;      //데미지
	int     uiLevel;       //변신 가능 레벨
	int     uiPrice;       //모집 비용
	int     uiPopulation;  //소모 인구	
    int     uiHP;          //체력
	int     uiMP;          //군수물자.
	int     uiKind;        //캐릭터 리스트의 인덱스번호
	int     uiMoney;       //변신 비용

}SIEGE_UNIT_TABLE, *LP_SIEGE_UNIT_TABLE;

enum
{
	BATTLE_SWORD = 0,	 //검병
	BATTLE_SPEAR,	     //창병
	BATTLE_HORSE,	     //기마병
	BATTLE_ARROW,	     //궁병
	BATTLE_DEFENCE,      //수비병 사용X
	BATTLE_RAMPAR,       //충차
	BATTLE_SIEGE,        //발석거
	BATTLE_SUPPLY,       //보급수레

	BATTLE_MAX,
};

class CSiegeUnitTable
{
protected:
	static CSiegeUnitTable     *pInstance;
	SIEGE_UNIT_TABLE            m_pSiegeUnit[BATTLE_MAX];

	void                        DelInstance();

public:
	CSiegeUnitTable();
	~CSiegeUnitTable();

	static CSiegeUnitTable            *GetInstance();


	void                        Init();
	void                        LoadFile(char *szFile);

	int                         SetDamage(int nUnitCode, int nValue);
	int                         GetDamage(int nUnitCode);       //데미지
	
	int                         SetSpeed(int nUnitCode, int nValue);
	int                         GetSpeed(int nUnitCode);         //속도 
	
	int                         SetLevel(int nUnitCode, int nValue);
	int                         GetLevel(int nUnitCode);         //레벨

	int                         SetDelay(int nUnitCode, int nValue);
	int                         GetDelay(int nUnitCode);         //딜레이 시간 
	
	int                         SetMaxAttackDis(int nUnitCode, int nValue);
	int                         GetMaxAttackDis(int nUnitCode);  //최대 사정거리
	
	int                         SetMinAttackDis(int nUnitCode, int nValue);
	int                         GetMinAttackDis(int nUnitCode);  //최소 
	
	int                         SetPrice(int nUnitCode, int nValue);
	int                         GetPrice(int nUnitCode);         //모집 비용

	int                         SetPopulation(int nUnitCode, int nValue);
	int                         GetPopulation(int nUnitCode);    //모집 인구

	int                         SetBasicHP(int nUnitCode, int nValue);
	int                         GetBasicHP(int nUnitCode);       //기본 체력

	int                         SetBasicMP(int nUnitCode, int nValue);
	int                         GetBasicMP(int nUnitCode);       //기본 마나

	int                         GetHP(int nUnitCode, int nBP);   //전투력에 따른 체력
	int                         GetMP(int nUnitCode, int nBP);   //전투력에 따른 마나

	int                         SetChangePrice(int nUnitCode, int nValue);
	int                         GetChangePrice(int nUnitCode);   //변신 비용

	int                         SetKindCode(int nUnitCode, int nValue); //캐릭터 인덱스를 세팅한다.
	int                         GetKindCode(int nUnitCode);             //캐릭터 인덱스를 가져온다.

	int                         GetKindUnitCode(int nKind);  //캐릭터 인덱스로 인덱스를 구한다.
	
	BOOL                        IsFieldAttackUnit(int nKind); //공성용 유닛인가?
};

#endif
