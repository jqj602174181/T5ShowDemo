package com.centerm.dispatch.instruction;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;

import org.json.JSONException;
import org.json.JSONStringer;

import android.annotation.SuppressLint;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Environment;
import android.os.RecoverySystem;
import android.util.Log;

import com.centerm.common.ByteHandlerClass;
import com.centerm.common.DevInfo;
import com.centerm.common.FileDigest;
import com.centerm.common.RWIniClass;
import com.centerm.common.SystemInfo;
import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.common.DealFileClass;
import com.centerm.dispatch.communication.ComService;


public final class GenInsThreeHandler implements Command
{
	
	
	//private T5DispatchDeviceInfo deviceInfo = null;
	
	public static String deviceInfoName="com.centerm.dispatch.instruction.T5DispatchDeviceInfo";
	@Override
	public byte[] executeCommand(byte[] data, Integer nLinkType) 
	{
		byte[][] tempdata = new byte[2][];
		
		Log.i(MainActivity.TAG, "GenInsThreeHandler dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		
		if( tempdata[0] != null )//函数名必须不为空
		{
			String methodname = new String(tempdata[0]);
			try			
			{
				//deviceInfo = T5DispatchDeviceInfo.initDeviceInfo(deviceInfoName);
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
	
	//获取健值对
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
	
	//获取升级状态
	public byte[] getUpdateStatus( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		try
		{
			JSONStringer jsonDevInfo = new JSONStringer();	//设备信息
			jsonDevInfo.object();
			jsonDevInfo.key("LocalVersion");	//当前版本
			jsonDevInfo.value(SystemInfo.getUpLocalVersion());
			jsonDevInfo.key("TargetVersion");	//目标版本
			jsonDevInfo.value(SystemInfo.getUpTargetVersion());
			jsonDevInfo.key("UpdateResult");	//升级结果
			jsonDevInfo.value(SystemInfo.getUpdateResult());
			jsonDevInfo.key("UpdateDetail");	//升级结果明细
			jsonDevInfo.value(SystemInfo.getUpdateDetail());
			jsonDevInfo.endObject();
			
			try
			{	//转为GBK编码
				byte[] jsonData = jsonDevInfo.toString().getBytes("GBK");
				if (jsonData != null)
				{
					retdata = new byte[jsonData.length+2];
					retdata[0] = (byte)0x02;
					System.arraycopy( jsonData, 0, retdata, 1, jsonData.length );
					retdata[jsonData.length+1] = (byte)0x03;
					comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				}
			}
			catch (UnsupportedEncodingException e)
			{
				retdata = new byte[3];//返回的数据
				retdata[0] = (byte)0x02;
				retdata[1] = (byte)0x55;
				retdata[2] = (byte)0x03;
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				e.printStackTrace();
			}
		}
		catch (JSONException e)
		{
			retdata = new byte[3];//返回的数据
			retdata[0] = (byte)0x02;
			retdata[1] = (byte)0x55;
			retdata[2] = (byte)0x03;
			comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );	
			e.printStackTrace();
		}
		return null;
	}
	
	//获取设备信息
	public byte[] getDeviceInfo( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		try
		{
			String info = T5DispatchDeviceInfo.getDeviceInfo();
			try
			{	//转为GBK编码
				byte[] jsonData = info.getBytes("GBK");
				if (jsonData != null)
				{
					retdata = new byte[jsonData.length+2];
					retdata[0] = (byte)0x02;
					System.arraycopy( jsonData, 0, retdata, 1, jsonData.length );
					retdata[jsonData.length+1] = (byte)0x03;
					comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				}
			}
			catch (UnsupportedEncodingException e)
			{
				retdata = new byte[3];//返回的数据
				retdata[0] = (byte)0x02;
				retdata[1] = (byte)0x55;
				retdata[2] = (byte)0x03;
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				e.printStackTrace();
			}
		}
		catch (JSONException e)
		{
			retdata = new byte[3];//返回的数据
			retdata[0] = (byte)0x02;
			retdata[1] = (byte)0x55;
			retdata[2] = (byte)0x03;
			comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );	
			e.printStackTrace();
		}
		return null;
	}
	
	//获取设备类型，T5返回CTT5类型
	public byte[] GetDevType( byte[] data, Integer linkType )
	{
		byte[] retdata = new byte[16];
		retdata[0] = 0x02;
		byte[] DevType = new String("CTT5").getBytes();
		System.arraycopy(DevType, 0, retdata, 1,  DevType.length);
		int nRetLen = 1 + DevType.length;
		retdata[nRetLen] = 0x03;
		nRetLen++;
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, nRetLen );
		return null;
	}
	public byte[] SetDisplayTimeout( byte[] data, Integer linkType )
	{
		try 
		{
			RWIniClass.WriteIniString( "/mnt/internal_sd/config/multimedia.ini", "Picture", "delaytime", new String(data,"GBK"));
		} 
		catch (UnsupportedEncodingException e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	//启动adb
	public byte[] startADB( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "startADB");
		byte[] retdata = {0x02, (byte)0xAA, 0x03};
		String usbConfig = getProp("sys.usb.config");
		if (usbConfig != null)
		{

			setProp("sys.rkadb.root", "1");
			usbConfig = usbConfig.replace(",adb", "");
			Log.i("StartADB",usbConfig );
			setProp("sys.usb.config", usbConfig+",adb");
		}
		else
		{
			Log.i("StartADB","usbConfig = null" );
			retdata[1] = (byte)0x55;
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//关闭adb
	public byte[] stopADB( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "stopADB");
		byte[] retdata = {0x02, (byte)0xAA, 0x03};
		String usbConfig = getProp("sys.usb.config");
		if (usbConfig != null)
		{
			Log.i("StartADB",usbConfig );
			usbConfig = usbConfig.replace(",adb", "");
			Log.i("StartADB",usbConfig );
			setProp("sys.usb.config", usbConfig);
			setProp("sys.rkadb.root", "0");
		}
		else
		{
			Log.i("StartADB","usbConfig = null" );
			retdata[1] = (byte)0x55;
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//运行APP
	public byte[] runApp( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "runApp1111111111");
		byte[] retdata = {0x02, (byte)0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] pkg = getKeyValue("pkg", tempdata[0]);
		if (pkg != null)
		{
			Log.i(MainActivity.TAG, new String(pkg));
			byte[] name = getKeyValue("name", tempdata[1]);
			if (name != null)
			{
				Log.i(MainActivity.TAG, new String(name));
				try
				{
					ComponentName appComponent = new ComponentName(new String(pkg), new String(name));
					Intent intent = new Intent(Intent.ACTION_MAIN);
					intent.addCategory(Intent.CATEGORY_LAUNCHER);
					intent.setComponent(appComponent);
					intent.setAction("android.intent.action.VIEW");
					intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
					DispatchApplication.getProcessContext().startActivity(intent);
					retdata[1] = (byte)0xAA;
				} 
				catch (Exception e)
				{
					Log.e(MainActivity.TAG, "Can't not start " + pkg);
				}
			}
		}
		
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//升级ZIP包
	public byte[] updateZip( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "updateZip");
		byte[] retdata = {0x02, (byte)0x55, 0x03};
		String tfPath = getTfPath();
		if (!tfPath.isEmpty())
		{
			File zip = new File(tfPath + "/update.zip");
			if (zip.exists())
			{
				try 
				{
					retdata[1] = (byte)0xAA;
		            RecoverySystem.installPackage(DispatchApplication.getProcessContext(), zip);
		        } 
				catch (IOException e) 
				{
		            e.printStackTrace();
		        }
			}
		}
		
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//上传文件(泛用)
	public byte[] pullFile( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "runApp");
		ComService comService = ComService.getInstance();
		byte[] retdata = {0x02, (byte)0x55, 0x03};
		byte[] path = getKeyValue("path", data);
		if (path != null)
		{
			String filePath = new String(path);
			if (DealFileClass.isFileExist(filePath))
			{
				String sendData = "file:" + filePath;
				comService.sendResultToJNI( linkType.intValue(), sendData.getBytes(), sendData.length() );
				return null;
			}
		}
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//删除文件(泛用)
	public byte[] specDeleteFile( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "runApp");
		byte[] retdata = {0x02, (byte)0x55, 0x03};
		byte[] path = getKeyValue("path", data);
		if (path != null)
		{
			String filePath = new String(path);
			if (DealFileClass.isFileExist(filePath))
			{
				DealFileClass.deleteFile(filePath);
				retdata[1] = (byte)0xAA;
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//查询文件(泛用)
	@SuppressLint("DefaultLocale")
	public byte[] specQueryFile( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "runApp");
		byte[] retdata = {0x02, (byte)0x55, 0x03};
		byte[] path = getKeyValue("path", data);
		if (path != null)
		{
			String filePath = new String(path);
			if (DealFileClass.isFileExist(filePath))
			{
				String strLen = String.format("%d", DealFileClass.getFileSize(filePath));
				String strMD5 = FileDigest.getFileMD5(filePath);
				String strFileInfo = strLen + "|" + strMD5;
				retdata = new byte[strFileInfo.length() + 2];
				retdata[0] = (byte)0x02;
				System.arraycopy(strFileInfo.getBytes(), 0, retdata, 1, strFileInfo.length());
				retdata[strFileInfo.length() + 1] = (byte)0x03;
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取系统属性
	private String getProp(String key)
	{
		Log.i("StartADB", "getProp");
		String line = ""; 
		Process process;
		try 
		{
			process = Runtime.getRuntime().exec("getprop " + key);
			InputStreamReader ir = new InputStreamReader(process.getInputStream());  
			BufferedReader input = new BufferedReader(ir);
	        line = input.readLine();
		}
		catch (IOException e) 
		{
			e.printStackTrace();
		}  
		return line;
	}
	
	//设置系统属性
	private String setProp(String key, String value)
	{
		Log.i("StartADB", "getProp");
		String line = ""; 
		Process process;
		try 
		{
			process = Runtime.getRuntime().exec("setprop " + key + " " + value);
			InputStreamReader ir = new InputStreamReader(process.getInputStream());  
			BufferedReader input = new BufferedReader(ir);
	        line = input.readLine();
		}
		catch (IOException e) 
		{
			e.printStackTrace();
		}  
		return line;
	}
	
	//获取TF卡路径
	private static String getTfPath()
	{
		String mount = new String();
		try
		{
			Runtime runtime = Runtime.getRuntime();
			Process proc = runtime.exec( "mount" );
			InputStream is = proc.getInputStream();
			InputStreamReader isr = new InputStreamReader( is );
			String line;
			BufferedReader br = new BufferedReader( isr );
			while( ( line = br.readLine() ) != null )
			{
				if( line.contains( "secure" ) )
				{
					continue;
				}
				if( line.contains( "asec" ) )
				{
					continue;
				}
				if( line.contains( "fat" ) )
				{
					String columns[] = line.split( " " );
					if( columns != null && columns.length > 1 )
					{
						mount = columns[1];
						if( !mount.equals( Environment
								.getExternalStorageDirectory().getPath() ) 
								&& !mount.contains( "/mnt/usb_storage" ) )
						{
							break;
						}
						else
						{
							mount = "";
						}
					}
				}
				else if( line.contains( "fuse" ) )
				{
					String columns[] = line.split( " " );
					if( columns != null && columns.length > 1 )
					{
						mount = mount.concat( columns[1] + "\n" );
					}
				}
			}
		}
		catch( FileNotFoundException e )
		{
			e.printStackTrace();
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
		return mount;
	}
}
