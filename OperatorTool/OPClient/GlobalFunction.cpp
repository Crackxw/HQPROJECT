#include "stdafx.h"
#include "OPClient.h"

#include "Graphics16.h"
#include "OPClientDlg.h"
#include "TextDlg.h"

#include "..\\common\\defines.h"



BOOL loadspr(char *file, XSPR &xspr)
{
	char szTemp[MAX_PATH];

	sprintf( szTemp, "%s\\%s", "\\\\220.85.18.3\\c$\\������\\�Ż�\\Online\\Item", file );

	if(xspr.Image)	delete []xspr.Image;

	if( file == NULL )	return FALSE;
	int version;
	FILE *fp;

	fp=fopen(szTemp, "rb");
	if(!fp)
	{
		return FALSE;
	}

	fread(&version, 4, 1, fp);
	if(version!=XSPR_VERSION)
	{
		return FALSE;
	}

    fseek(fp, 0, SEEK_SET);

    fread(&xspr.Header, sizeof(XSPR_HEADER), 1, fp);
    xspr.Image=new unsigned char[xspr.Header.TotalLength];

    if(!xspr.Image)
    {
		return FALSE;
    }

    fread(xspr.Image, xspr.Header.TotalLength, 1, fp);

    fclose(fp);
	
	return TRUE;
}

void SetFileName(char *pFileName)
{
	SYSTEMTIME	Time;

	char Temp[128];
	char Ext[128];

	sprintf(Ext, &pFileName[strlen(pFileName)-3]);

	GetSystemTime(&Time);

	sprintf(Temp, "%s_%d��-%d��-%d��-%d��.%s", pFileName, Time.wMonth, Time.wDay, Time.wHour-3, Time.wMinute, Ext);
	
	rename(pFileName, Temp);
}