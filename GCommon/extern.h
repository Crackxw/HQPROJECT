#ifndef _EXTERN_H
#define _EXTERN_H  1

class	_Map;
class	_Char;
class	cltCharInterface;
class	cltMapInterface;
class	_YSMap;
class	_Text;
class	_FileVary;
class	_GameButton;
class	_MyNetGame;
//class	_Bullet;
//class	_Smoke;
class	_Goods;
class	_GameRecordDataHeader;
class	_GameRecordDataField;
class	_KindInfo;
class	CCharFileLoadManager;										// 조합형 캐릭터

extern	int						bActive;							// 현재 액티브인지 아닌지 알려주는 변수  

// 버전 정보 
extern	char					VersionInfo[];
extern	DWORD					ClientVersionNumber;				// 게임 버젼

extern	cltGrp					clGrp;								// 그래픽 라이브러리 객체 

// 다이렉트 드로우 객체 
extern	unsigned char			ThunderPalette[];
extern	unsigned char			NightPalette[][768];
extern	unsigned char			OriginalNightPalette[][768];
extern	unsigned char			ResultPalette[];
extern	unsigned char			InitMenuPalette[];
extern	unsigned char			LobbyPalette[];

extern	unsigned char			ChooseStagePalette[];
extern	unsigned char			BriefingPalette[];
extern	unsigned char			CreditPalette[];

extern	HWND					Hwnd;
extern	HINSTANCE				hInst;

extern	unsigned char			OriginalGamePalette[];
extern	unsigned char			Plus_ChooseNationPalette[];

extern	UI32					GameScreenMode;

extern	int						CDStopSwitch;						// CD음악을 정지 시킨다. 

// 네트워크용 
extern	DWORD					PrevMultiClock;
extern	DWORD					MessageSendFrame;					// 명령이나 NO_MESSAGE를 보낸 프레임 
extern	BOOL					GameDropSwitch ;
extern	_MyNetGame				MyNet;

//------------------------------------
// HQNet에 승점을 돌려주는 변수 
//------------------------------------
extern	BOOL					HQNetResultSwitch;
extern	SI32					HQNetResult;
extern	SI32					HQNetScore;


//------------------------------------
// 게임 내부 변수 
//------------------------------------
extern	SHORT					GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern	BOOL					UserCheatSwitch;					// 사용자가 치트를 했는지 여부를 알려준다. 

extern	SI32					MaxMapXsize;
extern	SI32					MaxMapYsize;

extern	SHORT					MessageMode;						// 메시지가 누구에게 가는가?

// 각종 글꼴 
extern  HFONT					TitleFont;
extern  HFONT					StaffFont;
extern  HFONT					HelpFont;
extern  HFONT 					BriefingFont;
extern  HFONT 					SaveInfoFont;
extern  HFONT 					VersionInfoFont;
extern  HFONT 					VersionInfoPlusFont;
extern  HFONT 					NormalEditFont;
extern  HFONT 					NormalEditPlusFont;
extern  HFONT 					ArielNormalFont;
extern  HFONT 					ArielBoldFont;
extern  HFONT 					ChatEditFont;


extern	HFONT 					WinLoseFont;
extern	HFONT 					IDFont;




extern	_GameButton 			GameButton[];

extern	UI08 					ScreenMapBuffer[];					// 화면에 찍을 지도를 임시로 저장하는 곳 


extern	int 					MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 					MapCenterY[][MAX_MAP_YSIZE];


extern	DWORD 					GameScore;							// 사용자의 점수 , HQNet에 전송되어 랭킹을 매기는 기준이 된다. 

//////////////////////////////////////
// 게임 기록 저장에 관련된 변수들 
/////////////////////////////////////
extern	DWORD 					GameRecordSwitch;
extern	_GameRecordDataHeader	GRH;
extern	_GameRecordDataField	GRF;



extern	SHORT 					PointNumber;						// 전기 공격 관련 
extern	SHORT 					PointArray[];

extern	char 					CDPath[];
extern	char 					SavePath[];
extern	char 					CurrentDirectory[];
extern	char 					SerialCodeString[];
extern	char 					HQNetIP[];

extern	_Stage 					Stage;								// 스테이지 
	

extern	_Map 					Map;
extern	_YSMap 					SMap;								// 작은 지도용 데이터 

extern	_Config 				Config;								// 환경 변수 

extern	_NewID 					ZeroID;

extern	_Text 					Text;								// 텍스트 

extern	_FileVary 				FV;


extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터

extern	RECT					ScreenRect;							// 지도가 표시되는 영역 


extern	_FindPathGame		   *FindPathGame;

extern	_Char					CR[];								// 캐릭터의 포인터 
extern	cltCharInterface		clifChar;							// 캐릭터 관련 인터페이스 
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 

extern	SHORT 					BasicBlock;
extern	SHORT 					SwordBlock;
extern	SHORT 					WaterBlock;

extern	_MainData				GD;									// 게임의 주데이터 
extern	_Item 					Item[];
extern	_Order 					Order[];
extern	_Upgrade 				Upgrade[];
extern	_Goods 					GoodsInfo[];


