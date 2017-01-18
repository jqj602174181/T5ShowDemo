package com.centerm.dispatch.app;


import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.ipc.MessageType;

/**
 * 应用程序控制类
 *
 */
public class Application implements IAppCtrl 
{

	public static final int ACTIVITY = 1;
	public static final int SERVICE = 2;
	protected int id;			//应用程序id
	protected String name;		//应用程序类全限定名
	protected String pkg;		//应用程序包名
	protected Messenger msger;	//应用程序的messenger，通过它可以保持通信
	protected AppPriority applevel; //应用程序优先级
	protected AppStatus appstatus; //应用程序优先级
	public int startStyle= ACTIVITY;//启动方式,1Activity,2service
	public Application( int id, String name, String pkg )
	{
		this.id = id;
		this.name = name;
		this.pkg = pkg;
	}

	public int getId()
	{
		return id;
	}

	public void setId(int id) 
	{
		this.id = id;
	}
	
	public void setName(String name) 
	{
		this.name = name;
	}

	public String getPackage() 
	{
		return pkg;
	}

	public void setPackage(String pkg) 
	{
		this.pkg = pkg;
	}

	public Messenger getMessenger() 
	{
		return msger;
	}

	public void setMessenger(Messenger msger) 
	{
		this.msger = msger;
	}
	
	/**
	 * 是否完成了创建，并且一直运行在后台
	 * @return true表示激活状态，false表示未创建状态
	 */
	public boolean isCreated()
	{
		if( msger != null)
		{
			return true;
		}
		return false;
	}
	
	/**
	 * 发送消息给应用程序
	 * @param msg 待发送的消息
	 * @return true表示成功，false表示失败
	 */
	public boolean sendMessage( Message msg )
	{
		try
		{
			msger.send( msg );
			return true;
		}
		catch( Exception e )
		{
			//log和处理应用程序异常断开的问题
			return false;
		}
	}
	
	/**
	 * 等待应用程序执行完成这个Action
	 * @param action 待执行的动作
	 * @return true表示执行成功，false表示执行失败
	 */
	private boolean waitAction( Action action )
	{
		//return true;
		
		if( action.waitForDone() == Action.RESULT_OK )
		{
			return true;
		}		
		return false;
		
	}

	@Override
	public boolean create( Action action ) 
	{
		//如果msger存在，就不应该再次创建
		if( msger != null)
		{
			return true;
		}
		
		//包名和类名必须指定，否则无法创建成功
		if( pkg == null || name == null)
		{
			return false;
		}
        
		try 
		{
			//启动对应的应用程序		
			ComponentName appComponent = new ComponentName( pkg, name );
			
			
			if(startStyle==ACTIVITY){
				Intent intent = new Intent( Intent.ACTION_MAIN );
				intent.addCategory( Intent.CATEGORY_LAUNCHER );
				intent.setComponent( appComponent );
				intent.putExtra( Action.FLOWNO, action.flowNo );//附给流水号，应用程序在回馈成功创建时，应该返回流水号
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				DispatchApplication.getProcessContext().startActivity( intent );
			}else if(startStyle==SERVICE)
			{
				Intent intent = new Intent();
				intent.setComponent( appComponent );
				intent.putExtra( Action.FLOWNO, action.flowNo );//附给流水号，应用程序在回馈成功创建时，应该返回流水号
				DispatchApplication.getProcessContext().startService( intent );
			}
				
			
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		
		
		return waitAction( action );//等待应用程序启动结束，并返回结果
	}

	@Override
	public boolean start(Action action) 
	{
		
		//程序必须已在后台运行，才能发送消息
		if( false == isCreated() )
		{
			Log.i(MainActivity.TAG, "start---The program has already exited!");
			//应该先创建程序，再发送消息
			if ( create(action) == false )
			{
				Log.e(MainActivity.TAG, "start---Start the program interface operation failed!");
				return false;
			}
		}

		Log.i(MainActivity.TAG, "start---Start sending start message!");
		//以下代码只作测试用，未来一定要改
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_START;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//发送启动程序界面的消息
		
		return waitAction( action );//等待应用程序完成命令处理，并返回结果
	}

	@Override
	public boolean close(Action action) 
	{
		//程序必须已在后台运行，才能发送消息
		if( false == isCreated() )
		{
			Log.i(MainActivity.TAG, "close---The program has already exited!");
			//应该先创建程序，再发送消息
			if ( create(action) == false )
			{
				Log.e(MainActivity.TAG, "close---Start the program interface operation failed!");
				return false;
			}
		}
		
		Log.i(MainActivity.TAG, "close---Start sending start message!");
		//以下代码只作测试用，未来一定要改
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_CLOSE;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//发送关闭程序界面的消息
		
		return waitAction( action );//等待应用程序完成命令处理，并返回结果
	}

	@Override
	public boolean dealdata(Action action) 
	{
		//程序必须已在后台运行，才能发送消息
		if( false == isCreated() )
		{
			//应该先创建程序，再发送消息
			return false;
		}
		
		//以下代码只作测试用，未来一定要改
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_DATA;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//发送应用程序数据处理的消息
		
		return waitAction( action );//等待应用程序完成命令处理，并返回结果
	}

}
