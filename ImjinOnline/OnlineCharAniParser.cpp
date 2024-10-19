#include <Main.h>
#include "OnlineWorld.h"
#include "OnlineCharAniParser.h"
#include <DebugTool.h>
 
#define		ON_READ_CHAR_DIRECTION_INFO_DIRECTION		1
#define		ON_READ_CHAR_DIRECTION_INFO_FONT			2
#define		ON_READ_CHAR_DIRECTION_INFO_PARAMETER		3

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
CharAniParser::CharAniParser()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
CharAniParser::~CharAniParser()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ȭ���� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	CharAniParser::Open(char * filename, cltOnlineWorld *pOnlineWorld)
{
	pMyOnlineWorld	=	pOnlineWorld;
	return HQArielLabelScriptSystem::Open(filename);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ȭ���� �ݴ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void	CharAniParser::Close(void)
{
	HQArielLabelScriptSystem::Close();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ȭ���� �д´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	CharAniParser::Read(OnlineCharsAniInfo *pCharsAniInfo)
{	
	CHAR	szReadBuffer[1024];
	SI32	siBraceNum;	
	SI32	siCurrentParsingChar;
	SI32	siCurrentParsingCharAction;
	SI32	siCurrentParsingCharActionDirection;
	CHAR	szCharActionName[32], szFileName[1024];
	CHAR	szDummy1[256], szDummy2[256];
	SI32	i;

	SI32	*ptotalactionnum = NULL;

	// ��ü ĳ���� ���� ���Ѵ�.
	pCharsAniInfo->siTotalCharNum	=	GetTotalCharNum();

	if(pCharsAniInfo->siTotalCharNum == 0)
	{
		clGrp.Error("", "pCharsAniInfo->siTotalCharNum Is Zero [%s]");
		return FALSE;
	}

	// ĳ���͸� �Ҵ� �޴´�.
	pCharsAniInfo->pCharAniInfo		=	new	OnlineCharAniInfo[pCharsAniInfo->siTotalCharNum];

	// �Ҵ� ���� ĳ������ ������ �ʱ�ȭ�� �����ش�.(���δ� ��Ű�� �ʾƵ� �ݵ�� ���־�� �� ������ ���־�� �Ѵ�.
	for(i = 0; i < pCharsAniInfo->siTotalCharNum; i++)	
	{
		// �ʻ�ȭ �̸��� �ʱ�ȭ �����ش�.
		strcpy(pCharsAniInfo->pCharAniInfo[i].szPortraitFileName, "");
		pCharsAniInfo->pCharAniInfo[i].siPortraitFileFont	=	-1;
	}
	

	// ȭ�� �����͸� ó������ ������.
	fseek(fpScript, 0, SEEK_SET);
	
	// �������� ĳ������ ���� ������ ȭ�Ͽ��� �о�´�.
	siCurrentParsingChar	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ���ڿ� �� ���� '\n' �κ��� �߶󳽴�.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)		
		{	
			siBraceNum++;	
			
			switch(siBraceNum)
			{				
			case 1:				
				// ���ο� ĳ���� �̹Ƿ� �Ľ��ϴ� �׼� �ε��� ���� �ʱ�ȭ �Ѵ�.
				siCurrentParsingCharAction = 0;

				// ���ο� ĳ���� �׼� �κ��� �����̴�.
				// �� ��� �׼��� �ִ��� ���´�.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum	=	GetTotalActionNum();				

				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo	=	new OnlineCharActionInfo[pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum];								

				if(siCurrentParsingChar == 0)
					ptotalactionnum	=	&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum;
				break;
			case 2:
				// ���ο� �� �׼��� �ִϸ��̼��� �Ľ��ϴ� �׼� �ε��� ���� �ʱ�ȭ �Ѵ�.
				siCurrentParsingCharActionDirection	=	0;

				// ���ο� ĳ���� �׼� �ִϸ��̼� �κ��� �����̴�.
				// �� ��� �ִϸ��̼��� �ִ��� ���´�.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siDirectionNum				=	GetTotalDirectionNum();
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].pCharActionDirectionInfo		=	
							new OnlineCharActionDirectionInfo[pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siDirectionNum];
				break;
			}
		}
		else if(strstr(szReadBuffer, "}") != NULL)	
		{
			siBraceNum--;				

			switch(siBraceNum)
			{
			case 0:
				// ���� �Ľ��ϴ� ĳ���͸� �ϳ� ������Ų��.
				siCurrentParsingChar++;
				break;
			case 1:
				// ���� �Ľ��ϴ� �׼� �ε��� ���� ������Ų��.
				siCurrentParsingCharAction++;				
				break;
			}
		}
		else
		{
			// siBraceNum�� ������ ���� ��� �κ��� �Ľ��ϴ��� �˾Ƴ� �� �ִ�.		
			switch(siBraceNum)
			{
			case 0:				
				// ���ο� ĳ���͸� �Ľ��Ѵ�. ���ڿ� ID �κ��̴�.				

				// ���ڿ� ID�� ����Ѵ�.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiCharID	=	MAKEWORD(szReadBuffer[0], atoi(&szReadBuffer[1]));				
				break;
			case 1:
				if(strstr(szReadBuffer, "[PORTRAIT]") != NULL)
				{		
					// �ʻ�ȭ �̸��̴�.
					sscanf(szReadBuffer, "%s %s %s %d", szDummy1, szDummy2, 
													pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].szPortraitFileName, 
													&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siPortraitFileFont);
				
				}
				else if(strstr(szReadBuffer, "[RIP_WIDTH]") != NULL)
				{
					// Real Image Percent �̴�.(Width)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiWidthRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHT]") != NULL)
				{
					// Real Image Percent �̴�.(Height)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiHeightRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHTINDENT]") != NULL)
				{
					// Real Image Percent �̴�.(Height INDENT)
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siHeightRIPIndent);					
				}
				else if(strstr(szReadBuffer, "[FRAMEDELAY]") != NULL)
				{
					// Frame Delay�̴�.
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siFrameDelay);
				}
				else
				{
					sscanf(szReadBuffer, "%s %s", szCharActionName, szFileName);										

					// ���ڿ� �׼� �ڵ带 ���ڷ� ��ȯ�Ѵ�.
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siActionCode	=	pMyOnlineWorld->pOnlineCharActionCode->GetCharActionCode(szCharActionName);

					// ���ҽ����� �ش� ȭ���� �ε����� ���´�.				
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siFile		=	 pMyOnlineWorld->pOnlineResource->GetCharFileIndex(szFileName);				
				} 				
				break;
			case 2:
				// �ִϸ��̼� �� ������ �д´�.
				ReadCharActionDirectionInfo(szReadBuffer, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].pCharActionDirectionInfo[siCurrentParsingCharActionDirection]);

				// ���� �Ľ��ϴ� �� �׼��� ���� �ε��� ���� ���� ��Ų��.
				siCurrentParsingCharActionDirection++;				
				break;
			}
		}		
	}	

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ü ĳ���� ���� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	CharAniParser::GetTotalCharNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharNum;

	// ������ ȭ�� �����͸� �����Ѵ�.
	fgetpos(fpScript, &fpos);

	// ȭ�� �����͸� ȭ���� ó������ ������.
	fseek(fpScript, 0, SEEK_SET);

	siTotalCharNum	=	0;
	siBraceNum		=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ���ڿ� �� ���� '\n' �κ��� �߶󳽴�.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)	siBraceNum--;
		else
		{
			if(siBraceNum == 0)
				siTotalCharNum++;
		}
	}

	// ������ ȭ�� �����ͷ� ���� ��Ų��.
	fsetpos(fpScript, &fpos);

	return	siTotalCharNum;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ü �׼� ���� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	CharAniParser::GetTotalActionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharActionNum;

	// ������ ȭ�� �����͸� �����Ѵ�.
	fgetpos(fpScript, &fpos);	

	siTotalCharActionNum	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ���ڿ� �� ���� '\n' �κ��� �߶󳽴�.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

			if(siBraceNum == -1)			break;					// ĳ������ �׼� �κ��� ��� �� �о���.
			else if(siBraceNum == 0)		siTotalCharActionNum++;

		}		
	}

	// ������ ȭ�� �����ͷ� ���� ��Ų��.
	fsetpos(fpScript, &fpos);

	return	siTotalCharActionNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ü ���� ���� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	CharAniParser::GetTotalDirectionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalDirectionNum;

	// ������ ȭ�� �����͸� �����Ѵ�.
	fgetpos(fpScript, &fpos);

	siTotalDirectionNum	=	0;
	siBraceNum			=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ���ڿ� �� ���� '\n' �κ��� �߶󳽴�.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

		}
		else
		{
			if(siBraceNum == -1)			break;					// ĳ������ �׼� �κ��� ��� �� �о���.
			else	if(siBraceNum == 0)		siTotalDirectionNum++;
		}		
	}

	// ������ ȭ�� �����ͷ� ���� ��Ų��.
	fsetpos(fpScript, &fpos);

	return	siTotalDirectionNum;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ������ ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	CharAniParser::ReadCharActionDirectionInfo(CHAR *pszReadBuffer, OnlineCharActionDirectionInfo *pDirectionInfo)
{
	CHAR	cSep[] = "\t ";
	CHAR	*pToken;
	SI32	siStatus;	

	siStatus		=	ON_READ_CHAR_DIRECTION_INFO_DIRECTION;
	pToken			=	strtok(pszReadBuffer, cSep);	
	while(pToken != NULL)
	{
		switch(siStatus)
		{
		case ON_READ_CHAR_DIRECTION_INFO_DIRECTION:
			// ù��° ���ڿ��̴�. ������ ���ڿ��� ������ �ִ�.
			pDirectionInfo->siDirection	=	pMyOnlineWorld->GetDirection(pToken);
			siStatus					=	ON_READ_CHAR_DIRECTION_INFO_FONT;
			pDirectionInfo->siFrameNum	=	0;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_FONT:
			// ��Ʈ ��ȣ�̴�. 
			if(strstr(pToken, ",") == NULL)			
			{
				// �޸��� ������ ���� �ʴٴ� ���� ������ ���ڶ�� ���̴�.
				siStatus						=	ON_READ_CHAR_DIRECTION_INFO_PARAMETER;
				pDirectionInfo->siParameter1	=	0;			// �Ķ���͸� 0���� �ʱ�ȭ ���ش�.(� ������ ���� ��������� �𸣹Ƿ�)
			}
		
			pDirectionInfo->siFrame[pDirectionInfo->siFrameNum]	=	atoi(pToken);
			pDirectionInfo->siFrameNum++;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_PARAMETER:
			// �Ķ�����̴�.
			if(strstr(pToken, "REVERSE") != NULL)
			{
				// Reverse ����� �ִ�.
				pDirectionInfo->siParameter1	|=	ON_CHAR_ANI_REVERSE;				
			}
			break;
		}

		pToken			=	strtok(NULL, cSep);	
	}
}
