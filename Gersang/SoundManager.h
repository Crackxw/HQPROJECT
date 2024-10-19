/*
	���� SOUNDMANAGER ������ 2002. 2. 21.


	2002.  3. 21. �ı��ڿ� uiStaticSlotInfo, uiStreamSlotInfo �޸� ���� �߰�
	2002.  2. 21. �������� ���� ���̺귯���� �ϱ� ���� 
	              Directive.h�� Include ��Ű�� �ʾƵ� ������ �����ϰ� ��
*/
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <vector>
#include <GSL.h>

#ifndef R64
typedef               double R64;
#endif
#ifndef R32
typedef                float R32;
#endif
#ifndef UI64
typedef  unsigned long int  UI64;        // UI64 is equal to UI32 in the 32bit computer
#endif
#ifndef SI64
typedef    signed long int  SI64;
#endif
#ifndef UI32
typedef       unsigned int  UI32;
#endif
#ifndef SI32
typedef         signed int  SI32;
#endif
#ifndef UI16
typedef unsigned short int  UI16;
#endif
#ifndef SI16
typedef   signed short int  SI16;
#endif
#ifndef UI08
typedef       unsigned char UI08;
#endif
#ifndef SI08
typedef         signed char SI08;
#endif

#include "EAXSound.h"

#define MAX_DEFAULT_DESCRIPTION		255
#define MAX_DEFAULT_FILENAME		255
#define MAX_DEFAULT_STREAM			1
#define MAX_DEFAULT_STATIC			20
#define MAX_DEFAULT_SOUNDLIST		1500

class SoundManager
{
	struct SoundInfo_struct
	{
		BOOL bActive;
		UI16 uiID;
		UI08 pFileName[MAX_DEFAULT_FILENAME]; // FileName
		BOOL bStatic; // Static or Stream
		UI08 nDuplicate; // ������ ���� ����
		float fDelay; 
		float fDuplicateDelay; // ������ ���尡 �����ؼ� ��µɶ� �� ������ ������ Second ��
		BOOL bLoop; // ���尡 ��������ǵ� ��� �����Ұ���
		long siVolume; // DX -10000~0 ������ ������ ���ý��Ѿ� ��
		long siPan; // DX�� -10000~10000 ������ ������ ���ý��Ѿ� ��
		SI64 siPosX; // ������ǥ
		SI64 siPosY;
		//SI64 siPosZ;
		SI64 siRPosX; // Relative ��ǥ Listener�� ��ġ�� ���� �ǽð����� ����Ǿ�� �Ѵ�
		SI64 siRPosY;
		//SI64 siRPosZ;
		SI64 siEAX;
		BOOL bStop; // ���� ID�� ���ֵǰ� ������ �Ǵٽ� �����϶�� �ϸ� �� ���� TRUE���� ������ ���� �����ߴ��� ������Ű�� ���� �����Ѵ�
		DWORD dwPlayTime; // ���带 ������ �ð�
		DWORD dwEntireTime; // ��ü ���� ���ֽð�
	};	

	public:
		EAXSound *pEAXSound;

	protected:
		SI16 siNStatic; // Static Slot ����
		SI16 siNStream; // Stream Slot ����

		UI16 uiListIDNum; // Excel File�� �� Sound List ��
		UI16 uiLastIDNum; // uiListIDNum + ����ڰ� AddPlayID�� ��

		HWND m_hWnd;

#ifdef _OGG_PLAY
	private:
		long m_lStreamSoundVolume;
	public:
		inline long GetStreamSoundVolume() { return m_lStreamSoundVolume; }
#endif
	public:
		BOOL bSoundDriver;

	protected:
		SoundInfo_struct SoundList[MAX_DEFAULT_SOUNDLIST]; // Sound.txt�� ��� ������ �о�� �����Ų��
		UI16 uiSoundListIDInfo[MAX_DEFAULT_SOUNDLIST]; // Sound�� ID�� ���° Soundlist�� ����Ǿ� �ִ���

		UI16 *uiStaticSlotInfo; // Slot�� ��� ID�� ����Ǿ� �ִ���

		BOOL bStaticPlayList[MAX_DEFAULT_SOUNDLIST]; // SoundList���� �ε��� �Ǿ����� üũ
		UI16 uiStaticPlayListIDInfo[MAX_DEFAULT_SOUNDLIST]; // ID�� ��� Slot�� ����Ǿ� �ִ���

		UI16 *uiStreamSlotInfo; // Slot�� ��� ID�� ����Ǿ� �ִ���

		BOOL bStreamPlayList[MAX_DEFAULT_SOUNDLIST]; // SoundList���� �ε��� �Ǿ����� üũ
		UI16 uiStreamPlayListIDInfo[MAX_DEFAULT_SOUNDLIST]; // ID�� ��� Slot�� ����Ǿ� �ִ���

		UI16 uiStaticNum; // �ε��� Static Sound�� ��
		UI16 uiStreamNum; // �ε��� Stream Sound�� ��

		SI64 siListenerPosX; // Listener�� Position, �� ������ ������Ʈ���� �ٸ� ����� �Ÿ����� �����Ѵ�.
		SI64 siListenerPosY;

		SI16 siVolumeX, siVolumeY, siPan;

		UI16 uiMaxStaticVolume, uiMaxStreamVolume; // ������ �ִ밪 ����

	enum TIMER_COMMAND 
		{ TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE, TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };

	public:
		// ���ο� ����� ID �߰�
		UI16 AddPlayID(char *lpstrFile, 
			BOOL bStatic=TRUE, BOOL bLoop=FALSE, BOOL bStop=TRUE, UI08 uiDuplicate=0, 
			float fDelay=0, SI64 siEAX=0, SI64 siVolume=0, SI64 siPan=0, SI64 siX=0, SI64 siY=0);

