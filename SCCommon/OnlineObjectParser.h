#ifndef __ON_OBJECTPARSER_H
#define __ON_OBJECTPARSER_H


/********************************************************************************************
	파일명	:	OnlineObjectParser.cpp
	작성자	:	정 진 욱
	작성일	:	2001.07.24
	수정일	:	2001.07.25
********************************************************************************************/


#define	ATB_OBJECT_ISOBJECT			0x00000002
#define	ATB_OBJECT_ANI				0x00000004
#define	ATB_OBJECT_NPC01			0x00010000
#define	ATB_OBJECT_NPC02			0x00020000
#define	ATB_OBJECT_NPC03			0x00040000
#define	ATB_OBJECT_NPC04			0x00080000
#define	ATB_OBJECT_NPC05			0x00100000
#define	ATB_OBJECT_NPC06			0x00200000
#define	ATB_OBJECT_NPC07			0x00400000
#define	ATB_OBJECT_NPC08			0x00800000
#define	ATB_OBJECT_NPC09			0x01000000
#define	ATB_OBJECT_TRANS			0x02000000		// 투명하게 찍는 오브젝트


struct	ObjectHeader
{
	CHAR	szObjectName[100];
	CHAR	szObjectFileName[100];
	SI16	siCenterX, siCenterY;
	SI16	siXsize, siYsize;
	SI16	siFont, siKind, siFile, siFrame;
	SI32	siAttribute;
	SI32	siMark;
};

class OnlineObjectParser
{
private:
	ObjectHeader			*pObjectHeader;														// 오브젝트 구조체
	SI16					siObjectNum;														// 몇개의 오브젝트가 있나~

public:
	OnlineObjectParser();
	~OnlineObjectParser();

	BOOL					Init( CHAR *pFileName );											// 텍스를 받아 읽기
	SI16					GetObjectNum();														// 오브젝트 겟수를 리턴
	VOID					Free();																// 오브젝트 구조체를 날린다.
	CHAR					*GetObjectFileName( SI16 siIndex );									//오브젝트에서 스프라이트 파일 이름을 리턴을 해준다.


	ObjectHeader			*GetObjectHeader();													// 오브젝트 헤더를 리턴함
	ObjectHeader			*GetObject( SI16 siKind );											// KIND 와 동일한 오브텍트 헤더를 리턴함 
};

#endif