package com.centerm.systemtip;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.centerm.systemtip.common.SendMsgClass;
import com.centerm.systemtip.ipc.MessageChannel;
import com.centerm.systemtip.ipc.MessageHandler;
import com.centerm.systemtip.ipc.MessageType;

/**
 * ����Activity�ĸ��࣬��װ����Ĺ�ͬԪ�غͲ���
 *
 */
public abstract class CommonRetActivity extends Activity
{
	private final static int MSG_CLOSE_SYSTEMTIP = 1109;//����ϵͳ��ʾ	
	
	//�ɹ�����ֱ���޸ĵĲ���
	public int nLinkType = 0;               //���������ӽӿ�
	public boolean enableKeyEvent = false;   //�Ƿ���������Ӧ
	
	//�������಻�������޸�
	private  boolean bCloseFlag = false;	    //�Ƿ����ڹرս���
	protected boolean isSendSuccess = true;//�Ƿ��������ɹ���������0x02,0xAA,0x03
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		MainActivity.commonRetActivitys.add(this);
		
		nLinkType = getIntent().getIntExtra("linktype", -1);//��ȡ������������
		isSendSuccess = getIntent().getBooleanExtra("isSend", true);
		
		if(isSendSuccess){
			byte[] retdata = {0x02, (byte)0xAA, 0x03};
			Log.i("CommonRet",new String(retdata));
			sendMsgToMsgHandler(MessageHandler.MSG_START, retdata);//������Ϣ�����ȣ�֪ͨ��״̬�����˱仯
		}
		
	}
	
	//�ú��������ж�Ӧ�ó�����ռ�ڴ��Ƿ񵽴�һ���̶ȣ��絽��һ���̶���رոý��̡�
	private void checkMemoryOutAndClose()
	{
		int pids[] = { android.os.Process.myPid() };
		System.out.println( "Keypad pids="+ pids[0] );
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);  
		android.os.Debug.MemoryInfo[] memoryInfoArray = am.getProcessMemoryInfo( pids );
		android.os.Debug.MemoryInfo pidMemoryInfo = memoryInfoArray[0];

		System.out.println( "Keypad pidMemoryInfo.getTotalPss()" + pidMemoryInfo.getTotalPss() );
		if (pidMemoryInfo.getTotalPss() > 50000)
		{
			 System.out.println( "killed!");
			 android.os.Process.killProcess( android.os.Process.myPid() );
		}
	}
	
	@Override
	protected void onDestroy()
	{
		enableKeyEvent = false;//����������Ӧ������
		
		//�ر��������
		if( bCloseFlag )
		{
		//	byte[] retdata = {0x02, (byte)0xAA, 0x03};
		//	sendMsgToMsgHandler(MessageHandler.MSG_CLOSE, retdata);
		}
		super.onDestroy();
		checkMemoryOutAndClose();
	}
	

	/**
	 * ������Ϣ�����ȣ�֪ͨ��״̬�����˱仯
	 * @param msgWhat
	 */
	private void sendMsgToMsgHandler(int msgWhat, byte[] data)
	{
		if(MessageChannel.handler != null){
			Message msgTmp = MessageChannel.handler.obtainMessage(msgWhat);
			Bundle bundle = new Bundle();// �������bundle����Ϊobj���Բ��ܿ����
			bundle.putInt("linktype", nLinkType);
			bundle.putByteArray("data", data);
			msgTmp.setData(bundle);
			msgTmp.sendToTarget();
		}
	}
	
	//���ڴ���رս������Ϣ����
	@SuppressLint("HandlerLeak")
	private Handler mHandler = new Handler() 
	{
		@Override
		public void handleMessage(Message msg) 
		{
			if (msg.what == MSG_CLOSE_SYSTEMTIP )
			{
				bCloseFlag = true;
				finish();
			}
			super.handleMessage(msg);
		}
	};
	
	/**
	 * ������Ϣ��������
	 */
	public void closeSystemtip()
	{
		Message closeMsg = mHandler.obtainMessage(MSG_CLOSE_SYSTEMTIP);
		closeMsg.sendToTarget();
	}
	
	/**
	 * ���ͽ���������͵���
	 * @param retData
	 */
	public void sendResultToDispatch( byte[] retData )
	{
		SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_OPERATOR_END, 
				MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, retData );
	}
}