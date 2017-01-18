package com.centerm.util.financial;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class FingerPrintService {
	public static final String TAG = "FingerPrintService";
	private static FingerPrintService instance = null;
	private OnReadCardListener onReadCardListener = null;
	private boolean isWorking = false;
	private String companyCode;
	private int timeout;
	private byte[] finger = null;
	private byte[] fingerModel = null;
	
	private FingerPrintService()
	{
	}

	public interface OnReadCardListener {
		public void onComplete(byte[] msg);
		public void onError(String nErrCode, String strMsg);
		public void onCancel();
	}
	/**
	 * @brief ��ȡָ���ǿ�FingerServiceʵ��
	 * @return FingerServiceʵ��
	 */
	public static FingerPrintService getInstance()
	{
		if( instance == null )
		{
			instance = new FingerPrintService();
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
			Log.d(TAG, "read finger result:"+ret);
			if(ret == -7) {
				onReadCardListener.onCancel();
			} else if (ret > 0) {
				switch (msg.what) {
				case MesDefUtil.registerFinger: {
					onReadCardListener.onComplete(fingerModel);
					onReadCardListener = null;
					break;
				}
				case MesDefUtil.readFinger: {
					onReadCardListener.onComplete(finger);
					onReadCardListener = null;
					break;
				}
				default:
					break;
				}
			} else {
				onReadCardListener.onError(MsgTranslateUtil.getFigErrorCode(ret), MsgTranslateUtil.getFigErrorMsg(ret));
			}
			
			isWorking = false;
		}
	};
	
	//����ָ��ģ��Ǽǵ��߳�
	Runnable registerFingerRunnable = new Runnable() {
		@Override
		public void run() {
			fingerModel = new byte[2048];
			int ret = getFingerPrintEnroll(2, 9600, timeout, fingerModel.length, fingerModel);
			Log.i(TAG, "ret:"+ret);
			
			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			bundle.putByteArray("finger", fingerModel);
			Message msg = new Message();
			msg.what = MesDefUtil.registerFinger;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};
	
	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ�������ָ��ģ��Ǽ�
	 * */
	public void registerFinger(OnReadCardListener onReadCardListener)
	{
		if (isWorking)
		{
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(registerFingerRunnable).start();
	}
	
	//�����ȡָ���������߳�
	Runnable readFingerRunnable = new Runnable() {
		@Override
		public void run() {
			finger = new byte[2048];
			int ret = getFingerPrintCharacter(2, 9600, timeout, finger.length, finger);
			Log.i(TAG, "ret:"+ret);
			
			Bundle bundle = new Bundle();
			bundle.putInt("result", ret);
			bundle.putByteArray("finger", finger);
			Message msg = new Message();
			msg.what = MesDefUtil.readFinger;
			msg.setData(bundle);
			mHandler.sendMessage(msg);
		}
	};
	
	/**
	 * @author FLy
	 * @param onReadCardListener:OnReadCardListener ��������������ã����ص�ʹ��
	 * @return void
	 * ͨ����������������ã���Handler�е��õ����߶���ļ������ķ��������ȡָ������
	 **/
	public void readFinger(OnReadCardListener onReadCardListener)
	{
		if (isWorking)
		{
			return;
		}
		this.onReadCardListener = onReadCardListener;
		this.isWorking = true;
		new Thread(readFingerRunnable).start();
	}

	public void setCompanyCode(String companyCode) {
		this.companyCode = companyCode;
	}

	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}

	static 
    {
		//���ض�̬��
		try {
			System.loadLibrary("FingerPrint");
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
	
	native public int getFingerPrintCharacter( int nComNo, int dwBaud, int nTimeout, int nCharacterLen, byte[] pszCharacter);
	native public int getFingerPrintEnroll( int nComNo, int dwBaud, int nTimeout, int nFingerTemplateLen, byte[] pszFingerTemplate);
	native public int getFingerDeviceInfo( int nComNo, int dwBaud, int nTimeout, int nFingerTemplateLen, byte[] pszFingerTemplate);
}
