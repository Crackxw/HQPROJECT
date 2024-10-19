#ifndef	_ONLINE_RESOURCE_H
#define _ONLINE_RESOURCE_H

#include <graphics16.h>

#define	MAX_ONLINE_GAME_FILE				150			// �¶��ο��� ����ϴ� ���� ��������Ʈ(�ǳ� ������)
#define	MAX_ONLINE_TILE_FILE				8192		// �¶��ο��� ����ϴ� �� Ÿ�� ȭ�� ��.
#define	MAX_ONLINE_TILE_BASEDATAFILE		20			// �¶��ο��� ���� ����ϴ� Ÿ�� ȭ�� ��.(���� ȭ���� ����ִ� �������� ����ϴ� Tile, �� �� ��ŭ �̸� �޸𸮸� ���, �ʿ���� Tile�� ������, �ʿ��� Tile�� �ٽ� �ε��Ѵ�.)
#define	MAX_ONLINE_TILE_EXTDATAFILE			80			// MAX_ONLINE_TILE_BASEDATAFILE ���� ���ڶ� ��� ���� ���� �ִ� �ִ� ��(�� �κ��� �̸� �޸𸮸� ���� �ʰ�, �ʿ��Ҷ� ���� �޸𸮸� ��� �ʿ������ �׳� ������.
#define	MAX_ONLINE_TILEDATA_IMAGESIZE		358400
#define	MAX_ONLINE_IMOTICON_FILE			128			// �¶��ο��� ����ϴ� �� Imticon ȭ�� ��.
#define	MAX_ONLINE_ITEM_FILE				64			// �¶��ο��� ����ϴ� �� Item ȭ�� ��.
#define	MAX_ONLINE_FLAG_FILE				30			// �¶��ο��� ����ϴ� �� Flag ȭ�� ��.
#define	MAX_ONLINE_DATA_DELETCOUNT			30			// �¶��ο��� Load�� Data�� ������ Count
#define	MAX_ONLINE_OBJECT_FILE				8192		// �¶��ο��� ����ϴ� �� ������Ʈ ȭ�� ��.
#define	MAX_ONLINE_OBJECT_BASEDATAFILE		30			// �¶��ο��� ���� ����ϴ� Object ȭ�� ��.(���� ȭ���� ����ִ� �������� ����ϴ� Object, �� �� ��ŭ �̸� �޸𸮸� ���, �ʿ���� Object�� ������, �ʿ��� Object�� �ٽ� �ε��Ѵ�.)
#define	MAX_ONLINE_OBJECT_EXTDATAFILE		90			// MAX_ONLINE_OBJECT_BASEDATAFILE ���� ���ڶ� ��� ���� ���� �ִ� �ִ� ��, �ʿ��� �޸𸮰� Ŭ ��쿡�� �� Buffer�� ����Ѵ�.(�� �κ��� �̸� �޸𸮸� ���� �ʰ�, �ʿ��Ҷ� ���� �޸𸮸� ��� �ʿ������ �׳� ������.
#define	MAX_ONLINE_OBJECTDATA_IMAGESIZE		272144

// ���� ȭ��
#define ON_GAMESPR_DOWNTOWN_BOARDER			0			// ���ڰŸ� ����
#define	ON_GAMESPR_BALLOON					1			// ��ǳ��
#define	ON_GAMESPR_BATTLEFINDFFECT			2			// ���� Ÿ���� ������
#define	ON_GAMESPR_MINIMAP					3			// �̴ϸ�
#define	ON_GAMESPR_BATTLEWAITEFFECT			4			// ���� ��� ǥ��
#define	ON_GAMESPR_CHARACTERSLOT			5			// Character Slot
#define	ON_GAMESPR_MOUSE					6			// ���콺 ������.
#define	ON_GAMESPR_INVENTORY				7			// �κ��丮
#define	ON_GAMESPR_BOOKEXIT					8			// ��ο��� ������ ��ư
#define	ON_GAMESPR_NUMBER					9			// ���� �Է�â

