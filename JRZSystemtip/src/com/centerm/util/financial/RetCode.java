package com.centerm.util.financial;

public class RetCode {
	/*
	 * �������������ܳ��ֵĴ���DRV+7λ����
	 * ������	           ��������
	 * DRV0000001	δ֪����
	 * DRV0000002	��ʱ����
	 * DRV0000003	�򿪴���ʧ��
	 * DRV0000004	�򴮿ڷ�������ʧ��
	 * DRV0000005	�ӿڲ�������
	 * DRV0000006	�Ҳ�����̬���ӿ�
	 * DRV0000007	��̬���ӿ���ش���
	 * DRV0000008	ͨѶ����û�н���
	 */
	public final static String UNKNOWN_ERR = "DRV0000001";
	public final static String TIMEOUT_ERR = "DRV0000002";
	public final static String OPEN_SERIAL_ERR = "DRV0000003";
	public final static String SEND_MSG_ERR = "DRV0000004";
	public final static String PARAM_ERR = "DRV0000005";
	public final static String NOT_FIND_SO_Err = "DRV0000006";
	public final static String LOAD_SO_ERR = "DRV0000007";
	public final static String RECV_MSG_ERR = "DRV0000008";
	
	public final static String UNKNOWN_ERR_Msg = "δ֪����";
	public final static String TIMEOUT_ERR_MSG = "��ʱ����";
	public final static String OPEN_SERIAL_ERR_MSG = "�򿪴���ʧ��";
	public final static String SEND_MSG_ERR_MSG = "��������ʧ��";
	public final static String PARAM_ERR_MSG = "�ӿڲ�������";
	public final static String NOT_FIND_SO_Err_MSG = "�Ҳ�����̬���ӿ�";
	public final static String LOAD_SO_ERR_MSG = "��̬���ӿ���ش���";
	public final static String RECV_MSG_ERR_MSG = "��������ʧ��";
	
	/*
	 * ĳ���������ֵĴ���3λ�豸���ʹ���+7λ����
	 * �豸���ʹ���	�豸����
	 * PIN	                         �������
     * ICC	        IC��
     * IDC	                        ����֤��ID����
     * FIG			ָ����
	 * RDC			ˢ����
	 * SIN          ����ǩ��
	 */
	//ָ����
	public static final int FIG_ERR_OPENFAILED = -1;//���豸ʧ��
	public static final int FIG_ERR_WRITE =	-2;//����ָ��ʧ��
	public static final int FIG_ERR_FAIL = -3;//��ȡʧ��
	public static final int FIG_ERR_TIMEOUT = -8;//��ʱ
	
	public final static String FIG_FAIL_ERR = "RDC0000001";
	public final static String FIG_FAIL_ERR_MSG = "��ȡʧ��";
	
	//�ſ�
	public final static String RDC_PACKAGE_FORMAT_ERR = "RDC0000001";//���յ��ı��ĸ�ʽ����
	public final static String RDC_READ_CARD_ERR = "RDC0000002"; //��������ʧ��
	public final static String RDC_WRITE_CARD_ERR = "RDC0000003";//д������ʧ��
	public final static String RDC_CANCELED  = "RDC0000004";	 //�û�ȡ��
	public final static String RDC_PACKAGE_FORMAT_ERR_MSG = "���յ��ı��ĸ�ʽ����";
	public final static String RDC_READ_CARD_ERR_MSG = "��������ʧ��";
	public final static String RDC_WRITE_CARD_ERR_MSG = "д������ʧ��";
	public final static String RDC_CANCELED_MSG  = "�û�ȡ��";
	
