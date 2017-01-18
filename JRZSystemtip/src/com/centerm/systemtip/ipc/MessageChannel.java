package com.centerm.systemtip.ipc;

import com.centerm.systemtip.MainActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Messenger;
import android.os.Message;
import android.util.Log;

/**
 * ����ȼ����Ϣͨ��, ���õ�һʵ��ʵ��
 */
public class MessageChannel {

	public final static String THREADNAME = MainActivity.TAG + "_MessageChannel";//�߳�����
	public final static String DISPATCHCLS = "com.centerm.dispath.ipc.MessageService";//����ͨ�ŷ�������ȫ�޶���
	private Messenger localMsger;		//Ӧ�ó��򱾵ص�Messenger
	private Messenger remoteMsger;		//���ȳ����Messenger
//	private Handler handler;			//�����handler
	public static Handler handler;
	private HandlerThread thread;		//��Ϣ�����߳�
	private static MessageChannel channel;//��һʵ��
	private int flowNo;					//�����ڲ�Action����ˮ��
	
	private MessageChannel()
	{
		thread = new HandlerThread( THREADNAME );//������Ϣ�����߳�
		thread.start();
		handler = new MessageHandler( thread.getLooper() );
		localMsger = new Messenger( handler );//�������ص�Messenger
	}
	
	public static MessageChannel getInstance()
	{
		if( channel == null )
		{
			channel = new MessageChannel();//��һʵ��
		}
		return channel;
	}
	
	/**
	 * ��������ȵ�ͨ��ͨ��
	 * @param flowNo ����Ҫ���������ˮ��
	 */
	public void createChannel( int flowNo )
	{
		this.flowNo = flowNo;
		Intent intent = new Intent( DISPATCHCLS );
		MainActivity.getProcessContext().bindService( intent, rmServiceConn, Context.BIND_AUTO_CREATE );	
	}
	
	/**
	 * ������Ϣ�����ȳ���
	 * @param msg Ԥ���͵���Ϣ
	 * @return true��ʾ�ɹ���false��ʾʧ��
	 */
	public boolean sendMessage( Message msg )
	{
		try
		{
			msg.replyTo = localMsger;
			remoteMsger.send( msg );
			return true;
		}catch( Exception e )
		{
			//TODO:log
			return false;
		}
	}
	
	//Զ�̷�������
	private ServiceConnection rmServiceConn = new ServiceConnection()
	{
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			remoteMsger = new Messenger( service );
			Log.i( MainActivity.TAG, "Dispatch Service is connected!");
			
			//֪ͨ���ȣ�����ͨ��������messenger���뱾����ͨ��
			Message msg = Message.obtain();
			msg.what = MessageType.MSG_CONTACTME;
			msg.arg1 = MainActivity.ID;
			msg.arg2 = flowNo;
			sendMessage( msg );
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			remoteMsger = null;
		}
	};
}
