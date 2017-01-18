package com.centerm.util.financial;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class IDCardService {
	public static final int MSG_READIDCARD = 0x1001;
	public static final String TAG = "IDCardService";
	static IDCardService instance;

	private int port = 0;
	private int baud = 0;
	private int timeout = 0;
	private boolean isWorking = false;
	private OnReadCardListener rlistener = null;
	PersonInfo perInfo = new PersonInfo();

	public interface OnReadCardListener {
		public void onComplete(PersonInfo oPersonInfo);
		public void onError(String nErrCode, String strMsg);
		public void onCancel();
	}

	private IDCardService()
	{	
	}

	/*!
	 * \brief ��ȡ����֤serviceʵ��
	 * \return serviceʵ��
	 */
	public static IDCardService getInstance()
	{
		if( instance == null )
		{
			instance = new IDCardService();
		}
		return instance;
	}

	public void setComParams(int nPort, int nBaud)
	{
		this.port = nPort;
		this.baud = nBaud;
	}

	//	public boolean setBaud(int baud, int setBaud)
	//	{
	//		int ret = jniSetComBaud(port,baud,setBaud);
	//		Log.e("ret"," baud is ret is "+baud+" setBaud is "+setBaud+" ret is "+ret);
	//
	//		if(ret==0){
	//			return true;
	//		}
	//
	//		return false;
	//	}

	public void setTimeOut(int timeout)
	{
		this.timeout = timeout;
	}

	static 
	{
		try {
			//���ض�̬��
			System.loadLibrary( "idcaread" );
			System.loadLibrary( "idcard" );
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void readCard(OnReadCardListener rls)
	{
		Log.e(TAG, "readCard");
		if (isWorking)
		{
			Log.e(TAG, "readcard fail" + isWorking);
			return;
		}
		this.rlistener = rls;
		this.isWorking = true;

		new Thread(new Runnable() {
			@Override
			public void run() {
				int ret = ReadIDCard( port, baud, timeout, perInfo);
				Log.e(TAG, "ret:"+ret);

				Bundle bundle = new Bundle();
				bundle.putInt("result", ret);
				Message msg = new Message();
				msg.what = MSG_READIDCARD; 
				msg.setData(bundle);
				mHandler.sendMessage(msg);	

				Log.e(TAG, "sendMessage:");
			}
		}).start();
	}

	/*********************�ڲ�˽�з�������*************************/
	//��Ϣ����
	private Handler mHandler = new Handler(Looper.getMainLooper()) {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			Bundle bundle = msg.getData();
			int ret = 0;
			switch (msg.what) {
			case MSG_READIDCARD:
				if (rlistener == null)
				{
					Log.d(TAG, "rlistener is null");
					break;
				}

				ret = bundle.getInt("result");
				Log.d(TAG, "read card result:"+ret);
				if( ret == 0)
				{
					rlistener.onComplete(perInfo);
				}
				else if( ret == -7)
				{
					rlistener.onCancel();
				}
				else
				{
					rlistener.onError(MsgTranslateUtil.getIdcErrorCode(ret), 
							MsgTranslateUtil.getIdcErrorMsg(ret));
				}
				rlistener = null;
				break;
			default:
				break;
			}
			isWorking = false;
		}
	};

	/*********************��̬���з�������*************************/
	/*!
	 * \brief ������֤
	 * \param [in] port - ���ں�
	 * \param [in] baud - ������
	 * \param [in] timeout - ��ʱʱ��
	 * \param [in] nInfoLen - ���֤��Ϣ����������
	 * \param [out] byPersonInfo - ���֤�������ṹ��
	 * \return ״̬��
	 */
	private native int ReadIDCard(int port, int baud, int timeout, PersonInfo oPersonInfo );
	private native void CancelRead();

	//	private native int readIDCard(int port, int baud, int timeout, PersonInfo oPersonInfo );
	//	private native void cancelRead();
	//	private native int readIDCardData(int port, int baud, int timeout,byte[] data);
	//	private native int readIDCardData1(int port, int baud, int timeout,byte[] data);
	//	private native int jniSetComBaud(int port, int baud, int setBaud);
}
