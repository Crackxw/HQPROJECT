#include <GSL.h>
#include <math.h>

#ifdef _IMJIN2
#include <clGame.h>
#include <Main.h>
#include <etc.h>
#include <OnlineWorld.h>		// hojae_append (������ OK!)
#include "charanimation.h"
#include <kindinfo.h>	
#include "..\Gersang\maindata.h"
#endif

#include <stdio.h>
#include <OnlineMyWeapons.h>
#include <OnLineItem.h>
#include <ParametaBox.h>

#ifdef _IMJIN2
	extern _clGame*					pGame;
	extern	char 					SavePath[];
	extern _MainData				GD;
#endif


#include "BindJxFile.h"

SI32 ExpForLevelUp[MAX_ONLINE_LEVEL][3];

void _ImportantParameta::Init(	SI32 curexp,		SI32 level, SI32 str,				SI32 dex,		
								SI32 vit,			SI32 intl,				SI32 life, 
								SI32 mana,			SI32 ac,				SI32 bonus,         
								SI32 damageresist,	
								SI32 magicresist,
								SI32 equipmindamage,SI32 equipmaxdamage,
								SI32 liftconstant)
{
	SetCurExp(curexp);
	SetLevel(level);
	SetStr(str);
	SetDex(dex);
	SetVit(vit);
	SetInt(intl);
	SetAC(ac);
	SetBonus(bonus);
	SetLife(life);
	SetMana(mana);
	DamageResist		= damageresist;
	MagicResist			= magicresist;
	EquipMinDamage		= equipmindamage;
	EquipMaxDamage		= equipmaxdamage;
	LiftConstant		= liftconstant;
}


void  _ImportantParameta::Init(_ImportantParameta ip)
{

	Init(	ip.GetCurExp(),			ip.GetLevel(), ip.GetStr(), ip.GetDex(), 
			ip.GetVit(),			ip.GetInt(), ip.GetLife(), 
			ip.GetMana(),			ip.GetAC(),	ip.GetBonus(), 
			ip.DamageResist,	ip.MagicResist, 
			ip.EquipMinDamage,  ip.EquipMaxDamage,
			ip.LiftConstant);
}

void _ImportantParameta::UpdateCheckSum()
{
	siCheckSum = ~( GetCurExp() + GetStr()+ GetDex() + GetVit() + GetInt() + GetAC() + GetBonus() + GetLevel() + GetLife() + GetMana());
}


