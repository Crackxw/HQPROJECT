#ifndef __EDITSTACK__
#define __EDITSTACK__


#include <string.h>


template<class Type>
class SStack
{
public:
	int		m_MaxDataCount;									// Max Data Count
	int		m_NowUseDataCount;								// Now Use Data Count
	Type*	m_Data;											// Data

public:
	SStack()												// Constuctor
	{
		m_MaxDataCount    = 0;
		m_NowUseDataCount = 0;
		m_Data            = NULL;
	}
	virtual ~SStack()										// Destroyer
	{
		Release();
	}

	int		Create(int Size)								// Create
	{
		if(m_Data) Release();

		m_MaxDataCount    = Size;
		m_NowUseDataCount = 0;
		m_Data            = new Type[Size];

		return 1;
	}
	void	Release(void)									// Release
	{
		if(m_Data)
		{
			delete[] m_Data;
			m_Data = NULL;
		}
		m_MaxDataCount    = 0;
		m_NowUseDataCount = 0;
	}

	int		Add(Type Data)									// Add
	{
		if(m_NowUseDataCount >= m_MaxDataCount) return -1;

		// Data추가
		memmove(&m_Data[m_NowUseDataCount], &Data, sizeof(Type));
		m_NowUseDataCount++;

		return (m_NowUseDataCount - 1);
	}
	int		Delete(int ID)									// Delete
	{
		if(ID >= m_NowUseDataCount) return -1;

		// Data 삭제
		memmove(&m_Data[ID], &m_Data[ID + 1], sizeof(Type) * (m_NowUseDataCount - (ID + 1)));
		m_NowUseDataCount--;

		return 1;
	}
	void	DeleteAll(void)									// Delete All
	{
		m_NowUseDataCount = 0;		
	}
	
	int		DeleteType(Type Data)							// Delete Type
	{
		int		TempCount;

		for(TempCount = 0; TempCount < m_NowUseDataCount; TempCount++)
		{
			if(memcmp(&m_Data[TempCount], &Data, sizeof(Type)) == 0)
			{
				return Delete(TempCount);				
			}
		}
		return -1;
	}

	BOOL FindType(Type Data)
	{
		int		TempCount;

		for(TempCount = 0; TempCount < m_NowUseDataCount; TempCount++)
		{
			if(memcmp(&m_Data[TempCount], &Data, sizeof(Type)) == 0)
			{
				return  TRUE;				
			}
		}

		return FALSE;
	}

	
};


#endif
