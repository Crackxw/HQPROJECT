#include <windows.h>
#include <string.h>



/*
//============================================
// SStack<Type>::SStack() Constuctor
template<class Type>
SStack<Type>::SStack()
{
	m_MaxDataCount    = 0;
	m_NowUseDataCount = 0;
	m_Data            = NULL;
}

//============================================
// SStack<Type>::~SStack() Destroyer
template<class Type>
SStack<Type>::~SStack()
{
	Release();
}

//============================================
// SStack<Type>::Create() Create
template<class Type>
int SStack<Type>::Create(int Size)
{
	if(m_Data) Release();

	m_MaxDataCount    = Size;
	m_NowUseDataCount = 0;
	m_Data            = new Type[Size];

	return 1;
}

//============================================
// SStack<Type>::Release() Release
template<class Type>
void SStack<Type>::Release(void)
{
	if(m_Data)
	{
		delete[] m_Data;
		m_Data = NULL;
	}
	m_MaxDataCount    = 0;
	m_NowUseDataCount = 0;
}

//============================================
// SStack<Type>::Add() Add
template<class Type>
int SStack<Type>::Add(Type Data)
{
	if(m_NowUseDataCount >= m_MaxDataCount) return -1;

	// Data추가
	memmove(&m_Data[m_NowUseDataCount], &Data, sizeof(Type));
	m_NowUseDataCount++;

	return (m_NowUseDataCount - 1);
}

//============================================
// SStack<Type>::Delete() Delete
template<class Type>
int SStack<Type>::Delete(int ID)
{
	if(ID >= m_NowUseDataCount) return -1;

	// Data 삭제
	memmove(&m_Data[ID], &m_Data[ID + 1], sizeof(Type) * (m_NowUseDataCount - (ID + 1)));
	m_NowUseDataCount--;

	return 1;
}
*/
