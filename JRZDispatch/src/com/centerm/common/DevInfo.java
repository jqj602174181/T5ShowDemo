package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Properties;


//封装设备相关信息的获取功能
public class DevInfo {

	//EEPROM的布局, 单位是字节
	private static final int[][] EEPROM_RANGE = {
		{ 0, 15 },//0-15字节：产品序列或产品名称，多余的字节使用0x00填充。
		{ 16, 47},//16-47字节：产品序列号，多余的字节使用0x00填充。当前产线只使用13个字节
		{ 48, 55},//48-55字节：生产日期，格式是YYYYMMDD
		{ 56, 57 },//56-57字节，系统模式：取值为0x5AA5是应用模式，0x5A5A是生产模式，若为其他值，也表示生产模式
		{ 58, 59 },//58-59字节，自毁模式：取值为0x5AA5是开启自毁检测，0x5A5A是关闭自毁检测，若为其他值，也表示关闭自毁检测
		{ 60, 2047},//预留段
	};
	private static final int RANGE_NAME = 0;		//产品名称所在的range
	private static final int RANGE_SERIALNO = 1;	//产品序列号所在的range
	private static final int RANGE_MFG_DATE = 2;	//产品出厂日期
	private static final int RANGE_SYSTEM_MODE= 3;	//系统模式标志位
	private static final int RANGE_DESTROY_SWITCH= 4;//是否开启自毁检测功能的标志位

	private static final byte MODE_USER_FIRST_BYTE = (byte)0x5A;		//应用模式第1字节
	private static final byte MODE_USER_SECOND_BYTE = (byte)0xA5;		//应用模式第2字节
	private static final byte MODE_PRODUCE_FIRST_BYTE = (byte)0x5A;		//生产模式第1字节
	private static final byte MODE_PRODUCE_SECOND_BYTE = (byte)0x5A;	//生产模式第2字节
	private static final byte MODE_DESTROY_ON_FIRST_BYTE = (byte)0x5A;	//开启自毁检测第1字节
	private static final byte MODE_DESTROY_ON_SECOND_BYTE = (byte)0xA5;	//开启自毁检测第2字节
	private static final byte MODE_DESTROY_OFF_FIRST_BYTE = (byte)0x5A;	//关闭自毁检测第1字节
	private static final byte MODE_DESTROY_OFF_SECOND_BYTE = (byte)0x5A;//关闭自毁检测第2字节
	private static final String PATH_EERPOM = "/sys/devices/20072000.i2c/i2c-0/0-0050/eeprom"; //eeprom设备文件 路径

	public final static String DEV_INFO = "/mnt/internal_sd/device.xml";	//设备信息存放的文件
	public final static String UPDATE_INFO = "/mnt/internal_sd/updateinfo.ini";	 //升级信息存放的文件
	public final static String FILE_SYSTEM_INFO = "/mnt/internal_sd/config/systeminfo.xml";	 //系统版本存放的文件
	public final static String UpdateFail = "Fail";		//升级失败
	public final static String DEF_UPRESULT = "null";	//默认升级结果
	public final static String DEF_VERSION = "0.00";		//默认的版本号
	public final static String DEF_BUILD = "0000000000";	//默认的版本buid日期
	public final static String DEF_HARDWARE = "1.00";	//默认的硬件版本号
	public final static String UpdateProcessing = "Processing";	 //升级中
	public final static String UpdateSuccess = "Success";	//升级成功
	
	private static String DEF_PRODCUT_NAME = "T5";	//默认的产品名称

	/**
	 * 是否处于生成模式
	 * @return true表示是，false表示否
	 */
	public static boolean inProduceMode(){
		byte[] data = readEEPROM( RANGE_SYSTEM_MODE );
		if( data.length == 2 && data[0] == MODE_USER_FIRST_BYTE && data[1] == MODE_USER_SECOND_BYTE ){
			return false;
		}
		return true;
	}

	/**
	 * 设置系统模式为用户模式
	 * @return 成功与否
	 */
	public static boolean setSystemModeToUser(){
		byte[] appmode = { MODE_USER_FIRST_BYTE, MODE_USER_SECOND_BYTE };
		return writeEEPROM( RANGE_SYSTEM_MODE, appmode );
	}

	/**
	 * 设置系统模式为生产模式
	 * @return 成功与否
	 */
	public static boolean setSystemModeToProduce(){
		byte[] appmode = { MODE_PRODUCE_FIRST_BYTE, MODE_PRODUCE_SECOND_BYTE };
		return writeEEPROM( RANGE_SYSTEM_MODE, appmode );
	}

	/**
	 * 获得产品名称
	 * @return 产品名称
	 */
	public static String getProductName(){
		byte[] data = readEEPROM( RANGE_NAME );

		//去除末尾空数据，如果长度为0，则
		String name = new String( data ).trim();
		if( name.length() == 0 )
			return DEF_PRODCUT_NAME;
		return name;
	}

	/**
	 * 设置产品名称
	 * @param name 产品名称。若为null，将写入默认产品名称
	 * @return 是否成功设置
	 */
	public static boolean setProductName( String name ){
		if( name == null || name.length() == 0)
			name = DEF_PRODCUT_NAME;
		return writeEEPROM( RANGE_NAME, name );		
	}

	/**
	 * 获得产品序列号
	 * @return 产品序列号
	 */
	public static String getSerialNo(){
		byte[] data = readEEPROM( RANGE_SERIALNO );

		//去除末尾空数据，如果长度为0，则
		return new String( data ).trim();
	}

