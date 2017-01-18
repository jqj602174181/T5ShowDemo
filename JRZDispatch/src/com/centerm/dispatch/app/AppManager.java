package com.centerm.dispatch.app;



import java.util.List;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.os.Messenger;
import android.os.SystemClock;
import android.util.Log;
import android.util.SparseArray;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.communication.CommandEvent;

//！！！未来得好好考虑一下该对象的内部变量访问同步问题
/**
 * 负责管理各个应用程序控制对象
 */
public class AppManager 
{	
	 
	private static AppManager manager = new AppManager();	//管理器单一实例
	private SparseArray<Application> appTable;				//应用程序列表
	private SparseArray<Action> activeActionSet;			//记录处于激活状态的流水号所对应的Action
	
	/**
	 * 采用private，保证无法被外部实例化，从而实现单一实例
	 */
	private AppManager()
	{
		appTable = new SparseArray<Application>();
		activeActionSet = new SparseArray<Action>();
	}
	
	/**
	 * 获得应用程序管理的单一实例对象
	 * @return 应用程序管理器对象
	 */
	static public AppManager getManager()
	{
		return manager;
	}
	
	/**
	 * 将应用程序控制对象注册到管理列表中
	 * @param app 应用程序控制对象
	 * @return true表示注册成功，false表示注册失败。失败的原因是进行了重复注册。
	 */
	public boolean registerApp( Application app )
	{
		//判断是否已注册过。如果没有，就将对象加到列表中，完成注册过程
		Integer appId = app.getId();
		if( null == appTable.get( appId ) )
		{			
			appTable.put( appId, app );
			return true;
		}
		return false;	
	}
	
	/**
	 * 注销应用程序控制对象，其实就是从列表中删除。一般不会调用该方法
	 * @param appId 应用程序控制对象的ID
	 * @return true表示注销成功；false表示失败。失败的原因只可能是对象不存在。
	 */
	public boolean unregisterApp( Integer appId )
	{
		if( null != appTable.get( appId ) )
		{
			appTable.remove( appId );
			return true;
		}
		return false;	
	}
	
	/**
	 * 注销所有应用程序控制对象，当调度程序退出时调用。该函数无返回，因为它总是执行成功。
	 */
	public void unregisterAll()
	{
		appTable.clear();
	}
	
