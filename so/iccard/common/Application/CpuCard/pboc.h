#ifndef _PBOC_H_
#define _PBOC_H_

#ifndef LINUX_EDITION
#define _STDOUT _stdcall 
#include <windows.h>

#else

#define _STDOUT __attribute__  ((visibility("default"))) 

#endif

typedef enum
{
    NO='0',
    YES='1'
    
}YESORNO;


typedef struct
{
    int nPid;          //HID �豸��PID��
    int nVid;          //HID �豸��VID��
    int nPbNo;         //��չ�ж˿ڵ�Ĭ��ֵ
    int nReadTimeOut;  //�豸��ʱʱ��
    int nPortTimeOut;   //���ڳ�ʱʱ��
	YESORNO cDeviceAuth;//�豸��ȫ��֤
} ST_DEVICE_PARAM;



/*
**  PBOC����ϸ���׼�¼
*/
typedef struct Pboc_Detail
{
    char szDate[8+1];          /*<  ��������    YYYYMMDD    3 */
    char szTime[6+1];          /*<  ����ʱ��    HHMMSS  3 */
    char szAmount[12+1];       /*<  ��Ȩ���    B   6  */
    char szOtherAmount[12+1];  /*<  �������    B   6 */ 
    char szCountyCode[4+1];    /*<  �ն˹��Ҵ���    N3  2 */
    char szCurCode[4+1];       /*<  ���׻��Ҵ���    N3  2 */
    char szMerchen[20+1];      /*<  �̻�����    Ans 20 */
    char cTranceType[2+1];     /*<  ��������    N2  1 */
    char szATC[4+1];           /*<  Ӧ�ý��׼�������ATC��   B   2 */ 
    char sBeforeAmt[12+1];     /*<  Ȧ��ǰ��� */
    char sAfterAmt[12+1];      /*<  Ȧ��֮���� */
    char sAmtType[4+1];        /*<  ����  */
}ST_PBOC_DETAIL;

ST_PBOC_DETAIL stPbocList[10];  // ���ڿ�Ƭ��ౣ��10��������־



typedef struct 
{
    char szAcqID_n_9F01[6];             /**<(TERM)�յ��б�ʶ*/  
    char szAddTermCap_b_9F40[5];        /**<(TERM)�����ն�����*/    
    char szMerCateCode_n_9F15[2];       /**<(TERM)�̻�������*/
    char szMerID_ans_9F16[15];          /**<(TERM)�̻���ʶ*/
    char szMerNameLoc_ans[40];          /**<(TERM)�̻����ƺ�λ��*/
    char szTermCap_b_9F33[3];           /**<(TERM)�ն�����*/
    char szTermCountryCode_b_9F1A[2];   /**<(TERM)�ն˹��Ҵ���*/
    char szTermId_an_9F1C[8];           /**<(TERM)�ն˺�*/
    char szAppCode_b_9F42[2];           /**<(TERM)Ӧ�û��Ҵ���*/
    char szTVR_b_95[5];                 /**<(TERM)�ն���֤���*/
    char cTypeTerm_n_9F35;              /**<(TERM)�ն�����*/
    char cEntryMode_n_9F39;             /**<(TERM)���۵�(POS)���뷽ʽ*/
    char szCurCode_n_5F2A[2];           /**<(TERM)���׻��Ҵ���*/
    char szRefCurrCode_n_9F3C[2];       /**<(TERM)���ײο����Ҵ���*/
    char szIFD_an_9F1E[8];              /**<(TERM)�ӿ��豸��IFD�����к�*/

    char cEcIndicator_9F7A;             /**< (EC)�����ֽ�ָʾ��*/
    char cEcBalanceLimit_9F7B[6];          /**< (EC)�����ֽ��ն˽����޶�*/

    char szTransProp_9F66[4+1];         /**< (RF TERM) �ն˽�������*/
    char cRfStatusCheck;                /**< (RF TERM) �ǽӴ��ն�״̬���*/

    char cAppExDF60;                    /**< (TERM) CAPP����ָʾλ  �ֶ�֧��0x01 */         
    char cSm;                           /**< (TERM)�����㷨ָʾλ*/
}ST_TERMPARA;



#define SUCC  (0)
#define FAIL  (-1)

#define ASSERT_UNSUCC(e) \
    if ((e) != SUCC)\
{\
    return FAIL;\
}\


/*<  ��Ϊ�ɹ�ʱ ���ش�����  */
#define ASSERT_RET(e) \
    if ((e) != SUCC)\
{\
    return e;\
}\


#define  NOTOUCH_CARD_IN_UP           1   //�ǽӿ���λ���ϵ�
#define  TOUCH_CARD_IN_UP             2   //�Ӵ�����λ���ϵ�
#define  TOUCH_CARD_IN                3   //�Ӵ�����λ
#define  APP_POWER_TIME_OUT           2   //��Ƭ�ϵ糬ʱ


//���󷵻�ֵ
#define  RET_PARAM_ERR               (-1) //��������            
#define  RET_OPEN_PORT_FAIL          (-2) //�򿪴��ڣ�U�ڣ�ʧ�� 
#define  RET_TIMEOUT_ERR             (-3) //������ʱ            
#define  RET_CLOSE_PORT_FAIL         (-4) //�رմ���ʧ��
#define  RET_EX_PORT_FAIL            (-5) //����ת��ʧ��

