#ifndef	_SODEFINE_HEADER
#define	_SODEFINE_HEADER
//����������������������������������������������������������������������������������������
//	Server�� Define �Ǵ� ��
//����������������������������������������������������������������������������������������

// BONUS ��.
#define ON_BONUSMONEY_PER_DAY			300
#define ON_HEARTBEAT_INTERVAL			30000		// 30��
#define ON_HEARTBEAT_HIGH_LIMIT			150000		// 50�� (50�� �̻����� ������ �ڸ���.)
#define ON_HEARTBEAT_LOW_LIMIT			25000		// 20�� (20�� ���Ϸ� ������ �ڸ���.)
#define ON_HEARTBEAT_SAVE_NUM			5			// ��Ʈ ��Ʈ ���̺��� ����.

// ������ ����
#define ON_CAPITAL_RATE_BANK			0.001		//	���� �� �ܰ��� 0.1% (���� DB���� ó��)
#define ON_CAPITAL_RATE_TRADE			0.005		//	������� 0.5%, ���Ծ��� 0.5% (ó��)
#define ON_CAPITAL_RATE_EMPLOY			0.1			//	�뺴 ������� 10%(ó��), ���Ծ��� 10%(���� �� ��)
#define ON_CAPITAL_RATE_CURE			0.3			//	ġ�ᰡ���� 10% (ó��)
#define ON_CAPITAL_RATE_TICKET			0.3			//	�谪�� 10%(ó��)
#define ON_CAPITAL_RATE_INN				0.1			//	���Ŀ���� 10%(���� �� ��)
#define ON_CAPITAL_RATE_PLANT			0.01		//	����ǰ�� ���ذ����� 1%(ó��)

#define ON_TAX_PER_WORK                 10          //  �۾����� ����

#define ON_PARAM_SAVETIME				600000		//	Parameter�� �����ϴ� �ð�(10��)
//����������������������������������������������������������������������������������������
#endif	//_SODEFINE_HEADER

