package com.centerm.dispatch.ipc;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Messenger;
import android.util.Log;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;

public class MessageService extends Service
{
		
	//负责接收来自各个应用程序client的Message，然后转发给MessageHandler进行处理
	final Messenger messenger;		//服务的messenger
	final Handler handler;			//服务的handler
	
	/*
	 * 构造函数，创建messager
	 */
	public MessageService()
	{
		super();
		//将handler绑定到主线程中
		handler = new MessageHandler( DispatchApplication.getProcessContext().getMainLooper());
		messenger = new Messenger( handler );//创建messenger		
		
	}

	/**
	 * 每个Service必须重载onBind，以便接受Client的bind请求
	 */
	@Override
	public IBinder onBind(Intent intent) 
	{
		Log.v("dis", "bind ok");
		return messenger.getBinder();//直接返回binder接口
	}
}
