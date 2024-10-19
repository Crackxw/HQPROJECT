#ifndef _INCLUDE_LOGINLIB_LIB____
#define _INCLUDE_LOGINLIB_LIB____

int Joy_RAS_Function(
		int ServiceSN		// 서비스 코드

		,char ProcessSN		// 처리 타입 메뉴얼 참조
							// 연결되는 게임에 따라서 타입이 달라질 수 있다.


		,int Usernumber		// 조이온 유저고유번호 (안쓰는 경우 -1 넣을것)

		,char *Userid		// 조이온 아이디
		,int Useridsize		// 조이온 아이디의 길이(NULL문자 제외)
		,char *Rascode		// 이것은. 패스워드거나, 웹에서 넘겨준 인증 코드.
		,int Rascodesize	// 위 스트링의 길이(NULL문자 제외)
		,char *Userip		// 지금 접속한 클라이언트의 아이피 주소. - 예) "10.1.1.127" .
		,int Useripsize		// 위 스트링의 길이(NULL문자 제외)

		,char *Rasserverip	// RAS 서버 의 접속 아이피 주소
		,int Rasport		// RAS 서버 의 접속 포트 번호
		,char *RasMsg		// 결과값이 들어갈 char* 형 버퍼 포인터
							// 연결되는 게임에 따라서 버퍼의 사이즈가 조정될 수 있다.
		,int RasMsgmaxsize	// RasMsg 로 넘길 버퍼의 최대 사이즈(NULL문자 제외)..
							// 리턴 받은 데이터가 이 버퍼를 넘기면 에러를 리턴한다.
							// 최소 256 BYTE 이상이어야 한다.
		);

// 리턴 값은 메뉴얼 State 참조.

#endif