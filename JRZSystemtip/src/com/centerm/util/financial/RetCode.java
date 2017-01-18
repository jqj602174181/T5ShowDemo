package com.centerm.util.financial;

public class RetCode {
	/*
	 * 各类驱动都可能出现的错误：DRV+7位数字
	 * 错误编号	           错误描述
	 * DRV0000001	未知错误
	 * DRV0000002	超时错误
	 * DRV0000003	打开串口失败
	 * DRV0000004	向串口发送数据失败
	 * DRV0000005	接口参数错误
	 * DRV0000006	找不到动态链接库
	 * DRV0000007	动态链接库加载错误
	 * DRV0000008	通讯连接没有建立
	 */
	public final static String UNKNOWN_ERR = "DRV0000001";
	public final static String TIMEOUT_ERR = "DRV0000002";
	public final static String OPEN_SERIAL_ERR = "DRV0000003";
	public final static String SEND_MSG_ERR = "DRV0000004";
	public final static String PARAM_ERR = "DRV0000005";
	public final static String NOT_FIND_SO_Err = "DRV0000006";
	public final static String LOAD_SO_ERR = "DRV0000007";
	public final static String RECV_MSG_ERR = "DRV0000008";
	
	public final static String UNKNOWN_ERR_Msg = "未知错误";
	public final static String TIMEOUT_ERR_MSG = "超时错误";
	public final static String OPEN_SERIAL_ERR_MSG = "打开串口失败";
	public final static String SEND_MSG_ERR_MSG = "发送数据失败";
	public final static String PARAM_ERR_MSG = "接口参数错误";
	public final static String NOT_FIND_SO_Err_MSG = "找不到动态链接库";
	public final static String LOAD_SO_ERR_MSG = "动态链接库加载错误";
	public final static String RECV_MSG_ERR_MSG = "接收数据失败";
	
	/*
	 * 某类驱动出现的错误：3位设备类型代码+7位数字
	 * 设备类型代码	设备描述
	 * PIN	                         密码键盘
     * ICC	        IC卡
     * IDC	                        二代证（ID卡）
     * FIG			指纹仪
	 * RDC			刷卡器
	 * SIN          电子签名
	 */
	//指纹仪
	public static final int FIG_ERR_OPENFAILED = -1;//打开设备失败
	public static final int FIG_ERR_WRITE =	-2;//发送指令失败
	public static final int FIG_ERR_FAIL = -3;//获取失败
	public static final int FIG_ERR_TIMEOUT = -8;//超时
	
	public final static String FIG_FAIL_ERR = "RDC0000001";
	public final static String FIG_FAIL_ERR_MSG = "获取失败";
	
	//磁卡
	public final static String RDC_PACKAGE_FORMAT_ERR = "RDC0000001";//接收到的报文格式错误
	public final static String RDC_READ_CARD_ERR = "RDC0000002"; //读卡操作失败
	public final static String RDC_WRITE_CARD_ERR = "RDC0000003";//写卡操作失败
	public final static String RDC_CANCELED  = "RDC0000004";	 //用户取消
	public final static String RDC_PACKAGE_FORMAT_ERR_MSG = "接收到的报文格式错误";
	public final static String RDC_READ_CARD_ERR_MSG = "读卡操作失败";
	public final static String RDC_WRITE_CARD_ERR_MSG = "写卡操作失败";
	public final static String RDC_CANCELED_MSG  = "用户取消";
	
	public final static int RDC_ERR_OPENFAILED = -1;//打开设备失败
	public final static int RDC_ERR_WRITE =	-2;//发送指令失败
	public final static int RDC_ERR_READ = -3;//接收数据失败
	public final static int RDC_ERR_PACKAGE_FORMAT = -4;//接收到的报文格式错误
	public final static int RDC_ERR_READ_CARD = -5;//读卡操作失败
	public final static int RDC_ERR_WRITE_CARD = -6;//写卡操作失败
	public final static int RDC_ERR_CANCELED = -7;//用户取消
	public final static int RDC_ERR_TIMEOUT = -8;//超时
	public final static int RDC_ERR_OTHER = -100;//其他错误
	
	//二代证
	public final static String IDC_CANCELED  = "IDC0000001";	 //用户取消
	public final static String IDC_CHECK_ERR  = "IDC0000002";	 //校验错误
	public final static String IDC_CMD_ERR  = "IDC0000003";	  
	public final static String IDC_CANCELED_MSG  = "用户取消";	 
	public final static String IDC_CHECK_ERR_MSG  = "校验错误";	 
	public final static String IDC_CMD_ERR_MSG  = "指令错误";	     
	