	public final static int RDC_ERR_OPENFAILED = -1;//���豸ʧ��
	public final static int RDC_ERR_WRITE =	-2;//����ָ��ʧ��
	public final static int RDC_ERR_READ = -3;//��������ʧ��
	public final static int RDC_ERR_PACKAGE_FORMAT = -4;//���յ��ı��ĸ�ʽ����
	public final static int RDC_ERR_READ_CARD = -5;//��������ʧ��
	public final static int RDC_ERR_WRITE_CARD = -6;//д������ʧ��
	public final static int RDC_ERR_CANCELED = -7;//�û�ȡ��
	public final static int RDC_ERR_TIMEOUT = -8;//��ʱ
	public final static int RDC_ERR_OTHER = -100;//��������
	
	//����֤
	public final static String IDC_CANCELED  = "IDC0000001";	 //�û�ȡ��
	public final static String IDC_CHECK_ERR  = "IDC0000002";	 //У�����
	public final static String IDC_CMD_ERR  = "IDC0000003";	  
	public final static String IDC_CANCELED_MSG  = "�û�ȡ��";	 
	public final static String IDC_CHECK_ERR_MSG  = "У�����";	 
	public final static String IDC_CMD_ERR_MSG  = "ָ�����";	     
	
	public final static int IDC_ERR_ERROR  = -1;  
	public final static int IDC_ERR_PARAM  = -2;  
	public final static int IDC_ERR_TIMEOUT	= -3;   
	public final static int IDC_ERR_OPEN   = -4;  
	public final static int IDC_ERR_WRITE  = -5;  
	public final static int IDC_ERR_READ   = -6;   
	public final static int IDC_ERR_CANCEL = -7;   
	public final static int IDC_ERR_CHECK  = -8;
	public final static int IDC_ERR_CMD	   = -9;
	
	//����ǩ��
	public final static String SIN_CANCELED  = "SIN0000001";	 //�û�ȡ��
	public final static String SIN_ENCRY_FAIL  = "SIN0000002";	 //����ʧ��
	public final static String SIN_CANCELED_MSG  = "�û�ȡ��";	 //�û�ȡ��
	public final static String SIN_ENCRY_FAIL_MSG  = "����ʧ��";	 //����ʧ��
	
	//IC
	public final static String ICC_CLOSE_PORT_FAIL = "ICC0000001";//�رմ���ʧ��
	public final static String ICC_EX_PORT_FAIL = "ICC000002"; //����ת��ʧ��
	public final static String ICC_NO_CARD_FAIL = "ICC000003"; //����ת��ʧ��
	
	public final static String ICC_CLOSE_PORT_FAIL_MSG = "�رմ���ʧ��";//
	public final static String ICC_EX_PORT_FAIL_MSG = "����ת��ʧ��"; //
	public final static String ICC_NO_CARD_FAIL_MSG = "����ת��ʧ��"; //
	
	//-101---108
	public final static String ICC_POWER_ON_FAIL  = "ICC0000101";	 //�ϵ�ʧ��
	public final static String  ICC_POWER_OFF_FAIL  = "ICC0000102";//�µ�ʧ��  
	public final static String  ICC_TOUCH_CARD_OUT  = "ICC0000103";//�Ӵ�ʽIC������λ
	public final static String  ICC_UNTOUCH_CARD_OUT  = "ICC0000104";//�ǽӴ�ʽIC������λ
	public final static String  ICC_UNSUPT_TOUCH_CARD  = "ICC0000105";//��֧�ֽӴ�ʽIC��
	public final static String  ICC_TOUCH_CARD_ON  = "ICC0000106";//�Ӵ�ʽ�û����Ѿ��ϵ�
	public final static String  ICC_UNTOUCH_OFF  = "ICC0000107";//�ǽӴ�ʽ��δ����
	public final static String  ICC_UNTOUCH_ON_FAIL  = "ICC0000108";//�ǽӴ�ʽ������ʧ��
	
