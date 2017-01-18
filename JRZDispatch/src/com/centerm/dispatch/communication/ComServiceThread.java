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
	public  void initWait()//线程中调用 ，等待初始化完成
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
    
    	comService.startService();		//启动服务
    	initNotify();
    	while (!isStop) {		//循环获取主机指令
    	
    		CommandEvent event = comService.getParaseInsFromJNI();	//得到主机指令
    		try {
    			Log.i(TAG, "Get Host Cmd: linktype=" + event.getComDeivce() + ", program Index="
    				+ event.getProgramIndex() + ", cmdlen=" + event.getCmdData().length );
    		}
    		catch (Exception e) {
    			Log.e(TAG, "ComService Exception.........................");
    			e.printStackTrace();
    		}
    		
    		//event添加到事件队列
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
     * @param str 指令解析数据按一定格式组合后的数据
     * 
     * 将组合后的解析数据放入指令队列
     * 
     */
	public void dispatchCmd(CommandEvent event) 
	{
		//将指令信息添加到指令队列
		if (AppManager.getManager().isAddMsg(event) == true)
		{
			Log.i(TAG, "add message........................");
			DispatchApplication.msgList.addMsg(event); 
		}
	}
}
