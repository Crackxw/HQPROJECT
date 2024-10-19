#ifndef	_ONLINE_RESOURCE_H
#define _ONLINE_RESOURCE_H

#include <graphics16.h>

#define	MAX_ONLINE_GAME_FILE				150			// 온라인에서 사용하는 게임 스프라이트(판넬 따위등)
#define	MAX_ONLINE_TILE_FILE				8192		// 온라인에서 사용하는 총 타일 화일 수.
#define	MAX_ONLINE_TILE_BASEDATAFILE		20			// 온라인에서 실제 사용하는 타일 화일 수.(현재 화면을 찍어주는 기준으로 사용하는 Tile, 이 수 많큼 미리 메모리를 잡고, 필요없는 Tile은 버리고, 필요한 Tile을 다시 로딩한다.)
#define	MAX_ONLINE_TILE_EXTDATAFILE			80			// MAX_ONLINE_TILE_BASEDATAFILE 에서 모자랄 경우 쓸때 쓸수 있는 최대 수(이 부분은 미리 메모리를 잡지 않고, 필요할때 마다 메모리를 잡고 필요없으면 그냥 버린다.
#define	MAX_ONLINE_TILEDATA_IMAGESIZE		358400
#define	MAX_ONLINE_IMOTICON_FILE			128			// 온라인에서 사용하는 총 Imticon 화일 수.
#define	MAX_ONLINE_ITEM_FILE				64			// 온라인에서 사용하는 총 Item 화일 수.
#define	MAX_ONLINE_FLAG_FILE				30			// 온라인에서 사용하는 총 Flag 화일 수.
#define	MAX_ONLINE_DATA_DELETCOUNT			30			// 온라인에서 Load한 Data를 삭제할 Count
#define	MAX_ONLINE_OBJECT_FILE				8192		// 온라인에서 사용하는 총 오브젝트 화일 수.
#define	MAX_ONLINE_OBJECT_BASEDATAFILE		30			// 온라인에서 실제 사용하는 Object 화일 수.(현재 화면을 찍어주는 기준으로 사용하는 Object, 이 수 많큼 미리 메모리를 잡고, 필요없는 Object은 버리고, 필요한 Object을 다시 로딩한다.)
#define	MAX_ONLINE_OBJECT_EXTDATAFILE		90			// MAX_ONLINE_OBJECT_BASEDATAFILE 에서 모자랄 경우 쓸때 쓸수 있는 최대 수, 필요한 메모리가 클 경우에도 이 Buffer를 사용한다.(이 부분은 미리 메모리를 잡지 않고, 필요할때 마다 메모리를 잡고 필요없으면 그냥 버린다.
#define	MAX_ONLINE_OBJECTDATA_IMAGESIZE		272144

// 게임 화일
#define ON_GAMESPR_DOWNTOWN_BOARDER			0			// 저자거리 간판
#define	ON_GAMESPR_BALLOON					1			// 말풍선
#define	ON_GAMESPR_BATTLEFINDFFECT			2			// 전투 타켓을 잡을시
#define	ON_GAMESPR_MINIMAP					3			// 미니맵
#define	ON_GAMESPR_BATTLEWAITEFFECT			4			// 전투 대기 표시
#define	ON_GAMESPR_CHARACTERSLOT			5			// Character Slot
#define	ON_GAMESPR_MOUSE					6			// 마우스 모음집.
#define	ON_GAMESPR_INVENTORY				7			// 인벤토리
#define	ON_GAMESPR_BOOKEXIT					8			// 장부에서 나가기 버튼
#define	ON_GAMESPR_NUMBER					9			// 숫자 입력창

