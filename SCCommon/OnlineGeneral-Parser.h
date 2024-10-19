#ifndef __ONLINEGENERAL_PARSER_H
#define __ONLINEGENERAL_PARSER_H

struct	GeneralHeader
{
	SI32	siGeneralCharacterCode;															// 장수 Character Code

	SI32	siGeneralBaseCharacterCode;														// 장수로 변환가능한 Character Code
	SI32	siGeneralChangeNeedCharacterLv;													// 장수로 변환가능한 Character 의 최소 레벨

	SI32	siGeneralChangeNeedTradeGrade;													// 장수로 변환할때 필요한 신용등급
};


class OnlineGeneralParser
{
public:
	GeneralHeader			*pGeneralHeader;												// General의 모든 정보를 가진 구조체
	SI16					siGeneralNum;													// 총 General
	
public:
	OnlineGeneralParser();
	virtual	~OnlineGeneralParser();

	BOOL					ReadText(char *pFileName);										// General 로드
	VOID					Free();															// 지운다

	GeneralHeader*			GetGeneral(SI32 siGeneralCharacterCode);						// Get General
	SI32					GetID(char *pToken);											// 케릭터의 아이디를 리턴 받는다.
	GeneralHeader*			GetGeneralChar(SI32 siCharCode);
};


#endif
