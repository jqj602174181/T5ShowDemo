package com.centerm.util.financial;

public class MsgTranslateUtil {

	/**
	 * 获取指纹仪错误码
	 * @param ret
	 */
	public static String getFigErrorCode(int ret)
	{
		switch (ret) {
		case RetCode.FIG_ERR_OPENFAILED:
			return RetCode.OPEN_SERIAL_ERR;
		case RetCode.FIG_ERR_WRITE:
			return RetCode.SEND_MSG_ERR;
		case RetCode.FIG_ERR_FAIL:
			return RetCode.FIG_FAIL_ERR;
		case RetCode.FIG_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR;
		default:
			return RetCode.UNKNOWN_ERR;
		}
	}
	
	/**
	 * 获取指纹仪错误消息
	 * @param ret
	 */
	public static String getFigErrorMsg(int ret)
	{
		switch (ret) {
		case RetCode.FIG_ERR_OPENFAILED:
			return RetCode.OPEN_SERIAL_ERR_MSG;
		case RetCode.FIG_ERR_WRITE:
			return RetCode.SEND_MSG_ERR_MSG;
		case RetCode.FIG_ERR_FAIL:
			return RetCode.FIG_FAIL_ERR_MSG;
		case RetCode.FIG_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR_MSG;
		default:
			return RetCode.UNKNOWN_ERR_Msg;
		}
	}
	
	/**
	 * 获取磁卡错误码
	 * @param ret
	 */
	public static String getRdcErrorCode(int ret)
	{
		switch (ret) {
		case RetCode.RDC_ERR_OPENFAILED:
			return RetCode.OPEN_SERIAL_ERR;
		case RetCode.RDC_ERR_WRITE:
			return RetCode.SEND_MSG_ERR;
		case RetCode.RDC_ERR_PACKAGE_FORMAT:
			return RetCode.RDC_PACKAGE_FORMAT_ERR;
		case RetCode.RDC_ERR_READ_CARD:
			return RetCode.RDC_READ_CARD_ERR;
		case RetCode.RDC_ERR_WRITE_CARD:
			return RetCode.RDC_WRITE_CARD_ERR;
		case RetCode.RDC_ERR_CANCELED:
			return RetCode.RDC_CANCELED;
		case RetCode.RDC_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR;	
		default:
			return RetCode.UNKNOWN_ERR;
		}
	}
	
	/**
	 * 获取磁卡错误消息
	 * @param ret
	 */
	public static String getRdcErrorMsg(int ret)
	{
		switch (ret) {
		case RetCode.RDC_ERR_OPENFAILED:
			return RetCode.OPEN_SERIAL_ERR_MSG;
		case RetCode.RDC_ERR_WRITE:
			return RetCode.SEND_MSG_ERR_MSG;
		case RetCode.RDC_ERR_PACKAGE_FORMAT:
			return RetCode.RDC_PACKAGE_FORMAT_ERR;
		case RetCode.RDC_ERR_READ_CARD:
			return RetCode.RDC_READ_CARD_ERR_MSG;
		case RetCode.RDC_ERR_WRITE_CARD:
			return RetCode.RDC_WRITE_CARD_ERR_MSG;
		case RetCode.RDC_ERR_CANCELED:
			return RetCode.RDC_CANCELED_MSG;
		case RetCode.RDC_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR_MSG;	
		default:
			return RetCode.UNKNOWN_ERR_Msg;
		}
	}
	
