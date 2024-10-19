#ifndef __SKILL_DEFINE__
#define __SKILL_DEFINE__



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// DEFINE
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

namespace SKILL 
{	
	// 할당받은 command ( 9000 ~ 9004 ) 
	enum 
	{
			REQUEST_LEVELUP			 = 9000   ,				    // 스킬 레벨업 요청  
			RESPONSE_LEVELUP				  ,					// 스킬 레렙업에 대한 응답 			
	};

	// 필수로 전역 데이타를 써야 할	선언!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	const int	MERCENARY_NAME_LENGTH =	10;						// 용병 이름 길이 		
	const int	GUILD_NAME_LENGTH	  = 10;						// 길드 이름 길이 
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	enum
	{
			SPRINKLE_MONEY			 = 10  ,					// 1.  돈 뿌리기술 
			COLLECT_HERB			 = 20  ,					// 2.  약초 채집술 
			FARMING					 = 30  ,				    // 3.  농사 기술 
			LUMBERING				 = 40  ,					// 4.  나무 베기술
			BREEDING_STOCK			 = 50  ,					// 5.  목장 관리술 
			MINING					 = 60  ,					// 6.  채광술 
			EMPLOY_SENIORITY_CARRIER = 70  ,					// 7.  고급짐꾼 고용술 
			MAKING_TOOL				 = 80  ,					// 8.  도구 제조술  
			OPEN_STALL				 = 90  ,					// 9.  노점 개설술 
			MANAGE_GUILD			 = 100 ,					// 10. 상단 개설술 
			REFINING_WEAPON			 = 110 ,					// 11. 무기 제련술 
			REFINING_PANOPLY		 = 120 ,					// 12. 갑주 제련술 		
			EMPLOY_GENERAL			 = 130 ,					// 13. 장수 고용술 
			CLOAKING				 = 140 ,					// 14. 은둔술 		
			WARP					 = 150 ,					// 15. 축지법술 
	};

	enum
	{
		RESPONSE_USE_SKILL			= 9102
	};



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//  PACKET  
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	
//-----------------------------------------------------------------------------------------------------------------------
//	Name : sRequestLevelUp
//	Desc : 스킬 레벨업 요청 구조체
//-----------------------------------------------------------------------------------------------------------------------
typedef struct __sRequestLevelUp
{
	DWORD		command;							// msgType

	DWORD		skillKind;							// 스킬 종류 
	
}sRequestLevelUp;


//-----------------------------------------------------------------------------------------------------------------------
//	Name : sResponseLevelUp
//	Desc : 스킬 레벨업에 대한 응답 구조체 
//-----------------------------------------------------------------------------------------------------------------------
typedef struct __sResponseLevelUp
{
	DWORD		command;							// msgType

	DWORD		skillKind;
	DWORD		responseKind;						// 응답 종류 ( 성공, 실패 )

}sResponseLevelUp;


};

#endif