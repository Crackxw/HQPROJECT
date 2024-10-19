#ifndef _PACKETCMD_H	
#define _PACKETCMD_H	

#define HEARTBEAT					0x0001	

#define REQUEST_LOGIN				0x0101	
#define REQUEST_MOVECELL			0x0102	
#define REQUEST_OUTWORLD			0x0112  
#define REQUEST_CHATMESSAGE			0x0103	
#define REQUEST_WHISPER				0x0104	

#define REQUEST_JOINROOM			0x0201  
#define REQUEST_EXITROOM			0x0202  
#define REQUEST_CHATMESSAGE_ROOM	0x0205  
	
#define REPORT_LOGIN				0x1001	
#define REPORT_WHISPER				0x1002	

#define CHATMESSAGE					0x2001	
#define WHISPER						0x2002	

#define CHATMESSAGE_ROOM			0x2005  

//				
// 요청			
//				
// REQUEST_LOGIN : 유저 고유번호( 4byte ) + 유저 아이디( str )						
//	
// REQUEST_MOVECELL : 맵코드( 2byte ) + 셀 x좌표( 2byte ) + 셀 y좌표( 2byte )		
//
// REQUEST_OUTWORLD : NODATA					
//		
// REQUEST_CHATMESSAGE : 채팅문자열( str )		
//
// REQUEST_WHISPER : 유저 아이디( str ) + 채팅문자열( str )							
//
// REQUEST_JOINROOM : 방 고유번호( 4byte )
//
// REQUEST_EXITROOM : NODATA
//
// REQUEST_CHATMESSAGE_ROOM : 채팅문자열( str )
//
// 응답
//
// REPORT_LOGIN : 결과( 2 )		- 결과 : 0 성공,  1 실패								
//
// REPORT_WHISPER : 결과( 2 ) + 유저 아이디( str ) + 채팅문자열( str )	   - 결과 : 0 성공,  1 실패
//
// 
// CHATMESSAGE : 유저 고유번호( 4byte ) + 유저 아이디( str ) + 채팅문자열( str )	
//
// WHISPER : 유저 아이디( str ) + 채팅문자열( str )									
//
// CHATMESSAGE_ROOM : 유저 고유번호( 4byte ) + 유저 아이디( str ) + 채팅문자열( str )
//

#endif