	/**
	 * 设置应用程序本地的Messenger
	 * @param appId 应用程序ID
	 * @param msger 应用程序本地的Messenger
	 * @return true表示成功，false表示失败。失败的原因是找不到对应的应用程序控制对象
	 */
	public boolean setAppMessenger( Integer appId, Messenger msger)
	{
		Application app = appTable.get( appId );
		if( app != null )
		{
			app.setMessenger( msger );
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/**
	 * 设置应用程序状态
	 * @param appId 应用程序ID
	 * @param status 应用程序状态
	 * @return true表示成功，false表示失败。失败的原因是找不到对应的应用程序控制对象
	 */
	public boolean setAppStatus( Integer appId, int status )
	{
		Application app = appTable.get( appId );
		if( app != null )
		{
			app.appstatus.setStatus( status );
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public int getAppStatus( Integer appId )
	{
		Application app = appTable.get( appId );
		if( app != null )
		{
			return app.appstatus.getStatus();
		}
		else
		{
			return -1;
		}
	}
	
	public int getAppPriority( Integer appId )
	{
		Application app = appTable.get( appId );
		if( app != null )
		{
			return app.applevel.getPriority();
		}
		else
		{
			return -1;
		}
	}
	
	/**
	 * 比较应用程序状态
	 * @param appId 应用程序ID
	 * @param status 应用程序状态
	 * @return true表示状态相同，false表示失败。失败的原因是找不到对应的应用程序控制对象或比较失败
	 */
	public boolean cmpAppStatus( Integer appId, int status )
	{
		Application app = appTable.get( appId );
		if( app != null )
		{
			if (status == app.appstatus.getStatus()) 
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
	
	
	/**
	 * 比较应用程序优先级
	 * @param appId 应用程序ID
	 * @param status 应用程序优先级
	 * @return 1表示优先级高，0表示优先级低。失败的原因是找不到对应的应用程序控制对象或优先级低
	 */
	public int cmpAppPriority( Integer appId1, Integer appId2 )
	{
		Application app1 = appTable.get( appId1 );
	    Application app2 = appTable.get( appId2 );
	    
		if( app1 != null && app2 != null )
		{
			if (app1.applevel.getPriority() >= app2.applevel.getPriority()) 
			{
				return 1;
			}
			return 0;
		}
		else
		{
			return 0;
		}
	}
	
	/**
	 * @function 判断程序是否在运行
	 * @param appName
	 *            程序的名称
	 * @return
	 */
	@SuppressWarnings("static-access")
	private boolean isAppRunning(String appName) {
		Context context = DispatchApplication.getProcessContext().getApplicationContext();
		ActivityManager am = (ActivityManager) context.getSystemService(context.ACTIVITY_SERVICE);
		// 获取正在运行的应用
		List<RunningAppProcessInfo> process = am.getRunningAppProcesses();
		for (RunningAppProcessInfo run : process) 
		{
			if( run.processName.equals(appName) )
			{
				return true;
			}
		}
		return false;
	}

	
	/**
	 * 执行一个应用程序控制操作
	 * @param action 应用程序控制操作
	 * @return true表示执行成功，false表示执行失败。
	 */
	public boolean doAction( Action action )
	{
		boolean retVal = false;		//返回值
		Application app = null;		//应用程序实例
		
		//TODO:未来可以将下面的代码只放到若干个种类的action中处理，以便提高执行效率。因为有的Action
		//不需要等待其它应用程序执行操作。注意：若修改，需要同时修改最下方的remove代码
		//把action设置为正在执行的状态，并记录到Action的激活队列中
		action.setDoingState();
		activeActionSet.put( action.flowNo, action );
		Log.i(MainActivity.TAG, "doAction action.type=" + action.type);
		//根据动作类型，执行这个动作
		switch( action.type )
		{
			case Action.ACTION_CREATE://创建应用程序
				app = appTable.get( action.arg1 );
				retVal = app.create( action );
				break;
			case Action.ACTION_START://启动应用程序
				
				app = appTable.get( action.arg1 );
				Log.e(MainActivity.TAG," action_start "+ app.pkg);
				if (isAppRunning( app.pkg )  )
				{
					Log.e(MainActivity.TAG," action_start run");
				    retVal = app.start( action );
				}
				else//如果当前进程不存在则创建进程，然后发送消息
				{
					Log.e(MainActivity.TAG," action_start run112233");
					app.setMessenger( null ); 
					retVal = app.create( action );
					SystemClock.sleep(100);
					retVal = app.start( action );
				}
				break;
			case Action.ACTION_CLOSE://启动应用程序
				app = appTable.get( action.arg1 );
				retVal = app.close( action );
				break;
			case Action.ACTION_DATA://处理应用程序数据
				app = appTable.get( action.arg1 );
				if (isAppRunning(app.pkg) == false)
				{
					app.setMessenger( null ); 
					retVal = app.create( action );
				}
				retVal = app.dealdata(action);
				break;
			default:				
				break;
		}
		
		activeActionSet.remove( action.flowNo );//从激活列表中移除action，以便回收内存
		return retVal;
	}
	
	/**
	 * 处理通知消息
	 * @param notice 包含通知的各个信息
	 */
	public void doNotice( Notice notice )
	{
		switch( notice.type )
		{
			case Notice.TYPE_ACTION_RESULT://通知Action发生了状态变化
				Action action = activeActionSet.get( notice.flowNo );
				if( null != action )
				{
					action.setDoneState( notice.result );
					action.notityForDone();//通知另一个Action执行结束了。					
				}
				break;
		}
	}
	
	
	public int getCurPriority() 
	{
		int i = 0;
		Application app;
		
		for ( i = 0; i < appTable.size(); i++ )
		{
			app = appTable.get(appTable.valueAt(i).id);
			if (app.appstatus.getStatus() == AppStatus.STATUS_PROGRAM_RUN)
			{
				return app.applevel.getPriority();
			}
		}
		return -1;
	}
	
	public int getCurProgram() 
	{
		int i = 0;
		Application app;
		
		for ( i = 0; i < appTable.size(); i++ )
		{
			app = appTable.get(appTable.valueAt(i).id);
			if (app.appstatus.getStatus() == AppStatus.STATUS_PROGRAM_RUN)
			{
				return app.getId();
			}
		}
		return -1;
	}
	
	
	public boolean isAddMsg(CommandEvent event)
	{
		Application app;

		app = appTable.get(event.getProgramIndex());
		if ( app == null )
		{
			return true;
		}
		if ( app.applevel.getPriority() < DispatchApplication.msgList.getPriority() )
		{
			return false;
		}
		else if ( app.applevel.getPriority() > DispatchApplication.msgList.getPriority() )
		{
			DispatchApplication.msgList.emptyMsg();
			DispatchApplication.msgList.setPriority(app.applevel.getPriority());
		}

		return true;
	}
}