#define	ON_GAMESPR_MSGBOXBOARD2				10			// �޽����ڽ� ����
#define	ON_GAMESPR_SCROLLH					11			// ���� ��ũ�ѹ�
#define	ON_GAMESPR_SCROLLV					12			// ���� ��ũ�ѹ�
#define	ON_GAMESPR_SCROLLH_UP				13			// ���� ��ũ�� ��ư(UP)
#define	ON_GAMESPR_SCROLLV_UP				14			// ���� ��ũ�� ��ư(UP)
#define	ON_GAMESPR_SCROLLH_DOWN				15			// ���� ��ũ�� ��ư(DOWN)
#define	ON_GAMESPR_SCROLLV_DOWN				16			// ���� ��ũ�� ��ư(DOWN)
#define ON_GAMESPR_VILLAGEBOARDER			17			// ���� ����
#define ON_GAMESPR_WEATHER					18			// ����ȿ����
#define ON_GAMESPR_LOGIN_BUTTON				19			// �α��ι�ư��

#define	ON_GAMESPR_NUMBERBUTTON				20			// �� �е忡�� ���� ��ư
#define	ON_GAMESPR_MSGBOXBOARD1				21			// �޽����ڽ� ����
#define	ON_GAMESPR_BLACKTILE				22			// Black Tile
#define	ON_GAMESPR_NUMBER_BUTTON1			23			// ���� �Է�â ��ư 1
#define	ON_GAMESPR_NUMBER_BUTTON2			24			// ���� �Է�â ��ư 2
#define	ON_GAMESPR_NUMBER_BUTTON3			25			// ���� �Է�â ��ư 3
#define	ON_GAMESPR_NUMBER_GAGEBAR			26			// ���� �Է�â ������ ��
#define	ON_GAMESPR_MINIMAP_ENLARGE			27			// �̴ϸ� Ȯ���ư
#define	ON_GAMESPR_MINIMAP_REDUCTION		28			// �̴ϸ� ��ҹ�ư
#define	ON_GAMESPR_MINIMAP_NORMAL			29			// �̴ϸ� �������

#define	ON_GAMESPR_MINIMAP_NUMBER			30			// �̴ϸ� ���ڹ�ư
#define	ON_GAMESPR_MINIMAP_TOGGLE			31			// �̴ϸ� ��Ҹ�� ��ư
#define	ON_GAMESPR_MINIMAP_MINI				32			// �̴ϸ� ��Ҹ�� ����
#define	ON_GAMESPR_BUTTON_M					33			// ��ư �߰� ũ��
#define	ON_GAMESPR_NUMBER_14X14				34			// ����
#define	ON_GAMESPR_MOVEPOINT				35			// ���콺 Ŭ���� ǥ�� 
#define	ON_GAMESPR_IMOTICON_SLOT			36			// �̸�Ƽ�� Slot
#define	ON_GAMESPR_WARCOUNT					37			// ���� Count
#define	ON_GAMESPR_GUILDEFFECT_HNS			38			// ��� ��� Effect
#define	ON_GAMESPR_GUILDEFFECT_DAI			39			// ��� ��� Effect

#define	ON_GAMESPR_NUMBERBIG_RED			40			// ������ ū ���� 
#define ON_GAMESPR_NUMBERBIG_GREEN			41			// ��� ū ���� 
#define ON_GAMESPR_VILLAGEGAGE				42			// ���� ����
#define ON_GAMESPR_UPBIG					43			// 
#define ON_GAMESPR_DOWNBIG					44			// 
#define ON_GAMESPR_CHATTING_BACK			45			// ä�õ޹��
#define ON_GAMESPR_VILLAGEEXPLOSION			46			// ���� ����
#define ON_GAMESPR_NOTICE_SCROLL_BAR		47			// ��ũ�� ��
#define ON_GAMESPR_WARPEFFECT				48			// Warp Effect
#define ON_GAMESPR_LEVEL_UP					49			// Status Level Up ��ư

#define ON_GAMESPR_EXITBUTTON				50			// ��� ������ ��ư
#define ON_GAMESPR_ITEM_EXP					51			// ����ġ Item Icon
#define ON_GAMESPR_ITEM_MONEY				52			// �� Item Icon
#define ON_GAMESPR_ITEM_CREDIT				53			// �ſ뵵 Item Icon
#define ON_GAMESPR_QUESTWINDOW				54			// Quest Window
#define ON_GAMESPR_MSGBOX_BUTTON			55			// MsgBox Ȯ��,��ҹ�ư
#define ON_GAMESPR_CHANGEMAN				56			// ���Ÿ��. ���ڿ�8
#define ON_GAMESPR_CHANGEWOMAN				57			// ���Ÿ��. ���ڿ�
#define ON_GAMESPR_CHANGEEFFECT				58			// ����Effect
#define	ON_GAMESPR_GUILDEFFECT_DHS			59			// ��� ����� Effect

