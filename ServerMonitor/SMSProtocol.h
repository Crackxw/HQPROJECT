

/////////////////////////////////////////////////////////////////

#ifndef __SMS_SERVER_PROTOCOL_H_INCLUDED__
#define __SMS_SERVER_PROTOCOL_H_INCLUDED__

/////////////////////////////////////////////////////////////////

#define		SMS_SERVER_PORT				21000


#define		SMS_SERVER_RECV_BUF_SIZE	30000
#define		SMS_SERVER_SEND_BUF_SIZE	30000
#define		SMS_SERVER_QUEUE_SIZE		30000



/////////////////////////////////////////////////////////////////
// Message Type
//
#define				DELIVER_REQ_MSG		0
#define				DELIVER_RESP_MSG	11


#define				SS_SMS_REQ_MSG		20000
#define				SS_SMS_RESP_MSG		21000

/////////////////////////////////////////////////////////////////
// Message Structure
//
struct SMS_REQ_MSG
{
	DWORD	dwCallCount;		// 위즈정보기술(주)측 일련번호
	char	szUserCode[32];		// 사용자ID : JOYON
	char	szOrgPhone[32];		// 발신가입자번호 '0161112222'
	char	szDstPhone[32];		// 착신가입자번호 (특번)
	char	szCallMessage[200];	// 호출 메시지
	char    szRecvTime[16];		// "YYYYMMDDHHMMSS"
	char	szReserved[22];		// 여유...
};


struct SMS_RESP_MSG
{
	DWORD	dwCallCount;		// 위즈정보기술(주)측 일련번호
	char	szUserCode[32];		// 사용자 ID : JOYON
	LONG	lResult;			// 0:성공, -99:오류
};

/////////////////////////////////////////////////////////////////


#endif // __SMS_SERVER_PROTOCOL_H_INCLUDED__

/////////////////////////////////////////////////////////////////
