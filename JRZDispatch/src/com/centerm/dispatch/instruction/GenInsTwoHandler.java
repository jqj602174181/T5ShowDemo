package com.centerm.dispatch.instruction;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;

import android.os.SystemClock;
import android.util.Log;

import com.centerm.common.Algorithm_H;
import com.centerm.common.ByteHandlerClass;
import com.centerm.common.ChangeDataClass;
import com.centerm.common.RWIniClass;
import com.centerm.common.SpecialVer;
import com.centerm.common.SystemInfo;
import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.common.DealFileClass;
import com.centerm.dispatch.common.PlaySoundClass;
import com.centerm.dispatch.common.SafetyUnitClass;
import com.centerm.dispatch.common.StartAppsClass;
import com.centerm.dispatch.communication.ComService;
import com.centerm.dispatch.communication.ResultAcceptThread;


public final class GenInsTwoHandler implements Command
{	
	public static String deviceInfoName="com.centerm.dispatch.instruction.T5DispatchDeviceInfo";
	private T5DispatchDeviceInfo deviceInfo = null;
	@Override
	public byte[] executeCommand(byte[] data, Integer nLinkType) 
	{
		byte[][] tempdata = new byte[2][];
		
		Log.i(MainActivity.TAG, "GenInsTwoHandler dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		
		if( tempdata[0] != null )//函数名必须不为空
		{
			String methodname = new String(tempdata[0]);
			try			
			{
				deviceInfo = T5DispatchDeviceInfo.initDeviceInfo(deviceInfoName);
				Method method = this.getClass().getMethod( methodname , byte[].class, Integer.class );
				return (byte[])method.invoke( this, tempdata[1], Integer.valueOf(nLinkType) );//执行命令
			}
			catch( NoSuchMethodException e )
			{
				Log.w(MainActivity.TAG, this.getClass().getName() + ":Not find method " + methodname + "!");
			}				
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		
		return null;
	}
	
	//设置音量
	public byte[] getKeyValue( String key, byte[] data )
	{
		if (key == null && data == null)
		{
			return null;
		}

		 byte[][] tempdata = new byte[2][];	
		 tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'=');
		 if (tempdata[0] != null && new String(tempdata[0]).equals(key))
		 {
			 return tempdata[1];
		 }
		 
		 return null;
	}
	
	//重启系统
	public byte[] restartSystem( byte[] data, Integer linkType )
	{
		/*byte[] retdata = {0x02, 0x55, 0x03};
		if (StartAppsClass.rebootI90() == true )
		{
			retdata[1] = (byte)0xAA;
		}
		ComService.sendDataToHost( linkType.intValue(), retdata, retdata.length );//重启成功
		*/
		
		byte[] retdata = {0x02, (byte)0xAA, 0x03};
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );//重启成功
		
		SystemClock.sleep(1000);
		
		DispatchApplication.sendReboot();
		
		return null;
	}
	
