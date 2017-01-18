package com.centerm.dispatch.instruction;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import android.util.Log;
import com.centerm.common.ByteHandlerClass;
import com.centerm.dispatch.MainActivity;

/**
 * ����VersionHandler�ĸ��࣬�ṩ�˵������ຯ���������
 */
public final class VersionHandler implements Command
{
	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public byte[] executeCommand(byte[] data, Integer nLinkType) 
	{
		byte[][] tempdata = new byte[2][];
		byte[] retdata = null;
		
		Log.i(MainActivity.TAG, "executeCommand dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		
		//�ṩһ��ִ�к����Ļ��ƣ����÷������ִ�ж�Ӧ������				
		if( tempdata[0] != null )//���������벻Ϊ��
		{
			//��ȡ����
			String clsName = new String(tempdata[0]);

			try 
			{
				//�ҵ��࣬��ʵ����
				Class cls = Class.forName( "com.centerm.dispatch.instruction." + clsName );
				Object obj = cls.newInstance();
				
				//��������󷽷�
				Method method = cls.getMethod( "executeCommand" , byte[].class, Integer.class );
				retdata = (byte[])method.invoke( obj, tempdata[1], Integer.valueOf(nLinkType) );
			} 
			catch (ClassNotFoundException e1) 
			{
				e1.printStackTrace();
			} 
			catch (InstantiationException e) 
			{
				e.printStackTrace();
			} 
			catch (IllegalAccessException e) 
			{
				e.printStackTrace();
			}
			catch (NoSuchMethodException e1) 
			{
				e1.printStackTrace();
			}
			catch (IllegalArgumentException e) 
			{
				e.printStackTrace();
			} 
			catch (InvocationTargetException e) 
			{
				e.printStackTrace();
			}
		}
		
		return retdata;
	}
}