#define  RET_OTHER_ERR               (-100) //δ֪        
#define  RET_POWER_ON_FAIL           (-101) //�ϵ�ʧ��          
#define  RET_POWER_OFF_FAIL          (-102) //�µ�ʧ��  
#define  RET_TOUCH_CARD_OUT          (-103) //�Ӵ�ʽIC������λ
#define  RET_UNTOUCH_CARD_OUT        (-104) //�ǽӴ�ʽIC������λ
#define  RET_UNSUPT_TOUCH_CARD       (-105) //��֧�ֽӴ�ʽIC��
#define  RET_TOUCH_CARD_ON           (-106) //�Ӵ�ʽ�û����Ѿ��ϵ�
#define  RET_UNTOUCH_OFF             (-107) //�ǽӴ�ʽ��δ����
#define  RET_UNTOUCH_ON_FAIL         (-108) //�ǽӴ�ʽ������ʧ��

#define  RET_GET_APP_FAIL            (-201) //ѡ��Ӧ�û���ʧ��      
#define  RET_SELECT_APP_FAIL         (-202) //ѡ��Ӧ��ʧ��          
#define  RET_INIT_FAIL               (-203) //Ӧ�ó�ʼ��ʧ��            
#define  RET_GEN_ARQC_FAIL           (-204) //��ȡARQCʧ��      
#define  RET_FAIL_EXT_AUTH           (-205) //�ⲿ��֤����          
#define  RET_FAIL_EXEC_SRPT          (-206) //ִ�нű�����          
#define  RET_NOLOGENTER_ERR          (-207) //û����־���
#define  RET_CARD_LOCK               (-208) //��Ƭ������
#define  RET_CARD_LOCK_FOREVER       (-209) //��Ƭ������
#define  RET_PARASE_GAC_ERR          (-210) //����GAC����
#define  RET_GET_CDOL1_FAIL          (-211) //��ȡCDOL1����ʧ��
#define  RET_GET_CDOL2_FAIL          (-212) //��ȡCDOL2����ʧ��
#define  RET_GEN_PDOL_FAIL           (-213) //����PDOL����ʧ��
#define  RET_AIDLIST_LOCK            (-214) //Ӧ������
#define  RET_AIDLIST_SEL_FAIL        (-215) //Ӧ���б�ѡ��ʧ��
#define  RET_GEN_TC_FAIL             (-216) //����TCʧ��

#define  RET_PARSE_FAIL              (-301) //���ݽ���tlvʧ��
#define  RET_GENLOG_FAIL             (-302) //������־�ļ�ʧ��
#define  RET_SEND_RR_FAIL            (-303) //'ReadRecord'���������Ƭʧ��
#define  RET_CMD_RR_FAIL             (-304) //'ReadRecord'����ִ��ʧ��
#define  RET_SEND_IcGenArqc_FAIL     (-305) //'GenArqc'���������Ƭʧ��
#define  RET_CMD_IcGenArqc_FAIL      (-306) //'GenArqc'����ִ��ʧ��
#define  RET_SEND_ICExecute_FAIL     (-307) //'�ű�ִ��'���������Ƭʧ��
#define  RET_SCRIPT_FAIL             (-308) //'�ű�ִ��'����ִ��ʧ��
#define  RET_SEND_ICIssAuth_FAIL     (-309) //'��������֤'���������Ƭʧ��
#define  RET_CMD_ICIssAuth_FAIL      (-310) //'��������֤'����ִ��ʧ��
#define  RET_SEND_ICGenTc_FAIL       (-311) //'GenTc'���������Ƭʧ��
#define  RET_CMD_ICGenTc_FAIL        (-312) //'GenTc'ִ������ʧ��
#define  RET_SEND_ICGenAAC_FAIL      (-313) //'GenAAC'���������Ƭʧ��
#define  RET_CMD_ICGenAAC_FAIL       (-314) //'GenAAC'ִ������ʧ��
#define  RET_SEND_ICGpo_FAIL         (-315) //'GPOָ��'���������Ƭʧ��
#define  RET_CMD_ICGpo_FAIL          (-316) //'GPOָ��'ִ������ʧ��
#define  RET_SEND_ICSelect_FAIL      (-317) //'SLEECT'���������Ƭʧ��
#define  RET_CMD_ICSelect_FAIL       (-318) //'SLEECT'ִ������ʧ��
#define  RET_SEND_Getdata_FAIL       (-319) //'Getdata'���������Ƭʧ��
#define  RET_CMD_Getdata_FAIL        (-320) //'Getdata'ִ������ʧ��
#define  RET_SEND_APDU_FAIL          (-321) // SEND_APDUִ������ʧ��


#define  RET_DEVICE_AUTH_FAIL        (-1000) //�豸��֤ʧ��


