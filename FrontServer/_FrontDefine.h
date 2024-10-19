#ifndef __FRONT__
#define __FRONT__


namespace FRONT {



	// -- 공통 적인 응답 종류 
	enum {

		UNKNOWN_FAIL						    = -99 ,			// 알수 없는 실패 
		OK										=   1			// 성공 

	};


	const int  MAX_WORLD						=  99;			// 월드의	   최대 개수 
	const int  WORLDNAME_MAX_LENGTH				=  21;			// 월드 이름의 최대 길이 
	const int  NOTICE_MAX_LENGTH				=  3000;		// 공지 사항의 최대 길이 




	// -- 월드 현재 상태 종류 
	enum {

		NO_HEARTBEAT							= -98			// 허트비트 없음 
			
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

	
	
	


	// -- 월드 이름 설정 
	void	   setName(const char* a)
	{
		if( a ) 
		{
			memset(name, 0, sizeof(name));
			strcpy(name, a);
		}
	}	


	// -- 월드 '생성년월일' 설정 	
	void	   setCreateDate( WORD year, BYTE month, BYTE day )
	{
		DWORD high = ( ((DWORD)year) << 16 );
		DWORD low  = ( ((WORD)month) << 8 ) | day;

		create_date = high | low;
	}
	
	
	// -- 월드 생성년도 얻기 
	WORD	   getYear()
	{
		return	(WORD)(create_date >> 16);
	}

	
	// -- 월드 생성월 얻기 
	BYTE	   getMonth() 
	{
		return (BYTE)((create_date & 0x0000FF00) >> 8);
	}

	
	// -- 월드 생성일 얻기
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


	
	// -- 게이트 서버 아이피 설정 
	void	   setGateIP(char* a)
	{
		if( a ) 
		{
			memset(gateIP, 0, sizeof(gateIP));
			strcpy(gateIP, a);
		}
	}


	// -- 채트 서버 아이피 설정 
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





