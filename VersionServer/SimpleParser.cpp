#include <GSL.h>
#include "SimpleParser.h"

static char buff[1024 * 10];

SymbolInf::SymbolInf(void)
{
	m_type = ST_NONE;
	m_symbol[0] = '\0';
	m_pValue = NULL;
}

SymbolInf::~SymbolInf()
{
}

void SymbolInf::Create(SYMBOL_TYPE symbolType, char *pSymbolName, void *pValue, int valueSize, bool helpMsgOn)
{
	ASSERT(pSymbolName);
	m_type = symbolType;
	m_iValueSize = valueSize;
	m_pValue = pValue;

	ASSERT(strlen(pSymbolName) < SYMBOLNAMESIZE);
	strcpy(m_symbol, pSymbolName);

	if(helpMsgOn == true)
	{
		switch(symbolType)
		{
			case ST_INT :
				printf("Format>> %s = (integer)\n", pSymbolName);
				break;
			case ST_STRING :
				printf("Format>> %s = (string)\n", pSymbolName);
				break;
		}
	}
}

SYMBOL_TYPE SymbolInf::GetType(void)
{
	return(m_type);
}

char *SymbolInf::GetSymbol(void)
{
	return(m_symbol);
}

int SymbolInf::GetInt(void)
{
	return (*(int *)m_pValue);
}

char *SymbolInf::GetString(void)
{
	return((char*)m_pValue);
}

void SymbolInf::SetInt(int iValue)
{
	*(int *)m_pValue = iValue;
}

void SymbolInf::SetString(const char *pString)
{
	ASSERT((int)(strlen((char*)pString)+1) < m_iValueSize);
	strcpy((char*)m_pValue, pString);
}

void SymbolInf::SymbolOut(void)
{
	switch(GetType())
	{
		case ST_INT :
			printf("%s = (INT)%d\n", m_symbol, GetInt());
			break;
		case ST_STRING :
			printf("%s = (STRING)%s\n", m_symbol, GetString());
			break;
		default :			
			break;
	}
}

bool SymbolInf::IsValid(void)
{
	return((m_type==ST_NONE)? FALSE : TRUE);
}

SimpleParser::SimpleParser(int tableSize)
{
	m_iSymbolNumber = 0;
	m_pSymbolInf = new SymbolInf[tableSize];
}

SimpleParser::~SimpleParser()
{
	if(m_pSymbolInf != NULL) delete []m_pSymbolInf;
}

bool SimpleParser::AddIntSymbol(char *pSymbol, int *pValue, bool helpMsgOn)
{
	m_pSymbolInf[m_iSymbolNumber].Create(ST_INT, pSymbol, pValue, sizeof(int), helpMsgOn);
	m_iSymbolNumber++;

	return(true);
}

bool SimpleParser::AddStringSymbol(char *pSymbol, char *pValue, int stringSize, bool helpMsgOn)
{
	m_pSymbolInf[m_iSymbolNumber].Create(ST_STRING, pSymbol, pValue, stringSize, helpMsgOn);
	m_iSymbolNumber++;

	return(true);
}
/*
char *SimpleParser::GetStringSymbol(char *pSymbol)
{
	SymbolInf *pSymbolInf;

	pSymbolInf = FindSymbolInf(pSymbol);
	if(pSymbolInf != NULL) return(pSymbolInf->GetSymbol());

	return(NULL);
}

int SimpleParser::GetIntSymbol(char *pSymbol)
{
	SymbolInf *pSymbolInf;

	pSymbolInf = FindSymbolInf(pSymbol);
	if(pSymbolInf != NULL) return(pSymbolInf->GetInt());

	return(NULL);
}
*/
SymbolInf *SimpleParser::FindSymbolInf(const char *pSymbol)
{
	int i;

	for( i = 0; i < m_iSymbolNumber; i++ )
	{
		if(stricmp(pSymbol, m_pSymbolInf[i].GetSymbol()) == 0)
		{
			return(&m_pSymbolInf[i]);
		}
	}

	return(NULL);
}

bool SimpleParser::IsSep(char ch, char *sep)
{
	while(*sep != '\0')
	{
		if(ch == *sep) return(true);
		sep++;
	}

	return(false);
}

bool SimpleParser::IsNumberString(const char *pString)
{
	while(*pString != '\0')
	{
		if((*pString < '0') || (*pString > '9')) return(false);
		pString++;
	}

	return(true);
}