/*
* ��ƬICC����Ԫ��ǩ
*/
#define   EMV_TAG_9F26_IC_AC                         "\x9F\x26"     /**<  0 - Ӧ������(Application Cryptogram)*/
#define   EMV_TAG_9F42_IC_APPCURCODE                 "\x9F\x42"     /**<  1 - Ӧ�û��Ҵ���(Application Currency Code)*/
#define   EMV_TAG_9F44_IC_APPCUREXP                  "\x9F\x44"     /**<  2 - Ӧ�û���ָ��(Application Currency Exponent)*/
#define   EMV_TAG_9F05_IC_APPDISCDATA                "\x9F\x05"     /**<  3 - Ӧ���Զ�������(Application Discretionary Data)*/
#define   EMV_TAG_5F25_IC_APPEFFECTDATE              "\x5F\x25"     /**<  4 - Ӧ����Ч����(Application Effective Date)YYMMDD*/
#define   EMV_TAG_5F24_IC_APPEXPIREDATE              "\x5F\x24"     /**<  5 - Ӧ��ʧЧ����(Application Expiration Date)YYMMDD*/
#define   EMV_TAG_94_IC_AFL                          "\x94"         /**<  6 - Ӧ���ļ���λ��AFL(Application File Locator)   */
#define   EMV_TAG_4F_IC_AID                          "\x4F"         /**<  7 - Ӧ�ñ�ʶ��AID(Application Identifier) */
#define   EMV_TAG_82_IC_AIP                          "\x82"         /**<  8 - Ӧ�ý�������AIP(Application Interchange Profile) */
#define   EMV_TAG_50_IC_APPLABEL                     "\x50"         /**<  9 - Ӧ�ñ�ǩ(Application Label) */
#define   EMV_TAG_9F12_IC_APNAME                     "\x9F\x12"     /**< 10 - Ӧ����ѡ����(Application Preferred Name) */
#define   EMV_TAG_5A_IC_PAN                          "\x5A"         /**< 11 - Ӧ�����ʺ�PAN(Application Primary Account Number) */
#define   EMV_TAG_5F34_IC_PANSN                      "\x5F\x34"     /**< 12 - Ӧ�����ʺ����PAN_Sn(Application Primary Account Number Sequence Number) */
#define   EMV_TAG_87_IC_APID                         "\x87"         /**< 13 - Ӧ�����ȱ�ʶ��(Application Priority Indicator)*/
#define   EMV_TAG_9F3B_IC_APCUR                      "\x9F\x3B"     /**< 14 - Ӧ����ѡ����(Application Preference Currency)*/
#define   EMV_TAG_9F43_IC_APCUREXP                   "\x9F\x43"     /**< 15 - Ӧ����ѡ����ָ��(Application Preferece Currency Exponent)*/
#define   EMV_TAG_61_IC_APPTEMP                      "\x61"         /**< 16 - Ӧ��ģ��(Application Template)*/
#define   EMV_TAG_9F36_IC_ATC                        "\x9F\x36"     /**< 17 - Ӧ�ý��׼�����ATC(Application Transaction Counter)*/
#define   EMV_TAG_9F07_IC_AUC                        "\x9F\x07"     /**< 18 - Ӧ����;����AUC(Application Usage Control)*/
#define   EMV_TAG_9F08_IC_APPVERNO                   "\x9F\x08"     /**< 19 - Ӧ�ð汾��(Application Version Number)*/
#define   EMV_TAG_5F54_IC_BIC                        "\x5F\x54"     /**<  - �����б�ʶ��(Bank Identifier Code (BIC))*/
#define   EMV_TAG_8C_IC_CDOL1                        "\x8C"         /**< 20 - ��Ƭ���չ������ݶ����б�1CDOL1(Card Risk Management Data Object List 1)*/
#define   EMV_TAG_8D_IC_CDOL2                        "\x8D"         /**< 21 - ��Ƭ���չ������ݶ����б�2CDOL2(Card Risk Management Data Object List 2)*/
#define   EMV_TAG_5F20_IC_HOLDERNAME                 "\x5F\x20"     /**< 22 - �ֿ�������(Cardholder Name)*/
#define   EMV_TAG_9F0B_IC_HOLDERNAMEEX               "\x9F\x0B"     /**< 23 - �ֿ�����չ����(Cardholder Name Extended)*/
#define   EMV_TAG_8E_IC_CVMLIST                      "\x8E"         /**< 24 - �ֿ�����֤�����б�(Cardholder Verification Method List)*/
#define   EMV_TAG_8F_IC_CAPKINDEX                    "\x8F"         /**< 25 - ��֤���Ĺ�Կ����(Certification Authority Public Key Index)*/
#define   EMV_TAG_9F27_IC_CID                        "\x9F\x27"     /**< 26 - ������Ϣ����CID(Cryptogram Infomation Data)*/
#define   EMV_TAG_9F45_IC_DTAUTHCODE                 "\x9F\x45"     /**< 27 - ������֤��(Data Authentication Code)*/
#define   EMV_TAG_84_IC_DFNAME                       "\x84"         /**< 28 - DF����(Dedicated File Name)*/
#define   EMV_TAG_9D_IC_DDFNAME                      "\x9D"         /**< 29 - Ŀ¼�����ļ�DDF����(Directory Definition File)*/
#define   EMV_TAG_73_IC_DIRDISCTEMP                  "\x73"         /**< 30 - Ŀ¼�Զ���ģ��(Directory Discretionary Template)*/
#define   EMV_TAG_9F49_IC_DDOL                       "\x9F\x49"     /**< 31 - ��̬������֤���ݶ����б�DDOL(Dynamic Data Authentication Data Object List)*/
#define   EMV_TAG_BF0C_IC_FCIDISCDATA                "\xBF\x0C"     /**< 32 - FCI�������Զ�������(File Control Information Issuer Discretionary Data)*/
#define   EMV_TAG_A5_IC_FCIPROPTEMP                  "\xA5"         /**< 33 - FCIר��ģ��(File Control Information Proprietary Template)*/
#define   EMV_TAG_6F_IC_FCITEMP                      "\x6F"         /**< 34 - FCIģ��(File Control Information Template)*/
#define   EMV_TAG_9F4C_IC_ICCDYNNUM                  "\x9F\x4C"     /**< 35 - IC����̬��(ICC Dynamic Number)*/
#define   EMV_TAG_9F2D_IC_PECERT                     "\x9F\x2D"     /**< 36 - IC��PIN���ܹ�Կ֤��(ICC PIN Encipherment Public Key Certificate)*/
#define   EMV_TAG_9F2E_IC_PEEXP                      "\x9F\x2E"     /**< 37 - IC��PIN���ܹ�Կָ��(ICC PIN Encipherment Public Key Exponent)*/
#define   EMV_TAG_9F2F_IC_PERMD                      "\x9F\x2F"     /**< 38 - IC��PIN���ܹ�Կ����(ICC PIN Encipherment Public Key Remainder)*/
#define   EMV_TAG_9F46_IC_ICCPKCERT                  "\x9F\x46"     /**< 39 - IC����Կ֤��(ICC Public Key Certificate)*/
#define   EMV_TAG_9F47_IC_ICCPKEXP                   "\x9F\x47"     /**< 40 - IC����Կָ��(ICC Public Key Exponent)*/
#define   EMV_TAG_9F48_IC_ICCPKRMD                   "\x9F\x48"     /**< 41 - IC����Կ����(ICC Public Key Remainder)*/
#define   EMV_TAG_5F53_IC_IBAN                       "\x5F\X53"     /**<  - ���������˻���(International Bank Account Number (IBAN) )*/
#define   EMV_TAG_9F0D_IC_IAC_DEFAULT                "\x9F\x0D"     /**< 42 - ��������Ϊ����-ȱʡ(Issuer Action Code-Default)*/
#define   EMV_TAG_9F0E_IC_IAC_DENIAL                 "\x9F\x0E"     /**< 43 - ��������Ϊ����-�ܾ�(Issuer Action Code-Denial)*/
#define   EMV_TAG_9F0F_IC_IAC_ONLINE                 "\x9F\x0F"     /**< 44 - ��������Ϊ����-����(Issuer Action Code-Online)*/
#define   EMV_TAG_9F10_IC_ISSAPPDATA                 "\x9F\x10"     /**< 45 - ������Ӧ������(Issuer Application Data)*/
#define   EMV_TAG_9F11_IC_ISSCTINDEX                 "\x9F\x11"     /**< 46 - �����д��������(Issuer Code Table Index)*/
#define   EMV_TAG_5F28_IC_ISSCOUNTRYCODE             "\x5F\x28"     /**< 47 - �����й��Ҵ���(Issuer Country Code)*/
#define   EMV_TAG_5F55_IC_ALPHA2                     "\x5F\X55"     /**<  - �ն˹��Ҵ���(Issuer Country Code (alpha2 format) )*/
#define   EMV_TAG_5F56_IC_ALPHA3                     "\x5F\X56"     /**<  - �ն˹��Ҵ���(Issuer Country Code (alpha3 format)  )*/
#define   EMV_TAG_42_IC_IIN                          "\x42"         /**<  - ��������֤����(Issuer Identification Number (IIN)  )*/
#define   EMV_TAG_90_IC_ISSPKCERT                    "\x90"         /**< 48 - �����й�Կ֤��(Issuer Public Key Certificate)*/
#define   EMV_TAG_9F32_IC_ISSPKEXP                   "\x9F\x32"     /**< 49 - �����й�Կָ��(Issuer Public Key Exponent)*/
#define   EMV_TAG_92_IC_ISSPKRMD                     "\x92"         /**< 50 - �����й�Կ����(Issuer Public Key Remainder)*/
#define   EMV_TAG_5F50_IC_ISSURL                     "\x5F\x50"     /**< 51 - ������URL(Issuer URL)*/
#define   EMV_TAG_5F2D_IC_LANGPREF                   "\x5F\x2D"     /**< 52 - ��ѡ����(Language Preference)*/
#define   EMV_TAG_9F13_IC_LASTATC                    "\x9F\x13"     /**< 53 - �ϴ�����ATC�Ĵ���(Last Online Application Transaction Counter Register)*/
#define   EMV_TAG_9F14_IC_LCOL                       "\x9F\x14"     /**< 54 - �����ѻ���������(Lower Consecutive Offline Limit)*/
#define   EMV_TAG_9F17_IC_PINTRYCNTR                 "\x9F\x17"     /**< 55 - PIN���Լ�����(Personal Identification Number Try Counter)*/
#define   EMV_TAG_9F38_IC_PDOL                       "\x9F\x38"     /**< 56 - ����ѡ�����ݶ����б�PDOL(Processing Options Data Object List)*/
#define   EMV_TAG_80_IC_RMTF1                        "\x80"         /**< 57 - ��Ӧ��Ϣģ���ʽ1(Response Message Template Format 1)*/
#define   EMV_TAG_77_IC_RMTF2                        "\x77"         /**< 58 - ��Ӧ��Ϣģ���ʽ2(Response Message Template Format 2)*/
#define   EMV_TAG_5F30_IC_SERVICECODE                "\x5F\x30"     /**< 59 - ������(Service Code)*/
#define   EMV_TAG_88_IC_SFI                          "\x88"         /**< 60 - ���ļ���ʶ��SFI(Short File Indicator)*/
#define   EMV_TAG_9F4B_IC_SIGNDYNAPPDT               "\x9F\x4B"     /**< 61 - ǩ���Ķ�̬Ӧ������(Signed Dynamic Application Data)*/
#define   EMV_TAG_93_IC_SIGNSTAAPPDT                 "\x93"         /**< 62 - ǩ���ľ�̬Ӧ������(Signed Static Application Data)*/
#define   EMV_TAG_9F4A_IC_SDATAGLIST                 "\x9F\x4A"     /**< 63 - ��̬������֤��ǩ�б�(Static Data Authentication Tag List)*/
#define   EMV_TAG_9F1F_IC_TRACK1DATA                 "\x9F\x1F"     /**< 64 - 1�ŵ��Զ�������(Track 1 Discretionary Data)*/
#define   EMV_TAG_9F20_IC_TRACK2DATA                 "\x9F\x20"     /**< 65 - 2�ŵ��Զ�������(Track 2 Discretionary Data)*/
#define   EMV_TAG_57_IC_TRACK2EQUDATA                "\x57"         /**< 66 - 2�ŵ���Ч����(Track 2 Equivalent Data)*/
#define   EMV_TAG_97_IC_TDOL                         "\x97"         /**< 67 - ����֤�����ݶ����б�TDOL(Transaction Certificate Data Object List)*/
#define   EMV_TAG_9F23_IC_UCOL                       "\x9F\x23"     /**< 68 - �����ѻ���������(Upper Consecutive Offline Limit) */
#define   EMV_TAG_70_TEMP                            "\x70"         /**< 69 - ��������ģ��70    */
#define   EMV_TAG_9F61_IC_CERTNO                     "\x9F\x61"     /**< 70 - ֤����*/
#define   EMV_TAG_9F62_IC_CERTTYPE                   "\x9F\x62"     /**< 71 - ֤������*/    
#define   EMV_TAG_9F4D_IC_PBOCLOG                    "\x9F\x4D"     /**< 72 - PBOC LOG*/
#define   EMV_TAG_9F4F_IC_PBOCLOGFORMAT              "\x9F\x4F"     /**< 73 - PBOC LOG��ʽ*/
#define   EMV_TAG_DF4D_IC_UPLOAD_FORMAT              "\xDF\x4D"     /**<  Ȧ��Ľ�����־��ʽ*/
#define   EMV_TAG_9F69_IC_CERTIFCATION_DATA          "\x9F\x69"     /**< ��Ƭ��֤�������*/
#define   EMV_TAG_9F63_IC_IDENTIFY_INFOR             "\x9F\x63"     /**< ��Ƭ��ʶ��Ϣ ***/


