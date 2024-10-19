
#include "Text.h"
#include <stdio.h>

BOOL CText::Init()
{
	FILE	*fp =	NULL;
	SI32	siTextCounter;
	CHAR	szTextBuffer[1024];
	BOOL	bRet = TRUE;	
	CHAR	szNumber[1024];	
	CHAR	szFileName[512];
	CHAR	szFileName1[512];
	CHAR    szFileName2[512];
	CHAR	szFileName3[512];
	CHAR	*pStartString, *pEndString;

	// 각 언어 맞는 파일을 불러 온다.
	HKEY    hKeyResult = 0;    

	sprintf(szFileName, "Data\\Text\\Text.txt");				//Text
	sprintf(szFileName1, "Data\\Text\\Quest_Text.txt");			//QuestText	
	sprintf(szFileName2, "Data\\Text\\HelpText.txt");			//HelpText
	sprintf(szFileName3, "Data\\Text\\Online\\Korean.txt");

	// 우선 텍스트 ID 화일을 불러온다.
	siTextCounter = 0;
	if((fp = fopen(szFileName, "rt" )) != NULL)
	{			
		ZeroMemory(szTextBuffer, sizeof(szTextBuffer));					

		while(fgets(szTextBuffer, 1023, fp) != NULL)
		{	
			if(strstr(szTextBuffer, "\"") == NULL) 
				continue;
				
			if(siTextCounter >= ON_MAX_TEXT_NUM)
			{
				bRet = FALSE;
				goto END;
			}							

			ZeroMemory(szNumber, sizeof(szNumber));
			strncpy(szNumber, szTextBuffer, 8);		

			Texts[siTextCounter].uiID	=	atoi(szNumber);	
			

			if(Texts[siTextCounter].uiID == 8000459)
			{
				int a=0;
			}
			

			pStartString	=	strstr(szTextBuffer, "\"");
			pStartString	+=	2;
			pEndString		=	strstr(pStartString, "\"");

			if(pEndString - pStartString >= ON_MAX_TEXT_STRING_NUM)
			{
				bRet = FALSE;
				goto END;
			}
					
			ZeroMemory(Texts[siTextCounter].szText, sizeof(Texts[siTextCounter].szText));
			strncpy(Texts[siTextCounter].szText, pStartString, min(pEndString - pStartString, ON_MAX_TEXT_STRING_NUM - 1));

			if(strlen(Texts[siTextCounter].szText) >= ON_MAX_TEXT_STRING_NUM)
			{
				bRet = FALSE;
				goto END;
			}
			
			siTextCounter++;			
		}

		fclose(fp);
		fp	=	NULL;
	}
	else
	{
		bRet	=	FALSE;

		goto END;
	}

	// Quest Text
	siTextCounter = 0;
	if((fp = fopen(szFileName1, "rt" )) != NULL)
	{			
		ZeroMemory(szTextBuffer, sizeof(szTextBuffer));					

		while(fgets(szTextBuffer, 1023, fp) != NULL)
		{	
			if(strstr(szTextBuffer, "\"") == NULL)
				continue;
				
			if(siTextCounter >= ON_MAX_TEXT_NUM_2048)
			{
				bRet = FALSE;
				goto END;
			}							

			ZeroMemory(szNumber, sizeof(szNumber));
			strncpy(szNumber, szTextBuffer, 8);		

			QuestTexts[siTextCounter].uiID	=	atoi(szNumber);				

			pStartString	=	strstr(szTextBuffer, "\"");
			pStartString	+=	2;
			pEndString		=	strstr(pStartString, "\"");

			if(pEndString - pStartString >= ON_MAX_TEXT_STRING_1024)
			{
				bRet = FALSE;
				goto END;
			}
					
			strncpy(QuestTexts[siTextCounter].szText, pStartString, min(pEndString - pStartString, ON_MAX_TEXT_STRING_1024 - 1));


			if(strlen(QuestTexts[siTextCounter].szText) >= ON_MAX_TEXT_STRING_1024)
			{
				bRet = FALSE;
				goto END;
			}
			
			siTextCounter++;			
		}

		fclose(fp);
		fp	=	NULL;
	}
	else
	{
		bRet	=	FALSE;

		goto END;
	}
	
	//Help Text	
	siTextCounter = 0;
	if((fp = fopen(szFileName2, "rt" )) != NULL)
	{			
		ZeroMemory(szTextBuffer, sizeof(szTextBuffer));					

		while(fgets(szTextBuffer, 1023, fp) != NULL)
		{	
			if(strstr(szTextBuffer, "\"") == NULL)
				continue;
				
			if(siTextCounter >= ON_MAX_TEXT_NUM_2048)
			{
				bRet = FALSE;
				goto END;
			}							

			ZeroMemory(szNumber, sizeof(szNumber));
			strncpy(szNumber, szTextBuffer, 8);		

			HelpTexts[siTextCounter].uiID	=	atoi(szNumber);				

			pStartString	=	strstr(szTextBuffer, "\"");
			pStartString	+=	2;
			pEndString		=	strstr(pStartString, "\"");

			if(pEndString - pStartString >= ON_MAX_TEXT_STRING_1024)
			{
				bRet = FALSE;
				goto END;
			}
					
			strncpy(HelpTexts[siTextCounter].szText, pStartString, min(pEndString - pStartString, ON_MAX_TEXT_STRING_1024 - 1));


			if(strlen(HelpTexts[siTextCounter].szText) >= ON_MAX_TEXT_STRING_1024)
			{
				bRet = FALSE;
				goto END;
			}
			
			siTextCounter++;			
		}

		fclose(fp);
		fp	=	NULL;
	}
	else
	{
		bRet	=	FALSE;

		goto END;
	}


	//미니게임 텍스트	
	siTextCounter = 0;
	if((fp = fopen(szFileName3, "rt" )) != NULL)
	{			
		ZeroMemory(szTextBuffer, sizeof(szTextBuffer));					

		while(fgets(szTextBuffer, 1023, fp) != NULL)
		{	
			if(strstr(szTextBuffer, "\"") == NULL)
				continue;
				
			if(siTextCounter >= ON_MAX_TEXT_MINIGAME_NUM)
			{
				bRet = FALSE;
				goto END;
			}							

			ZeroMemory(szNumber, sizeof(szNumber));
			strncpy(szNumber, szTextBuffer, 4);		

			MiniGameTexts[siTextCounter].uiID	=	atoi(szNumber);				

			pStartString	=	strstr(szTextBuffer, "\"");
			pStartString	+=	2;
			pEndString		=	strstr(pStartString, "\"");

			if(pEndString - pStartString >= ON_MAX_TEXT_STRING_NUM)
			{
				bRet = FALSE;
				goto END;
			}
					
			strncpy(MiniGameTexts[siTextCounter].szText, pStartString, min(pEndString - pStartString, ON_MAX_TEXT_STRING_NUM - 1));


			if(strlen(MiniGameTexts[siTextCounter].szText) >= ON_MAX_TEXT_STRING_NUM)
			{
				bRet = FALSE;
				goto END;
			}
			
			siTextCounter++;			
		}

		fclose(fp);
		fp	=	NULL;
	}
	else
	{
		bRet	=	FALSE;

		goto END;
	}

END:
	if(fp)
	{
		fclose(fp);
		fp	=	NULL;
	}

	sprintf((char*)EmptyText,      "Empty");
	sprintf((char*)QuestEmptyText, "Empty");
	sprintf((char*)EmptyHelpText, "Empty");
	sprintf((char*)EmptyMinigameText, "Empty");

	return bRet;
}