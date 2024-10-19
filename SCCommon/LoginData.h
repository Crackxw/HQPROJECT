#ifndef	_LOGIN_DATA_HEADER
#define _LOGIN_DATA_HEADER


#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include "OnlineMsgBox.h"
#include "OnlineNotice.h"
#include "_FrontDefine.h"
#include "_LoginDefine.h"
#include "_FrontUserDefine.h"

// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� ����
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

enum
{   
	ON_LOGINMGR_LICENSE						= 0,
	ON_LOGINMGR_FRONTSERVER_CONNECT,
	ON_LOGINMGR_CLIENT_VERSION_CHECK,
	ON_LOGINMGR_CLIENT_VERSION_CHECK_WAIT,
	ON_LOGINMGR_NOTICE_REQUEST,
	ON_LOGINMGR_SERVER_INFO_REQUEST,
	ON_LOGINMGR_SERVER_SELECT,
	ON_LOGINMGR_GAMESERVER_LOGIN,
	ON_LOGINMGR_NOTICE,
	ON_LOGINMGR_ACCOUNT_LOGIN,
	ON_LOGINMGR_RECV_PWDATA,
	ON_LOGINMGR_CHAR_SELECT,
	ON_LOGINMGR_CHAR_CREATE,
	ON_LOGINMGR_IN_GAMEPOLL,
	ON_LOGINMGR_EXIT,
	ON_LOGINMGR_WAIT
};


enum
{   
	ON_RETURN_NONE		= 0,
	ON_RETURN_OK,
	ON_RETURN_EXIT,
	ON_RETURN_CREATE
};

// ������ ĳ���� ���� ����ü
struct SMyChar 
{
	SI16							siImKind;
	CHAR							cDefaultBody[5];				// ĳ������ ����Ʈ ��(��: KMN)
	SI16							siHeroAniAction;				// Animation �׸��� ��� Order����
	SI16							siHeroBaseWClass;				// ���⿡ ���� �ൿ�� �� �ִ� ���� CLASS
	SI16							siHeroWeaponEnum;				// ĳ���� �ִϸ��̼� �ӵ�
	SI16							siDir;							// ĳ������ ���� ���⿡ �´� ��Ʈ��ȣ�� ����� �� �ʿ��� ����
	SI16							siAniStep;						// �ִϸ��̼� ���� �ܰ�
	SI32							siFont;							// ������ ĳ���ʹ� ĳ���� �ִϸ��̼� ���� ������ ù��° ��Ʈ ��ȣ(�� ����� ��Ʈ �� * DirectionNo)
	BOOL							bReverse;
	SI16							siCenterX, siCenterY;			// ĳ���� �߽� ��ǥ
}; 

// �ʱ� �ε� ȭ��
struct OnlineLoadingData
{
	UI08	uiFrame;
	XSPR	BackImage, BackOverImage, AniLogoImage, PlayGradeImage;
	XSPR	m_Spr_CompanyLogo;										// actdoll (2004/09/02 4:11) : ȸ�� �ΰ�
};

// �������, �������� ȭ��
struct OnlineNoticeData
{
	_Button			OkButton, ExitButton;
	XSPR			NoticeImage;

	OnlineChatList	*m_pNoticeList;
};

// ���� ����ȭ��
struct OnlineSelectServerData
{
	FRONT::sWorldInfo*	pWorldInfo;								// ��� ������� ����
	SI16				siWorldCount;							// ������ �����

	SI16				siSelectStatus, siSelect;
	_Button				OkButton, ExitButton;
	XSPR				ServerBoxImage, ServerSelImage, PingImage, PingRingImage, ScrollAreaImage, *pScrollBarImage;

	SI32				m_siScroll;								// ������ �ִ� ������ �ε���
	RECT				m_rtScrollArea;
	SI32				m_siScrollBarY;
	float				m_fOldScrollY;

	OnlineSelectServerData()
	{
		pWorldInfo = NULL;
	}
};

// �α��� ȭ�鿡�� �ʿ��� ����Ÿ��.
struct	OnlineLogInData
{	
//	OnlineEditControl		*pOnlineIDEditControl;								// ID �Է¿� ����Ʈ ��Ʈ��.
//	OnlineEditControl		*pOnlinePassWordEditControl;						// �н����� �Է¿� ����Ʈ ��Ʈ��.	

	// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� �ڵ� ����
	HOEC					m_hOecID;											// ID�Է¿� ����Ʈ ��Ʈ�� �ڵ�
	HOEC					m_hOecPW;											// �н����� �Է¿� ����Ʈ ��Ʈ�� �ڵ�
	
	XSPR					LogInBoxImage;										// �α��� �̹���

	_Button					OkButton, ExitButton;								// OK �� EXIT ��ư.(�α��ν�)
	_Button					RegistrationButton, JoyOnButton;

	SI32					siLogInStatus;										// �α��� �Ҷ��� ����.
	SI16					siCount;											// �α��� ��ư�� ���� �ܼ�
	CHAR					szID[LOGIN::ID_MAX_LENGTH], szPW[LOGIN::PW_MAX_LENGTH];
//	RECT					rtIDRect, rtPWRect;

