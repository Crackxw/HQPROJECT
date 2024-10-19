/*
	최종 SOUNDMANAGER 수정일 2002. 2. 21.


	2002.  3. 21. 파괴자에 uiStaticSlotInfo, uiStreamSlotInfo 메모리 해제 추가
	2002.  2. 21. 범용적인 사운드 라이브러리로 하기 위해 
	              Directive.h를 Include 시키지 않아도 컴파일 가능하게 함
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
		UI08 nDuplicate; // 복제할 사운드 개수
		float fDelay; 
		float fDuplicateDelay; // 복제된 사운드가 연속해서 출력될때 각 사이의 딜레이 Second 값
		BOOL bLoop; // 사운드가 연주종료되도 계속 연주할건지
		long siVolume; // DX -10000~0 사이의 값으로 세팅시켜야 함
		long siPan; // DX의 -10000~10000 사이의 값으로 세팅시켜야 함
		SI64 siPosX; // 절대좌표
		SI64 siPosY;
		//SI64 siPosZ;
		SI64 siRPosX; // Relative 좌표 Listener의 위치에 따라 실시간으로 변경되어야 한다
		SI64 siRPosY;
		//SI64 siRPosZ;
		SI64 siEAX;
		BOOL bStop; // 같은 ID가 연주되고 있을때 또다시 연주하라고 하면 이 값이 TRUE임을 참조해 이전 연주했던걸 중지시키고 새로 연주한다
		DWORD dwPlayTime; // 사운드를 연주한 시간
		DWORD dwEntireTime; // 전체 사운드 연주시간
	};	

	public:
		EAXSound *pEAXSound;

	protected:
		SI16 siNStatic; // Static Slot 개수
		SI16 siNStream; // Stream Slot 개수

		UI16 uiListIDNum; // Excel File의 총 Sound List 수
		UI16 uiLastIDNum; // uiListIDNum + 사용자가 AddPlayID한 수

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
		SoundInfo_struct SoundList[MAX_DEFAULT_SOUNDLIST]; // Sound.txt의 모든 내용을 읽어와 저장시킨다
		UI16 uiSoundListIDInfo[MAX_DEFAULT_SOUNDLIST]; // Sound의 ID가 몇번째 Soundlist에 저장되어 있는지

		UI16 *uiStaticSlotInfo; // Slot에 몇번 ID가 저장되어 있는지

		BOOL bStaticPlayList[MAX_DEFAULT_SOUNDLIST]; // SoundList에서 로딩이 되었는지 체크
		UI16 uiStaticPlayListIDInfo[MAX_DEFAULT_SOUNDLIST]; // ID가 몇번 Slot에 저장되어 있는지

		UI16 *uiStreamSlotInfo; // Slot에 몇번 ID가 저장되어 있는지

		BOOL bStreamPlayList[MAX_DEFAULT_SOUNDLIST]; // SoundList에서 로딩이 되었는지 체크
		UI16 uiStreamPlayListIDInfo[MAX_DEFAULT_SOUNDLIST]; // ID가 몇번 Slot에 저장되어 있는지

		UI16 uiStaticNum; // 로딩된 Static Sound의 수
		UI16 uiStreamNum; // 로딩된 Stream Sound의 수

		SI64 siListenerPosX; // Listener의 Position, 매 루프시 업데이트시켜 다른 사운드와 거리감을 세팅한다.
		SI64 siListenerPosY;

		SI16 siVolumeX, siVolumeY, siPan;

		UI16 uiMaxStaticVolume, uiMaxStreamVolume; // 볼륨의 최대값 설정

	enum TIMER_COMMAND 
		{ TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE, TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };

	public:
		// 새로운 사용자 ID 추가
		UI16 AddPlayID(char *lpstrFile, 
			BOOL bStatic=TRUE, BOOL bLoop=FALSE, BOOL bStop=TRUE, UI08 uiDuplicate=0, 
			float fDelay=0, SI64 siEAX=0, SI64 siVolume=0, SI64 siPan=0, SI64 siX=0, SI64 siY=0);

		// 사용자가 설정한 ID삭제
		BOOL DelPlayID(UI16 uiID); 

		// 사운드 ID 버퍼를 메모리 해제
		void ReleaseSound(UI16 uiID);
		// 모든 사운드 ID 버퍼를 메모리 해제
		void ReleaseAllSound(void);
		// 모든 Static 사운드 메모리 해제
		void ReleaseAllStaticSound(void);
		// 모든 Stream 사운드 메모리 해제
		void ReleaseAllStreamSound(void);
		// Sound가 연주중인지 검사
		BOOL IsPlaying(UI16 uiID);

		BOOL IsStop(UI16 uiID);


		SoundManager(HWND hWnd, char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream);
		~SoundManager();

		void ReCreate(char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream); // 재생성

		void SetEmulate3DValue(SI16 siVolumeX1, SI16 siVolumeY1, SI16 siPan1);

		BOOL LoadPlayList(char *lpstrFile); 
																							 																					
		void PlaySound(UI16 uiID);
		void StopSound(UI16 uiID);
		void StopAllSound(void); // 모든 Sound의 연주를 Stop시킨다.
		void PauseAllSound(void); // Static Sound만 연주를 Pause시킨다 (ex:게임중 게임을 잠시 정지시킬때(메뉴를 불렀을시) 사용)

		// uiVolume은 0(MIN)~1000(MAX) 이나 600이하의 값은 거의 0과 동일하다
		void SetAllStaticSoundVolume(UI16 uiMaxVolume); // 모든 Static 사운드의 Volume을 조정한다. 3D Sound에선 비추천 (ex:메뉴에서 Static 사운드(효과음) 볼륨을 줄일때)
		void SetAllStreamSoundVolume(UI16 uiMaxVolume); // 모든 Stream 사운드의 Volume을 조정한다. 3D Sound에선 비추천 (ex:메뉴에서 Stream 사운드(뮤직) 볼륨을 줄일때)
		void SetAllSoundVolume(UI16 uiMaxVolume); // 모든 사운드의 Volume을 조정한다. 3D Sound에선 비추천 (ex:메뉴에서 사운드 볼륨을 줄일때)

		void SetListenerPosition_Quarter(SI64 siX, SI64 siY); // Quarter View 게임에서 주인공의 좌표(중심좌표)를 업데이트 시킨다(Static Sound만 적용된다)
		void SetCharPosition_Quarter(UI16 uiID, SI64 siX, SI64 siY); // Quarter View게임에서 타일의 좌표를 받아 사운드 포지션을 제어한다(Static Sound만 적용된다)

		void SetListenerPosition_Top(SI64 siX, SI64 siY); // Top View 게임에서 주인공의 좌표(중심좌표)를 업데이트 시킨다(Static Sound만 적용된다)
		void SetCharPosition_Top(UI16 uiID, SI64 siX, SI64 siY); // Top View게임에서 타일의 좌표를 받아 사운드 포지션을 제어한다(Static Sound만 적용된다)
		
		void SetEAXEnvironment(UI16 uiID, DWORD dwValue); // 0 ~ 26 (define 참조)

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

		float DXUtil_Timer(TIMER_COMMAND command); // DirectX 8.0에서 가져온 라이브러리
};

// SetEAXEnvironment 0 ~ 26 까지 사운드 설정
#define	EAX_GENERIC				0
#define EAX_PADDED_CELL			1	// 솜을 넣은 독실
#define EAX_ROOM				2
#define EAX_BATHROOM			3
#define EAX_LIVINGROOM			4
#define EAX_STONE_ROOM			5
#define EAX_AUDITORIUM			6	// 강당, 청중석
#define EAX_CONVERT_HALL		7
#define EAX_CAVE				8
#define EAX_ARENA				9	// 원형 경기장
#define EAX_HANGAR				10	// 오두막, 격납고
#define EAX_CARPETED_HALLWAY	11	// 카페트를 덮힌 통로?
#define EAX_HALLWAY				12
#define EAX_STONE_CORRIDOR		13	// 복로된 복도
#define EAX_ALLEY				14	// 뒷골목, 오솔길
#define EAX_FOREST				15
#define EAX_CITY				16
#define EAX_MOUNTAINS			17
#define EAX_QUARRY				18	// 채석장
#define EAX_PLAIN				19
#define EAX_PARKINGLOT			20	// 주차장
#define EAX_SEWER_PIPE			21
#define EAX_UNDER_WATER			22
#define EAX_DRUGGED				23	// 약먹은 
#define EAX_DIZZY				24	// 현기증나는 
#define EAX_PSYCHOTIC			25	// 정신병의

#endif