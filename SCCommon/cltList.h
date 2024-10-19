#ifndef _CLT_LIST_H_
#define _CLT_LIST_H_

// #include <GSL.h>

template<class Type>
class cltList
{
private:
	int		m_nLen;
	int		m_nMax;
	int		m_nPos;
	Type	*m_pData;    
	
public:
	
	cltList()
	{
		m_nLen	=	0; 
		m_nMax	=	0;
		m_nPos	=	-1;
		m_pData	=	NULL;
	}

	virtual		~cltList()
	{
		Release();
	}

	void	Create( int nSize )
	{
		if( m_pData )	Release();
		m_nLen	=	0;
		m_nMax	=	nSize;
		m_nPos	=	-1;
		m_pData	=	new Type[nSize];
	}

	void	Release() 
	{
		if( m_pData )
		{
			delete[] m_pData;
			m_pData	=	NULL;
		}
		m_nLen	=	0; 
		m_nMax	=	0;
		m_nPos	=	-1;
	}

	void	MakeEmpty()
	{
		m_nLen	=	0;
		m_nPos	=	-1;
	}

	bool	Add( Type *pData )
	{
		if( m_nLen >= m_nMax )		return false;
		memcpy( &m_pData[m_nLen], pData, sizeof(Type) );
		m_nLen++;
		return true;
	}

	bool	Insert( int index, Type *pData )
	{
		if( m_nLen >= m_nMax )		return false;
		if( m_nLen < index )		return false;
		if( m_nLen == index )		return Add(pData);

		memmove( &m_pData[index+2], &m_pData[index+1], sizeof(Type)*(m_nLen-(index+1)) );
		memcpy(&m_pData[index+1], pData, sizeof(Type));
		m_nLen++;
		return true;
	}

	bool	Delete( int index )
	{
		if( index >= m_nLen || index < 0)		return false;
		memmove( &m_pData[index], &m_pData[index+1], sizeof(Type) * (m_nLen-(index+1)) );
		m_nLen--;
		return true;
	}

	bool	Delete( Type *pData )
	{
		int		i;
		for( i = 0; i < m_nLen; i++ )
		{
			if( memcmp(&m_pData[i], pData, sizeof(Type)) == 0 )
			{
				return Delete(i);
			}
		}
		return false;
	}

	void	ResetPos()
	{
		m_nPos	=	-1;
	}

	void	ResetEndPos()
	{
		m_nPos	=	m_nLen;
	}

	Type*	GetNextData()
	{
		if( ++m_nPos >= m_nLen )			return NULL;
		return &m_pData[m_nPos];
	}

	Type*	GetPrevData()
	{
		if( --m_nPos < 0 )					return NULL;
		return &m_pData[m_nPos];
	}

	Type*	GetData( int index )
	{
		if( index >= m_nLen || index < 0)	return NULL;
		return &m_pData[index]; 
	}

	int		GetLength()
	{
		return m_nLen; 
	}

	int		GetPos(Type *pData)
	{
		int		i;
		for( i = 0; i < m_nLen; i++ )
		{
			if( memcmp(&m_pData[i], pData, sizeof(Type)) == 0 )
			{
				return i;
			}
		}
		return -1;
	}

	int		GetCurPos()
	{
		return m_nPos;
	}
};


#endif