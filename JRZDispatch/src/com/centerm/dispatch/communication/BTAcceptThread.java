package com.centerm.dispatch.communication;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.centerm.common.ChangeDataClass;
import com.centerm.common.RWIniClass;
import com.centerm.dispatch.DispatchApplication;

/**
 * T5 �������������߳�
 * @author chenling
 * @since 20150727
 */
public class BTAcceptThread extends Thread 
{
	private static String NAME = "BTServerSocket";
	private static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";

	protected BluetoothAdapter mAdapter = null;

	public static String btThreadName = "com.centerm.dispatch.communication.BTAcceptThread";
	public static boolean isReflact = false;//�Ƿ��÷����ʼ��,ͨ�ð汾���÷������ʼ�������ư汾�÷����ʼ�������ַ��汾
	private String TAG = "BTAcceptThread";
//	private BluetoothSocket newSocket = null;
	protected ReadThread mreadThread = null;
	
	private boolean isBluetoothOpen = false;
	private static BTAcceptThread bTAcceptThread = null;
	
	
	byte[] data; 
	protected BTAcceptThread btThread = null;
	
	protected boolean isDisconnect = false;//�Ƿ�Ͽ�����
	
	public static boolean isCanDisconnectBluetooth = false;
	/**
	 * @param data���洢ǩ������
	 * @param linkType�����ӷ�ʽ
	 * @return ǩ������
	 */
	public byte[] getSignData( byte[] data, Integer linkType )
	{
		Log.i("HWrite","getSignData");
		Bitmap bmp = BitmapFactory.decodeFile("/mnt/sdcard/hw.png");
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bmp.compress(Bitmap.CompressFormat.PNG, 100, baos);
		data = baos.toByteArray();
		bmp.recycle();
		return data;
	}
	
