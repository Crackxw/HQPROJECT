#ifndef __ON_OBJECTPARSER_H
#define __ON_OBJECTPARSER_H


/********************************************************************************************
	���ϸ�	:	OnlineObjectParser.cpp
	�ۼ���	:	�� �� ��
	�ۼ���	:	2001.07.24
	������	:	2001.07.25
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
#define	ATB_OBJECT_TRANS			0x02000000		// �����ϰ� ��� ������Ʈ


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
	ObjectHeader			*pObjectHeader;														// ������Ʈ ����ü
	SI16					siObjectNum;														// ��� ������Ʈ�� �ֳ�~

public:
	OnlineObjectParser();
	~OnlineObjectParser();

	BOOL					Init( CHAR *pFileName );											// �ؽ��� �޾� �б�
	SI16					GetObjectNum();														// ������Ʈ �ټ��� ����
	VOID					Free();																// ������Ʈ ����ü�� ������.
	CHAR					*GetObjectFileName( SI16 siIndex );									//������Ʈ���� ��������Ʈ ���� �̸��� ������ ���ش�.


	ObjectHeader			*GetObjectHeader();													// ������Ʈ ����� ������
	ObjectHeader			*GetObject( SI16 siKind );											// KIND �� ������ ������Ʈ ����� ������ 
};

#endif