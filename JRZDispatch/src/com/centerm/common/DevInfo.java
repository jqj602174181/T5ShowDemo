package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Properties;


//��װ�豸�����Ϣ�Ļ�ȡ����
public class DevInfo {

	//EEPROM�Ĳ���, ��λ���ֽ�
	private static final int[][] EEPROM_RANGE = {
		{ 0, 15 },//0-15�ֽڣ���Ʒ���л��Ʒ���ƣ�������ֽ�ʹ��0x00��䡣
		{ 16, 47},//16-47�ֽڣ���Ʒ���кţ�������ֽ�ʹ��0x00��䡣��ǰ����ֻʹ��13���ֽ�
		{ 48, 55},//48-55�ֽڣ��������ڣ���ʽ��YYYYMMDD
		{ 56, 57 },//56-57�ֽڣ�ϵͳģʽ��ȡֵΪ0x5AA5��Ӧ��ģʽ��0x5A5A������ģʽ����Ϊ����ֵ��Ҳ��ʾ����ģʽ
		{ 58, 59 },//58-59�ֽڣ��Ի�ģʽ��ȡֵΪ0x5AA5�ǿ����Իټ�⣬0x5A5A�ǹر��Իټ�⣬��Ϊ����ֵ��Ҳ��ʾ�ر��Իټ��
		{ 60, 2047},//Ԥ����
	};
	private static final int RANGE_NAME = 0;		//��Ʒ�������ڵ�range
	private static final int RANGE_SERIALNO = 1;	//��Ʒ���к����ڵ�range
	private static final int RANGE_MFG_DATE = 2;	//��Ʒ��������
	private static final int RANGE_SYSTEM_MODE= 3;	//ϵͳģʽ��־λ
	private static final int RANGE_DESTROY_SWITCH= 4;//�Ƿ����Իټ�⹦�ܵı�־λ

	private static final byte MODE_USER_FIRST_BYTE = (byte)0x5A;		//Ӧ��ģʽ��1�ֽ�
	private static final byte MODE_USER_SECOND_BYTE = (byte)0xA5;		//Ӧ��ģʽ��2�ֽ�
	private static final byte MODE_PRODUCE_FIRST_BYTE = (byte)0x5A;		//����ģʽ��1�ֽ�
	private static final byte MODE_PRODUCE_SECOND_BYTE = (byte)0x5A;	//����ģʽ��2�ֽ�
	private static final byte MODE_DESTROY_ON_FIRST_BYTE = (byte)0x5A;	//�����Իټ���1�ֽ�
	private static final byte MODE_DESTROY_ON_SECOND_BYTE = (byte)0xA5;	//�����Իټ���2�ֽ�
	private static final byte MODE_DESTROY_OFF_FIRST_BYTE = (byte)0x5A;	//�ر��Իټ���1�ֽ�
	private static final byte MODE_DESTROY_OFF_SECOND_BYTE = (byte)0x5A;//�ر��Իټ���2�ֽ�
	private static final String PATH_EERPOM = "/sys/devices/20072000.i2c/i2c-0/0-0050/eeprom"; //eeprom�豸�ļ� ·��

	public final static String DEV_INFO = "/mnt/internal_sd/device.xml";	//�豸��Ϣ��ŵ��ļ�
	public final static String UPDATE_INFO = "/mnt/internal_sd/updateinfo.ini";	 //������Ϣ��ŵ��ļ�
	public final static String FILE_SYSTEM_INFO = "/mnt/internal_sd/config/systeminfo.xml";	 //ϵͳ�汾��ŵ��ļ�
	public final static String UpdateFail = "Fail";		//����ʧ��
	public final static String DEF_UPRESULT = "null";	//Ĭ���������
	public final static String DEF_VERSION = "0.00";		//Ĭ�ϵİ汾��
	public final static String DEF_BUILD = "0000000000";	//Ĭ�ϵİ汾buid����
	public final static String DEF_HARDWARE = "1.00";	//Ĭ�ϵ�Ӳ���汾��
	public final static String UpdateProcessing = "Processing";	 //������
	public final static String UpdateSuccess = "Success";	//�����ɹ�
	