	//获取设备状态
	public byte[] getDeviceStatus( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		retdata[1] = (byte)0xAA;
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//启动pqc程序
	public byte[] startPQC( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		try
		{
			byte[] retdata = new byte[3];
			retdata[0] = (byte)0x02;
			retdata[1] = (byte)0xAA;
			retdata[2] = (byte)0x03;
			comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
			Thread.sleep(1000);
			comService.stopService();
			Thread.sleep(1000);
			StartAppsClass.startPQC();
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return null;
	}
	
	//设置版本信息
	public byte[] setVersion( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] type = getKeyValue("type", tempdata[0]);
		if (type != null)
		{
			int index = Integer.parseInt(new String(type));
			
			//取值范围0x31-0x35；0x31表示硬件版本信息；0x32表示指令集版本信息；0x33表示BISO信息；0x34表示系统版本信息；0x35表示应用版本信息,0x36设置版本信息
			if ( index >= 1 && index <= 6 )
			{
				byte[] value = getKeyValue("data", tempdata[1]);
				if (value != null)
				{
					switch(index)
					{
					    case 1:
					    	SystemInfo.setHardware(new String(value));
					    	break;
					    case 2:
					    	RWIniClass.WriteIniString("/mnt/internal_sd/config/keypad.ini", "Keypad", "version", new String(value));
					    	break;
					    case 3:
					    	tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(value, (byte)'|');
					    	if (tempdata[0] != null )
					    	{
					    		//
					    		SystemInfo.setBIOSVersion(new String(tempdata[0]));
					    	}
					    	if (tempdata[1] != null )
					    	{
					    		SystemInfo.setBIOSBuild(new String(tempdata[1]));
					    	}		    	
					    	break;
					    case 4:
					    	tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(value, (byte)'|');
					    	if (tempdata[0] != null )
					    	{
					    		SystemInfo.setSystemVersion(new String(tempdata[0]));
					    	}
					    	if (tempdata[1] != null )
					    	{
					    		SystemInfo.setSystemBuild(new String(tempdata[1]));
					    	}		    	
					    	break;
					    case 5:
					    	tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(value, (byte)'|');
					    	if (tempdata[0] != null )
					    	{
					    		
					    		SystemInfo.setAppsVersion(new String(tempdata[0]));
					    	}
					    	if (tempdata[1] != null )
					    	{
					    		SystemInfo.setAppsBulid(new String(tempdata[1]));
					    	}		    	
					    	break;
					    case 6:
					    	DispatchApplication.saveSerialNum(new String(value));
					    	break;
					}
					retdata[1] = (byte)0xAA;
				}
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取版本信息
	public byte[] getVersion( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		byte[] type = getKeyValue("type", data);
		if (type != null)
		{
			int index = Integer.parseInt(new String(type));
			
			//取值范围0x31-0x35；0x31表示硬件版本信息；0x32表示指令集版本信息；0x33表示BIOS版本信息；0x34表示系统版本信息；0x35表示应用版本信息；0x36表示加密芯片版本信息；
			if ( index >= 1 && index <= 6 )
			{
				String version = "";
				switch(index)
				{
				    case 1:
				    	version = SystemInfo.getHardware();
				    	if (version == null)
				    	{
				    		version = "1.0";
				    	}
				    	break;
				    case 2:
				    	version = RWIniClass.ReadIniString("/mnt/internal_sd/config/keypad.ini", "Keypad", "version");
				    	if (version == null)
				    	{
				    		version = "0";
				    	}
				    	break;
				    case 3:
				    	String biosversion = SystemInfo.getBIOSVersion();
				    	String biosbuild = SystemInfo.getBIOSBuild();
				    	version = biosversion + "|" + biosbuild;
				    	break;
				    case 4:
				    	String systemversion = SystemInfo.getSystemVersion();
				    	String systembuild = SystemInfo.getSystemBuild();
				    	version = systemversion + "|" + systembuild;
				    	break;
				    case 5:	
				    	String appsversion = SystemInfo.getAppsVersion();		
				    	String appsbuild = SystemInfo.getAppsBuild();
				    	version = appsversion + "|" + appsbuild;
				    	break;
				    case 6:
				    	version = Algorithm_H.getEncryptVersion();
				    	break;
				}
				retdata = new byte[ 2 + version.getBytes().length ];
		    	retdata[0] = (byte)0x02;
		    	System.arraycopy( version.getBytes(), 0, retdata, 1, version.getBytes().length );
				retdata[version.getBytes().length + 1] = (byte)0x03;
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				return null;
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//播放提示语音
	public byte[] playSound( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[] file = getKeyValue("file", data);
		if (file != null)
		{
			String filename = null;
			try 
			{
				filename = "/mnt/internal_sd/media/sound/" + new String(file, "GBK");
			} 
			catch (UnsupportedEncodingException e) 
			{
				e.printStackTrace();
			}
			if (DealFileClass.isFileExist(filename) == true)
			{
				PlaySoundClass playsound = new PlaySoundClass();
				playsound.playAudio("file://" + filename);
				retdata[1] = (byte)0xAA;
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//设置设备编号
	public byte[] setDeviceNo( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[] deviceno = getKeyValue("deviceno", data);
		if (deviceno != null)
		{
		
			//SystemInfo.setDevSerial( new String(deviceno) );
		//	DevInfo.setSerialNo( new String(deviceno));
			deviceInfo.setDeviceNo( new String(deviceno));
			retdata[1] = (byte)0xAA;
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取设备编号
	public byte[] getDeviceNo( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		String strDeviceNo =DispatchApplication.serialNumber;
		if ( strDeviceNo != null )
		{
			retdata = new byte[ 2 + strDeviceNo.getBytes().length ];
			retdata[0] = (byte)0x02;
			System.arraycopy( strDeviceNo.getBytes(), 0, retdata, 1, strDeviceNo.getBytes().length );
			retdata[strDeviceNo.getBytes().length + 1] = (byte)0x03;
			comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
			return null;
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//设置设备生产日期
	public byte[] setDeviceDate( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[] devicedate = getKeyValue("devicedate", data);
		if (devicedate != null)
		{
			SystemInfo.setProductionDate( new String(devicedate) );
			retdata[1] = (byte)0xAA;
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取设备生产日期
	public byte[] getDeviceDate( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		String strDeviceDate = SystemInfo.getProductionDate();
		if ( strDeviceDate != null )
		{
			retdata = new byte[ 2 + strDeviceDate.getBytes().length ];
			retdata[0] = (byte)0x02;
			System.arraycopy( strDeviceDate.getBytes(), 0, retdata, 1, strDeviceDate.getBytes().length );
			retdata[strDeviceDate.getBytes().length + 1] = (byte)0x03;
			comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
			return null;
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//硬件握手
	public byte[] handShake( byte[] data, Integer linkType )
	{
		if (SpecialVer.isVersion("ZYB") || SpecialVer.isVersion("XJYC"))
			return oldHandShake(data, linkType);
		return commonHandShake(data, linkType);
	} 	
	public byte[] commonHandShake( byte[] data, Integer linkType )
	{			
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		byte[] normalkey = "9ECDEBF9348498894304E80E1E29160F".getBytes();
		byte[] randomkey = "71594B1E0ABBF72892988AF5CAD74984".getBytes();
		byte[] checkstring = "CENTERM==ZNZDCPB".getBytes();
		byte[] encdata = getKeyValue("encdata", data);
		
		if (encdata != null)
		{
			byte[] combnormalkey = ChangeDataClass.hexStringToBytes(normalkey);
			byte[] combrandomkey = ChangeDataClass.hexStringToBytes(randomkey);
			byte[] combencdata = ChangeDataClass.starCombination(encdata);
			byte[] random2 = SafetyUnitClass.desSelectDecrypt(combnormalkey, combencdata);
			
			byte[] random3 = new byte[16];
			
			for (int i = 0; i < 16; i++)
			{
				random3[i] = (byte)(random2[i]^checkstring[i]);
			}
			
			byte[] newencdata = null;
			if ((((random2[0]^random2[9])&0xFF) == 0xFF)&&(((random2[3]^random2[11])&0xFF) == 0xFF))
			{
				newencdata = SafetyUnitClass.desSelectEncrypt(combrandomkey, random3);
			}
			else
			{
				newencdata = SafetyUnitClass.desSelectEncrypt(combnormalkey, random3);
			}
			
			
			if (newencdata != null)
			{
				byte[] divnewencdata = ChangeDataClass.starDivide(newencdata);
				int valuelen = divnewencdata.length + 3;
				retdata = new byte[valuelen];
				retdata[0] = (byte)0x02;
				retdata[1] = (byte)0xAA;
				System.arraycopy(divnewencdata, 0, retdata, 2, divnewencdata.length);
				retdata[valuelen-1] = (byte)0x03;
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				return null;
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	//硬件握手
	public byte[] oldHandShake( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		byte[] enckey = ")YG6,i1a".getBytes();
		byte[] deckey = "C;z7A1[d".getBytes();
		byte[] encdata = getKeyValue("encdata", data);
		if (encdata != null)
		{
			byte[] combdata = ChangeDataClass.starCombination(encdata);
			byte[] newdata = SafetyUnitClass.desSelectDecrypt(deckey, combdata);
			byte[] tempdata = SafetyUnitClass.desSelectEncrypt(enckey, newdata);
			byte[] divdata = ChangeDataClass.starDivide(tempdata);
			if (divdata != null)
			{
				int valuelen = divdata.length + 2;
				retdata = new byte[valuelen];
				retdata[0] = (byte)0x02;
				System.arraycopy(divdata, 0, retdata, 1, divdata.length);
				retdata[valuelen-1] = (byte)0x03;
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				return null;
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	byte[] data1; 
	private static final String PICTURE_FILE_PATH = "/mnt/sdcard/hw.png";
	private static final String XMLTURE_FILE_PATH = "/mnt/sdcard/hw.xml";
	public byte[] getSignPic( byte[] data, Integer linkType )
	{
		sendSignFileData1(PICTURE_FILE_PATH,linkType);
		return null;
	}



	public static final byte[] DESKEY={(byte)0x8E,0x4A,0x18,0x59,0x08,(byte)0xA0,(byte)0xD2,0x4F};
		 //  public static final byte[] DESKEY={0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38};
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
	
	

	/**
	 * 发送签名数据
	 */
	public byte[] getSignData(byte[] data, Integer linkType)
	{

		sendSignFileData1(XMLTURE_FILE_PATH,linkType);
		
		return null;
	}
	
	private void sendSignFileData(String fileName,int linkType)
	{
		File file = new File(fileName);
		int pkgLen = 1024;
	
		int length = (int)file.length();
		byte[] byteLen = IntToByte(length);//将长度转换成4个字节长度的字节数组

	
		ResultAcceptThread.getInstance().sendData(byteLen,linkType);//先发送长度，然后间隔100ms再发送数据
		InputStream inputStream=null;

		
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		int c= 0;
		try {
			byte[] temp = new byte[pkgLen-8];
			inputStream = new FileInputStream(fileName);
			c = inputStream.read(temp);
		
			int sum = c;
			byte tempData[] = null;

			
			while (c>0) {
				
				sum = sum+c;
				tempData = new byte[c];
	
				System.arraycopy(temp, 0, tempData, 0,c);
				byte[]desData =SafetyUnitClass.desEncrypt(DESKEY, tempData);//
				
				int len = desData.length;
				byte[] sendData = new byte[len+8];
				byte[] sendByteLen = IntToByte(len);//加密后的数据长度
				byte[] realByteLen = IntToByte(c);//没加密前的数据长度
				System.arraycopy(sendByteLen, 0, sendData, 0,4);
				System.arraycopy(realByteLen, 0, sendData, 4,4);
				System.arraycopy(desData, 0, sendData, 8,len);	
				ResultAcceptThread.getInstance().sendData(sendData, linkType);
				c = inputStream.read(temp);
				
			}
			inputStream.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	
		}
		//传完图片删掉，避免下次什么都不传的时候再次传上次签名的图片
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	
		}
	
        if(file.exists()){
        	//file.delete();
        }
	}
	
	private void sendSignFileData1(String fileName,int linkType)
	{
		File file = new File(fileName);
		int pkgLen = 1024;
	
		int length = (int)file.length();
		byte[] byteLen = IntToByte(length);//将长度转换成4个字节长度的字节数组

		
		ResultAcceptThread.getInstance().sendData(byteLen,linkType);//先发送长度，然后间隔100ms再发送数据
		try { 
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		byte[] initData = new byte[pkgLen];
		InputStream inputStream=null;
		int c= 0;
		try {
			byte[] temp = new byte[pkgLen];
			inputStream = new FileInputStream(fileName);
			c = inputStream.read(temp);
		
			int sum = c;
			byte tempData[] = null;
			while (c>0) {
				
				sum = sum+c;
				tempData = new byte[c];
				System.arraycopy(temp, 0, tempData, 0,c);
				ResultAcceptThread.getInstance().sendData(tempData, linkType);
			
				System.arraycopy(initData, 0, temp, 0, temp.length);
				
				c = inputStream.read(temp);
				
			}
			inputStream.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	
		}
		//传完图片删掉，避免下次什么都不传的时候再次传上次签名的图片
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	
		}
	
        if(file.exists()){
        	file.delete();
        }
	}
	
}

