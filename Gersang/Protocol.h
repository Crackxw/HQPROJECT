#ifndef _CHATTING_PROTOCOL_H
#define _CHATTING_PROTOCOL_H
// 0: 로그인 관련 요청, 1: 채팅 관련 요청, 2,3: 전체맵에서 요청, 4: 도시에서 요청
// 5: 로그인 관련 응답, 6: 채팅 관련 응답, 7,8: 전체맵에서 응답, 9: 도시에서 응답
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속 거부
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_RESPONSE_SERVER_FULL				'\xff'	// 사용자가 너무 많아서 접속을 거부한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속하는 클라이언트의 종류를 구분한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_CHATTING_REQUEST_CLIENTKIND					'\x01'	
#define ON_CHATTING_RESPONSE_CLIENTKIND					'\x51'

#define	ON_CHATTING_RET_CLIENT_KIND_OK					'\x00'	// 정상적인 클라이언트다.
#define ON_CHATTING_RET_CLIENT_KIND_CLIENT				'\x01'	// 다른 클라이언트다.
#define ON_CHATTING_RET_CLIENT_KIND_USER				'\x02'	// 해당 유저용의 클라이언트가 아니다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그인한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_LOGIN						'\x02'
#define ON_CHATTING_RESPONSE_LOGIN						'\x52'

#define ON_CHATTING_RET_LOGINSERVER_LOGIN_OK			'\x00'	// 로그인을 허락한다.
#define ON_CHATTING_RET_GAMESERVER_LOGIN_OK				'\x01'	// 로그인을 허락한다.
#define ON_CHATTING_RET_LOGIN_ID						'\x02'	// 같은 아이디가 이미 접속중이다.
#define ON_CHATTING_RET_LOGIN_NO						'\x03'	// 로그인할 수 없다.
#define ON_CHATTING_RET_CHATSERVER_LOGIN_OK				'\x04'	// 로그인을 허락한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그아웃한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_LOGOUT						'\x03'
#define ON_CHATTING_RESPONSE_LOGOUT						'\x53'

#define ON_CHATTING_RET_LOGOUT_OK						'\x00'	// 로그 아웃을 허락한다.
#define ON_CHATTING_RET_LOGOUT_NO						'\x01'	// 로그 아웃할 수 없다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 대화할 상대를 추가하거나 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_CONTROLCHAR					'\x04'

#define ON_CHATTING_REQ_CONTROLCHAR_ADD					'\x00'	// 상대를 추가한다.
#define ON_CHATTING_REQ_CONTROLCHAR_DEL					'\x01'	// 상대를 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 채팅리스트 삭제
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_INITLIST					'\x05'	// 전투나 마을에 들어갈때...
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 메세지를 전달한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_MSG							'\x11'
#define ON_CHATTING_RESPONSE_MSG						'\x61'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 귓속말을 전달한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_EAR_MSG						'\x12'
#define ON_CHATTING_RESPONSE_EAR_MSG					'\x62'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 해당 플레이어에대해 대화거부를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHATTING_REQUEST_IGNORE						'\x14'
#define ON_CHATTING_RESPONSE_IGNORE						'\x64'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#endif