#define	ON_GAMESPR_GUILDEFFECT_HND			60			// ��� �ൿ���� Effect
#define ON_GMAESPR_LOGIN_SCROLL_BAR			61			// �α��ο� ���̴� ��ũ�ѹ�
#define ON_GAMESPR_LOGIN_MESSAGE_BOX1		62			// �α��� �޼��� �ڽ�1
#define ON_GAMESPR_LOGIN_MESSAGE_BOX2		63			// �α��� �޼��� �ڽ�2
#define ON_GAMESPR_LOGIN_MESSAGE_BUTTON		64			// �α��� �ż��� �ڽ� ��ư.
#define ON_GAMESPR_PANNEL_SCROLL_BAR		65

#define ON_GAMESPR_QUEST_SCR_HANDLE			66
#define ON_GAMESPR_GUARDIAN_EFFECT			67			// ��ȣ�� ���� ����Ʈ

// robypark 2004/10/29 14:15
// ������ ȿ������ ���ҽ� �߰�
#define ON_GAMESPR_SIEGEWARFARE_TRANSFORM	68			// �Ϲ� ���ΰ� ���� <==> ������ ���� ���� ����Ʈ

// robypark 2005/2/18 18:14
// ������ �� ��� ���� ü�� ������(%�� ǥ��)
#define ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY					69

// robypark 2004/11/9 19:44
// ������ ���� ȿ�� ���� �޴� ĳ���� ǥ�� ���ҽ� �߰�
// ������� �Ʊ��� ��� �Ķ��� �迭
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_BLUE	70
   
// ������� ������ ��� ������ �迭
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_RED		71

// ������� �����ܵ� ���� ��ܵ� �ƴ� ���
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_UNRELATED	72

// robypark 2004/11/10 15:58
// ������ ���� ĳ���� ǥ�� ���ҽ� �߰�
// ������� �Ʊ��� ��� �Ķ��� �迭
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_BLUE			73

// ������� ������ ��� ������ �迭
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_RED			74
 
// ������� �����ܵ� ���� ��ܵ� �ƴ� ���
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_UNRELATED	75

// robypark 2004/11/24 18:39
// ������ ȿ������ ���ҽ� �߰�
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION				76			// �߰��� ��� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW			77			// ġ���� ���� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL			78			// ���� �ݰ� �⸧ ���� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE		79			// ���� �ݰ� �� ���� ����Ʈ

// robypark 2004/12/12 18:7
// ������ ���� ȿ�� ���ҽ� �߰�
#define ON_GAMESPR_SIEGEWARFARE_RETRANSFORM					80			// ������ ������ ���¿��� ���� �������� ����Ͽ� ������ ��� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT	81			// ������ ������ ����ǰ�� �ٴڳ� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED	82			// ������ ������ ����ǰ�� �޾Ҵ�. ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_DEFENCE_UP	83			// ���� ���� ��� ����Ʈ
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_OCCUPIED_VILLAGE		84			// ���� ���� ȿ��

// robypark 2004/12/12 11:10
#define ON_GAMESPR_EFFECT_ENABLED_OCCUPY_VILLAGE			85			// �˺����� ������ �����Ͽ� �����϶�� ����Ʈ
#define ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE		86			// ĳ���Ͱ� �����������̽� �ȿ� �ִ� ��� ĳ���� ��ſ� �׷�����.

// robypark 2005/1/7 15:53
// ������ ���� �̹���
// ������ ���� ü�� ������(%�� ǥ��)
#define ON_GAMESPR_GAUGE_UNIT_HEALTH						87
// ������ ���� ����ǰ ������(%�� ǥ��)
#define ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT			88
// ������ ���� ü��, ����ǰ ������ ��� �̹���
#define ON_GAMESPR_GAUGE_UNIT_BACKGROUND					89
// ���� ���� �� ������
#define ON_GAMESPR_GAUGE_VILLAGE_DEFENCE					90
// ���� ���� �� ������ ��� �̹���
#define ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND			91
// ���� �ֵб� �� ������
#define ON_GAMESPR_GAUGE_VILLAGE_GARRISON					92
// ���� �ֵб� �� ������ ��� �̹���
#define ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND		93

