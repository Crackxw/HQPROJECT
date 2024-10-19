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
class	CCharFileLoadManager;										// ������ ĳ����

extern	int						bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  

// ���� ���� 
extern	char					VersionInfo[];
extern	DWORD					ClientVersionNumber;				// ���� ����

extern	cltGrp					clGrp;								// �׷��� ���̺귯�� ��ü 

// ���̷�Ʈ ��ο� ��ü 
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

extern	int						CDStopSwitch;						// CD������ ���� ��Ų��. 

// ��Ʈ��ũ�� 
extern	DWORD					PrevMultiClock;
extern	DWORD					MessageSendFrame;					// ����̳� NO_MESSAGE�� ���� ������ 
extern	BOOL					GameDropSwitch ;
extern	_MyNetGame				MyNet;

//------------------------------------
// HQNet�� ������ �����ִ� ���� 
//------------------------------------
extern	BOOL					HQNetResultSwitch;
extern	SI32					HQNetResult;
extern	SI32					HQNetScore;


//------------------------------------
// ���� ���� ���� 
//------------------------------------
extern	SHORT					GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern	BOOL					UserCheatSwitch;					// ����ڰ� ġƮ�� �ߴ��� ���θ� �˷��ش�. 

extern	SI32					MaxMapXsize;
extern	SI32					MaxMapYsize;

extern	SHORT					MessageMode;						// �޽����� �������� ���°�?

// ���� �۲� 
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

extern	UI08 					ScreenMapBuffer[];					// ȭ�鿡 ���� ������ �ӽ÷� �����ϴ� �� 


extern	int 					MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 					MapCenterY[][MAX_MAP_YSIZE];


extern	DWORD 					GameScore;							// ������� ���� , HQNet�� ���۵Ǿ� ��ŷ�� �ű�� ������ �ȴ�. 

//////////////////////////////////////
// ���� ��� ���忡 ���õ� ������ 
/////////////////////////////////////
extern	DWORD 					GameRecordSwitch;
extern	_GameRecordDataHeader	GRH;
extern	_GameRecordDataField	GRF;



extern	SHORT 					PointNumber;						// ���� ���� ���� 
extern	SHORT 					PointArray[];

extern	char 					CDPath[];
extern	char 					SavePath[];
extern	char 					CurrentDirectory[];
extern	char 					SerialCodeString[];
extern	char 					HQNetIP[];

extern	_Stage 					Stage;								// �������� 
	

extern	_Map 					Map;
extern	_YSMap 					SMap;								// ���� ������ ������ 

extern	_Config 				Config;								// ȯ�� ���� 

extern	_NewID 					ZeroID;

extern	_Text 					Text;								// �ؽ�Ʈ 

extern	_FileVary 				FV;


extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����

extern	RECT					ScreenRect;							// ������ ǥ�õǴ� ���� 


extern	_FindPathGame		   *FindPathGame;

extern	_Char					CR[];								// ĳ������ ������ 
extern	cltCharInterface		clifChar;							// ĳ���� ���� �������̽� 
extern	cltMapInterface			clifMap;							// �ʰ��� �������̽� 

extern	SHORT 					BasicBlock;
extern	SHORT 					SwordBlock;
extern	SHORT 					WaterBlock;

extern	_MainData				GD;									// ������ �ֵ����� 
extern	_Item 					Item[];
extern	_Order 					Order[];
extern	_Upgrade 				Upgrade[];
extern	_Goods 					GoodsInfo[];


extern	SHORT 					MapEditMode;						// ���� ���� ����ΰ�?
extern	_MapEdit 				MapEdit;

extern	_Help 					Help;

// ��� ���� 
extern	BOOL 					ManEnemySwitch;						// �������ϱ⿡�� ����� ���� �����ϴ°�?

// �޴� ���� 
extern	SHORT 					MenuOfMouse;						// ���콺 Ŀ���� ��ġ�ϰ� �ִ� �޴��� ��ȣ 
extern	SHORT 					ShortCutMenuOfMouse;
extern	SHORT 					CrewMenuOfMouse;
extern	SHORT 					WeaponMenuOfMouse;

extern	BOOL 					IsMosueInControlSwitch;				// ���콺�� ���� ��Ʈ������ ��ġ�ϰ� �ִ�.

