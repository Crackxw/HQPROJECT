/*CURRENT_VERSION	20031222001*/
/*
 *	Version 20031222001
 *	 - DLG_ITEM_FOUND 값이 DLG_ITEM_SEMI_FOUND 로 고정되어 출력되는 문제 수정.
 *
 *	Version 20031219001
 *	 - GetDLGitemTextA(void) 함수의 결과값의 형(type)변경. BOOL -> INT
 *	 - 5가지 상태를 결과 값으로 되돌려 줌.
 *		DLG_ITEM_NOT_GET_SSHOT		: 라이브러리 모듈 에러 2
 *		DLG_ITEM_NOT_LOAD_MODULE	: 라이브러리 모듈 에러 1
 *		DLG_ITEM_NOT_FOUND			: 매크로 또는 의심이 가는 모듈 검출 없음.
 *		DLG_ITEM_FOUND				: 등록된 매크로의 검출 및 제거 완료.
 *		DLG_ITEM_SEMI_FOUND			: 의심이 가는 모듈 검출
 *		DLG_ITEM_FOUND_TWO			: 의심이 가는 모듈 검출 및 등록된 매크로의 검출 및 제거 완료.
 *
 *	Version	20031218001
 *	 - 20031217003.버전에서 Log파일 출력시 모든 프로세서를 출력 하던 부분을 마작 매크로로 추정되는 프로세서에 한해 출력 되게 수정.
 *	 - 프로세스 메모리내에 로드된 매크로로 의심되는 모듈의 제거방법 변경.
 *	 - 내부 테스트용 라이브러리 제거.
 *
 *	Version 20031217003
 *	 - 20031217002.버전에서 Log파일이 무한히 커지는 버그 수정.
 *
 *	Version 20031217002
 *	 - 현재 메모리에서 매크로로 추정되는 모듈 발견시 로그파일로 출력하는 기능 추가.
 *	 - 내부 테스트용 라이브러리 추가.
 *
 *	Version 20031217002 이전 버전들
 *	 - 특기 사항 없음.
 */

typedef enum	DLG_ITEM_TEXT
{
	DLG_ITEM_NOT_GET_SSHOT=-2,		//Couldn't Start This Module #2 /*라이브러리 모듈 에러 2*/
	DLG_ITEM_NOT_LOAD_MODULE=-1,	//Couldn't Start This Module #1 /*라이브러리 모듈 에러 1*/
	DLG_ITEM_NOT_FOUND=0,			//Must Return This 0!! <Normal state, Doubtfull or Macro stuffs does not exist in Memory./*매크로 또는 의심이 가는 모듈 검출 없음.*/
	DLG_ITEM_FOUND =1,				//Find Autoplay	/*등록된 매크로의 검출 및 제거 완료.*/
	DLG_ITEM_SEMI_FOUND=2,			//Find Doubtfull Stuff in memory of Client /*의심이 가는 모듈 검출*/
	DLG_ITEM_FOUND_TWO = 3			//Find DLG_ITEM_FOUND and DLG_ITEM_SEMI_FOUND STATE /*의심이 가는 모듈 검출 및 등록된 매크로의 검출 및 제거 완료.*/
};
extern inline int GetDLGitemTextA(void);
