/************************************************************************************************
	파일명 : OnlineMemoryProtected.cpp
	담당자 : 이윤석
************************************************************************************************/
#include <GSL.h>
#include <math.h>
#include <stdio.h>
#include "OnlineMemoryProtected.h"


//========================================================
// CMemoryProtected::CMemoryProtected() 생성자
CMemoryProtected::CMemoryProtected()
{
	Constructor();
}

//========================================================
// CMemoryProtected::~CMemoryProtected() 소멸자
CMemoryProtected::~CMemoryProtected()
{
	Destroyer();
}

//========================================================
// CMemoryProtected::Constructor() 생성자
void CMemoryProtected::Constructor(void)
{
	m_MemoryProtectedStart = NULL;
	m_MemoryProtectedEnd   = NULL;
}

//========================================================
// CMemoryProtected::Destroyer() 소멸자
void CMemoryProtected::Destroyer(void)
{
	Release();
}

//========================================================
// CMemoryProtected::Create() Create
BOOL CMemoryProtected::Create(void)
{
	Release();

	m_MemoryProtectedStart = NULL;
	m_MemoryProtectedEnd   = NULL;

	return TRUE;
}

//========================================================
// CMemoryProtected::Release() Release
void CMemoryProtected::Release(void)
{
	if(m_MemoryProtectedStart)
	{
		SMemoryProtected*  lpNowMemoryProtected;
		SMemoryProtected*  lpNextMemoryProtected;

		lpNowMemoryProtected = m_MemoryProtectedStart;
		while(lpNowMemoryProtected)
		{
			lpNextMemoryProtected = lpNowMemoryProtected->m_Next;
			delete lpNowMemoryProtected;

			lpNowMemoryProtected = lpNextMemoryProtected;
		}
	}

	m_MemoryProtectedStart = NULL;
	m_MemoryProtectedEnd   = NULL;
}

//========================================================
// CMemoryProtected::AddMemory() Add Memory
BOOL CMemoryProtected::AddMemory(void* lpMemory, int Size)
{
	if(lpMemory == NULL) return FALSE;
	if(Size     <  0)    return FALSE;

	SMemoryProtected*  lpNewMemoryProtected;

	lpNewMemoryProtected = new SMemoryProtected;

	lpNewMemoryProtected->m_lpMemory   = lpMemory;
	lpNewMemoryProtected->m_Size       = Size;
	lpNewMemoryProtected->m_CheckValue = CheckValue(lpNewMemoryProtected);
	lpNewMemoryProtected->m_Next       = NULL;
	lpNewMemoryProtected->m_Priv       = NULL;

	if(m_MemoryProtectedStart)
	{
		lpNewMemoryProtected->m_Priv = m_MemoryProtectedEnd;
		m_MemoryProtectedEnd->m_Next = lpNewMemoryProtected;
		m_MemoryProtectedEnd         = lpNewMemoryProtected;
	}
	else
	{
		m_MemoryProtectedStart = lpNewMemoryProtected;
		m_MemoryProtectedEnd   = lpNewMemoryProtected;
	}

	return TRUE;
}

//========================================================
// CMemoryProtected::DeleteMemory() Delete Memory
void CMemoryProtected::DeleteMemory(void* lpMemory)
{
	SMemoryProtected*  lpNowMemoryProtected;

	// 같은 메모리를 찾는다.
	lpNowMemoryProtected = m_MemoryProtectedStart;
	while(lpNowMemoryProtected)
	{
		if(lpNowMemoryProtected->m_lpMemory == lpMemory)
		{
			// 메모리 위치만 같으면 같은 메모리로 인식한다.
			if(lpNowMemoryProtected == m_MemoryProtectedStart) m_MemoryProtectedStart = lpNowMemoryProtected->m_Next;
			if(lpNowMemoryProtected == m_MemoryProtectedEnd)   m_MemoryProtectedEnd   = lpNowMemoryProtected->m_Priv;

			if(lpNowMemoryProtected->m_Next != NULL)
			{
				lpNowMemoryProtected->m_Next->m_Priv = lpNowMemoryProtected->m_Priv;
			}
			if(lpNowMemoryProtected->m_Priv != NULL)
			{
				lpNowMemoryProtected->m_Priv->m_Next = lpNowMemoryProtected->m_Next;
			}

			delete lpNowMemoryProtected;
			return;
		}

		lpNowMemoryProtected = lpNowMemoryProtected->m_Next;
	}
}

//========================================================
// CMemoryProtected::UpdateMemory() Update Memory
BOOL CMemoryProtected::UpdateMemory(void* lpMemory)
{
	SMemoryProtected*  lpNowMemoryProtected;

	// 같은 메모리를 찾는다.
	lpNowMemoryProtected = m_MemoryProtectedStart;
	while(lpNowMemoryProtected)
	{
		if(lpNowMemoryProtected->m_lpMemory == lpMemory)
		{
			// 메모리 위치만 같으면 같은 메모리로 인식한다.
			lpNowMemoryProtected->m_CheckValue = CheckValue(lpNowMemoryProtected);
			return TRUE;
		}

		lpNowMemoryProtected = lpNowMemoryProtected->m_Next;
	}

	return FALSE;
}

//========================================================
// CMemoryProtected::CheckChange() 바꿘 부분이 있는지 등록된 모든 Memory를 검사한다.
//========================================================
// Retrun Value : TRUE 일경우 바꿔었다.
BOOL CMemoryProtected::CheckChange(void)
{
	if(m_MemoryProtectedStart)
	{
		SMemoryProtected*  lpNowMemoryProtected;

		lpNowMemoryProtected = m_MemoryProtectedStart;
		while(lpNowMemoryProtected)
		{
			if(lpNowMemoryProtected->m_CheckValue != CheckValue(lpNowMemoryProtected))
			{
				return TRUE;
			}

			lpNowMemoryProtected = lpNowMemoryProtected->m_Next;
		}
	}

	return FALSE;
}

//========================================================
// CMemoryProtected::CheckValue() Check Value를 생성한다.
//========================================================
// lpMemoryData의 m_lpMemory와 m_Size 인자를 이용해서 Check Value를 만든다.
// 만드는 방식은 메모리 주소와 내용을 더하는것이다.
//========================================================
// Return Value : Check Value 값을 만들어서 돌려준다.
DWORD CMemoryProtected::CheckValue(SMemoryProtected* lpMemoryData)
{
	DWORD		TempNowValue;
	char*		lpMemory;
	int			MemoryCount;
	int			MemorySize;

	TempNowValue = 0;
	lpMemory     = (char*)lpMemoryData->m_lpMemory;
	MemorySize   = lpMemoryData->m_Size;

	TempNowValue = MemorySize;
	for(MemoryCount = 0; MemoryCount < MemorySize; MemoryCount++, lpMemory++)
	{
		TempNowValue += (*lpMemory);
	}

	return TempNowValue;
}

