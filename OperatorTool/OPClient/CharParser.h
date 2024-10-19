#pragma once

#include "OnlineChar-Parser.h"

class CharParser : public OnlineCharParser
{
public:
	CharParser()		{};
	~CharParser()		{};

	BOOL	Init( CHAR *pCharFile );
};