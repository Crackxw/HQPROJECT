

#include "stdafx.h"
#include "DBJobRecord.h"

//CDBJobRecord::CDBJobRecord()
//{
//}
//
//CDBJobRecord::~CDBJobRecord()
//{
//}
//
//void CDBJobRecord::Initialize()
//{
//}
//
//void CDBJobRecord::Finalize()
//{
//}
//
//bool CDBJobRecord::Open()
//{
////	m_Config.SetConfigFileName( DEFAULT_CONFIG_FILE );
////	LPCTSTR strGMSDBConnString = m_Config.GetGMSDBConnectionString();
//
//	if( !m_GMSDBConn.Open( "Provider=SQLOLEDB;Password=tantra#007;User ID=SA;Initial Catalog=Tantra_GMS_Data;Data Source=61.254.146.11" ) ){
//		MessageBox( NULL, "GMS DB connection failed", "Error", MB_OK );
//		return false;
//	}	
//	return true;
//}
//
//bool CDBJobRecord::StrinagValidation( char* str )
//{
//	int nLen = (int)strlen(str);
//
//	for(int nCount=0 ; nCount < nLen-1 ; nCount++){
//		if( str[nCount] == '\n' || str[nCount] == '\r' ){
//			str[nCount] = ' ';
//		}
//		//nCh = (int)str[nCount] & 0x000000ff;
//		//nRet = isalnum(nCh);
//		//if(nRet != 0)	continue;
//
//		//if( nCh == 0x0000000D /*\n*/){
//		//	str[nCount] = '';
//		//	str[nCount+1] =''; 
//		//	continue;
//		//}
//	}
//
//	return true;
//}
//
//bool CDBJobRecord::InsertJobLog( int nJobID,
//								 CString strMaker, 
//								 CString strJobSubstance, 
//								 CString strApplyServer )
//{
//	if( m_GMSDBConn.IsOpen() == false )
//		return false;
//
//	char szQuery[12000];
//	memset( szQuery, 0, sizeof( szQuery ) );
//
//	char szSubstance[JOB_SUBSTANCE_MAX_SIZE];
//	memset( szSubstance, 0, sizeof(szSubstance));
//	strncpy( szSubstance, strJobSubstance.GetBuffer(0), JOB_SUBSTANCE_MAX_SIZE );
//
//	StrinagValidation(szSubstance);
//
//	sprintf( szQuery, "Insert Into tblPatchInfo(JobID,Maker,JobSubstance,ApplyServer) \
//					  Values(%d,'%s','%s','%s')", 
//					    nJobID,
//						strMaker.GetBuffer(0), 
//						szSubstance,
//						strApplyServer.GetBuffer(0) );
//
//	CADORecordset cRecordset( &m_GMSDBConn );
//
//	bool b = cRecordset.Open( szQuery, CADORecordset::openQuery );
//
//	return b;
//}
//
//bool CDBJobRecord::UpdateJobLog( int nJobID, 
//								 CString strJobSubstance )
//{
//	if( m_GMSDBConn.IsOpen() == false )
//		return false;
//
//	char szQuery[12000];
//	memset( szQuery, 0, sizeof( szQuery ) );
//
//	sprintf( szQuery, "Update tlbPatchInfo Set JobSubstance='%s' where %d=JobID",						
//						strJobSubstance.GetBuffer(0), 
//						nJobID );
//
//	CADORecordset cRecordset( &m_GMSDBConn );
//
//	bool b = cRecordset.Open( szQuery, CADORecordset::openQuery );
//
//	return b;
//}
//
//bool CDBJobRecord::GetSubstanceLog( BYTE bySearchType, 
//								    S_PATCH_INFO pData[JOB_SEARCH_MAX_SIZE], 
//									char szMaker[JOB_MAKDER_MAX_SIZE], 
//								    SYSTEMTIME sStartTime, 
//									SYSTEMTIME sEndTime, 
//									int& nCount )
//{
//	char szQuery[2048] = {0,};
//	nCount = 0;
//
//	if( bySearchType == JOB_MAKER_SEARCH_TYPE ){ // maker search.
//		sprintf( szQuery, "Select top 50 JobID,Maker,JobSubstance,ApplyServer,DT From tblPatchInfo \
//						  Where ('%s' = Maker) and \
//						(year(DT) Between %d And %d) and (month(DT) Between %d AND %d) and (day(DT) Between %d and %d) \
//						order by DT asc",
//						szMaker, 
//						sStartTime.wYear, sEndTime.wYear, 
//						sStartTime.wMonth, sEndTime.wMonth, 
//						sStartTime.wDay, sEndTime.wDay );
//	}
//	else{ // date search
//		sprintf( szQuery, "Select top 50 JobID,Maker,JobSubstance,ApplyServer,DT From tblPatchInfo \
//						  Where (year(DT) Between %d And %d) and (month(DT) Between %d AND %d) and (day(DT) Between %d and %d) \
//						  order by DT asc",
//						  sStartTime.wYear, sEndTime.wYear, 
//						  sStartTime.wMonth, sEndTime.wMonth, 
//						  sStartTime.wDay, sEndTime.wDay );
//	}
//
//	CADORecordset cRecordset( &m_GMSDBConn );
//	bool b = cRecordset.Open( szQuery, CADORecordset::openQuery );
//
//	if( b == false ) return false;
//
//	S_PATCH_INFO sSubstance;
//	while( cRecordset.IsEof() == false ){
//		memset( &sSubstance, 0, sizeof( S_PATCH_INFO ) );
//		sSubstance.nJobID = (int)cRecordset.GetFieldValue( "JobID" );
//		cRecordset.GetFieldValue( "Maker", sSubstance.szMaker  );
//		cRecordset.GetFieldValue( "ApplyServer", sSubstance.szApplyServer );
//		cRecordset.GetFieldValue( "JobSubstance", sSubstance.szSubstance );
//		cRecordset.GetFieldValue( "DT", sSubstance.szDate );		
//		
//		memcpy( &pData[nCount], &sSubstance, sizeof( S_PATCH_INFO ) );
//
//		++nCount;
//
//		if( nCount >= JOB_SEARCH_MAX_SIZE )
//			break;
//
//		cRecordset.MoveNext();
//	}
//
//	return true;
//}
//
//BYTE CDBJobRecord::DeleteSubstanceLog( int nJobID )
//{
//	char szQuery[2048] = {0,};
//
//	CADORecordset cRecordset( &m_GMSDBConn );
//
//	sprintf( szQuery, "Select top 1 JobID From tblPatchInfo Where %d=JobID", nJobID );
//	bool b = cRecordset.Open( szQuery, CADORecordset::openQuery );
//	if( b == false ) return 0x02;
//
//	if( cRecordset.IsEof() == true ){
//		return 0x01;
//	}
//
//	sprintf( szQuery, "Delete From tblPatchInfo Where %d=JobID", nJobID );
//	b = cRecordset.Open( szQuery, CADORecordset::openQuery );
//
//	return b == true ? 0x00 : 0x02;
//}



