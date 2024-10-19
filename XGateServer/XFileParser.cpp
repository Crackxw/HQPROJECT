#include "XFileParser.h"

XFileParser::XFileParser()
{
	m_pBuf = NULL;
}

XFileParser::~XFileParser()
{
	Close();
}

BOOL XFileParser::Create( char *szFileName, int bufsize )
{
	HANDLE hFile;
	DWORD dwFileSize;
	DWORD dwBytesRead;

	hFile = CreateFile( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	
	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize <= 0 ) { CloseHandle( hFile ); return FALSE; }

	if( m_pBuf ) delete [] m_pBuf;

	// 버퍼 생성
	m_pBuf = new char[ dwFileSize ];

	// 파일 전체를 버퍼로 읽어온다
	ReadFile( hFile, m_pBuf, dwFileSize, &dwBytesRead, NULL );

	// 파일을 닫는다
	CloseHandle( hFile );

	if( dwFileSize != dwBytesRead ) {
		delete [] m_pBuf;
		m_pBuf = NULL;
		return FALSE;
	}

	return TRUE;
}

void XFileParser::Close()
{

	if( m_pBuf ) delete [] m_pBuf;
	m_pBuf = NULL;
}

void XFileParser::ReadData( char *szKey, char *szData )
{
	
}

