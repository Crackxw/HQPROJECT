#ifndef __LOGIN__
#define __LOGIN__


namespace LOGIN {


	enum {

		REQUEST_LOGIN						  = 30000 ,			// 로그인을   요청 한다 
		REQUEST_LOGOUT								  ,			// 로그아웃을 요청 한다 
																// (게임 서버가 재시작 될때)
		
		REQUEST_NEW_CHAR							  ,			// 새로운 캐릭터 생성을 요청 한다 
		REQUEST_DEL_CAHR							  ,			// 기존 캐릭터의 삭제를 요청 한다 
		REQUEST_CHARS_INFO							  ,			// 캐릭터들의 정보를 요청 한다 
		

		RESPONSE_LOGIN								  ,			// 로그인 요청에 응답 한다 
		RESPONSE_LOGOUT								  ,			// 로그아웃 요청에 응답 한다 


		RESPONSE_NEW_CHAR							  ,			// 새로운 캐릭터 생성 요청에 응답 한다 
		RESPONSE_DEL_CHAR							  ,			// 기존 캐릭터의 삭제 요청에 응답 한다 
		RESPONSE_CHARS_INFO							  			// 캐릭터들의 정보 요청에 응답 한다 

	};


	// 공통적인 응답 종류			( !! DB에서도 쓰인다 !! )
	enum {

		UNKNOWN_FAIL							= -99 ,			// 알수 없는 실패 
		OK										=	1			// 성공 

	};


	// 로그인에 대한 응답 종류		( !! DB에서도 쓰인다 !! )
	enum {

		NOTHAS_AUTH							    = -98 ,			// 권한을 가지고 있지 않은 경우 
		INVALID_ID									  ,			// 아이디가 올바르지 않은 경우 
		INVALID_PASSWORD							  ,			// 패스워드가 올바르지 않은 경우 
		OVER_PLAY_VALIDITY							  ,			// 사용 기간(시간)이 만료된 경우 
		OVER_PLAY_VALIDITY_TIME_PERDAY				  ,			// 일일 사용 기간이 만료된 경우 
		OVER_PLAY_VALIDITY_TERM_PERDAY				  ,			// 일일 사용 시간이 만료된 경우 
		ALREADY_LOGIN								  ,			// 이미 로그인 되어 있는 경우 
		USED_ALL_IP									  ,			// 결제한 IP수량을 모두 사용중인 경우 
		
		NOT_ENOUGHT_POINT							  ,			// 결제 포인트가 부족한 경우 
		FAIL_SHORT_TIME								  ,			// 5분 동안 3번 이상 실패하여 잠겨 있는 경우 
		LOCK_MAIN_ACCOUNT							  ,			// 메인 계정이 잠겨 있는 경우 
		LOCK_SERVICE_ACCOUNT						  ,			// 서비스/게임 계정이 잠겨 있는 경우
		FAIL_CHANGE_STATE							  ,			// 로그인 상태 변경에 실패 했을 경우 
		TOOMANYUSERS								  			// 사용자수 초과
	};
	

	// 캐릭터 생성에 대한 응답 종류 
	enum {

		REPEAT_NAME								= -98 ,			// 동일한 이름이 있는 경우 		
		RESTRICT_NAME								  ,			// 금지된 문자열이 있는 이름인 경우 
	};





	const int ID_MAX_LENGTH						= 21;			// 아이디 최대 길이  
	const int PW_MAX_LENGTH						= 21;			// 패스워드 최대 길이 
	const int NAME_MAX_LENGTH					= 11;			// 이름의 최대 길이 

	const int CHAR_MAX_NUM						= 3;			// 로그인 캐릭터의 최대 수 
																// (LIMIT = 255)






//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 캐릭터의 기본 로그인 정보 
//--------------------------------------------------------------------------------------------
struct sCharInfo
{
	BYTE		slot;									
	char		name[NAME_MAX_LENGTH];						
	WORD		kind;										
	__int64		mnMoney;										// 소지금
	int			credit;											// 신용도
	int			level;											// 레벨
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인을 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestLogin
{
	DWORD			command;
	
	char			id		 [ID_MAX_LENGTH];		
	char			password [PW_MAX_LENGTH];			
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseLogin
{
	DWORD			command;
	
	char			responseKind;				    // 응답 종류 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 새로운 캐릭터 생성을 요청 한다  
//--------------------------------------------------------------------------------------------
struct sRequestNewChar
{
	DWORD			command;

	char			name [NAME_MAX_LENGTH];			// 캐릭터의 이름  
	WORD			kind;							// 캐릭터의 종류 
	
																							// (분배된 보너스 수치값) 
	WORD			Str;							// 힘		 
	WORD			Dex;							// 민첩성			
	WORD			Vit;							// 체력				
	WORD			Int;							// 지력
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 새로운 캐릭터 생성 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseNewCharFront
{
	DWORD			command;

	char			responseKind;					// 성공 1 : 실패 : 0
	
};

struct sResponseNewChar : public sResponseNewCharFront
{
	BYTE			newSlot;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 기존 캐릭터 삭제를 요청 한다  
//--------------------------------------------------------------------------------------------
struct sRequestDelChar
{
	DWORD			command;
	
	BYTE			delSlot;						// 삭제할 캐릭터의 슬롯 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 기존 캐릭터 삭제 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseDelChar
{
	DWORD			command;
	
	char			responseKind;					 // 성공 1 : 실패 0
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 캐릭터들의 정보를 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestCharsInfo
{ 
	DWORD			command;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 캐릭터들의 정보 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseCharsInfoFront
{
	DWORD			command;
	BYTE			numChar;						// 캐릭터 수 
};

struct sResponseCharsInfo: public sResponseCharsInfoFront
{
	sCharInfo		charsInfo[CHAR_MAX_NUM];		// 캐릭터들의 정보 
};



};

#endif



