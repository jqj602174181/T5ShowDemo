package com.centerm.dispatch;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

public class DeviceInfo {
	private static final Uri CONTENT_URI = 
			Uri.parse( "content://com.centerm.remoteserver.deviceInfo/infos");

	//列表变量名
	private static final String KEY_NAME = "name";	//配置名称
	private static final String KEY_VALUE = "value";	//配置的值
	private Context context;//上下文
	public static final String systemVersionKey 		= "SystemVersion";
	public static final String financialVersionKey		="FinancialVersion";
	public static final String jiaMiVersionKey			="JiaMiVersion";
	public static final String serializableKey			= "serializable";
	public static final String dataKey					= "data";
	public static final String macAddrKey				= "macAddr";
	public static final String appVersionKey			="appVersion";
	public static final String fingerKey 				= "finger";
	public static final String financialUpateVersionKey = "FinancialUpdateVersion";//升级时用到的版本
	
	private DeviceInfo()
	{
		
	}
	
	public  DeviceInfo(Context context)
	{
		this.context = context;
	}
	
	
	//更新配置值
		private boolean setConf( String name, String value ){
			ContentResolver cr = context.getContentResolver();
			
			//生成行数据
			ContentValues updateValues = new ContentValues();
			updateValues.put( KEY_VALUE, value );
			
			String where = KEY_NAME + "='" + name + "'";;
			String[] selectionArgs = null;
			int updateRowCount = cr.update(CONTENT_URI,
					updateValues, where, selectionArgs );
			
		
			if( updateRowCount > 0 ){
				return true;
			}else{
				return false;
			}
		}
		
		
		private boolean insertConf(String name,String value)
		{
			ContentResolver cr = context.getContentResolver();
			
			//生成行数据
			ContentValues insertValues = new ContentValues();
			insertValues.put( KEY_NAME, name );
			insertValues.put( KEY_VALUE, value );
	
			Uri url = cr.insert(CONTENT_URI, insertValues);
			
			if( url != null ){
				return true;
			}else{
				return false;
			}
		}
	//获得配置 
		private String getConf( String name ){
			ContentResolver cr = context.getContentResolver();

			//生成参数
			String[] results_columns = new String[]{
					KEY_NAME,
					KEY_VALUE
			};		
			String selection = KEY_NAME + "='" + name + "'";
			String[] selectionArgs = null;
			String sortOrder = null;

			//查询数据库
			Cursor cursor = cr.query( CONTENT_URI,
					results_columns, selection, selectionArgs, sortOrder);

			//得到值后返回，默认值是空
			String value = null;
			if(cursor!=null){
				if( cursor.moveToFirst()){
					int columnIndex = cursor.getColumnIndexOrThrow( KEY_VALUE );
					value = cursor.getString( columnIndex );
				}
				cursor.close();
			}
			return value;		
		}	
		
		/*
		 * 获取系统版本号
		 */
		public String getSystemVersion()
		{
			return getConf(systemVersionKey);
		}
		/*
		 *更新系统版本号
		 */
		public boolean setSystemVersion(String systemVersion)
		{
			return setConf(systemVersionKey, systemVersion);
		}
		
		/*
		 * 获取升级用到的金融模块版本号
		 */
		public String getFinancialUpdateVersion()
		{
			return getConf(financialUpateVersionKey);
		}
		/*
		 * 设置升级用到的金融模块版本号
		 */
		public boolean setFinancialUpdateVersion(String financialVersion)
		{
			return setConf(financialUpateVersionKey, financialVersion);
		}
		/*
		 * 获取显示用到的金融模块版本号
		 */
		public String getFinancialVersion()
		{
			return getConf(financialVersionKey);
		}
		/*
		 * 更新显示用到的金融模块版本号
		 */
		public boolean setFinancialVersion(String financialVersion)
		{
			return setConf(financialVersionKey, financialVersion);
		}
		/*
		 * 更新加密芯片金块版本号
		 */
		public boolean setJiaMiVersion(String jiaMiVersion)
		{
			return setConf(jiaMiVersionKey, jiaMiVersion);
		}
		/*
		 * 获得加密芯片金块版本号
		 */
		public String getJiaMiVersion()
		{
			return getConf(jiaMiVersionKey);
		}

		/*
		 * 更新出厂日期
		 */
		public boolean setData(String data)
		{
			return setConf(dataKey,data);
		}
		/*
		 * 获得出厂日期
		 */
		public String getData()
		{
			return getConf(dataKey);
		}
		
		/*
		 * 更新序列号
		 */
		public boolean setSerializable(String serializable)
		{
			return setConf(serializableKey,serializable);
		}
		/*
		 * 获得序列号
		 */
		public String getSerializable()
		{
			return getConf(serializableKey);
		}
		
		/*
		 * 设置蓝牙地址
		 */
		public boolean setMacAddr(String macAddr)
		{
			return setConf(macAddrKey, macAddr);
		}
		/*
		 * 获得蓝牙地址
		 */
		public String getMacAddr()
		{
			return getConf(macAddrKey);
		}
		
		
		/*
		 * 更新指纹仪版本
		 */
		public boolean  setFingerVersion(String fingerVersion)
		{
			return setConf(fingerKey, fingerVersion);
		}
		
	
		/*
		 * 获得指纹仪版本
		 */
		public String getFingerVersion()
		{
			return getConf(fingerKey);
		}
		
		
		/*
		 * 向数据库中插入系统版本
		 */
		public boolean insertSystemVersion(String systemVersion)
		{
			return insertConf(systemVersionKey, systemVersion);
		}
		
		
		/*
		 * 向数据库中插入显示用的金融版本
		 */
		public boolean insertFinancialVersion(String financialVersion)
		{
			return insertConf(financialVersionKey, financialVersion);
		}
		/*
		 * 向数据库中插入更新用的金融版本
		 */
		public boolean  insertFinancialUpdateVersion(String fingerVersion)
		{
			return setConf(financialUpateVersionKey, fingerVersion);
		}
		/*
		 * 向数据库插入加密芯片版本
		 */
		public boolean insertJiaMiVersion(String jiaMiVersion)
		{
			return insertConf(jiaMiVersionKey, jiaMiVersion);
		}
		
		
		/*
		 * 向数据库插入出厂日期
		 */
		public boolean insertData(String data)
		{
			return insertConf(dataKey,data);
		}
	
		
		/*
		 * 向数据库插入序列号
		 */
		public boolean insertSerializable(String serializable)
		{
			return insertConf(serializableKey,serializable);
		}
		/*
		 * 向数据库插入蓝牙地址
		 */
		public boolean insertMacAddr(String macAddr)
		{
			return insertConf(macAddrKey, macAddr);
		}
		/*
		 * 向数据库插入指纹仪版本
		 */
		public boolean  insertFingerVersion(String fingerVersion)
		{
			return insertConf(fingerKey, fingerVersion);
		}
		/*
		 * 向数据库插入应用版本
		 */
		public boolean insertAppVersion(String appVersion)
		{
			return insertConf(appVersionKey, appVersion);
		}
		
		/*
		 * 更新应用版本
		 */
		public boolean  setAppVersion(String appVersion)
		{
			return setConf(appVersionKey, appVersion);
		}
		/*
		 * 获取应用版本
		 */
		public String getAppVersion()
		{
			return getConf(appVersionKey);
		}
		
}
