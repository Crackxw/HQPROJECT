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
// ��û			
//				
// REQUEST_LOGIN : ���� ������ȣ( 4byte ) + ���� ���̵�( str )						
//	
// REQUEST_MOVECELL : ���ڵ�( 2byte ) + �� x��ǥ( 2byte ) + �� y��ǥ( 2byte )		
//
// REQUEST_OUTWORLD : NODATA					
//		
// REQUEST_CHATMESSAGE : ä�ù��ڿ�( str )		
//
// REQUEST_WHISPER : ���� ���̵�( str ) + ä�ù��ڿ�( str )							
//
// REQUEST_JOINROOM : �� ������ȣ( 4byte )
//
// REQUEST_EXITROOM : NODATA
//
// REQUEST_CHATMESSAGE_ROOM : ä�ù��ڿ�( str )
//
// ����
//
// REPORT_LOGIN : ���( 2 )		- ��� : 0 ����,  1 ����								
//
// REPORT_WHISPER : ���( 2 ) + ���� ���̵�( str ) + ä�ù��ڿ�( str )	   - ��� : 0 ����,  1 ����
//
// 
// CHATMESSAGE : ���� ������ȣ( 4byte ) + ���� ���̵�( str ) + ä�ù��ڿ�( str )	
//
// WHISPER : ���� ���̵�( str ) + ä�ù��ڿ�( str )									
//
// CHATMESSAGE_ROOM : ���� ������ȣ( 4byte ) + ���� ���̵�( str ) + ä�ù��ڿ�( str )
//

#endif