// dead penalty�� ���� ���׸���� ���� �̹���
#define ON_GAMESPR_ITEM_OVER_DEAD_PENALTY					94

// robypark 2005/2/16 14:39
// ��� ����
#define ON_GAMESPR_NUMBER_GREEN								95
// ���� ����
#define ON_GAMESPR_NUMBER_RED								96
// û�� ����
#define ON_GAMESPR_NUMBER_BLUE								97
// ��� ����
#define ON_GAMESPR_NUMBER_WHITE								98

// robypark 2004/11/4 15:52
// ������ ���� �ٴ� ȿ�� ���ҽ� �߰�
#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_EFFECT				99		// �߸� ���� ���� ��ȯ ����Ʈ

#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_BLUE_EFFECT			100		// �Ʊ� ���� ���� ��ȯ ����Ʈ

#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_RED_EFFECT			101		// ���� ���� ���� ��ȯ ����Ʈ

// �ε� ����
#define ON_LOAD_KIND_TI				1

typedef struct
{
	SI16			m_TileIndex;						// Tile Index(-1 �� ���� ���̴�.)
	SI16			m_YetTileIndex;						// Tile Index(-1 �� ���� ���̴�.)
	SI32			m_LastUseCount;						// ���� �� ���� �ð�(����Ҷ� ���� �ٽ� �����Ѵ�.)

	XSPR			m_XSPR;								// Image
	UI08*			m_ImageBuffer;						// ���� �̹����� �� Buffer
} OnlineResource_BaseData;

typedef struct
{
	SI16			m_TileIndex;						// Tile Index(-1 �� ���� ���̴�.)
	SI16			m_YetTileIndex;						// Tile Index(-1 �� ���� ���̴�.)
	SI32			m_LastUseCount;						// ���� �� ���� �ð�(����Ҷ� ���� �ٽ� �����Ѵ�.)

	XSPR			m_XSPR;								// Image
} OnlineResource_ExtData;

typedef struct
{
	SI16			m_VillageSize;						// ������ Size(0 : ū����, 1 : �߰�����, 2 : ��������)
	SI16			m_VillageState;						// ������ �����ڵ�
	SI16			m_VillageFirePer;					// ���̳�����

	SI16			m_FireCount;						// �Ҽ�
	SI16			m_FirePositionX[16];				// ����ġ
	SI16			m_FirePositionY[16];				// ����ġ
	SI16			m_FireSize[16];						// �� ũ��
} OnlineResource_VillageFireData;

// robypark 2004/12/28 16:4
// ������ �׸� �� �̹������� ��ġ ���� �����ϴ� ����ü.
// VillageImagePositionInfo.txt���Ϸ� �����Ͽ� ����ϱ� ����. hq\\Online\\Village\\VillageImagePositionInfo.txt
typedef struct _OnlineResource_VillageImagePositionInfo
{
	SI16			m_ssVillageState;						// ������ �����ڵ�
	SI16			m_ssVillageSize;						// ������ Size(0 : ū����, 1 : �߰�����, 2 : ��������)

	// Ÿ�� ��ǥ �ƴ�. ���� �߽� Ÿ�Ͽ��� ������ �Ÿ�(��ȯ�� ���� dot �Ÿ�)
	SI16			m_ssVillageGuildFlagPosX;				// ��� ��� ��ġ
	SI16			m_ssVillageGuildFlagPosY;				// ��� ��� ��ġ

	// Ÿ�� ��ǥ �ƴ�. ���� �߽� Ÿ�Ͽ��� ������ �Ÿ�(��ȯ�� ���� dot �Ÿ�)
	SI16			m_ssSurrenderFlagPosX;					// ���� ���� ��� ��ġ
	SI16			m_ssSurrenderFlagPosY;					// ���� ���� ��� ��ġ

	// Ÿ�� ��ǥ �ƴ�. ���� �߽� Ÿ�Ͽ��� ������ �Ÿ�(��ȯ�� ���� dot �Ÿ�)
	SI16			m_ssVillageDrawCenterX;					// ������ �׷����� ������ǥ
	SI16			m_ssVillageDrawCenterY;					// ������ �׷����� ������ǥ

	// ���� �߽�(��ȯ�� dot)���� ���� ������ �Ÿ��� ��ġ�ϴ� ��
	SI16			m_ssVillageGarrisionGaugePosX;			// ���� �ֵб�(����) ǥ�� ������ ��ġ
	SI16			m_ssVillageGarrisionGaugePosY;			// ���� �ֵб�(����) ǥ�� ������ ��ġ
	SI16			m_ssVillageDefenceGaugePosX;			// ���� ���� ǥ�� ������ ��ġ
	SI16			m_ssVillageDefenceGaugePosY;			// ���� ���� ǥ�� ������ ��ġ
	SI16			m_ssEnabledOccupyVillagePosX;			// �������� �����Ͽ� ���� ������ �϶�� �̹��� ��ġ
	SI16			m_ssEnabledOccupyVillagePosY;			// �������� �����Ͽ� ���� ������ �϶�� �̹��� ��ġ

} OnlineResource_VillageImagePositionInfo, *LPOnlineResource_VillageImagePositionInfo;

