#ifndef _PARAMETABOX_H
#define _PARAMETABOX_H

#pragma warning ( disable : 4786 )

#include <Directives.h>
#include <Windows.h>
#include <stdio.h>


#define MAX_ONLINE_LEVEL 150

class	CItem;
class	MyWeapons;

SI32 EncodePassword(SI32 number);
SI32 DecodePassword(SI32 number);

//--------------------------------
// 전투시 전달되어야 할 필수 파라메타. 
//--------------------------------
class _ImportantParameta{
private:
	SI32	siSeed;
	SI32	siCheckSum;
	SI32	siCurExp;
	SI32	siLevel;
	SI16	siStr;
	SI16	siDex;
	SI16	siVit;
	SI16	siInt;
	SI16	siAC;
	SI16	siBonus;
	SI32	siLife;
	SI32	siMana;

	bool    m_isCycleSaveToDB;		// 주기적으로 DB에 저장 여부 
	bool	m_isSaveToDB;			// DB에 저장 여부 

public:
	inline void	setCycleSaveToDB(bool which)  { m_isCycleSaveToDB  = which; }
	inline bool	isCycleSaveToDB()			  { return m_isCycleSaveToDB;   }

	inline void	setSaveToDB(bool which)		  { m_isSaveToDB = which; }
	inline bool	isSaveToDB()				  { return m_isSaveToDB;  }
			
	SI16    Reserved;

	SI16	DamageResist;
	SI16	MagicResist;

	SI16	EquipMinDamage;
	SI16	EquipMaxDamage;

	SI32	LiftConstant;
	
	
	void Init(	SI32 CurExp,		SI32 level,			SI32 Str,				SI32 Dex,		
				SI32 Vit,			SI32 Int,				SI32 life, 
				SI32 mana,			SI32 AC,				SI32 bonus,         
				SI32 DamageResist,	SI32 MagicResist,		SI32 EquipMinDamage,SI32 EquipMaxDamage,
				SI32 liftconstant);

	void Init(_ImportantParameta ip);

	void UpdateCheckSum();

	// 해킹의 가능성을 탐지한다. 
	// TRUE : 이상없음. 
	// FALSE : 이상있음. 
	BOOL CheckCheckSum();

	SI32 GetCurExp()const
	{
		return DecodePassword(siCurExp);
	}
	void SetCurExp(SI32 exp)
	{
		siCurExp = EncodePassword(exp);
		UpdateCheckSum();
		
		// 경험치가 변경되었음을 알린다. 
		setCycleSaveToDB(TRUE);
		setSaveToDB(TRUE);
	}

