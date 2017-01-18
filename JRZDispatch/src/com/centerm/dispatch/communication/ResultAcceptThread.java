package com.centerm.dispatch.communication;

import java.io.ByteArrayOutputStream;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

public class ResultAcceptThread extends Thread {
	
	private static final String TAG = "ResultAcceptThread";
	private static ResultAcceptThread instance = null;
	private HIDAcceptThread hidThread = null;
	private BTAcceptThread btThread = null;
	private ComService comService = null;
	
	private ResultAcceptThread() {
		hidThread = HIDAcceptThread.getInstance();
		btThread = BTAcceptThread.getInstance();
		comService = ComService.getInstance();
    }
	
	public static ResultAcceptThread getInstance(){
		if (null == instance) {
			instance = new ResultAcceptThread();
		} 
		return instance;
	}
	
	 @Override
	public void run() {
		 while (true) {
	
			 ResultEvent event = comService.getResultFromJNI();	//得到jni回传的数据
			 if (event != null) {
			
				 btThread.doExit();
				 if (event.getLinkType() == ComService.LINK_HID) {
					 hidThread.writeHid(event.getResultData());
				 }
				 else if (event.getLinkType() == ComService.LINK_BLUETOOTH) {
					 btThread.sendData(event.getResultData());
					 
				 }
				 
			 }
			 else {
				 Log.e(TAG, "event null.........................");
			 }
		 }
	 }
	 
	 
	 public void sendData(byte[] data, int linkType)
	 {
		 if(linkType == ComService.LINK_BLUETOOTH){
			 btThread.sendData(data);
		 }else if(linkType==ComService.LINK_HID){
			 hidThread.writeHid(data);
		 }
	 }
}
