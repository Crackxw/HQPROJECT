#ifndef __CSIEGEUNITETABLE_H__
#define __CSIEGEUNITETABLE_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tag_Unite_table
{
	
	int     uiSpeed;       //�̵� �ӵ�
	int     uiDelay;       //���� ������
	int     uiMinDis;      //�ּ� �����Ÿ�
	int     uiMaxDis;      //�ִ� �����Ÿ�
	int     uiDamage;      //������
	int     uiLevel;       //���� ���� ����
	int     uiPrice;       //���� ���
	int     uiPopulation;  //�Ҹ� �α�	
    int     uiHP;          //ü��
	int     uiMP;          //��������.
	int     uiKind;        //ĳ���� ����Ʈ�� �ε�����ȣ
	int     uiMoney;       //���� ���

}SIEGE_UNIT_TABLE, *LP_SIEGE_UNIT_TABLE;

enum
{
	BATTLE_SWORD = 0,	 //�˺�
	BATTLE_SPEAR,	     //â��
	BATTLE_HORSE,	     //�⸶��
	BATTLE_ARROW,	     //�ú�
	BATTLE_DEFENCE,      //���� ���X
	BATTLE_RAMPAR,       //����
	BATTLE_SIEGE,        //�߼���
	BATTLE_SUPPLY,       //���޼���

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
	int                         GetDamage(int nUnitCode);       //������
	
	int                         SetSpeed(int nUnitCode, int nValue);
	int                         GetSpeed(int nUnitCode);         //�ӵ� 
	
	int                         SetLevel(int nUnitCode, int nValue);
	int                         GetLevel(int nUnitCode);         //����

	int                         SetDelay(int nUnitCode, int nValue);
	int                         GetDelay(int nUnitCode);         //������ �ð� 
	
	int                         SetMaxAttackDis(int nUnitCode, int nValue);
	int                         GetMaxAttackDis(int nUnitCode);  //�ִ� �����Ÿ�
	
	int                         SetMinAttackDis(int nUnitCode, int nValue);
	int                         GetMinAttackDis(int nUnitCode);  //�ּ� 
	
	int                         SetPrice(int nUnitCode, int nValue);
	int                         GetPrice(int nUnitCode);         //���� ���

	int                         SetPopulation(int nUnitCode, int nValue);
	int                         GetPopulation(int nUnitCode);    //���� �α�

	int                         SetBasicHP(int nUnitCode, int nValue);
	int                         GetBasicHP(int nUnitCode);       //�⺻ ü��

	int                         SetBasicMP(int nUnitCode, int nValue);
	int                         GetBasicMP(int nUnitCode);       //�⺻ ����

	int                         GetHP(int nUnitCode, int nBP);   //�����¿� ���� ü��
	int                         GetMP(int nUnitCode, int nBP);   //�����¿� ���� ����

	int                         SetChangePrice(int nUnitCode, int nValue);
	int                         GetChangePrice(int nUnitCode);   //���� ���

	int                         SetKindCode(int nUnitCode, int nValue); //ĳ���� �ε����� �����Ѵ�.
	int                         GetKindCode(int nUnitCode);             //ĳ���� �ε����� �����´�.

	int                         GetKindUnitCode(int nKind);  //ĳ���� �ε����� �ε����� ���Ѵ�.
	
	BOOL                        IsFieldAttackUnit(int nKind); //������ �����ΰ�?
};

#endif