	OnlineLogInData()
	{
//		pOnlineIDEditControl = NULL;
//		pOnlinePassWordEditControl = NULL;
		m_hOecID	= NULL;
		m_hOecPW	= NULL;
	}
};

// �ɸ��͸� ���� & �����Ѵ�.
struct	OnlineSelectCharacterData
{
	XSPR					m_ImgButtonBaseSpr,m_ImgCharSelectBase;
	XSPR					m_ImgCharSelect[8];
	XSPR					m_ImgCharSelectBox;

	OnlineMsgBox			*pOnlineMsgBoxDel;									// ĳ���� ���� �޽��� �ڽ�
	_Button					BExit, OkButton,BNewChar, BDelChar;					// ��ư��
	_Button					BSelect[3];
	OnlineNotice			*pOnlineNotice;
	SMyChar					*pMyChar; 


	SI16					siSelectKind[3];
	SI16					siStatus; 
	SI16					siMaxCharNum;										// ���� ������ �ִ� �ɸ��� ��
	SI16					siKind;												// ���ΰ� �ɸ��� ����
	SI16					siNowChar;											// ���� �������� �ִ� �ɸ���
	UI08					uiSlot;												// ���� ������ �ɸ��� ����
	UI08					uiDelSlot;											// ������ ����
	DWORD					dwAnyTime;
	BOOL					bButtonAction;										// ĳ������ �޽����ڽ��� �� ������ ��ư������ ����

	OnlineSelectCharacterData()
	{
		pOnlineMsgBoxDel = NULL;
		pOnlineNotice = NULL;
		pMyChar = NULL;
	}
};


// ������ ���� ���� ����Ÿ�� �޴� ������ ȭ�鿡�� ���̴� ����Ÿ��.
struct	OnlineGameServerLogIn
{
	SI16					siStatus;
	SI32					bMessageBoxFloating;								// ���������� ������ �����Ͽ��°�.
	DWORD					siLogInTime;										// �α��� �� �ð�.	
	BOOL					bLogInOk;
};

// ���ο� ĳ���� ���� 
struct	OnlineCreateCharacter
{
	SI16					siStatus;
	UI08					uiSlot;												// ��� �ִ� ���� ��ȣ
	SI16					siKind;												// ������ �ɸ���
	CHAR					szID[LOGIN::NAME_MAX_LENGTH];						// ����Ʈ �ڽ����� �޴� ID	
	UI08					Totalb, siStra, siDext, siHp2, siInteli;			// ���ʽ�, ��, ��ø��, ����, ����

//	OnlineEditControl		*pOnlinIDEditBox;									// ID �շϿ� ����Ʈ ��Ʈ��.	
	// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� �ڵ� ����
	HOEC					m_hOecCharName;										// ĳ���� �̸� ����Ʈ ��Ʈ��

	XSPR					m_ImgCharList, m_ImgSelectChar[8],m_ImgLargeChar[8];
	XSPR					m_ImgWhiteBox,m_ImgInfoBoxBase;
	XSPR					m_ImgInfoBoxUp,m_ImgInfoBoxDn;
	XSPR					m_ImgButtonSpr,m_ImgButtonBaseSpr;
	XSPR					m_ImgNewCharSpr;

	UI16					uiCharCode[100];									// �ɸ��� ����
	SI32					siCharTotalNum;										// �� �ɸ��� ���� ��

	_Button					m_BttnSelectChar[8];
	_Button					m_BttnInfoBoxUp[5],m_BttnInfoBoxDn[5];
	_Button					m_BttnOk,m_BttnBack;

	OnlineCreateCharacter()
	{
//		pOnlinIDEditBox = NULL;
		m_hOecCharName	= NULL;
	}
};

// �α��� ���ݿ� ���� �ʿ��� ����Ÿ��.
struct	OnlineLogInMgr
{
	// �������κκ�
	SI16						siStatus;
	BOOL						bMouseDown;
	POINT						ptMouse;
	DWORD						m_StartTickCount;
	BOOL						bStatusBack;							// ����,���� ���� �޽��� Ȯ�ν� ���µǵ���
	DWORD						LogInTime;								// 5���� ���Ḧ���� �α����� �ð�üũ

	XSPR						BackImage, BackOverImage, AniLogoImage, LogoImage, PlayGradeImage;
	XSPR						SButtonImage, LButtonImage;
	XSPR						m_Spr_CompanyLogo;

	CHAR						szNotice[FRONT::NOTICE_MAX_LENGTH];		// ��������

	OnlineNoticeData			m_NoticeData;							// �������
	OnlineSelectServerData		m_SelectServerData;						// ��������
	OnlineLogInData				m_LogInData;							// ���� �α���
	OnlineSelectCharacterData	m_SelectCharData;						// �ɸ��͸� ����
	OnlineCreateCharacter		m_CreateCharacter;						// �ɸ��͸� ����
	OnlineGameServerLogIn		m_GameServerLogIn;						// ���Ӽ��� �α���	
};

#endif