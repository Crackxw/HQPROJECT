//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 컨트롤 서버, 게이트 서버, 게임 서버 에서의 패킷 커맨드 정의
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 9
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XPACKETCMD_H
#define _XPACKETCMD_H

#define	JXCMD_REQUEST_PLAYERACCOUNT					0x0101
#define JXCMD_REPORT_PLAYERACCOUNT					0x0102

#define JXCMD_LOGOUT_PLAYER							0x0103
#define JXCMD_LOGOUT_PLAYER_BYLOCALID				0x0104

#define	JXCMD_SENDPACKET_TO_PLAYER					0x0105		// 특정 플레이어에게 패킷 전달 
#define JXCMD_SENDPACKET_TO_PLAYERGROUP				0x0106		// 여러 플레이어들에게 패킷 전달 

#define JXCMD_SENDPACKET_TO_GAMESERVER				0x0107		// 게임 서버로 패킷 전달 

#define JXCMD_SENDPACKET_TO_PLAYERALL				0x0108		// 전체 플레이어들에게 패킷 전달 

#endif
