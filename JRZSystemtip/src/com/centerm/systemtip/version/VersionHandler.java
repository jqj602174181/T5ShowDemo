package com.centerm.systemtip.version;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.util.Log;

import com.centerm.common.ByteHandlerClass;
import com.centerm.systemtip.MainActivity;

/**
 * ����VersionHandler�ĸ��࣬�ṩ�˵������ຯ���������
 */
public final class VersionHandler implements Command
{
	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public void executeCommand(byte[] data, Integer nLinkType, Integer nFlowNo) 
	{
		byte[][] tempdata = new byte[2][];
		
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
				Class cls = Class.forName( "com.centerm.systemtip.version." + clsName );
				Object obj = cls.newInstance();
				
				//��������󷽷�
				Method method = cls.getMethod( "executeCommand" , byte[].class, Integer.class, Integer.class );
				method.invoke( obj, tempdata[1], nLinkType, nFlowNo );
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
	}
}