	public final static String ICC_POWER_ON_FAIL_MSG  = "�ϵ�ʧ��";	 //
	public final static String  ICC_POWER_OFF_FAIL_MSG  = "�µ�ʧ��";//  
	public final static String  ICC_TOUCH_CARD_OUT_MSG  = "�Ӵ�ʽIC������λ";//
	public final static String  ICC_UNTOUCH_CARD_OUT_MSG  = "�ǽӴ�ʽIC������λ";//
	public final static String  ICC_UNSUPT_TOUCH_CARD_MSG  = "��֧�ֽӴ�ʽIC��";//
	public final static String  ICC_TOUCH_CARD_ON_MSG  = "�Ӵ�ʽ�û����Ѿ��ϵ�";//
	public final static String  ICC_UNTOUCH_OFF_MSG  = "�ǽӴ�ʽ��δ����";//
	public final static String  ICC_UNTOUCH_ON_FAIL_MSG  = "�ǽӴ�ʽ������ʧ��";//
	
	//-201--216
	public final static String  ICC_GET_APP_FAIL  = "ICC00000201";//ѡ��Ӧ�û���ʧ��      
	public final static String  ICC_SELECT_APP_FAIL = "ICC0000202";//ѡ��Ӧ��ʧ��          
	public final static String  ICC_INIT_FAIL  = "ICC0000203";//Ӧ�ó�ʼ��ʧ��
	public final static String ICC_GEN_ARQC_FAIL  = "ICC0000204";	 //��ȡARQCʧ��
	public final static String ICC_FAIL_EXT_AUTH  = "ICC0000205";	 //�ⲿ��֤����   
	public final static String ICC_FAIL_EXEC_SRPT  = "ICC0000206";	 //ִ�нű�����  
	public final static String ICC_NOLOGENTER_ERR  = "ICC0000207";	 //û����־���
	public final static String ICC_CARD_LOCK  = "ICC0000208";	     //��Ƭ������
	public final static String ICC_CARD_LOCK_FOREVER  = "ICC0000209";//��Ƭ������
	public final static String ICC_PARASE_GAC_ERR  = "ICC0000210";	 //����GAC����
	public final static String ICC_GET_CDOL1_FAIL  = "ICC0000211";	 //��ȡCDOL1����ʧ��
	public final static String ICC_GET_CDOL2_FAIL  = "ICC0000212";	 //��ȡCDOL2����ʧ��
	public final static String ICC_GEN_PDOL_FAIL  = "ICC0000213";	 //����PDOL����ʧ��
	public final static String ICC_AIDLIST_LOCK  = "ICC0000214";	 //ָ�����
	public final static String  ICC_AIDLIST_SEL_FAIL  = "ICC0000215";//Ӧ���б�ѡ��ʧ��
	public final static String  ICC_GEN_TC_FAIL  = "ICC0000216";//����TCʧ��
	
	public final static String  ICC_GET_APP_FAIL_MSG  = "ѡ��Ӧ�û���ʧ�� ";//     
	public final static String  ICC_SELECT_APP_FAIL_MSG = "ѡ��Ӧ��ʧ��";//          
	public final static String  ICC_INIT_FAIL_MSG  = "Ӧ�ó�ʼ��ʧ��";//
	public final static String ICC_GEN_ARQC_FAIL_MSG  = "��ȡARQCʧ��";	 //
	public final static String ICC_FAIL_EXT_AUTH_MSG  = "�ⲿ��֤���� ";	 //  
	public final static String ICC_FAIL_EXEC_SRPT_MSG  = "ִ�нű�����  ";	 //
	public final static String ICC_NOLOGENTER_ERR_MSG  = "û����־���";	 //
	public final static String ICC_CARD_LOCK_MSG  = "��Ƭ������";	     //
	public final static String ICC_CARD_LOCK_FOREVER_MSG  = "��Ƭ������";//
	public final static String ICC_PARASE_GAC_ERR_MSG  = "����GAC����";	 //
	public final static String ICC_GET_CDOL1_FAIL_MSG  = "��ȡCDOL1����ʧ��";	 //
	public final static String ICC_GET_CDOL2_FAIL_MSG  = "��ȡCDOL2����ʧ��";	 //
	public final static String ICC_GEN_PDOL_FAIL_MSG  = "����PDOL����ʧ��";	 //
	public final static String ICC_AIDLIST_LOCK_MSG  = "ָ�����";	 //
	public final static String  ICC_AIDLIST_SEL_FAIL_MSG  = "Ӧ���б�ѡ��ʧ��";//
	public final static String  ICC_GEN_TC_FAIL_MSG  = "����TCʧ��";//
	
