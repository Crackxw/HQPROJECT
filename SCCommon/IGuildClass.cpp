#include <GSL.h>
#include "GuildClassParser.h"
#include "IGuildClass.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:20:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIGuildClass::cltIGuildClass()
{	

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:21:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIGuildClass::~cltIGuildClass()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.	
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:21:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuildClass::Init(CHAR *pszFileName)
{
	if(GuildClassParser.Init(pszFileName) == TRUE)
	{
		m_siJWJCode	=	GetNumericClassCode(SO_GUILD_SC_JWJ);			// '雖錳濠' 囀萄朝 號檜 噙檜嘎煎 嘐葬 嫡嬴 場朝棻.
		m_siSHNCode	=	GetNumericClassCode(SO_GUILD_SC_SHN);			// '餌��' 囀萄朝 號檜 噙檜嘎煎 嘐葬 嫡嬴 場朝棻.
		m_siHNSCode	=	GetNumericClassCode(SO_GUILD_SC_HNS);			// 'ч熱' 囀萄朝 號檜 噙檜嘎煎 嘐葬 嫡嬴 場朝棻.
		m_siDHSCode	=	GetNumericClassCode(SO_GUILD_SC_DHS);			// '渠ч熱'
		m_siHNDCode	=	GetNumericClassCode(SO_GUILD_SC_HND);			// 'ч翕渠濰'
		m_siDAICode	=	GetNumericClassCode(SO_GUILD_SC_DAI);			// '渠寞'

		return TRUE;
	}
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬п薯.
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:23:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuildClass::Free()
{
	GuildClassParser.Free();	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝 囀萄.(僥濠)蒂 餌辨ж罹 霜晝 囀萄.(璋濠)蒂 橢橫螞棻.	
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:24:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetNumericClassCode(CHAR *pszCode)
{
	SI32	i;

	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(stricmp(GuildClassParser.m_pstGuildClassInfo[i].szClassCode, pszCode) == 0)
			return GuildClassParser.m_pstGuildClassInfo[i].siClassCode;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝 囀萄.(璋濠)蒂 餌辨ж罹 霜晝 囀萄.(僥濠)蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-26 螃瞪 11:30:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltIGuildClass::GetStringClassCode(SI32 siCode)
{
	SI32	i;

	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode)
			return GuildClassParser.m_pstGuildClassInfo[i].szClassCode;
	}

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝 檜葷擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 5:17:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetClassName(SI32 siCode)
{
	SI32	i;

	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode)
			return GuildClassParser.m_pstGuildClassInfo[i].uiClassName;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	舒 霜晝 囀萄蒂 綠掖ж罹 堪擎 霜嬪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 3:11:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltIGuildClass::GetHighClass(CHAR *pszCode1, CHAR *pszCode2)
{	
	SI32	i;
	SI32	siRanking1, siRanking2;

	if(pszCode1 == NULL || pszCode2 == NULL)	return NULL;

	siRanking1	=	-1;
	siRanking2	=	-1;
	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(stricmp(GuildClassParser.m_pstGuildClassInfo[i].szClassCode, pszCode1) == 0)
		{
			siRanking1	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking2 != -1)	break;
		}

		if(stricmp(GuildClassParser.m_pstGuildClassInfo[i].szClassCode, pszCode2) == 0)
		{
			siRanking2	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking1 != -1)	break;
		}
	}

	if(siRanking1 == -1 || siRanking2 == -1)	return	NULL;

	if(siRanking1 < siRanking2)			return pszCode1;	
	else								return pszCode2;
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	舒 霜晝 囀萄蒂 綠掖ж罹 堪擎 霜嬪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-25 螃瞪 10:08:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetHighClass(SI32 siCode1, SI32 siCode2)
{
	SI32	i;
	SI32	siRanking1, siRanking2;

	if(siCode1 == 0 || siCode2 == 0)	return 0;

	siRanking1	=	-1;
	siRanking2	=	-1;
	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode1)
		{
			siRanking1	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking2 != -1)	break;
		}

		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode2)
		{
			siRanking2	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking1 != -1)	break;
		}
	}

	if(siRanking1 == -1 || siRanking2 == -1)	return 0;

	if(siRanking1 < siRanking2)			return siCode1;	
	else								return siCode2;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'雖錳濠'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-27 螃瞪 10:40:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetJWJCode()
{
	return	m_siJWJCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'餌��'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-27 螃瞪 10:40:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetSHNCode()
{
	return	m_siSHNCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'ч熱'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-28 螃�� 2:22:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetHNSCode()
{
	return	m_siHNSCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'渠ч熱'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2003-02-11 螃�� 2:01 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetDHSCode()
{
	return	m_siDHSCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'ч翕渠濰'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2003-02-11 螃�� 2:01 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetHNDCode()
{
	return	m_siHNDCode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	'渠寞'曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2003-04-01 螃�� 21:10 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::GetDAICode()
{
	return	m_siDAICode;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	舒 霜晝擊 綠掖и棻.
//	熱薑 橾濠	:	2002-03-28 螃�� 2:23:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuildClass::ClassComp(SI32 siCode1, SI32 siCode2)
{
	SI32	i;
	SI32	siRanking1, siRanking2;

	if(siCode1 == 0 || siCode2 == 0)	return SO_GUILD_CLASSCOMP_ERROR;

	siRanking1	=	-1;
	siRanking2	=	-1;
	for(i = 0; i < GuildClassParser.m_siClassNum; i++)
	{
		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode1)
		{
			siRanking1	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking2 != -1)	break;
		}

		if(GuildClassParser.m_pstGuildClassInfo[i].siClassCode == siCode2)
		{
			siRanking2	=	GuildClassParser.m_pstGuildClassInfo[i].siRanking;

			if(siRanking1 != -1)	break;
		}
	}

	if(siRanking1 == -1 || siRanking2 == -1)	return SO_GUILD_CLASSCOMP_ERROR;

	if(siRanking1 < siRanking2)			return SO_GUILD_CLASSCOMP_HIGH;	
	else if(siRanking1 > siRanking2)	return SO_GUILD_CLASSCOMP_LOW;	
	else								return SO_GUILD_CLASSCOMP_EQUAL;
}

