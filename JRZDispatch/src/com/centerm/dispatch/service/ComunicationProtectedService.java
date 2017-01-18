package com.centerm.dispatch.service;

import java.util.List;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;

import android.app.ActivityManager;
import android.app.Service;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class ComunicationProtectedService extends Service {
	private static final String TAG = "ComunicationProtectedService"; 
	
	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}

	@Override
	public void onCreate()
	{
		super.onCreate();
		new Thread() {
			public void run() {
				while (true) {
					boolean isRun = isServiceRunning(getApplication(),"com.centerm.dispatch.service.ComunicationService");
					if (isRun == false){
						Log.i(TAG, "������������ComunicationService: " + ComunicationProtectedService.class);
						Intent intent = new Intent(ComunicationProtectedService.this, ComunicationService.class);
						DispatchApplication.getProcessContext().startService(intent);
					}
					
					/*boolean isKeypadRun = isServiceRunning(MainActivity.getProcessContext(),"com.centerm.dispatch.service.KeypadFxService");
					if (isKeypadRun == false) {
						Log.i(TAG, "������������KeypadFxService: " + KeypadFxService.class);
						Intent intent = new Intent(ComunicationProtectedService.this, KeypadFxService.class);
						MainActivity.getProcessContext().startService(intent);
					}*/
					
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			};
		}.start();
	}

	/**
	 * ����startServiceʱ���÷����Զ�ִ��  
	 */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {  
        Log.i(TAG, "onStartCommand");  
        return START_STICKY;  
    }
    
    @Override  
    public void onDestroy() { 
    	Log.e(TAG, "onDestroy"); 
    	//��Service��onDestroy()������Servic
    	Intent sevice = new Intent(this, ComunicationService.class);  
    	this.startService(sevice);     
    	super.onDestroy();
    }
    
  //�����Ƿ�����
  	private boolean isServiceRunning(Context context, String serviceName) {
  		boolean isRunning = false;
  		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
  		List<RunningServiceInfo> lists = am.getRunningServices(30);
  		for (RunningServiceInfo info : lists) {//�жϷ���
  			if(info.service.getClassName().equals(serviceName)){
  				isRunning = true;
  				break;
  			}
  		}
  		
  		return isRunning;
  	}
}