/*
* �ն�TERM����Ԫ��ǩ
*/
#define   EMV_TAG_9F01_TM_ACQID         "\x9F\x01"    /**< 0 - �յ��б�ʶ��(Acquirer Identifier)*/
#define   EMV_TAG_9F40_TM_CAP_AD        "\x9F\x40"    /**< 1 - �����ն�����(Additional Terminal Capability)*/
#define   EMV_TAG_81_TM_AUTHAMNTB       "\x81"        /**< 2 - ��Ȩ���|������(Amount,Authorised<Binary>)*/
#define   EMV_TAG_9F02_TM_AUTHAMNTN     "\x9F\x02"    /**< 3 - ��Ȩ���|����(Amount,Authorised<Binary>)*/
#define   EMV_TAG_9F04_TM_OTHERAMNTB    "\x9F\x04"    /**< 4 - �������|������(Amount,Other<Binary>)*/
#define   EMV_TAG_9F03_TM_OTHERAMNTN    "\x9F\x03"    /**< 5 - �������|����(Amount,Other<Numeric>)*/
#define   EMV_TAG_9F3A_TM_REFCURAMNT    "\x9F\x3A"    /**< 6 - �ο����ҽ��(Amount,Reference Currency)*/
#define   EMV_TAG_9F06_TM_AID           "\x9F\x06"    /**< 7 - �ն�AID(Terminal Application Identifier)*/
#define   EMV_TAG_9F09_TM_APPVERNO      "\x9F\x09"    /**< 8 - �ն�Ӧ�ð汾��(Terminal Application Version Number)*/
#define   EMV_TAG_5F57_TM_ACCOUNTTYPE   "\x5F\x57"    /**< 9- �˻�����(Account Type)*/
#define   EMV_TAG_8A_TM_ARC             "\x8A"        /**< 10 - ��Ȩ��Ӧ��ARC(Authorisation Response Code)*/
#define   EMV_TAG_9F34_TM_CVMRESULT     "\x9F\x34"    /**< 11 - �ֿ�����֤�������(Cardholder Verification Method Results)*/
#define   EMV_TAG_9F22_TM_CAPKINDEX     "\x9F\x22"    /**< 12 - �ն�CA��Կ����(Terminal Certification Authority Public Key Index)*/
#define   EMV_TAG_9F1E_TM_IFDSN         "\x9F\x1E"    /**< 13 - IFD���к�(Interface Device Serial Number)*/
#define   EMV_TAG_83_TM_COMTEM          "\x83"        /**<14- ����ģ��(Command Template )*/
#define   EMV_TAG_86_TM_ISSSCRCMD       "\x86"        /**< 15 - �����нű�����(Issuer Script Command)*/
#define   EMV_TAG_9F18_TM_ISSSCRID      "\x9F\x18"    /**< 16 - �����нű���ʶ��(Issuer Script Identifier)*/
#define   EMV_TAG_9F15_TM_MCHCATCODE    "\x9F\x15"    /**< 17 - �̻�������(Merchant Category Code)*/
#define   EMV_TAG_9F16_TM_MCHID         "\x9F\x16"    /**< 18 - �̻���ʶ��(Merchant Identifier)*/
#define   EMV_TAG_9F39_TM_POSENTMODE    "\x9F\x39"    /**< 19 - POS����ģʽ(Point-of-Service Entry Mode)*/
#define   EMV_TAG_9F33_TM_CAP           "\x9F\x33"    /**< 20 - �ն�����(Terminal Capabilities)*/
#define   EMV_TAG_9F1A_TM_CNTRYCODE     "\x9F\x1A"    /**< 21 - �ն˹��Ҵ���(Terminal Country Code)*/
#define   EMV_TAG_9F1B_TM_FLOORLMT      "\x9F\x1B"    /**< 22 - �ն��޶�(Terminal Floor Limit)*/
#define   EMV_TAG_9F1C_TM_TERMID        "\x9F\x1C"    /**< 23 - �ն˱�ʶ��(Terminal Identification)*/
#define   EMV_TAG_9F1D_TM_RMDATA        "\x9F\x1D"    /**< 24 - �ն˷��չ�������(Terminal Risk Management Data)*/
#define   EMV_TAG_9F35_TM_TERMTYPE      "\x9F\x35"    /**< 25 - �ն�����(Terminal Type)*/
#define   EMV_TAG_95_TM_TVR             "\x95"        /**< 26 - �ն���֤���TVR(Terminal Verification Result)*/
#define   EMV_TAG_98_TM_TCHASH          "\x98"        /**< 27 - ����֤���ϣֵ(Transaction Certificate Hash Value)*/
#define   EMV_TAG_5F2A_TM_CURCODE       "\x5F\x2A"    /**< 28 - ���׻��Ҵ���(Transaction Currency Code)*/
#define   EMV_TAG_5F36_TM_CUREXP        "\x5F\x36"    /**< 29 - ���׻���ָ��(Transaction Currency Exponent)*/
#define   EMV_TAG_9A_TM_TRANSDATE       "\x9A"        /**< 30 - ��������(Transaction Date)*/
#define   EMV_TAG_99_TM_PINDATA         "\x99"        /**< 31 - PIN����(Transaction Personal Identification Number Data)*/
#define   EMV_TAG_9F3C_TM_REFCURCODE    "\x9F\x3C"    /**< 32 - ���ײο����Ҵ���(Transaction Reference Currency Code)*/
#define   EMV_TAG_9F3D_TM_REFCUREXP     "\x9F\x3D"    /**< 33 - ���ײο�����ָ��(Transaction Reference Currency Exponent)*/
#define   EMV_TAG_9F41_TM_TRSEQCNTR     "\x9F\x41"    /**< 34 - �������м�����(Transcation Sequence Counter)*/
#define   EMV_TAG_9B_TM_TSI             "\x9B"        /**< 35 - ����״̬��ϢTSI(Transaction Status Information)*/
#define   EMV_TAG_9F21_TM_TRANSTIME     "\x9F\x21"    /**< 36 - ����ʱ��(Transaction Time)*/
#define   EMV_TAG_9C_TM_TRANSTYPE       "\x9C"        /**< 37 - ��������(Transaction Type)*/
#define   EMV_TAG_9F37_TM_UNPNUM        "\x9F\x37"    /**< 38 -  ����Ԥ֪��(Unpredictable Number) */
#define   EMV_TAG_9F4E_TM_NAMELOC       "\x9F\x4E"    /**< 39- �̻����Ƶ�ַ*/

