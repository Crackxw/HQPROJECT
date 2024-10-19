/************************************************************************************************
	���ϸ� : OnlineMemoryProtected.cpp
	����� : ������
************************************************************************************************/
#include <GSL.h>
#include <math.h>
#include <stdio.h>
#include "OnlineMemoryProtected.h"


//========================================================
// CMemoryProtected::CMemoryProtected() ������
CMemoryProtected::CMemoryProtected()
{
	Constructor();
}

//========================================================
// CMemoryProtected::~CMemoryProtected() �Ҹ���
CMemoryProtected::~CMemoryProtected()
{
	Destroyer();
}

//========================================================
// CMemoryProtected::Constructor() ������
void CMemoryProtected::Constructor(void)
{
	m_MemoryProtectedStart = NULL;
	m_MemoryProtectedEnd   = NULL;
}

//========================================================
// CMemoryProtected::Destroyer() �Ҹ���
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

	// ���� �޸𸮸� ã�´�.
	lpNowMemoryProtected = m_MemoryProtectedStart;
	while(lpNowMemoryProtected)
	{
		if(lpNowMemoryProtected->m_lpMemory == lpMemory)
		{
			// �޸� ��ġ�� ������ ���� �޸𸮷� �ν��Ѵ�.
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

	// ���� �޸𸮸� ã�´�.
	lpNowMemoryProtected = m_MemoryProtectedStart;
	while(lpNowMemoryProtected)
	{
		if(lpNowMemoryProtected->m_lpMemory == lpMemory)
		{
			// �޸� ��ġ�� ������ ���� �޸𸮷� �ν��Ѵ�.
			lpNowMemoryProtected->m_CheckValue = CheckValue(lpNowMemoryProtected);
			return TRUE;
		}

		lpNowMemoryProtected = lpNowMemoryProtected->m_Next;
	}

	return FALSE;
}

//========================================================
// CMemoryProtected::CheckChange() �مR �κ��� �ִ��� ��ϵ� ��� Memory�� �˻��Ѵ�.
//========================================================
// Retrun Value : TRUE �ϰ�� �ٲ����.
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
// CMemoryProtected::CheckValue() Check Value�� �����Ѵ�.
//========================================================
// lpMemoryData�� m_lpMemory�� m_Size ���ڸ� �̿��ؼ� Check Value�� �����.
// ����� ����� �޸� �ּҿ� ������ ���ϴ°��̴�.
//========================================================
// Return Value : Check Value ���� ���� �����ش�.
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

