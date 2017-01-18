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
 * T5 蓝牙监听连接线程
 * @author chenling
 * @since 20150727
 */
public class BTAcceptThread extends Thread 
{
	private static String NAME = "BTServerSocket";
	private static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";

	protected BluetoothAdapter mAdapter = null;

	public static String btThreadName = "com.centerm.dispatch.communication.BTAcceptThread";
	public static boolean isReflact = false;//是否用反射初始化,通用版本不用反反射初始化，定制版本用反射初始化，如浦发版本
	private String TAG = "BTAcceptThread";
//	private BluetoothSocket newSocket = null;
	protected ReadThread mreadThread = null;
	
	private boolean isBluetoothOpen = false;
	private static BTAcceptThread bTAcceptThread = null;
	
	
	byte[] data; 
	protected BTAcceptThread btThread = null;
	
	protected boolean isDisconnect = false;//是否断开蓝牙
	
	public static boolean isCanDisconnectBluetooth = false;
	/**
	 * @param data：存储签名数据
	 * @param linkType：连接方式
	 * @return 签名数据
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
	 * 将int转换为byte数组
	 * @param length	int数值
	 * @return	byte数组
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
	

	
	public  void waitBluetoothOpen()//等待蓝牙开启
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
		Log.d(TAG,"开始蓝牙监听线程");
		mAdapter = BluetoothAdapter.getDefaultAdapter();
		//检查蓝牙设备是否开启，如没有开启则开启蓝牙设备
		openBludtooth();
        
		 BluetoothServerSocket mmServerSocket = null;
		 try {
			mmServerSocket = mAdapter.listenUsingRfcommWithServiceRecord(NAME, UUID.fromString(MY_UUID));
		} catch (IOException e3) {
			// TODO Auto-generated catch block
			e3.printStackTrace();
		}  
            // 	mAdapter.
        /* 阻塞的调用：监听连接的请求,直到收到请求或者出现异常。
		 * 只有远程设备发送一个连接请求，并且携带的UUID与侦听它socket注册的UUID匹配，连接请求才会被接受
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
          			/* 创建一个蓝牙服务器 
          			 * 参数分别：服务器名称、UUID	 */	
                  	mmServerSocket = mAdapter.listenUsingRfcommWithServiceRecord(NAME, UUID.fromString(MY_UUID));  
                 // 	mAdapter.
             //     	mAdapter.listenUsingInsecureRfcommWithServiceRecord(name, uuid);
                  } catch (IOException e) 
                  {
                  	e.printStackTrace();
                  	Log.e(TAG, "创建蓝牙服务器失败"+e.getMessage());
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
        	Log.i(TAG,"开始监听");
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
            	//接受新的连接后释放之前存在的通道
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
            	e.printStackTrace();//出现错误，比如该调用被放弃，或者超时。
            	Log.e(TAG, "监听连接失败"+e.getMessage());
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
            
            //连接建立
            try {
                if (newSocket != null) {  
                	Log.e(TAG, "连接建立成功");
                	isDisconnect = false;
                    manageConnectedSocket(newSocket); 
                    /* 除非需要再接收另外的连接，否则的话调用close()
                     * 与TCP/IP不同，RFCOMM同一时刻 一个信道只允许一个客户端连接，
                     * 因此大多数情况下意味着在BluetoothServerSocket接受一个连接请求后应该
                     * 立即调用close()
                     */
                    //mmServerSocket.close();
                //    mmServerSocket.
                    //break;  
                } else {
                	Log.e(TAG, "连接建立失败");
                }
			} catch (Exception e) {
				e.printStackTrace();
				Log.e(TAG, "连接异常关闭"+e.getMessage());
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
	
	//设置蓝牙永久可见
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
	/* 管理建立连接的socket */
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
	
	//读取数据
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
                    //循环读取报文
                	bytes = btIs.read(buffer);
                    if (bytes > 0)
                    {
                    	//发送到Jni解析队列进行解析
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
     * 子类根据情况进行重写，让蓝牙不可见，通用版本先不实现
     */
    protected void doCloseBluetoothDiscoverable()
    {
    	//通用版本什么不做
    }
    /*
     * 子类根据情况进行重写，让蓝牙不可见，通用版本先不实现
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
