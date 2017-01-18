package com.centerm.common;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Properties;

import android.bluetooth.BluetoothAdapter;
import android.util.Log;

public class SystemUtil {
	
	public final static String DEF_VERSION = "0.00";		//Ĭ�ϵİ汾��
	public final static String FILE_SYSTEM_INFO = "/mnt/internal_sd/config/systeminfo.xml";	//ϵͳ�汾��ŵ��ļ�

	/**
	 * ��ȡbuild.prop�е�ָ�����ԣ�������Ϊ"ro.product.version"ʱ������ֵΪϵͳ�汾��
	 * @param PropertiesName ��ȡ����ַ������ںź������Ϣ
	 * @return  ���ػ�ȡ������Ϣ
	 */
	public static String GetBuildProproperties(String PropertiesName) {
		String ProperValue = null;
		InputStream is = null;
		BufferedReader br = null;
		try {
			is = new BufferedInputStream(new FileInputStream(new File(
					"/system/build.prop")));
			br = new BufferedReader(new InputStreamReader(is));
			String strTemp = "";
			while ((strTemp = br.readLine()) != null) {
				// ����ļ�û�ж��������
				if (strTemp.indexOf(PropertiesName) != -1) {
					ProperValue = strTemp.substring(strTemp.indexOf("=") + 1);
					break;
				}
			}
			br.close();
			is.close();
		} catch (Exception e) {
			if (e.getMessage() != null)
				System.out.println(e.getMessage());
			else
				e.printStackTrace();
		}

		return ProperValue;
	}
	
	public static String getSystemVersion()
	{
		String version = SystemUtil.GetBuildProproperties("ro.product.version");
		byte []data = version.getBytes();
		version = new String(data,0,4);
		return version;
	}
	
	public static String getSystemVersion1()
	{
		String version = SystemUtil.GetBuildProproperties("ro.product.version");
		return version;
	}
	
	/**
	 * ��ȡ������ַ
	 * @return (String)������ַ
	 */
	public static  String getBluetoothAddress()	
	{
		BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
		return btAdapter.getAddress();
		
	}

	public static String getSystemInfo( String key, String defValue, String filePath )
	{
		File file = new File( filePath );
		if( file.exists() == false )
		{
			return defValue;
		}
		try
		{
			//���������ļ���������
			FileInputStream fileStream = new  FileInputStream( file );
			Properties systemInfo = new Properties();
			systemInfo.loadFromXML( fileStream );
			fileStream.close();
			String strPorperty = systemInfo.getProperty( key );
			if (null == strPorperty)
			{
				return defValue;
			}
			else
			{
				return strPorperty;
			}
		}
		catch( Exception e )
		{
			e.printStackTrace();
			return defValue;
		}
	}
	//��ȡ�汾��
			

	public static String getAppVersion()
	{
		String appVersion = getSystemInfo( "appsversion", DEF_VERSION, FILE_SYSTEM_INFO );
		Log.e("appVer"," appVersion is "+appVersion);
		return appVersion;
	}


}
