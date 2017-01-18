package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Properties;

/**
 * ϵͳ��Ϣ��
 *
 */
public class SystemInfo 
{
	final static String FILE_SYSTEM_INFO = "/mnt/internal_sd/config/systeminfo.xml";	//ϵͳ�汾��ŵ��ļ�
	final static String FILE_KEYPAD_CONFIG = "/mnt/internal_sd/config/keypad.ini";
	final static String DEF_VERSION = "0.00";		//Ĭ�ϵİ汾��
	final static String DEF_BUILD = "0000000000";	//Ĭ�ϵİ汾buid����
	
	final static String DEV_INFO = "/mnt/internal_sd/device.xml";			//�豸��Ϣ��ŵ��ļ�
	final static String UPDATE_INFO = "/mnt/internal_sd/config/updateinfo.ini";	//������Ϣ��ŵ��ļ�
	final static String DEF_HARDWARE = "1.00";							//Ĭ�ϵ�Ӳ���汾��
	final static String DEF_DEVNAME = "�ƶ�����";						//Ĭ���豸����
	final static String DEF_MODEL = "T5";								//Ĭ���豸�ͺ�
	final static String DEF_MANUFACTURE = "centerm";					//Ĭ����������
	final static String DEF_DATE = "20140101";							//Ĭ���豸��������
	final static String DEF_SERIAL = "000000000000";					//Ĭ���豸���
	final static String DEF_UPRESULT = "null";							//Ĭ���������
	final static String UpdateSuccess = "Success";						//�����ɹ�
	final static String UpdateFail = "Fail";							//����ʧ��
	final static String UpdateProcessing = "Processing";				//������
	