class OnlineResource
{
private:
	char			m_TempBuffer[256];										// ���ο��� ����ϴ��ӽ� Buffer

	XSPR			xsprOnlineGame[MAX_ONLINE_GAME_FILE];					// ���ӿ� ��������Ʈ.
	XSPR			xsprOnlineImoticon[MAX_ONLINE_IMOTICON_FILE];			// �̸�Ƽ�ܿ� ��������Ʈ.
	XSPR			xsprOnlineFlagM[MAX_ONLINE_FLAG_FILE];					// ��߿� ��������Ʈ.
	XSPR			xsprOnlineFlagV[MAX_ONLINE_FLAG_FILE];					// ��߿� ��������Ʈ.
	XSPR			xsprOnlineItem[MAX_ONLINE_ITEM_FILE];					// Item�� ��������Ʈ.
	XSPR			xsprOnlineVillageEffect[1][1][3];						// Village Effect�� ��������Ʈ.
	XSPR			xsprOnlineVillageAttackEffect[3];						// Village Attack Effect�� ��������Ʈ.

	static	CHAR	szOnlineGameFile[MAX_ONLINE_GAME_FILE][128];			// ĳ���� ��������Ʈ�� ȭ�ϸ�
	static	CHAR	szOnlineTileFile[MAX_ONLINE_TILE_FILE][128];			// Ÿ�Ͽ� ��������Ʈ�� ȭ�ϸ�
	SI16			szOnlineTileFilePos[MAX_ONLINE_TILE_FILE];
	static	CHAR	szOnlineItemFile[MAX_ONLINE_ITEM_FILE][128];			// Item�� ��������Ʈ�� ȭ�ϸ�
	static	CHAR	szOnlineObjectFile[MAX_ONLINE_OBJECT_FILE][128];		// ������Ʈ�� ��������Ʈ�� ȭ�ϸ�
	SI16			szOnlineObjectFilePos[MAX_ONLINE_OBJECT_FILE];

	SI16			siCount;												// �� �ɸ��� ��������Ʈ ��

	SI16							siVillageFirePositionCount;				// ������ ��Ż�� ��Ÿ�� ��ġ ��
	OnlineResource_VillageFireData*	sVillageFirePosition;					// ������ ��Ż�� ��Ÿ�� ��ġ

	// robypark 2004/12/28 17:13
	SI16										m_ssCntVillageImagePositionInfo;	// ���� �׸� �� ���Ǵ� �̹����� ������ ���� ����
	LPOnlineResource_VillageImagePositionInfo	m_lpsVillageImagePositionInfo;		// ���� �׸� �� ���Ǵ� �̹����� ������ ���� ����

public:
	OnlineResource_BaseData		ImageBaseTileData[MAX_ONLINE_TILE_BASEDATAFILE];		// Tile Data
	OnlineResource_ExtData		ImageExtTileData[MAX_ONLINE_TILE_EXTDATAFILE];			// Tile Data
	OnlineResource_BaseData		ImageBaseObjectData[MAX_ONLINE_OBJECT_BASEDATAFILE];	// Object Data
	OnlineResource_ExtData		ImageExtObjectData[MAX_ONLINE_OBJECT_EXTDATAFILE];		// Object Data

public:
	OnlineResource();											// ������
	~OnlineResource();											// �Ҹ���
	BOOL	Init();												// �¶��ο��� ���Ǵ� ���ҽ�(ȭ�ϵ�)�� �ε��Ѵ�.
	VOID	LoadCharSpr( char *pDirectory,  char *pPolder );	// �ɸ��� ��������Ʈ ������ ã�Ƽ� �ε���
	SI32	AddItemSpr( char *pFileName );						// �������� �ε���(�ʱ⿡ �ε����� �ʰ� �߰��� �ε� �Ҽ� ����)
	VOID	Free();												// ����.
	SI32	GetCharFileIndex(CHAR *pszFileName);				// ĳ���� ��������Ʈ �迭�� �ε����� ���´�.
	SI32	GetObjectFileIndex(CHAR *pszFileName);				// ������Ʈ ��������Ʈ �迭�� �ε����� ���´�.
	SI32	GetGameFileIndex(CHAR *pszFileName);				// ���� ��������Ʈ �迭�� �ε����� ���´�.
	SI32	GetItemFileIndex(CHAR *pszFileName);				// Item ��������Ʈ �迭�� �ε����� ���´�.

