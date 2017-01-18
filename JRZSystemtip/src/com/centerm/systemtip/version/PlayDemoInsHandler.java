package com.centerm.systemtip.version;


import java.lang.reflect.Method;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.centerm.common.ByteHandlerClass;
import com.centerm.systemtip.MainActivity;
import com.centerm.systemtip.PlayDemoActivity;

@SuppressLint("DefaultLocale")
public final class PlayDemoInsHandler implements Command
{
	
	public static Class<?> playDemoActivity = PlayDemoActivity.class ;
	@Override
	public void executeCommand(byte[] data, Integer nLinkType, Integer nFlowNo) 
	{
		byte[][] tempdata = new byte[2][];
		
		Log.i(MainActivity.TAG, "PlayDemoInsHandler dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		
		if( tempdata[0] != null )//���������벻Ϊ��
		{
			String methodname = new String(tempdata[0]);
			try	
			{
				Method method = this.getClass().getMethod( methodname , byte[].class, Integer.class, Integer.class  );
				method.invoke( this, tempdata[1], nLinkType, nFlowNo);//ִ������
			}
			catch( NoSuchMethodException e )
			{
				Log.w(MainActivity.TAG, this.getClass().getName() + ":Not find method " + methodname + "!");
			}				
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
	}
	
	//��ȡ��ֵ
	public byte[] getKeyValue( String key, byte[] data )
	{
		if (key == null && data == null)
		{
			return null;
		}
		
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'=');
		if (tempdata[0] != null && new String(tempdata[0]).equals(key))
		{
			return tempdata[1];
		}
		 
		return null;
	}
	
	//����ָ�����ģ��
	public void openPlayDemo( byte[] data, Integer linkType, Integer nFlowNo )
	{
		int playType = 0;
		byte[] playData = getKeyValue("data", data);
		if (playData != null) {
			playType = (playData[0] & 0x0F);
			
			Context context = MainActivity.getProcessContext().getApplicationContext();//��ȡ������
			Intent intent = new Intent();
			intent.putExtra("linktype", linkType.intValue());
			intent.putExtra("playtype", playType);
			try
			{
				intent.setClass( context, playDemoActivity );
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				context.startActivity(intent);//����activity
			}
			catch (Exception e)
			{
				Log.e("TIP", "Can't not start PlayDemoActivity.class!");
			}
		}
		
	}
	
	//������ʾ������ʱʱ��
	public void setTimeout( byte[] data, Integer linkType, Integer nFlowNo )
	{
		
	}
}
