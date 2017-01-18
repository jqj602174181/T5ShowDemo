package com.centerm.systemtip;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.Application;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.centerm.systemtip.ipc.MessageChannel;

public class MainActivity extends Activity {

	public static final int STATUS_PROGRAM_UNOPEN = -1;
	public static final int STATUS_PROGRAM_CLOSE = 0;
	public static final int STATUS_PROGRAM_RUN = 1;
	public static final int STATUS_PROGRAM_STOP = 2;
	public static final String TAG = "TIP";					//ϵͳ��ʾ�����log��ǩ
	public static final int ID = 8;							//ϵͳ��ʾ�����id
	private static Application application;					//Ӧ�ó���������
	private static MessageChannel messageChannel = null;	//��Ϣ�ܵ�
	
	public static List<CommonRetActivity> commonRetActivitys = new ArrayList<CommonRetActivity>();
	public static List<CommonActivity> commonActivitys = new ArrayList<CommonActivity>();
	
	/**
	 * ��ȡ������ص������Ķ���
	 * @return ������ص������Ķ���
	 */
	public static Application getProcessContext()
	{
		return application;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		
		//�쳣��Ϣ����
		Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() 
		{
			public void uncaughtException(Thread thread, Throwable ex) 
			{
				//����һ���߳��쳣��ͳһ�Ĵ���
				System.out.println(ex.getLocalizedMessage());
				Log.e( TAG, "System error..................................");
				 android.os.Process.killProcess( android.os.Process.myPid() );
			}
		});
		
		//�����߳���Ϣ����ת����MainLoopHandler���д��� ��������������Ϣ
		if( messageChannel == null )//ֻ����һʵ������
		{
			application = getApplication();//�õ�������ص�������
			
			Intent intent = getIntent();
			int flowNo = intent.getIntExtra( "FlowNO", 1 );
			messageChannel = MessageChannel.getInstance();//����ͨ�Źܵ��߳�
			messageChannel.createChannel( flowNo );//��������ȵ�����
		}
		else//���ȳ���ֻ����һ��ʵ�������У���Ӧ�ó��ֶ������ʵ��Ĵ���
		{
			Log.e( TAG, "Can't create two app instance!" );
		}
		finish();//����activity
	}
}
