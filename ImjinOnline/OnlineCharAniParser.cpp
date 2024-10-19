#include <Main.h>
#include "OnlineWorld.h"
#include "OnlineCharAniParser.h"
#include <DebugTool.h>
 
#define		ON_READ_CHAR_DIRECTION_INFO_DIRECTION		1
#define		ON_READ_CHAR_DIRECTION_INFO_FONT			2
#define		ON_READ_CHAR_DIRECTION_INFO_PARAMETER		3

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
CharAniParser::CharAniParser()
{

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
CharAniParser::~CharAniParser()
{

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	È­ÀÏÀ» ¿¬´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	CharAniParser::Open(char * filename, cltOnlineWorld *pOnlineWorld)
{
	pMyOnlineWorld	=	pOnlineWorld;
	return HQArielLabelScriptSystem::Open(filename);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	È­ÀÏÀ» ´Ý´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void	CharAniParser::Close(void)
{
	HQArielLabelScriptSystem::Close();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	È­ÀÏÀ» ÀÐ´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// ÀüÃ¼ Ä³¸¯ÅÍ ¼ö¸¦ ±¸ÇÑ´Ù.
	pCharsAniInfo->siTotalCharNum	=	GetTotalCharNum();

	if(pCharsAniInfo->siTotalCharNum == 0)
	{
		clGrp.Error("", "pCharsAniInfo->siTotalCharNum Is Zero [%s]");
		return FALSE;
	}

	// Ä³¸¯ÅÍ¸¦ ÇÒ´ç ¹Þ´Â´Ù.
	pCharsAniInfo->pCharAniInfo		=	new	OnlineCharAniInfo[pCharsAniInfo->siTotalCharNum];

	// ÇÒ´ç ¹ÞÀº Ä³¸¯ÅÍÀÇ Á¤º¸¸¦ ÃÊ±âÈ­¸¦ ½ÃÄÑÁØ´Ù.(ÀüºÎ´Ù ½ÃÅ°Áö ¾Ê¾Æµµ ¹Ýµå½Ã ÇØÁÖ¾î¾ß ÇÒ º¯¼ö´Â ÇØÁÖ¾î¾ß ÇÑ´Ù.
	for(i = 0; i < pCharsAniInfo->siTotalCharNum; i++)	
	{
		// ÃÊ»óÈ­ ÀÌ¸§À» ÃÊ±âÈ­ ½ÃÄÑÁØ´Ù.
		strcpy(pCharsAniInfo->pCharAniInfo[i].szPortraitFileName, "");
		pCharsAniInfo->pCharAniInfo[i].siPortraitFileFont	=	-1;
	}
	

	// È­ÀÏ Æ÷ÀÎÅÍ¸¦ Ã³À½À¸·Î µ¹¸°´Ù.
	fseek(fpScript, 0, SEEK_SET);
	
	// ÀÌÁ¦ºÎÅÍ Ä³¸¯ÅÍÀÇ ´ëÇÑ Á¤º¸¸¦ È­ÀÏ¿¡¼­ ÀÐ¾î¿Â´Ù.
	siCurrentParsingChar	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ¹®ÀÚ¿­ ¸Ç µÚÀÇ '\n' ºÎºÐÀº Àß¶ó³½´Ù.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)		
		{	
			siBraceNum++;	
			
			switch(siBraceNum)
			{				
			case 1:				
				// »õ·Î¿î Ä³¸¯ÅÍ ÀÌ¹Ç·Î ÆÄ½ÌÇÏ´Â ¾×¼Ç ÀÎµ¦½º °ªÀ» ÃÊ±âÈ­ ÇÑ´Ù.
				siCurrentParsingCharAction = 0;

				// »õ·Î¿î Ä³¸¯ÅÍ ¾×¼Ç ºÎºÐÀÇ ½ÃÀÛÀÌ´Ù.
				// ÃÑ ¸î°³ÀÇ ¾×¼ÇÀÌ ÀÖ´ÂÁö ¾ò¾î¿Â´Ù.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum	=	GetTotalActionNum();				

				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo	=	new OnlineCharActionInfo[pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum];								

				if(siCurrentParsingChar == 0)
					ptotalactionnum	=	&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siTotalActionNum;
				break;
			case 2:
				// »õ·Î¿î ÇÑ ¾×¼ÇÀÇ ¾Ö´Ï¸ÞÀÌ¼ÇÀ» ÆÄ½ÌÇÏ´Â ¾×¼Ç ÀÎµ¦½º °ªÀ» ÃÊ±âÈ­ ÇÑ´Ù.
				siCurrentParsingCharActionDirection	=	0;

				// »õ·Î¿î Ä³¸¯ÅÍ ¾×¼Ç ¾Ö´Ï¸ÞÀÌ¼Ç ºÎºÐÀÇ ½ÃÀÛÀÌ´Ù.
				// ÃÑ ¸î°³ÀÇ ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ ÀÖ´ÂÁö ¾ò¾î¿Â´Ù.
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
				// ÇöÀç ÆÄ½ÌÇÏ´Â Ä³¸¯ÅÍ¸¦ ÇÏ³ª Áõ°¡½ÃÅ²´Ù.
				siCurrentParsingChar++;
				break;
			case 1:
				// ÇöÀç ÆÄ½ÌÇÏ´Â ¾×¼Ç ÀÎµ¦½º °ªÀ» Áõ°¡½ÃÅ²´Ù.
				siCurrentParsingCharAction++;				
				break;
			}
		}
		else
		{
			// siBraceNumÀÇ °³¼ö·Î ÇöÀç ¾î´À ºÎºÐÀ» ÆÄ½ÌÇÏ´ÂÁö ¾Ë¾Æ³¾ ¼ö ÀÖ´Ù.		
			switch(siBraceNum)
			{
			case 0:				
				// »õ·Î¿î Ä³¸¯ÅÍ¸¦ ÆÄ½ÌÇÑ´Ù. ¹®ÀÚ¿­ ID ºÎºÐÀÌ´Ù.				

				// ¹®ÀÚ¿­ ID¸¦ ±â¾ïÇÑ´Ù.
				pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiCharID	=	MAKEWORD(szReadBuffer[0], atoi(&szReadBuffer[1]));				
				break;
			case 1:
				if(strstr(szReadBuffer, "[PORTRAIT]") != NULL)
				{		
					// ÃÊ»óÈ­ ÀÌ¸§ÀÌ´Ù.
					sscanf(szReadBuffer, "%s %s %s %d", szDummy1, szDummy2, 
													pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].szPortraitFileName, 
													&pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siPortraitFileFont);
				
				}
				else if(strstr(szReadBuffer, "[RIP_WIDTH]") != NULL)
				{
					// Real Image Percent ÀÌ´Ù.(Width)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiWidthRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHT]") != NULL)
				{
					// Real Image Percent ÀÌ´Ù.(Height)
					sscanf(szReadBuffer, "%s %s %u", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].uiHeightRIP);					
				}
				else if(strstr(szReadBuffer, "[RIP_HEIGHTINDENT]") != NULL)
				{
					// Real Image Percent ÀÌ´Ù.(Height INDENT)
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siHeightRIPIndent);					
				}
				else if(strstr(szReadBuffer, "[FRAMEDELAY]") != NULL)
				{
					// Frame DelayÀÌ´Ù.
					sscanf(szReadBuffer, "%s %s %d", szDummy1, szDummy2, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].siFrameDelay);
				}
				else
				{
					sscanf(szReadBuffer, "%s %s", szCharActionName, szFileName);										

					// ¹®ÀÚ¿­ ¾×¼Ç ÄÚµå¸¦ ¼ýÀÚ·Î º¯È¯ÇÑ´Ù.
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siActionCode	=	pMyOnlineWorld->pOnlineCharActionCode->GetCharActionCode(szCharActionName);

					// ¸®¼Ò½º¿¡¼­ ÇØ´ç È­ÀÏÀÇ ÀÎµ¦½º¸¦ ¾ò¾î¿Â´Ù.				
					pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].siFile		=	 pMyOnlineWorld->pOnlineResource->GetCharFileIndex(szFileName);				
				} 				
				break;
			case 2:
				// ¾Ö´Ï¸ÞÀÌ¼Ç ÁÙ Á¤º¸¸¦ ÀÐ´Â´Ù.
				ReadCharActionDirectionInfo(szReadBuffer, &pCharsAniInfo->pCharAniInfo[siCurrentParsingChar].pCharActionInfo[siCurrentParsingCharAction].pCharActionDirectionInfo[siCurrentParsingCharActionDirection]);

				// ÇöÀç ÆÄ½ÌÇÏ´Â ÇÑ ¾×¼ÇÀÇ ¹æÇâ ÀÎµ¦½º °ªÀ» Áõ°¡ ½ÃÅ²´Ù.
				siCurrentParsingCharActionDirection++;				
				break;
			}
		}		
	}	

	return TRUE;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀüÃ¼ Ä³¸¯ÅÍ ¼ö¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	CharAniParser::GetTotalCharNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharNum;

	// ÇöÀçÀÇ È­ÀÏ Æ÷ÀÎÅÍ¸¦ ÀúÀåÇÑ´Ù.
	fgetpos(fpScript, &fpos);

	// È­ÀÏ Æ÷ÀÎÅÍ¸¦ È­ÀÏÀÇ Ã³À½À¸·Î µ¹¸°´Ù.
	fseek(fpScript, 0, SEEK_SET);

	siTotalCharNum	=	0;
	siBraceNum		=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ¹®ÀÚ¿­ ¸Ç µÚÀÇ '\n' ºÎºÐÀº Àß¶ó³½´Ù.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)	siBraceNum--;
		else
		{
			if(siBraceNum == 0)
				siTotalCharNum++;
		}
	}

	// ¿ø·¡ÀÇ È­ÀÏ Æ÷ÀÎÅÍ·Î º¹±¸ ½ÃÅ²´Ù.
	fsetpos(fpScript, &fpos);

	return	siTotalCharNum;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀüÃ¼ ¾×¼Ç ¼ö¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	CharAniParser::GetTotalActionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalCharActionNum;

	// ÇöÀçÀÇ È­ÀÏ Æ÷ÀÎÅÍ¸¦ ÀúÀåÇÑ´Ù.
	fgetpos(fpScript, &fpos);	

	siTotalCharActionNum	=	0;
	siBraceNum				=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ¹®ÀÚ¿­ ¸Ç µÚÀÇ '\n' ºÎºÐÀº Àß¶ó³½´Ù.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

			if(siBraceNum == -1)			break;					// Ä³¸¯ÅÍÀÇ ¾×¼Ç ºÎºÐÀº ¸ðµÎ ´Ù ÀÐ¾ú´Ù.
			else if(siBraceNum == 0)		siTotalCharActionNum++;

		}		
	}

	// ¿ø·¡ÀÇ È­ÀÏ Æ÷ÀÎÅÍ·Î º¹±¸ ½ÃÅ²´Ù.
	fsetpos(fpScript, &fpos);

	return	siTotalCharActionNum;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀüÃ¼ ¹æÇâ ¼ö¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	CharAniParser::GetTotalDirectionNum()
{
	CHAR		szReadBuffer[1024];
	fpos_t		fpos;	
	SI32		siBraceNum;
	SI32		siTotalDirectionNum;

	// ÇöÀçÀÇ È­ÀÏ Æ÷ÀÎÅÍ¸¦ ÀúÀåÇÑ´Ù.
	fgetpos(fpScript, &fpos);

	siTotalDirectionNum	=	0;
	siBraceNum			=	0;
	while(fgets(szReadBuffer, 1023, fpScript) != NULL)
	{	
		// ¹®ÀÚ¿­ ¸Ç µÚÀÇ '\n' ºÎºÐÀº Àß¶ó³½´Ù.
		szReadBuffer[strlen(szReadBuffer)-2] = 0;

		if(strstr(szReadBuffer, "{") != NULL)	siBraceNum++;
		else if(strstr(szReadBuffer, "}") != NULL)
		{
			siBraceNum--;

		}
		else
		{
			if(siBraceNum == -1)			break;					// Ä³¸¯ÅÍÀÇ ¾×¼Ç ºÎºÐÀº ¸ðµÎ ´Ù ÀÐ¾ú´Ù.
			else	if(siBraceNum == 0)		siTotalDirectionNum++;
		}		
	}

	// ¿ø·¡ÀÇ È­ÀÏ Æ÷ÀÎÅÍ·Î º¹±¸ ½ÃÅ²´Ù.
	fsetpos(fpScript, &fpos);

	return	siTotalDirectionNum;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¹æÇâÀÇ ÇÁ·¹ÀÓ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
			// Ã¹¹øÂ° ¹®ÀÚ¿­ÀÌ´Ù. ¹æÇâÀÇ ¹®ÀÚ¿­À» °¡Áö°í ÀÖ´Ù.
			pDirectionInfo->siDirection	=	pMyOnlineWorld->GetDirection(pToken);
			siStatus					=	ON_READ_CHAR_DIRECTION_INFO_FONT;
			pDirectionInfo->siFrameNum	=	0;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_FONT:
			// ÆùÆ® ¹øÈ£ÀÌ´Ù. 
			if(strstr(pToken, ",") == NULL)			
			{
				// ÄÞ¸¶¸¦ °¡Áö°í ÀÖÁö ¾Ê´Ù´Â °ÍÀº ¸¶Áö¸· ¼ýÀÚ¶ó´Â °ÍÀÌ´Ù.
				siStatus						=	ON_READ_CHAR_DIRECTION_INFO_PARAMETER;
				pDirectionInfo->siParameter1	=	0;			// ÆÄ¶ó¹ÌÅÍ¸¦ 0À¸·Î ÃÊ±âÈ­ ÇØÁØ´Ù.(¾î¶² ¾²·¹±â °ªÀÌ µé¾îÀÖÀ»ÁÙ ¸ð¸£¹Ç·Î)
			}
		
			pDirectionInfo->siFrame[pDirectionInfo->siFrameNum]	=	atoi(pToken);
			pDirectionInfo->siFrameNum++;
			break;
		case ON_READ_CHAR_DIRECTION_INFO_PARAMETER:
			// ÆÄ¶ó¹ÌÅÍÀÌ´Ù.
			if(strstr(pToken, "REVERSE") != NULL)
			{
				// Reverse ±â´ÉÀÌ ÀÖ´Ù.
				pDirectionInfo->siParameter1	|=	ON_CHAR_ANI_REVERSE;				
			}
			break;
		}

		pToken			=	strtok(NULL, cSep);	
	}
}