	private static String DEF_PRODCUT_NAME = "T5";	//Ĭ�ϵĲ�Ʒ����

	/**
	 * �Ƿ�������ģʽ
	 * @return true��ʾ�ǣ�false��ʾ��
	 */
	public static boolean inProduceMode(){
		byte[] data = readEEPROM( RANGE_SYSTEM_MODE );
		if( data.length == 2 && data[0] == MODE_USER_FIRST_BYTE && data[1] == MODE_USER_SECOND_BYTE ){
			return false;
		}
		return true;
	}

	/**
	 * ����ϵͳģʽΪ�û�ģʽ
	 * @return �ɹ����
	 */
	public static boolean setSystemModeToUser(){
		byte[] appmode = { MODE_USER_FIRST_BYTE, MODE_USER_SECOND_BYTE };
		return writeEEPROM( RANGE_SYSTEM_MODE, appmode );
	}

	/**
	 * ����ϵͳģʽΪ����ģʽ
	 * @return �ɹ����
	 */
	public static boolean setSystemModeToProduce(){
		byte[] appmode = { MODE_PRODUCE_FIRST_BYTE, MODE_PRODUCE_SECOND_BYTE };
		return writeEEPROM( RANGE_SYSTEM_MODE, appmode );
	}

	/**
	 * ��ò�Ʒ����
	 * @return ��Ʒ����
	 */
	public static String getProductName(){
		byte[] data = readEEPROM( RANGE_NAME );

		//ȥ��ĩβ�����ݣ��������Ϊ0����
		String name = new String( data ).trim();
		if( name.length() == 0 )
			return DEF_PRODCUT_NAME;
		return name;
	}

	/**
	 * ���ò�Ʒ����
	 * @param name ��Ʒ���ơ���Ϊnull����д��Ĭ�ϲ�Ʒ����
	 * @return �Ƿ�ɹ�����
	 */
	public static boolean setProductName( String name ){
		if( name == null || name.length() == 0)
			name = DEF_PRODCUT_NAME;
		return writeEEPROM( RANGE_NAME, name );		
	}

	/**
	 * ��ò�Ʒ���к�
	 * @return ��Ʒ���к�
	 */
	public static String getSerialNo(){
		byte[] data = readEEPROM( RANGE_SERIALNO );

		//ȥ��ĩβ�����ݣ��������Ϊ0����
		return new String( data ).trim();
	}

	/**
	 * ����������
	 * @param name ��Ʒ���к�
	 * @return �Ƿ�ɹ�����
	 */
	public static boolean setSerialNo( String no ){
		if( no == null || no.length() == 0)
			return false;
		return writeEEPROM( RANGE_SERIALNO, no );		
	}

	/**
	 * ��ò�Ʒ��������
	 * @return �������ڣ���ʽ��YYYYMMDD
	 */
	public static String getManufactureDate(){
		byte[] data = readEEPROM( RANGE_MFG_DATE );

		//ȥ��ĩβ�����ݣ��������Ϊ0����
		return new String( data ).trim();
	}
	
	/**
	 * ���õ�ǰ����Ϊ��������
	 * @return �Ƿ�ɹ�����
	 */
	public static boolean setManufactureDate( ){
		SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd");
		String date = format.format( new Date());
		return writeEEPROM( RANGE_MFG_DATE, date );		
	}

	/**
	 * �Իټ�⹦���Ƿ���
	 * @return �Ƿ���
	 */
	public static boolean isDestroyOn(){
		byte[] data = readEEPROM( RANGE_DESTROY_SWITCH );
		if( data.length == 2 && data[0] == MODE_DESTROY_ON_FIRST_BYTE && data[1] == MODE_DESTROY_ON_SECOND_BYTE ){
			return true;
		}
		return false;//Ĭ���ǲ�����
	}

	/**
	 * �����Իټ�⹦��
	 * @return �Ƿ�ɹ�����
	 */
	public static boolean enableDestroy(){
		byte[] mode = { MODE_DESTROY_ON_FIRST_BYTE,MODE_DESTROY_ON_SECOND_BYTE };
		return writeEEPROM( RANGE_DESTROY_SWITCH, mode );
	}

