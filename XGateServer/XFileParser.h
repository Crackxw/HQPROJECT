#ifndef _XFILEPARSER_H
#define _XFILEPARSER_H

#include <windows.h>

class XFileParser
{
public:
	XFileParser();
	~XFileParser();

public:
	BOOL	Create( char *szFileName, int bufsize );
	void	Close();

	void	ReadData( char *szKey, char *szData );

public:
	char	*m_pBuf;

};


#endif