	//-301--307
	public final static String  ICC_PARSE_FAIL  = "ICC0000301";//���ݽ���tlvʧ��
	public final static String  ICC_GENLOG_FAIL  = "ICC0000302";//������־�ļ�ʧ��
	public final static String  ICC_SEND_RR_FAIL  = "ICC0000303";//'ReadRecord'���������Ƭʧ��
	public final static String  ICC_CMD_RR_FAIL  = "ICC0000304";//'ReadRecord'����ִ��ʧ��
	public final static String  ICC_SEND_IcGenArqc_FAIL  = "ICC0000305";//'GenArqc'���������Ƭʧ��
	public final static String  ICC_CMD_IcGenArqc_FAIL  = "ICC0000306";//'GenArqc'����ִ��ʧ��
	public final static String  ICC_SEND_ICExecute_FAIL  = "ICC0000307";//'�ű�ִ��'���������Ƭʧ��
	
	public final static String  ICC_PARSE_FAIL_MSG  = "���ݽ���tlvʧ��";//
	public final static String  ICC_GENLOG_FAIL_MSG  = "������־�ļ�ʧ��";//
	public final static String  ICC_SEND_RR_FAIL_MSG  = "ReadRecord���������Ƭʧ��";//
	public final static String  ICC_CMD_RR_FAIL_MSG  = "ReadRecord����ִ��ʧ��";//
	public final static String  ICC_SEND_IcGenArqc_FAIL_MSG  = "GenArqc���������Ƭʧ��";//
	public final static String  ICC_CMD_IcGenArqc_FAIL_MSG  = "GenArqc����ִ��ʧ��";//
	public final static String  ICC_SEND_ICExecute_FAIL_MSG  = "�ű�ִ�з��������Ƭʧ��";//
	
	//-308---316
	public final static String ICC_SCRIPT_FAIL             = "ICC0000308"; //'�ű�ִ��'����ִ��ʧ��
	public final static String ICC_SEND_ICIssAuth_FAIL     = "ICC0000309"; //'��������֤'���������Ƭʧ��
	public final static String ICC_CMD_ICIssAuth_FAIL      = "ICC0000310"; //'��������֤'����ִ��ʧ��
	public final static String ICC_SEND_ICGenTc_FAIL       = "ICC0000311"; //'GenTc'���������Ƭʧ��
	public final static String ICC_CMD_ICGenTc_FAIL        = "ICC0000312"; //'GenTc'ִ������ʧ��
	public final static String ICC_SEND_ICGenAAC_FAIL      = "ICC0000313"; //'GenAAC'���������Ƭʧ��
	public final static String ICC_CMD_ICGenAAC_FAIL       = "ICC0000314"; //'GenAAC'ִ������ʧ��
	public final static String  ICC_SEND_ICGpo_FAIL  = "ICC0000315";//'GPOָ��'���������Ƭʧ��
	public final static String  ICC_CMD_ICGpo_FAIL  = "ICC0000316";//'GPOָ��'ִ������ʧ��
	
	public final static String ICC_SCRIPT_FAIL_MSG             = "�ű�ִ������ִ��ʧ��"; //
	public final static String ICC_SEND_ICIssAuth_FAIL_MSG     = "��������֤���������Ƭʧ��"; //
	public final static String ICC_CMD_ICIssAuth_FAIL_MSG      = "��������֤����ִ��ʧ��"; //
	public final static String ICC_SEND_ICGenTc_FAIL_MSG       = "GenTc���������Ƭʧ��"; //
	public final static String ICC_CMD_ICGenTc_FAIL_MSG        = "GenTcִ������ʧ��"; //
	public final static String ICC_SEND_ICGenAAC_FAIL_MSG      = "GenAAC���������Ƭʧ��"; //
	public final static String ICC_CMD_ICGenAAC_FAIL_MSG       = "GenAACִ������ʧ��"; //
	public final static String  ICC_SEND_ICGpo_FAIL_MSG  = "GPOָ��������Ƭʧ��";//
	public final static String  ICC_CMD_ICGpo_FAIL_MSG  = "GPOָ��ִ������ʧ��";//
	
