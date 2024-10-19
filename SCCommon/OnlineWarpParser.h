#include <windows.h>
#include "Directives.h"

#ifndef _ONLINE_WARP_PARSER_H
#define _ONLINE_WARP_PARSER_H

#define	MAX_WARP_DEST_LENGTH		32			// ���� �������� �ִ� �ؽ�Ʈ ����


// Code		Destination		Destination_Type	Location X	Location Y	Map index
struct strWarpInfo
{
	SI16		siCode;								// ��ȣ
	CHAR		szDest[MAX_WARP_DEST_LENGTH+1];		// ���� ���(�����)
	SI32		siOnlineTextNo;						// �¶��� �ؽ�Ʈ������ ��ȣ	// actdoll (2004/06/08 13:12) : �̹��� �߰���
	CHAR		cType;								// ���� Ÿ��
	UI16		uiX;								// ��ǥ x
	UI16		uiY;								// ��ǥ y
	UI16		uiMapIndex;							// �� ��ȣ
};


class cltOnlineWarpParser
{
private:
	SI32			m_siInfoCount;				// ����
	strWarpInfo		*m_pstWarpInfoList;			// ���� ���� ����Ʈ�� ������

public:
	cltOnlineWarpParser();
	~cltOnlineWarpParser();
	BOOL			Init( CHAR *pszFileName, OnlineText *pOnlineText );
	SI32			GetWarpCode( CHAR *szDest );		// ���� �������� ���� �ڵ带 ����.
	strWarpInfo*	GetWarpInfo( CHAR *szDest );		// ���� �������� ���� ������ ����.
	strWarpInfo*	GetWarpInfo( SI32 siCode );			// ���� �ڵ�� ���� ������ ����.
	VOID			Free();
};

#endif