#define	ON_GAMESPR_MSGBOXBOARD2				10			// 메시지박스 보드
#define	ON_GAMESPR_SCROLLH					11			// 가로 스크롤바
#define	ON_GAMESPR_SCROLLV					12			// 세로 스크롤바
#define	ON_GAMESPR_SCROLLH_UP				13			// 가로 스크롤 버튼(UP)
#define	ON_GAMESPR_SCROLLV_UP				14			// 세로 스크롤 버튼(UP)
#define	ON_GAMESPR_SCROLLH_DOWN				15			// 가로 스크롤 버튼(DOWN)
#define	ON_GAMESPR_SCROLLV_DOWN				16			// 세로 스크롤 버튼(DOWN)
#define ON_GAMESPR_VILLAGEBOARDER			17			// 마을 간판
#define ON_GAMESPR_WEATHER					18			// 날씨효과들
#define ON_GAMESPR_LOGIN_BUTTON				19			// 로그인버튼들

#define	ON_GAMESPR_NUMBERBUTTON				20			// 넘 패드에서 숫자 버튼
#define	ON_GAMESPR_MSGBOXBOARD1				21			// 메시지박스 보드
#define	ON_GAMESPR_BLACKTILE				22			// Black Tile
#define	ON_GAMESPR_NUMBER_BUTTON1			23			// 숫자 입력창 버튼 1
#define	ON_GAMESPR_NUMBER_BUTTON2			24			// 숫자 입력창 버튼 2
#define	ON_GAMESPR_NUMBER_BUTTON3			25			// 숫자 입력창 버튼 3
#define	ON_GAMESPR_NUMBER_GAGEBAR			26			// 숫자 입력창 게이지 바
#define	ON_GAMESPR_MINIMAP_ENLARGE			27			// 미니맵 확대버튼
#define	ON_GAMESPR_MINIMAP_REDUCTION		28			// 미니맵 축소버튼
#define	ON_GAMESPR_MINIMAP_NORMAL			29			// 미니맵 보통비율

#define	ON_GAMESPR_MINIMAP_NUMBER			30			// 미니맵 숫자버튼
#define	ON_GAMESPR_MINIMAP_TOGGLE			31			// 미니맵 축소모드 버튼
#define	ON_GAMESPR_MINIMAP_MINI				32			// 미니맵 축소모드 바탕
#define	ON_GAMESPR_BUTTON_M					33			// 버튼 중간 크기
#define	ON_GAMESPR_NUMBER_14X14				34			// 숫자
#define	ON_GAMESPR_MOVEPOINT				35			// 마우스 클릭시 표시 
#define	ON_GAMESPR_IMOTICON_SLOT			36			// 이모티콘 Slot
#define	ON_GAMESPR_WARCOUNT					37			// 전투 Count
#define	ON_GAMESPR_GUILDEFFECT_HNS			38			// 길드 행수 Effect
#define	ON_GAMESPR_GUILDEFFECT_DAI			39			// 길드 대방 Effect

#define	ON_GAMESPR_NUMBERBIG_RED			40			// 붉은색 큰 숫자 
#define ON_GAMESPR_NUMBERBIG_GREEN			41			// 녹색 큰 숫자 
#define ON_GAMESPR_VILLAGEGAGE				42			// 마을 방어력
#define ON_GAMESPR_UPBIG					43			// 
#define ON_GAMESPR_DOWNBIG					44			// 
#define ON_GAMESPR_CHATTING_BACK			45			// 채팅뒷배경
#define ON_GAMESPR_VILLAGEEXPLOSION			46			// 마을 폭발
#define ON_GAMESPR_NOTICE_SCROLL_BAR		47			// 스크롤 바
#define ON_GAMESPR_WARPEFFECT				48			// Warp Effect
#define ON_GAMESPR_LEVEL_UP					49			// Status Level Up 버튼

#define ON_GAMESPR_EXITBUTTON				50			// 모든 나가기 버튼
#define ON_GAMESPR_ITEM_EXP					51			// 겸험치 Item Icon
#define ON_GAMESPR_ITEM_MONEY				52			// 돈 Item Icon
#define ON_GAMESPR_ITEM_CREDIT				53			// 신용도 Item Icon
#define ON_GAMESPR_QUESTWINDOW				54			// Quest Window
#define ON_GAMESPR_MSGBOX_BUTTON			55			// MsgBox 확인,취소버튼
#define ON_GAMESPR_CHANGEMAN				56			// 변신모습. 남자용8
#define ON_GAMESPR_CHANGEWOMAN				57			// 변신모습. 여자용
#define ON_GAMESPR_CHANGEEFFECT				58			// 변신Effect
#define	ON_GAMESPR_GUILDEFFECT_DHS			59			// 길드 대행수 Effect

