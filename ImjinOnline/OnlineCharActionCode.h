/************************************************************************************************
	파일명 : OnlineCharActionCode.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef _ONLINE_CHAR_ACTION_CODE_H
#define _ONLINE_CHAR_ACTION_CODE_H

#include "Online.h"

enum	{ON_CHAR_ACTION_CODE_UNKNOWN, ON_CHAR_ACTION_CODE_WAIT, ON_CHAR_ACTION_CODE_MOVE, ON_CHAR_ACTION_CODE_NUM, ON_CHAR_ACTION_CODE_ATTACK, ON_CHAR_ACTION_CODE_SPELL_MAGIC};


class	OnlineCharActionCode
{
public:
	SI32	GetCharActionCode(CHAR *pszCharActionName);				// 문자열로 된 캐릭터의 액션을 숫자로 변환한다.
};

#endif