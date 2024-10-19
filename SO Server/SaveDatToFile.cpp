#include "SaveDatToFile.h"
#include <stdio.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	데이타를 화일에 저장한다.
//	수정 일자	:	2002-05-13 오전 10:35:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltSaveDatToFile::SaveDatToFile(CHAR *pszFileName, CHAR *pDat, DWORD dwSize)
{	
	DWORD	dwCount;
	CHAR	szFileName[1024];
	FILE	*fp;

	if((fp = fopen("SaveDatToFile Enable.txt", "r")) != NULL)	fclose(fp);
	else														return	FALSE;

	dwCount	=	0;
	
	while(1)
	{
		sprintf(szFileName, "%s : [%s] : [%06d]", pszFileName, dwCount);

		if((fp = fopen(szFileName, "r")) == NULL)
		{
			// 아직 화일이 없다.
			
			// 화일에 저장한다.
			if((fp = fopen(szFileName, "wb")) == NULL)
			{
				fwrite(pDat, dwSize, 1, fp);
				fclose(fp);
				break;
			}			
		}
		else
		{
			// 이미 그 화일은 있다.
			// 닫는다.
			fclose(fp);
		}

		dwCount++;
	}

	return	TRUE;
}
