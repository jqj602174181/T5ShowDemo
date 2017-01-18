package com.centerm.util.financial;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

/*!
 * @brief �ſ�������
 * @par ˵��:������Ҫ���ṩ�Դſ��Ĳ�������һʵ���ࡣ
 */
public class MsgCardService {
	public static final int MSG_READCARD = 0x1001;
	public static final int MSG_WRITECARD = 0x1002;
	private static final String TAG = "MsgCardService";
	static MsgCardService instance;
	private int port = 0;
	private int baud = 0;
	private int timeout = 0;
	private boolean isWorking = false;
	private OnReadCardListener rlistener = null;
	private OnWriteCardListener wlistener = null;
	
	public interface OnReadCardListener {
		public void onComplete(byte[] byTrack1, byte[] byTrack2, byte[] byTrack3);
		public void onError(String nErrCode, String strMsg);
		public void onCancel();
	}
	
	public interface OnWriteCardListener {
		public void onWriteCardSuc();
		public void onError(int errId);
	}
	
	private MsgCardService()
	{
	}
	
	/*!
	 * @brief ��ȡ�ſ�serviceʵ��
	 * @return serviceʵ��
	 */
	public static MsgCardService getInstance()
	{
		if( instance == null )
		{
			instance = new MsgCardService();
		}
		return instance;
	}
	
	public void cancelRead() {
		cancelReadMsgCard();
	}
	
	/**
	 * @author Fly
	 * @param Looper.myLooper()ȷ�����е���Ϣ����Ϊ��ǰ���̶���
	 * @since 2015-08-01 
	 * �����̷߳���������Ϣ���ݣ����ݽ�������Ϣ���ݽ��в�ͬ�ķ����ص�
	 * */
	Handler mHandler = new Handler(Looper.getMainLooper()) {
		@Override
		public void handleMessage(Message msg) {
			Bundle bundle = msg.getData();
			int ret = 0;
			
			switch (msg.what) {
			case MSG_READCARD:
				if (rlistener == null) {
					Log.d("MagCardService", "rlistener is null");
					return;
				}
				byte[] byTrack1 = bundle.getByteArray("track1");
				byte[] byTrack2 = bundle.getByteArray("track2");
				byte[] byTrack3 = bundle.getByteArray("track3");
				ret = bundle.getInt("result");
				Log.d("MagCardService", "read card result:"+ret);
				if (ret == -7) {
					rlistener.onCancel();
				} else if (ret != 0 && ret != -7) {
					rlistener.onError(MsgTranslateUtil.getRdcErrorCode(ret), 
							MsgTranslateUtil.getRdcErrorMsg(ret));
				} else {
					rlistener.onComplete(byTrack1, byTrack2, byTrack3);
				}
				rlistener = null;
				break;
			case MSG_WRITECARD:
				break;
			default:
				break;
			}
			isWorking = false;
		}
	};
	
	//��������۴ŵ����߳�
	Runnable getBookAcctRunnable = new Runnable() {
		@Override
		public void run() {
			Log.e(TAG, "getBookAcctRunnable");
			Log.e(TAG, "timeout"+ String.valueOf(timeout));
			
			byte[] byTrack1 = new byte[256];
			byte[] byTrack2 = new byte[256];
			byte[] byTrack3 = new byte[256];
			int ret = readMsgCard(port, baud, timeout,
					byTrack1, 0,
					byTrack2, byTrack2.length,
					byTrack3, byTrack3.length);
			
			Log.e(TAG, "ret:"+ret);
			String track2 = new String(byTrack2);
			Log.e(TAG, "track2:" + track2.trim());
			
			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			bundle.putByteArray("track1", byTrack1);
			bundle.putByteArray("track2", byTrack2);
			bundle.putByteArray("track3", byTrack3);
			
			Message msg = new Message();
			msg.what = MSG_READCARD;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};
	
	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�����������۴ŵ�
	 * */
	public void getBookAcct(OnReadCardListener rls)
	{
		if(isWorking)
		{
			Log.e(TAG, "isWorking:"+isWorking);
			return;
		}
		this.rlistener = rls;
		this.isWorking = true;
		new Thread(getBookAcctRunnable).start();
	}
	
	public void setComParams(int nPort, int nBaud)
	{
		this.port = nPort;
		this.baud = nBaud;
	}
	
	public void setTimeout(int timeout)
	{
		this.timeout = timeout;
	}

	/*********************��̬���з�������*************************/
	static
    {
		try {
			System.loadLibrary("MsgCard");
		} catch (Exception e) {
			e.printStackTrace();
		}
    }

    native public int readMsgCard( int nComNo, int dwBaud, int nTimeout, byte[] szTrack1, int nTrack1Len,  byte[] szTrack2, int nTrack2Len,  byte[] szTrack3, int nTrack3Len );
    native public int writeMsgCard( int nComNo, int dwBaud, int nTimeout, String szTrack1, String szTrack2, String szTrack3  );
    native public int cancelReadMsgCard();
}
