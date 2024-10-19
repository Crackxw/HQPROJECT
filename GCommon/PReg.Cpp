//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <WinError.h>
#include <main.h>
#include "OnlineVersion.h"


extern SI32 g_siAdType;


//---------------------------------------------------------------------------
BOOL GetPath(char *Sp)
{
	HKEY    hKeyResult = 0;
    DWORD   dwDisposition = 0;
    LONG    lResult;
    DWORD   SpSize, CpSize;
	FILE*	fp;

	fp = fopen("TEST.Dat", "r");
	if(!fp)
		lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_ALL_ACCESS, &hKeyResult);

    if(lResult == ERROR_SUCCESS)
    {
    	SpSize = 1024;
        CpSize = 1024;

        RegQueryValueEx(hKeyResult, "Path", 0, NULL, (UCHAR *)Sp, &SpSize);
    	RegCloseKey(hKeyResult);

        if(strlen(Sp) == 0) return FALSE;

        return TRUE;
    }
    else
    {
    	strcpy(Sp, "");
        return FALSE;
    }
	if(fp)
		fclose(fp);
}

//------------------------------------------------------------------
void SetGameVersion(char Str[])
{
	HKEY hKeyResult;	
    DWORD dwDisposition;

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, "",
       	REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyResult, &dwDisposition);
    RegSetValueEx(hKeyResult, "Version", 0, REG_SZ, (BYTE *)Str, strlen(Str));

    RegCloseKey(hKeyResult);
}

void SetServerVersion()
{
	HKEY hKeyResult;	
    DWORD dwDisposition;
	char Server[16];

	wsprintf( Server, "%d", ON_CLINETVERSION );


	RegCreateKeyEx( HKEY_LOCAL_MACHINE, REG_PATH, 0, "",
       	REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyResult, &dwDisposition);
    RegSetValueEx(hKeyResult, "ServerVersion", 0, REG_SZ, (BYTE *)Server, strlen(Server));

    RegCloseKey(hKeyResult);

}

//-----------------------------------------------------------------
BOOL GetGameVersion(char Str[])
{
    HKEY    hKeyResult = 0;
    LONG    lResult;
    DWORD   VerSize;
	FILE*	fp;

	fp = fopen("TEST.Dat", "r");

	if(!fp)
		lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_ALL_ACCESS, &hKeyResult);

    if(lResult == ERROR_SUCCESS)
    {
        RegQueryValueEx(hKeyResult, "Version", 0, NULL, (UCHAR *)Str, &VerSize);

    	RegCloseKey(hKeyResult);

        if(strlen(Str) == 0) return FALSE;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
	fclose(fp);
}


SI32 GetAdType()
{
    HKEY    hKeyResult = 0;
    LONG    lResult;
	DWORD	Temp;
	CHAR	Str[128];
	ZeroMemory(Str, sizeof(Str));
	SI32	retVal=0;

    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_ALL_ACCESS, &hKeyResult);

    if(lResult == ERROR_SUCCESS)
    {
		// 매직 스테이션 인가 본다.
		lResult = RegQueryValueEx(hKeyResult, "Ad_Type", 0, NULL, (UCHAR *)Str, &Temp);

		if( ERROR_SUCCESS == lResult )
		{
			if( strstr(Str, "MagicStation") )
			{
				retVal = REG_MAGICSTATION;
			}
		}

    	RegCloseKey(hKeyResult);

        if(strlen(Str) == 0) return 0;

        return retVal;
    }
    else
    {
    	RegCloseKey(hKeyResult);
        return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Description	:	필요없는 파일을 지운다.
BOOL	DeleteData()
{
	HANDLE					hFind;
	WIN32_FIND_DATA			lpfd;
	CHAR					cFileName[1024], cTempFileName[1024];
	const SI32				siFolderNum = 6;
	SI32					i;

	CHAR cFolderName[siFolderNum][1024]	=
	{
		".\\Online\\JohapChar\\COF\\KMA\\",
		".\\Online\\JohapChar\\COF\\KWO\\",
		".\\Online\\JohapChar\\COF\\JMA\\",
		".\\Online\\JohapChar\\COF\\JWO\\",
		".\\Online\\JohapChar\\COF\\CMA\\",
		".\\Online\\JohapChar\\COF\\CWO\\"
	};


	for( i=0; i<siFolderNum; i++ )
	{
		// 현재 경로의 모든 COF 파일들을 검사한다.
		wsprintf(cFileName, "%s*.COF", cFolderName[i]);

		ZeroMemory(&lpfd, sizeof(WIN32_FIND_DATA));
		hFind = FindFirstFile(cFileName, &lpfd);

		if( hFind != INVALID_HANDLE_VALUE )
		{
			while( TRUE )
			{
				switch( lpfd.dwFileAttributes )
				{
				case FILE_ATTRIBUTE_ARCHIVE:
					wsprintf(cTempFileName, "%s%s", cFolderName[i], lpfd.cFileName);
					DeleteFile(cTempFileName);
					break;
				}
				if( FindNextFile(hFind, &lpfd) == 0 ) break;
			}

			FindClose(hFind);
		}
	}

	return TRUE;
}




