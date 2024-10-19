#ifndef	Y_SCRIPT_HEADER
#define Y_SCRIPT_HEADER

#include <windows.h>

#define		Y_SCRIPT_MAX_KINDOF_NUM		4
#define		Y_SCRIPT_KINDOF_WHO			0
#define		Y_SCRIPT_KINDOF_PORTRAIT	1
#define		Y_SCRIPT_KINDOF_JVIFILE		2
#define		Y_SCRIPT_KINDOF_SPEECH		3

#define		Y_SCRIPT_NARATION_MAX_CHAR	2048

#define		Y_SCRIPT_COMMAND_NUM				11
#define		Y_SCRIPT_COMMAND_UNKNOWN			-1
#define		Y_SCRIPT_COMMAND_SPEECH				0
#define		Y_SCRIPT_COMMAND_CHANGETITLE		1
#define		Y_SCRIPT_COMMAND_CHANGEMUSIC		2
#define		Y_SCRIPT_COMMAND_SETDELAYTIME		3
#define		Y_SCRIPT_COMMAND_NARATION			4
#define		Y_SCRIPT_COMMAND_DRAWMODE			5
#define		Y_SCRIPT_COMMAND_PROCESSMODE		6
#define		Y_SCRIPT_COMMAND_OBJECTIVE			7
#define		Y_SCRIPT_COMMAND_PLAYSOUND			8
#define		Y_SCRIPT_COMMAND_TITLE				9
#define		Y_SCRIPT_COMMAND_NOEND				10

#define		Y_SCRIPT_COMMAND_MODE_EXCLUSIVE		0
#define		Y_SCRIPT_COMMAND_MODE_COOPERATIVE	1

#define		Y_SCRIPT_COMMAND_SPEECH_STRING					"SPEECH"
#define		Y_SCRIPT_COMMAND_CHANGETITLE_STRING				"CHANGETITLE"
#define		Y_SCRIPT_COMMAND_CHANGEMUSIC_STRING				"CHANGEMUSIC"
#define		Y_SCRIPT_COMMAND_SETDELAYTIME_STRING			"SETDELAYTIME"
#define		Y_SCRIPT_COMMAND_NARATION_STRING				"NARATION"
#define		Y_SCRIPT_COMMAND_DRAWMODE_STRING				"SHOWMODE"
#define		Y_SCRIPT_COMMAND_PROCESSMODE_STRING				"PROCESSMODE"
#define		Y_SCRIPT_COMMAND_EXCLUSIVE_STRING				"EXCLUSIVE"
#define		Y_SCRIPT_COMMAND_COOPERATIVE_STRING				"COOPERATIVE"
#define		Y_SCRIPT_COMMAND_OBJECTIVE_STRING				"OBJECTIVE"
#define		Y_SCRIPT_COMMAND_PLAYSOUND_STRING				"PLAYSOUND"
#define		Y_SCRIPT_COMMAND_TITLE_STRING					"TITLE"
#define		Y_SCRIPT_COMMAND_NOEND_STRING					"NOEND"

struct _y_script_command_header
{	
	SHORT	nCommand;
	CHAR*	lpParameter;
};

struct _y_Script_Speech_Header
{	
	CHAR	lpName[128];
	SHORT	PortraitNum;
	CHAR	lpWaveFileName[1024];
	CHAR	lpString[1024];	
};

struct _y_Script_Ojbective_Header
{
	CHAR	szObjective1[256];
	CHAR	szObjective2[256];
};

struct _y_Script_Naration_Header
{
	CHAR*	lpString;							//�����̼� ���ڿ�.
	CHAR	lpFileName[1024];					//�����̼� ���̺� ȭ��.
};

struct _y_Script_DelayTime_Header
{
	DWORD	dwStartTime;
	DWORD	dwDelayTime;
};

