//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상]
//	File Name		: OnlineCharAni.h
//	Birth Date		: 2004. 02. 25.
//	Creator			: 거 상 팀
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용하지 않는 소스입니다.
//		=== Disable Source Files Comment ===
//		기존 소스에서 이 클래스를 사용하는 필수 부분이 전부 주석처리 되어 있었습니다.
//		따라서 실행파일의 규모를 줄이기 위해 해당 코드를 주석처리합니다.
//		나중에 필요한 부분이 있거나 이것 때문에 문제가 발생할 경우 다시 복구시키면 됩니다.
//
//===================================================================================================

/*
#ifndef _ONLINE_CHAR_ANI_H
#define _ONLINE_CHAR_ANI_H

#include "Online.h"
#include "OnlineCharAniParser.h"

// 모든 캐릭터의 프레임 정보를 저장하고 있다.
class	OnlineCharAni
{
private:	
	OnlineCharsAniInfo	*pOnlineCharsAniInfo;
public:
	OnlineCharAni();																	// 생성자.
	~OnlineCharAni();																	// 소멸자.

	BOOL				Init(cltOnlineWorld *pOnlineWorld);								// 초기화,
	VOID				Free();															// 해제.
	BOOL				IsVaildID(UI16 uiCharID);										// 유효한 캐릭터의 id인가?

	OnlineCharAniInfo*				GetCharAniInfo(UI16 uiCharID);															// 한 캐릭터의 모든 애니메이션 정보를 가르키는 포인터를 구한다.
	OnlineCharActionInfo*			GetCharActionInfo(OnlineCharAniInfo *pOnlineCharAniInfo, SI32 siActionCode);			// 한 캐릭터의 한 액션의 모든 애니메이션 정보를 가르키는 포인터를 구한다.
	OnlineCharActionDirectionInfo*	GetCharDirectionInfo(OnlineCharActionInfo *pOnlineCharActionInfo, SI32 siDirection);	// 한 캐릭터의 한 액션의 한 방향의 애니메이션 정보를 가르키는 포인터를 구한다.
};

#endif
*/