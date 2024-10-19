/************************************************************************************************
	파일명 : OnlineObjectKI.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_OBJECTKI_H
#define _ONLINE_OBJECTKI_H

#include <Online.h>

#define	ATB_OBJECT_VILLAGE			0x00000001		// 마을 오브젝트이다.
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



class	OnlineObjectKI
{
private:
	CHAR	*m_pszMark;		// Mark
	CHAR	szName[32];		// 오브젝트 이름
	SI32	siKind;			// 종류.

	SI16	siFile;			// 화일 인덱스.
	SI16	siFont;			// 화일 폰트.
	SI16	siSprCenterX;	// 
	SI16	siSprCenterY;
	
	SI16	siXsize;		// 오브젝트가 차지 하는 X 타일
	SI16	siYsize;		// 오브젝트가 차지 하는 Y 타일

	SI32	siObjectAtb;	// 오브젝트 속성.
	SI16	siTotalFrame;	// 총 프래임 수 

	SI16	siVillageKind;	// 마을의 종류.
	
public:
	OnlineObjectKI();
	~OnlineObjectKI();

	// 초기화.
	VOID	Init(CHAR *pszName, SI32 siKind, SI16 siFile, SI16 siFont, SI16 siXsize, SI16 siYsize,
				 SI16 siSprCenterX, SI16 siSprCenterY, SI32 siObjectAtb, SI16 siVillageKind, SI16 siTotal, CHAR *pszMark);

	// 이름을 구한다.
	VOID	GetName(CHAR pszName[32]);

	// 설명을 구한다.
	CHAR	*GetMark(VOID);

	// 화일을 구한다.
	SI16	GetFile();

	// 폰트를 구한다.
	SI16	GetFont();

	// Xsize를 구한다.
	SI16	GetXsize();

	// Ysize를 구한다.
	SI16	GetYsize();

	// 스프라이트 X 중심점을 구한다.
	SI16	GetSprCenterX();

	// 스프라이트 Y 중심점을 구한다.
	SI16	GetSprCenterY();

	// 속성을 얻어온다.
	SI32	GetAtb()				{ return siObjectAtb; }

	// 마을 종류를 얻어온다.
	SI16	GetVillageKind();

	// 총 프래임을 얻는다.
	SI16	GetTotalFrame();

	// Kind를 얻어온다.
	SI32	GetKind()	{return siKind;};
};

#endif