typedef _y_Script_Speech_Header			y_Script_Speech_Header,		*LPy_Script_Speech_Header;
typedef	_y_script_command_header		y_script_command_header,	*LPy_script_command_header;
typedef _y_Script_Naration_Header		y_Script_Naration_Header,	*LPy_Script_Naration_Header;
typedef	_y_Script_Ojbective_Header		y_Script_Objective_Header,	*LPy_Script_Objective_Header;
typedef	_y_Script_DelayTime_Header		y_Script_DelayTime_Header,  *LPy_Script_DelayTime_Header;

class _Y_Script
{
private:	
	SHORT						m_nDrawMode;
	SHORT						m_nProcessMode;	

	BOOL						m_IsLoad;
	BOOL						m_IsPlay;
	BOOL						m_IsNoEnd;
	UI16						m_BackgroundMusic;
	UI16						m_dwPlaySoundChannelNum;
	UI16						m_dwNarationChannelNum;	
	DWORD						m_dwNarationEndTime;
	XSPR						m_TitleSpr;


	SHORT						m_Index;	
	WORD						m_wTotalCommandNum;		
	LPy_script_command_header	m_lpScriptCommand;
	y_Script_DelayTime_Header	m_DelayTime;
	
	
	
	BOOL		IsTxtFile(CHAR *FileName);
	BOOL		IsScrFile(CHAR *FileName);
	SHORT		GetScriptCommand(CHAR *strCommand);
	
	VOID		ProcessScriptCommand(SHORT nCommand, FILE *fp);
	BOOL		ExcuteScriptCommand(LPy_script_command_header lpScriptCommandHeader);
	VOID		CancelScriptCommand(LPy_script_command_header lpScriptCommandHeader);	

	BOOL		LoadSoundFiles();								//���� ȭ�ϵ��� ��� �о� ���δ�.
	VOID		StopPrevSounds(SHORT CurrentSound);				//������ ���带 ���ش�.

	BOOL		LoadTitleFile(LPSTR filename);
	VOID		FreeTitleFile();	
	BOOL		GetText(CHAR* lpText, FILE *fp);				
	
	BOOL		IsValidIndex(SHORT Index);		
	BOOL		IsFinishedPrevCommand(LPy_script_command_header lpScriptCommandHeader);

	VOID		Put();

	SHORT		GetDrawMode(CHAR*	lpString);
	SHORT		GetProcessMode(CHAR*	lpString);
	BOOL		IsCancelPrevCommand();

	DWORD		FrameToClock(SHORT Frame);						//�������� Ŭ�� ������ �ٲ��ش�.
public:	
	_Y_Script();
	~_Y_Script();

	BOOL		LoadFile(CHAR* FileName);						//��ũ��Ʈ ȭ���� �ҷ� �´�.
	BOOL		IsLoad();										//��ũ��Ʈ�� �ε�Ǿ� �ִ��� ����.
	BOOL		IsPlay();										//��ũ��Ʈ�� ���������� �˻��Ѵ�.
	VOID		Play();											//��ũ��Ʈ ȭ���� ���ֽ�Ų��.

	VOID		Action();										//�������� �۾��� �����Ѵ�.
																//�ֱ������� ȣ���� �־�� �ϴ� �Լ�.
	
	VOID		Stop();											//���� �ϴ� ���� �����.
	VOID		Replay();										//�ٽ� ó�� ���� ���� ��Ų��.
	VOID		Free();											//�޸𸮿��� ���� ��Ų��.
	
	BOOL		IsExclusiveMode(SHORT	nWhatMode);				//nWhatMode�� ��Ÿ�� ���� ���� ����.
	BOOL		IsCooperativeMode(SHORT	nWhatMode);				//nWhatMode�� ������ ���� ���� ����.

	BOOL		GetObjective(CHAR* szFileName, CHAR* lpszGetObjective1, CHAR* lpszGetObjective2);			//������ ��´�.
	BOOL		GetTitle(CHAR* szFileName, CHAR* lpszGetTitle);
};

#endif









