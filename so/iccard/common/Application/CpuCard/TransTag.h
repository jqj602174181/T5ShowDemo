/*!
 * \file trans_app.h
 * \version V1.00
 * \author zff
 * \date 20110923
 * \brief 借贷记交易流程实现需要的标签头文件
 *
 * \par 修改记录：
 *          \<flag\>    \<author\>  \<date\>    \<version\> \<description\> \n
 *
 * \details Copyright(C) Ceneterm Technology Co., Ltd. All Right Reserved
 */
 
#ifndef _TRANS_TAG_H_
#define _TRANS_TAG_H_

#define TAG_PSE                                                          "\x70"     
#define TAG_APP                                                          "\x61"
#define TAG_DIR_DISCRETIONARY                                            "\x73"            //目录自定义模板
#define TAG_ISSUER_SCRIPT1                                               "\x71"            //发卡行脚本1
#define TAG_ISSUER_SCRIPT2                                               "\x72"            //发卡行脚本2
#define TAG_FCI                                                          "\x6F"
#define TAG_FCI_PROPRIETARY                                              "\xA5"            //FCI专有模板
#define TAG_RESPONSE_MSG_FORMAT2                                         "\x77"            //响应报文模板格式2
#define TAG_FCI_ISSUER_DISCRETIONARY_DATA                                "\xBF\x0C"        //FCI发卡行自定义数据
#define TAG_AID                                                          "\x4F"            //应用标识
#define TAG_APP_LABEL                                                    "\x50"            //应用标签
#define TAG_FID                                                          "\x51"            /*K-CASH only?*/
#define TAG_TRACK2_EQUIVALENT_DATA                                       "\x57"            //二道等效数据
#define TAG_PAN                                                          "\x5A"            //应用主帐号
#define TAG_RESPONSE_MSG_FORMAT1                                         "\x80"            //响应报文模板格式1 
#define TAG_TRM_AMT_AUTHORISED_BIN                                       "\x81"            //交易授权金额
#define TAG_AIP                                                          "\x82"            //应用交互特征
#define TAG_DFNAME                                                       "\x84"            //专心文件(DF)名
#define TAG_TRMCOMMAND                                                   "\x83"            //命令模板
#define TAG_ISSUER_SCRIPT_COMMAND                                        "\x86"            //发卡行脚本
#define TAG_APP_PRIORITY_INDICATOR                                       "\x87"            //应用优先级指示器
#define TAG_SFI                                                          "\x88"            //短文件表示SFI
#define TAG_AUXN_CODE                                                    "\x89"            //授权码
#define TAG_ARC                                                          "\x8A"            //授权应答码
#define TAG_CDOL1                                                        "\x8C"            //卡片风险管理数据对象列表1(CDOL1)
#define TAG_CDOL2                                                        "\x8D"            //卡片风险管理数据对象列表2(CDOL2)
#define TAG_CVM_LIST                                                     "\x8E"            //持卡人验证方法(CVM)列表
#define TAG_CA_PK_INDEX                                                  "\x8F"            //认证中心公钥索引
#define TAG_ISSUER_PKCERT                                                "\x90"            //发卡行公钥证书
#define TAG_ISSUER_AUTH_DATA                                             "\x91"            //发卡行鉴定数据
#define TAG_ISSUER_PKR                                                   "\x92"            //发卡行公钥余项
#define TAG_SIGNED_SAD                                                   "\x93"            //签名静态应用数据
#define TAG_AFL                                                          "\x94"            //应用文件定位器
#define TAG_TRM_TVR                                                      "\x95"            //终端验证结果
#define TAG_TDOL                                                         "\x97"            //交易证书数据对象列表(TDOL)
#define TAG_TRM_TC_HASHVALUE                                             "\x98"            //交易证书(TC)哈希值
#define TAG_TRM_TRXN_PINDATA                                             "\x99"            //交易个人密码(PIN)数据
#define TAG_TRM_TRXN_DATE                                                "\x9A"            //交易日期
#define TAG_TRM_TSI                                                      "\x9B"            //交易状态信息
#define TAG_TRM_TRXN_TYPE                                                "\x9C"            //交易类型
#define TAG_DDF_NAME                                                     "\x9D"            //目录数据文件(DDF)名
#define TAG_C1_MONETA                                                    "\xC1"            /*MONETA only?*/
#define TAG_C2_MONETA                                                    "\xC2"            /*MONETA only?*/
#define TAG_CARD_HOLDER_NAME                                             "\x5F\x20"        //持卡人姓名
#define TAG_APP_EXPIRATION_DATE                                          "\x5F\x24"        //应用失效日期
#define TAG_APP_EFFECTIVE_DATE                                           "\x5F\x25"        //应用生效日期
#define TAG_ISSUER_COUNTRY_CODE                                          "\x5F\x28"        //发卡行国家代码
#define TAG_TRM_TRXN_CURRENCY_CODE                                       "\x5F\x2A"        //交易货币代码
#define TAG_LANGUAGE_PREFERENCE                                          "\x5F\x2D"        //首选语言
#define TAG_SERVICE_CODE                                                 "\x5F\x30"        //服务码
#define TAG_PAN_SEQNO                                                    "\x5F\x34"        //应用主帐号(PAN)系列号
#define TAG_TRM_TRXN_CURRENCY_EXP                                        "\x5F\x36"        //交易货币指数
#define TAG_ISSUER_URL                                                   "\x5F\x50"        //发卡行URL
#define TAG_TRM_ACQUIRER_ID                                              "\x9F\x01"        //收单行标识
#define TAG_TRM_AMOUNT_AUTHORISED                                        "\x9F\x02"        //授权金额(数字)
#define TAG_TRM_AMOUNT_OTHER1                                            "\x9F\x03"        //其它金额(数字)
#define TAG_TRM_AMOUNT_OTHER2                                            "\x9F\x04"        //其它金额(二进制)
#define TAG_APP_DOSCRETIONARY_DATA                                       "\x9F\x05"        //应用自定义数据
#define TAG_TRM_AID                                                      "\x9F\x06"        //应用标识
#define TAG_AUC                                                          "\x9F\x07"        //应用用途控制
#define TAG_APP_VERSION_NO                                               "\x9F\x08"        //应用版本号。
#define TAG_TRM_APP_VERSION_NO                                           "\x9F\x09"        //应用版本号
#define TAG_CARDHOLDER_NAME_EXT                                          "\x9F\x0B"        //持卡人姓名扩展
#define TAG_IAC_DEFAULT                                                  "\x9F\x0D"        //发卡行行为代码-缺省
#define TAG_IAC_DENIAL                                                   "\x9F\x0E"        //发卡行行为代码-拒绝
#define TAG_IAC_ONLINE                                                   "\x9F\x0F"        //发卡行行为代码-联机
#define TAG_ISSUER_APP_DATA                                              "\x9F\x10"        //发卡行应用数据
#define TAG_ISSUER_CODE_TABLE_INDEX                                      "\x9F\x11"        //发卡行代码表索引
#define TAG_APP_PREFERRED_NAME                                           "\x9F\x12"        //应用首选名
#define TAG_LAST_ONLINE_ATCR                                             "\x9F\x13"        //最近联机交易序号(ATC)寄存器
#define TAG_LCO_LIMIT                                                    "\x9F\x14"        //连续脱机交易下限
#define TAG_TRM_MCC                                                      "\x9F\x15"        //商户类别代码
#define TAG_TRM_MID                                                      "\x9F\x16"        //商户标识
#define TAG_PIN_TRY_COUNTER                                              "\x9F\x17"        //个人密码(PIN)重试次数
#define TAG_ISSUER_SCRIPT_ID                                             "\x9F\x18"        //发卡行脚本标识
#define TAG_TRM_COUNTRY_CODE                                             "\x9F\x1A"        //终端国家代码
#define TAG_TRM_FLOOR_LIMIT                                              "\x9F\x1B"        //终端最低限额
#define TAG_TRM_IDENTIFICATION                                           "\x9F\x1C"        //终端标识
#define TAG_TRM_RISK_MANAGEMENT_DATA                                     "\x9F\x1D"        //终端风险管理数据
#define TAG_TRM_IFD_SERIAL_NO                                            "\x9F\x1E"        //接口设备(IFD)系列号
#define TAG_TRACK1_DISCRETIONARY_DATA                                    "\x9F\x1F"        //一磁道等效数据
#define TAG_TRACK2_DISCRETIONARY_DATA                                    "\x9F\x20"        //二磁道自定义数据
#define TAG_TRM_TRXN_TIME                                                "\x9F\x21"        //交易时间
#define TAG_TRM_CAPKI                                                    "\x9F\x22"        //认证中心公钥索引
#define TAG_UCO_LIMIT                                                    "\x9F\x23"        //连续脱机交易上限
#define TAG_AC                                                           "\x9F\x26"        //应用密文
#define TAG_CRYPTOGRAMINFODATA                                           "\x9F\x27"        //密文信息数据
#define TAG_ICCPIN_EPKC                                                  "\x9F\x2D"        //IC卡PIN加密公钥证书
#define TAG_ICCPIN_EPKE                                                  "\x9F\x2E"        //IC卡PIN加密公钥指数
#define TAG_ICCPIN_EPKR                                                  "\x9F\x2F"        //IC卡PIN加密公钥余项
#define TAG_ISSUER_PKE                                                   "\x9F\x32"        //发卡行公钥指数
#define TAG_TRM_CAPA                                                     "\x9F\x33"        //终端性能
#define TAG_TRM_CVMR                                                     "\x9F\x34"        //持卡人验证方法(CVM)结果
#define TAG_TRM_TYPE                                                     "\x9F\x35"        //终端类型
#define TAG_ATC                                                          "\x9F\x36"        //应用交易系列号
#define TAG_TRM_UNPREDICTABLE_NO                                         "\x9F\x37"        //随机数
#define TAG_PDOL                                                         "\x9F\x38"        //处理选项数据对象列表(PDOL)
#define TAG_TRM_POS_ENTRY_MODE                                           "\x9F\x39"        //POS输入方式
#define TAG_TRM_AMOUNT_REF_CURRENCY                                      "\x9F\x3A"        //参考货币金额
#define TAG_App_REF_CURRENCY                                             "\x9F\x3B"        //应用参考货币
#define TAG_TRM_TRXN_REF_CURRENCY_CODE                                   "\x9F\x3C"        //交易参考货币
#define TAG_TRM_TRXN_REF_CURRENCY_EXP                                    "\x9F\x3D"        //交易参考货币指数
#define TAG_TRM_CAPA_ADDITIONAL                                          "\x9F\x40"        //附加终端性能
#define TAG_TRM_TRXN_SEQUENCE_COUNTER                                    "\x9F\x41"        //交易序号计数器
#define TAG_APP_CURRENCY_CODE                                            "\x9F\x42"        //应用货币代码
#define TAG_APP_REF_CURRENCY_EXP                                         "\x9F\x43"        //应用参考货币指数
#define TAG_APP_CURRENCY_EXP                                             "\x9F\x44"        //应用货币指数
#define TAG_DAC                                                          "\x9F\x45"        //数据鉴定码
#define TAG_ICC_PKC                                                      "\x9F\x46"        //IC卡公钥证书
#define TAG_ICC_PKE                                                      "\x9F\x47"        //IC卡公钥指数
#define TAG_ICC_PKR                                                      "\x9F\x48"        //IC卡公钥余项
#define TAG_DDOL                                                         "\x9F\x49"        //动态数据对象列表(DDOL)
#define TAG_SDA_TAG_LIST                                                 "\x9F\x4A"        //静态数据鉴定标签列表
#define TAG_SIGNED_DAD                                                   "\x9F\x4B"        //签名动态应用数据
#define TAG_ICC_DYNAMIC_NO                                               "\x9F\x4C"        //IC卡动态数
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

