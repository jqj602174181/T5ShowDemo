package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Properties;

/**
 * 系统信息类
 *
 */
public class SystemInfo 
{
	final static String FILE_SYSTEM_INFO = "/mnt/internal_sd/config/systeminfo.xml";	//系统版本存放的文件
	final static String FILE_KEYPAD_CONFIG = "/mnt/internal_sd/config/keypad.ini";
	final static String DEF_VERSION = "0.00";		//默认的版本号
	final static String DEF_BUILD = "0000000000";	//默认的版本buid日期
	
	final static String DEV_INFO = "/mnt/internal_sd/device.xml";			//设备信息存放的文件
	final static String UPDATE_INFO = "/mnt/internal_sd/config/updateinfo.ini";	//升级信息存放的文件
	final static String DEF_HARDWARE = "1.00";							//默认的硬件版本号
	final static String DEF_DEVNAME = "移动伴侣";						//默认设备名称
	final static String DEF_MODEL = "T5";								//默认设备型号
	final static String DEF_MANUFACTURE = "centerm";					//默认生产厂商
	final static String DEF_DATE = "20140101";							//默认设备生产日期
	final static String DEF_SERIAL = "000000000000";					//默认设备编号
	final static String DEF_UPRESULT = "null";							//默认升级结果
	final static String UpdateSuccess = "Success";						//升级成功
	final static String UpdateFail = "Fail";							//升级失败
	final static String UpdateProcessing = "Processing";				//升级中
	
	/**
	 * 获取系统的信息项
	 * @param key key名称
	 * @param defValue 默认值
	 * @return key的值
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
			
			//加载属性文件，并返回
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
	 * 获取系统的信息项
	 * @param section 段名
	 * @param key key名称
	 * @param defValue 默认值
	 * @return key的值
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
	 * 设置系统信息
	 * @param key 键名
	 * @param value 键值
	 * @return true表示成功，false表示失败
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
			if( file.exists() == true )//若文件存在，则从文件中得到所有属性值
			{
				FileInputStream inStream = new  FileInputStream( file );	
				systemInfo.loadFromXML( inStream );
				inStream.close();
			}
			
			//开始保存新信息
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
	 * 设置系统信息
	 * @param section 段名
	 * @param key 键名
	 * @param value 键值
	 * @return true表示成功，false表示失败
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
	 * 获取系统版本号
	 */
	public static String getSystemVersion()
	{
		return getSystemInfo( "systemversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * 获取系统版本日期
	 */
	public static String getSystemBuild()
	{
		return getSystemInfo( "systembuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	/**
	 * 保存系统版本号
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
	 * 保存系统版本日期
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
	 * 获取APP版本号
	 */
	public static String getAppsVersion()
	{
		return getSystemInfo( "appsversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * 获取APP版本日期
	 */
	public static String getAppsBuild()
	{
		return getSystemInfo( "appsbuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	
	/**
	 * 保存APP版本号
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
	 * 保存APP版本日期
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
	 * 获取BIOS版本号
	 */
	public static String getBIOSVersion()
	{
		return getSystemInfo( "biosversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}
	
	/**
	 * 获取BIOS版本日期
	 */
	public static String getBIOSBuild()
	{
		return getSystemInfo( "biosbuild", DEF_BUILD, FILE_SYSTEM_INFO );
	}
	
	/**
	 * 保存BIOS版本号
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
	 * 保存BIOS版本日期
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
	 * 获取设备名称
	 */
	public static String getDevName()
	{
		return getSystemInfo( "name", DEF_DEVNAME, DEV_INFO );
	}
	
	/**
	 * 保存设备名称
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
	 * 获取设备型号
	 */
	public static String getDevModel()
	{
		return getSystemInfo( "model", DEF_MODEL, DEV_INFO );
	}
	
	/**
	 * 保存设备型号
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
	 * 获取生产厂商
	 */
	public static String getManufacture()
	{
		return getSystemInfo( "manufacture", DEF_MANUFACTURE, DEV_INFO );
	}
	
	/**
	 * 保存生产厂商
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
	 * 获取设备生产日期
	 */
	public static String getProductionDate()
	{
		return getSystemInfo( "date", DEF_DATE, DEV_INFO );
		//return new String(GlobalConfService.comService.getDevDate());
	}
	
	/**
	 * 保存设备生产日期
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
	 * 获取设备编号
	 */
	public static String getDevSerial()
	{
		return getSystemInfo( "serail", DEF_SERIAL, DEV_INFO );
		//return new String(GlobalConfService.comService.getDevSN());
	}
	
	/**
	 * 保存设备型号
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
	 * 获取硬件版本号
	 */
	public static String getHardware()
	{
		return getSystemInfo( "hardware", DEF_HARDWARE, DEV_INFO );
	}
	
	/**
	 * 保存硬件版本号
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
	 * 获取升级信息当前版本
	 */
	public static String getUpLocalVersion()
	{
		return getSystemInfo( "Update", "localver", DEF_VERSION, UPDATE_INFO );
	}
	
	/**
	 * 保存升级信息当前版本
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
	 * 获取升级信息目标版本
	 */
	public static String getUpTargetVersion()
	{
		return getSystemInfo( "Update", "targetver", DEF_VERSION, UPDATE_INFO );
	}
	
	/**
	 * 保存升级信息目标版本
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
	 * 获取升级结果
	 */
	public static String getUpdateResult()
	{
		return getSystemInfo( "Update", "result", DEF_UPRESULT, UPDATE_INFO );
	}
	
	/**
	 * 保存升级结果
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
	 * 获取升级结果明细
	 */
	public static String getUpdateDetail()
	{
		return getSystemInfo( "Update", "detail", DEF_UPRESULT, UPDATE_INFO );
	}
	
	/**
	 * 保存升级结果明细
	 */
	public static boolean setUpdateDetail( String detail )
	{
		if (detail == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "detail", detail, UPDATE_INFO );
	}
	
	// 获取子版本索引
	public static int getSubVersion() 
	{
		return RWIniClass.ReadIniInt(FILE_KEYPAD_CONFIG, "Keypad", "subversion", 0);
	}
}
