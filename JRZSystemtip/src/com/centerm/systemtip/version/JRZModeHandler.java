package com.centerm.systemtip.version;

import java.lang.reflect.Method;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.centerm.common.ByteHandlerClass;
import com.centerm.systemtip.ICcardActivity;
import com.centerm.systemtip.IDcardActivity;
import com.centerm.systemtip.MagcardActivity;
import com.centerm.systemtip.MainActivity;

@SuppressLint("DefaultLocale")
public final class JRZModeHandler implements Command
{
	@Override
	public void executeCommand(byte[] data, Integer nLinkType, Integer nFlowNo) 
	{
		byte[][] tempdata = new byte[2][];

		Log.i(MainActivity.TAG, "JRZModeHandler dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');

		if( tempdata[0] != null )//函数名必须不为空
		{
			String methodname = new String(tempdata[0]);
			try	
			{
				Method method = this.getClass().getMethod( methodname , byte[].class, Integer.class, Integer.class  );
				method.invoke( this, tempdata[1], nLinkType, nFlowNo);//执行命令
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

	//获取健值
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

	//磁卡
	public void startMagcardActivity( byte[] data, Integer linkType, Integer nFlowNo )
	{
		Context context = MainActivity.getProcessContext().getApplicationContext();//获取上下文
		Intent intent = new Intent();
		intent.putExtra("linktype", linkType.intValue());
		byte[] tmpdata = getKeyValue("data", data);
		if (tmpdata != null){
			try
			{
				intent.putExtra("timeout", new String(tmpdata));
				intent.setClass( context, MagcardActivity.class );
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				context.startActivity(intent);//启动activity
			}
			catch (Exception e)
			{
				Log.e("TIP", "Can't not start MagcardActivity.class!");
			}
		}
	}

	//IC卡
	public void startICcardActivity( byte[] data, Integer linkType, Integer nFlowNo )
	{
		Context context = MainActivity.getProcessContext().getApplicationContext();//获取上下文
		Intent intent = new Intent();
		intent.putExtra("linktype", linkType.intValue());
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] tmpdata = getKeyValue("timeout", tempdata[0]);
		byte[] value = getKeyValue("flag", tempdata[1]);
		if (tmpdata != null && value != null){
			try
			{
				intent.putExtra("timeout", new String(tmpdata));
				intent.putExtra("flag", new String(value)); //接触 非接
				intent.setClass( context, ICcardActivity.class );
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				context.startActivity(intent);//启动activity
			}
			catch (Exception e)
			{
				Log.e("TIP", "Can't not start ICcardActivity.class!");
			}
		}
	}

	//二代证
	public void startIDcardActivity( byte[] data, Integer linkType, Integer nFlowNo )
	{
		Context context = MainActivity.getProcessContext().getApplicationContext();//获取上下文
		Intent intent = new Intent();
		intent.putExtra("linktype", linkType.intValue());
		byte[] tmpdata = getKeyValue("data", data);
		if (tmpdata != null){
			try
			{
				intent.putExtra("timeout", new String(tmpdata));
				intent.setClass( context, IDcardActivity.class );
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				context.startActivity(intent);//启动activity
			}
			catch (Exception e)
			{
				Log.e("TIP", "Can't not start IDcardActivity.class!");
			}
		}
	}
}