	/**
	 * ��ȡϵͳ����Ϣ��
	 * @param key key����
	 * @param defValue Ĭ��ֵ
	 * @return key��ֵ
	 */
	public static String getSystemInfo( String key, String defValue, String filePath )
	{
		File file = new File( filePath );
		if( file.exists() == false )
		{
			return defValue;
		}
		
		try
		{
			Runtime.getRuntime().exec("chmod 0777 " + filePath);
			
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
	
	/**
	 * ��ȡϵͳ����Ϣ��
	 * @param section ����
	 * @param key key����
	 * @param defValue Ĭ��ֵ
	 * @return key��ֵ
	 */
	public static String getSystemInfo( String section, String key, String defValue, String filePath )
	{
		RWIniClass.resetIniMap( filePath );
		String strValue = RWIniClass.ReadIniString(filePath, section, key );
		if ( strValue != null )
		{
			return strValue;
		}
		else
		{
			return defValue;
		}
	}
	
	/**
	 * ����ϵͳ��Ϣ
	 * @param key ����
	 * @param value ��ֵ
	 * @return true��ʾ�ɹ���false��ʾʧ��
	 */
	public static boolean setSystemInfo( String key, String value, String filePath )
	{
		if ( key == null || value == null )
		{
			return false;
		}
		
		try
		{
			Properties systemInfo = new Properties();
			File file = new File( filePath );
			if( file.exists() == true )//���ļ����ڣ�����ļ��еõ���������ֵ
			{
				FileInputStream inStream = new  FileInputStream( file );	
				systemInfo.loadFromXML( inStream );
				inStream.close();
			}
			
			//��ʼ��������Ϣ
			systemInfo.setProperty( key, value );			
			FileOutputStream outStream = new FileOutputStream( file );
			systemInfo.storeToXML( outStream, null );
			Runtime.getRuntime().exec("sync");
		}
		catch( Exception e )
		{
			e.printStackTrace(); 
		}
		return false;
	}
	
	/**
	 * ����ϵͳ��Ϣ
	 * @param section ����
	 * @param key ����
	 * @param value ��ֵ
	 * @return true��ʾ�ɹ���false��ʾʧ��
	 */
	public static boolean setSystemInfo( String section, String key, String value, String filePath )
	{
		if ( section == null || key == null || value == null )
		{
			return false;
		}
		RWIniClass.resetIniMap( filePath );
		RWIniClass.WriteIniString( filePath, section, key, value );
		return true;
	}

	/**
	 * ��ȡϵͳ�汾��
	 */
	public static String getSystemVersion()
	{
		return getSystemInfo( "systemversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ��ȡϵͳ�汾����
	 */
	public static String getSystemBuild()
	{
		return getSystemInfo( "systembuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ����ϵͳ�汾��
	 */
	public static boolean setSystemVersion( String version )
	{
		if (version == null)
		{
			return false;
		}
		return setSystemInfo( "systemversion", version, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ����ϵͳ�汾����
	 */
	public static boolean setSystemBuild( String bulid )
	{
		if (bulid == null)
		{
			return false;
		}
		return setSystemInfo( "systembuild", bulid, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ��ȡAPP�汾��
	 */
	public static String getAppsVersion()
	{
		return getSystemInfo( "appsversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ��ȡAPP�汾����
	 */
	public static String getAppsBuild()
	{
		return getSystemInfo( "appsbuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	
	/**
	 * ����APP�汾��
	 */
	public static boolean setAppsVersion( String version )
	{
		if (version == null)
		{
			return false;
		}
		return setSystemInfo( "appsversion", version, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ����APP�汾����
	 */
	public static boolean setAppsBulid( String build )
	{
		if (build == null)
		{
			return false;
		}
		return setSystemInfo( "appsbuild", build, FILE_SYSTEM_INFO );
	}
	
	
	/**
	 * ��ȡBIOS�汾��
	 */
	public static String getBIOSVersion()
	{
		return getSystemInfo( "biosversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ��ȡBIOS�汾����
	 */
	public static String getBIOSBuild()
	{
		return getSystemInfo( "biosbuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ����BIOS�汾��
	 */
	public static boolean setBIOSVersion( String version )
	{
		if (version == null)
		{
			return false;
		}
		return setSystemInfo( "biosversion", version, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ����BIOS�汾����
	 */
	public static boolean setBIOSBuild( String build )
	{
		if (build == null)
		{
			return false;
		}
		return setSystemInfo( "biosbuild", build, FILE_SYSTEM_INFO );
	}
	
	/**
	 * ��ȡ�豸����
	 */
	public static String getDevName()
	{
		return getSystemInfo( "name", DEF_DEVNAME, DEV_INFO );
	}
	
	/**
	 * �����豸����
	 */
	public static boolean setDevName( String build )
	{
		if (build == null)
		{
			return false;
		}
		return setSystemInfo( "name", build, DEV_INFO );
	}
	
	/**
	 * ��ȡ�豸�ͺ�
	 */
	public static String getDevModel()
	{
		return getSystemInfo( "model", DEF_MODEL, DEV_INFO );
	}
	
	/**
	 * �����豸�ͺ�
	 */
	public static boolean setDevMode( String build )
	{
		if (build == null)
		{
			return false;
		}
		return setSystemInfo( "model", build, DEV_INFO );
	}
	
	/**
	 * ��ȡ��������
	 */
	public static String getManufacture()
	{
		return getSystemInfo( "manufacture", DEF_MANUFACTURE, DEV_INFO );
	}
	
	/**
	 * ������������
	 */
	public static boolean setManufacture( String manufacture )
	{
		if (manufacture == null)
		{
			return false;
		}
		return setSystemInfo( "manufacture", manufacture, DEV_INFO );
	}
	
	/**
	 * ��ȡ�豸��������
	 */
	public static String getProductionDate()
	{
		return getSystemInfo( "date", DEF_DATE, DEV_INFO );
		//return new String(GlobalConfService.comService.getDevDate());
	}
	
	/**
	 * �����豸��������
	 */
	public static boolean setProductionDate( String date )
	{
		if (date == null)
		{
			return false;
		}
		return setSystemInfo( "date", date, DEV_INFO );
		/*int nRet =  GlobalConfService.comService.setDevDate(date.getBytes());
		if (nRet == 0)
		{
			return true;
		}
		else
		{
			return false;
		}*/
	}
	
	/**
	 * ��ȡ�豸���
	 */
	public static String getDevSerial()
	{
		return getSystemInfo( "serail", DEF_SERIAL, DEV_INFO );
		//return new String(GlobalConfService.comService.getDevSN());
	}
	
	/**
	 * �����豸�ͺ�
	 */
	public static boolean setDevSerial( String serial )
	{
		if (serial == null)
		{
			return false;
		}
		return setSystemInfo( "serail", serial, DEV_INFO );
		/*int nRet =  GlobalConfService.comService.setDevSN(serial.getBytes(), serial.length());
		if (nRet == 0)
		{
			return true;
		}
		else
		{
			return false;
		}*/
	}
	
	/**
	 * ��ȡӲ���汾��
	 */
	public static String getHardware()
	{
		return getSystemInfo( "hardware", DEF_HARDWARE, DEV_INFO );
	}
	
	/**
	 * ����Ӳ���汾��
	 */
	public static boolean setHardware( String hare )
	{
		if (hare == null)
		{
			return false;
		}
		return setSystemInfo( "hardware", hare, DEV_INFO );
	}
	
	/**
	 * ��ȡ������Ϣ��ǰ�汾
	 */
	public static String getUpLocalVersion()
	{
		return getSystemInfo( "Update", "localver", DEF_VERSION, UPDATE_INFO );
	}
	
	/**
	 * ����������Ϣ��ǰ�汾
	 */
	public static boolean setUpLocalVersion( String version )
	{
		if (version == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "localver", version, UPDATE_INFO );
	}
	
	/**
	 * ��ȡ������ϢĿ��汾
	 */
	public static String getUpTargetVersion()
	{
		return getSystemInfo( "Update", "targetver", DEF_VERSION, UPDATE_INFO );
	}
	
	/**
	 * ����������ϢĿ��汾
	 */
	public static boolean setUpTargetVersion( String version )
	{
		if (version == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "targetver", version, UPDATE_INFO );
	}
	
	/**
	 * ��ȡ�������
	 */
	public static String getUpdateResult()
	{
		return getSystemInfo( "Update", "result", DEF_UPRESULT, UPDATE_INFO );
	}
	
	/**
	 * �����������
	 */
	public static boolean setUpdateResult( String result )
	{
		if (result == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "result", result, UPDATE_INFO );
	}
	
	/**
	 * ��ȡ���������ϸ
	 */
	public static String getUpdateDetail()
	{
		return getSystemInfo( "Update", "detail", DEF_UPRESULT, UPDATE_INFO );
	}
	
	/**
	 * �������������ϸ
	 */
	public static boolean setUpdateDetail( String detail )
	{
		if (detail == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "detail", detail, UPDATE_INFO );
	}
	
	// ��ȡ�Ӱ汾����
	public static int getSubVersion() 
	{
		return RWIniClass.ReadIniInt(FILE_KEYPAD_CONFIG, "Keypad", "subversion", 0);
	}
}