// ��ŷ�� ���ɼ��� Ž���Ѵ�. 
// TRUE : �̻����. 
// FALSE : �̻�����. 
BOOL _ImportantParameta::CheckCheckSum()
{
	if((~siCheckSum) == ( GetCurExp() + GetStr()+ GetDex() + GetVit() + GetInt() + GetAC() + GetBonus() + GetLevel() + GetLife() + GetMana()))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _ImportantParameta::IncreaseExp(SI32 exp)
{
	if(GetCurExp() + exp < 0)
	{
		SetCurExp(0);
	}
	else
	{
		SetCurExp(GetCurExp() + exp);
	}

	if(GetCurExp() >= GetRequiredExpForLevel(GetLevel() + 1)
	&& GetLevel() < (MAX_ONLINE_LEVEL - 1))
	{
		//printf("������ ������.");
		return TRUE;
	}


	//printf("����ġ ����: %d %d", GetCurExp(), GetRequiredExpForLevel(GetLevel() + 1));
	return FALSE;
}


SI32 _ParametaBox::GetMyExp() const
{
	SI32 apower = (MaxApower + MinApower)/2;
	SI32 dpower = GetTotalAC();

	return min(1200 + 800 + 2000, (SI32) sqrt( (apower + dpower + GetTotalDex()) * (GetMaxLife()/2) ) / 2 + IP.GetLevel());

}

SI32 _ParametaBox::CalcApower()
{
#ifdef _IMJIN2
	SI32 diff = MaxApower - MinApower;
	if(diff < 0)diff = 0;
	
	return MinApower + AssistApowerByGEffect + GD.Frame % (diff + 1); // ��� ȿ���� ���� ���ݷµ� ����Ѵ�. 
#else
	return 0;
#endif
}

void _ParametaBox::Init(SI32 curexp,		SI32 level,			SI32 str,				SI32 dex,		
						SI32 vit,			SI32 intl,				SI32 life, 
						SI32 mana,			SI32 ac,				SI32 bonus,         
						SI32 damageresist,	
						SI32 magicresist,	
						SI32 equipmindamage,SI32 equipmaxdamage,
						SI32 liftconstant)
{
	ZeroMemory(this, sizeof(_ParametaBox));

	IP.Init(curexp,		level,			str,		dex,		
						vit,			intl,		life, mana, ac,			bonus, 
						damageresist,	
						magicresist,	
						equipmindamage,	equipmaxdamage,
						liftconstant);
}

void  _ParametaBox::Init(_ImportantParameta ip)
{

	Init(	ip.GetCurExp(),			ip.GetLevel(), ip.GetStr(), ip.GetDex(), 
			ip.GetVit(),				ip.GetInt(), ip.GetLife(), 
			ip.GetMana(),			ip.GetAC(),	ip.GetBonus(), 
			ip.DamageResist,	ip.MagicResist, 
			ip.EquipMinDamage,  ip.EquipMaxDamage,
			ip.LiftConstant);
}



BOOL _ParametaBox::Action(CItem *pItem, MyWeapons MW)
{
	SI32 weightrate = 1;

	// �������� ���. 
#ifdef _IMJIN2
	NextExp = ExpForLevelUp[ IP.GetLevel() ][1];
#endif

	// ���ǰ�� ���� �Ķ��Ÿ ����ġ�� �ݿ��Ѵ�.
	AssistAC				= 0;
	AssistEquipMinDamage = 0;
	AssistEquipMaxDamage = 0;
	AssistDamageResist	= 0;
	AssistMagicResist	= 0;
	AssistDex			= 0;
	AssistStr			= 0;
	AssistVit			= 0;
	AssistInt			= 0; 
//	AssistHp			= 0;

	// ��� ������ ���� ȿ���� �����Ѵ�. 
	ItemOperation(pItem, MW);


	TotalStr			= IP.GetStr()			+ AssistStr;
	TotalDex			= IP.GetDex()			+ AssistDex;
	TotalVit			= IP.GetVit()			+ AssistVit;
	TotalInt			= IP.GetInt()			+ AssistInt;
	TotalAC				= IP.GetAC()			+ AssistAC;
	TotalDamageResist	= IP.DamageResist	+ AssistDamageResist;
	TotalMagicResist	= IP.MagicResist	+ AssistMagicResist;
	TotalEquipMinDamage	= IP.EquipMinDamage	+ AssistEquipMinDamage;
	TotalEquipMaxDamage	= IP.EquipMaxDamage	+ AssistEquipMaxDamage;

	MaxLife				= GetTotalVit() * 4; //+ AssistHp;
	MaxMana				= GetTotalInt() * 4;

	MinApower			= TotalEquipMinDamage + IP.GetLevel()*6/10 + TotalStr*4/10;
	MaxApower			= TotalEquipMaxDamage + IP.GetLevel()*6/10 + TotalStr*4/10;
	Dpower				= TotalAC + IP.GetLevel()*15/10 + TotalStr*2/10	;			
	MaxWeight			= GetLiftPower();
	
	if(MaxWeight)	weightrate = (CurWeight/MaxWeight);
	else			weightrate = 1;

	//MoveSpeed			= max(25, TotalDex * (1 - weightrate)*100/350);

	AR					=  (IP.GetLevel()  + TotalDex) / 4;
	DR					=  AR + GetTotalDpower() + AssistDpowerByGEffect; // ���ȿ���� �ݿ��Ѵ�. 


	// ��ŷ�� ����� �ִ� ���, 
	if(IP.CheckCheckSum() == FALSE)
		return FALSE;

	return TRUE;
}

SI32 _ParametaBox::GetHitRate(SI32 enemydr)const			
{

	if((AR + enemydr) ==0)
	{
		return 0;
	}
	else
	{
		return AR * 100 / (AR + enemydr);
	}
	
	return 0;
}

// �� ������ �������� ���Ѵ�. 
SI32 _ParametaBox::GetBattlePower()const
{
	SI32 avgpower = (MinApower + MaxApower + GetTotalDex() + GetTotalInt()) / 3 ;

	return (avgpower * (IP.GetLife())) / 20;
}

// �� ������ �ִ� �������� ���Ѵ�. 
SI32 _ParametaBox::GetMaxBattlePower()const
{
	SI32 avgpower = (MinApower + MaxApower + GetTotalDex() + GetTotalInt()) / 3 ;

	return (avgpower * (GetMaxLife())) / 20;
}

// �� �������� ������ �� �ִ°�
BOOL _ParametaBox::CanEquip(CItem *pItem, SI32 siItem)const
{
#ifdef _IMJIN2
	SI32 reqLv;

	CItemHeader *pItemHeader = pItem->GetItem( siItem );

	if( pItemHeader == NULL )		return FALSE;

	if( pItemHeader->m_siGroupCode & ON_ITEM_SWORD )				// ����
	{
		reqLv  = pItemHeader->m_siReqLv;

		if( IP.GetLevel() < reqLv )		return FALSE;
	}

	else if( pItemHeader->m_siGroupCode & ON_ITEM_ARMOR ||  pItemHeader->m_siGroupCode & ON_ITEM_DRESS )			// ����
	{
	}
	
	else if( pItemHeader->m_siGroupCode & ON_ITEM_HELMAT )		//
	{
	}

	else if( pItemHeader->m_siGroupCode & ON_ITEM_GLOVES )		//
	{
	}

	else if( pItemHeader->m_siGroupCode & ON_ITEM_BELT )			//
	{
	}

	else if( pItemHeader->m_siGroupCode & ON_ITEM_BOOTS )				//
	{
	}

	else if( pItemHeader->m_siGroupCode & ON_ITEM_BOOTS )				//
	{
	}
#endif
	return TRUE;
}



BOOL _ParametaBox::ItemOperation(CItem *pItem, MyWeapons MW)
{

	SI32 i;
	CItemHeader *pItemHeader;

	if( pItem == NULL ) return TRUE;

	GEffectDpower = 0;
	GEffectApower = 0;
	GEffectHeal   = 0;

	for( i=0; i<WEAR_ITEM_NUM; i++ )
	{
		pItemHeader	=	pItem->GetItem(MW.GetWearItem(i));
		if( pItemHeader )
		{
			if( i == ON_WEAR_WEAPON_ITEM )	// ����
			{
				if( pItemHeader->pWeaponHeader )
				{
					AssistEquipMinDamage += pItemHeader->pWeaponHeader->siMinDam;
					AssistEquipMaxDamage += pItemHeader->pWeaponHeader->siMaxDam;

				}
			}
			else
			{
//				AssistEquipMinDamage += pItemHeader->m_siAttack;
//				AssistEquipMaxDamage += pItemHeader->m_siAttack;
				AssistDamageResist	+= pItemHeader->m_siDamageRegist;
				AssistMagicResist	+= pItemHeader->m_siMagicRegist;
			}

			// �Ǻ��� ��쿡�� ���ַ� ��� ���� ������ ���� �ȴ�. 
			if( pItemHeader->m_siGroupCode & ON_ITEM_DRESS)
			{
				GEffectDpower	= pItemHeader->m_siReqStr;
				GEffectApower	= pItemHeader->m_siReqDex;
//				GEffectHeal		= pItemHeader->m_siMedic;
			}

			AssistAC			+= pItemHeader->m_siDefense;
			AssistDex			+= pItemHeader->m_siDex;
			AssistStr			+= pItemHeader->m_siStrength;
			AssistVit			+= pItemHeader->m_siVitality;
			AssistInt			+= pItemHeader->m_siInt;
		}
	}


	return TRUE;
}



// level�� �ʿ��� ����ġ ��. (1���� �����Ѵ�.)
SI32 GetRequiredExpForLevel(SI32 level)
{
	if(level < 1)return 0;
	return ExpForLevelUp[level - 1][1];
}


BOOL LoadExpData(BOOL bTest)
{
	FILE	*fp;
	SI32	data;
	CHAR	buffer[1024], temp[5];
	INT		index;

	if(bTest == TRUE)
	{
		if((fp = BindJXFile::GetInstance()->GetFile( "Test_ExpForLevelUp.dat" )) == NULL)	
		{
			return FALSE;
		}
	}
	else
	{
		if((fp = BindJXFile::GetInstance()->GetFile( "ExpForLevelUp.dat" )) == NULL)	
		{
			return FALSE;
		}
	}


	// ù��° 5���� �����Ѵ�. 
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);

	index = 0;

	while( fgets(buffer, 1024, fp) != NULL)
	{ 
		if( index >= MAX_ONLINE_LEVEL)	break;

		ZeroMemory(temp, sizeof(temp));
		memcpy(temp, buffer, sizeof(temp)-1);

		sscanf(buffer, "%d %d %d %d", &ExpForLevelUp[index][0], &ExpForLevelUp[index][1], &ExpForLevelUp[index][2], &data);
		index++;
	}

	fclose(fp);

	return TRUE;
}


SI32 EncodePassword(SI32 number)
{

	return (number^777);

}

SI32 DecodePassword(SI32 number)
{
	return (number^777);
}



