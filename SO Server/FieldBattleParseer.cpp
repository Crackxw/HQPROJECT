#include <stdio.h>
#include "FieldBattleParser.h"
#include "stdlib.h"
#include "Defines.h"
#include "gscProtocol.h"

CFieldBattleParser::CFieldBattleParser()
{

}

CFieldBattleParser::~CFieldBattleParser()
{

}

void CFieldBattleParser::Init()
{
	
}

void CFieldBattleParser::Load(char* szFileName)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = "\t";
	CHAR	*pToken;
	int		i	=	0;

	ZeroMemory(&m_stFBUnit,sizeof(strFieldBattleUnit) * FIELD_BATTLE_ATTACK_UNIT_NUM);

	if((fp = fopen(szFileName, "r")) != NULL)
	{
		fgets(szReadBuffer, 1023, fp);
		fgets(szReadBuffer, 1023, fp);

		while(fgets(szReadBuffer, 1023, fp) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			pToken =	strtok(NULL, cSep);
			m_stFBUnit[i].uiKind		= atoi(pToken);
			pToken =    strtok(NULL, cSep);
			if(atoi(pToken) == 1)
				m_stFBUnit[i].bBattleUnit = TRUE;
			else
				m_stFBUnit[i].bBattleUnit = FALSE;
			pToken =	strtok(NULL, cSep);
			m_stFBUnit[i].uiItemID		= atoi(pToken);
			pToken =	strtok(NULL, cSep);
			m_stFBUnit[i].siBaseDamage	= atoi(pToken);

			for(int j = 0 ; j < FIELD_BATTLE_DEFENSE_UNIT_NUM ; j++)
			{
				pToken =	strtok(NULL, cSep);
				m_stFBUnit[i].stFBPer[j].siPercent = atoi(pToken);
			}
			i++;
		}
	}

	fclose(fp);
}

short CFieldBattleParser::GetCalcDamage(short siAttackUnit,short siDefenseUnit,short siAttackType)
{
	float siDamage;
	float siPercent;

	if(siAttackType == ON_ATTACK_TYPE_NEARCAPTAIN)
	{
		siDamage = (float)m_stFBUnit[siAttackUnit].siBaseDamage/5;
		siDamage += m_stFBUnit[siAttackUnit].siBaseDamage;
	}
	else if(siAttackType == ON_ATTACK_TYPE_CAPTAIN)
	{
		siDamage = (float)m_stFBUnit[siAttackUnit].siBaseDamage*2;
	}
	else
	{
		siDamage = m_stFBUnit[siAttackUnit].siBaseDamage;
	}

	if(siAttackUnit == -1 || siDefenseUnit == -1)		return 0;

	siPercent = m_stFBUnit[siAttackUnit].stFBPer[siDefenseUnit].siPercent;

	if(siPercent != 0)
	{
		siPercent = siPercent/100;

		if(siPercent != 0)
		{
			siDamage = siPercent * siDamage;
			return (short)(siDamage + 0.5);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
/*
short CFieldBattleParser::GetCalcDamageByDefenceSoldier(short siDefenceSoldier,short siDefenseUnit,short siAttackType)
{
	float fDamage;
	float siPercent;// = m_stFBUnit[FIELD_BATTLE_DEFENSE].stFBPer[siDefenseUnit].siPercent;
	short siDamage;

	siPercent /= 100;

 	fDamage  = (float)(m_stFBUnit[FIELD_BATTLE_DEFENSE].siBaseDamage * siPercent);
	siDamage = (short)(fDamage + 0.5);

	if(ON_VILLAGE_ATTACK_OIL == siAttackType)
	{
		if(siDefenseUnit == FIELD_BATTLE_SWORD)
			siDamage =  siDamage * siDefenceSoldier * 3;
		else 
			siDamage =  siDamage * siDefenceSoldier;
	}
	else if(ON_VILLAGE_ATTACK_STONE == siAttackType)
	{
		if(siDefenseUnit == FIELD_BATTLE_RAMPART)
			siDamage = siDamage * siDefenceSoldier * 3;
		else
			siDamage = siDamage * siDefenceSoldier;
	}
	else	// 방어력 향상이나 아무것도 없는 경우.
	{
		siDamage = siDamage * siDefenceSoldier;
	}

	return siDamage;
}
*/
short CFieldBattleParser::GetUnitKind(unsigned short uiAUnitKind)
{
	for(short i = 0 ; i < FIELD_BATTLE_ATTACK_UNIT_NUM ;i++)
	{
		if(m_stFBUnit[i].uiKind == uiAUnitKind)
			return i;
	}
	
	return -1;
}

BOOL CFieldBattleParser::IsFieldAttackUnit(unsigned short uiKind)
{
	for(int i = 0 ; i < FIELD_BATTLE_ATTACK_UNIT_NUM ; i++)
	{
		if(m_stFBUnit[i].bBattleUnit == TRUE)
			if(m_stFBUnit[i].uiKind == uiKind)
				return TRUE;
	}

	return FALSE;
}

unsigned short CFieldBattleParser::GetItemID(short slotID)
{
	for(int i = 0 ; i < FIELD_BATTLE_ATTACK_UNIT_NUM ; i++)
	{
		if(i == slotID)	
		{
			return m_stFBUnit[i].uiItemID;
		}
	}
	return -1;
}

CAttackPowerParser::CAttackPowerParser()
{

}

CAttackPowerParser::~CAttackPowerParser()
{

}

void CAttackPowerParser::Load(char* szFileName)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = "\t";
	CHAR	*pToken;
	int		i	=	0;

	ZeroMemory(&m_stAttackPowerDamage,sizeof(strAttackPowerDamage) * ATTACK_POWER_DAMAGE_NUM);

	if((fp = fopen(szFileName,"r")) != NULL)
	{
		fgets(szReadBuffer, 1023, fp);

		while(fgets(szReadBuffer, 1023, fp) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			pToken	=	strtok(NULL, cSep);
			
			pToken	=	strtok(NULL, cSep);
			m_stAttackPowerDamage[i].siMinAttackPower	=	atoi(pToken);
			
			pToken	=	strtok(NULL, cSep);
			m_stAttackPowerDamage[i].siMaxAttackPower	=	atoi(pToken);

			pToken	=	strtok(NULL, cSep);
			m_stAttackPowerDamage[i].siAttackPercent	=	atoi(pToken);

			pToken	=	strtok(NULL, cSep);
			m_stAttackPowerDamage[i].siDefencePercent	=	atoi(pToken);

			i++;
		}
	}	
	fclose(fp);
}

short CAttackPowerParser::GetAttackPercent(int siAttackPower)
{
	//siAttackPower
	for(int i = 0; i < ATTACK_POWER_DAMAGE_NUM; i++)
	{
		if((m_stAttackPowerDamage[i].siMaxAttackPower >= siAttackPower) && 
			(m_stAttackPowerDamage[i].siMinAttackPower <= siAttackPower))
			return m_stAttackPowerDamage[i].siAttackPercent;
	}
	return 0;
}	

short CAttackPowerParser::GetDefencePercent(int siAttackPower)
{
	for(int i = 0; i < ATTACK_POWER_DAMAGE_NUM; i++)
	{
		if((m_stAttackPowerDamage[i].siMaxAttackPower >= siAttackPower) && 
			(m_stAttackPowerDamage[i].siMinAttackPower <= siAttackPower))
			return m_stAttackPowerDamage[i].siDefencePercent;
	}
	return 0;
}
