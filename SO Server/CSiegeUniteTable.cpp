#include "CSiegeUniteTable.h"

CSiegeUnitTable *CSiegeUnitTable::pInstance = NULL;

CSiegeUnitTable::CSiegeUnitTable()
{
}

CSiegeUnitTable::~CSiegeUnitTable()
{
	DelInstance();
}

void CSiegeUnitTable::DelInstance()
{
	if(pInstance != NULL)
		delete pInstance;

	pInstance = NULL;
}

CSiegeUnitTable *CSiegeUnitTable::GetInstance()
{
	if(pInstance == NULL)
		pInstance = new CSiegeUnitTable;

	return pInstance;
}

void CSiegeUnitTable::LoadFile(char *szFile)
{
	if(szFile == NULL) return;

	FILE *fp = fopen(szFile, "r"); //������ �д´�.
	if(fp == NULL) return;

	char szReadBuff[2048] = {0,};

	int readSize = (int)fread(szReadBuff, 1, 2048, fp);
	if(readSize > 0)
		szReadBuff[readSize] = '\0';

	fclose(fp);

	int i = -1;
	char acSeps[] = " \t\n";
	char* pcToken;
	pcToken = strtok(szReadBuff, acSeps);
	
	printf("CSiegeUnitTable���� ����");

	while(pcToken != NULL)
	{
		pcToken = strtok(NULL, acSeps);
		i = atoi(pcToken);                 //�ε���

		pcToken = strtok(NULL, acSeps);
		SetKindCode(i, atoi(pcToken));     // ĳ���� �ε���

		pcToken = strtok(NULL, acSeps);
		SetPopulation(i, atoi(pcToken));   // �ʿ��α�

		pcToken = strtok(NULL, acSeps);
		SetPrice(i, atoi(pcToken));        // ���� ���
	
		pcToken = strtok(NULL, acSeps);
		SetLevel(i, atoi(pcToken));        // ����

		pcToken = strtok(NULL, acSeps);
		SetDamage(i, atoi(pcToken));       // ������
		
		pcToken = strtok(NULL, acSeps);
		SetBasicHP(i, atoi(pcToken));      // hp
		
		pcToken = strtok(NULL, acSeps);
		SetSpeed(i, atoi(pcToken));	       // �̵��ӵ�		

		pcToken = strtok(NULL, acSeps);
		SetDelay(i, atoi(pcToken));        // ������ �ð�

		pcToken = strtok(NULL, acSeps);
		SetMaxAttackDis(i, atoi(pcToken)); // �ִ� ���� �Ÿ�

		pcToken = strtok(NULL, acSeps);
		SetMinAttackDis(i, atoi(pcToken)); // �ּ� ���� �Ÿ�			
		
		pcToken = strtok(NULL, acSeps);
		SetChangePrice(i, atoi(pcToken));  // ���� ���
		
		pcToken = strtok(NULL, acSeps);    
		SetBasicMP(i, atoi(pcToken));      // �⺻ ������

		pcToken = strtok(NULL, acSeps);
	}

	printf("CSiegeUnitTable���� ��");
}

//-----------------------------------------------------------------
void CSiegeUnitTable::Init()
{
	//�޸𸮸� �ʱ�ȭ �Ѵ�.
	memset(m_pSiegeUnit, 0, sizeof(SIEGE_UNIT_TABLE) * BATTLE_MAX);

	//������ �д´�.
	LoadFile("BattleUnit.txt");
}

int CSiegeUnitTable::SetDamage(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiDamage = nValue;
	return m_pSiegeUnit[nUnitCode].uiDamage;
}

int CSiegeUnitTable::GetDamage(int nUnitCode)       //������
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiDamage;
}

int CSiegeUnitTable::SetSpeed(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiSpeed = nValue;

	return m_pSiegeUnit[nUnitCode].uiSpeed;
}

int CSiegeUnitTable::GetSpeed(int nUnitCode)         //�ӵ� 
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiSpeed;
}

int CSiegeUnitTable::SetLevel(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiLevel = nValue;
	return m_pSiegeUnit[nUnitCode].uiLevel;
}

int CSiegeUnitTable::GetLevel(int nUnitCode)         //����
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiLevel;
}

int CSiegeUnitTable::SetDelay(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiDelay = nValue;
	return m_pSiegeUnit[nUnitCode].uiDelay;
}

