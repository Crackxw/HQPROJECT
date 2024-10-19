/************************************************************************************************
	파일명 : OnlineCharActionCode.cpp
	담당자 : 이윤석

	Character의 동작코드를 검사한다.
************************************************************************************************/
#include <GSL.h>
#include "OnlineCharActionCode.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	문자열로 된 캐릭터의 액션을 숫자로 변환한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	OnlineCharActionCode::GetCharActionCode(CHAR *pszCharActionName)
{
	SI32	i;
	CHAR	szActionName[ON_CHAR_ACTION_CODE_NUM][16] =
	{
		"",
		"WAIT",
		"MOVE"
	};

	for(i = 1; i < ON_CHAR_ACTION_CODE_NUM; i++)
		if(stricmp(szActionName[i], pszCharActionName) == 0)
			return i;

	return 0;
}