extern	SHORT 					MapEditMode;						// 지도 편집 모드인가?
extern	_MapEdit 				MapEdit;

extern	_Help 					Help;

// 통신 관련 
extern	BOOL 					ManEnemySwitch;						// 여럿이하기에서 사람인 적이 존재하는가?

// 메뉴 관련 
extern	SHORT 					MenuOfMouse;						// 마우스 커서가 위치하고 있는 메뉴의 번호 
extern	SHORT 					ShortCutMenuOfMouse;
extern	SHORT 					CrewMenuOfMouse;
extern	SHORT 					WeaponMenuOfMouse;

extern	BOOL 					IsMosueInControlSwitch;				// 마우스가 게임 콘트롤위에 위치하고 있다.

// 입력관련
extern  _Chat 					Chat;
extern	_Cheat 					Cheat;

// 마우스 관련 변수 
extern	SHORT 					GameLeftPressSwitch;
extern	SHORT 					GameRightPressSwitch;
extern	SHORT 					GameMouseX, GameMouseY;
extern	SHORT 					GameRealX, GameRealY;
extern	DWORD 					GameWParam;

extern	SHORT 					Mouse_X, Mouse_Y;
extern	int 					DragSwitch;
extern	int 					DragDoneSwitch;
extern	int 					DragStartX, DragStartY, DragEndX, DragEndY;
extern	int 					MapDragSwitch;
extern	int 					MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
extern	int 					MouseMode;							// 마우스가 처한 상태를 저장 
extern	int 					MenuMode;							// 메뉴의 상태를 저장 
extern	_NewID 					UnderMouseID;
extern	SHORT 					UnderMouseCropType;
extern	SHORT 					UnderMouseCropMount;
extern	SHORT 					SoloPlaySwitch;						// 혼자 하기 모드인가 여럿이 하기 모드인가?
extern	SHORT 					MapX, MapY;							// 마우스가 위치한 small 좌표 
extern	SHORT 					MouseHarvestX, MouseHarvestY;
extern	SHORT 					GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	SHORT 					GamePauseSwitch;					// 게임이 잠시 중단되었는가?
extern  BOOL					bGameAllyWindowSwitch;

extern	int   					MultiPlayDelayTime;

extern	int 					FlyVibrateData[];

// 키보드 관련
extern	int 					ControlKeyPressSwitch;
extern	int 					ShiftKeyPressSwitch;
extern	int 					TabPressDoneSwitch;


extern	BOOL 					CDInSwitch;							// 게임 cd가 들어있는가?
extern	BOOL 					DPLauncherSwitch;					// dp launcher에 의해 작동되었는가

extern	SI32					UserPlayerNumber;					// 사용자 관련 
	
extern	clock_t					StartClock, CurrentClock, PrevClock;// 시간 관련 
extern	clock_t					ClickClock;
extern	int						TimeLimitSecond;					// 시간 제한 게임에서의 제한 시간(초단위)



extern	XSPR 					TileSpr[];							// 지도 폰트용 타일 
extern	XSPR 					CastleSpr[];						// 성에 사용될 폰트용 타일 

extern	XSPR 					TitleSpr;							// 타이틀 배경 그림에 사용될 포맷 

extern	XSPR 					PannelSpr;
extern	XSPR 					PortraitSpr;
extern	XSPR 					OnlinePortraitSpr;					// 거상에서 쓸 초상화

extern	_ScreenPoint 			SP;									// 화면에 사용될 여러 정보 

// 컬러 테이블 
extern	unsigned char			RDarkTable[][32];
extern	unsigned char			GDarkTable[][64];
extern	unsigned char			BDarkTable[][32];
extern	UI08					WaterShadowTable[][256];			//[bg][spr]
extern	unsigned char			DarkTable[][256];					// 어두운 단계 
extern	unsigned char			TeamDarkTable[][MAX_DARK_STEP][256];


/////////////////////////////////////////////////////
// 캐릭터용 각종 전역 변수 
////////////////////////////////////////////////////
// 곡물 수확용 화일과 폰트 (조선 농부)
extern	SHORT 					KFarmerHarvestFrameNumber;
extern	SHORT 					KFarmerHarvestFile;
extern	SHORT 					KFarmerHarvestFont;	

// 곡물 수확용 화일과 폰트 (일본 수확기)
extern	SHORT 					JHarvesterHarvestFrameNumber;
extern	SHORT 					JHarvesterHarvestFile;
extern	SHORT 					JHarvesterHarvestFont;



// 변신하기전에 이전 상태 저장 변수 
extern	DWORD 					GlobalHealthPercent;
extern	SHORT 					GlobalSelectedSwitch;
extern	SHORT 					GlobalUnitTempVary1;
extern	_NewID 					GlobalNewID;


extern	SHORT 					General2KBirthdaySwitch;			// 이순신 장군 탄신일 인가여부
extern	SHORT 					General4KBirthdaySwitch;			// 석가 탄신일인지 여부 
extern	SHORT 					IndependenceDaySwitch;				// 광복절인지 여부

extern  char 					UserSelectMapFileName[];			// 사용자가 선택한 지도 이름을 저장할 공간   

extern	int 					GeneralList[];						// 국가별 장수의 리스트 

#endif