	public final static int IDC_ERR_ERROR  = -1;  
	public final static int IDC_ERR_PARAM  = -2;  
	public final static int IDC_ERR_TIMEOUT	= -3;   
	public final static int IDC_ERR_OPEN   = -4;  
	public final static int IDC_ERR_WRITE  = -5;  
	public final static int IDC_ERR_READ   = -6;   
	public final static int IDC_ERR_CANCEL = -7;   
	public final static int IDC_ERR_CHECK  = -8;
	public final static int IDC_ERR_CMD	   = -9;
	
	//电子签名
	public final static String SIN_CANCELED  = "SIN0000001";	 //用户取消
	public final static String SIN_ENCRY_FAIL  = "SIN0000002";	 //加密失败
	public final static String SIN_CANCELED_MSG  = "用户取消";	 //用户取消
	public final static String SIN_ENCRY_FAIL_MSG  = "加密失败";	 //加密失败
	
	//IC
	public final static String ICC_CLOSE_PORT_FAIL = "ICC0000001";//关闭串口失败
	public final static String ICC_EX_PORT_FAIL = "ICC000002"; //设置转口失败
	public final static String ICC_NO_CARD_FAIL = "ICC000003"; //设置转口失败
	
	public final static String ICC_CLOSE_PORT_FAIL_MSG = "关闭串口失败";//
	public final static String ICC_EX_PORT_FAIL_MSG = "设置转口失败"; //
	public final static String ICC_NO_CARD_FAIL_MSG = "设置转口失败"; //
	
	//-101---108
	public final static String ICC_POWER_ON_FAIL  = "ICC0000101";	 //上电失败
	public final static String  ICC_POWER_OFF_FAIL  = "ICC0000102";//下电失败  
	public final static String  ICC_TOUCH_CARD_OUT  = "ICC0000103";//接触式IC卡不在位
	public final static String  ICC_UNTOUCH_CARD_OUT  = "ICC0000104";//非接触式IC卡不在位
	public final static String  ICC_UNSUPT_TOUCH_CARD  = "ICC0000105";//不支持接触式IC卡
	public final static String  ICC_TOUCH_CARD_ON  = "ICC0000106";//接触式用户卡已经上电
	public final static String  ICC_UNTOUCH_OFF  = "ICC0000107";//非接触式卡未激活
	public final static String  ICC_UNTOUCH_ON_FAIL  = "ICC0000108";//非接触式卡激活失败
	
	public final static String ICC_POWER_ON_FAIL_MSG  = "上电失败";	 //
	public final static String  ICC_POWER_OFF_FAIL_MSG  = "下电失败";//  
	public final static String  ICC_TOUCH_CARD_OUT_MSG  = "接触式IC卡不在位";//
	public final static String  ICC_UNTOUCH_CARD_OUT_MSG  = "非接触式IC卡不在位";//
	public final static String  ICC_UNSUPT_TOUCH_CARD_MSG  = "不支持接触式IC卡";//
	public final static String  ICC_TOUCH_CARD_ON_MSG  = "接触式用户卡已经上电";//
	public final static String  ICC_UNTOUCH_OFF_MSG  = "非接触式卡未激活";//
	public final static String  ICC_UNTOUCH_ON_FAIL_MSG  = "非接触式卡激活失败";//
	
	//-201--216
	public final static String  ICC_GET_APP_FAIL  = "ICC00000201";//选择应用环境失败      
	public final static String  ICC_SELECT_APP_FAIL = "ICC0000202";//选择应用失败          
	public final static String  ICC_INIT_FAIL  = "ICC0000203";//应用初始化失败
	public final static String ICC_GEN_ARQC_FAIL  = "ICC0000204";	 //获取ARQC失败
	public final static String ICC_FAIL_EXT_AUTH  = "ICC0000205";	 //外部认证错误   
	public final static String ICC_FAIL_EXEC_SRPT  = "ICC0000206";	 //执行脚本错误  
	public final static String ICC_NOLOGENTER_ERR  = "ICC0000207";	 //没有日志入口
	public final static String ICC_CARD_LOCK  = "ICC0000208";	     //卡片被锁定
	public final static String ICC_CARD_LOCK_FOREVER  = "ICC0000209";//卡片被锁定
	public final static String ICC_PARASE_GAC_ERR  = "ICC0000210";	 //解析GAC错误
	public final static String ICC_GET_CDOL1_FAIL  = "ICC0000211";	 //获取CDOL1数据失败
	public final static String ICC_GET_CDOL2_FAIL  = "ICC0000212";	 //获取CDOL2数据失败
	public final static String ICC_GEN_PDOL_FAIL  = "ICC0000213";	 //生产PDOL数据失败
	public final static String ICC_AIDLIST_LOCK  = "ICC0000214";	 //指令错误
	public final static String  ICC_AIDLIST_SEL_FAIL  = "ICC0000215";//应用列表选择失败
	public final static String  ICC_GEN_TC_FAIL  = "ICC0000216";//生成TC失败
	