	/**
	 * 获取二代证错误码
	 * @param ret
	 */
	public static String getIdcErrorCode(int ret)
	{
		switch (ret) {
		case RetCode.IDC_ERR_ERROR:
			return RetCode.UNKNOWN_ERR;
		case RetCode.IDC_ERR_PARAM  :
			return RetCode.PARAM_ERR;
		case RetCode.IDC_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR;
		case RetCode.IDC_ERR_OPEN   :
			return RetCode.OPEN_SERIAL_ERR;
		case RetCode.IDC_ERR_WRITE  :
			return RetCode.SEND_MSG_ERR;
		case RetCode.IDC_ERR_READ   :
			return RetCode.RECV_MSG_ERR;
		case RetCode.IDC_ERR_CANCEL :
			return RetCode.IDC_CANCELED;	
		case RetCode.IDC_ERR_CHECK  :
			return RetCode.IDC_CHECK_ERR;
		case RetCode.IDC_ERR_CMD: 
			return RetCode.IDC_CMD_ERR;
		default:
			return RetCode.UNKNOWN_ERR;
		}
	}
	
	/**
	 * 获取二代证错误消息
	 * @param ret
	 */
	public static String getIdcErrorMsg(int ret)
	{
		switch (ret) {
		case RetCode.IDC_ERR_ERROR:
			return RetCode.UNKNOWN_ERR_Msg;
		case RetCode.IDC_ERR_PARAM  :
			return RetCode.PARAM_ERR_MSG;
		case RetCode.IDC_ERR_TIMEOUT:
			return RetCode.TIMEOUT_ERR_MSG;
		case RetCode.IDC_ERR_OPEN   :
			return RetCode.OPEN_SERIAL_ERR_MSG;
		case RetCode.IDC_ERR_WRITE  :
			return RetCode.SEND_MSG_ERR_MSG;
		case RetCode.IDC_ERR_READ   :
			return RetCode.RECV_MSG_ERR_MSG;
		case RetCode.IDC_ERR_CANCEL :
			return RetCode.IDC_CANCELED_MSG;	
		case RetCode.IDC_ERR_CHECK  :
			return RetCode.IDC_CHECK_ERR_MSG;
		case RetCode.IDC_ERR_CMD: 
			return RetCode.IDC_CMD_ERR_MSG;
		default:
			return RetCode.UNKNOWN_ERR_Msg;
		}
	}
	
