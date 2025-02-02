#include <Main.h>
#include "OnlineWorld.h"
#include "OnlineCharAniParser.h"
#include <DebugTool.h>
 
#define		ON_READ_CHAR_DIRECTION_INFO_DIRECTION		1
#define		ON_READ_CHAR_DIRECTION_INFO_FONT			2
#define		ON_READ_CHAR_DIRECTION_INFO_PARAMETER		3

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CharAniParser::CharAniParser()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CharAniParser::~CharAniParser()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�倣狨� 翱棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	CharAniParser::Open(char * filename, cltOnlineWorld *pOnlineWorld)
{
	pMyOnlineWorld	=	pOnlineWorld;
	return HQArielLabelScriptSystem::Open(filename);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�倣狨� 殘朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void	CharAniParser::Close(void)
{
	HQArielLabelScriptSystem::Close();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�倣狨� 檗朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 瞪羹 議葛攪 熱蒂 掘и棻.
	pCharsAniInfo->siTotalCharNum	=	GetTotalCharNum();

	if(pCharsAniInfo->siTotalCharNum == 0)
	{
		clGrp.Error("", "pCharsAniInfo->siTotalCharNum Is Zero [%s]");
		return FALSE;
	}

	// 議葛攪蒂 й渡 嫡朝棻.
	pCharsAniInfo->pCharAniInfo		=	new	OnlineCharAniInfo[pCharsAniInfo->siTotalCharNum];

	// й渡 嫡擎 議葛攪曖 薑爾蒂 蟾晦�飛� 衛麵遽棻.(瞪睡棻 衛酈雖 彊嬴紫 奩萄衛 п輿橫撿 й 滲熱朝 п輿橫撿 и棻.
	for(i = 0; i < pCharsAniInfo->siTotalCharNum; i++)	
	{
		// 蟾鼻�� 檜葷擊 蟾晦�� 衛麵遽棻.
		strcpy(pCharsAniInfo->pCharAniInfo[i].szPortraitFileName, "");
		pCharsAniInfo->pCharAniInfo[i].siPortraitFileFont	=	-1;
	}
	

	// �倣� ん檣攪蒂 籀擠戲煎 給萼棻.
	fseek(fpScript, 0, SEEK_SET);
	
	// 檜薯睡攪 議葛攪曖 渠и 薑爾蒂 �倣狤□� 檗橫螞棻.
	siCurrentParsingChar	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// 僥濠翮 裔 菴曖 '\n' 睡碟擎 澀塭魚棻.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)		
		{	
			siBraceNum++;	
			
			switch(siBraceNum)
			{				
			case 1:				
				// 億煎遴 議葛攪 檜嘎煎 だ諒ж朝 擋暮 檣策蝶 高擊 蟾晦�� и棻.
				siCurrentParsingCharAction = 0;

				// 億煎遴 議葛攪 擋暮 睡碟曖 衛濛檜棻.
				// 識 賃偃曖 擋暮檜 氈朝雖 橢橫螞棻.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum	=	GetTotalActionNum();				

				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo	=	new OnlineCharActionInfo[pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum];								

				if(siCurrentParsingChar == 0)
					ptotalactionnum	=	&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum;
				break;
			case 2:
				// 億煎遴 и 擋暮曖 擁棲詭檜暮擊 だ諒ж朝 擋暮 檣策蝶 高擊 蟾晦�� и棻.
				siCurrentParsingCharActionDirection	=	0;

				// 億煎遴 議葛攪 擋暮 擁棲詭檜暮 睡碟曖 衛濛檜棻.
				// 識 賃偃曖 擁棲詭檜暮檜 氈朝雖 橢橫螞棻.
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
				// ⑷營 だ諒ж朝 議葛攪蒂 ж釭 隸陛衛鑑棻.
				siCurrentParsingChar++;
				break;
			case 1:
				// ⑷營 だ諒ж朝 擋暮 檣策蝶 高擊 隸陛衛鑑棻.
				siCurrentParsingCharAction++;				
				break;
			}
		}
		else
		{
			// siBraceNum曖 偃熱煎 ⑷營 橫替 睡碟擊 だ諒ж朝雖 憲嬴鳥 熱 氈棻.		
			switch(siBraceNum)
			{
			case 0:				
				// 億煎遴 議葛攪蒂 だ諒и棻. 僥濠翮 ID 睡碟檜棻.				

				// 僥濠翮 ID蒂 晦橘и棻.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiCharID	=	MAKEWORD(szReadBuffer[0], atoi(&szReadBuffer[1]));				
				break;
			case 1:
				if(strstr(szReadBuffer, "[PORTRAIT]") != NULL)
				{		
					// 蟾鼻�� 檜葷檜棻.
					sscanf(szReadBuffer, "%s %s %s %d", szDummy1, szDummy2, 
													pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].szPortraitFileName, 
													&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siPortraitFileFont);
				
				}
				else if(strstr(szReadBuffer, "[RIP_WIDTH]") != NULL)
				{
					// Real Image Percent 檜棻.(Width)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiWidthRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHT]") != NULL)
				{
					// Real Image Percent 檜棻.(Height)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiHeightRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHTINDENT]") != NULL)
				{
					// Real Image Percent 檜棻.(Height INDENT)
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siHeightRIPIndent);					
				}
				else if(strstr(szReadBuffer, "[FRAMEDELAY]") != NULL)
				{
					// Frame Delay檜棻.
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siFrameDelay);
				}
				else
				{
					sscanf(szReadBuffer, "%s %s", szCharActionName, szFileName);										

					// 僥濠翮 擋暮 囀萄蒂 璋濠煎 滲�納挬�.
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siActionCode	=	pMyOnlineWorld->pOnlineCharActionCode->GetCharActionCode(szCharActionName);

					// 葬模蝶縑憮 п渡 �倣狨� 檣策蝶蒂 橢橫螞棻.				
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siFile		=	 pMyOnlineWorld->pOnlineResource->GetCharFileIndex(szFileName);				
				} 				
				break;
			case 2:
				// 擁棲詭檜暮 還 薑爾蒂 檗朝棻.
				ReadCharActionDirectionInfo(szReadBuffer, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].pCharActionDirectionInfo[siCurrentParsingCharActionDirection]);

				// ⑷營 だ諒ж朝 и 擋暮曖 寞щ 檣策蝶 高擊 隸陛 衛鑑棻.
				siCurrentParsingCharActionDirection++;				
				break;
			}
		}		
	}	

	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪羹 議葛攪 熱蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	CharAniParser::GetTotalCharNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharNum;

	// ⑷營曖 �倣� ん檣攪蒂 盪濰и棻.
	fgetpos(fpScript, &fpos);

	// �倣� ん檣攪蒂 �倣狨� 籀擠戲煎 給萼棻.
	fseek(fpScript, 0, SEEK_SET);

	siTotalCharNum	=	0;
	siBraceNum		=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// 僥濠翮 裔 菴曖 '\n' 睡碟擎 澀塭魚棻.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)	siBraceNum--;
		else
		{
			if(siBraceNum == 0)
				siTotalCharNum++;
		}
	}

	// 錳楚曖 �倣� ん檣攪煎 犒掘 衛鑑棻.
	fsetpos(fpScript, &fpos);

	return	siTotalCharNum;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪羹 擋暮 熱蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	CharAniParser::GetTotalActionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharActionNum;

	// ⑷營曖 �倣� ん檣攪蒂 盪濰и棻.
	fgetpos(fpScript, &fpos);	

	siTotalCharActionNum	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// 僥濠翮 裔 菴曖 '\n' 睡碟擎 澀塭魚棻.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

			if(siBraceNum == -1)			break;					// 議葛攪曖 擋暮 睡碟擎 賅舒 棻 檗歷棻.
			else if(siBraceNum == 0)		siTotalCharActionNum++;

		}		
	}

	// 錳楚曖 �倣� ん檣攪煎 犒掘 衛鑑棻.
	fsetpos(fpScript, &fpos);

	return	siTotalCharActionNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪羹 寞щ 熱蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	CharAniParser::GetTotalDirectionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalDirectionNum;

	// ⑷營曖 �倣� ん檣攪蒂 盪濰и棻.
	fgetpos(fpScript, &fpos);

	siTotalDirectionNum	=	0;
	siBraceNum			=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// 僥濠翮 裔 菴曖 '\n' 睡碟擎 澀塭魚棻.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

		}
		else
		{
			if(siBraceNum == -1)			break;					// 議葛攪曖 擋暮 睡碟擎 賅舒 棻 檗歷棻.
			else	if(siBraceNum == 0)		siTotalDirectionNum++;
		}		
	}

	// 錳楚曖 �倣� ん檣攪煎 犒掘 衛鑑棻.
	fsetpos(fpScript, &fpos);

	return	siTotalDirectionNum;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	寞щ曖 Щ溯歜 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
			// 羅廓簞 僥濠翮檜棻. 寞щ曖 僥濠翮擊 陛雖堅 氈棻.
			pDirectionInfo->siDirection	=	pMyOnlineWorld->GetDirection(pToken);
			siStatus					=	ON_READ_CHAR_DIRECTION_INFO_FONT;
			pDirectionInfo->siFrameNum	=	0;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_FONT:
			// アお 廓��檜棻. 
			if(strstr(pToken, ",") == NULL)			
			{
				// 巍葆蒂 陛雖堅 氈雖 彊棻朝 匙擎 葆雖虞 璋濠塭朝 匙檜棻.
				siStatus						=	ON_READ_CHAR_DIRECTION_INFO_PARAMETER;
				pDirectionInfo->siParameter1	=	0;			// だ塭嘐攪蒂 0戲煎 蟾晦�� п遽棻.(橫雯 噙溯晦 高檜 菟橫氈擊還 賅腦嘎煎)
			}
		
			pDirectionInfo->siFrame[pDirectionInfo->siFrameNum]	=	atoi(pToken);
			pDirectionInfo->siFrameNum++;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_PARAMETER:
			// だ塭嘐攪檜棻.
			if(strstr(pToken, "REVERSE") != NULL)
			{
				// Reverse 晦棟檜 氈棻.
				pDirectionInfo->siParameter1	|=	ON_CHAR_ANI_REVERSE;				
			}
			break;
		}

		pToken			=	strtok(NULL, cSep);	
	}
}
