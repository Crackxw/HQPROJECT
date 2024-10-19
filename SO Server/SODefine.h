#ifndef	_SODEFINE_HEADER
#define	_SODEFINE_HEADER
//────────────────────────────────────────────
//	Server만 Define 되는 것
//────────────────────────────────────────────

// BONUS 돈.
#define ON_BONUSMONEY_PER_DAY			300
#define ON_HEARTBEAT_INTERVAL			30000		// 30초
#define ON_HEARTBEAT_HIGH_LIMIT			150000		// 50초 (50초 이상으로 나오면 자른다.)
#define ON_HEARTBEAT_LOW_LIMIT			25000		// 20초 (20초 이하로 나오면 자른다.)
#define ON_HEARTBEAT_SAVE_NUM			5			// 하트 비트 세이브한 개수.

// 마을의 수익
#define ON_CAPITAL_RATE_BANK			0.001		//	전장 총 잔고의 0.1% (현재 DB에서 처리)
#define ON_CAPITAL_RATE_TRADE			0.005		//	매출액의 0.5%, 매입액의 0.5% (처리)
#define ON_CAPITAL_RATE_EMPLOY			0.1			//	용병 매출액의 10%(처리), 매입액의 10%(아직 안 됨)
#define ON_CAPITAL_RATE_CURE			0.3			//	치료가격의 10% (처리)
#define ON_CAPITAL_RATE_TICKET			0.3			//	배값의 10%(처리)
#define ON_CAPITAL_RATE_INN				0.1			//	숙식요금의 10%(아직 안 됨)
#define ON_CAPITAL_RATE_PLANT			0.01		//	생산품의 기준가격의 1%(처리)

#define ON_TAX_PER_WORK                 10          //  작업량당 세금

#define ON_PARAM_SAVETIME				600000		//	Parameter를 저장하는 시간(10분)
//────────────────────────────────────────────
#endif	//_SODEFINE_HEADER