#define	ON_GAMESPR_GUILDEFFECT_HND			60			// 길드 행동대장 Effect
#define ON_GMAESPR_LOGIN_SCROLL_BAR			61			// 로그인에 쓰이는 스크롤바
#define ON_GAMESPR_LOGIN_MESSAGE_BOX1		62			// 로그인 메세지 박스1
#define ON_GAMESPR_LOGIN_MESSAGE_BOX2		63			// 로그인 메세지 박스2
#define ON_GAMESPR_LOGIN_MESSAGE_BUTTON		64			// 로그인 매세지 박스 버튼.
#define ON_GAMESPR_PANNEL_SCROLL_BAR		65

#define ON_GAMESPR_QUEST_SCR_HANDLE			66
#define ON_GAMESPR_GUARDIAN_EFFECT			67			// 수호부 착용 이펙트

// robypark 2004/10/29 14:15
// 공성전 효과관련 리소스 추가
#define ON_GAMESPR_SIEGEWARFARE_TRANSFORM	68			// 일반 주인공 유닛 <==> 공성전 유닛 변신 이펙트

// robypark 2005/2/18 18:14
// 공성전 적 상단 유닛 체력 게이지(%로 표시)
#define ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY					69

// robypark 2004/11/9 19:44
// 공성전 대장 효과 영향 받는 캐릭터 표시 리소스 추가
// 사용자의 아군일 경우 파란색 계열
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_BLUE	70
   
// 사용자의 적군일 경우 붉은색 계열
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_RED		71

// 사용자의 적대상단도 같은 상단도 아닌 경우
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_UNRELATED	72

// robypark 2004/11/10 15:58
// 공성전 대장 캐릭터 표시 리소스 추가
// 사용자의 아군일 경우 파란색 계열
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_BLUE			73

// 사용자의 적군일 경우 붉은색 계열
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_RED			74
 
// 사용자의 적대상단도 같은 상단도 아닌 경우
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_UNRELATED	75

// robypark 2004/11/24 18:39
// 공성전 효과관련 리소스 추가
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION				76			// 견고한 방어 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW			77			// 치명적 공격 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL			78			// 마을 반격 기름 공격 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE		79			// 마을 반격 돌 공격 이펙트

// robypark 2004/12/12 18:7
// 공성전 관련 효과 리소스 추가
#define ON_GAMESPR_SIEGEWARFARE_RETRANSFORM					80			// 공성전 유닛인 상태에서 재차 소집권을 사용하여 변신할 경우 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT	81			// 공성전 유닛의 보급품이 바닥난 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED	82			// 공성전 유닛이 보급품을 받았다. 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_DEFENCE_UP	83			// 마을 방어력 향상 이펙트
#define ON_GAMESPR_SIEGEWARFARE_EFFECT_OCCUPIED_VILLAGE		84			// 마을 점령 효과

// robypark 2004/12/12 11:10
#define ON_GAMESPR_EFFECT_ENABLED_OCCUPY_VILLAGE			85			// 검병으로 마을을 공격하여 점령하라는 이펙트
#define ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE		86			// 캐릭터가 수성인터페이스 안에 있는 경우 캐릭터 대신에 그려진다.

// robypark 2005/1/7 15:53
// 게이지 관련 이미지
// 공성전 유닛 체력 게이지(%로 표시)
#define ON_GAMESPR_GAUGE_UNIT_HEALTH						87
// 공성전 유닛 보급품 게이지(%로 표시)
#define ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT			88
// 공성전 유닛 체력, 보급품 게이지 배경 이미지
#define ON_GAMESPR_GAUGE_UNIT_BACKGROUND					89
// 마을 방어력 량 게이지
#define ON_GAMESPR_GAUGE_VILLAGE_DEFENCE					90
// 마을 방어력 량 게이지 배경 이미지
#define ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND			91
// 마을 주둔군 수 게이지
#define ON_GAMESPR_GAUGE_VILLAGE_GARRISON					92
// 마을 주둔군 수 게이지 배경 이미지
#define ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND		93

