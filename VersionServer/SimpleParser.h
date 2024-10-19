#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

typedef enum {
	ST_NONE,
	ST_INT,
	ST_STRING,
} SYMBOL_TYPE;

#define SYMBOLNAMESIZE 128

class SymbolInf {
private:
	SYMBOL_TYPE m_type;
	char m_symbol[SYMBOLNAMESIZE];
	void *m_pValue;
	int m_iValueSize;

public:
	SymbolInf(void);
	~SymbolInf();

	void Create(SYMBOL_TYPE symbolType, char *pSymbolName, void *pValue, int valueSize, bool helpMsgOn = false);
	void SetInt(int value);
	void SetString(const char *pString);

	SYMBOL_TYPE GetType(void);
	char *GetSymbol(void);
	int GetInt(void);
	char *GetString(void);
	bool IsValid(void);
	
	void SymbolOut(void);
};

class SimpleParser {
private:
	int			m_iSymbolNumber;
	SymbolInf	*m_pSymbolInf;

private:
	SymbolInf	*FindSymbolInf(const char *pSymbol);
	int GetStringToken(char *pBuff);
	const char *GetToken(char **p, char *sep);
	const char *GetStringToken(char **ppBuff, char *sep);
	const char *GetIntToken(char **p, char *sep);
	bool IsSep(char ch, char *sep);
	bool IsNumberString(const char *pString);

public:
	SimpleParser(int tableSize = 100);
	~SimpleParser();

	bool Do(char *pFileName);

	bool AddStringSymbol(char *pSymbol, char *pValue, int stringSize, bool helpMsgOn = false);
	bool AddIntSymbol(char *pSymbol, int *pValue, bool helpMsgOn = false);
//	int GetIntSymbol(char *pSymbol);
//	char *GetStringSymbol(char *pSymbol);

	void OutSymbol(void);
};

#endif
