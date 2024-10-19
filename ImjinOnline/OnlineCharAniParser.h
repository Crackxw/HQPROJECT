#ifndef _ONLINE_CHAR_ANI_PARSER_H
#define _ONLINE_CHAR_ANI_PARSER_H

#include "Online.h"
#include "OnlineBaseParser.h"

#define	MAX_FRAMENUM_PER_ANI		32		// 한 캐릭터의 한 방향당 최대 프레임 수.

#define	ON_CHAR_ANI_REVERSE			1		// 캐릭터 애니메이션에 리버스 기능이 있다.

// 한 캐릭터의 한 액션의 방향당 애니메이션 정보를 선언한 구조체이다.
struct	OnlineCharActionDirectionInfo
{
	SI32	siDirection;

	SI16	siFrameNum;								// 프레임 개수.
	SI16	siFrame[MAX_FRAMENUM_PER_ANI];			// 각 프레임당 화일에서의 폰트 번호.	
	SI32	siParameter1;							// 파라미터1 
};

// 한 캐릭터의 한 액션의 애니메이션 정보를 선언한 구조체이다.
struct	OnlineCharActionInfo
{
	SI32	siActionCode;											// 액션의 코드.
	
	SI16	siFile;													// 화일
	SI16	siDirectionNum;											// 액션의 방향 개수.

	OnlineCharActionDirectionInfo	*pCharActionDirectionInfo;		// 캐릭터의 방향의 대한 프레임 정보.
};

// 한 캐릭터의 애니메이션 정보를 선언한 구조체이다.
struct	OnlineCharAniInfo
{
	UI16					uiCharID;								// 캐릭터의 ID
	CHAR					szPortraitFileName[1024];				// 초상화 화일 이름,
	UI16					siPortraitFileFont;						// 초상화 화일 폰트 번호.
	UI32					uiWidthRIP;								// Width Real Image Percent
	UI32					uiHeightRIP;							// Height Real Image Percent
	SI32					siHeightRIPIndent;						// Height Indent
	SI32					siFrameDelay;							// Frame Delay.

	SI32					siTotalActionNum;
	OnlineCharActionInfo	*pCharActionInfo;
};

// 모든 캐릭터들의 애니메이션 정보를 선언한 구조체이다.
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
	CharAniParser();		// 생성자.
	~CharAniParser();		// 소멸자.

	virtual	BOOL	Open(char * filename, cltOnlineWorld *pOnlineWorld);			// 화일을 연다.
	virtual void	Close(void);													// 화일을 닫는다.
	virtual	BOOL	Read(OnlineCharsAniInfo *pCharsAniInfo);						// 화일을 읽는다.

	SI32			GetTotalCharNum();																					// 전체 캐릭터 수를 구한다.
	SI32			GetTotalActionNum();																				// 전체 액션 수를 구한다.
	SI32			GetTotalDirectionNum();																				// 전체 방향 수를 구한다.
	VOID			ReadCharActionDirectionInfo(CHAR *pszReadBuffer, OnlineCharActionDirectionInfo *pDirectionInfo);	// 방향의 프레임 정보를 얻어온다.
};

#endif
