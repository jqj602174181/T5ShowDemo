package com.centerm.dispatch.communication;

import android.util.Log;


/* 主机通讯服务接口 */
public class ComService extends Thread {
	public static final int LINK_HID = 0;
	public static final int LINK_BLUETOOTH = 1;
	private static ComService instance = null;
	
    public ComService() {
    	
    }
    
    public static ComService getInstance(){
		if (null == instance) {
			instance = new ComService();
		} 
		return instance;
	}
   
	public native void startService();
    public native void stopService();
    public native CommandEvent getParaseInsFromJNI();
    public native ResultEvent getResultFromJNI();
    public  native int sendResultToJNI(int comDevice, byte[] data, int len);
    public  native int addInsDataToJNI(int comDevice, byte[] data, int len);
    
    public static String comserviceName="comservice";
    static {
    	Log.e("log"," comService is "+comserviceName);
    	System.loadLibrary("comservice");
    	//System.loadLibrary("jhcomservice");
    	
    }
    
}