		// ����ڰ� ������ ID����
		BOOL DelPlayID(UI16 uiID); 

		// ���� ID ���۸� �޸� ����
		void ReleaseSound(UI16 uiID);
		// ��� ���� ID ���۸� �޸� ����
		void ReleaseAllSound(void);
		// ��� Static ���� �޸� ����
		void ReleaseAllStaticSound(void);
		// ��� Stream ���� �޸� ����
		void ReleaseAllStreamSound(void);
		// Sound�� ���������� �˻�
		BOOL IsPlaying(UI16 uiID);

		BOOL IsStop(UI16 uiID);


		SoundManager(HWND hWnd, char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream);
		~SoundManager();

		void ReCreate(char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream); // �����

		void SetEmulate3DValue(SI16 siVolumeX1, SI16 siVolumeY1, SI16 siPan1);

		BOOL LoadPlayList(char *lpstrFile); 
																							 																					
		void PlaySound(UI16 uiID);
		void StopSound(UI16 uiID);
		void StopAllSound(void); // ��� Sound�� ���ָ� Stop��Ų��.
		void PauseAllSound(void); // Static Sound�� ���ָ� Pause��Ų�� (ex:������ ������ ��� ������ų��(�޴��� �ҷ�����) ���)

		// uiVolume�� 0(MIN)~1000(MAX) �̳� 600������ ���� ���� 0�� �����ϴ�
		void SetAllStaticSoundVolume(UI16 uiMaxVolume); // ��� Static ������ Volume�� �����Ѵ�. 3D Sound���� ����õ (ex:�޴����� Static ����(ȿ����) ������ ���϶�)
		void SetAllStreamSoundVolume(UI16 uiMaxVolume); // ��� Stream ������ Volume�� �����Ѵ�. 3D Sound���� ����õ (ex:�޴����� Stream ����(����) ������ ���϶�)
		void SetAllSoundVolume(UI16 uiMaxVolume); // ��� ������ Volume�� �����Ѵ�. 3D Sound���� ����õ (ex:�޴����� ���� ������ ���϶�)

		void SetListenerPosition_Quarter(SI64 siX, SI64 siY); // Quarter View ���ӿ��� ���ΰ��� ��ǥ(�߽���ǥ)�� ������Ʈ ��Ų��(Static Sound�� ����ȴ�)
		void SetCharPosition_Quarter(UI16 uiID, SI64 siX, SI64 siY); // Quarter View���ӿ��� Ÿ���� ��ǥ�� �޾� ���� �������� �����Ѵ�(Static Sound�� ����ȴ�)

		void SetListenerPosition_Top(SI64 siX, SI64 siY); // Top View ���ӿ��� ���ΰ��� ��ǥ(�߽���ǥ)�� ������Ʈ ��Ų��(Static Sound�� ����ȴ�)
		void SetCharPosition_Top(UI16 uiID, SI64 siX, SI64 siY); // Top View���ӿ��� Ÿ���� ��ǥ�� �޾� ���� �������� �����Ѵ�(Static Sound�� ����ȴ�)
		
		void SetEAXEnvironment(UI16 uiID, DWORD dwValue); // 0 ~ 26 (define ����)

	protected:
		void Create(char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream);

		void LoadStaticSound(UI16 uiSlot, UI08 *pFileName, UI08 uiDuplicate);
		void PlayStaticSound(UI16 uiSlot, BOOL bLoop);
		void StopStaticSound(UI16 uiSlot, BOOL fPosition);

		void LoadStreamSound(UI16 uiSlot, UI08 *pFileName, BOOL bLoop);
		void PlayStreamSound(UI16 uiSlot, BOOL bLoop);
		void StopStreamSound(UI16 uiSlot);

		void SetSoundVolume(SI16 siID, long siVolume1000); // 0(Min) ~ 1000(Max)
		void SetSoundPan(UI16 uiID, long lPan100); // 0(Left) ~ 100(Right)

		float DXUtil_Timer(TIMER_COMMAND command); // DirectX 8.0���� ������ ���̺귯��
};

// SetEAXEnvironment 0 ~ 26 ���� ���� ����
#define	EAX_GENERIC				0
#define EAX_PADDED_CELL			1	// ���� ���� ����
#define EAX_ROOM				2
#define EAX_BATHROOM			3
#define EAX_LIVINGROOM			4
#define EAX_STONE_ROOM			5
#define EAX_AUDITORIUM			6	// ����, û�߼�
#define EAX_CONVERT_HALL		7
#define EAX_CAVE				8
#define EAX_ARENA				9	// ���� �����
#define EAX_HANGAR				10	// ���θ�, �ݳ���
#define EAX_CARPETED_HALLWAY	11	// ī��Ʈ�� ���� ���?
#define EAX_HALLWAY				12
#define EAX_STONE_CORRIDOR		13	// ���ε� ����
#define EAX_ALLEY				14	// �ް��, ���ֱ�
#define EAX_FOREST				15
#define EAX_CITY				16
#define EAX_MOUNTAINS			17
#define EAX_QUARRY				18	// ä����
#define EAX_PLAIN				19
#define EAX_PARKINGLOT			20	// ������
#define EAX_SEWER_PIPE			21
#define EAX_UNDER_WATER			22
#define EAX_DRUGGED				23	// ����� 
#define EAX_DIZZY				24	// ���������� 
#define EAX_PSYCHOTIC			25	// ���ź���

#endif