int CSiegeUnitTable::GetDelay(int nUnitCode)         //������ �ð� 
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiDelay;
}

int CSiegeUnitTable::SetMaxAttackDis(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiMaxDis = nValue;
	return m_pSiegeUnit[nUnitCode].uiMaxDis;
}

int CSiegeUnitTable::GetMaxAttackDis(int nUnitCode)  //�ִ� �����Ÿ�
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiMaxDis;
}

int CSiegeUnitTable::SetMinAttackDis(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiMinDis = nValue;
	return m_pSiegeUnit[nUnitCode].uiMinDis;
}

int CSiegeUnitTable::GetMinAttackDis(int nUnitCode)  //�ּ� 
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiMinDis;
}

int CSiegeUnitTable::SetPrice(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiPrice = nValue;
	return m_pSiegeUnit[nUnitCode].uiPrice;
}

int CSiegeUnitTable::GetPrice(int nUnitCode)         //���� ���
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiPrice;
}

int CSiegeUnitTable::SetPopulation(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiPopulation = nValue;
	return m_pSiegeUnit[nUnitCode].uiPopulation;
}

int CSiegeUnitTable::GetPopulation(int nUnitCode)    //���� �α�
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiPopulation;
}

int CSiegeUnitTable::SetBasicHP(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	m_pSiegeUnit[nUnitCode].uiHP = nValue;

	return m_pSiegeUnit[nUnitCode].uiHP;
}

int CSiegeUnitTable::GetBasicHP(int nUnitCode)       //�⺻ ü��
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiHP;
}

int CSiegeUnitTable::SetBasicMP(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;

	m_pSiegeUnit[nUnitCode].uiMP = nValue;
	return m_pSiegeUnit[nUnitCode].uiMP;
}

int CSiegeUnitTable::GetBasicMP(int nUnitCode)       //�⺻ ����
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiMP;
}

int CSiegeUnitTable::GetHP(int nUnitCode, int nBP)   //�����¿� ���� ü��
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nBP < 0) return -1;
	
	int nTemp = 0;
	int nHP = GetBasicHP(nUnitCode);

	if(nBP >= 0 && nBP <= 70000)            nTemp = 100;
    else if(nBP >= 70001  && nBP <= 140000) nTemp = 200;
	else if(nBP >= 140001 && nBP <= 210000) nTemp = 300;
	else if(nBP >= 210001 && nBP <= 280000) nTemp = 400;
	else if(nBP >= 280001 )                 nTemp = 500;

	return nHP + nTemp;
}

int CSiegeUnitTable::GetMP(int nUnitCode, int nBP)   //�����¿� ���� ����
{
	if(nUnitCode != BATTLE_SUPPLY) return -1;
	if(nBP < 0) return -1;

	int nTemp = 0;
	int nMP = GetBasicMP(nUnitCode);

	if(nBP >= 0 && nBP <= 70000)            nTemp = 100;
	else if(nBP >= 70001  && nBP <= 140000) nTemp = 200;
	else if(nBP >= 140001 && nBP <= 210000) nTemp = 300;
	else if(nBP >= 210001 && nBP <= 280000) nTemp = 400;
	else if(nBP >= 280001 )                 nTemp = 500;

	return nMP + nTemp;
}

int CSiegeUnitTable::SetChangePrice(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiMoney = nValue;
	return m_pSiegeUnit[nUnitCode].uiMoney;
}

int CSiegeUnitTable::GetChangePrice(int nUnitCode)   //���� ���
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiMoney;
}

int CSiegeUnitTable::SetKindCode(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	m_pSiegeUnit[nUnitCode].uiKind = nValue;
	return m_pSiegeUnit[nUnitCode].uiKind;
}

int CSiegeUnitTable::GetKindCode(int nUnitCode)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiKind;
}

int CSiegeUnitTable::GetKindUnitCode(int nKind)
{
	if(nKind < 0) return -1;

	int i = 0;
	for(; i < BATTLE_MAX; i++)
	{
		if(m_pSiegeUnit[i].uiKind == (signed)nKind)
		{
			return i;
		}
	}

	return -1;
}

BOOL CSiegeUnitTable::IsFieldAttackUnit(int nKind)
{
	if(nKind < 0) return FALSE;

	for(int i = 0; i < BATTLE_MAX; i++)
	{
		if(i == 4) continue;
		if(m_pSiegeUnit[i].uiKind == (signed)nKind) return TRUE;
	}

	return FALSE;
}