	XSPR	*GetxsprGame(SI16 siIndex);							// private(���ӿ� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprTile(SI16 siIndex);							// private(Ÿ�Ͽ� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprObject(SI16 siIndex);						// private(������Ʈ�� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprImoticon(SI16 siIndex);						// private(�̸�Ƽ�ܿ� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprItem(SI16 siIndex);							// private(Item�� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprFlagM(SI16 siIndex);						// private(���(���̵� �տ� �ٴ°�)�� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprFlagV(SI16 siIndex);						// private(���(�����ȿ���)�� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	XSPR	*GetxsprVillageEffect(SI16 siSize, SI16 siPercent);	// private(���� ȿ���� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.
	OnlineResource_VillageFireData*	GetVillageFirePosition(SI16 VillageSize, SI16 State, SI16 Per);	// ������ ��Ż�� ��Ÿ�� ��ġ
	XSPR	*GetxsprVillageAttackEffect(SI16 Type);				// private(���� ���� ȿ���� ��������Ʈ)�� ������ �� �ִ� �Լ��� �����.

	BOOL	Prepare(void);										// Prepare

	//-----------------------------------------------
	// ��ȯ�̲�
	//-----------------------------------------------
	BOOL	LoadInformationDirectoryRecurs(char * pCurPath, SI32 siKind);

	// robypark 2004/12/28 17:13
	// desc: ���� �׸� �� ���Ǵ� �̹����� ������ ���� ���� HQ���� �б�
	BOOL	LoadVillageImagePositionInfoFile(void);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/28 17:13
	// desc: ���� �׸� �� ���Ǵ� �̹����� ���ϸ� �� ���� ũ�� ���
	// parameter:
	// SI16	ssVillageState[IN]: ������ �����ڵ�
	// char	*pszVillageSizeImageFilename[IN]: ������ ũ�⸦ ��Ÿ���� �̹��� ���� ��(town49.spr, town25.spr, town9.spr)
	// SI16	&ssVillageSize[OUT]: ������ ũ�Ⱚ(0 : ū����, 1 : �߰�����, 2 : ��������)
	// char *pszVillageImageFilename[OUT]: ���� �̹��� ���ϸ�. ���� ������ 70% �̻��� ���
	// char *pszBrokenVillageImageFilename[OUT]: �μ��� ���� �̹��� ���ϸ�. ���� ������ 70%�̸��� ���
	//----------------------------------------------------------------------------------------------
	void	GetVillageImageFilename(SI16 ssVillageState, char *pszVillageSizeImageFilename, SI16 &ssVillageSize,
											char *pszVillageImageFilename, char *pszBrokenVillageImageFilename);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/28 17:13
	// desc: ���� �׸� �� ���Ǵ� �̹����� ������ ���� ��� ���
	// parameter:
	// SI16	ssVillageState[IN]: ������ �����ڵ�
	// SI16	ssVillageSize[IN]: ������ ũ�Ⱚ(0 : ū����, 1 : �߰�����, 2 : ��������)
	//----------------------------------------------------------------------------------------------
	LPOnlineResource_VillageImagePositionInfo	GetVillageImagePositionInfoHeader(SI16 ssVillageState, SI16 ssVillageSize);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/29 17:26
	// desc: ���� ũ�� ���
	// parameter:
	// char	*pszVillageSizeImageFilename[IN]: ������ ũ�⸦ ��Ÿ���� �̹��� ���� ��(town49.spr, town25.spr, town9.spr)
	//----------------------------------------------------------------------------------------------
	SI16	GetVillageSize(char *pszVillageSizeImageFilename);

};


#endif