	public final static String  ICC_GET_APP_FAIL_MSG  = "选择应用环境失败 ";//     
	public final static String  ICC_SELECT_APP_FAIL_MSG = "选择应用失败";//          
	public final static String  ICC_INIT_FAIL_MSG  = "应用初始化失败";//
	public final static String ICC_GEN_ARQC_FAIL_MSG  = "获取ARQC失败";	 //
	public final static String ICC_FAIL_EXT_AUTH_MSG  = "外部认证错误 ";	 //  
	public final static String ICC_FAIL_EXEC_SRPT_MSG  = "执行脚本错误  ";	 //
	public final static String ICC_NOLOGENTER_ERR_MSG  = "没有日志入口";	 //
	public final static String ICC_CARD_LOCK_MSG  = "卡片被锁定";	     //
	public final static String ICC_CARD_LOCK_FOREVER_MSG  = "卡片被锁定";//
	public final static String ICC_PARASE_GAC_ERR_MSG  = "解析GAC错误";	 //
	public final static String ICC_GET_CDOL1_FAIL_MSG  = "获取CDOL1数据失败";	 //
	public final static String ICC_GET_CDOL2_FAIL_MSG  = "获取CDOL2数据失败";	 //
	public final static String ICC_GEN_PDOL_FAIL_MSG  = "生产PDOL数据失败";	 //
	public final static String ICC_AIDLIST_LOCK_MSG  = "指令错误";	 //
	public final static String  ICC_AIDLIST_SEL_FAIL_MSG  = "应用列表选择失败";//
	public final static String  ICC_GEN_TC_FAIL_MSG  = "生成TC失败";//
	
	//-301--307
	public final static String  ICC_PARSE_FAIL  = "ICC0000301";//数据解析tlv失败
	public final static String  ICC_GENLOG_FAIL  = "ICC0000302";//生成日志文件失败
	public final static String  ICC_SEND_RR_FAIL  = "ICC0000303";//'ReadRecord'发送命令到卡片失败
	public final static String  ICC_CMD_RR_FAIL  = "ICC0000304";//'ReadRecord'命令执行失败
	public final static String  ICC_SEND_IcGenArqc_FAIL  = "ICC0000305";//'GenArqc'发送命令到卡片失败
	public final static String  ICC_CMD_IcGenArqc_FAIL  = "ICC0000306";//'GenArqc'命令执行失败
	public final static String  ICC_SEND_ICExecute_FAIL  = "ICC0000307";//'脚本执行'发送命令到卡片失败
	
	public final static String  ICC_PARSE_FAIL_MSG  = "数据解析tlv失败";//
	public final static String  ICC_GENLOG_FAIL_MSG  = "生成日志文件失败";//
	public final static String  ICC_SEND_RR_FAIL_MSG  = "ReadRecord发送命令到卡片失败";//
	public final static String  ICC_CMD_RR_FAIL_MSG  = "ReadRecord命令执行失败";//
	public final static String  ICC_SEND_IcGenArqc_FAIL_MSG  = "GenArqc发送命令到卡片失败";//
	public final static String  ICC_CMD_IcGenArqc_FAIL_MSG  = "GenArqc命令执行失败";//
	public final static String  ICC_SEND_ICExecute_FAIL_MSG  = "脚本执行发送命令到卡片失败";//
	
