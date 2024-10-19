#ifndef	WARP_PARSER
#define WARP_PARSER

#include <windows.h>
#include "../SCCommon/GSCDefine.h"


struct WarpData
{
	int siKind;
	unsigned short	siMapIndex;
	int siX;
	int siY;
};

class CWarpParser
{
public:
	
	WarpData m_stWarpData[8];

	CWarpParser();
	~CWarpParser();

	void	Load(char* szFileName);
	short	GetMapPosition(unsigned short siKind);
};

#endif