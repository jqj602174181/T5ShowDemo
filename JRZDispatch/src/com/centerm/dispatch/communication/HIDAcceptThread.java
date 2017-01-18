package com.centerm.dispatch.communication;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

import com.centerm.common.ChangeDataClass;

import android.provider.ContactsContract.Contacts.Data;
import android.util.Log;

public class HIDAcceptThread extends Thread {
	private static final String TAG = "HIDAcceptThread";
	private static HIDAcceptThread instance = null;
	private FileOutputStream fout;
    private FileInputStream fin;
    
	private HIDAcceptThread(){
		openHid();
	}
	
	public static HIDAcceptThread getInstance(){
		if(null == instance){
			instance = new HIDAcceptThread();
			return instance;
			//return new HIDAcceptThread();
		} else {
			return instance;
		}

	}

	@Override
	public void run() {
		new readThread().start();
	}
	
    public int openHid()
    {
    	int ret = 0;
        try {
            this.fout = new FileOutputStream("/dev/android_hid");
        } catch(Exception e) {
        	ret = -1;
            e.printStackTrace();
        }

        try {
            this.fin = new FileInputStream("/dev/android_hid");
        } catch(Exception e) {
        	ret = -2;
            e.printStackTrace();
        }
        return ret;
    }
    
    public int closeHid()
    {
        if( null != this.fout ){
            try{
                this.fout.close();
            } catch (IOException e){
                e.printStackTrace();
            }
        }

        if( null != this.fin ) {
            try {
                this.fin.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return 0;
    }

    public int writeHid(byte[] bytes)
    {
        byte[] send = new byte[1024];
        int length = bytes.length;
        int pos = 0;
        int len = 0;
    	
        if( null != this.fout )
        {
            while(length>0)
            {
                len = 1024;
                if( length < 1024 ) {
                    len = length;
                }

                Arrays.fill(send, (byte) 0x00);
                System.arraycopy( bytes, pos, send, 0, len );
                length -= len;
                pos    += len;

                try {
                	//Log.e(TAG, "send Hid is "+ new String(ChangeDataClass.bytesToHexString(send)));
                    this.fout.write(send);
                    //
                    this.fout.flush();

                } catch(Exception e) {
                    e.printStackTrace();
                }

            }
        }

        return bytes.length;
    }

    public int readHid(byte[] bytes)
    {
        int ret = -1;

        if( null != this.fin )
        {
            try
            {

                ret = this.fin.read(bytes);        
              

            }
            catch(Exception e)
            {
                e.printStackTrace();
            }

        }
        //Log.i("hid read", "hid read len"+ret);
        return ret;
    }
    
    private class readThread extends Thread{
    	@Override
    	public void run(){
            byte[] buffer = new byte[1024];
            int bytes = 0;
            int len = 0;
            while (true) {
                //循环读取报文
            	bytes = readHid(buffer);

            	Log.i(TAG, "len="+bytes);

    
            	//计算有效长度
            	for (len=bytes; len>0; len--) {
            		if (buffer[len-1] != 0x00) {
            			break;
            		}
            	}
            	
            	
            	/*
              	
                int datalen = 96;
               	byte[] data = new byte[datalen];
               	
               	for(int i=0;i<32;i++){
               		data[i] = '1';
               	}
            	for(int i=32;i<64;i++){
               		data[i] = '2';
               	}
            	
            	for(int i=64;i<96;i++){
               		data[i] = '3';
               	}
         
               	String data1 = "313437323538333639303132333435363738393130333435";
               	String data2 = "3132333438313336373E3B333F323C313D333A393C313D3B";
               	String data3 = "393837363534333231303A3B3C3D3E3F3132333435363738";
               	
               	int len1 = data1.getBytes().length;
              // 	System.arraycopy(data1.getBytes(), 0, data, 0, len1);
               	
            	byte flag = 1;
            
               	//byte[] tempData ={0x02,0x00,0x00,0x04,0x00,03};
               	byte[] tempData =new byte[datalen+10];
               	tempData[0] = 0x02;
            	tempData[1] = 0x30;
            	tempData[2] = 0x30;
            	tempData[3] = 0x36;
            	tempData[4] = 0x30;
               	tempData[5] = 0x32;
            	tempData[6] = 0x32;
            	tempData[7] = 0x33;
            	tempData[8] = 0x34;
            	tempData[tempData.length-1] = 0x03;
           
            	System.arraycopy(data, 0,tempData, 9, data.length);
            	String date = new String(tempData,0,tempData.length);
              	Log.e(TAG, "read Hid is "+ new String(ChangeDataClass.bytesToHexString(tempData))+" data is "+date);
              		ComService.getInstance().addInsDataToJNI(ComService.LINK_HID, tempData, tempData.length);
              	*/
            	
              	//Log.e(TAG, "read Hid is "+ new String(ChangeDataClass.bytesToHexString(buffer))+" data is "+date);
    			ComService.getInstance().addInsDataToJNI(ComService.LINK_HID, buffer, len);
 
            }
    	}
    }
}
