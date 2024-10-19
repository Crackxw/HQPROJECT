/*CURRENT_VERSION	20031222001*/
/*
 *	Version 20031222001
 *	 - DLG_ITEM_FOUND ���� DLG_ITEM_SEMI_FOUND �� �����Ǿ� ��µǴ� ���� ����.
 *
 *	Version 20031219001
 *	 - GetDLGitemTextA(void) �Լ��� ������� ��(type)����. BOOL -> INT
 *	 - 5���� ���¸� ��� ������ �ǵ��� ��.
 *		DLG_ITEM_NOT_GET_SSHOT		: ���̺귯�� ��� ���� 2
 *		DLG_ITEM_NOT_LOAD_MODULE	: ���̺귯�� ��� ���� 1
 *		DLG_ITEM_NOT_FOUND			: ��ũ�� �Ǵ� �ǽ��� ���� ��� ���� ����.
 *		DLG_ITEM_FOUND				: ��ϵ� ��ũ���� ���� �� ���� �Ϸ�.
 *		DLG_ITEM_SEMI_FOUND			: �ǽ��� ���� ��� ����
 *		DLG_ITEM_FOUND_TWO			: �ǽ��� ���� ��� ���� �� ��ϵ� ��ũ���� ���� �� ���� �Ϸ�.
 *
 *	Version	20031218001
 *	 - 20031217003.�������� Log���� ��½� ��� ���μ����� ��� �ϴ� �κ��� ���� ��ũ�η� �����Ǵ� ���μ����� ���� ��� �ǰ� ����.
 *	 - ���μ��� �޸𸮳��� �ε�� ��ũ�η� �ǽɵǴ� ����� ���Ź�� ����.
 *	 - ���� �׽�Ʈ�� ���̺귯�� ����.
 *
 *	Version 20031217003
 *	 - 20031217002.�������� Log������ ������ Ŀ���� ���� ����.
 *
 *	Version 20031217002
 *	 - ���� �޸𸮿��� ��ũ�η� �����Ǵ� ��� �߽߰� �α����Ϸ� ����ϴ� ��� �߰�.
 *	 - ���� �׽�Ʈ�� ���̺귯�� �߰�.
 *
 *	Version 20031217002 ���� ������
 *	 - Ư�� ���� ����.
 */

typedef enum	DLG_ITEM_TEXT
{
	DLG_ITEM_NOT_GET_SSHOT=-2,		//Couldn't Start This Module #2 /*���̺귯�� ��� ���� 2*/
	DLG_ITEM_NOT_LOAD_MODULE=-1,	//Couldn't Start This Module #1 /*���̺귯�� ��� ���� 1*/
	DLG_ITEM_NOT_FOUND=0,			//Must Return This 0!! <Normal state, Doubtfull or Macro stuffs does not exist in Memory./*��ũ�� �Ǵ� �ǽ��� ���� ��� ���� ����.*/
	DLG_ITEM_FOUND =1,				//Find Autoplay	/*��ϵ� ��ũ���� ���� �� ���� �Ϸ�.*/
	DLG_ITEM_SEMI_FOUND=2,			//Find Doubtfull Stuff in memory of Client /*�ǽ��� ���� ��� ����*/
	DLG_ITEM_FOUND_TWO = 3			//Find DLG_ITEM_FOUND and DLG_ITEM_SEMI_FOUND STATE /*�ǽ��� ���� ��� ���� �� ��ϵ� ��ũ���� ���� �� ���� �Ϸ�.*/
};
extern inline int GetDLGitemTextA(void);
