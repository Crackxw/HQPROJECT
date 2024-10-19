#ifndef __CAUTHORIZER__
#define __CAUTHORIZER__





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CAuthorizer
{

public:
	CAuthorizer() {};
	virtual ~CAuthorizer() {};

	virtual int			reqLogin   ( char* id, char* password, char* ip, DWORD* idNumber )	= 0;
	virtual int			reqLogout  ( char* id, char* password, char* ip, DWORD* idNumber )	= 0;
	virtual int			reqRecCash ( char* id, char* password, char* ip, DWORD* idNumber )	= 0;
};





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CNoneAuthorizer: public CAuthorizer
{

public:
	CNoneAuthorizer() {};
	~CNoneAuthorizer() {};



	int			reqLogin   ( char* id, char* password, char* ip, DWORD* idNumber ) { return TRUE; }
	int			reqLogout  ( char* id, char* password, char* ip, DWORD* idNumber ) { return TRUE; }
	int			reqRecCash ( char* id, char* password, char* ip, DWORD* idNumber ) { return TRUE; }
};




#endif