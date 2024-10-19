/************************************************************************************************
	���ϸ� : OnlineObjectKI.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_OBJECTKI_H
#define _ONLINE_OBJECTKI_H

#include <Online.h>

#define	ATB_OBJECT_VILLAGE			0x00000001		// ���� ������Ʈ�̴�.
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
#define	ATB_OBJECT_TRANS			0x02000000		// �����ϰ� ��� ������Ʈ



class	OnlineObjectKI
{
private:
	CHAR	*m_pszMark;		// Mark
	CHAR	szName[32];		// ������Ʈ �̸�
	SI32	siKind;			// ����.

	SI16	siFile;			// ȭ�� �ε���.
	SI16	siFont;			// ȭ�� ��Ʈ.
	SI16	siSprCenterX;	// 
	SI16	siSprCenterY;
	
	SI16	siXsize;		// ������Ʈ�� ���� �ϴ� X Ÿ��
	SI16	siYsize;		// ������Ʈ�� ���� �ϴ� Y Ÿ��

	SI32	siObjectAtb;	// ������Ʈ �Ӽ�.
	SI16	siTotalFrame;	// �� ������ �� 

	SI16	siVillageKind;	// ������ ����.
	
public:
	OnlineObjectKI();
	~OnlineObjectKI();

	// �ʱ�ȭ.
	VOID	Init(CHAR *pszName, SI32 siKind, SI16 siFile, SI16 siFont, SI16 siXsize, SI16 siYsize,
				 SI16 siSprCenterX, SI16 siSprCenterY, SI32 siObjectAtb, SI16 siVillageKind, SI16 siTotal, CHAR *pszMark);

	// �̸��� ���Ѵ�.
	VOID	GetName(CHAR pszName[32]);

	// ������ ���Ѵ�.
	CHAR	*GetMark(VOID);

	// ȭ���� ���Ѵ�.
	SI16	GetFile();

	// ��Ʈ�� ���Ѵ�.
	SI16	GetFont();

	// Xsize�� ���Ѵ�.
	SI16	GetXsize();

	// Ysize�� ���Ѵ�.
	SI16	GetYsize();

	// ��������Ʈ X �߽����� ���Ѵ�.
	SI16	GetSprCenterX();

	// ��������Ʈ Y �߽����� ���Ѵ�.
	SI16	GetSprCenterY();

	// �Ӽ��� ���´�.
	SI32	GetAtb()				{ return siObjectAtb; }

	// ���� ������ ���´�.
	SI16	GetVillageKind();

	// �� �������� ��´�.
	SI16	GetTotalFrame();

	// Kind�� ���´�.
	SI32	GetKind()	{return siKind;};
};

#endif