/**<����������*/
#define   EMV_TAG_89_TM_AUTHCODE        "\x89"        /**< ��Ȩ��(Authorization Code)*/
#define   EMV_TAG_91_TM_ISSAUTHDT       "\x91"        /**<  ��������֤����(Issuer Authentication Data)*/
#define   EMV_TAG_71_ISS_RMTF           "\x71"        /**< �����нű�����71ģ��*/
#define   EMV_TAG_72_ISS_RMTF           "\x72"        /**< �����нű�����72ģ��*/
#define   EMV_TAG_71_ISS_SCRIPT         "\x3F"        /**< �����нű�����71ģ��*/
#define   EMV_TAG_72_ISS_SCRIPT         "\x7F"        /**< �����нű�����72ģ��*/

/**<����Ϊ����AID��������*/
#define EMV_TAG_DF01_TM_ASI             "\xDF\x01"    /**< 1 - Ӧ��ѡ��ָʾ��,  ָ���ն��Ƿ�֧��AID����ƥ�䷽ʽ*/
#define EMV_TAG_DF11_TM_DEFAULTTAC      "\xDF\x11"    /**< 2 - �ն���Ϊ���룭Ĭ��*/
#define EMV_TAG_DF12_TM_ONLINETAC       "\xDF\x12"    /**< 3 - �ն���Ϊ���룭����*/
#define EMV_TAG_DF13_TM_REFUSETAC       "\xDF\x13"    /**< 4 - �ն���Ϊ���룭�ܾ�*/
#define EMV_TAG_DF14_TM_DDOL            "\xDF\x14"    /**< 5 - ȱʡ��̬������֤���ݶ����б�(�䳤)*/
#define EMV_TAG_DF15_TM_RANHOLD         "\xDF\x15"    /**< 6 - ƫ�����ѡ�����ֵ*/
#define EMV_TAG_DF16_TM_RANMAXPER       "\xDF\x16"    /**< 7 - ƫ�����ѡ������Ŀ��ٷ���*/
#define EMV_TAG_DF17_TM_RANTARPER       "\xDF\x17"    /**< 8 - ���ѡ���Ŀ��ٷ���*/
#define EMV_TAG_DF18_TM_ONLINEPINCAP    "\xDF\x18"    /**< 9 - �ն�����PIN����*/
#define EMV_TAG_DF02_TM_CAPKMOD         "\xDF\x02"    /**< 9 - (CAPK)��֤����ģ*/
#define EMV_TAG_DF03_TM_CAPKCHECKSUM    "\xDF\x03"    /**< 10 - ��֤���Ĺ�ԿУ���*/    
#define EMV_TAG_DF04_TM_CAPKEXPONENT    "\xDF\x04"    /**< 11 - ��֤���Ĺ�Կָ��*/
#define EMV_TAG_DF05_TM_CAPKEXPIRE      "\xDF\x05"    /**< 12 - ��֤���Ĺ�Կ��Ч��*/
#define EMV_TAG_DF06_TM_CAPKHASHFLAG    "\xDF\x06"    /**< 13 - ��֤���Ĺ�Կ��ϣ�㷨��ʶ*/
#define EMV_TAG_DF07_TM_CAPKALGORITHM   "\xDF\x07"    /**< 14 - ��֤���Ĺ�Կ�㷨��ʶ*/
#define EMV_TAG_DF31_IC_IISSCRIRES      "\xDF\x31"    /**< 15 - �������ű����(Issuer Script Results)   */
#define EMV_TAG_DF69_TM_SM              "\xDF\x69"    /**< 16 - SM�㷨֧��ָʾ��*/
#define EMV_TAG_DF60_TM_CAPP            "\xDF\x60"    /**< 17 - �ն�֧�ֵ�CAPP ��������*/
#define EMV_TAG_DF61_IC_SP              "\xDF\x61"    /**< 18 -  1  �ֶο۷�Ӧ�ñ�ʶ */
#define EMV_TAG_DF62_IC_SPDD_LIMIT      "\xDF\x62"    /**< 19-   6 �����ֽ�ֶο۷ѵ����޶�*/
#define EMV_TAG_DF63_IC_SPDD_AMOUNT     "\xDF\x63"    /**< 20 -  6 �����ֽ�ֶο۷��ѵֿ۶�*/

