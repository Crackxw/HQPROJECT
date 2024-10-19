//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ��Ʈ�� �ļ� ���̽�
//	File Name		: OnlineCommonParser.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�����Ͱ� ÷�ε� ����Ʈ ������ �Ľ��� �� �� �� �ִ� Ŭ�����Դϴ�.
//		�Ľ� ������ �������� �Ǿ����� ��� �ս���(?) �����͸� �̾Ƴ� �� �ֽ��ϴ�.
//		�ϴ� �� �ļ��� ������ �ڷ��� ������ ����, �÷��� ������ ���е˴ϴ�.
//		������� ����, �Ǹ��ִ� ��, �ּ�(;) ���� ���õǸ�, �׻� ������ ������ ���۵Ǳ� ������ �� �ڷ��� �÷���
//		��� �Ǵ����� ���� �÷� ���� ��ɾ�(#)�� �ݵ�� �����ؾ� �մϴ�.
//
//===================================================================================================

#ifndef	__ONLINE_COMMON_PARSER_H__
#define __ONLINE_COMMON_PARSER_H__

#define		DF_VALUE_OCP_BUFFER_SIZE		4096

class OnlineCommonParser
{
private:
	FILE		*m_pFile;				// ���� ������(�ܺο��� ����)

	int			m_iMaxAllLineCount;		// �� ������ ��ü ���� ����
	int			m_iMaxDataLineCount;	// �� ������ ������ ���� ����
	int			m_iMaxColumnCount;		// �� ������ �ִ� �÷� ����

	int			m_iCurrentLineNo;		// ���� ���� ��ȣ(1������ ����)
	int			m_iCurrentColumnNo;		// ���� �÷� ��ȣ(1������ ����)
	
	char		m_pszLineString[DF_VALUE_OCP_BUFFER_SIZE];	// ���� �ϳ��� ���� ��Ʈ���� �����ϴ� ����(�ӽ� ������)
	char		m_pszErrFileName[MAX_PATH];					// ���� ���� ��½� �� �̸�

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