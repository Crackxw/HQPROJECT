//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	임진록 온라인 [거상] - 국가별 정보 데이터베이스 파서기
//	File Name		:	ReadIP.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	거 상 팀
//	Editer			:	조 현 준
//	Copyright		:	Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		HQ 내의 국가별 정보 데이터베이스 파일(ServiceInfo.cfg)에서 
//		ntn.con에 설정된 플래그대로 골라낼 때 쓰는 함수입니다.
//
//===================================================================================================

#include <GSL.h>
#include "ReadIP.h"

SConnectServerInfo	g_stConnectServerInfo = {0,};
