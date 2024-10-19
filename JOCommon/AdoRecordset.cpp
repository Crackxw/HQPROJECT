
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoRecordset.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////////////////

#include "AdoRecordset.h"
#include <stdarg.h>
#include <tchar.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////

CAdoRecordset::CAdoRecordset()
{
	m_pRecordset = NULL;
	m_strQuery = "";
	m_strLastError = "";
	m_dwLastError = 0;
	m_pRecBinding = NULL;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_nEditStatus = CAdoRecordset::dbEdit;
	m_nSearchDirection = CAdoRecordset::searchForward;
}

//////////////////////////////////////////////////////////////////////////////////

CAdoRecordset::CAdoRecordset(CAdoConnection* pAdoConnection)
{
	m_pRecordset = NULL;	
	m_strQuery = "";
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_nEditStatus = CAdoRecordset::dbEditNone;
	m_nSearchDirection = CAdoRecordset::searchForward;

	m_pConnection = pAdoConnection->GetActiveConnection();
}

//////////////////////////////////////////////////////////////////////////////////

CAdoRecordset::~CAdoRecordset()
{
	Close();
	if(m_pRecordset)
		m_pRecordset.Release();	
	m_pRecordset = NULL;	
	m_pRecBinding = NULL;
	m_strQuery = "";
	m_strLastError = "";
	m_dwLastError = 0;
	m_nEditStatus = dbEditNone;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec, int nOption)
{
	Close();
	
	if(strlen(lpstrExec) > 0)
		m_strQuery = lpstrExec;

	BOOL bIsSelect = FALSE;

	try{
		m_pRecordset->CursorType = adOpenStatic;
		m_pRecordset->CursorLocation = adUseClient;
		if(bIsSelect || nOption == openQuery)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenStatic, adLockOptimistic, adCmdUnknown);
		else if(nOption == openTable)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenKeyset, adLockOptimistic, adCmdTable);
		else{
			return false;
		}
	}
	catch(_com_error &e){
		dump_com_error(e);
		return false;
	}

	return m_pRecordset != NULL;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Open(LPCTSTR lpstrExec, int nOption)
{
	if(m_pConnection->GetState() == adStateClosed)
		return false;

	return Open(m_pConnection, lpstrExec, nOption);
}

//////////////////////////////////////////////////////////////////////////////////

_variant_t CAdoRecordset::GetFieldValue(LPCTSTR lpFieldName)
{
	_variant_t vtValue;

	try
	{
		vtValue = m_pRecordset->Fields->GetItem(lpFieldName)->GetValue();
		return vtValue;
	}
	catch(_com_error& e){
		dump_com_error(e);
		return vtValue;
	}
}

//////////////////////////////////////////////////////////////////////////////////

