//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 스트링 파서 베이스
//	File Name		: OnlineCommonParser.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		데이터가 첨부된 리스트 파일을 파싱할 때 쓸 수 있는 클래스입니다.
//		파싱 기준이 공통으로 되어있을 경우 손쉽게(?) 데이터를 뽑아낼 수 있습니다.
//		일단 이 파서의 기준은 자료의 라인은 엔터, 컬럼은 탭으로 구분됩니다.
//		공백란의 엔터, 탭만있는 줄, 주석(;) 등은 무시되며, 항상 데이터 라인이 시작되기 전에는 이 자료의 컬럼이
//		몇개나 되는지에 대한 컬럼 공지 명령어(#)가 반드시 존재해야 합니다.
//
//===================================================================================================

#ifndef	__ONLINE_COMMON_PARSER_H__
#define __ONLINE_COMMON_PARSER_H__

#define		DF_VALUE_OCP_BUFFER_SIZE		4096

class OnlineCommonParser
{
private:
	FILE		*m_pFile;				// 파일 포인터(외부에서 물림)

	int			m_iMaxAllLineCount;		// 이 파일의 전체 라인 갯수
	int			m_iMaxDataLineCount;	// 이 파일의 데이터 라인 갯수
	int			m_iMaxColumnCount;		// 이 파일의 최대 컬럼 갯수

	int			m_iCurrentLineNo;		// 현재 라인 번호(1번부터 시작)
	int			m_iCurrentColumnNo;		// 현재 컬럼 번호(1번부터 시작)
	
	char		m_pszLineString[DF_VALUE_OCP_BUFFER_SIZE];	// 라인 하나에 대한 스트링을 포함하는 버퍼(임시 데이터)
	char		m_pszErrFileName[MAX_PATH];					// 에러 파일 출력시 그 이름

public:
	
	OnlineCommonParser		( void );
	~OnlineCommonParser		( void );

	BOOL	Initialize		( FILE *pFile, int &iRetErrLineNo, BOOL &bRetColumnLack );
	void	Release			( void );

	int		ParseDataLine	( void );
	BOOL	GetValue		( char		*pszRecvBuff,	int iRecvBuffSize,	int &iLineNo, int &iColumnNo );
	BOOL	GetValue		( char		&chRecvBuff,						int &iLineNo, int &iColumnNo );
	BOOL	GetValue		( short		&shRecvBuff,						int &iLineNo, int &iColumnNo );
	BOOL	GetValue		( int		&iRecvBuff,							int &iLineNo, int &iColumnNo );
	BOOL	GetValue		( __int64	&i64RecvBuff,						int &iLineNo, int &iColumnNo );
	BOOL	GetValue		( double	&fRecvBuff,							int &iLineNo, int &iColumnNo );

	int		GetMaxAllLineCount	( void )		{ return m_iMaxAllLineCount; }
	int		GetMaxDataLineCount	( void )		{ return m_iMaxDataLineCount; }
	int		GetMaxColumnCount	( void )		{ return m_iMaxColumnCount; }
	int		GetCurrentLineNo	( void )		{ return m_iCurrentLineNo; }
	int		GetCurrentColumnNo	( void )		{ return m_iCurrentColumnNo; }

	BOOL	IsEndOfLine			( void )		{ return ( m_iMaxAllLineCount == m_iCurrentLineNo ); }
	BOOL	IsEndOfColumn		( void )		{ return ( m_iMaxColumnCount == m_iCurrentColumnNo ); }

	void	SetErrFileName		( char *pszErrFileName )	{ strcpy( m_pszErrFileName, pszErrFileName ); }
	static	void	PrintError	( char *pszErrString, ... );
};

#endif//__ONLINE_COMMON_PARSER_H__