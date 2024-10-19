// MList.cpp: implementation of the CMList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMList::CMList()
{   
    m_TotalCount    = 0;
    m_Pos           = 0;
    m_Head			= NULL;
    m_Tail          = NULL;    
    m_Point         = NULL;
}

CMList::~CMList()		
{
    CMNode *pTemp, *pNext = m_Head;						
    
    while ( pNext )
    {        
        pTemp = pNext;
        pNext = pNext->pNext;
//		SAFE_DELETE ( pTemp->Data );
        SAFE_DELETE ( pTemp );    
    }

    m_TotalCount    = 0;
    m_Pos           = 0;
    m_Head          = NULL;
    m_Tail          = NULL;    
    m_Point         = NULL;
}

int  CMList::AddTail ( LPVOID data )
{
    
    CMNode *T_Node       = new CMNode;				 
    
    T_Node->Data	    = data;
    T_Node->pNext		= NULL;             	    
    T_Node->pBack		= m_Tail;   				
    
    if (m_Tail)
        m_Tail->pNext = T_Node;
    else
        m_Head = T_Node;
    
    m_Point = m_Tail = T_Node;
    m_Pos = m_TotalCount++;
    
    return m_TotalCount;    
}


int  CMList::AddHead ( LPVOID data )
{
    CMNode *T_Node       = new CMNode;
    
    T_Node->Data	    = data;
    T_Node->pNext		= m_Head;             
    T_Node->pBack		= NULL;   				       

    if (m_Head) 
        m_Head->pBack = T_Node;
    else
        m_Tail = T_Node;

    m_Head = m_Point = T_Node;        
    m_Pos = 0;
    m_TotalCount++;
    
    return TRUE;    
}


BOOL CMList::RemoveAt ( int index )
{    
    if ( m_TotalCount <= index ) return FALSE;
    
    SetPos ( index );

    if (!index)
    {
        m_Head = m_Point->pNext;
        delete m_Point;
        m_Point = m_Head;
        if (m_TotalCount==1) m_Tail = NULL;
    }
    else if (index == m_TotalCount-1)
    {
        m_Tail = m_Point->pBack;
        SAFE_DELETE( m_Point);
        m_Point = m_Tail;
        m_Pos--;
    }
    else
    {
        (m_Point->pBack)->pNext = m_Point->pNext;
        (m_Point->pNext)->pBack = m_Point->pBack;
        CMNode *temp = m_Point->pBack;
        SAFE_DELETE( m_Point );
        m_Point = temp;        
        m_Pos--;
    }

    m_TotalCount--;
    
    return TRUE;
}

void   CMList::RemoveAll ( void )
{    
    CMNode * pTemp, *pNext = m_Head;						
    
    while ( pNext != NULL )
    {
        pTemp = pNext;
        pNext = pNext->pNext;
        SAFE_DELETE ( pTemp );
    }

    m_TotalCount = 0;
    m_Head = NULL;
    m_Tail = NULL;
}

LPVOID CMList::GetAt ( int index )
{   
    if ( m_TotalCount <= index ) return NULL;    

    SetPos( index );

    return m_Point->Data;
}

void CMList::SetPos( int index )
{
    int count = index - m_Pos;

    if (!count) return ;

    if (count>0)
    {
        if ( count > (m_TotalCount-1)-index )
        {
            m_Pos = m_TotalCount-1;
            m_Point = m_Tail;
            count = -((m_TotalCount-1)-index);
        }
    }
    else
    {
        if ( (-count) > index )
        {
            m_Pos = 0;
            m_Point = m_Head;
            count = index;
        }
    }

    if (count>0)
    {
        for (int i=0 ; i< count ; i++)
        {
            m_Point = m_Point->pNext;
            m_Pos ++;
        }
    }
    else 
    {
        for (int i=0; i<(-count); i++)
        {
            m_Point = m_Point->pBack;
            m_Pos--;
        }
    }
}

void* CMList::ToHead( int index )
{
    if ( m_TotalCount <= index  || index<=0 ) return m_Head->Data;    
    
    SetPos( index );

    (m_Point->pBack)->pNext = m_Point->pNext;
    if (m_Point->pNext) (m_Point->pNext)->pBack = m_Point->pBack;
    else m_Tail = m_Point->pBack;
    
    m_Point->pNext = m_Head;
    m_Point->pBack = NULL;  
    m_Head->pBack = m_Point;
    m_Head = m_Point;
    
    m_Pos = 0;    

    return m_Point->Data;
}