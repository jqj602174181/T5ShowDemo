package com.centerm.dispatch;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

public class DeviceInfo {
	private static final Uri CONTENT_URI = 
			Uri.parse( "content://com.centerm.remoteserver.deviceInfo/infos");

	//�б������
	private static final String KEY_NAME = "name";	//��������
	private static final String KEY_VALUE = "value";	//���õ�ֵ
	private Context context;//������
	public static final String systemVersionKey 		= "SystemVersion";
	public static final String financialVersionKey		="FinancialVersion";
	public static final String jiaMiVersionKey			="JiaMiVersion";
	public static final String serializableKey			= "serializable";
	public static final String dataKey					= "data";
	public static final String macAddrKey				= "macAddr";
	public static final String appVersionKey			="appVersion";
	public static final String fingerKey 				= "finger";
	public static final String financialUpateVersionKey = "FinancialUpdateVersion";//����ʱ�õ��İ汾
	
	private DeviceInfo()
	{
		
	}
	
	public  DeviceInfo(Context context)
	{
		this.context = context;
	}
	
	
	//��������ֵ
		private boolean setConf( String name, String value ){
			ContentResolver cr = context.getContentResolver();
			
			//����������
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
			
			//����������
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
	//������� 
		private String getConf( String name ){
			ContentResolver cr = context.getContentResolver();

			//���ɲ���
			String[] results_columns = new String[]{
					KEY_NAME,
					KEY_VALUE
			};		
			String selection = KEY_NAME + "='" + name + "'";
			String[] selectionArgs = null;
			String sortOrder = null;

			//��ѯ���ݿ�
			Cursor cursor = cr.query( CONTENT_URI,
					results_columns, selection, selectionArgs, sortOrder);

			//�õ�ֵ�󷵻أ�Ĭ��ֵ�ǿ�
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
		 * ��ȡϵͳ�汾��
		 */
		public String getSystemVersion()
		{
			return getConf(systemVersionKey);
		}
		/*
		 *����ϵͳ�汾��
		 */
		public boolean setSystemVersion(String systemVersion)
		{
			return setConf(systemVersionKey, systemVersion);
		}
		
		/*
		 * ��ȡ�����õ��Ľ���ģ��汾��
		 */
		public String getFinancialUpdateVersion()
		{
			return getConf(financialUpateVersionKey);
		}
		/*
		 * ���������õ��Ľ���ģ��汾��
		 */
		public boolean setFinancialUpdateVersion(String financialVersion)
		{
			return setConf(financialUpateVersionKey, financialVersion);
		}
		/*
		 * ��ȡ��ʾ�õ��Ľ���ģ��汾��
		 */
		public String getFinancialVersion()
		{
			return getConf(financialVersionKey);
		}
		/*
		 * ������ʾ�õ��Ľ���ģ��汾��
		 */
		public boolean setFinancialVersion(String financialVersion)
		{
			return setConf(financialVersionKey, financialVersion);
		}
		/*
		 * ���¼���оƬ���汾��
		 */
		public boolean setJiaMiVersion(String jiaMiVersion)
		{
			return setConf(jiaMiVersionKey, jiaMiVersion);
		}
		/*
		 * ��ü���оƬ���汾��
		 */
		public String getJiaMiVersion()
		{
			return getConf(jiaMiVersionKey);
		}

		/*
		 * ���³�������
		 */
		public boolean setData(String data)
		{
			return setConf(dataKey,data);
		}
		/*
		 * ��ó�������
		 */
		public String getData()
		{
			return getConf(dataKey);
		}
		
		/*
		 * �������к�
		 */
		public boolean setSerializable(String serializable)
		{
			return setConf(serializableKey,serializable);
		}
		/*
		 * ������к�
		 */
		public String getSerializable()
		{
			return getConf(serializableKey);
		}
		
		/*
		 * ����������ַ
		 */
		public boolean setMacAddr(String macAddr)
		{
			return setConf(macAddrKey, macAddr);
		}
		/*
		 * ���������ַ
		 */
		public String getMacAddr()
		{
			return getConf(macAddrKey);
		}
		
		
		/*
		 * ����ָ���ǰ汾
		 */
		public boolean  setFingerVersion(String fingerVersion)
		{
			return setConf(fingerKey, fingerVersion);
		}
		
	
		/*
		 * ���ָ���ǰ汾
		 */
		public String getFingerVersion()
		{
			return getConf(fingerKey);
		}
		
		
		/*
		 * �����ݿ��в���ϵͳ�汾
		 */
		public boolean insertSystemVersion(String systemVersion)
		{
			return insertConf(systemVersionKey, systemVersion);
		}
		
		
		/*
		 * �����ݿ��в�����ʾ�õĽ��ڰ汾
		 */
		public boolean insertFinancialVersion(String financialVersion)
		{
			return insertConf(financialVersionKey, financialVersion);
		}
		/*
		 * �����ݿ��в�������õĽ��ڰ汾
		 */
		public boolean  insertFinancialUpdateVersion(String fingerVersion)
		{
			return setConf(financialUpateVersionKey, fingerVersion);
		}
		/*
		 * �����ݿ�������оƬ�汾
		 */
		public boolean insertJiaMiVersion(String jiaMiVersion)
		{
			return insertConf(jiaMiVersionKey, jiaMiVersion);
		}
		
		
		/*
		 * �����ݿ�����������
		 */
		public boolean insertData(String data)
		{
			return insertConf(dataKey,data);
		}
	
		
		/*
		 * �����ݿ�������к�
		 */
		public boolean insertSerializable(String serializable)
		{
			return insertConf(serializableKey,serializable);
		}
		/*
		 * �����ݿ����������ַ
		 */
		public boolean insertMacAddr(String macAddr)
		{
			return insertConf(macAddrKey, macAddr);
		}
		/*
		 * �����ݿ����ָ���ǰ汾
		 */
		public boolean  insertFingerVersion(String fingerVersion)
		{
			return insertConf(fingerKey, fingerVersion);
		}
		/*
		 * �����ݿ����Ӧ�ð汾
		 */
		public boolean insertAppVersion(String appVersion)
		{
			return insertConf(appVersionKey, appVersion);
		}
		
		/*
		 * ����Ӧ�ð汾
		 */
		public boolean  setAppVersion(String appVersion)
		{
			return setConf(appVersionKey, appVersion);
		}
		/*
		 * ��ȡӦ�ð汾
		 */
		public String getAppVersion()
		{
			return getConf(appVersionKey);
		}
		
}
