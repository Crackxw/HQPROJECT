/************************************************************************************************
	���ϸ� : OnlineObject.h
	����� : ������
************************************************************************************************/
#ifndef _ONLINE_OBJECT_H
#define _ONLINE_OBJECT_H

#include "OnlineVillage-Parser.h"

// OnlineObject ���� �������� ����ϴ� ����Ÿ.
struct	OnlineVillageDataOfObject
{
	UI16				uiVillageCode;
	UI08				uiVillageStateCode;
	VillageHeader*		pVillageHeader;
	CHAR				szVillageFileName[1024];
	CHAR				szVillageFileNameHalfE[1024];

	SI16				siGuildFlagXPos;
	SI16				siGuildFlagYPos;

	SI16				siVillageSize;
	SI16				siVillageExplosionFlag;
	DWORD				siVillageExplosionSoundWait;

	// robypark 2004/12/28 19:3
	// Ÿ�� ��ǥ �ƴ�. ���� �߽� Ÿ�Ͽ��� ������ �Ÿ�(��ȯ�� ���� dot �Ÿ�)
	SI16				m_ssSurrenderFlagPosX;					// ���� ���� ��� ��ġ
	SI16				m_ssSurrenderFlagPosY;					// ���� ���� ��� ��ġ

	SI16				m_ssEnabledOccupyVillagePosX;			// �������� �����Ͽ� ���� ������ �϶�� �̹��� ��ġ
	SI16				m_ssEnabledOccupyVillagePosY;			// �������� �����Ͽ� ���� ������ �϶�� �̹��� ��ġ

	// ���� �߽�(��ȯ�� dot)���� ���� ������ �Ÿ��� ��ġ�ϴ� ��
	SI16				m_ssVillageGarrisionGaugePosX;			// ���� �ֵб�(����) ǥ�� ������ ��ġ
	SI16				m_ssVillageGarrisionGaugePosY;			// ���� �ֵб�(����) ǥ�� ������ ��ġ
	SI16				m_ssVillageDefenceGaugePosX;			// ���� ���� ǥ�� ������ ��ġ
	SI16				m_ssVillageDefenceGaugePosY;			// ���� ���� ǥ�� ������ ��ġ
};

// OnlineObject ���� Portal���� ����ϴ� ����Ÿ.
struct	OnlinePortalDataOfObject
{
	SI16	siPortalCode;
	UI08	uifTileDraw;
	CHAR	szPortalFileName[1024];
};


class	OnlineObject
{
private:
	CHAR	szName[32];
	CHAR	*m_pszMark;

	SI16	siID;
	SI16	siKind;
	SI32	siObjectKindIndex;
	SI32	siAtb;				// �Ӽ�.
	SI16	siTotalFrame;		// �� �� ���������� �ִ� ���̼��� �Ǵ°�?

	SI16	siFile;
	SI16	siFont;	
	SI16	siFile2;
	SI16	siFont2;	
	SI16	siSprCenterX;
	SI16	siSprCenterY;

	SI16	siX;
	SI16	siY;

	SI16	siXsize;
	SI16	siYsize;

	SI16	siVillageKind;		// �����̶�� � �����ΰ�?

	
	OnlineVillageDataOfObject*	pOnlineVillageDataOfObject;
	OnlinePortalDataOfObject*	pOnlinePortalDataOfObject;

	cltOnlineWorld	*pMyOnlineWorld;		
	
public:
	OnlineObject();
	~OnlineObject();

	// �ʱ�ȭ.
	VOID	Init(SI16 siID, SI16 siKind, SI16 siX, SI16 siY, cltOnlineWorld *pOnlineWorld);	

	// �ʱ�ȭ.(������ ���)
	VOID	Init(SI16 siID, VillageHeader *pVillageHeader, cltOnlineWorld *pOnlineWorld);	

	// �ʱ�ȭ.(Portal�� ���)
	VOID	Init(SI16 siID, PortalHeader *pPortalHeader, cltOnlineWorld *pOnlineWorld);	

	// �����Ѵ�.
	VOID	Free();

	// �ǹ��� �̸��� ���´�.
	VOID	GetName(CHAR pszName[32]);

	// ������ ���´�.
	CHAR	*GetMark(VOID);

	// �����̶�� ������ ���´�.
	SI16	GetVillageKind();

	// �ʱ�ȭ�� �Ǿ��°�?
	BOOL	IsInit()			{ return pMyOnlineWorld != NULL ? TRUE : FALSE; }

	// ȭ�鿡 �׸���.
	VOID	Draw(SI16 SelectID); 
	VOID	DrawTileType(SI16 SelectID); 

	// robypark 2005/1/7 20:7
	// ���� ���� ������ ���
	VOID	DrawGaugeVillageDefence(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// robypark 2005/1/7 20:7
	// ���� ���� ������ ���
	VOID	DrawGaugeVillageGarrison(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// robypark 2005/1/7 20:7
	// ���� �˺����� �����ϴ� ����Ʈ ���
	VOID	DrawEffectEnabledOccupyVillage(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// �׼�.
	VOID	Action();

	// �Ӽ��� �ִ��� ���´�.
	BOOL	IsAtb(SI32 siAtb);	

	// �ǹ��� �ִ� ��ġ�� ���´�.
	VOID	GetPos(SI16 *psiX, SI16 *psiY);

	// �ǹ��� �����ϴ� ������ ���´�.
	VOID	GetSize(SI16 *psiXsize, SI16 *psiYsize);

	// ������ ��� ������ ����Ǵ� ȭ���� ���´�.
	VOID	GetVillageFileName(CHAR *pszVillageFileName);

	// ���׸� ��� ���� ������ ȭ�Ͽ� ����.
	VOID	WriteFileForBug(FILE *fp);

	// ������Ʈ�� ������ ���´�
	SI16	GetKind();
	SI32	GetKindIndex()	{ return siObjectKindIndex; }

	// ������Ʈ�� ��Ż���� �˻��Ѵ�.
	BOOL	IsPortal();

	// Portal ID�� �����ش�.
	SI16	GetPortalID(void);

	// Portal Name�� �����ش�.
	char*	GetPortalName(void);

	// Village Code�� �����ش�.
	UI16	GetVillageCode(void);

	// x, y ��ġ�� Image�� �ִ��� �˻��Ѵ�.
	BOOL	CheckImage(SI16 x, SI16 y, LPDIRECTDRAWSURFACE7 pSurface);

	// Tile Draw ���� �˻�
	BOOL	CheckTileDraw(void)
	{
		if(pOnlinePortalDataOfObject)
			if(pOnlinePortalDataOfObject->uifTileDraw)
				return TRUE;

		return FALSE;
	}
};

#endif
