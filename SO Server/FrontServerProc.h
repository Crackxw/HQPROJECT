#ifndef __CFRONTSERVERPROC__
#define __CFRONTSERVERPROC__

#include "../FrontServer/_FrontDefine.h"



class CWinsock;



//����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//
// class : 
//
//����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
class CFrontServerProc: public CWinsock
{

public:
	CFrontServerProc();	
	~CFrontServerProc();

	
	
	//--------------------------------------------------------------------------------
	bool					ReportConcurrent    ( int world_id, int concurrent );



private:
	
	//--------------------------------------------------------------------------------
	bool					Connect();

	bool					Send(char* data, int dataSize);


	int						getPacketSize (char* header);	
	int						getChecksum   (char* header);	
	int						calcChecksum  (char* data, int size);

};



#endif