/**<�Զ����ǩ*/
#define EMV_TAG_B0_TM_PDOL              "\xB0"         /**<PDOL*/
#define EMV_TAG_B1_TM_DDOL1             "\xB1"         /**<DDOL1*/
#define EMV_TAG_B2_TM_DDOL2             "\xB2"         /**<DDOL2*/
#define EMV_TAG_B3_TM_DTDOL             "\xB3"         /**<�ն�ȱʡTDOL*/
#define EMV_TAG_B4_TM_ISSPK             "\xB4"         /**<�����й�Կ*/
#define EMV_TAG_B8_TM_DYNAMICSIG        "\xB8"         /**<��̬ǩ����֤����*/
#define EMV_TAG_B9_TM_AUTHDATA          "\xB9"         /**<��̬��֤����*/
#define EMV_TAG_BD_TM_ONLINEPIN         "\xBD"         /**<������������*/
#define EMV_TAG_BE_MSD_TRACK2           "\xBE"         /**< MSD���ŵ�����*/
#define EMV_TAG_BF_IC_TRACK2_EXPIRY     "\xBA"         /**< ���ŵ���Ч��*/
#define EMV_TAG_BB_IC_PK                "\xBB"         /**<IC����Կ*/
#define EMV_TAG_C5_IC_CTTA              "\xC5"         /*�ۼƽ����ܽ��*/
#define EMV_TAG_C6_IC_CTTAL             "\xC6"         /*�ۼƽ����ܽ������*/

