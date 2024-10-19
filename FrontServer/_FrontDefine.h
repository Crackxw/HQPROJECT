#ifndef __FRONT__
#define __FRONT__


namespace FRONT {



	// -- ���� ���� ���� ���� 
	enum {

		UNKNOWN_FAIL						    = -99 ,			// �˼� ���� ���� 
		OK										=   1			// ���� 

	};


	const int  MAX_WORLD						=  99;			// ������	   �ִ� ���� 
	const int  WORLDNAME_MAX_LENGTH				=  21;			// ���� �̸��� �ִ� ���� 
	const int  NOTICE_MAX_LENGTH				=  3000;		// ���� ������ �ִ� ���� 




	// -- ���� ���� ���� ���� 
	enum {

		NO_HEARTBEAT							= -98			// ��Ʈ��Ʈ ���� 
			
	};



















//--------------------------------------------------------------------------------------------
//  name : 
//  desc : 
//--------------------------------------------------------------------------------------------
struct sWorldInfo								
{
	char	   name [WORLDNAME_MAX_LENGTH];		
	WORD	   world_id;							
	short	   world_state;
	WORD	   concurrent;						
	DWORD	   create_date;

	
	
	


	// -- ���� �̸� ���� 
	void	   setName(const char* a)
	{
		if( a ) 
		{
			memset(name, 0, sizeof(name));
			strcpy(name, a);
		}
	}	


	// -- ���� '���������' ���� 	
	void	   setCreateDate( WORD year, BYTE month, BYTE day )
	{
		DWORD high = ( ((DWORD)year) << 16 );
		DWORD low  = ( ((WORD)month) << 8 ) | day;

		create_date = high | low;
	}
	
	
	// -- ���� �����⵵ ��� 
	WORD	   getYear()
	{
		return	(WORD)(create_date >> 16);
	}

	
	// -- ���� ������ ��� 
	BYTE	   getMonth() 
	{
		return (BYTE)((create_date & 0x0000FF00) >> 8);
	}

	
	// -- ���� ������ ���
	BYTE	   getDay() 
	{
		return (BYTE)(create_date & 0x000000FF);
	}

};

struct sWorldInfo2: public sWorldInfo			
{
	char	   gateIP[16];						
	WORD	   gatePort;

	char	   chatIP[16];						
	WORD	   chatPort;


	
	// -- ����Ʈ ���� ������ ���� 
	void	   setGateIP(char* a)
	{
		if( a ) 
		{
			memset(gateIP, 0, sizeof(gateIP));
			strcpy(gateIP, a);
		}
	}


	// -- äƮ ���� ������ ���� 
	void	   setChatIP(char* a)
	{
		if( a ) 
		{
			memset(chatIP, 0, sizeof(chatIP));
			strcpy(chatIP, a);
		}
	}

};




};



#endif