	//-308---316
	public final static String ICC_SCRIPT_FAIL             = "ICC0000308"; //'脚本执行'命令执行失败
	public final static String ICC_SEND_ICIssAuth_FAIL     = "ICC0000309"; //'发卡行认证'发送命令到卡片失败
	public final static String ICC_CMD_ICIssAuth_FAIL      = "ICC0000310"; //'发卡行认证'命令执行失败
	public final static String ICC_SEND_ICGenTc_FAIL       = "ICC0000311"; //'GenTc'发送命令到卡片失败
	public final static String ICC_CMD_ICGenTc_FAIL        = "ICC0000312"; //'GenTc'执行命令失败
	public final static String ICC_SEND_ICGenAAC_FAIL      = "ICC0000313"; //'GenAAC'发送命令到卡片失败
	public final static String ICC_CMD_ICGenAAC_FAIL       = "ICC0000314"; //'GenAAC'执行命令失败
	public final static String  ICC_SEND_ICGpo_FAIL  = "ICC0000315";//'GPO指令'发送命令到卡片失败
	public final static String  ICC_CMD_ICGpo_FAIL  = "ICC0000316";//'GPO指令'执行命令失败
	
	public final static String ICC_SCRIPT_FAIL_MSG             = "脚本执行命令执行失败"; //
	public final static String ICC_SEND_ICIssAuth_FAIL_MSG     = "发卡行认证发送命令到卡片失败"; //
	public final static String ICC_CMD_ICIssAuth_FAIL_MSG      = "发卡行认证命令执行失败"; //
	public final static String ICC_SEND_ICGenTc_FAIL_MSG       = "GenTc发送命令到卡片失败"; //
	public final static String ICC_CMD_ICGenTc_FAIL_MSG        = "GenTc执行命令失败"; //
	public final static String ICC_SEND_ICGenAAC_FAIL_MSG      = "GenAAC发送命令到卡片失败"; //
	public final static String ICC_CMD_ICGenAAC_FAIL_MSG       = "GenAAC执行命令失败"; //
	public final static String  ICC_SEND_ICGpo_FAIL_MSG  = "GPO指令发送命令到卡片失败";//
	public final static String  ICC_CMD_ICGpo_FAIL_MSG  = "GPO指令执行命令失败";//
	
	//-317--321
	public final static String  ICC_SEND_ICSelect_FAIL  = "ICC0000317";//'SLEECT'发送命令到卡片失败
	public final static String  ICC_CMD_ICSelect_FAIL  = "ICC0000318";//'SLEECT'执行命令失败
	public final static String  ICC_SEND_Getdata_FAIL  = "ICC0000319";//'Getdata'发送命令到卡片失败
	public final static String  ICC_CMD_Getdata_FAIL  = "ICC0000320";//'Getdata'执行命令失败
	public final static String  ICC_SEND_APDU_FAIL  = "ICC0000321";// SEND_APDU执行命令失败
	
	public final static String  ICC_SEND_ICSelect_FAIL_MSG  = "SLEECT发送命令到卡片失败";//
	public final static String  ICC_CMD_ICSelect_FAIL_MSG  = "SLEECT执行命令失败";//
	public final static String  ICC_SEND_Getdata_FAIL_MSG  = "Getdata发送命令到卡片失败";//
	public final static String  ICC_CMD_Getdata_FAIL_MSG  = "Getdata执行命令失败";//
	public final static String  ICC_SEND_APDU_FAIL_MSG  = "SEND_APDU执行命令失败";//
	
	//错误返回值
	public final static int ICC_ERR_PARAM_ERR               = -1; //参数错误            
	public final static int ICC_ERR_OPEN_PORT_FAIL          = -2; //打开串口（U口）失败 
	public final static int ICC_ERR_TIMEOUT_ERR             = -3; //读卡超时            
	public final static int ICC_ERR_CLOSE_PORT_FAIL         = -4; //关闭串口失败
	public final static int ICC_ERR_EX_PORT_FAIL            = -5; //设置转口失败
	public final static int ICC_ERR_NO_CARD_FAIL            = -11; //无卡

	 //上电失败          
	public final static int ICC_ERR_OTHER_ERR               = -100; //未知        
	public final static int ICC_ERR_POWER_ON_FAIL           = -101;
	public final static int ICC_ERR_POWER_OFF_FAIL          = -102; //下电失败  
	public final static int ICC_ERR_TOUCH_CARD_OUT          = -103; //接触式IC卡不在位
	public final static int ICC_ERR_UNTOUCH_CARD_OUT        = -104; //非接触式IC卡不在位
	public final static int ICC_ERR_UNSUPT_TOUCH_CARD       = -105; //不支持接触式IC卡
	public final static int ICC_ERR_TOUCH_CARD_ON           = -106; //接触式用户卡已经上电
	public final static int ICC_ERR_UNTOUCH_OFF             = -107; //非接触式卡未激活
	public final static int ICC_ERR_UNTOUCH_ON_FAIL         = -108; //非接触式卡激活失败