// �Է°���
extern  _Chat 					Chat;
extern	_Cheat 					Cheat;

// ���콺 ���� ���� 
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
extern	int 					MouseMode;							// ���콺�� ó�� ���¸� ���� 
extern	int 					MenuMode;							// �޴��� ���¸� ���� 
extern	_NewID 					UnderMouseID;
extern	SHORT 					UnderMouseCropType;
extern	SHORT 					UnderMouseCropMount;
extern	SHORT 					SoloPlaySwitch;						// ȥ�� �ϱ� ����ΰ� ������ �ϱ� ����ΰ�?
extern	SHORT 					MapX, MapY;							// ���콺�� ��ġ�� small ��ǥ 
extern	SHORT 					MouseHarvestX, MouseHarvestY;
extern	SHORT 					GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	SHORT 					GamePauseSwitch;					// ������ ��� �ߴܵǾ��°�?
extern  BOOL					bGameAllyWindowSwitch;

extern	int   					MultiPlayDelayTime;

extern	int 					FlyVibrateData[];

// Ű���� ����
extern	int 					ControlKeyPressSwitch;
extern	int 					ShiftKeyPressSwitch;
extern	int 					TabPressDoneSwitch;


extern	BOOL 					CDInSwitch;							// ���� cd�� ����ִ°�?
extern	BOOL 					DPLauncherSwitch;					// dp launcher�� ���� �۵��Ǿ��°�

extern	SI32					UserPlayerNumber;					// ����� ���� 
	
extern	clock_t					StartClock, CurrentClock, PrevClock;// �ð� ���� 
extern	clock_t					ClickClock;
extern	int						TimeLimitSecond;					// �ð� ���� ���ӿ����� ���� �ð�(�ʴ���)



extern	XSPR 					TileSpr[];							// ���� ��Ʈ�� Ÿ�� 
extern	XSPR 					CastleSpr[];						// ���� ���� ��Ʈ�� Ÿ�� 

extern	XSPR 					TitleSpr;							// Ÿ��Ʋ ��� �׸��� ���� ���� 

extern	XSPR 					PannelSpr;
extern	XSPR 					PortraitSpr;
extern	XSPR 					OnlinePortraitSpr;					// �Ż󿡼� �� �ʻ�ȭ

extern	_ScreenPoint 			SP;									// ȭ�鿡 ���� ���� ���� 

// �÷� ���̺� 
extern	unsigned char			RDarkTable[][32];
extern	unsigned char			GDarkTable[][64];
extern	unsigned char			BDarkTable[][32];
extern	UI08					WaterShadowTable[][256];			//[bg][spr]
extern	unsigned char			DarkTable[][256];					// ��ο� �ܰ� 
extern	unsigned char			TeamDarkTable[][MAX_DARK_STEP][256];


/////////////////////////////////////////////////////
// ĳ���Ϳ� ���� ���� ���� 
////////////////////////////////////////////////////
// � ��Ȯ�� ȭ�ϰ� ��Ʈ (���� ���)
extern	SHORT 					KFarmerHarvestFrameNumber;
extern	SHORT 					KFarmerHarvestFile;
extern	SHORT 					KFarmerHarvestFont;	

// � ��Ȯ�� ȭ�ϰ� ��Ʈ (�Ϻ� ��Ȯ��)
extern	SHORT 					JHarvesterHarvestFrameNumber;
extern	SHORT 					JHarvesterHarvestFile;
extern	SHORT 					JHarvesterHarvestFont;



// �����ϱ����� ���� ���� ���� ���� 
extern	DWORD 					GlobalHealthPercent;
extern	SHORT 					GlobalSelectedSwitch;
extern	SHORT 					GlobalUnitTempVary1;
extern	_NewID 					GlobalNewID;


extern	SHORT 					General2KBirthdaySwitch;			// �̼��� �屺 ź���� �ΰ�����
extern	SHORT 					General4KBirthdaySwitch;			// ���� ź�������� ���� 
extern	SHORT 					IndependenceDaySwitch;				// ���������� ����

extern  char 					UserSelectMapFileName[];			// ����ڰ� ������ ���� �̸��� ������ ����   

extern	int 					GeneralList[];						// ������ ����� ����Ʈ 

#endif