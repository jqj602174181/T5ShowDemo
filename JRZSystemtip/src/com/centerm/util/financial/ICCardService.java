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
	 * @brief ��ȡIC��ICCardServiceʵ��
	 * @return ICCardServiceʵ��
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
	 * @param Looper.myLooper()ȷ�����е���Ϣ����Ϊ��ǰ���̶���
	 * @since 2015-08-01 
	 * �����̷߳���������Ϣ���ݣ����ݽ�������Ϣ���ݽ��в�ͬ�ķ����ص�
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
						typeTmp = 1;//��
					}
					if (icType[0] == (byte)0x32) {
						typeTmp = 2;//�ǽ�
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

	//�����ȡIC�û���Ϣ���߳�
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
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�������õ���IC���û���Ϣ
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

	//�����ȡIC��ARQCֵ���߳�
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
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�������õ���IC��ARQCֵ
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

	//������IC������ARPC������д���ű����߳�
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
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�������"��IC������ARPC������д���ű�"
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

	//�����IC��������ϸ���߳�
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
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�����IC��������ϸ
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

	//�����ȡ��д��������Ϣ���߳�
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
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�����ȡ��д��������Ϣ
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
			//���ض�̬��
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
	 * \brief ��ȡ����Ϣ
	 * \param [in] portType - �˿�����(C-����, U-USB)C
	 * \param [in] portNo - �˿ں�1
	 * \param [in] tagList - ����־�б�
	 * \param [in] AIDList - Ӧ���б�
	 * \param [out] userInfo - ������Ϣ
	 * \param [out] icType - IC����־   0x32���ǽ�      0x31���Ӵ� 
	 * \param [out] lpicappdata - ȫ��״̬����
	 * \param [out] errMsg - �������������Ϣ
	 * \return ״̬��
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
