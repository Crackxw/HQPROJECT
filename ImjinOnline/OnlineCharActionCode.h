/************************************************************************************************
	���ϸ� : OnlineCharActionCode.h
	����� : ������
************************************************************************************************/
#ifndef _ONLINE_CHAR_ACTION_CODE_H
#define _ONLINE_CHAR_ACTION_CODE_H

#include "Online.h"

enum	{ON_CHAR_ACTION_CODE_UNKNOWN, ON_CHAR_ACTION_CODE_WAIT, ON_CHAR_ACTION_CODE_MOVE, ON_CHAR_ACTION_CODE_NUM, ON_CHAR_ACTION_CODE_ATTACK, ON_CHAR_ACTION_CODE_SPELL_MAGIC};


class	OnlineCharActionCode
{
public:
	SI32	GetCharActionCode(CHAR *pszCharActionName);				// ���ڿ��� �� ĳ������ �׼��� ���ڷ� ��ȯ�Ѵ�.
};

#endif