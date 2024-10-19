/*
〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

	프로그래머  : 정진욱
	최초 제작일	: 2003.04.09

〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
*/
#include	<GSL.h>

#include <stdio.h>
#include "bindJXFile.h"
#include "utility.h"
#include "zlib.h"

BindJXFile *BindJXFile::_bindJxFile = NULL;


BindJXFile *BindJXFile::GetInstance()
{
	if( _bindJxFile == NULL ) _bindJxFile = new BindJXFile;
	return _bindJxFile;
}

BOOL BindJXFile::openJX( string filename, char *folder )
{
	_file	= filename;
	string path = filename;

	if( folder )
	{
		_folder = folder;
		path = _folder + "\\" + filename;
	}	

	_JxFile = fopen( path.c_str(), "rb" );

	if( !_JxFile )		return FALSE;
	
	int			nTotal;
	JXHeader	Jx;
	char		file[MAX_PATH];
	unsigned long	size, pos, compress = 0;

	fread( &nTotal, sizeof(int), 1, _JxFile );

	for( int i=0; i<nTotal; i++ )
	{
		fseek( _JxFile, (long)compress, SEEK_CUR );


		fread( file, sizeof(char), MAX_PATH, _JxFile );
		fread( &size, sizeof(unsigned long), 1, _JxFile );
		fread( &pos, sizeof(unsigned long), 1, _JxFile );
		fread( &compress, sizeof(unsigned long), 1, _JxFile );
		

		Jx.create( size, pos );
		Jx.SetCompressSize( compress );
		efficientAddOrUpdata( _headres, file, Jx );		
	}

	return TRUE;
}

BOOL BindJXFile::create( char *folder, string jx_file )
{
	char path[MAX_PATH];
	
	_file	= jx_file;
	_folder = folder;

	sprintf( path, "%s\\*.*", folder );
	findFile( path );

	if( _headres.empty() )	return FALSE;

	makeJxFile(jx_file);
	return TRUE;
}

BYTE *BindJXFile::GetFile( string filename, unsigned long &size )
{
	pair<JX_Header::iterator, JX_Header::iterator > find = _headres.equal_range( filename );		

	if( _stricmp( find.first->first.c_str() ,filename.c_str() ) == 0 )
	{		
		fseek( _JxFile, find.first->second.GetFilePos() + MAX_PATH + (sizeof(unsigned long) * 3), SEEK_SET );

		if( _pData ) delete []_pData;

		_pData = new BYTE[ find.first->second.GetFileSize() ];
		BYTE *pCompressData = new BYTE[ find.first->second.GetCompress() ];


		unsigned long tempsize = -1;
		

		fread( pCompressData, find.first->second.GetCompress(), 1, _JxFile );

		uncompress( _pData, &tempsize, pCompressData, find.first->second.GetCompress() );

		size = tempsize;

		delete []pCompressData;
		return _pData;
	}

	// 예외 처리
	else
	{
		for( JX_Header::iterator i=_headres.begin(); i!=_headres.end(); ++i )
		{
			if( _stricmp( i->first.c_str(), filename.c_str() ) == 0 )
			{
				fseek( _JxFile, i->second.GetFilePos() + MAX_PATH + (sizeof(unsigned long) * 3), SEEK_SET );
				
				if( _pData ) delete []_pData;


				_pData = new BYTE[ i->second.GetFileSize() ];
				BYTE *pCompressData = new BYTE[ i->second.GetCompress() ];
				
				
				fread( pCompressData, i->second.GetCompress(), 1, _JxFile );

				
				uncompress( _pData, &size, pCompressData, i->second.GetCompress() );
				
				delete []pCompressData;
				return _pData;
			}
		}
	}

	return NULL;
}

