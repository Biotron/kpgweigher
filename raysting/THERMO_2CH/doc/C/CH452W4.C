/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2005.4
 CH452��4�߽ӿ��ӳ���
****************************
 CH45X��4�߽ӿڣ����������ж�Ϊ3��I/O���ţ��������ж�Ϊ4��I/O������SPIʱ��
 �����Ʒ�ʽҲ���Բ���CH451�����ж�CH451��ѡ��4�ߴ��нӿڳ�ʼ���Ӻ�������ʡȥ
*************************************************************************** */
#include	"PIN.H"			// �޸ĸ��ļ�����ӦӲ������/��Ƭ���ͺŵ�
#include	"CH452CMD.H"	// CH452��������

 //������������ӳ���
 //����һ�޷������ͱ����洢12�ֽڵ������֡�
void CH452_Write(unsigned short cmd)
{
	unsigned char i;
	DISABLE_KEY_INTERRUPT;		  //��ֹ�����ж�,��ֹ��������б�CH452�ж϶������жϷ���������ٴδ���
	CH452_LOAD_CLR;                 //���ʼ,LOAD=0
	CH452_LOAD_D_OUT;		// ����LOADΪ�������
	CH452_DOUT_D_IN;		// ����DOUTΪ���뷽��
	CH452_DIN_D_OUT;		// ����DINΪ�������
	CH452_DCLK_D_OUT;		// ����DCLKΪ�������
	for(i=0;i!=12;i++)				//����12λ���ݣ���λ��ǰ
	{
		if (cmd&1) {CH452_DIN_SET;}
		else {CH452_DIN_CLR;}  // ���λ����
//		CH452_DIN=cmd&1;
    	CH452_DCLK_CLR;
    	cmd>>=1;
    	CH452_DCLK_SET;             //��������Ч
	}
  	CH452_LOAD_SET;         //��������,LOAD������
	DELAY_1US;				//�������ݼ������ڲ�С��6US,CH451����Ҫ������
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
  	ENABLE_KEY_INTERRUPT;
}

//*************************************************
//���밴�������ӳ��򣬴�CH452��ȡ
unsigned char CH452_Read( void )
{
	unsigned char i;
  	unsigned char cmd,keycode;		//���������֣������ݴ洢��
  	DISABLE_KEY_INTERRUPT;			               //��ֹ�����ж�,��ֹ��������б�CH452�ж϶������жϷ���������ٴδ���	
  	cmd=0x07;			               //��������������
  	CH452_LOAD_CLR;
  	for(i=0;i!=4;i++)  // ֻ��Ҫ������4λ,�෢Ҳ����,��Ӧ��ȷ��������µ�4λ�Ǹ�������
	{
		if (cmd&1) {CH452_DIN_SET;}
		else {CH452_DIN_CLR;}  // ���λ����
//		CH452_DIN=cmd&1;		      //�������λ
    	CH452_DCLK_CLR;			 
    	cmd>>=1;			      //������һλ
    	CH452_DCLK_SET;			      //����ʱ����������֪ͨCH451����λ����
 	}
  	CH452_LOAD_SET;				      //��������������֪ͨCH451������������
  	keycode=0;				      //���keycode
  	for(i=0;i!=7;i++)
	{
		keycode<<=1;			      //��������keycode,��λ��ǰ,��λ�ں�
		if (CH452_DOUT) keycode++;      //�Ӹߵ��Ͷ���451������
//    	keycode|=CH452_DOUT;
    	CH452_DCLK_CLR;			      //����ʱ��������֪ͨCH451�����һλ
    	CH452_DCLK_SET;
 	}
  	CLEAR_KEY_INTER_FLAG;	     //���жϱ�־
  	ENABLE_KEY_INTERRUPT;
  	return(keycode);			     //���ؼ�ֵ
}