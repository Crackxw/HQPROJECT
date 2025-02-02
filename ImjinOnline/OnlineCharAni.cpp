//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 歜霞煙 螞塭檣 [剪鼻]
//	File Name		: OnlineCharAni.cpp
//	Birth Date		: 2004. 02. 25.
//	Creator			: 剪 鼻 ぜ
//	Editer			: 褻 ⑷ 遽 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 餌辨ж雖 彊朝 模蝶殮棲棻.
//		=== Disable Source Files Comment ===
//		晦襄 模蝶縑憮 檜 贗楚蝶蒂 餌辨ж朝 в熱 睡碟檜 瞪睡 輿戮籀葬 腎橫 氈歷蝗棲棻.
//		評塭憮 褒чだ橾曖 敘賅蒂 還檜晦 嬪п п渡 囀萄蒂 輿戮籀葬м棲棻.
//		釭醞縑 в蹂и 睡碟檜 氈剪釭 檜匙 陽僥縑 僥薯陛 嫦儅й 唳辦 棻衛 犒掘衛酈賊 腌棲棻.
//
//===================================================================================================

/*
#include <Main.h>
#include "OnlineCharAni.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineCharAni::OnlineCharAni()
{
	pOnlineCharsAniInfo		=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineCharAni::~OnlineCharAni()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��,
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и 議葛攪曖 賅萇 擁棲詭檜暮 薑爾蒂 陛腦酈朝 ん檣攪蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineCharAniInfo*		OnlineCharAni::GetCharAniInfo(UI16 uiCharID)
{
	SI32	i;	

	for(i = 0; i < pOnlineCharsAniInfo->siTotalCharNum; i++)
	{	
		if(uiCharID == pOnlineCharsAniInfo->pCharAniInfo[i].uiCharID)		
			return &pOnlineCharsAniInfo->pCharAniInfo[i];		
	}

	clGrp.Error("OnlineCharAni::GetCharAniInfo", "п渡 Char曖 Animation擊 瓊朝等 褒ぬц蝗棲棻. Char ID : [%d]", uiCharID);

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и 議葛攪曖 и 擋暮曖 賅萇 擁棲詭檜暮 薑爾蒂 陛腦酈朝 ん檣攪蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	clGrp.Error("OnlineCharAni::GetCharActionInfo", "п渡 Char曖 Action Info蒂 瓊朝等 褒ぬц蝗棲棻. [%d]", siActionCode);

	return NULL;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и 議葛攪曖 и 擋暮曖 и 寞щ曖 擁棲詭檜暮 薑爾蒂 陛腦酈朝 ん檣攪蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineCharActionDirectionInfo*	OnlineCharAni::GetCharDirectionInfo(OnlineCharActionInfo *pOnlineCharActionInfo, SI32 siDirection)
{
	SI32	i;

	for(i = 0; i < pOnlineCharActionInfo->siDirectionNum; i++)
	{
		if(siDirection == pOnlineCharActionInfo->pCharActionDirectionInfo[i].siDirection)
			return &pOnlineCharActionInfo->pCharActionDirectionInfo[i];
	}
	
	clGrp.Error("OnlineCharAni::GetCharDirectionInfo", "п渡 Char曖 DirectionInfo擊 瓊朝等 褒ぬц蝗棲棻. [%d]", siDirection);

	return NULL;
}
 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 議葛攪曖 id檣陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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