	SI32 GetStr()const
	{
		return DecodePassword(siStr);
	}
	void SetStr(SI32 dat)
	{
		siStr = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetDex()const
	{
		return DecodePassword(siDex);
	}
	void SetDex(SI32 dat)
	{
		siDex = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetVit()const
	{
		return DecodePassword(siVit);
	}
	void SetVit(SI32 dat)
	{
		siVit = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetInt()const
	{
		return DecodePassword(siInt);
	}
	void SetInt(SI32 dat)
	{
		siInt = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetAC()const
	{
		return DecodePassword(siAC);
	}
	void SetAC(SI32 dat)
	{
		siAC = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetBonus()const
	{
		return DecodePassword(siBonus);
	}
	void SetBonus(SI32 dat)
	{
		siBonus = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetLevel()const
	{
		return DecodePassword(siLevel);
	}
	void SetLevel(SI32 dat)
	{
		siLevel = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetLife()const
	{
		return DecodePassword(siLife);
	}
	void SetLife(SI32 dat)
	{
		siLife = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	SI32 GetMana()const
	{
		return DecodePassword(siMana);
	}
	void SetMana(SI32 dat, SI32 siParam = 1)
	{
		/*
		if( dat >= 10000 )
		{
			FILE	*fp;

			if( ( fp = fopen( "..\\SetMana.txt", "at" ) ) != NULL )
			{
				fprintf( fp, "Param : [ %d ], Mana : [ %d ]  \n", siParam, dat );
				fclose( fp );
			}
		}
		*/

		siMana = EncodePassword(dat);
		UpdateCheckSum();

		setSaveToDB(TRUE);
	}

	// 경험치를 올려준다. 
	BOOL IncreaseExp(SI32 exp);

	//ImportantParameta에 의한 최대 체력값. 
	SI32 GetMaxLifeByIP()const{return GetVit()*4;}

	//ImportantParameta에 의한 최대 마나값. 
	SI32 GetMaxManaByIP()const{return GetInt()*4;}
};

class _ParametaBox{
public:

_ImportantParameta IP;

//---------------------
// 레벨 관련 스테이터스 
//---------------------
	SI32 NextExp;

	SI32 MyExp;


//---------------------
//  메인 스테이터스 
//---------------------
	SI32 AssistStr;
	SI32 TotalStr;
	
	SI32 AssistDex;
	SI32 TotalDex;
	
	SI32 AssistVit;
	SI32 TotalVit;
	
	SI32 AssistInt;
	SI32 TotalInt;
	
	SI32 AssistAC;
	SI32 TotalAC;
	
	////////////////////
//	SI32 AssistHp;
	////////////////////

	SI32 AssistDamageResist;
	SI32 TotalDamageResist;

	SI32 AssistMagicResist;
	SI32 TotalMagicResist;
	
	SI32 AssistEquipMinDamage;
	SI32 AssistEquipMaxDamage;
	SI32 TotalEquipMinDamage;
	SI32 TotalEquipMaxDamage;

	SI32 GEffectApower;
	SI32 GEffectDpower;
	SI32 GEffectHeal;

	SI32 AssistApowerByGEffect;
	SI32 AssistDpowerByGEffect;

	SI32 Dpower;

//---------------------
//  서브 스테이터스 
//---------------------
	SI32 MaxLife;
	SI32 MaxMana;
	SI32 MinApower;
	SI32 MaxApower;
	SI32 MoveSpeed;
protected:
	SI32 AR;
	SI32 DR;
public:
	SI32 MaxWeight;
	SI32 CurWeight;
	SI32	Credit;

	SI32 siItemAttackRange;

	// 공격력 관련 
	SI32 			AssistApowerFrame;
	
	// 방어력 관련. 
	SI32 			AssistDpowerFrame;

public:


	void Init(	SI32 CurExp,		SI32 level,			SI32 Str,				SI32 Dex,		
				SI32 Vit,			SI32 Int,				SI32 Life,				SI32 Mana, 
				SI32 AC,			SI32 bonus,				SI32 DamageResist,	
				SI32 MagicResist,	
				SI32 EquipMinDamage,SI32 EquipMaxDamage,
				SI32 liftconstant);

	void Init(_ImportantParameta ip);

	BOOL Action(CItem *pItem, MyWeapons MW);

	SI32 GetCurExp()			{return IP.GetCurExp();}
	SI32 GetNextExp()const			{return NextExp;}
	SI32 GetTotalStr()const			{return TotalStr;}
	SI32 GetTotalDex()const			{return TotalDex;}
	SI32 GetTotalVit()const			{return TotalVit;}
	SI32 GetTotalInt()const			{return TotalInt;}
	SI32 GetAC()					{return IP.GetAC();}
	void SetAC(SI32 ac)			{IP.SetAC(ac);}


		
	SI32 GetTotalAC()const			{return TotalAC;}
	SI32 GetTotalDamageResist()const	{return TotalDamageResist;}
	void SetTotalDamageResist(SI32 siDamageResist){TotalDamageResist = siDamageResist;}
	SI32 GetTotalMagicResist()const	{return TotalMagicResist;}
	void SetTotalMagicResist(SI32 siMagicResist){TotalMagicResist = siMagicResist;}
	SI32 GetTotalEquipMinDamage()const{return TotalEquipMinDamage;}
	SI32 GetTotalEquipMaxDamage()const{return TotalEquipMaxDamage;}
	SI32 GetMoveSpeed()const			{return MoveSpeed;}
	SI32 GetAR()const				{return AR;}
	SI32 GetDR()const				{return DR;}
	SI32 GetMaxWeight()const			{return MaxWeight;}
	SI32 GetCurWeight()const			{return CurWeight;}
	SI32 GetCredit()const			{return Credit;}
	SI32 GetLiftPower()const			{return TotalStr * IP.LiftConstant / 50;}
	SI32 GetHitRate(SI32 enemydr)const;			

	SI32 GetMaxLife()const			{return MaxLife;}
	void SetMaxLife(SI32 life)	{MaxLife = life;}
	SI32 GetMaxMana()const			{return MaxMana;}
	void SetMaxMana(SI32 mana)	{MaxMana = mana;}

	SI32 GetMinApower()const{return MinApower;}
	SI32 GetMaxApower()const{return MaxApower;}
	SI32 GetDpower()const{return Dpower;}
	SI32 GetTotalDpower()const{return Dpower + AssistDpowerByGEffect;}

	SI32 GetItemAttackRange()const{return siItemAttackRange;}
	void SetItemAttackRange(SI32 range){siItemAttackRange = range;}

	// 최소공격력, 최대 공격력을 참고하여 실질 공격력을 얻어온다. 
	SI32 CalcApower();

	


	_ImportantParameta GetImportantParameta(){return IP;}
	_ImportantParameta SetImportantParameta(_ImportantParameta ip){return IP = ip;}

	// 각 유닛의 전투력을 구한다. 
	SI32 GetBattlePower()const;
	// 각 유닛의 최대 전투력을 구한다. 
	SI32 GetMaxBattlePower()const;


	SI32 GetMyExp()const;

	BOOL CanEquip(CItem *pItem, SI32 siItem)const;

	BOOL ItemOperation(CItem *pItem, MyWeapons MW);
};


	BOOL LoadExpData(BOOL bTest);


	// level에 필요한 경험치 값. (1부터 시작한다.)
	SI32 GetRequiredExpForLevel(SI32 level);


#endif