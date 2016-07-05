/*!
 * \file trans_app.h
 * \version V1.00
 * \author zff
 * \date 20110923
 * \brief ����ǽ�������ʵ����Ҫ�ı�ǩͷ�ļ�
 *
 * \par �޸ļ�¼��
 *          \<flag\>    \<author\>  \<date\>    \<version\> \<description\> \n
 *
 * \details Copyright(C) Ceneterm Technology Co., Ltd. All Right Reserved
 */
 
#ifndef _TRANS_TAG_H_
#define _TRANS_TAG_H_

#define TAG_PSE                                                          "\x70"     
#define TAG_APP                                                          "\x61"
#define TAG_DIR_DISCRETIONARY                                            "\x73"            //Ŀ¼�Զ���ģ��
#define TAG_ISSUER_SCRIPT1                                               "\x71"            //�����нű�1
#define TAG_ISSUER_SCRIPT2                                               "\x72"            //�����нű�2
#define TAG_FCI                                                          "\x6F"
#define TAG_FCI_PROPRIETARY                                              "\xA5"            //FCIר��ģ��
#define TAG_RESPONSE_MSG_FORMAT2                                         "\x77"            //��Ӧ����ģ���ʽ2
#define TAG_FCI_ISSUER_DISCRETIONARY_DATA                                "\xBF\x0C"        //FCI�������Զ�������
#define TAG_AID                                                          "\x4F"            //Ӧ�ñ�ʶ
#define TAG_APP_LABEL                                                    "\x50"            //Ӧ�ñ�ǩ
#define TAG_FID                                                          "\x51"            /*K-CASH only?*/
#define TAG_TRACK2_EQUIVALENT_DATA                                       "\x57"            //������Ч����
#define TAG_PAN                                                          "\x5A"            //Ӧ�����ʺ�
#define TAG_RESPONSE_MSG_FORMAT1                                         "\x80"            //��Ӧ����ģ���ʽ1 
#define TAG_TRM_AMT_AUTHORISED_BIN                                       "\x81"            //������Ȩ���
#define TAG_AIP                                                          "\x82"            //Ӧ�ý�������
#define TAG_DFNAME                                                       "\x84"            //ר���ļ�(DF)��
#define TAG_TRMCOMMAND                                                   "\x83"            //����ģ��
#define TAG_ISSUER_SCRIPT_COMMAND                                        "\x86"            //�����нű�
#define TAG_APP_PRIORITY_INDICATOR                                       "\x87"            //Ӧ�����ȼ�ָʾ��
#define TAG_SFI                                                          "\x88"            //���ļ���ʾSFI
#define TAG_AUXN_CODE                                                    "\x89"            //��Ȩ��
#define TAG_ARC                                                          "\x8A"            //��ȨӦ����
#define TAG_CDOL1                                                        "\x8C"            //��Ƭ���չ������ݶ����б�1(CDOL1)
#define TAG_CDOL2                                                        "\x8D"            //��Ƭ���չ������ݶ����б�2(CDOL2)
#define TAG_CVM_LIST                                                     "\x8E"            //�ֿ�����֤����(CVM)�б�
#define TAG_CA_PK_INDEX                                                  "\x8F"            //��֤���Ĺ�Կ����
#define TAG_ISSUER_PKCERT                                                "\x90"            //�����й�Կ֤��
#define TAG_ISSUER_AUTH_DATA                                             "\x91"            //�����м�������
#define TAG_ISSUER_PKR                                                   "\x92"            //�����й�Կ����
#define TAG_SIGNED_SAD                                                   "\x93"            //ǩ����̬Ӧ������
#define TAG_AFL                                                          "\x94"            //Ӧ���ļ���λ��
#define TAG_TRM_TVR                                                      "\x95"            //�ն���֤���
#define TAG_TDOL                                                         "\x97"            //����֤�����ݶ����б�(TDOL)
#define TAG_TRM_TC_HASHVALUE                                             "\x98"            //����֤��(TC)��ϣֵ
#define TAG_TRM_TRXN_PINDATA                                             "\x99"            //���׸�������(PIN)����
#define TAG_TRM_TRXN_DATE                                                "\x9A"            //��������
#define TAG_TRM_TSI                                                      "\x9B"            //����״̬��Ϣ
#define TAG_TRM_TRXN_TYPE                                                "\x9C"            //��������
#define TAG_DDF_NAME                                                     "\x9D"            //Ŀ¼�����ļ�(DDF)��
#define TAG_C1_MONETA                                                    "\xC1"            /*MONETA only?*/
#define TAG_C2_MONETA                                                    "\xC2"            /*MONETA only?*/
#define TAG_CARD_HOLDER_NAME                                             "\x5F\x20"        //�ֿ�������
#define TAG_APP_EXPIRATION_DATE                                          "\x5F\x24"        //Ӧ��ʧЧ����
#define TAG_APP_EFFECTIVE_DATE                                           "\x5F\x25"        //Ӧ����Ч����
#define TAG_ISSUER_COUNTRY_CODE                                          "\x5F\x28"        //�����й��Ҵ���
#define TAG_TRM_TRXN_CURRENCY_CODE                                       "\x5F\x2A"        //���׻��Ҵ���
#define TAG_LANGUAGE_PREFERENCE                                          "\x5F\x2D"        //��ѡ����
#define TAG_SERVICE_CODE                                                 "\x5F\x30"        //������
#define TAG_PAN_SEQNO                                                    "\x5F\x34"        //Ӧ�����ʺ�(PAN)ϵ�к�
#define TAG_TRM_TRXN_CURRENCY_EXP                                        "\x5F\x36"        //���׻���ָ��
#define TAG_ISSUER_URL                                                   "\x5F\x50"        //������URL
#define TAG_TRM_ACQUIRER_ID                                              "\x9F\x01"        //�յ��б�ʶ
#define TAG_TRM_AMOUNT_AUTHORISED                                        "\x9F\x02"        //��Ȩ���(����)
#define TAG_TRM_AMOUNT_OTHER1                                            "\x9F\x03"        //�������(����)
#define TAG_TRM_AMOUNT_OTHER2                                            "\x9F\x04"        //�������(������)
#define TAG_APP_DOSCRETIONARY_DATA                                       "\x9F\x05"        //Ӧ���Զ�������
#define TAG_TRM_AID                                                      "\x9F\x06"        //Ӧ�ñ�ʶ
#define TAG_AUC                                                          "\x9F\x07"        //Ӧ����;����
#define TAG_APP_VERSION_NO                                               "\x9F\x08"        //Ӧ�ð汾�š�
#define TAG_TRM_APP_VERSION_NO                                           "\x9F\x09"        //Ӧ�ð汾��
#define TAG_CARDHOLDER_NAME_EXT                                          "\x9F\x0B"        //�ֿ���������չ
#define TAG_IAC_DEFAULT                                                  "\x9F\x0D"        //��������Ϊ����-ȱʡ
#define TAG_IAC_DENIAL                                                   "\x9F\x0E"        //��������Ϊ����-�ܾ�
#define TAG_IAC_ONLINE                                                   "\x9F\x0F"        //��������Ϊ����-����
#define TAG_ISSUER_APP_DATA                                              "\x9F\x10"        //������Ӧ������
#define TAG_ISSUER_CODE_TABLE_INDEX                                      "\x9F\x11"        //�����д��������
#define TAG_APP_PREFERRED_NAME                                           "\x9F\x12"        //Ӧ����ѡ��
#define TAG_LAST_ONLINE_ATCR                                             "\x9F\x13"        //��������������(ATC)�Ĵ���
#define TAG_LCO_LIMIT                                                    "\x9F\x14"        //�����ѻ���������
#define TAG_TRM_MCC                                                      "\x9F\x15"        //�̻�������
#define TAG_TRM_MID                                                      "\x9F\x16"        //�̻���ʶ
#define TAG_PIN_TRY_COUNTER                                              "\x9F\x17"        //��������(PIN)���Դ���
#define TAG_ISSUER_SCRIPT_ID                                             "\x9F\x18"        //�����нű���ʶ
#define TAG_TRM_COUNTRY_CODE                                             "\x9F\x1A"        //�ն˹��Ҵ���
#define TAG_TRM_FLOOR_LIMIT                                              "\x9F\x1B"        //�ն�����޶�
#define TAG_TRM_IDENTIFICATION                                           "\x9F\x1C"        //�ն˱�ʶ
#define TAG_TRM_RISK_MANAGEMENT_DATA                                     "\x9F\x1D"        //�ն˷��չ�������
#define TAG_TRM_IFD_SERIAL_NO                                            "\x9F\x1E"        //�ӿ��豸(IFD)ϵ�к�
#define TAG_TRACK1_DISCRETIONARY_DATA                                    "\x9F\x1F"        //һ�ŵ���Ч����
#define TAG_TRACK2_DISCRETIONARY_DATA                                    "\x9F\x20"        //���ŵ��Զ�������
#define TAG_TRM_TRXN_TIME                                                "\x9F\x21"        //����ʱ��
#define TAG_TRM_CAPKI                                                    "\x9F\x22"        //��֤���Ĺ�Կ����
#define TAG_UCO_LIMIT                                                    "\x9F\x23"        //�����ѻ���������
#define TAG_AC                                                           "\x9F\x26"        //Ӧ������
#define TAG_CRYPTOGRAMINFODATA                                           "\x9F\x27"        //������Ϣ����
#define TAG_ICCPIN_EPKC                                                  "\x9F\x2D"        //IC��PIN���ܹ�Կ֤��
#define TAG_ICCPIN_EPKE                                                  "\x9F\x2E"        //IC��PIN���ܹ�Կָ��
#define TAG_ICCPIN_EPKR                                                  "\x9F\x2F"        //IC��PIN���ܹ�Կ����
#define TAG_ISSUER_PKE                                                   "\x9F\x32"        //�����й�Կָ��
#define TAG_TRM_CAPA                                                     "\x9F\x33"        //�ն�����
#define TAG_TRM_CVMR                                                     "\x9F\x34"        //�ֿ�����֤����(CVM)���
#define TAG_TRM_TYPE                                                     "\x9F\x35"        //�ն�����
#define TAG_ATC                                                          "\x9F\x36"        //Ӧ�ý���ϵ�к�
#define TAG_TRM_UNPREDICTABLE_NO                                         "\x9F\x37"        //�����
#define TAG_PDOL                                                         "\x9F\x38"        //����ѡ�����ݶ����б�(PDOL)
#define TAG_TRM_POS_ENTRY_MODE                                           "\x9F\x39"        //POS���뷽ʽ
#define TAG_TRM_AMOUNT_REF_CURRENCY                                      "\x9F\x3A"        //�ο����ҽ��
#define TAG_App_REF_CURRENCY                                             "\x9F\x3B"        //Ӧ�òο�����
#define TAG_TRM_TRXN_REF_CURRENCY_CODE                                   "\x9F\x3C"        //���ײο�����
#define TAG_TRM_TRXN_REF_CURRENCY_EXP                                    "\x9F\x3D"        //���ײο�����ָ��
#define TAG_TRM_CAPA_ADDITIONAL                                          "\x9F\x40"        //�����ն�����
#define TAG_TRM_TRXN_SEQUENCE_COUNTER                                    "\x9F\x41"        //������ż�����
#define TAG_APP_CURRENCY_CODE                                            "\x9F\x42"        //Ӧ�û��Ҵ���
#define TAG_APP_REF_CURRENCY_EXP                                         "\x9F\x43"        //Ӧ�òο�����ָ��
#define TAG_APP_CURRENCY_EXP                                             "\x9F\x44"        //Ӧ�û���ָ��
#define TAG_DAC                                                          "\x9F\x45"        //���ݼ�����
#define TAG_ICC_PKC                                                      "\x9F\x46"        //IC����Կ֤��
#define TAG_ICC_PKE                                                      "\x9F\x47"        //IC����Կָ��
#define TAG_ICC_PKR                                                      "\x9F\x48"        //IC����Կ����
#define TAG_DDOL                                                         "\x9F\x49"        //��̬���ݶ����б�(DDOL)
#define TAG_SDA_TAG_LIST                                                 "\x9F\x4A"        //��̬���ݼ�����ǩ�б�
#define TAG_SIGNED_DAD                                                   "\x9F\x4B"        //ǩ����̬Ӧ������
#define TAG_ICC_DYNAMIC_NO                                               "\x9F\x4C"        //IC����̬��
#define TAG_APP_CURRENCY_CODE_VISA                                       "\x9F\x51"        /*VISA only*/
#define TAG_APP_DEFAULT_ACTION_VISA                                      "\x9F\x52"        /*VISA only*/
#define TAG_TRM_TCC                                                      "\x9F\x53"        /*EUROPAY only(TCC)*/
#define TAG_CONSECUTIVET_LIMITINT_VISA                                   "\x9F\x53"        /*VISA only*/
#define TAG_CUMULATIVE_TTA_LIMIT_VISA                                    "\x9F\x54"        /*VISA only*/
#define TAG_GEO_GRAPHIC_INDICATOR_VISA                                   "\x9F\x55"        /*VISA only*/
#define TAG_ISSUER_AUXN_INDICATOR_VISA                                   "\x9F\x56"        /*VISA only*/
#define TAG_ISSUER_COUNTRY_CODE_VISA                                     "\x9F\x57"        /*VISA only*/
#define TAG_LCOLIMITVISA                                                 "\x9F\x58"        /*VISA only*/
#define TAG_UCOLIMITVISA                                                 "\x9F\x59"        /*VISA only*/
#define TAG_ISSUER_URL2_VISA                                             "\x9F\x5A"        /*VISA only*/
#define TAG_TRM_ISR                                                      "\x9F\x5B"
#define TAG_CUMULATIVE_TTAU_LIMITVISA                                    "\x9F\x5C"        /*VISA only*/
#define TAG_CONSECUTIVET_LIMIT_VISA                                      "\x9F\x72"        /*VISA only*/
#define TAG_CURRENCY_CONVERSION_FACTOR                                   "\x9F\x73"
#define TAG_VLP_ISSUER_AUXN_CODE_VISA                                    "\x9F\x74"        /*VISA only*/
#define TAG_CUMULATIVE_TTA_LIMIT_DC_VISA                                 "\x9F\x75"        /*VISA only*/
#define TAG_SECONDARY_APP_CURRENCY_CODE                                  "\x9F\x76"
#define TAG_VLP_FUNDS_LIMIT_VISA                                         "\x9F\x77"        /*VISA only*/
#define TAG_VLP_SINGLE_TRXN_LIMIT_VISA                                   "\x9F\x78"        /*VISA only*/
#define TAG_VLP_AVAILABLE_FUNDS                                          "\x9F\x79"
#define TAG_TRM_SUPPORT_INDICATOR                                        "\x9F\x7A"
#define TAG_TRM_VLP_TRM_TRXN_LIMIT                                       "\x9F\x7B"        /*Visa Low-Value Payment(VLP)*/
#define TAG_CPLC_HISTORY_FID_VISA                                        "\x9F\x7F"        /*VISA only*/
#define TAG_TRM_SCRIPT                                                   "\x17"            /*Bitel Defined TrmScript*/
#define TAG_TRM_CAPK                                                     "\x18"            /*Bitel Defined TrmCAPK  */
#define TAG_TRM_PDOL_DATA                                                "\x19"            /*Bitel Defined PDOL_data*/
#define TAG_TRM_CDOL1DATA                                                "\x1A"            /*Bitel Defined CDOL1data*/
#define TAG_TRM_CDOL2DATA                                                "\x1B"            /*Bitel Defined CDOL1data*/
#define TAG_EOT4                                                         "\x04"            /*Bitel Defined DataEOT  */

#endif

