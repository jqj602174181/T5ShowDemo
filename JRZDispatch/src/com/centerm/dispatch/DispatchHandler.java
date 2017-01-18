package com.centerm.dispatch;

import com.centerm.dispatch.app.Action;
import com.centerm.dispatch.app.AppManager;
import com.centerm.dispatch.app.KeypadApp;
import com.centerm.dispatch.app.SignatureApp;
import com.centerm.dispatch.app.SystemtipApp;
import com.centerm.dispatch.communication.ComServiceThread;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class DispatchHandler extends Handler {
	//可处理的消息列表
	public final static int MSG_CREATE = 0;		//调度程序创建的消息
	public final static int MSG_DESTORY = 1;	//调度程序销毁退出的消息
	public final static int MSG_COMMAND = 2;	//主机发送来的命令
	private int flowNo = 1;						//各个Action的流水号
	private ComServiceThread comServiceThread = null;
	private DistributeCmd distributeCmd = null;

	private boolean isInit = false;

	public DispatchHandler(Looper looper) {
		super(looper);
	
	}

	public void stopThread()
	{
		if(comServiceThread!=null){
    		comServiceThread.stopThread();
    		comServiceThread = null;
    	}
		
		if(distributeCmd!=null){
    		distributeCmd.stopThread();
    		distributeCmd = null;
    	}
	}
	  /*
		 * 
		 */
	public  void initWait()//线程中调用 ，等待初始化完成
	{
	
		int time = 50;
		while (!isInit) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			time--;
			if(time==0)break;
		}
	}
		
	public void initNotify()
	{
		isInit = true;
	}
	/*
	 * 获取唯一的流水号
	 */
	public int getUniqueFlowNo() {
		return flowNo++;		
	}
	
	@Override
	public void handleMessage(Message msg) 
	{
		switch (msg.what) {
		case MSG_CREATE:			//服务创建
			initApplication();
	    	initService();
			break;
		case MSG_DESTORY:			//调度程序销毁的消息
			System.exit(msg.arg1);	//退出主线程
			break;
		case MSG_COMMAND:			//处理来自主机的命令
			Log.i(MainActivity.TAG, "Start processing MSG_COMMAND message!");
			AppManager.getManager().doAction( (Action)msg.obj );
			break;
		}
	}
	
	
	private void initService()
	{
		new Thread(){
			public void run(){
				
				comServiceThread = ComServiceThread.getInstance();
		    	comServiceThread.start();
		    	comServiceThread.initWait();
		    	distributeCmd = new DistributeCmd(DispatchHandler.this); //创建指令分发线程
				distributeCmd.start();//启动线程
				initNotify();//初始化完成
			}
		}.start();
	}
	/*
	 * 创建程序的各项初始服务，并启动各个应用程序
	 */
	private void initApplication() {
		Message msg;
		Action action;

		//TODO:未来可以考虑采用反射技术将下面的代码采用读配置的方式，动态生成
		//创建各个应用程序。总是先注册app，然后挂载创建的动作	
		AppManager.getManager().registerApp(new KeypadApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), KeypadApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The KeypadApp program started successfully!");
	
		AppManager.getManager().registerApp(new SignatureApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), SignatureApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The SignatureApp program started successfully!");
	
		AppManager.getManager().registerApp(new SystemtipApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), SystemtipApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The SystemtipApp program started successfully!");
	
		/*AppManager.getManager().registerApp(new MediaplayerApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), MediaplayerApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The MediaplayerApp program started successfully!");


		//启动广告播放
		action = new Action(Action.ACTION_START, getUniqueFlowNo(), MediaplayerApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The MediaplayerApp program started successfully!..........................");
		 */
	}
}
