#ifndef __CRAS__
#define __CRAS__

typedef DWORD	_dword;
typedef WORD	_word;


class CAuthorizer;


//������������������������������������������������������������������������������������������������������������������������
class CRas: public CAuthorizer
{
private:
	int			checkResultString ( char* resString );


public:
	CRas();
	~CRas();

	


	int			reqLogin  (char* id, char* password, char* ip, DWORD* idNumber); 
	int			reqLogout (char* id, char* password, char* ip, DWORD* idNumber); 
	int			reqRecCash(char* id, char* password, char* ip, DWORD* idNumber); 
};


#endif