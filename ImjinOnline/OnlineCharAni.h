//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�]
//	File Name		: OnlineCharAni.h
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
#ifndef _ONLINE_CHAR_ANI_H
#define _ONLINE_CHAR_ANI_H

#include "Online.h"
#include "OnlineCharAniParser.h"

// ��� ĳ������ ������ ������ �����ϰ� �ִ�.
class	OnlineCharAni
{
private:	
	OnlineCharsAniInfo	*pOnlineCharsAniInfo;
public:
	OnlineCharAni();																	// ������.
	~OnlineCharAni();																	// �Ҹ���.

	BOOL				Init(cltOnlineWorld *pOnlineWorld);								// �ʱ�ȭ,
	VOID				Free();															// ����.
	BOOL				IsVaildID(UI16 uiCharID);										// ��ȿ�� ĳ������ id�ΰ�?

	OnlineCharAniInfo*				GetCharAniInfo(UI16 uiCharID);															// �� ĳ������ ��� �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
	OnlineCharActionInfo*			GetCharActionInfo(OnlineCharAniInfo *pOnlineCharAniInfo, SI32 siActionCode);			// �� ĳ������ �� �׼��� ��� �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
	OnlineCharActionDirectionInfo*	GetCharDirectionInfo(OnlineCharActionInfo *pOnlineCharActionInfo, SI32 siDirection);	// �� ĳ������ �� �׼��� �� ������ �ִϸ��̼� ������ ����Ű�� �����͸� ���Ѵ�.
};

#endif
*/