_variant_t CAdoRecordset::GetFieldValue(short snIndex)
{
	_variant_t vtValue;

	try
	{
		vtValue = m_pRecordset->Fields->GetItem(snIndex)->GetValue();
		return vtValue;
	}
	catch(_com_error& e){
		dump_com_error(e);
		return vtValue;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue)
{
	if(m_nEditStatus == dbEditNone)
		return false;
	
	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtValue;
		return true;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;	
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::SetFieldValue(_variant_t vtIndex, _variant_t vtValue)
{
	if(m_nEditStatus == dbEditNone)
		return false;

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtValue;
		return true;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsFieldNull(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsFieldNull(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

DWORD CAdoRecordset::GetRecordCount()
{
	DWORD nRows = 0;

	nRows = m_pRecordset->GetRecordCount();

	if(nRows == -1)
	{
		nRows = 0;
		if(m_pRecordset->EndOfFile != VARIANT_TRUE)
			m_pRecordset->MoveFirst();
		
		while(m_pRecordset->EndOfFile != VARIANT_TRUE)
		{
			nRows++;
			m_pRecordset->MoveNext();
		}
		if(nRows > 0)
			m_pRecordset->MoveFirst();
	}
	
	return nRows;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsOpen()
{
	if(m_pRecordset)
		return m_pRecordset->GetState() != adStateClosed;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::Close()
{
	if(IsOpen())
		m_pRecordset->Close();	
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
	HRESULT hr;
	m_pRecBinding = NULL;

	//Open the binding interface.
	if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pRecBinding )))
	{
		_com_issue_error(hr);
		return false;
	}
	
	//Bind the recordset to class
	if(FAILED(hr = m_pRecBinding->BindToRecordset(&pAdoRecordBinding)))
	{
		_com_issue_error(hr);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
	FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

	return GetFieldInfo(pField, fldInfo);
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

	return GetFieldInfo(pField, fldInfo);
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo)
{
	memset(fldInfo, 0, sizeof(CADOFieldInfo));

	strcpy(fldInfo->m_strName, (LPCTSTR)pField->GetName());
	fldInfo->m_lDefinedSize = pField->GetDefinedSize();
	fldInfo->m_nType = pField->GetType();
	fldInfo->m_lAttributes = pField->GetAttributes();
	if(!IsEof())
		fldInfo->m_lSize = pField->GetActualSize();
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::GetChunk(LPCTSTR lpFieldName, LPCTSTR& strValue)
{
	//LPCTSTR str = "";
	//long lngSize, lngOffSet = 0;
	//_variant_t varChunk;    
	//int ChunkSize = 100;

	//lngSize = m_pRecordset->Fields->GetItem(lpFieldName)->ActualSize;
	//	
	//while(lngOffSet < lngSize)
	//{
	//	varChunk = m_pRecordset->Fields->GetItem(lpFieldName)->GetChunk(ChunkSize);
	//	str += varChunk.bstrVal;
	//	lngOffSet += ChunkSize;
	//}

	//lngOffSet = 0;
	//strValue = str;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR CAdoRecordset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
	_bstr_t varOutput;
	_bstr_t varNull("");
	_bstr_t varCols("\t");
	_bstr_t varRows("\r");

	if(strlen(lpCols) != 0)
		varCols = _bstr_t(lpCols);

	if(strlen(lpRows) != 0)
		varRows = _bstr_t(lpRows);
	
	if(numRows == 0)
		numRows =(long)GetRecordCount();			
			
	varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

	return (LPCTSTR)varOutput;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::Edit()
{
	m_nEditStatus = dbEdit;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::AddNew()
{
	m_nEditStatus = dbEditNone;
	if(m_pRecordset->AddNew() != S_OK)
		return false;

	m_nEditStatus = dbEditNew;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::AddNew(CADORecordBinding &pAdoRecordBinding)
{
	try
	{
		if(m_pRecBinding->AddNew(&pAdoRecordBinding) != S_OK)
		{
			return false;
		}
		else
		{
			m_pRecBinding->Update(&pAdoRecordBinding);
			return true;
		}
			
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}	
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Update()
{
	bool bret = true;

	if(m_nEditStatus != dbEditNone)
	{
		try
		{
			if(m_pRecordset->Update() != S_OK)
				bret = false;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			bret = false;
		}
	}

	m_nEditStatus = dbEditNone;
	return bret;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::CancelUpdate()
{
	m_pRecordset->CancelUpdate();
	m_nEditStatus = dbEditNone;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::SetBookmark()
{
	if(m_varBookmark.vt != VT_EMPTY)
	{
		m_pRecordset->Bookmark = m_varBookmark;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Delete()
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return false;

	if(m_pRecordset->Update() != S_OK)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Find(LPCTSTR lpFind, int nSearchDirection)
{
	m_strFind = lpFind;
	m_nSearchDirection = nSearchDirection;

	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else if(m_nSearchDirection == searchBackward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		m_nSearchDirection = searchForward;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::FindFirst(LPCTSTR lpFind)
{
	m_pRecordset->MoveFirst();
	return Find(lpFind);
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::FindNext()
{
	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind );
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::Clone(CAdoRecordset &pRs)
{
	try
	{
		pRs.m_pRecordset = m_pRecordset->Clone(adLockUnspecified);
		return true;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::SetFilter(LPCTSTR strFilter)
{
	try
	{
		m_pRecordset->PutFilter(strFilter);
		return true;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR CAdoRecordset::GetQuery()
{
	return m_strQuery;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::SetQuery(LPCSTR strQuery)
{
	m_strQuery = strQuery;
}

//////////////////////////////////////////////////////////////////////////////////

long CAdoRecordset::GetFieldCount()
{
	return m_pRecordset->Fields->GetCount();
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsEof()
{
	return m_pRecordset->EndOfFile == VARIANT_TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsEOF()
{
	return m_pRecordset->EndOfFile == VARIANT_TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsBof()
{
	return m_pRecordset->BOF == VARIANT_TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsBOF()
{
	return m_pRecordset->BOF == VARIANT_TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::MoveFirst()
{
	m_pRecordset->MoveFirst();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::MoveNext() 
{
	m_pRecordset->MoveNext();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::MovePrevious() 
{
	m_pRecordset->MovePrevious();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::MoveLast() 
{
	m_pRecordset->MoveLast();
}

//////////////////////////////////////////////////////////////////////////////////

long CAdoRecordset::GetAbsolutePage()
{
	return m_pRecordset->GetAbsolutePage();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::SetAbsolutePage(int nPage)
{
	m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);
}

//////////////////////////////////////////////////////////////////////////////////

long CAdoRecordset::GetPageCount()
{
	return m_pRecordset->GetPageCount();
}

//////////////////////////////////////////////////////////////////////////////////

long CAdoRecordset::GetPageSize()
{
	return m_pRecordset->GetPageSize();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::SetPageSize(int nSize)
{
	m_pRecordset->PutPageSize(nSize);
}

//////////////////////////////////////////////////////////////////////////////////

long CAdoRecordset::GetAbsolutePosition()
{
	return m_pRecordset->GetAbsolutePosition();
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::SetAbsolutePosition(int nPosition)
{
	m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR CAdoRecordset::GetLastErrorString() 
{
	return m_strLastError;
}

//////////////////////////////////////////////////////////////////////////////////

DWORD CAdoRecordset::GetLastError()
{
	return m_dwLastError;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::GetBookmark()
{
	m_varBookmark = m_pRecordset->Bookmark;
}

//////////////////////////////////////////////////////////////////////////////////

bool CAdoRecordset::IsConnectionOpen()
{
	return m_pConnection->GetState() != adStateClosed;
}

//////////////////////////////////////////////////////////////////////////////////

_RecordsetPtr CAdoRecordset::GetRecordset()
{
	return m_pRecordset;
}

//////////////////////////////////////////////////////////////////////////////////

void CAdoRecordset::dump_com_error(_com_error& e)
{
	_bstr_t bstrSource		(e.Source());
	_bstr_t bstrDescription	(e.Description());
	char	szSource[256] = {0,};
	char	szDescription[512] = {0,};
	char	szBuf[1024] = {0,};

	strncpy(szSource, bstrSource, 256);
	strncpy(szDescription, bstrDescription, 512);
	
	sprintf(szBuf, "meaning = %lx\nSource = %s\nDescription = %s", 
		szSource, szDescription);

	m_LogWriter.PrintF("AdoError.Log", "Code = %lx\n%s", 
		e.Error(), szBuf );
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR IntToStr(int nVal)
{
	LPCTSTR strRet;
	char buff[10];
	
	itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR LongToStr(long lVal)
{
	LPCTSTR strRet;
	char buff[20];
	
	ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR DblToStr(double dblVal)
{
	LPCTSTR strRet;
	char buff[50];

   _gcvt(dblVal, 10, buff);
	strRet = buff;
	return strRet;
}

//////////////////////////////////////////////////////////////////////////////////

LPCTSTR DblToStr(float fltVal)
{
	LPCTSTR strRet = "";
	char buff[50];
	
   _gcvt(fltVal, 10, buff);
	strRet = buff;
	return strRet;
}

//////////////////////////////////////////////////////////////////////////////////