	/**
	 * 获取IC卡错误码
	 * @param ret
	 */
	public static String getIccErrorCode(int ret)
	{
		switch (ret) {
		case RetCode.ICC_ERR_PARAM_ERR:
			return RetCode.PARAM_ERR;
		case RetCode.ICC_ERR_OPEN_PORT_FAIL:
			return RetCode.OPEN_SERIAL_ERR;
		case RetCode.ICC_ERR_TIMEOUT_ERR:
			return RetCode.TIMEOUT_ERR;
		case RetCode.ICC_ERR_CLOSE_PORT_FAIL:
			return RetCode.ICC_CLOSE_PORT_FAIL;
		case RetCode.ICC_ERR_EX_PORT_FAIL:
			return RetCode.ICC_EX_PORT_FAIL;
		case RetCode.ICC_ERR_NO_CARD_FAIL:
			return RetCode.ICC_NO_CARD_FAIL;
			
		case RetCode.ICC_ERR_OTHER_ERR :
			return RetCode.UNKNOWN_ERR;	
		case RetCode.ICC_ERR_POWER_ON_FAIL :
			return RetCode.ICC_POWER_ON_FAIL;
		case RetCode.ICC_ERR_POWER_OFF_FAIL: 
			return RetCode.ICC_POWER_OFF_FAIL;
		case RetCode.ICC_ERR_TOUCH_CARD_OUT:
			return RetCode.ICC_TOUCH_CARD_OUT;
		case RetCode.ICC_ERR_UNTOUCH_CARD_OUT:
			return RetCode.ICC_UNTOUCH_CARD_OUT;
		case RetCode.ICC_ERR_UNSUPT_TOUCH_CARD:
			return RetCode.ICC_UNSUPT_TOUCH_CARD;
		case RetCode.ICC_ERR_TOUCH_CARD_ON:
			return RetCode.ICC_TOUCH_CARD_ON;
		case RetCode.ICC_ERR_UNTOUCH_OFF:
			return RetCode.ICC_UNTOUCH_OFF;
		case RetCode.ICC_ERR_UNTOUCH_ON_FAIL:
			return RetCode.ICC_UNTOUCH_ON_FAIL;
			
		case RetCode.ICC_ERR_GET_APP_FAIL :
			return RetCode.ICC_GET_APP_FAIL;	
		case RetCode.ICC_ERR_FAIL_EXEC_SRPT  :
			return RetCode.ICC_FAIL_EXEC_SRPT;
		case RetCode.ICC_ERR_NOLOGENTER_ERR: 
			return RetCode.ICC_NOLOGENTER_ERR;
		case RetCode.ICC_ERR_CARD_LOCK:
			return RetCode.ICC_CARD_LOCK;
		case RetCode.ICC_ERR_CARD_LOCK_FOREVER  :
			return RetCode.ICC_CARD_LOCK_FOREVER;
		case RetCode.ICC_ERR_PARASE_GAC_ERR:
		case RetCode.ICC_ERR_GET_CDOL1_FAIL   :
			return RetCode.ICC_GET_CDOL1_FAIL;
		case RetCode.ICC_ERR_GET_CDOL2_FAIL  :
			return RetCode.ICC_GET_CDOL2_FAIL;	
		case RetCode.ICC_ERR_GEN_PDOL_FAIL   :
			return RetCode.ICC_GEN_PDOL_FAIL;
		case RetCode.ICC_ERR_AIDLIST_LOCK :
			return RetCode.ICC_AIDLIST_LOCK;	
		case RetCode.ICC_ERR_AIDLIST_SEL_FAIL  :
			return RetCode.ICC_AIDLIST_SEL_FAIL;
		case RetCode.ICC_ERR_GEN_TC_FAIL: 
			return RetCode.ICC_GEN_TC_FAIL;	
			
			
		case RetCode.ICC_ERR_PARSE_FAIL  :
			return RetCode.ICC_PARSE_FAIL;
		case RetCode.ICC_ERR_GENLOG_FAIL:
			return RetCode.ICC_GENLOG_FAIL;
		case RetCode.ICC_ERR_SEND_RR_FAIL   :
			return RetCode.ICC_SEND_RR_FAIL;
		case RetCode.ICC_ERR_CMD_RR_FAIL  :
			return RetCode.ICC_CMD_RR_FAIL;
		case RetCode. ICC_ERR_SEND_IcGenArqc_FAIL:
			return RetCode.ICC_SEND_IcGenArqc_FAIL;
		case RetCode. ICC_ERR_CMD_IcGenArqc_FAIL:
			return RetCode.ICC_CMD_IcGenArqc_FAIL;	
		case RetCode.ICC_ERR_SEND_ICExecute_FAIL  :
			return RetCode.ICC_SEND_ICExecute_FAIL;
		case RetCode.ICC_ERR_SCRIPT_FAIL: 
			return RetCode.ICC_SCRIPT_FAIL;
		case RetCode.ICC_ERR_SEND_ICIssAuth_FAIL  :
			return RetCode.ICC_SEND_ICIssAuth_FAIL;
		case RetCode.ICC_ERR_CMD_ICIssAuth_FAIL:
			return RetCode.ICC_CMD_ICIssAuth_FAIL;
		case RetCode.ICC_ERR_SEND_ICGenTc_FAIL:
			return RetCode.ICC_SEND_ICGenTc_FAIL;
		case RetCode.ICC_ERR_CMD_ICGenTc_FAIL  :
			return RetCode.ICC_CMD_ICGenTc_FAIL;
		case RetCode.ICC_ERR_SEND_ICGenAAC_FAIL   :
			return RetCode.ICC_SEND_ICGenAAC_FAIL;
		case RetCode.ICC_ERR_CMD_ICGenAAC_FAIL :
			return RetCode.ICC_CMD_ICGenAAC_FAIL;	
		case RetCode.ICC_ERR_SEND_ICGpo_FAIL  :
			return RetCode.ICC_SEND_ICGpo_FAIL;
		case RetCode.ICC_ERR_CMD_ICGpo_FAIL: 
			return RetCode.ICC_CMD_ICGpo_FAIL;
		case RetCode.ICC_ERR_SEND_ICSelect_FAIL  :
			return RetCode.ICC_SEND_ICSelect_FAIL;
		case RetCode.ICC_ERR_CMD_ICSelect_FAIL:
			return RetCode.ICC_CMD_ICSelect_FAIL;
		case RetCode.ICC_ERR_SEND_Getdata_FAIL  :
			return RetCode.ICC_SEND_Getdata_FAIL;
		case RetCode.ICC_ERR_CMD_Getdata_FAIL:
			return RetCode.ICC_CMD_Getdata_FAIL;
		case RetCode.ICC_ERR_SEND_APDU_FAIL :
			return RetCode.ICC_SEND_APDU_FAIL;	
		default:
			return RetCode.UNKNOWN_ERR;
		}
	}
	