// dead penalty를 위한 덧그리기시 사용될 이미지
#define ON_GAMESPR_ITEM_OVER_DEAD_PENALTY					94

// robypark 2005/2/16 14:39
// 녹색 숫자
#define ON_GAMESPR_NUMBER_GREEN								95
// 적색 숫자
#define ON_GAMESPR_NUMBER_RED								96
// 청색 숫자
#define ON_GAMESPR_NUMBER_BLUE								97
// 흰색 숫자
#define ON_GAMESPR_NUMBER_WHITE								98

// robypark 2004/11/4 15:52
// 공성전 유닛 바닥 효과 리소스 추가
#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_EFFECT				99		// 중립 공성 유닛 사환 이펙트

#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_BLUE_EFFECT			100		// 아군 공성 유닛 사환 이펙트

#define ON_GAMESPR_SIEGEWARFARE_SOLDIER_RED_EFFECT			101		// 적군 공성 유닛 사환 이펙트

// 로딩 종류
#define ON_LOAD_KIND_TI				1

typedef struct
{
	SI16			m_TileIndex;						// Tile Index(-1 이 없는 값이다.)
	SI16			m_YetTileIndex;						// Tile Index(-1 이 없는 값이다.)
	SI32			m_LastUseCount;						// 삭제 될 남은 시간(사용할때 마다 다시 갱신한다.)

	XSPR			m_XSPR;								// Image
	UI08*			m_ImageBuffer;						// 실제 이미지가 들어갈 Buffer
} OnlineResource_BaseData;

typedef struct
{
	SI16			m_TileIndex;						// Tile Index(-1 이 없는 값이다.)
	SI16			m_YetTileIndex;						// Tile Index(-1 이 없는 값이다.)
	SI32			m_LastUseCount;						// 삭제 될 남은 시간(사용할때 마다 다시 갱신한다.)

	XSPR			m_XSPR;								// Image
} OnlineResource_ExtData;

typedef struct
{
	SI16			m_VillageSize;						// 마을의 Size(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
	SI16			m_VillageState;						// 마을의 국가코드
	SI16			m_VillageFirePer;					// 불이난정도

	SI16			m_FireCount;						// 불수
	SI16			m_FirePositionX[16];				// 불위치
	SI16			m_FirePositionY[16];				// 불위치
	SI16			m_FireSize[16];						// 불 크기
} OnlineResource_VillageFireData;

// robypark 2004/12/28 16:4
// 마을을 그릴 때 이미지들의 위치 등을 저장하는 구조체.
// VillageImagePositionInfo.txt파일로 추출하여 사용하기 위함. hq\\Online\\Village\\VillageImagePositionInfo.txt
typedef struct _OnlineResource_VillageImagePositionInfo
{
	SI16			m_ssVillageState;						// 마을의 국가코드
	SI16			m_ssVillageSize;						// 마을의 Size(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)

	// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
	SI16			m_ssVillageGuildFlagPosX;				// 상단 깃발 위치
	SI16			m_ssVillageGuildFlagPosY;				// 상단 깃발 위치

	// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
	SI16			m_ssSurrenderFlagPosX;					// 마을 점령 깃발 위치
	SI16			m_ssSurrenderFlagPosY;					// 마을 점령 깃발 위치

	// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
	SI16			m_ssVillageDrawCenterX;					// 마을이 그려지는 중점좌표
	SI16			m_ssVillageDrawCenterY;					// 마을이 그려지는 중점좌표

	// 마을 중심(변환된 dot)에서 위로 떨어진 거리에 위치하는 값
	SI16			m_ssVillageGarrisionGaugePosX;			// 마을 주둔군(수비병) 표시 게이지 위치
	SI16			m_ssVillageGarrisionGaugePosY;			// 마을 주둔군(수비병) 표시 게이지 위치
	SI16			m_ssVillageDefenceGaugePosX;			// 마을 방어력 표시 게이지 위치
	SI16			m_ssVillageDefenceGaugePosY;			// 마을 방어력 표시 게이지 위치
	SI16			m_ssEnabledOccupyVillagePosX;			// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치
	SI16			m_ssEnabledOccupyVillagePosY;			// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치

} OnlineResource_VillageImagePositionInfo, *LPOnlineResource_VillageImagePositionInfo;

