package com.centerm.dispatch.communication;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.app.AppManager;

import android.util.Log;

public class ComServiceThread extends Thread {
	
	private static final String TAG = "ComServiceThread";
	private static ComServiceThread instance = null;
	private ComService comService = null;
	public boolean isInit = false;
	private boolean isStop =false;
	private ComServiceThread() {
		comService = ComService.getInstance();
	
    }
    
    public static ComServiceThread getInstance(){
		if (null == instance) {
			instance = new ComServiceThread();
		} 
		return instance;
	}
    /*
	 * 
	 */
	public  void initWait()//�߳��е��� ���ȴ���ʼ�����
	{
		Log.e("ser","start service");
		int time = 60;
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
		Log.e("ser","end service");
	}
	
	public void initNotify()
	{
		isInit = true;
	}
    @Override
	public void run() {
    
    	comService.startService();		//��������
    	initNotify();
    	while (!isStop) {		//ѭ����ȡ����ָ��
    	
    		CommandEvent event = comService.getParaseInsFromJNI();	//�õ�����ָ��
    		try {
    			Log.i(TAG, "Get Host Cmd: linktype=" + event.getComDeivce() + ", program Index="
    				+ event.getProgramIndex() + ", cmdlen=" + event.getCmdData().length );
    		}
    		catch (Exception e) {
    			Log.e(TAG, "ComService Exception.........................");
    			e.printStackTrace();
    		}
    		
    		//event��ӵ��¼�����
    		if (event != null) {
    			dispatchCmd(event);
    		}
    		else {
    			Log.e(TAG, "event null.........................");
    		}
    	}
    	Log.e("th","stop is ComServiceThread ");
    	
	}

    
    public void stopThread()
    {
    	isStop = true;
    	comService.stopService();
    }
    /**
     * 
     * @param str ָ��������ݰ�һ����ʽ��Ϻ������
     * 
     * ����Ϻ�Ľ������ݷ���ָ�����
     * 
     */
	public void dispatchCmd(CommandEvent event) 
	{
		//��ָ����Ϣ��ӵ�ָ�����
		if (AppManager.getManager().isAddMsg(event) == true)
		{
			Log.i(TAG, "add message........................");
			DispatchApplication.msgList.addMsg(event); 
		}
	}
}
