#ifndef __CGASH__
#define __CGASH__


typedef DWORD			_dword;
typedef WORD			_word;


class CAuthorizer;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CGash: public CAuthorizer
{
private:
	const _dword	m_clsContext;


private:
	int			checkResultString ( char* resString );


public:
	CGash();
	~CGash();

	


	int			reqLogin  (char* id, char* password, char* ip, DWORD* idNumber); 
	int			reqLogout (char* id, char* password, char* ip, DWORD* idNumber); 
	int			reqRecCash(char* id, char* password, char* ip, DWORD* idNumber); 
};


#endif