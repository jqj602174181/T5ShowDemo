package com.centerm.util.financial;

public class MesDefUtil {
	public static final int DEF_TIMEOUT = 5;		//Ĭ�ϳ�ʱʱ�䣬��λs
	public static final int MSG_HEAD_LEN = 8;		//����ͷ����
	public static final int MSG_SIZE_LEN = 4;		//���ĳ���
	public static final int MSG_TYPE_LEN = 2;		//�������ͳ���
	public static final int MSG_CHECK_LEN = 1;		//����У���볤��

	public static final int MSG_BODY_MAX = 4096;	//��������󳤶�

	/* ����SO��ִ����� */
	public static final int READ_SUCCESS = 0;
	public static final int READ_FAIL = -1;
	public static final int READ_TIMEOUT = -2;

	public static final String TRANSFERTYPE_HID_STRING = "UID";
	public static final String TRANSFERTYPE_BT_STRING = "BLUETOOTH";
	public static final String TRANSFERTYPE_BLE_STRING = "BLE";

	//�ſ������ۻ�IC���ŵ����˿ں��Լ�������
	public static final int MAGNETICCARDPORT = 2;
	public static final int MAGNETICCARDBAUD = 9600;

	//ID���˿ڲ�����
	public static final int IDCARDPORT = 2;
	public static final int IDCARDBAUD = 9600;

	//IC���˿������Լ��˿ں�
	public static final String ICCARDPORTTYPE = "C";//�˿�����(C-����, U-USB)
	public static final String ICCARDPORTNO = "1";//�˿ں�

	/* ������������ӿ� */
	public final static int readPin = 1001;
	public final static int readPinSuccess = 1101;
	public final static int readPinFail = 1102;

	public final static int keyAffusePin  = 1002;
	public final static int keyAffusePinSuccess  = 1201;
	public final static int keyAffusePinFail  = 1202;

	public final static int keyFlagPin  = 1003;
	public final static int keyFlagPinSuccess  = 1301;
	public final static int keyFlagPinFail  = 1302;
	public final static int keyM2ForM4 = 1303;

	public final static int newKeyPair  = 1004;
	public final static int newKeyPairSuccess  = 1401;
	public final static int newKeyPairFail  = 1402;

	public final static int decodeKey  = 1005;
	public final static int decodeKeySuccess  = 1501;
	public final static int decodeKeyFail  = 1502;
	public final static int decodeBlockPin = 1503;

	/* ������������ӿ� �������� */
	public final static int saveSm2 = 1006;
	public final static int saveSm2Success = 1601;
	public final static int saveSm2Fail = 1602;

	public final static int saveSm2_1 = 1011;
	public final static int saveSm2_1Success = 1701;
	public final static int saveSm2_1Fail = 1702;

	public final static int saveSm2_2 = 1012;
	public final static int saveSm2_2Success = 1801;
	public final static int saveSm2_2Fail = 1802;

	public final static int getSN = 1007; //��ȡT5���к�
	public final static int getSNSuccess = 1701;
	public final static int getSNFail = 1702;

	public final static int saveMKAndMKName = 1008; //�·�����Կ������Կ����
	public final static int saveMKAndMKNameSuccess = 1801;
	public final static int saveMKAndMKNameFail = 1802;

	public final static int getMKName = 1009; //��ȡ����Կ����
	public final static int getMKNameSuccess = 1901;
	public final static int getMKNameFail = 1902;
	
	public final static int saveWorkKey = 1010; //���湤����Կ
	public final static int saveWorkKeySuccess = 1108; 
	public final static int saveWorkKeyFail = 1180; 
	
	/* ָ���������ӿ� */
	public final static int registerFinger = 2001;
	public final static int registerFingerSuccess = 2101;
	public final static int registerFingerFail = 2102;

	public final static int readFinger = 2002;
	public final static int readFingerSuccess = 2201;
	public final static int readFingerFail = 2202;

	/* ����֤�����ӿ� */
	public final static int getIDCardInfo = 3001;
	public final static int getIDCardInfoSuccess = 3101;
	public final static int getIDCardInfoFail = 3102;

	public final static int getIDFullInfo  = 3002;
	public final static int getIDFullInfoSuccess = 3201;
	public final static int getIDFullInfoFail = 3202;

	/* �ſ� */
	public final static int getBookAcct = 4001;
	public final static int getBookAcctSuccess = 4101;
	public final static int getBookAcctFail = 4102;

	/* ����ǩ�� */
	public final static int getSignature = 5001;
	public final static int getSignatureSuccess = 5101;
	public final static int getSignatureFail = 5102;

	public final static int getEncrySignature = 5003;
	public final static int getEncrySignatureSuccess = 5301;
	public final static int getEncrySignatureFail = 5302;

	public final static int keyAffuseSign  = 5002;
	public final static int keyAffuseSignSuccess  = 5201;
	public final static int keyAffuseSignFail  = 5202;

	public final static int getSignPhotoData  = 5004;
	public final static int getSignPhotoDataSuccess  = 5401;
	public final static int getSignPhotoDataFail  = 5402;

	/* IC�� */
	public final static int getICCardInfo = 6001;
	public final static int getICCardInfoSuccess = 6101;
	public final static int getICCardInfoFail = 6102;

	public final static int genARQC  = 6002;
	public final static int genARQCSuccess = 6201;
	public final static int genARQCFail = 6202;

	public final static int ARPC_ExeICScript = 6003;
	public final static int ARPC_ExeICScriptSuccess = 6301;
	public final static int ARPC_ExeICScriptFail = 6302;

	public final static int getTxDetail  = 6004;
	public final static int getTxDetailSuccess = 6401;
	public final static int getTxDetailFail = 6402;

	public final static int getCardRdWrtCap = 6005;
	public final static int getCardRdWrtCapSuccess = 6501;
	public final static int getCardRdWrtCapFail = 6502;

	public final static int getAllCardInfo = 6008;
	public final static int getAllCardInfoSuccess = 6801;
	public final static int getAllCardInfoFail = 6802;
}
