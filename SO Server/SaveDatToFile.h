#ifndef	SAVEDAT_TO_FILE_HEADER
#define SAVEDAT_TO_FILE_HEADER

#include <windows.h>

class	cltSaveDatToFile
{
public:
	BOOL	SaveDatToFile(CHAR *pszFileName, CHAR *pDat, DWORD dwSize);			// ����Ÿ�� ȭ�Ͽ� �����Ѵ�.
};

#endif
