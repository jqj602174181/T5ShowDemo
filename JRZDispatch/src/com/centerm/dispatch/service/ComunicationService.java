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
	private  DispatchHandler dispatchHandler = null;	//�ַ��߳���Ϣ����
	private  HandlerThread dispatchThread = null;		//�ַ��߳�
	
	private Handler startHandle = new Handler()
	{

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			//����HID�����߳�
			hidThread = HIDAcceptThread.getInstance();
			hidThread.start();
			
			//�����������ݼ����߳�
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
		//����ҵ��ַ��߳�
		/*
		createHandlerThread();
		
		//�������������߳�
		btThread =  BTAcceptThread.getInstance();
		if (btThread.isAlive() == false) {
			btThread.start();
		}
		
		//����HID�����߳�
		hidThread = HIDAcceptThread.getInstance();
		hidThread.start();
		
		//�����������ݼ����߳�
		resultAcceptThread = ResultAcceptThread.getInstance();
		resultAcceptThread.start();
		//������ء�����״̬������
    	Intent keypadInt = new Intent(DispatchApplication.getProcessContext(), TipFxService.class);
    	DispatchApplication.getProcessContext().startService(keypadInt);
    	*/
		
		//�����ڴ˴�������ֹcpu��������
		
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
						dispatchHandler.initWait();//�ȴ���ʼ�����
					}
				
					//�������������߳�
					btThread =  BTAcceptThread.getInstance();
					btThread.start();
					
					btThread.waitBluetoothOpen();//�ȴ����������ɹ�
					
					//������ء�����״̬������
			    	Intent keypadInt = new Intent(DispatchApplication.getProcessContext(), TipFxService.class);
			    	DispatchApplication.getProcessContext().startService(keypadInt);
					
			    	
			    	startHandle.sendEmptyMessage(0);
				}
			}.start();
			
			
			//����ͨ�ŷ���
	  // 	Intent intent = new Intent(DispatchApplication.getProcessContext(), ComunicationService.class);
	  // 	DispatchApplication.getProcessContext().startService(intent);
	}
	public void createHandlerThread()
	{
		if (dispatchHandler == null) {
  			//	application = getApplication();				//�õ�������ص�������
  				dispatchThread = new HandlerThread(TAG);	//������Ϣ�ַ������߳�
  				dispatchThread.start();						//�����߳�
  				dispatchHandler = new DispatchHandler(dispatchThread.getLooper());	//������ַ��������̴߳���
  				
  				//������������Ϣ
  				Message msg = dispatchHandler.obtainMessage(DispatchHandler.MSG_CREATE);
  				dispatchHandler.sendMessage(msg);
  				Log.e(TAG, "dispatch app is running!");			
  			}
  			else {	
  				//���ȳ���ֻ����һ��ʵ�������У���Ӧ�ó��ֶ������ʵ��Ĵ���
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
    
    //�����Ƿ�����
  	private static boolean isServiceRunning(Context context, String serviceName) {
  		boolean isRunning = false;
  		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
  		List<RunningServiceInfo> lists = am.getRunningServices(100);
  		for (RunningServiceInfo info : lists) {//�жϷ���
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