	/**
	 * 获取IC卡错误消息
	 * @param ret
	 */
	public static String getIccErrorMsg(int ret)
	{
		switch (ret) {
		case RetCode.ICC_ERR_PARAM_ERR:
			return RetCode.PARAM_ERR_MSG;
		case RetCode.ICC_ERR_OPEN_PORT_FAIL:
			return RetCode.OPEN_SERIAL_ERR_MSG;
		case RetCode.ICC_ERR_TIMEOUT_ERR:
			return RetCode.TIMEOUT_ERR_MSG;
		case RetCode.ICC_ERR_CLOSE_PORT_FAIL:
			return RetCode.ICC_CLOSE_PORT_FAIL_MSG;
		case RetCode.ICC_ERR_EX_PORT_FAIL:
			return RetCode.ICC_EX_PORT_FAIL_MSG;
		case RetCode.ICC_ERR_NO_CARD_FAIL:
			return RetCode.ICC_NO_CARD_FAIL_MSG;
			
		case RetCode.ICC_ERR_OTHER_ERR :
			return RetCode.UNKNOWN_ERR_Msg;	
		case RetCode.ICC_ERR_POWER_ON_FAIL :
			return RetCode.ICC_POWER_ON_FAIL_MSG;
		case RetCode.ICC_ERR_POWER_OFF_FAIL: 
			return RetCode.ICC_POWER_OFF_FAIL_MSG;
		case RetCode.ICC_ERR_TOUCH_CARD_OUT:
			return RetCode.ICC_TOUCH_CARD_OUT_MSG;
		case RetCode.ICC_ERR_UNTOUCH_CARD_OUT:
			return RetCode.ICC_UNTOUCH_CARD_OUT_MSG;
		case RetCode.ICC_ERR_UNSUPT_TOUCH_CARD:
			return RetCode.ICC_UNSUPT_TOUCH_CARD_MSG;
		case RetCode.ICC_ERR_TOUCH_CARD_ON:
			return RetCode.ICC_TOUCH_CARD_ON_MSG;
		case RetCode.ICC_ERR_UNTOUCH_OFF:
			return RetCode.ICC_UNTOUCH_OFF_MSG;
		case RetCode.ICC_ERR_UNTOUCH_ON_FAIL:
			return RetCode.ICC_UNTOUCH_ON_FAIL_MSG;
			
		case RetCode.ICC_ERR_GET_APP_FAIL :
			return RetCode.ICC_GET_APP_FAIL_MSG;	
		case RetCode.ICC_ERR_FAIL_EXEC_SRPT  :
			return RetCode.ICC_FAIL_EXEC_SRPT_MSG;
		case RetCode.ICC_ERR_NOLOGENTER_ERR: 
			return RetCode.ICC_NOLOGENTER_ERR_MSG;
		case RetCode.ICC_ERR_CARD_LOCK:
			return RetCode.ICC_CARD_LOCK_MSG;
		case RetCode.ICC_ERR_CARD_LOCK_FOREVER  :
			return RetCode.ICC_CARD_LOCK_FOREVER_MSG;
		case RetCode.ICC_ERR_PARASE_GAC_ERR:
			return RetCode.ICC_PARASE_GAC_ERR_MSG;
		case RetCode.ICC_ERR_GET_CDOL1_FAIL   :
			return RetCode.ICC_GET_CDOL1_FAIL_MSG;
		case RetCode.ICC_ERR_GET_CDOL2_FAIL  :
			return RetCode.ICC_GET_CDOL2_FAIL_MSG;	
		case RetCode.ICC_ERR_GEN_PDOL_FAIL   :
			return RetCode.ICC_GEN_PDOL_FAIL_MSG;
		case RetCode.ICC_ERR_AIDLIST_LOCK :
			return RetCode.ICC_AIDLIST_LOCK_MSG;	
		case RetCode.ICC_ERR_AIDLIST_SEL_FAIL  :
			return RetCode.ICC_AIDLIST_SEL_FAIL_MSG;
		case RetCode.ICC_ERR_GEN_TC_FAIL: 
			return RetCode.ICC_GEN_TC_FAIL_MSG;	
			
			
		case RetCode.ICC_ERR_PARSE_FAIL  :
			return RetCode.ICC_PARSE_FAIL_MSG;
		case RetCode.ICC_ERR_GENLOG_FAIL:
			return RetCode.ICC_GENLOG_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_RR_FAIL   :
			return RetCode.ICC_SEND_RR_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_RR_FAIL  :
			return RetCode.ICC_CMD_RR_FAIL_MSG;
		case RetCode. ICC_ERR_SEND_IcGenArqc_FAIL:
			return RetCode.ICC_SEND_IcGenArqc_FAIL_MSG;
		case RetCode. ICC_ERR_CMD_IcGenArqc_FAIL:
			return RetCode.ICC_CMD_IcGenArqc_FAIL_MSG;	
		case RetCode.ICC_ERR_SEND_ICExecute_FAIL  :
			return RetCode.ICC_SEND_ICExecute_FAIL_MSG;
		case RetCode.ICC_ERR_SCRIPT_FAIL: 
			return RetCode.ICC_SCRIPT_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_ICIssAuth_FAIL  :
			return RetCode.ICC_SEND_ICIssAuth_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_ICIssAuth_FAIL:
			return RetCode.ICC_CMD_ICIssAuth_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_ICGenTc_FAIL:
			return RetCode.ICC_SEND_ICGenTc_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_ICGenTc_FAIL  :
			return RetCode.ICC_CMD_ICGenTc_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_ICGenAAC_FAIL   :
			return RetCode.ICC_SEND_ICGenAAC_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_ICGenAAC_FAIL :
			return RetCode.ICC_CMD_ICGenAAC_FAIL_MSG;	
		case RetCode.ICC_ERR_SEND_ICGpo_FAIL  :
			return RetCode.ICC_SEND_ICGpo_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_ICGpo_FAIL: 
			return RetCode.ICC_CMD_ICGpo_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_ICSelect_FAIL  :
			return RetCode.ICC_SEND_ICSelect_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_ICSelect_FAIL:
			return RetCode.ICC_CMD_ICSelect_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_Getdata_FAIL  :
			return RetCode.ICC_SEND_Getdata_FAIL_MSG;
		case RetCode.ICC_ERR_CMD_Getdata_FAIL:
			return RetCode.ICC_CMD_Getdata_FAIL_MSG;
		case RetCode.ICC_ERR_SEND_APDU_FAIL :
			return RetCode.ICC_SEND_APDU_FAIL_MSG;	
		default:
			return RetCode.UNKNOWN_ERR_Msg;
		}
	}

}