FILE *BindJXFile::GetFile( string filename )
{
	unsigned long size = -1;
	
	// actdoll (2004/05/18 13:11) : 긴급에러수정
	//	이전에는 HQ파일의 텍스트를 오픈할 때 게임 루트에 아래의 "JxTemp"파일을 생성하여 임시 보관소로 작업을 하였다.
	//	그러나 이 파일을 이용한 해킹 사례가 발생한 것으로 판단하고 데이터의 저장 형태를 임시 파일 형태로 교체하도록 하겠다.


	char	pszDir[MAX_PATH] = {0,}, pszTemp[MAX_PATH];
	int		iName = 14972238, iRet = GetSystemDirectory( pszDir, MAX_PATH );
	if( pszDir[iRet-1] != '\\' )	{ pszDir[iRet] = '\\'; pszDir[iRet+1] = NULL; }
	for( int i=0; i<iRet; i++ )	{ iName += i; iName += pszDir[i]; }
	itoa( iName, pszTemp, 16 );
	strcat( pszDir, "nt" );
	strcat( pszDir, pszTemp );
	strcat( pszDir, ".drv" );

	FILE *fp	= fopen( pszDir, "wb" );
	BYTE *pData	= GetFile( filename, size );	
	fwrite( pData, size, 1, fp );
	fclose( fp );
	
	return fopen( pszDir, "rb" );
}

void BindJXFile::Restoration()
{	
	for( JX_Header::iterator i = _headres.begin(); i != _headres.end(); ++i )
	{	
		string path = i->first;
		if( !_folder.empty() )	path = _folder + "\\" + i->first;

		FILE *fp = fopen( path.c_str(), "wb" );		

		if( fp == NULL )	
		{
			createFolder( i->first );
			fp = fopen( i->first.c_str(), "wb" );
		}

		unsigned long size;

		fwrite( GetFile( i->first, size ), i->second.GetFileSize(), 1, fp );
		fclose(fp);
	}
}

//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
void BindJXFile::destroy()
{	
	if( _pData )	delete []_pData;
	if( _JxFile )
	{
		fclose( _JxFile );
		_JxFile = NULL;
	}

	_headres.clear();	
	
	// actdoll (2004/05/18 13:11) : 긴급에러수정
	//	이전에는 HQ파일의 텍스트를 오픈할 때 게임 루트에 아래의 "JxTemp"파일을 생성하여 임시 보관소로 작업을 하였다.
	//	그러나 이 파일을 이용한 해킹 사례가 발생한 것으로 판단하고 데이터의 저장 형태를 임시 파일 형태로 교체하도록 하겠다.

	char	pszDir[MAX_PATH] = {0,}, pszTemp[MAX_PATH];
	int		iName = 14972238, iRet = GetSystemDirectory( pszDir, MAX_PATH );
	if( pszDir[iRet-1] != '\\' )	{ pszDir[iRet] = '\\'; pszDir[iRet+1] = NULL; }
	for( int i=0; i<iRet; i++ )	{ iName += i; iName += pszDir[i]; }
	itoa( iName, pszTemp, 16 );
	strcat( pszDir, "nt" );
	strcat( pszDir, pszTemp );
	strcat( pszDir, ".drv" );

	FILE *fp = fopen( pszDir, "r" );
	if( fp )
	{
		fclose(fp);
		DeleteFile( pszDir );
	}

	if( _bindJxFile )
	{
		delete _bindJxFile;
		_bindJxFile = NULL; 
	}
}

