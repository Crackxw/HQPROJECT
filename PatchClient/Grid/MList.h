#ifndef __MLIST_H__
#define __MLIST_H__

#include <windows.h>
#define SAFE_DELETE(p)      { if ( (p) != NULL ) { delete (p);     (p) = NULL; } }
class CMNode												
{

public:

		CMNode()
		{
			Data	= NULL;
			pBack	= NULL;
			pNext	= NULL;
		};

		~CMNode()
        {
			pBack	= NULL;
			pNext	= NULL;
        };
        
        LPVOID	Data;		// ����Ǵ� ������ (LPVOID �����̴�)
        CMNode*	pBack;		// �ٷ� ���� ������ ������
        CMNode*	pNext;		// �ٷ� ���� ������ ������        
};

class CMList  
{
public:
	CMList();
	virtual ~CMList();

public:    
    int     AddHead     ( LPVOID data );                            // ������ ���� 
    int     AddTail     ( LPVOID data );                            // ������ ����
    int     GetCount    ( void ) const { return m_TotalCount; };	// ����� ��� ������ ���� ���
    BOOL    RemoveAt    ( int index );	    			            // ������ ����
    void*   GetAt	    ( int index );							    // ������ ���
    void    RemoveAll   ( void );                                   
    void    SetPos      ( int index );
    void*   ToHead      ( int index );
    
private:    
    CMNode   *m_Head;
    CMNode   *m_Tail;
    CMNode   *m_Point;
    int		m_TotalCount;											// ����� ��� ������ ����
    int     m_Pos;

};

#endif
