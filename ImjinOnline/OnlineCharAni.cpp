//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�]
//	File Name		: OnlineCharAni.cpp
//	Birth Date		: 2004. 02. 25.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ������� �ʴ� �ҽ��Դϴ�.
//		=== Disable Source Files Comment ===
//		���� �ҽ����� �� Ŭ������ ����ϴ� �ʼ� �κ��� ���� �ּ�ó�� �Ǿ� �־����ϴ�.
//		���� ���������� �Ը� ���̱� ���� �ش� �ڵ带 �ּ�ó���մϴ�.
//		���߿� �ʿ��� �κ��� �ְų� �̰� ������ ������ �߻��� ��� �ٽ� ������Ű�� �˴ϴ�.
//
//===================================================================================================

/*
#include <Main.h>
#include "OnlineCharAni.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineCharAni::OnlineCharAni()
{
	pOnlineCharsAniInfo		=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineCharAni::~OnlineCharAni()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ,
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� ĳ������ ��� �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineCharAniInfo*		OnlineCharAni::GetCharAniInfo(UI16 uiCharID)
{
	SI32	i;	

	for(i = 0; i < pOnlineCharsAniInfo->siTotalCharNum; i++)
	{	
		if(uiCharID == pOnlineCharsAniInfo->pCharAniInfo[i].uiCharID)		
			return &pOnlineCharsAniInfo->pCharAniInfo[i];		
	}

	clGrp.Error("OnlineCharAni::GetCharAniInfo", "�ش� Char�� Animation�� ã�µ� �����߽��ϴ�. Char ID : [%d]", uiCharID);

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� ĳ������ �� �׼��� ��� �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	clGrp.Error("OnlineCharAni::GetCharActionInfo", "�ش� Char�� Action Info�� ã�µ� �����߽��ϴ�. [%d]", siActionCode);

	return NULL;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� ĳ������ �� �׼��� �� ������ �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineCharActionDirectionInfo*	OnlineCharAni::GetCharDirectionInfo(OnlineCharActionInfo *pOnlineCharActionInfo, SI32 siDirection)
{
	SI32	i;

	for(i = 0; i < pOnlineCharActionInfo->siDirectionNum; i++)
	{
		if(siDirection == pOnlineCharActionInfo->pCharActionDirectionInfo[i].siDirection)
			return &pOnlineCharActionInfo->pCharActionDirectionInfo[i];
	}
	
	clGrp.Error("OnlineCharAni::GetCharDirectionInfo", "�ش� Char�� DirectionInfo�� ã�µ� �����߽��ϴ�. [%d]", siDirection);

	return NULL;
}
 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȿ�� ĳ������ id�ΰ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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