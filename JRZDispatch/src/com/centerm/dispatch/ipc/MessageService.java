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
		
	//����������Ը���Ӧ�ó���client��Message��Ȼ��ת����MessageHandler���д���
	final Messenger messenger;		//�����messenger
	final Handler handler;			//�����handler
	
	/*
	 * ���캯��������messager
	 */
	public MessageService()
	{
		super();
		//��handler�󶨵����߳���
		handler = new MessageHandler( DispatchApplication.getProcessContext().getMainLooper());
		messenger = new Messenger( handler );//����messenger		
		
	}

	/**
	 * ÿ��Service��������onBind���Ա����Client��bind����
	 */
	@Override
	public IBinder onBind(Intent intent) 
	{
		Log.v("dis", "bind ok");
		return messenger.getBinder();//ֱ�ӷ���binder�ӿ�
	}
}
