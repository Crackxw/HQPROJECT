#ifndef __CMAP__
#define __CMAP__


#pragma warning(disable: 4786)
#pragma warning(disable: 4503)

#include <map>
using namespace std;




template<typename KEY_T, typename DATA_T>
class CMap
{
private:
	std::map<KEY_T, DATA_T>					m_map;																					
	std::map<KEY_T, DATA_T>::iterator		m_mapCurPos;


public:

	CMap()
	{
		setFirst();
	}
		
	~CMap()
	{

	}

	void			insert(KEY_T key, DATA_T data)
	{
		m_map.insert( std::map<KEY_T,DATA_T>::value_type(key,data) );
	}

	void			erase(KEY_T key)
	{
		m_map.erase(key);
	}

	void			setFirst()
	{
		m_mapCurPos = m_map.end();
	}

	DATA_T getNext()
	{
		if( m_mapCurPos ==  m_map.end() ) return NULL;
			
		++m_mapCurPos;
		return (m_mapCurPos->second);
	}
};

#endif