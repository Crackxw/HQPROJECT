#include "SaveDatToFile.h"
#include <stdio.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����Ÿ�� ȭ�Ͽ� �����Ѵ�.
//	���� ����	:	2002-05-13 ���� 10:35:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			// ���� ȭ���� ����.
			
			// ȭ�Ͽ� �����Ѵ�.
			if((fp = fopen(szFileName, "wb")) == NULL)
			{
				fwrite(pDat, dwSize, 1, fp);
				fclose(fp);
				break;
			}			
		}
		else
		{
			// �̹� �� ȭ���� �ִ�.
			// �ݴ´�.
			fclose(fp);
		}

		dwCount++;
	}

	return	TRUE;
}
