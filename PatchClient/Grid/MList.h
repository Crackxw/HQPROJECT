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
        
        LPVOID	Data;		// 저장되는 데이터 (LPVOID 형태이다)
        CMNode*	pBack;		// 바로 앞의 데이터 포인터
        CMNode*	pNext;		// 바로 뒤의 데이터 포인터        
};

class CMList  
{
public:
	CMList();
	virtual ~CMList();

public:    
    int     AddHead     ( LPVOID data );                            // 데이터 삽입 
    int     AddTail     ( LPVOID data );                            // 데이터 삽입
    int     GetCount    ( void ) const { return m_TotalCount; };	// 저장된 모든 데이터 갯수 얻기
    BOOL    RemoveAt    ( int index );	    			            // 데이터 삭제
    void*   GetAt	    ( int index );							    // 데이터 얻기
    void    RemoveAll   ( void );                                   
    void    SetPos      ( int index );
    void*   ToHead      ( int index );
    
private:    
    CMNode   *m_Head;
    CMNode   *m_Tail;
    CMNode   *m_Point;
    int		m_TotalCount;											// 저장된 모든 데이터 갯수
    int     m_Pos;

};

#endif