void BindJXFile::findFile( char *path )
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;	
	char szDir[ MAX_PATH ], drive[ _MAX_DRIVE ], newpath[ MAX_PATH ];
	BOOL bResult = TRUE;		

	hSrch = FindFirstFile( path, &wfd );

	while(bResult)
	{
		_splitpath( path, drive, szDir, NULL, NULL );

		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( wfd.cFileName[0] != '.' )
			{
				sprintf( newpath, "%s%s%s\\*.*", drive, szDir, wfd.cFileName );
				findFile( newpath );
			}
		}
		else
		{
			sprintf( newpath, "%s%s%s", drive, szDir, wfd.cFileName );

			// actdoll (2004/05/18 13:11) : 긴급에러수정
			//	이전에는 HQ파일의 텍스트를 오픈할 때 게임 루트에 아래의 "JxTemp"파일을 생성하여 임시 보관소로 작업을 하였다.
			//	그러나 이 파일을 이용한 해킹 사례가 발생한 것으로 판단하고 데이터의 저장 형태를 임시 파일 형태로 교체하도록 하겠다.

			char	pszDir[MAX_PATH] = {0,}, pszTemp[MAX_PATH];
			int		iName = 14972238, iRet = GetSystemDirectory( pszDir, MAX_PATH );
			if( pszDir[iRet-1] != '\\' )	{ pszDir[iRet] = '\\'; pszDir[iRet+1] = NULL; }
			for( int i=0; i<iRet; i++ )	{ iName += i; iName += pszDir[i]; }
			itoa( iName, pszTemp, 16 );
			strcat( pszDir, "nt" );
			strcat( pszDir, pszTemp );
			strcat( pszDir, ".drv" );



			if( strcmp( wfd.cFileName, "zlib.dll" ) != 0 && strcmp( wfd.cFileName, "HQFile.exe" ) != 0 && strcmp( wfd.cFileName, _file.c_str() ) != 0 
				&& strcmp( wfd.cFileName, pszDir ) != 0 )
			{
				JXHeader jx;

				if( strstr( newpath, (char*)_folder.c_str() ) != 0 )
					memmove( newpath, &newpath[ _folder.size() + 1 ], strlen( newpath ) - _folder.size() + 1 );

				efficientAddOrUpdata( _headres, newpath, jx );
			}
		}

		bResult = FindNextFile( hSrch, &wfd );
	}

	FindClose( hSrch );
}

void BindJXFile::makeJxFile( string &filename )
{
	string path = _folder + "\\" + filename;

	_JxFile = fopen( path.c_str(), "wb" );

	int nTotalFileNum = _headres.size();
	fwrite( &nTotalFileNum, sizeof(int), 1, _JxFile );


	for( JX_Header::iterator i = _headres.begin(); i != _headres.end(); ++i )
		makeJxFile_writeToFile(i);

	fclose( _JxFile );
}

void BindJXFile::makeJxFile_writeToFile(JX_Header::iterator header)
{
	FILE *fp;

	string path = _folder + "\\" + header->first;

	if( fp = fopen( path.c_str(), "rb" ) )
	{
		header->second.create( getTotalFileSize(fp), ftell(_JxFile) );
		
		BYTE *pData		= new BYTE[ header->second.GetFileSize() * 2 ];
		fread( pData, header->second.GetFileSize(), 1, fp );


		unsigned long nCompressSize = -1;
		BYTE *pCompress = new BYTE[ header->second.GetFileSize() * 2 ];


		compress( pCompress, &nCompressSize, pData, header->second.GetFileSize() );
		header->second.SetCompressSize(nCompressSize);

		char szTemp[MAX_PATH];
		strcpy( szTemp, header->first.c_str() );
		fwrite( szTemp, sizeof(char), MAX_PATH, _JxFile );					// 파일 이름
		fwrite( &header->second.GetFileSize(), sizeof(unsigned long), 1, _JxFile );			// 파일 크기
		fwrite( &header->second.GetFilePos(), sizeof(unsigned long), 1, _JxFile );			// JX에서 위치
		fwrite( &header->second.GetCompress(), sizeof(unsigned long), 1, _JxFile );
		fwrite( pCompress, nCompressSize, 1, _JxFile );										// 데이타

		delete []pData;
		delete []pCompress;
		fclose( fp );
	}
}

long BindJXFile::getTotalFileSize( FILE *fp )
{
	fseek( fp, 0, SEEK_SET );
	fseek( fp, 0, SEEK_END );
	
	long nResult = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	return nResult;
}

void BindJXFile::createFolder( string name )
{
	char temp[128], addPath[128];

	strcpy( temp, name.c_str() );
	ZeroMemory( addPath, sizeof(addPath));

	char *pToken = strtok( temp, "\"\\" );

	while( pToken )
	{
		if( addPath[0] != 0 )
			strcat( addPath, "\\" );

		strcat( addPath, pToken );

		CreateDirectory ( addPath, NULL );
		pToken = strtok( NULL, "\"\\" );

		if( pToken == NULL )				return;
		if( strstr( pToken, "." ) != NULL )	return;
	}
	
}
