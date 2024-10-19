#ifndef	HQFILEMANAGER_HEADER
#define HQFILEMANAGER_HEADER

#include <windows.h>
#include <Directives.h>

#define	HQFILE_FILE_MAX_STRING		256

struct FILEHEADER
{
	char	szFileName[256];
	char	szDirectory[256];
	UINT	FileSize;
	UINT	FIlePos;
};

struct FILEINFOHEAD
{
	CHAR	Sort;
	UINT	HeadPos;
};

class HQFileManager  
{
public:
	FILEHEADER		*pHead;
private:
	
	UINT			TotalFileNum;	

	FILE			*pFileHead;
	FILE			*pFileData;

public:		
	HQFileManager();
	virtual ~HQFileManager();

	FILE			*GetData( char *FileName );
	BOOL			HqFileOpen( char *pFileNameHead, char *pFileNameData );
	VOID			DeCode( char *pCode );
	VOID			FileDeCode( char *pFileNameHead, char *pFileNameData );
	BOOL			Createdirectory(char *dir, UINT index, BOOL bDir = FALSE );

	VOID			Free();
};


#endif