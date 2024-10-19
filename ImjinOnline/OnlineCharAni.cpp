//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ÀÓÁø·Ï ¿Â¶óÀÎ [°Å»ó]
//	File Name		: OnlineCharAni.cpp
//	Birth Date		: 2004. 02. 25.
//	Creator			: °Å »ó ÆÀ
//	Editer			: Á¶ Çö ÁØ (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: »ç¿ëÇÏÁö ¾Ê´Â ¼Ò½ºÀÔ´Ï´Ù.
//		=== Disable Source Files Comment ===
//		±âÁ¸ ¼Ò½º¿¡¼­ ÀÌ Å¬·¡½º¸¦ »ç¿ëÇÏ´Â ÇÊ¼ö ºÎºÐÀÌ ÀüºÎ ÁÖ¼®Ã³¸® µÇ¾î ÀÖ¾ú½À´Ï´Ù.
//		µû¶ó¼­ ½ÇÇàÆÄÀÏÀÇ ±Ô¸ð¸¦ ÁÙÀÌ±â À§ÇØ ÇØ´ç ÄÚµå¸¦ ÁÖ¼®Ã³¸®ÇÕ´Ï´Ù.
//		³ªÁß¿¡ ÇÊ¿äÇÑ ºÎºÐÀÌ ÀÖ°Å³ª ÀÌ°Í ¶§¹®¿¡ ¹®Á¦°¡ ¹ß»ýÇÒ °æ¿ì ´Ù½Ã º¹±¸½ÃÅ°¸é µË´Ï´Ù.
//
//===================================================================================================

/*
#include <Main.h>
#include "OnlineCharAni.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineCharAni::OnlineCharAni()
{
	pOnlineCharsAniInfo		=	NULL;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineCharAni::~OnlineCharAni()
{
	Free();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­,
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineCharAni::Init(cltOnlineWorld *pOnlineWorld)
{
	CharAniParser		*pAniParser;
	BOOL				bRet = TRUE;;

	pAniParser				=	new	CharAniParser;
	pOnlineCharsAniInfo		=	new OnlineCharsAniInfo;		

	if(pAniParser->Open("Online\\CharInfo\\CharInfo.txt", pOnlineWorld) == TRUE)
	{
		pAniParser->Read(pOnlineCharsAniInfo);
		pAniParser->Close();
	}
	else
	{
		bRet	=	FALSE;
		goto END;
	}
	
END:
	if(pAniParser)
	{
		delete pAniParser;
		pAniParser	=	NULL;
	}

	return bRet;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇØÁ¦.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharAni::Free()
{
	SI32	i, j;		
	
	if(pOnlineCharsAniInfo)
	{
		if(pOnlineCharsAniInfo->pCharAniInfo != NULL)
		{
			for(i = 0; i < pOnlineCharsAniInfo->siTotalCharNum; i++)
			{			
				if(pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo != NULL)
				{
					for(j = 0; j < pOnlineCharsAniInfo->pCharAniInfo[i].siTotalActionNum; j++)
					{			
						if(pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo[j].pCharActionDirectionInfo != NULL)
						{
							delete [] pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo[j].pCharActionDirectionInfo;
							pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo[j].pCharActionDirectionInfo	=	NULL;
						}
					}		

					delete [] pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo;
					pOnlineCharsAniInfo->pCharAniInfo[i].pCharActionInfo	=	NULL;
				}
			}

			delete [] pOnlineCharsAniInfo->pCharAniInfo;
			pOnlineCharsAniInfo->pCharAniInfo	=	NULL;
		}

		delete pOnlineCharsAniInfo;
		pOnlineCharsAniInfo	=	NULL;
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇÑ Ä³¸¯ÅÍÀÇ ¸ðµç ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ °¡¸£Å°´Â Æ÷ÀÎÅÍ¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineCharAniInfo*		OnlineCharAni::GetCharAniInfo(UI16 uiCharID)
{
	SI32	i;	

	for(i = 0; i < pOnlineCharsAniInfo->siTotalCharNum; i++)
	{	
		if(uiCharID == pOnlineCharsAniInfo->pCharAniInfo[i].uiCharID)		
			return &pOnlineCharsAniInfo->pCharAniInfo[i];		
	}

	clGrp.Error("OnlineCharAni::GetCharAniInfo", "ÇØ´ç CharÀÇ AnimationÀ» Ã£´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù. Char ID : [%d]", uiCharID);

	return NULL;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇÑ Ä³¸¯ÅÍÀÇ ÇÑ ¾×¼ÇÀÇ ¸ðµç ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ °¡¸£Å°´Â Æ÷ÀÎÅÍ¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineCharActionInfo*	OnlineCharAni::GetCharActionInfo(OnlineCharAniInfo *pOnlineCharAniInfo, SI32 siActionCode)
{
	SI32	i;
	
	for(i = 0; i < pOnlineCharAniInfo->siTotalActionNum; i++)
	{
		if(siActionCode == pOnlineCharAniInfo->pCharActionInfo[i].siActionCode)
		{
			return &pOnlineCharAniInfo->pCharActionInfo[i];
		}
	}
	clGrp.Error("OnlineCharAni::GetCharActionInfo", "ÇØ´ç CharÀÇ Action Info¸¦ Ã£´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù. [%d]", siActionCode);

	return NULL;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇÑ Ä³¸¯ÅÍÀÇ ÇÑ ¾×¼ÇÀÇ ÇÑ ¹æÇâÀÇ ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ °¡¸£Å°´Â Æ÷ÀÎÅÍ¸¦ ±¸ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
OnlineCharActionDirectionInfo*	OnlineCharAni::GetCharDirectionInfo(OnlineCharActionInfo *pOnlineCharActionInfo, SI32 siDirection)
{
	SI32	i;

	for(i = 0; i < pOnlineCharActionInfo->siDirectionNum; i++)
	{
		if(siDirection == pOnlineCharActionInfo->pCharActionDirectionInfo[i].siDirection)
			return &pOnlineCharActionInfo->pCharActionDirectionInfo[i];
	}
	
	clGrp.Error("OnlineCharAni::GetCharDirectionInfo", "ÇØ´ç CharÀÇ DirectionInfoÀ» Ã£´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù. [%d]", siDirection);

	return NULL;
}
 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	À¯È¿ÇÑ Ä³¸¯ÅÍÀÇ idÀÎ°¡?
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	OnlineCharAni::IsVaildID(UI16 uiCharID)
{
	SI32	i;

	for(i = 0; i < pOnlineCharsAniInfo->siTotalCharNum; i++)
	{	
		if(uiCharID == pOnlineCharsAniInfo->pCharAniInfo[i].uiCharID)		
			return TRUE;
	}

	return FALSE;
}
 */