/**< �����ֽ� EC ֧��˫���ֿ�Ƭ����*/
#define EMV_TAG_DF79_IC_DOUBLE_CUR_EC_BALANCE               "\xDF\x79"  /**< �����ֽ���EC Balance��*/
#define EMV_TAG_DF77_IC_DOUBLE_CUR_EC_BALANCELIMIT          "\xDF\x77"  /**< �����ֽ�������ޣ�EC Balance Limit��*/
#define EMV_TAG_DF78_IC_DOUBLE_CUR_EC_STL                   "\xDF\x78"  /**< �����ֽ𵥱ʽ����޶EC Single Transaction Limit��*/
#define EMV_TAG_DF76_IC_DOUBLE_CUR_EC_RESETTHRESHOLD        "\xDF\x76"  /**< �����ֽ�������ֵ��EC Reset Threshold��*/
#define EMV_TAG_DF71_IC_DOUBLE_CUR_EC_CURCODE               "\xDF\x71"  /**< �����ֽ���Ҵ��루EC Cur Code��*/

/**< �����ֽ� EC��Ƭ����*/
#define EMV_TAG_9F79_IC_EC_BALANCE              "\x9F\x79"  /**< �����ֽ���EC Balance��*/
#define EMV_TAG_9F77_IC_EC_BALANCELIMIT         "\x9F\x77"  /**< �����ֽ�������ޣ�EC Balance Limit��*/
#define EMV_TAG_9F74_IC_EC_IAC                  "\x9F\x74"  /**< �����ֽ𷢿�����Ȩ�루EC Issuer Authorization Code��*/
#define EMV_TAG_9F78_IC_EC_STL                  "\x9F\x78"  /**< �����ֽ𵥱ʽ����޶EC Single Transaction Limit��*/
#define EMV_TAG_9F6D_IC_EC_RESETTHRESHOLD       "\x9F\x6D"  /**< �����ֽ�������ֵ��EC Reset Threshold��*/
#define EMV_TAG_9F51_IC_EC_CURCODE              "\x9F\x51"  /**< �����ֽ���Ҵ���**/

