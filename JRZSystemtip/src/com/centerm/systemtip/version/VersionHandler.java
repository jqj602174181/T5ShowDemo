package com.centerm.systemtip.version;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.util.Log;

import com.centerm.common.ByteHandlerClass;
import com.centerm.systemtip.MainActivity;

/**
 * 所有VersionHandler的父类，提供了调用子类函数反射机制
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
		
		//提供一种执行函数的机制，采用反射机制执行对应的命令				
		if( tempdata[0] != null )//函数名必须不为空
		{
			//获取类名
			String clsName = new String(tempdata[0]);

			try 
			{
				//找到类，并实例化
				Class cls = Class.forName( "com.centerm.systemtip.version." + clsName );
				Object obj = cls.newInstance();
				
				//调用类对象方法
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