const char *SimpleParser::GetToken(char **ppBuff, char *sep)
{
	char *pBuff = *ppBuff;
	int buffIndex;

	buffIndex = 0;

	while(IsSep(*pBuff, sep) == true) pBuff++;

	while(1)
	{
		if(*pBuff == '\0')
		{
			if(buffIndex == 0) return(NULL);
			break;
		}
		if(IsSep(*pBuff, sep) == true) break;
		buff[buffIndex++] = *pBuff++;
	}

	buff[buffIndex] = '\0';
	*ppBuff = pBuff;
	return((const char*)buff);
}

const char *SimpleParser::GetStringToken(char **ppBuff, char *sep)
{
	char *pBuff = *ppBuff;
	int buffIndex;

	while(IsSep(*pBuff, sep)== true) pBuff++;
	if(*pBuff !='\"') return(NULL);
	pBuff++;

	buffIndex = 0;

	while(*pBuff != '\"')
	{
		if(*pBuff == '\0')
		{
			if(buffIndex == 0) return(NULL);
			break;
		}
		if(*pBuff == '\\')
		{
			pBuff++;
			if(*pBuff == '\0') return(NULL);
		}

		buff[buffIndex++] = *pBuff++;
	}

	buff[buffIndex] = '\0';
	*ppBuff = pBuff;
	return((const char*)buff);
}

const char *SimpleParser::GetIntToken(char **ppBuff, char *sep)
{
	static char buff[1024 * 10];
	char *pBuff = *ppBuff;
	int buffIndex;

	while(IsSep(*pBuff, sep)== true) pBuff++;

	buffIndex = 0;

	while(((*pBuff >= '0') && (*pBuff <= '9')) == true) 
	{
		if(*pBuff == '\0')
		{
			if(buffIndex == 0) return(NULL);
			break;
		}
		while(IsSep(*pBuff, " \t")== true) pBuff++;
		buff[buffIndex++] = *pBuff++;
	}

	buff[buffIndex] = '\0';
	*ppBuff = pBuff;
	return((const char*)buff);
}

bool SimpleParser::Do(char *pFileName)
{
	bool	bReturn = true;
	CHAR	szReadBuffer[1024];
	CHAR	*pBuff;
	CHAR	cSep[] = " \t\n";
	const char	*pToken;
//	int		symbolIndex;
	SymbolInf *pSymbolInf;
//	CHAR	szSymbol[1024];
	int		lineNumber = 0;

	FILE *pFile = fopen(pFileName, "r");

	if(pFile == NULL) return(false);

	while(fgets(szReadBuffer, 1023, pFile) != NULL)
	{
		pBuff = szReadBuffer;
		lineNumber++;
		pToken = GetToken(&pBuff, cSep);
		if(pToken == NULL) continue;
		if(memcmp(pToken,"//", 2) == 0) continue;

		pSymbolInf = FindSymbolInf(pToken);
		if(pSymbolInf == NULL) 
		{
			printf("It is not defined symbol(%s)\n", pToken);
			if(pFile != NULL) fclose(pFile);
			return(false);
		}

		pToken = GetToken(&pBuff, cSep);
		if(stricmp(pToken, "=") != 0)
		{
			printf("line(%d) parsing error!\n", lineNumber, pToken);
			if(pFile != NULL) fclose(pFile);
			return(false);
		}

		switch(pSymbolInf->GetType())
		{
			case ST_INT:
				if((pToken = GetIntToken(&pBuff, cSep) ) == NULL)
				{
					printf("line(%d) error. Parameter must be number!\n", lineNumber, pToken);
					if(pFile != NULL) fclose(pFile);
					return(false);
				}
				pSymbolInf->SetInt(atoi(pToken));
				break;
			case ST_STRING:
				if((pToken = GetStringToken(&pBuff, cSep) ) == NULL)
				{
					printf("line(%d) error. Parameter must be string!\n", lineNumber, pToken);
					if(pFile != NULL) fclose(pFile);
					return(false);
				}
				pSymbolInf->SetString(pToken);
				break;
			default:
				break;
		}
	}

	if(pFile != NULL) fclose(pFile);

	return bReturn;
}

void SimpleParser::OutSymbol(void)
{
	for( int i = 0; i < m_iSymbolNumber; i++ )
	{
		if(m_pSymbolInf[i].IsValid() == true) m_pSymbolInf[i].SymbolOut();
	}
}