/**< �����ֽ� EC�ն�����*/
#define EMV_TAG_9F7A_TM_EC_TSI                  "\x9F\x7A"  /**< �����ֽ��ն�֧��ָʾ����EC Terminal Support Indicator��*/
#define EMV_TAG_9F7B_TM_EC_TTL                  "\x9F\x7B"  /**< �����ֽ��ն˽����޶EC Terminal Transaction Limit��*/

#define EMV_TAG_DF4F_UPLOAD_FORMAT              "\xDF\x4F" /**< Ȧ�潻�׵����ڸ�ʽ*/
#define EMV_TAG_DF79_UPLOAG_BALANCE             "\xDF\x79" /**< Ȧ�潻����־�Ľ��**/

/**< �ǽӴ��ն�����*/
#define EMV_TAG_9F66_TM_RF_TRANSPROP            "\x9F\x66"  /**< �ն˽�������*/
#define EMV_TAG_DF19_TM_RF_LIMIT                "\xDF\x19"  /**< �ǽӴ��ն˽����޶�*/
#define EMV_TAG_DF20_TM_RF_OFFLINELIMIT         "\xDF\x20"  /**< �ǽӴ��ն��ѻ������޶�*/
#define EMV_TAG_9F5D_IC_RF_BALANCE              "\x9F\x5D"  /**< �ѻ��������*/
#define EMV_TAG_9F6C_IC_RF_TRANSPROP            "\x9F\x6C"  /**< ��Ƭ��������*/
#define EMV_TAG_DF21_TM_CVMLIMIT                "\xDF\x21"  /**< �ն�ִ��CVM�޶�*/

#define EMV_TAG_DF62_IC_SUB_FEE_AMT_LIMIT       "\xDF\x62" 
#define EMV_TAG_DF63_IC_SUB_FEE_AMT_PAY         "\xDF\x63"


#define GET_IC_STATICDATA "\x80\xCA"
#define ICC_PORT_9600   "\x1B\x25\x49\x34"

typedef int ( *TranceDetail )( ST_PBOC_DETAIL* ,int , char *  );


int  tFunc_SetDeviceParameter( ST_DEVICE_PARAM* pstDeviceParam );
int  tFunc_OpenPort( int  nPortComNo, int  nBaund );
int  tFunc_ClosePort();
int  tFunc_SetExPort ( char cExPort );
int  tFunc_PowerOn( int *pATRLen, unsigned char *pATR );
int  tFunc_PowerOff();
void tFunc_SetSlot( int nSlot );
void tFunc_GetSlot( int * nSlot );
int  tFunc_CardPresent( int nSlot);
int  tFunc_SendCommand( int nCmdLen, unsigned char *pszCmd, int *pnRespLen, unsigned char *pszResp );
void tFunc_SetCmdErr( int nErrCode );
void tFunc_GetCmdErr( int * pnErrCode );
int  tFunc_SetTermParam( ST_TERMPARA * stInTermParam );
int  tFunc_SetAidList ( char *pszAidList, int nAidLen );
void tFunc_SetInfoTag ( char *pszTagList[], int nNum );
int  tFunc_GetIcCardInfo( int * pnUsrInfoLen, char *pszUserInfo );
int  tFunc_SetAppTransactionTag ( char *pszTranceInfo );
void tFunc_SetMesgTag (  char * pszArqc[], int nNumArqc, char *pszTc[], int nNumTc );
int  tFunc_GenARQC ( int * pnArqcLen, char *pszArqc );
int  tFunc_IssueAuth ( char *pszARPC, int nARPCLen, char * szAuthCode );
int  tFunc_CompleteTransaction( unsigned char * in_pszAuthCode );
int  tFunc_WriteScript( char *pszScript, int nScriptLen, char *sResult );
int  tFunc_GetTC ( int *pnTCLen, char *pszTC );


#endif