	/**
	 * ��intת��Ϊbyte����
	 * @param length	int��ֵ
	 * @return	byte����
	 */
	public static byte[] IntToByte(int length)
	{
		byte[] byteArray = new byte[4];
		byteArray[3] = (byte)(length >>> 24);
		byteArray[2] = (byte)(length >>> 16);
		byteArray[1] = (byte)(length >>> 8);
		byteArray[0] = (byte)(length);
		return byteArray;
	}
	

	
	public  void waitBluetoothOpen()//�ȴ���������
	{
		int time = 50;
		while (!isBluetoothOpen) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			time--;
			if(time==0)
				break;
			
		}
	}
	public  BTAcceptThread()
	{
	}
	private static BTAcceptThread initBTAcceptThread(String className)
	{
		Class<?> c = null;

		BTAcceptThread btAcceptThread = null;
		try {
			c = Class.forName(className);

			btAcceptThread = (BTAcceptThread) c.newInstance();
			
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}  catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return btAcceptThread;
	
	}
	public static BTAcceptThread getInstance(){
		
		if(isReflact){
			if(bTAcceptThread == null){
				bTAcceptThread = initBTAcceptThread(btThreadName);
			}
		}else{
			if(bTAcceptThread == null){
				bTAcceptThread = new BTAcceptThread();
			}
		}
		
		return bTAcceptThread;
	}
	
	
	private void openBludtooth()
	{
		if (!mAdapter.isEnabled()) {
		
			mAdapter.enable();
			while(!mAdapter.isEnabled()){
				
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			if (mAdapter.isEnabled()) {
				mAdapter.setName(mAdapter.getName());
				RWIniClass.WriteIniString("/mnt/internal_sd/config/factory.ini", "Factory", "bluetoothName", mAdapter.getName());
				setDiscoverableTimeout(mAdapter,0);
				isBluetoothOpen = true;
			}
		}
	}
	@Override
	public void run() 
	{
		Log.d(TAG,"��ʼ���������߳�");
		mAdapter = BluetoothAdapter.getDefaultAdapter();
		//��������豸�Ƿ�������û�п������������豸
		openBludtooth();
        
		 BluetoothServerSocket mmServerSocket = null;
		 try {
			mmServerSocket = mAdapter.listenUsingRfcommWithServiceRecord(NAME, UUID.fromString(MY_UUID));
		} catch (IOException e3) {
			// TODO Auto-generated catch block
			e3.printStackTrace();
		}  
            // 	mAdapter.
        /* �����ĵ��ã��������ӵ�����,ֱ���յ�������߳����쳣��
		 * ֻ��Զ���豸����һ���������󣬲���Я����UUID��������socketע���UUIDƥ�䣬��������Żᱻ����
		 */
		 int conncectNum = 0;
        while (true)
        {
        	

        	if(mmServerSocket==null){
        		
        		mAdapter.disable();
        		try {
					Thread.sleep(100);
				} catch (InterruptedException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
        		
        		openBludtooth();
        		  try {
          			/* ����һ������������ 
          			 * �����ֱ𣺷��������ơ�UUID	 */	
                  	mmServerSocket = mAdapter.listenUsingRfcommWithServiceRecord(NAME, UUID.fromString(MY_UUID));  
                 // 	mAdapter.
             //     	mAdapter.listenUsingInsecureRfcommWithServiceRecord(name, uuid);
                  } catch (IOException e) 
                  {
                  	e.printStackTrace();
                  	Log.e(TAG, "��������������ʧ��"+e.getMessage());
                  }
        	}
          
        	if(mmServerSocket==null){
        		if(mreadThread!=null){
        			mreadThread.interrupt();
        			mreadThread = null;
        		}
        		
        		Log.e("socket"," mmServerSocket is "+mmServerSocket);
        		
        		DispatchApplication.sendRestart();
        		return;
        	}
        	Log.i(TAG,"��ʼ����");
        	BluetoothSocket	newSocket = null;
            try {
            	
            	
            	newSocket = mmServerSocket.accept();
   
            	if (newSocket == null){
            		DispatchApplication.sendRestart();
            		return;
            	}else{
            	//	closeBluetoothDiscoverable(mAdapter);
            		doCloseBluetoothDiscoverable();
            	}
            	conncectNum++;
            	if(conncectNum>5){
            		//DispatchApplication.sendRestart();
            		//return;
            	}
            	Log.e("con"," connect Num is "+conncectNum);
            	/*
            	//�����µ����Ӻ��ͷ�֮ǰ���ڵ�ͨ��
            	if (btIs != null){
            		btIs.close();
            	}
            	if (btOs != null){
            		btOs.close();
            	}
            	if (mreadThread != null){
            		mreadThread.interrupt();
            	}
            	*/
            	Log.e(TAG,"mreadThread mreadThread");
            } catch (IOException e) {  
            	e.printStackTrace();//���ִ��󣬱���õ��ñ����������߳�ʱ��
            	Log.e(TAG, "��������ʧ��"+e.getMessage());
            	try {
					mmServerSocket.close();//
				//	mmServerSocket.
					mmServerSocket = null;
				} catch (IOException e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
				}
            	
            	continue;
            }
            
            //���ӽ���
            try {
                if (newSocket != null) {  
                	Log.e(TAG, "���ӽ����ɹ�");
                	isDisconnect = false;
                    manageConnectedSocket(newSocket); 
                    /* ������Ҫ�ٽ�����������ӣ�����Ļ�����close()
                     * ��TCP/IP��ͬ��RFCOMMͬһʱ�� һ���ŵ�ֻ����һ���ͻ������ӣ�
                     * ��˴�����������ζ����BluetoothServerSocket����һ�����������Ӧ��
                     * ��������close()
                     */
                    //mmServerSocket.close();
                //    mmServerSocket.
                    //break;  
                } else {
                	Log.e(TAG, "���ӽ���ʧ��");
                }
			} catch (Exception e) {
				e.printStackTrace();
				Log.e(TAG, "�����쳣�ر�"+e.getMessage());
				try {
					Thread.sleep(2000);
				} catch (InterruptedException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				//return;
			}
        }			
	}
	
	//�����������ÿɼ�
	protected static  void setDiscoverableTimeout(BluetoothAdapter adapter,int timeout) {
		//BluetoothAdapter adapter=BluetoothAdapter.getDefaultAdapter();
		try {
			Method setDiscoverableTimeout = BluetoothAdapter.class.getMethod("setDiscoverableTimeout", int.class);
			setDiscoverableTimeout.setAccessible(true);
			Method setScanMode =BluetoothAdapter.class.getMethod("setScanMode", int.class,int.class);
			setScanMode.setAccessible(true);

			setDiscoverableTimeout.invoke(adapter, timeout);
			setScanMode.invoke(adapter, BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE,timeout);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	protected static void closeBluetoothDiscoverable(BluetoothAdapter adapter) {
		try {
			Method setDiscoverableTimeout = BluetoothAdapter.class.getMethod("setDiscoverableTimeout", int.class);
			setDiscoverableTimeout.setAccessible(true);
			Method setScanMode =BluetoothAdapter.class.getMethod("setScanMode", int.class,int.class);
			setScanMode.setAccessible(true);
			
			setDiscoverableTimeout.invoke(adapter, 1);
			setScanMode.invoke(adapter, BluetoothAdapter.SCAN_MODE_NONE,1);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/* ���������ӵ�socket */
	private void manageConnectedSocket(BluetoothSocket newSocket)
	{
		if(mreadThread!=null){
			mreadThread.interrupt();
			mreadThread = null;
		}
		
		try {
			Thread.sleep(50);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		mreadThread = new ReadThread(newSocket);
		mreadThread.start();
	}
	
	//��ȡ����
    protected class ReadThread extends Thread { 
    	private BluetoothSocket newSocket;
    	private InputStream btIs = null;
    	private OutputStream btOs = null;
    	private CheckThread checkThread;
    	public ReadThread(BluetoothSocket newSocket){
    		this.newSocket = newSocket;
    	}
    	private boolean isStop = false;
        @Override
		public void run() {
        	int bytes;
        	try {
				btIs = newSocket.getInputStream();
				btOs = newSocket.getOutputStream();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
				Log.e("IoExe", "readThread 112233 IOException:" + e1.getMessage());
			}
        //	checkThread = new CheckThread();
       // 	checkThread.start();
        //    byte intData[] =new byte[4];
       	   byte[] buffer = new byte[1024];
       	   String tag  = "IoExe";
            while (!isStop) {
                try {
                    //ѭ����ȡ����
                	bytes = btIs.read(buffer);
                    if (bytes > 0)
                    {
                    	//���͵�Jni�������н��н���
                    	byte[] dt = new byte[bytes];
                    	System.arraycopy(buffer, 0, dt, 0, bytes);
                    
                    //	Log.e("btr"," data is "+new String(ChangeDataClass.bytesToHexString(dt))+" data is "+new String(dt));
                    	doRead(buffer,bytes);
                    	if(isDisconnect){
                    		exit();
                    		break;
                    	}
                    }else{
                    
                    	exit();
                    	break;
                    }
                } catch (IOException e) {
                	Log.e("IoExe", "readThread IOException:" + e.getMessage());
                	exit();
                    break;
                    
                }
            }
        }
        
        public void exit()
        {
        	if(isStop)return;
        	
        	try {
        		isStop = true;
				btIs.close();
				btOs.close();
	    		newSocket.close();
	    		newSocket = null;
	    		btIs = null;
	    		btOs = null;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        	doDiscoverableTimeout();
        }
        
        
    	public boolean isDeviceConnect() {
    		boolean isConnected = false;
    		return isConnected;
    	}
		@Override
		public void interrupt() {
			// TODO Auto-generated method stub
			super.interrupt();
			exit();
		}
        
        
		public void writeDate(final byte[] data)
		{
			if (btOs == null || data == null) {
	    		
	    		return;
	    	}

			
	    	try {
				btOs.write(data);
			
			   	//Log.e(TAG, "BT sendData is "+ new String(ChangeDataClass.bytesToHexString(data)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		public void writeDate(final byte[] data,int offset,int len)
		{
			if (btOs == null || data == null) {
	    		
	    		return;
	    	}

	    	try {
				btOs.write(data,offset,len);
			
			   	//Log.e(TAG, "BT sendData is "+ new String(ChangeDataClass.bytesToHexString(data)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		
		private class CheckThread extends Thread{
			
			public void run()
			{
				while(!isStop)
				{
					boolean is  = false;
					is = newSocket.isConnected();
				//	Log.e("soc"," socke close is "+is);
					if(newSocket!=null&&!is){
	            		
	            		exit();
	            		break;
	            	}
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			//	Log.e("soc"," isStop isStop is "+isStop);
			}
		}
    }
    
  
    
    /*
     * ����������������д�����������ɼ���ͨ�ð汾�Ȳ�ʵ��
     */
    protected void doCloseBluetoothDiscoverable()
    {
    	//ͨ�ð汾ʲô����
    }
    /*
     * ����������������д�����������ɼ���ͨ�ð汾�Ȳ�ʵ��
     */
    protected void  doDiscoverableTimeout()
    {
    	
    }
    public void sendData(final byte[] data) {
    	
    	
    	int maxLen = 1024;
    //	Log.e("btr"," send data is "+new String(ChangeDataClass.bytesToHexString(data)));
    	if(mreadThread!=null&&data!=null){
    		int len = data.length;
    	//	Log.e("index"," data len is "+len);
    		if(len>maxLen){
    			int index = len/maxLen;
    	
    			int last = len%maxLen;
    			//Log.e("index"," index is "+index+" last is "+last);
    			byte[]sendData = null; 
    			int offset = 0;
    			for(int i=0;i<index;i++){
    				sendData = new byte[maxLen];
    				
    				System.arraycopy(data, offset, sendData, 0, maxLen);
    				offset = offset+maxLen;
    				mreadThread.writeDate(sendData);
    			
    			}
    			
    			if(last>0){
    				sendData = new byte[last];
    				System.arraycopy(data, offset, sendData, 0, last);
    				mreadThread.writeDate(sendData);
    			}
    		}else{
    			mreadThread.writeDate(data);
    		}
    	
    	}
    	
    }
    
    public void doRead(byte[] buffer,int bytes)
    {
    	ComService.getInstance().addInsDataToJNI(ComService.LINK_BLUETOOTH, buffer, bytes);
    	
     	
    	
    }
    
    public void doExit()
    {
    	
    }
}
