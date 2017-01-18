package com.centerm.dispatch;

import android.app.Activity;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

import com.centerm.androidkill.aidl.RemoteApkOperator;
import com.centerm.common.RWIniClass;
import com.centerm.dispatch.common.MsgListClass;
import com.centerm.dispatch.service.ComunicationService;
import com.centerm.remoteserver.aidl.RemoteInfo;
// com.centerm.dispatch.DispatchApplication
public class DispatchApplication extends Application{

	public static Context context;
	public static final String TAG = "Dispath";				//调度程序的log标签

	public static  MsgListClass msgList = new MsgListClass();   //指令队列

	private MyServiceConnection connection;
	
	private static  RemoteApkOperator remoteApkOperator;
	
	public static String serialNumber=null;
//	public static String t5SericalNumber = null;
	private ApkRemoteConnection apkRemoteConnection;
	private static final String restartAction="com.centerm.dispatch.restart";
	private RemoteInfo remoteInfo;
	public final static String  dispatchTag  = "dispatch";
	public  static final String sericalFile ="/mnt/sdcard/serical.ini";  
	
	public static final String sericalTag = "serical";
	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		context = this;
		 //异常信息捕获
		
		/*
  		Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() 
  		{
  			public void uncaughtException(Thread thread, Throwable ex) 
  			{
  				//任意一个线程异常后统一的处理
  				System.out.println(ex.getLocalizedMessage());
  				Log.e( TAG, "dispatch error..................................");
  			}
  		});*/
	
		apkRemoteConnection = new ApkRemoteConnection();
		apkRemoteConnection.bindService();
		
		StandbyWakeUp standbyWakeUp = new StandbyWakeUp();
		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction(Intent.ACTION_SCREEN_OFF);
		intentFilter.addAction(Intent.ACTION_SCREEN_ON);
		registerReceiver(standbyWakeUp, intentFilter);
	}
	
	public void startService()
	{
		
		//DeviceInfoThread deviceInfoThread = new DeviceInfoThread(this, this);
		//deviceInfoThread.startThread();
		
		
		connection = new MyServiceConnection();
		connection.bindService();
		ComunicationService.startService(DispatchApplication.this);
		
	}
	
	public MsgListClass getMsgList()
	{
		return msgList;
	}
	
	
	public static Context getProcessContext()
	{
		return context;
	}


	
	private class MyServiceConnection implements ServiceConnection{
		private String actionName = "com.centerm.remoteserver.updatestate";
		
		private Handler handler = new Handler()
		{
			
			public void handleMessage(Message msg)
			{
				super.handleMessage(msg);
				doInit();
				Log.e("ser","sericalNumber is "+serialNumber);
				connection.unbindService();
				
			
			}
		};
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
		
			remoteInfo = RemoteInfo.Stub.asInterface(service);
			if(remoteInfo!=null){
				try {
					remoteInfo.startGetDeviceInfo(true);
				} catch (RemoteException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			handler.sendEmptyMessageDelayed(0, 2000);
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			Log.i(TAG, "onServiceDisconnected...");
		}
		
		
		private void bindService()
		{
			Intent intent  = new Intent(actionName);
			intent.setPackage("com.centerm.remoteserver");
			DispatchApplication.this.bindService(intent, this, Context.BIND_AUTO_CREATE);	
		}

		private void unbindService()
		{
			DispatchApplication.this.unbindService(this);
		}
	
	}
	
	public static void sendReboot()
	{
		
		//Intent intent = new Intent("com.centerm.reboot");
		//context.sendBroadcast(intent);
		if(remoteApkOperator!=null){
			try {
				remoteApkOperator.reboot();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	public static void sendRestart()
	{
		//Intent intent = new Intent(restartAction);
		//context.sendBroadcast(intent);
		
		if(remoteApkOperator!=null){
			try {
				remoteApkOperator.restartDispatch();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public static void showTip(String text)
	{
		Toast.makeText(context, text, Toast.LENGTH_LONG).show();
	}
	
	private class ApkRemoteConnection implements ServiceConnection{
		private String actionName = "com.centerm.restart.service";
		
	
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
		
			remoteApkOperator = RemoteApkOperator.Stub.asInterface(service);
			if(remoteApkOperator!=null){
				
			}
			
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			Log.i(TAG, "onServiceDisconnected...");
		}
		
		
		private void bindService()
		{
			Intent intent  = new Intent(actionName);
			intent.setPackage("com.centerm.protect.service");
			DispatchApplication.this.bindService(intent, this, Context.BIND_AUTO_CREATE);	
		}

		private void unbindService()
		{
			DispatchApplication.this.unbindService(this);
		}
	
	}
	
	public void doInit()
	{
		DeviceInfo deviceInfo = new DeviceInfo(DispatchApplication.this);
		SharedPreferences sharedPreferences = getSharedPreferences(dispatchTag, Activity.MODE_PRIVATE);
		serialNumber = sharedPreferences.getString(sericalTag, "");
		
		
		if(serialNumber.length()==0){
		
			serialNumber =deviceInfo.getSerializable();
			SharedPreferences.Editor editor = sharedPreferences.edit(); 
			editor.putString(sericalTag, serialNumber);
			editor.commit();
		}else{
			
		}
		
	}
	
	
	public static void saveSerialNum(String number)
	{
		
		SharedPreferences sharedPreferences = context.getSharedPreferences(dispatchTag, Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPreferences.edit(); 
		serialNumber = number;
		editor.putString(sericalTag, serialNumber);
		editor.commit();
		
	
	}

	
	public static void setSerialNum(String number)
	{
		
		serialNumber = number;
	}

	
}