class OnlineResource
{
private:
	char			m_TempBuffer[256];										// 내부에서 사용하는임시 Buffer

	XSPR			xsprOnlineGame[MAX_ONLINE_GAME_FILE];					// 게임용 스프라이트.
	XSPR			xsprOnlineImoticon[MAX_ONLINE_IMOTICON_FILE];			// 이모티콘용 스프라이트.
	XSPR			xsprOnlineFlagM[MAX_ONLINE_FLAG_FILE];					// 깃발용 스프라이트.
	XSPR			xsprOnlineFlagV[MAX_ONLINE_FLAG_FILE];					// 깃발용 스프라이트.
	XSPR			xsprOnlineItem[MAX_ONLINE_ITEM_FILE];					// Item용 스프라이트.
	XSPR			xsprOnlineVillageEffect[1][1][3];						// Village Effect용 스프라이트.
	XSPR			xsprOnlineVillageAttackEffect[3];						// Village Attack Effect용 스프라이트.

	static	CHAR	szOnlineGameFile[MAX_ONLINE_GAME_FILE][128];			// 캐릭터 스프라이트용 화일명
	static	CHAR	szOnlineTileFile[MAX_ONLINE_TILE_FILE][128];			// 타일용 스프라이트용 화일명
	SI16			szOnlineTileFilePos[MAX_ONLINE_TILE_FILE];
	static	CHAR	szOnlineItemFile[MAX_ONLINE_ITEM_FILE][128];			// Item용 스프라이트용 화일명
	static	CHAR	szOnlineObjectFile[MAX_ONLINE_OBJECT_FILE][128];		// 오브젝트용 스프라이트용 화일명
	SI16			szOnlineObjectFilePos[MAX_ONLINE_OBJECT_FILE];

	SI16			siCount;												// 총 케릭터 스프라이트 수

	SI16							siVillageFirePositionCount;				// 마을이 불탈때 불타는 위치 수
	OnlineResource_VillageFireData*	sVillageFirePosition;					// 마을이 불탈때 불타는 위치

	// robypark 2004/12/28 17:13
	SI16										m_ssCntVillageImagePositionInfo;	// 마을 그릴 때 사용되는 이미지들 포지션 정보 개수
	LPOnlineResource_VillageImagePositionInfo	m_lpsVillageImagePositionInfo;		// 마을 그릴 때 사용되는 이미지들 포지션 정보 변수

public:
	OnlineResource_BaseData		ImageBaseTileData[MAX_ONLINE_TILE_BASEDATAFILE];		// Tile Data
	OnlineResource_ExtData		ImageExtTileData[MAX_ONLINE_TILE_EXTDATAFILE];			// Tile Data
	OnlineResource_BaseData		ImageBaseObjectData[MAX_ONLINE_OBJECT_BASEDATAFILE];	// Object Data
	OnlineResource_ExtData		ImageExtObjectData[MAX_ONLINE_OBJECT_EXTDATAFILE];		// Object Data

public:
	OnlineResource();											// 생성자
	~OnlineResource();											// 소멸자
	BOOL	Init();												// 온라인에서 사용되는 리소스(화일등)을 로드한다.
	VOID	LoadCharSpr( char *pDirectory,  char *pPolder );	// 케릭터 스프라이트 파일을 찾아서 로드함
	SI32	AddItemSpr( char *pFileName );						// 아이템을 로드함(초기에 로드하지 않고 추가로 로드 할수 있음)
	VOID	Free();												// 해제.
	SI32	GetCharFileIndex(CHAR *pszFileName);				// 캐릭터 스프라이트 배열의 인덱스를 얻어온다.
	SI32	GetObjectFileIndex(CHAR *pszFileName);				// 오브젝트 스프라이트 배열의 인덱스를 얻어온다.
	SI32	GetGameFileIndex(CHAR *pszFileName);				// 게임 스프라이트 배열의 인덱스를 얻어온다.
	SI32	GetItemFileIndex(CHAR *pszFileName);				// Item 스프라이트 배열의 인덱스를 얻어온다.

