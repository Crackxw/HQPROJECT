#ifndef _INCLUDE_LOGINLIB_LIB____
#define _INCLUDE_LOGINLIB_LIB____

int Joy_RAS_Function(
		int ServiceSN		// ���� �ڵ�

		,char ProcessSN		// ó�� Ÿ�� �޴��� ����
							// ����Ǵ� ���ӿ� ���� Ÿ���� �޶��� �� �ִ�.


		,int Usernumber		// ���̿� ����������ȣ (�Ⱦ��� ��� -1 ������)

		,char *Userid		// ���̿� ���̵�
		,int Useridsize		// ���̿� ���̵��� ����(NULL���� ����)
		,char *Rascode		// �̰���. �н�����ų�, ������ �Ѱ��� ���� �ڵ�.
		,int Rascodesize	// �� ��Ʈ���� ����(NULL���� ����)
		,char *Userip		// ���� ������ Ŭ���̾�Ʈ�� ������ �ּ�. - ��) "10.1.1.127" .
		,int Useripsize		// �� ��Ʈ���� ����(NULL���� ����)

		,char *Rasserverip	// RAS ���� �� ���� ������ �ּ�
		,int Rasport		// RAS ���� �� ���� ��Ʈ ��ȣ
		,char *RasMsg		// ������� �� char* �� ���� ������
							// ����Ǵ� ���ӿ� ���� ������ ����� ������ �� �ִ�.
		,int RasMsgmaxsize	// RasMsg �� �ѱ� ������ �ִ� ������(NULL���� ����)..
							// ���� ���� �����Ͱ� �� ���۸� �ѱ�� ������ �����Ѵ�.
							// �ּ� 256 BYTE �̻��̾�� �Ѵ�.
		);

// ���� ���� �޴��� State ����.

#endif