	public final static int ICC_ERR_GET_APP_FAIL            = -201; //选择应用环境失败      
	public final static int ICC_ERR_SELECT_APP_FAIL         = -202; //选择应用失败          
	public final static int ICC_ERR_INIT_FAIL               = -203; //应用初始化失败  
	public final static int ICC_ERR_GEN_ARQC_FAIL           = -204; //获取ARQC失败      
	public final static int ICC_ERR_FAIL_EXT_AUTH           = -205; //外部认证错误          
	public final static int ICC_ERR_FAIL_EXEC_SRPT          = -206; //执行脚本错误          
	public final static int ICC_ERR_NOLOGENTER_ERR          = -207; //没有日志入口
	public final static int ICC_ERR_CARD_LOCK               = -208; //卡片被锁定
	public final static int ICC_ERR_CARD_LOCK_FOREVER       = -209; //卡片被锁定
	public final static int ICC_ERR_PARASE_GAC_ERR          = -210; //解析GAC错误
	public final static int ICC_ERR_GET_CDOL1_FAIL          = -211; //获取CDOL1数据失败
	public final static int ICC_ERR_GET_CDOL2_FAIL          = -212; //获取CDOL2数据失败
	public final static int ICC_ERR_GEN_PDOL_FAIL           = -213; //生产PDOL数据失败
	public final static int ICC_ERR_AIDLIST_LOCK            = -214; //应用锁定
	public final static int ICC_ERR_AIDLIST_SEL_FAIL        = -215; //应用列表选择失败
	public final static int ICC_ERR_GEN_TC_FAIL             = -216; //生成TC失败

	public final static int ICC_ERR_PARSE_FAIL              = -301; //数据解析tlv失败
	public final static int ICC_ERR_GENLOG_FAIL             = -302; //生成日志文件失败
	public final static int ICC_ERR_SEND_RR_FAIL            = -303; //'ReadRecord'发送命令到卡片失败
	public final static int ICC_ERR_CMD_RR_FAIL             = -304; //'ReadRecord'命令执行失败
	public final static int ICC_ERR_SEND_IcGenArqc_FAIL     = -305; //'GenArqc'发送命令到卡片失败
	public final static int ICC_ERR_CMD_IcGenArqc_FAIL      = -306; //'GenArqc'命令执行失败
	public final static int ICC_ERR_SEND_ICExecute_FAIL     = -307; //'脚本执行'发送命令到卡片失败
	
	public final static int ICC_ERR_SCRIPT_FAIL             = -308; //'脚本执行'命令执行失败
	public final static int ICC_ERR_SEND_ICIssAuth_FAIL     = -309; //'发卡行认证'发送命令到卡片失败
	public final static int ICC_ERR_CMD_ICIssAuth_FAIL      = -310; //'发卡行认证'命令执行失败
	public final static int ICC_ERR_SEND_ICGenTc_FAIL       = -311; //'GenTc'发送命令到卡片失败
	public final static int ICC_ERR_CMD_ICGenTc_FAIL        = -312; //'GenTc'执行命令失败
	public final static int ICC_ERR_SEND_ICGenAAC_FAIL      = -313; //'GenAAC'发送命令到卡片失败
	public final static int ICC_ERR_CMD_ICGenAAC_FAIL       = -314; //'GenAAC'执行命令失败
	public final static int ICC_ERR_SEND_ICGpo_FAIL         = -315; //'GPO指令'发送命令到卡片失败
	public final static int ICC_ERR_CMD_ICGpo_FAIL          = -316; //'GPO指令'执行命令失败
	public final static int ICC_ERR_SEND_ICSelect_FAIL      = -317; //'SLEECT'发送命令到卡片失败
	public final static int ICC_ERR_CMD_ICSelect_FAIL       = -318; //'SLEECT'执行命令失败
	public final static int ICC_ERR_SEND_Getdata_FAIL       = -319; //'Getdata'发送命令到卡片失败
	public final static int ICC_ERR_CMD_Getdata_FAIL        = -320; //'Getdata'执行命令失败
	public final static int ICC_ERR_SEND_APDU_FAIL          = -321; // SEND_APDU执行命令失败
}