	XSPR	*GetxsprGame(SI16 siIndex);							// private(게임용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprTile(SI16 siIndex);							// private(타일용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprObject(SI16 siIndex);						// private(오브젝트용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprImoticon(SI16 siIndex);						// private(이모티콘용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprItem(SI16 siIndex);							// private(Item용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprFlagM(SI16 siIndex);						// private(깃발(아이디 앞에 붙는것)용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprFlagV(SI16 siIndex);						// private(깃발(마을안에서)용 스프라이트)에 접근할 수 있는 함수를 만든다.
	XSPR	*GetxsprVillageEffect(SI16 siSize, SI16 siPercent);	// private(마을 효과용 스프라이트)에 접근할 수 있는 함수를 만든다.
	OnlineResource_VillageFireData*	GetVillageFirePosition(SI16 VillageSize, SI16 State, SI16 Per);	// 마을이 불탈때 불타는 위치
	XSPR	*GetxsprVillageAttackEffect(SI16 Type);				// private(마을 공격 효과용 스프라이트)에 접근할 수 있는 함수를 만든다.

	BOOL	Prepare(void);										// Prepare

	//-----------------------------------------------
	// 정환이꺼
	//-----------------------------------------------
	BOOL	LoadInformationDirectoryRecurs(char * pCurPath, SI32 siKind);

	// robypark 2004/12/28 17:13
	// desc: 마을 그릴 때 사용되는 이미지들 포지션 정보 파일 HQ에서 읽기
	BOOL	LoadVillageImagePositionInfoFile(void);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/28 17:13
	// desc: 마을 그릴 때 사용되는 이미지들 파일명 및 마을 크기 얻기
	// parameter:
	// SI16	ssVillageState[IN]: 마을의 국가코드
	// char	*pszVillageSizeImageFilename[IN]: 마을의 크기를 나타내는 이미지 파일 명(town49.spr, town25.spr, town9.spr)
	// SI16	&ssVillageSize[OUT]: 마을의 크기값(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
	// char *pszVillageImageFilename[OUT]: 마을 이미지 파일명. 마을 방어력이 70% 이상일 경우
	// char *pszBrokenVillageImageFilename[OUT]: 부서진 마을 이미지 파일명. 마을 방어력이 70%미만일 경우
	//----------------------------------------------------------------------------------------------
	void	GetVillageImageFilename(SI16 ssVillageState, char *pszVillageSizeImageFilename, SI16 &ssVillageSize,
											char *pszVillageImageFilename, char *pszBrokenVillageImageFilename);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/28 17:13
	// desc: 마을 그릴 때 사용되는 이미지들 포지션 정보 헤더 얻기
	// parameter:
	// SI16	ssVillageState[IN]: 마을의 국가코드
	// SI16	ssVillageSize[IN]: 마을의 크기값(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
	//----------------------------------------------------------------------------------------------
	LPOnlineResource_VillageImagePositionInfo	GetVillageImagePositionInfoHeader(SI16 ssVillageState, SI16 ssVillageSize);

	//----------------------------------------------------------------------------------------------
	// robypark 2004/12/29 17:26
	// desc: 마을 크기 얻기
	// parameter:
	// char	*pszVillageSizeImageFilename[IN]: 마을의 크기를 나타내는 이미지 파일 명(town49.spr, town25.spr, town9.spr)
	//----------------------------------------------------------------------------------------------
	SI16	GetVillageSize(char *pszVillageSizeImageFilename);

};


#endif

