package com.centerm.dispatch.service;

import java.util.List;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.DispatchHandler;
import com.centerm.dispatch.communication.BTAcceptThread;
import com.centerm.dispatch.communication.HIDAcceptThread;
import com.centerm.dispatch.communication.ResultAcceptThread;

import android.app.ActivityManager;
import android.app.Service;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;

public class ComunicationService extends Service {

	private static String ServiceName = "com.centerm.dispatch.service.ComunicationService";
	private static final String TAG = "ComunicationService"; 
	private BTAcceptThread btThread = null;
	private HIDAcceptThread hidThread = null;
	private ResultAcceptThread resultAcceptThread = null;
	private  DispatchHandler dispatchHandler = null;	//分发线程消息处理
	private  HandlerThread dispatchThread = null;		//分发线程
	
	private Handler startHandle = new Handler()
	{

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			//创建HID监听线程
			hidThread = HIDAcceptThread.getInstance();
			hidThread.start();
			
			//创建返回数据监听线程
			resultAcceptThread = ResultAcceptThread.getInstance();
			resultAcceptThread.start();
		}
		
	};
	@Override
	public IBinder onBind(Intent intent) {
		Log.i(TAG, "onBind");
		return null;
	}
	
	@Override
	public void onCreate()
	{
		super.onCreate();
		
		/*
		if(!isServiceRunning(this,"com.example.androidkill.KillService")){
			Intent intent = new Intent();
			intent.setPackage("com.centerm.protect.service");
			intent.setAction("com.centerm.restart.service");
			this.startService(intent);

			Log.e("kill","kill service");
		}
		*/
		//创建业务分发线程
		/*
		createHandlerThread();
		
		//创建蓝牙监听线程
		btThread =  BTAcceptThread.getInstance();
		if (btThread.isAlive() == false) {
			btThread.start();
		}
		
		//创建HID监听线程
		hidThread = HIDAcceptThread.getInstance();
		hidThread.start();
		
		//创建返回数据监听线程
		resultAcceptThread = ResultAcceptThread.getInstance();
		resultAcceptThread.start();
		//开启电池、蓝牙状态检测服务
    	Intent keypadInt = new Intent(DispatchApplication.getProcessContext(), TipFxService.class);
    	DispatchApplication.getProcessContext().startService(keypadInt);
    	*/
		
		//可以在此处加锁防止cpu进入休眠
		
		initThread();
	}

	
	private void initThread()
	{
		new Thread(){
				public void run()
				{
					super.run();
				
					createHandlerThread();
					if(dispatchHandler!=null){
						dispatchHandler.initWait();//等待初始化完成
					}
				
					//创建蓝牙监听线程
					btThread =  BTAcceptThread.getInstance();
					btThread.start();
					
					btThread.waitBluetoothOpen();//等待蓝牙开启成功
					
					//开启电池、蓝牙状态检测服务
			    	Intent keypadInt = new Intent(DispatchApplication.getProcessContext(), TipFxService.class);
			    	DispatchApplication.getProcessContext().startService(keypadInt);
					
			    	
			    	startHandle.sendEmptyMessage(0);
				}
			}.start();
			
			
			//开启通信服务
	  // 	Intent intent = new Intent(DispatchApplication.getProcessContext(), ComunicationService.class);
	  // 	DispatchApplication.getProcessContext().startService(intent);
	}
	public void createHandlerThread()
	{
		if (dispatchHandler == null) {
  			//	application = getApplication();				//得到进程相关的上下文
  				dispatchThread = new HandlerThread(TAG);	//创建消息分发处理线程
  				dispatchThread.start();						//启动线程
  				dispatchHandler = new DispatchHandler(dispatchThread.getLooper());	//将命令分发交给该线程处理
  				
  				//产生创建的消息
  				Message msg = dispatchHandler.obtainMessage(DispatchHandler.MSG_CREATE);
  				dispatchHandler.sendMessage(msg);
  				Log.e(TAG, "dispatch app is running!");			
  			}
  			else {	
  				//调度程序只能有一个实体在运行，不应该出现多个调度实体的存在
  				Log.e( TAG, "Can't create two dispatch app!" );
  				Message msg = dispatchHandler.obtainMessage(DispatchHandler.MSG_CREATE);
  				dispatchHandler.sendMessage(msg);
  			}
		
	}
    
    @Override  
    public void onDestroy() {
    	Log.i(TAG, " ComunicationService onDestroy"); 
    	if(dispatchHandler!=null){
    		dispatchHandler.stopThread();
    	}
    	
    	super.onDestroy();
    }
    
    //服务是否运行
  	private static boolean isServiceRunning(Context context, String serviceName) {
  		boolean isRunning = false;
  		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
  		List<RunningServiceInfo> lists = am.getRunningServices(100);
  		for (RunningServiceInfo info : lists) {//判断服务
  			if(info.service.getClassName().equals(serviceName)){
  				isRunning = true;
  				break;
  			}
  		}
  		
  		return isRunning;
  	}
  	
  	public static void startService(Context context)
  	{
  		boolean is = isServiceRunning(context, ServiceName);
  		if(!is){
  			Intent service = new Intent();
  			service.setClass(context, ComunicationService.class);
  			context.startService(service);
  		}
  	}
}
