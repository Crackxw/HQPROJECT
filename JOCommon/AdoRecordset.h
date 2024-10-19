
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoRecordset.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////////////////

#ifndef __ADORECORDSET_H__
#define __ADORECORDSET_H__

//////////////////////////////////////////////////////////////////////////////////

#include "AdoConnection.h"
#include <icrsint.h>
#include <math.h>
#include "..\\JOCommon\\LogWriter.h"

//////////////////////////////////////////////////////////////////////////////////

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "EndOfFile")

//////////////////////////////////////////////////////////////////////////////////

#pragma warning (disable: 4146)

//////////////////////////////////////////////////////////////////////////////////

struct CADOFieldInfo
{
	char m_strName[30];
	short m_nType;
	long m_lSize;
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;
	BOOL m_bAllowZeroLength;
	long m_lCollatingOrder;
};

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR IntToStr(int nVal);
LPCTSTR LongToStr(long lVal);
LPCTSTR DblToStr(double dblVal);
LPCTSTR DblToStr(float fltVal);

//////////////////////////////////////////////////////////////////////////////////

class CAdoRecordset{
public : 
	enum cadoOpenEnum		{ openUnknown=0, openQuery=1, openTable=2, openStoredProc=3, openStoredProc_No_Rs=4 };
	enum cadoEditEnum		{ dbEditNone=0, dbEditNew=1, dbEdit=2 };
	enum cadoPositionEnum	{ positionUnknown=-1, positionBOF=-2, positionEOF=-3 };
	enum cadoSearchEnum		{ searchForward=1, searchBackward=-1 };
	enum cadoDataType
	{
		typeEmpty = adEmpty,
		typeTinyInt = adTinyInt,
		typeSmallInt = adSmallInt,
		typeInteger = adInteger,
		typeBigInt = adBigInt,
		typeUnsignedTinyInt = adUnsignedTinyInt,
		typeUnsignedSmallInt = adUnsignedSmallInt,
		typeUnsignedInt = adUnsignedInt,
		typeUnsignedBigInt = adUnsignedBigInt,
		typeSingle = adSingle,
		typeDouble = adDouble,
		typeCurrency = adCurrency,
		typeDecimal = adDecimal,
		typeNumeric = adNumeric,
		typeBoolean = adBoolean,
		typeError = adError,
		typeUserDefined = adUserDefined,
		typeVariant = adVariant,
		typeIDispatch = adIDispatch,
		typeIUnknown = adIUnknown,
		typeGUID = adGUID,
		typeDate = adDate,
		typeDBDate = adDBDate,
		typeDBTime = adDBTime,
		typeDBTimeStamp = adDBTimeStamp,
		typeBSTR = adBSTR,
		typeChar = adChar,
		typeVarChar = adVarChar,
		typeLongVarChar = adLongVarChar,
		typeWChar = adWChar,
		typeVarWChar = adVarWChar,
		typeLongVarWChar = adLongVarWChar,
		typeBinary = adBinary,
		typeVarBinary = adVarBinary,
		typeLongVarBinary = adLongVarBinary,
		typeChapter = adChapter,
		typeFileTime = adFileTime,
		typePropVariant = adPropVariant,
		typeVarNumeric = adVarNumeric,
		typeArray = adVariant
	};

	_variant_t GetFieldValue(LPCTSTR lpFieldName);
	_variant_t GetFieldValue(short snIndex);

	bool SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue);
	bool SetFieldValue(_variant_t vtIndex, _variant_t vtValue);

	void CancelUpdate();
	bool Update();
	void Edit();
	bool AddNew();
	bool AddNew(CADORecordBinding &pAdoRecordBinding);

	bool Find(LPCTSTR lpFind, int nSearchDirection = CAdoRecordset::searchForward);
	bool FindFirst(LPCTSTR lpFind);
	bool FindNext();
	
	
	LPCTSTR GetQuery();		
	void SetQuery(LPCSTR strQuery);
	bool RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	bool IsOpen();
	void Close();
	bool Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = "", int nOption = CAdoRecordset::openUnknown);
	bool Open(LPCTSTR lpstrExec = "", int nOption = CAdoRecordset::openUnknown);
	long GetFieldCount();		
	bool Clone(CAdoRecordset& pRs);
	bool IsFieldNull(LPCTSTR lpFieldName);
	bool IsFieldNull(int nIndex);
	bool IsFieldEmpty(LPCTSTR lpFieldName);
	bool IsFieldEmpty(int nIndex);	
	bool IsEof();		
	bool IsEOF();		
	bool IsBof();		
	bool IsBOF();		
	void MoveFirst();		
	void MoveNext();		
	void MovePrevious();		
	void MoveLast();
	long GetAbsolutePage();
	void SetAbsolutePage(int nPage);
	long GetPageCount();		
	long GetPageSize();
	void SetPageSize(int nSize);
	long GetAbsolutePosition();
	void SetAbsolutePosition(int nPosition);
	bool GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	bool GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	bool GetChunk(LPCTSTR lpFieldName, LPCTSTR& strValue);
	LPCTSTR GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	LPCTSTR GetLastErrorString();
	DWORD GetLastError();
	void GetBookmark();
	bool SetBookmark();
	bool Delete();
	bool IsConnectionOpen();
	_RecordsetPtr GetRecordset();
	bool SetFilter(LPCTSTR strFilter);
	bool GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo);

	void dump_com_error(_com_error &e);

	void SetRecordset(const _RecordsetPtr pRecordset)
	{ m_pRecordset = pRecordset; }

	CAdoRecordset();
	CAdoRecordset(CAdoConnection* pAdoConnection);	
	~CAdoRecordset();
	

private :
	_RecordsetPtr	m_pRecordset;	
	
private : 
	CLogWriter	m_LogWriter;
	
protected :
	_ConnectionPtr m_pConnection; // _ConnectionPtr 霤褻辨 偌羹.
	int m_nSearchDirection;
	LPCTSTR m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	LPCTSTR m_strLastError;
	DWORD m_dwLastError;	
	IADORecordBinding *m_pRecBinding;
	LPCTSTR m_strQuery;
};

//////////////////////////////////////////////////////////////////////////////////

#endif




