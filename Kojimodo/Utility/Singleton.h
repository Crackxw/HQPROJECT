#ifndef __CSINGLETON__
#define __CSINGLETON__


#include <assert.h>

template <typename T>
class CSingleton
{

private:
	static T*	  m_pSingleton;
	
protected:	
	CSingleton()
	{
		assert( m_pSingleton == NULL );

		int offset = (int)(T*)1 - (int)(CSingleton<T>*)(T*)1;

		m_pSingleton = (T*)((int)this + offset);		
	}

	~CSingleton()
	{
		assert( m_pSingleton ); 

		m_pSingleton = NULL;
	}


public:
	static T*	P()
	{
		assert( m_pSingleton );

		return m_pSingleton;	
	}

};

template <typename T> T*   CSingleton<T>::m_pSingleton = NULL;

#endif