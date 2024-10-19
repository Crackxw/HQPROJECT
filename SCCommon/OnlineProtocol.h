#ifndef _ONLINE_PROTOCOL_H
#define _ONLINE_PROTOCOL_H
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투서버와의 프로토콜
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0: 게임서버->전투서버,	1:플레이어->전투서버
// 5: 전투서버->게임서버,	6:전투서버->플레이어
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 방 생성한다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_CREATE_BATTLEROOM			'\x01'
#define ON_RESPONSE_CREATE_BATTLEROOM			'\x51'

#define ON_RET_CREATE_BATTLEROOM_OK				'\x00'
#define ON_RET_CREATE_BATTLEROOM_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 방을 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_DELETE_BATTLEROOM			'\x03'
#define ON_RESPONSE_DELETE_BATTLEROOM			'\x53'

#define ON_RET_DELETE_BATTLEROOM_OK				'\x00'
#define ON_RET_DELETE_BATTLEROOM_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 방에 플레이어를 조인 시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_JOIN_BATTLEROOM				'\x02'
#define ON_RESPONSE_JOIN_BATTLEROOM				'\x52'

#define ON_RET_JOIN_BATTLEROOM_OK				'\x00'
#define ON_RET_JOIN_BATTLEROOM_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_BEGIN_BATTLEROOM				'\x05'
#define ON_RESPONSE_BEGIN_BATTLEROOM			'\x55'

#define ON_RET_BEGIN_BATTLEROOM_OK				'\x00'
#define ON_RET_BEGIN_BATTLEROOM_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어들간의 메세지를 중계한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_BATTLE_MESSAGE				'\x10'
#define ON_RESPONSE_BATTLE_MESSAGE				'\x60'

#define ON_RET_BATTLE_MESSAGE_OK				'\x00'
#define ON_RET_BATTLE_MESSAGE_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투서버로부터 특정 메세지를 받는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_ADD_CRITICALDATA				'\x0A'
#define ON_RESPONSE_ADD_CRITICALDATA			'\x5A'

#define ON_RET_ADD_CRITICALDATA_OK				'\x00'
#define ON_RET_ADD_CRITICALDATA_NO				'\x01'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투 서버에 있는 플레이어의 정보를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_REQUEST_BATTLE_PLAYERINFO			'\x0B'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투 서버에서 방이 삭제되었다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_BATTLEROOM_FINISHED					'\x5C'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투 서버에서 방이 삭제되었다는 메시지를 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_BATTLEROOM_FINISHED_RECV_OK			'\x0C'
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투 서버에 핑을 보내준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_BATTLE_PING							'\x0D'
#endif