	/**
	 * 设置序列码
	 * @param name 产品序列号
	 * @return 是否成功设置
	 */
	public static boolean setSerialNo( String no ){
		if( no == null || no.length() == 0)
			return false;
		return writeEEPROM( RANGE_SERIALNO, no );		
	}

	/**
	 * 获得产品生产日期
	 * @return 生产日期，格式是YYYYMMDD
	 */
	public static String getManufactureDate(){
		byte[] data = readEEPROM( RANGE_MFG_DATE );

		//去除末尾空数据，如果长度为0，则
		return new String( data ).trim();
	}
	
	/**
	 * 设置当前日期为生产日期
	 * @return 是否成功设置
	 */
	public static boolean setManufactureDate( ){
		SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd");
		String date = format.format( new Date());
		return writeEEPROM( RANGE_MFG_DATE, date );		
	}

	/**
	 * 自毁检测功能是否开启
	 * @return 是否开启
	 */
	public static boolean isDestroyOn(){
		byte[] data = readEEPROM( RANGE_DESTROY_SWITCH );
		if( data.length == 2 && data[0] == MODE_DESTROY_ON_FIRST_BYTE && data[1] == MODE_DESTROY_ON_SECOND_BYTE ){
			return true;
		}
		return false;//默认是不开启
	}

	/**
	 * 开启自毁检测功能
	 * @return 是否成功设置
	 */
	public static boolean enableDestroy(){
		byte[] mode = { MODE_DESTROY_ON_FIRST_BYTE,MODE_DESTROY_ON_SECOND_BYTE };
		return writeEEPROM( RANGE_DESTROY_SWITCH, mode );
	}

	/**
	 * 关闭自毁检测功能
	 * @return 是否成功设置
	 */
	public static boolean disableDestroy(){
		byte[] mode = { MODE_DESTROY_OFF_FIRST_BYTE,MODE_DESTROY_OFF_SECOND_BYTE };
		return writeEEPROM( RANGE_DESTROY_SWITCH, mode );
	}

	//读取eeprom中的第range字段内容
	private static byte[] readEEPROM( int rangeIndex ){
		int[] range = EEPROM_RANGE[rangeIndex];
		int offset = range[0];
		int count = range[1] - range[0] + 1;
		return SystemFile.readBin( PATH_EERPOM, offset, count );
	}

	//写入eeprom中的第range字段内容
	private static boolean writeEEPROM( int rangeIndex, String data ){
		return writeEEPROM( rangeIndex, data.getBytes() );
	}

	//写入eeprom中的第range字段内容
	private static boolean writeEEPROM( int rangeIndex, byte[] data ){
		int[] range = EEPROM_RANGE[rangeIndex];
		int offset = range[0];
		int count = range[1] - range[0] + 1;

		//若写入数据够长，则填充0x00
		byte[] content = data;
		if( content.length < count ){
			byte[] buf = new byte[count];//系统自动会初始化为0x00，因此不需要后续填充
			System.arraycopy( content, 0, buf, 0, content.length );
			content = buf;
		}		
		return SystemFile.write( PATH_EERPOM, offset, content );
	}


	/**
	 * 获得eMMC存储大小
	 * @return 存储大小字节数
	 */
	public static long getRomSize(){
		String devname = "mmcblk0";
		String partitions =  readSystemFile( "/proc/partitions");
		if( partitions.length() > 0 ){//读取正确
			//分成多行，然后找到mmcblk0，取出前方的块大小
			String[] lines = partitions.split( "\n" );
			for( int i = 0; i < lines.length; i++ ){
				String line = lines[i];

				//把行字符串按空格分隔开，并取出倒数第2个(第0个是主设备号，第1个是从设备号，第2个块数，第3个是设备名称)
				if( line.endsWith( devname ) ){
					String item = getLineItem( line, 2 );
					long blkcnt = Long.parseLong( item );
					return blkcnt * 1024;
				}
			}
		}
		return 0;
	}

	//处理一行数据，去除空格，返回第index个字符串，index从0开始
	private static String getLineItem( String line, int index ){
		String[] items = line.split( " " );
		for( int i = 0, n = 0; i < items.length; i++ ){
			String item = items[i].trim();
			if( item.length() == 0 )//去除全为空格的项
				continue;

			if( n == index ){
				return item;
			}
			n++;
		}
		return "";
	}

	/**
	 * 从系统文件中读出数据，返回String类型
	 */
	private static String readSystemFile( String path ){
		return SystemFile.read( path );
	}

	/**
	 * 获取自动升级标识
	 */
	public static String getUpdateAuto()
	{
		return getSystemInfo( "Update", "auto", "false", UPDATE_INFO );
	}

	/**
	 * 保存自动升级标识
	 */
	public static boolean setUpdateAuto( String flag )
	{
		if (flag == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "auto", flag, UPDATE_INFO );
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
	 * 获取APP版本号
	 */
	public static String getAppsVersion()
	{
		return getSystemInfo( "appsversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}

	/**
	 * 获取硬件版本号
	 */
	public static String getHardware()
	{
		return getSystemInfo( "hardware", DEF_HARDWARE, DEV_INFO );
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

	public static boolean setUpdateComplete( String flag )
	{
		if (flag == null)
		{
			return false;
		}
		return setSystemInfo( "Update", "complete", flag, UPDATE_INFO );
	}

	public static String getUpdateComplete()
	{
		return getSystemInfo( "Update", "complete", "false", UPDATE_INFO );
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
	 * 获取升级结果
	 */
	public static String getUpdateResult()
	{
		return getSystemInfo( "Update", "result", DEF_UPRESULT, UPDATE_INFO );
	}

}
