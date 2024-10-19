/************************************************************************************************
	���ϸ� : OnlineMemoryProtected.h
	����� : ������

    ���� :  Ư�� �κ��� �޸𸮰� ������ �ʴ´ٰ� �����Ҷ�, �� �޸𸮰� �ٲ������ �˻��Ѵ�.
	        ó�� Class�� �޸𸮿� �� Size�� �ָ�, �� ���� �����ϰ� DWORD������ �����Ѵ�.
		    �����ð����� �˻��ϴ� �Լ��� �����Ű��, �� ��ġ�� �ٽ� �˻��Ͽ� ������ �˻��ϰ�
		   �ٸ��� �޸𸮰� �ٲ���ٰ� �ν��Ѵ�.
		    ū �޸𸮾��� DWORD�� �ٲٸ�, ��Ȯ���� ����������, ������� Hacking Tool�� ���� ��ȭ����
		   ������ ����ϴ�.
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
	CMemoryProtected();													// ������
	~CMemoryProtected();												// �Ҹ���

	void		Constructor(void);										// ������
	void		Destroyer(void);										// �Ҹ���

	BOOL		Create(void);											// Create
	void		Release(void);											// Release

	BOOL		AddMemory(void* lpMemory, int Size);					// Add Memory
	void		DeleteMemory(void* lpMemory);							// Delete Memory
	BOOL		UpdateMemory(void* lpMemory);							// Update Memory

	BOOL		CheckChange(void);										// �مR �κ��� �ִ��� ��ϵ� ��� Memory�� �˻��Ѵ�.

protected:
	DWORD		CheckValue(SMemoryProtected* lpMemoryData);				// Check Value�� �����Ѵ�.
};


#endif