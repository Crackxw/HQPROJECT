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

	FILE *fp = fopen(szFile, "r"); //파일을 읽는다.
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
	
	printf("CSiegeUnitTable설정 시작");

	while(pcToken != NULL)
	{
		pcToken = strtok(NULL, acSeps);
		i = atoi(pcToken);                 //인덱스

		pcToken = strtok(NULL, acSeps);
		SetKindCode(i, atoi(pcToken));     // 캐릭터 인덱스

		pcToken = strtok(NULL, acSeps);
		SetPopulation(i, atoi(pcToken));   // 필요인구

		pcToken = strtok(NULL, acSeps);
		SetPrice(i, atoi(pcToken));        // 모집 비용
	
		pcToken = strtok(NULL, acSeps);
		SetLevel(i, atoi(pcToken));        // 레벨

		pcToken = strtok(NULL, acSeps);
		SetDamage(i, atoi(pcToken));       // 데미지
		
		pcToken = strtok(NULL, acSeps);
		SetBasicHP(i, atoi(pcToken));      // hp
		
		pcToken = strtok(NULL, acSeps);
		SetSpeed(i, atoi(pcToken));	       // 이동속도		

		pcToken = strtok(NULL, acSeps);
		SetDelay(i, atoi(pcToken));        // 딜레이 시간

		pcToken = strtok(NULL, acSeps);
		SetMaxAttackDis(i, atoi(pcToken)); // 최대 공격 거리

		pcToken = strtok(NULL, acSeps);
		SetMinAttackDis(i, atoi(pcToken)); // 최소 공격 거리			
		
		pcToken = strtok(NULL, acSeps);
		SetChangePrice(i, atoi(pcToken));  // 변신 비용
		
		pcToken = strtok(NULL, acSeps);    
		SetBasicMP(i, atoi(pcToken));      // 기본 마나량

		pcToken = strtok(NULL, acSeps);
	}

	printf("CSiegeUnitTable설정 끝");
}

//-----------------------------------------------------------------
void CSiegeUnitTable::Init()
{
	//메모리를 초기화 한다.
	memset(m_pSiegeUnit, 0, sizeof(SIEGE_UNIT_TABLE) * BATTLE_MAX);

	//파일을 읽는다.
	LoadFile("BattleUnit.txt");
}

int CSiegeUnitTable::SetDamage(int nUnitCode, int nValue)
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	if(nValue < 0) return -1;

	m_pSiegeUnit[nUnitCode].uiDamage = nValue;
	return m_pSiegeUnit[nUnitCode].uiDamage;
}

int CSiegeUnitTable::GetDamage(int nUnitCode)       //데미지
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

int CSiegeUnitTable::GetSpeed(int nUnitCode)         //속도 
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

int CSiegeUnitTable::GetLevel(int nUnitCode)         //레벨
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

int CSiegeUnitTable::GetDelay(int nUnitCode)         //딜레이 시간 
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

int CSiegeUnitTable::GetMaxAttackDis(int nUnitCode)  //최대 사정거리
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

int CSiegeUnitTable::GetMinAttackDis(int nUnitCode)  //최소 
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

int CSiegeUnitTable::GetPrice(int nUnitCode)         //모집 비용
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

int CSiegeUnitTable::GetPopulation(int nUnitCode)    //모집 인구
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

int CSiegeUnitTable::GetBasicHP(int nUnitCode)       //기본 체력
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

int CSiegeUnitTable::GetBasicMP(int nUnitCode)       //기본 마나
{
	if(nUnitCode < 0 || nUnitCode >= BATTLE_MAX) return -1;
	return m_pSiegeUnit[nUnitCode].uiMP;
}

int CSiegeUnitTable::GetHP(int nUnitCode, int nBP)   //전투력에 따른 체력
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

int CSiegeUnitTable::GetMP(int nUnitCode, int nBP)   //전투력에 따른 마나
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

int CSiegeUnitTable::GetChangePrice(int nUnitCode)   //변신 비용
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