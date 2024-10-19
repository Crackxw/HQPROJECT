/************************************************************************************************
	파일명 : OnlineMemoryProtected.h
	담당자 : 이윤석

    설명 :  특정 부분의 메모리가 변하지 않는다고 가정할때, 그 메모리가 바꿔었는지 검사한다.
	        처음 Class에 메모리와 그 Size를 주면, 이 값을 간단하게 DWORD형으로 저장한다.
		    일정시간마다 검사하는 함수를 실행시키면, 이 수치가 다시 검사하여 같은지 검사하고
		   다르면 메모리가 바꿔었다고 인식한다.
		    큰 메모리양을 DWORD로 바꾸면, 정확성은 떨어지지만, 어느정도 Hacking Tool의 대한 변화정도
		   감지는 충분하다.
************************************************************************************************/
#ifndef __MEMORYPROTECTED_H__
#define __MEMORYPROTECTED_H__


typedef struct SMemoryProtected
{
	void*					m_lpMemory;
	int						m_Size;

	DWORD					m_CheckValue;

	SMemoryProtected*		m_Next;
	SMemoryProtected*		m_Priv;
} SMemoryProtected;


class CMemoryProtected
{
protected:
	SMemoryProtected*		m_MemoryProtectedStart;
	SMemoryProtected*		m_MemoryProtectedEnd;

public:
	CMemoryProtected();													// 생성자
	~CMemoryProtected();												// 소멸자

	void		Constructor(void);										// 생성자
	void		Destroyer(void);										// 소멸자

	BOOL		Create(void);											// Create
	void		Release(void);											// Release

	BOOL		AddMemory(void* lpMemory, int Size);					// Add Memory
	void		DeleteMemory(void* lpMemory);							// Delete Memory
	BOOL		UpdateMemory(void* lpMemory);							// Update Memory

	BOOL		CheckChange(void);										// 바꿘 부분이 있는지 등록된 모든 Memory를 검사한다.

protected:
	DWORD		CheckValue(SMemoryProtected* lpMemoryData);				// Check Value를 생성한다.
};


#endif