	/**
	 * �ر��Իټ�⹦��
	 * @return �Ƿ�ɹ�����
	 */
	public static boolean disableDestroy(){
		byte[] mode = { MODE_DESTROY_OFF_FIRST_BYTE,MODE_DESTROY_OFF_SECOND_BYTE };
		return writeEEPROM( RANGE_DESTROY_SWITCH, mode );
	}

	//��ȡeeprom�еĵ�range�ֶ�����
	private static byte[] readEEPROM( int rangeIndex ){
		int[] range = EEPROM_RANGE[rangeIndex];
		int offset = range[0];
		int count = range[1] - range[0] + 1;
		return SystemFile.readBin( PATH_EERPOM, offset, count );
	}

	//д��eeprom�еĵ�range�ֶ�����
	private static boolean writeEEPROM( int rangeIndex, String data ){
		return writeEEPROM( rangeIndex, data.getBytes() );
	}

	//д��eeprom�еĵ�range�ֶ�����
	private static boolean writeEEPROM( int rangeIndex, byte[] data ){
		int[] range = EEPROM_RANGE[rangeIndex];
		int offset = range[0];
		int count = range[1] - range[0] + 1;

		//��д�����ݹ����������0x00
		byte[] content = data;
		if( content.length < count ){
			byte[] buf = new byte[count];//ϵͳ�Զ����ʼ��Ϊ0x00����˲���Ҫ�������
			System.arraycopy( content, 0, buf, 0, content.length );
			content = buf;
		}		
		return SystemFile.write( PATH_EERPOM, offset, content );
	}


	/**
	 * ���eMMC�洢��С
	 * @return �洢��С�ֽ���
	 */
	public static long getRomSize(){
		String devname = "mmcblk0";
		String partitions =  readSystemFile( "/proc/partitions");
		if( partitions.length() > 0 ){//��ȡ��ȷ
			//�ֳɶ��У�Ȼ���ҵ�mmcblk0��ȡ��ǰ���Ŀ��С
			String[] lines = partitions.split( "\n" );
			for( int i = 0; i < lines.length; i++ ){
				String line = lines[i];

				//�����ַ������ո�ָ�������ȡ��������2��(��0�������豸�ţ���1���Ǵ��豸�ţ���2����������3�����豸����)
				if( line.endsWith( devname ) ){
					String item = getLineItem( line, 2 );
					long blkcnt = Long.parseLong( item );
					return blkcnt * 1024;
				}
			}
		}
		return 0;
	}

	//����һ�����ݣ�ȥ���ո񣬷��ص�index���ַ�����index��0��ʼ
	private static String getLineItem( String line, int index ){
		String[] items = line.split( " " );
		for( int i = 0, n = 0; i < items.length; i++ ){
			String item = items[i].trim();
			if( item.length() == 0 )//ȥ��ȫΪ�ո����
				continue;

			if( n == index ){
				return item;
			}
			n++;
		}
		return "";
	}

	/**
	 * ��ϵͳ�ļ��ж������ݣ�����String����
	 */
	private static String readSystemFile( String path ){
		return SystemFile.read( path );
	}

	/**
	 * ��ȡ�Զ�������ʶ
	 */
	public static String getUpdateAuto()
	{
		return getSystemInfo( "Update", "auto", "false", UPDATE_INFO );
	}

	/**
	 * �����Զ�������ʶ
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
	 * ��ȡAPP�汾��
	 */
	public static String getAppsVersion()
	{
		return getSystemInfo( "appsversion", DEF_VERSION, FILE_SYSTEM_INFO );
	}

	/**
	 * ��ȡӲ���汾��
	 */
	public static String getHardware()
	{
		return getSystemInfo( "hardware", DEF_HARDWARE, DEV_INFO );
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
	 * ��ȡ�������
	 */
	public static String getUpdateResult()
	{
		return getSystemInfo( "Update", "result", DEF_UPRESULT, UPDATE_INFO );
	}

}
