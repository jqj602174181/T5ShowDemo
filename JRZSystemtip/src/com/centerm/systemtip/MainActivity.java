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
	public static final String TAG = "TIP";					//系统提示程序的log标签
	public static final int ID = 8;							//系统提示程序的id
	private static Application application;					//应用程序上下文
	private static MessageChannel messageChannel = null;	//消息管道
	
	public static List<CommonRetActivity> commonRetActivitys = new ArrayList<CommonRetActivity>();
	public static List<CommonActivity> commonActivitys = new ArrayList<CommonActivity>();
	
	/**
	 * 获取进程相关的上下文对象
	 * @return 进程相关的上下文对象
	 */
	public static Application getProcessContext()
	{
		return application;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		
		//异常信息捕获
		Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() 
		{
			public void uncaughtException(Thread thread, Throwable ex) 
			{
				//任意一个线程异常后统一的处理
				System.out.println(ex.getLocalizedMessage());
				Log.e( TAG, "System error..................................");
				 android.os.Process.killProcess( android.os.Process.myPid() );
			}
		});
		
		//将主线程消息处理转交给MainLoopHandler进行处理， 并产生创建的消息
		if( messageChannel == null )//只允许单一实例存在
		{
			application = getApplication();//得到进程相关的上下文
			
			Intent intent = getIntent();
			int flowNo = intent.getIntExtra( "FlowNO", 1 );
			messageChannel = MessageChannel.getInstance();//创建通信管道线程
			messageChannel.createChannel( flowNo );//建立与调度的连接
		}
		else//调度程序只能有一个实体在运行，不应该出现多个调度实体的存在
		{
			Log.e( TAG, "Can't create two app instance!" );
		}
		finish();//结束activity
	}
}
