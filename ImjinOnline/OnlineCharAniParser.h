#ifndef _ONLINE_CHAR_ANI_PARSER_H
#define _ONLINE_CHAR_ANI_PARSER_H

#include "Online.h"
#include "OnlineBaseParser.h"

#define	MAX_FRAMENUM_PER_ANI		32		// �� ĳ������ �� ����� �ִ� ������ ��.

#define	ON_CHAR_ANI_REVERSE			1		// ĳ���� �ִϸ��̼ǿ� ������ ����� �ִ�.

// �� ĳ������ �� �׼��� ����� �ִϸ��̼� ������ ������ ����ü�̴�.
struct	OnlineCharActionDirectionInfo
{
	SI32	siDirection;

	SI16	siFrameNum;								// ������ ����.
	SI16	siFrame[MAX_FRAMENUM_PER_ANI];			// �� �����Ӵ� ȭ�Ͽ����� ��Ʈ ��ȣ.	
	SI32	siParameter1;							// �Ķ����1 
};

// �� ĳ������ �� �׼��� �ִϸ��̼� ������ ������ ����ü�̴�.
struct	OnlineCharActionInfo
{
	SI32	siActionCode;											// �׼��� �ڵ�.
	
	SI16	siFile;													// ȭ��
	SI16	siDirectionNum;											// �׼��� ���� ����.

	OnlineCharActionDirectionInfo	*pCharActionDirectionInfo;		// ĳ������ ������ ���� ������ ����.
};

// �� ĳ������ �ִϸ��̼� ������ ������ ����ü�̴�.
struct	OnlineCharAniInfo
{
	UI16					uiCharID;								// ĳ������ ID
	CHAR					szPortraitFileName[1024];				// �ʻ�ȭ ȭ�� �̸�,
	UI16					siPortraitFileFont;						// �ʻ�ȭ ȭ�� ��Ʈ ��ȣ.
	UI32					uiWidthRIP;								// Width Real Image Percent
	UI32					uiHeightRIP;							// Height Real Image Percent
	SI32					siHeightRIPIndent;						// Height Indent
	SI32					siFrameDelay;							// Frame Delay.

	SI32					siTotalActionNum;
	OnlineCharActionInfo	*pCharActionInfo;
};

// ��� ĳ���͵��� �ִϸ��̼� ������ ������ ����ü�̴�.
struct	OnlineCharsAniInfo
{	
	SI32				siTotalCharNum;
	OnlineCharAniInfo	*pCharAniInfo;
};

class	CharAniParser : public HQArielLabelScriptSystem
{
private:
	cltOnlineWorld	*pMyOnlineWorld;	
public:
	CharAniParser();		// ������.
	~CharAniParser();		// �Ҹ���.

	virtual	BOOL	Open(char * filename, cltOnlineWorld *pOnlineWorld);			// ȭ���� ����.
	virtual void	Close(void);													// ȭ���� �ݴ´�.
	virtual	BOOL	Read(OnlineCharsAniInfo *pCharsAniInfo);						// ȭ���� �д´�.

	SI32			GetTotalCharNum();																					// ��ü ĳ���� ���� ���Ѵ�.
	SI32			GetTotalActionNum();																				// ��ü �׼� ���� ���Ѵ�.
	SI32			GetTotalDirectionNum();																				// ��ü ���� ���� ���Ѵ�.
	VOID			ReadCharActionDirectionInfo(CHAR *pszReadBuffer, OnlineCharActionDirectionInfo *pDirectionInfo);	// ������ ������ ������ ���´�.
};

#endif