	//-317--321
	public final static String  ICC_SEND_ICSelect_FAIL  = "ICC0000317";//'SLEECT'���������Ƭʧ��
	public final static String  ICC_CMD_ICSelect_FAIL  = "ICC0000318";//'SLEECT'ִ������ʧ��
	public final static String  ICC_SEND_Getdata_FAIL  = "ICC0000319";//'Getdata'���������Ƭʧ��
	public final static String  ICC_CMD_Getdata_FAIL  = "ICC0000320";//'Getdata'ִ������ʧ��
	public final static String  ICC_SEND_APDU_FAIL  = "ICC0000321";// SEND_APDUִ������ʧ��
	
	public final static String  ICC_SEND_ICSelect_FAIL_MSG  = "SLEECT���������Ƭʧ��";//
	public final static String  ICC_CMD_ICSelect_FAIL_MSG  = "SLEECTִ������ʧ��";//
	public final static String  ICC_SEND_Getdata_FAIL_MSG  = "Getdata���������Ƭʧ��";//
	public final static String  ICC_CMD_Getdata_FAIL_MSG  = "Getdataִ������ʧ��";//
	public final static String  ICC_SEND_APDU_FAIL_MSG  = "SEND_APDUִ������ʧ��";//
	
	//���󷵻�ֵ
	public final static int ICC_ERR_PARAM_ERR               = -1; //��������            
	public final static int ICC_ERR_OPEN_PORT_FAIL          = -2; //�򿪴��ڣ�U�ڣ�ʧ�� 
	public final static int ICC_ERR_TIMEOUT_ERR             = -3; //������ʱ            
	public final static int ICC_ERR_CLOSE_PORT_FAIL         = -4; //�رմ���ʧ��
	public final static int ICC_ERR_EX_PORT_FAIL            = -5; //����ת��ʧ��
	public final static int ICC_ERR_NO_CARD_FAIL            = -11; //�޿�

	 //�ϵ�ʧ��          
	public final static int ICC_ERR_OTHER_ERR               = -100; //δ֪        
	public final static int ICC_ERR_POWER_ON_FAIL           = -101;
	public final static int ICC_ERR_POWER_OFF_FAIL          = -102; //�µ�ʧ��  
	public final static int ICC_ERR_TOUCH_CARD_OUT          = -103; //�Ӵ�ʽIC������λ
	public final static int ICC_ERR_UNTOUCH_CARD_OUT        = -104; //�ǽӴ�ʽIC������λ
	public final static int ICC_ERR_UNSUPT_TOUCH_CARD       = -105; //��֧�ֽӴ�ʽIC��
	public final static int ICC_ERR_TOUCH_CARD_ON           = -106; //�Ӵ�ʽ�û����Ѿ��ϵ�
	public final static int ICC_ERR_UNTOUCH_OFF             = -107; //�ǽӴ�ʽ��δ����
	public final static int ICC_ERR_UNTOUCH_ON_FAIL         = -108; //�ǽӴ�ʽ������ʧ��

