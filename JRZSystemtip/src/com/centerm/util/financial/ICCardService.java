package com.centerm.util.financial;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class ICCardService {
	public static final String TAG = "ICCardService";
	private static ICCardService instance = null;
	private OnReadCardListener onReadCardListener = null;
	private boolean isWorking = false;
	private int iIcFlag;
	private String aryTagList;
	private String strAIDList;
	private int timeout;
	private String arqcInput;

	private byte[] szICCardInfo = null;
	private byte[] pszTxDetail = null;
	private byte[] szARQC = null;
	private byte[] pszTC = null;
	private byte[] pszScriptResult = null;
	private byte[] icType = null;

	private ICCardService()
	{
	}

	public interface OnReadCardListener {
		public void onComplete(byte[] msg, int iIcFlag);
		public void onComplete(byte[] msgOne, byte[] msgTwo, int iIcFlag);
		public void onComplete();
		public void onError(String nErrCode, String strMsg);
		public void onCancel();
	}

	/**
	 * @brief 获取IC卡ICCardService实例
	 * @return ICCardService实例
	 */
	public static ICCardService getInstance()
	{
		if( instance == null )
		{
			instance = new ICCardService();
		}
		return instance;
	}

	/**
	 * @author Fly
	 * @param Looper.myLooper()确定运行的消息队列为当前进程队列
	 * @since 2015-08-01 
	 * 处理线程发送来的消息内容，根据解析的消息内容进行不同的方法回调
	 * */
	private Handler mHandler = new Handler(Looper.getMainLooper()) {
		@Override
		public void handleMessage(Message msg) {
			if (onReadCardListener == null)
			{
				isWorking = false;
				return;
			}
			Bundle bundle = msg.getData();
			int ret = 0;
			ret = bundle.getInt("result");
			Log.d(TAG, "read card result:"+ret);
			if (ret == -7) {
				onReadCardListener.onCancel();
			} else if (ret != 0 && ret != -7) {
				onReadCardListener.onError(MsgTranslateUtil.getIccErrorCode(ret), 
						MsgTranslateUtil.getIccErrorMsg(ret));
			} else {
				int typeTmp = 0;
				if (icType != null) {
					if (icType[0] == (byte)0x31) {
						typeTmp = 1;//接
					}
					if (icType[0] == (byte)0x32) {
						typeTmp = 2;//非接
					}
				}
				Log.e("iccardservice", "typeTmp:"+String.valueOf(typeTmp));
				switch (msg.what) {
				case MesDefUtil.getICCardInfo: {
					if( ret == 0)
					{
						onReadCardListener.onComplete(szICCardInfo, typeTmp);
					} 
					onReadCardListener = null;
					break;
				}
				case MesDefUtil.genARQC: {
					if( ret == 0)
					{
						onReadCardListener.onComplete(szARQC, typeTmp);
					}
					onReadCardListener = null;
					break;
				}
				case MesDefUtil.ARPC_ExeICScript: {
					if( ret == 0)
					{
						onReadCardListener.onComplete(pszTC, pszScriptResult, typeTmp);
					}
					onReadCardListener = null;
					break;
				}
				case MesDefUtil.getTxDetail: {
					if( ret == 0)
					{
						onReadCardListener.onComplete(pszTxDetail, typeTmp);
					}
					onReadCardListener = null;
					break;
				}
				case MesDefUtil.getCardRdWrtCap: {
					if( ret == 0)
					{
						onReadCardListener.onComplete(bundle.getString("cap").getBytes(), typeTmp);
					}
					onReadCardListener = null;
					break;
				}
				default:
					break;
				}
			}
			isWorking = false;
		}
	};

	//定义获取IC用户信息的线程
	Runnable getICCardInfoRunnable = new Runnable() {
		@Override
		public void run() {
			szICCardInfo = new byte[2048];
			icType = new byte[16];
			int ret = getICCardInfo(2, iIcFlag, timeout, aryTagList, strAIDList, szICCardInfo, icType);

			Log.e(TAG, "ictype:" + new String(icType));//XXX
			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			Message msg = new Message();
			msg.what = MesDefUtil.getICCardInfo;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};

	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener 传入监听器的引用，供回调使用
	 * @return void
	 * 通过传入监听器的引用，在Handler中调用调用者定义的监听器的方法处理得到的IC卡用户信息
	 * */
	public void getICCardInfo(OnReadCardListener onReadCardListener)
	{
		if (isWorking)
		{
			Log.e(TAG, "readcard fail " + isWorking);
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(getICCardInfoRunnable).start();
	}

	//定义获取IC卡ARQC值的线程
	Runnable genARQCRunnable = new Runnable() {
		@Override
		public void run() {
			szARQC = new byte[2048];
			icType = new byte[16];
			int ret = generateARQC(2, iIcFlag, timeout, strAIDList, arqcInput, szARQC, icType);

			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			Message msg = new Message();
			msg.what = MesDefUtil.genARQC;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};

	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener 传入监听器的引用，供回调使用
	 * @return void
	 * 通过传入监听器的引用，在Handler中调用调用者定义的监听器的方法处理得到的IC卡ARQC值
	 * */
	public void genARQC(OnReadCardListener onReadCardListener) {
		if (isWorking)
		{
			Log.e(TAG, "genARQC fail" + isWorking);
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(genARQCRunnable).start();
	}

	//定义向IC卡发送ARPC，发送写卡脚本的线程
	Runnable ARPC_ExeICScriptRunnable = new Runnable() {
		@Override
		public void run() {
			pszTC = new byte[1024];
			pszScriptResult = new byte[1024];
			icType = new byte[16];
			int ret = writeCard(2, iIcFlag, timeout, "000012", "000012", pszTC, 
					pszScriptResult, icType);
			Log.e(TAG, "ret:"+ret);

			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			Message msg = new Message();
			msg.what = MesDefUtil.ARPC_ExeICScript;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};

	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener 传入监听器的引用，供回调使用
	 * @return void
	 * 通过传入监听器的引用，在Handler中调用调用者定义的监听器的方法处理"向IC卡发送ARPC，发送写卡脚本"
	 * */
	public void ARPC_ExeICScript(OnReadCardListener onReadCardListener) {
		if (isWorking)
		{
			Log.e(TAG, "ARPC_ExeICScript fail" + isWorking);
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(ARPC_ExeICScriptRunnable).start();
	}

	//定义读IC卡交易明细的线程
	Runnable getTxDetailRunnable = new Runnable() {
		@Override
		public void run() {
			pszTxDetail = new byte[2048];
			icType = new byte[16];
			int ret = getTransActionLog(2, iIcFlag, timeout, strAIDList, pszTxDetail, icType);

			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			Message msg = new Message();
			msg.what = MesDefUtil.getTxDetail;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};

	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener 传入监听器的引用，供回调使用
	 * @return void
	 * 通过传入监听器的引用，在Handler中调用调用者定义的监听器的方法读IC卡交易明细
	 * */
	public void getTxDetail(OnReadCardListener onReadCardListener) {
		if (isWorking)
		{
			Log.e(TAG, "getTxDetail fail" + isWorking);
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(getTxDetailRunnable).start();
	}

	//定义获取读写器能力信息的线程
	Runnable getCardRdWrtCapRunnable = new Runnable() {
		@Override
		public void run() {
			int ret = 0;
			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			bundle.putString("cap", "6");
			Message msg = new Message();
			msg.what = MesDefUtil.getCardRdWrtCap;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};

	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener 传入监听器的引用，供回调使用
	 * @return void
	 * 通过传入监听器的引用，在Handler中调用调用者定义的监听器的方法获取读写器能力信息
	 * */
	public void getCardRdWrtCap(OnReadCardListener onReadCardListener) {
		if (isWorking)
		{
			Log.i(TAG, "getCardRdWrtCap fail" + isWorking);
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(getCardRdWrtCapRunnable).start();
	}

	public void setiIcFlag(int iIcFlag) {
		this.iIcFlag = iIcFlag;
	}

	public void setAryTagList(String aryTagList) {
		this.aryTagList = aryTagList;
	}

	public void setStrAIDList(String strAIDList) {
		this.strAIDList = strAIDList;
	}

	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}
	public void setArqcInput(String arqcInput) {
		this.arqcInput = arqcInput;
	}

	static 
	{
		try {
			//加载动态库
			System.loadLibrary("ICCard");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public int getICCAndMSardStatus(int timeOut, byte[] szMsgData)
	{
		int ret = getICCAndMSardStatus(2, timeOut, szMsgData);
		if(ret==1||ret==2){
			iIcFlag = ret;
		}else if(ret<=0){

		}
		return ret;
	}

	/*!
	 * \brief 获取卡信息
	 * \param [in] portType - 端口类型(C-串口, U-USB)C
	 * \param [in] portNo - 端口号1
	 * \param [in] tagList - 卡标志列表
	 * \param [in] AIDList - 应用列表
	 * \param [out] userInfo - 卡内信息
	 * \param [out] icType - IC卡标志   0x32：非接      0x31：接触 
	 * \param [out] lpicappdata - 全局状态参数
	 * \param [out] errMsg - 外设操作错误信息
	 * \return 状态码
	 */
	native public int getICCardInfo( int nComNo, int iIcFlag, int timeout, String szTagList, String szAidList, 
			byte[]szIccardInfo, byte[] icType);
	native public int generateARQC(  int nComNo, int iIcFlag, int timeout, String szAidList, String szTxData, 
			byte[] szARQC,  byte[] icType);
	native public int writeCard( int nComNo, int iIcFlag, int timeout, String pszTxData, String pszARPC, 
			byte[]pszTC, byte[]pszScriptResult,  byte[] icType);
	native public int getTransActionLog( int nComNo, int iIcFlag, int timeout, String szAidList, 
			byte[] pszTxDetail,  byte[] icType);

	public native int getICCAndMSardStatus(int nComNo,  int nTimeOut, byte[] szMsgData);

}