	public final static int ICC_ERR_GET_APP_FAIL            = -201; //ѡ��Ӧ�û���ʧ��      
	public final static int ICC_ERR_SELECT_APP_FAIL         = -202; //ѡ��Ӧ��ʧ��          
	public final static int ICC_ERR_INIT_FAIL               = -203; //Ӧ�ó�ʼ��ʧ��  
	public final static int ICC_ERR_GEN_ARQC_FAIL           = -204; //��ȡARQCʧ��      
	public final static int ICC_ERR_FAIL_EXT_AUTH           = -205; //�ⲿ��֤����          
	public final static int ICC_ERR_FAIL_EXEC_SRPT          = -206; //ִ�нű�����          
	public final static int ICC_ERR_NOLOGENTER_ERR          = -207; //û����־���
	public final static int ICC_ERR_CARD_LOCK               = -208; //��Ƭ������
	public final static int ICC_ERR_CARD_LOCK_FOREVER       = -209; //��Ƭ������
	public final static int ICC_ERR_PARASE_GAC_ERR          = -210; //����GAC����
	public final static int ICC_ERR_GET_CDOL1_FAIL          = -211; //��ȡCDOL1����ʧ��
	public final static int ICC_ERR_GET_CDOL2_FAIL          = -212; //��ȡCDOL2����ʧ��
	public final static int ICC_ERR_GEN_PDOL_FAIL           = -213; //����PDOL����ʧ��
	public final static int ICC_ERR_AIDLIST_LOCK            = -214; //Ӧ������
	public final static int ICC_ERR_AIDLIST_SEL_FAIL        = -215; //Ӧ���б�ѡ��ʧ��
	public final static int ICC_ERR_GEN_TC_FAIL             = -216; //����TCʧ��

	public final static int ICC_ERR_PARSE_FAIL              = -301; //���ݽ���tlvʧ��
	public final static int ICC_ERR_GENLOG_FAIL             = -302; //������־�ļ�ʧ��
	public final static int ICC_ERR_SEND_RR_FAIL            = -303; //'ReadRecord'���������Ƭʧ��
	public final static int ICC_ERR_CMD_RR_FAIL             = -304; //'ReadRecord'����ִ��ʧ��
	public final static int ICC_ERR_SEND_IcGenArqc_FAIL     = -305; //'GenArqc'���������Ƭʧ��
	public final static int ICC_ERR_CMD_IcGenArqc_FAIL      = -306; //'GenArqc'����ִ��ʧ��
	public final static int ICC_ERR_SEND_ICExecute_FAIL     = -307; //'�ű�ִ��'���������Ƭʧ��
	
	public final static int ICC_ERR_SCRIPT_FAIL             = -308; //'�ű�ִ��'����ִ��ʧ��
	public final static int ICC_ERR_SEND_ICIssAuth_FAIL     = -309; //'��������֤'���������Ƭʧ��
	public final static int ICC_ERR_CMD_ICIssAuth_FAIL      = -310; //'��������֤'����ִ��ʧ��
	public final static int ICC_ERR_SEND_ICGenTc_FAIL       = -311; //'GenTc'���������Ƭʧ��
	public final static int ICC_ERR_CMD_ICGenTc_FAIL        = -312; //'GenTc'ִ������ʧ��
	public final static int ICC_ERR_SEND_ICGenAAC_FAIL      = -313; //'GenAAC'���������Ƭʧ��
	public final static int ICC_ERR_CMD_ICGenAAC_FAIL       = -314; //'GenAAC'ִ������ʧ��
	public final static int ICC_ERR_SEND_ICGpo_FAIL         = -315; //'GPOָ��'���������Ƭʧ��
	public final static int ICC_ERR_CMD_ICGpo_FAIL          = -316; //'GPOָ��'ִ������ʧ��
	public final static int ICC_ERR_SEND_ICSelect_FAIL      = -317; //'SLEECT'���������Ƭʧ��
	public final static int ICC_ERR_CMD_ICSelect_FAIL       = -318; //'SLEECT'ִ������ʧ��
	public final static int ICC_ERR_SEND_Getdata_FAIL       = -319; //'Getdata'���������Ƭʧ��
	public final static int ICC_ERR_CMD_Getdata_FAIL        = -320; //'Getdata'ִ������ʧ��
	public final static int ICC_ERR_SEND_APDU_FAIL          = -321; // SEND_APDUִ������ʧ��
}
