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

//������δ���úúÿ���һ�¸ö�����ڲ���������ͬ������
/**
 * ����������Ӧ�ó�����ƶ���
 */
public class AppManager 
{	
	 
	private static AppManager manager = new AppManager();	//��������һʵ��
	private SparseArray<Application> appTable;				//Ӧ�ó����б�
	private SparseArray<Action> activeActionSet;			//��¼���ڼ���״̬����ˮ������Ӧ��Action
	
	/**
	 * ����private����֤�޷����ⲿʵ�������Ӷ�ʵ�ֵ�һʵ��
	 */
	private AppManager()
	{
		appTable = new SparseArray<Application>();
		activeActionSet = new SparseArray<Action>();
	}
	
	/**
	 * ���Ӧ�ó������ĵ�һʵ������
	 * @return Ӧ�ó������������
	 */
	static public AppManager getManager()
	{
		return manager;
	}
	
	/**
	 * ��Ӧ�ó�����ƶ���ע�ᵽ�����б���
	 * @param app Ӧ�ó�����ƶ���
	 * @return true��ʾע��ɹ���false��ʾע��ʧ�ܡ�ʧ�ܵ�ԭ���ǽ������ظ�ע�ᡣ
	 */
	public boolean registerApp( Application app )
	{
		//�ж��Ƿ���ע��������û�У��ͽ�����ӵ��б��У����ע�����
		Integer appId = app.getId();
		if( null == appTable.get( appId ) )
		{			
			appTable.put( appId, app );
			return true;
		}
		return false;	
	}
	
	/**
	 * ע��Ӧ�ó�����ƶ�����ʵ���Ǵ��б���ɾ����һ�㲻����ø÷���
	 * @param appId Ӧ�ó�����ƶ����ID
	 * @return true��ʾע���ɹ���false��ʾʧ�ܡ�ʧ�ܵ�ԭ��ֻ�����Ƕ��󲻴��ڡ�
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
	 * ע������Ӧ�ó�����ƶ��󣬵����ȳ����˳�ʱ���á��ú����޷��أ���Ϊ������ִ�гɹ���
	 */
	public void unregisterAll()
	{
		appTable.clear();
	}
	
	/**
	 * ����Ӧ�ó��򱾵ص�Messenger
	 * @param appId Ӧ�ó���ID
	 * @param msger Ӧ�ó��򱾵ص�Messenger
	 * @return true��ʾ�ɹ���false��ʾʧ�ܡ�ʧ�ܵ�ԭ�����Ҳ�����Ӧ��Ӧ�ó�����ƶ���
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
	 * ����Ӧ�ó���״̬
	 * @param appId Ӧ�ó���ID
	 * @param status Ӧ�ó���״̬
	 * @return true��ʾ�ɹ���false��ʾʧ�ܡ�ʧ�ܵ�ԭ�����Ҳ�����Ӧ��Ӧ�ó�����ƶ���
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
	 * �Ƚ�Ӧ�ó���״̬
	 * @param appId Ӧ�ó���ID
	 * @param status Ӧ�ó���״̬
	 * @return true��ʾ״̬��ͬ��false��ʾʧ�ܡ�ʧ�ܵ�ԭ�����Ҳ�����Ӧ��Ӧ�ó�����ƶ����Ƚ�ʧ��
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
	 * �Ƚ�Ӧ�ó������ȼ�
	 * @param appId Ӧ�ó���ID
	 * @param status Ӧ�ó������ȼ�
	 * @return 1��ʾ���ȼ��ߣ�0��ʾ���ȼ��͡�ʧ�ܵ�ԭ�����Ҳ�����Ӧ��Ӧ�ó�����ƶ�������ȼ���
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
	 * @function �жϳ����Ƿ�������
	 * @param appName
	 *            ���������
	 * @return
	 */
	@SuppressWarnings("static-access")
	private boolean isAppRunning(String appName) {
		Context context = DispatchApplication.getProcessContext().getApplicationContext();
		ActivityManager am = (ActivityManager) context.getSystemService(context.ACTIVITY_SERVICE);
		// ��ȡ�������е�Ӧ��
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
	 * ִ��һ��Ӧ�ó�����Ʋ���
	 * @param action Ӧ�ó�����Ʋ���
	 * @return true��ʾִ�гɹ���false��ʾִ��ʧ�ܡ�
	 */
	public boolean doAction( Action action )
	{
		boolean retVal = false;		//����ֵ
		Application app = null;		//Ӧ�ó���ʵ��
		
		//TODO:δ�����Խ�����Ĵ���ֻ�ŵ����ɸ������action�д����Ա����ִ��Ч�ʡ���Ϊ�е�Action
		//����Ҫ�ȴ�����Ӧ�ó���ִ�в�����ע�⣺���޸ģ���Ҫͬʱ�޸����·���remove����
		//��action����Ϊ����ִ�е�״̬������¼��Action�ļ��������
		action.setDoingState();
		activeActionSet.put( action.flowNo, action );
		Log.i(MainActivity.TAG, "doAction action.type=" + action.type);
		//���ݶ������ͣ�ִ���������
		switch( action.type )
		{
			case Action.ACTION_CREATE://����Ӧ�ó���
				app = appTable.get( action.arg1 );
				retVal = app.create( action );
				break;
			case Action.ACTION_START://����Ӧ�ó���
				
				app = appTable.get( action.arg1 );
				Log.e(MainActivity.TAG," action_start "+ app.pkg);
				if (isAppRunning( app.pkg )  )
				{
					Log.e(MainActivity.TAG," action_start run");
				    retVal = app.start( action );
				}
				else//�����ǰ���̲������򴴽����̣�Ȼ������Ϣ
				{
					Log.e(MainActivity.TAG," action_start run112233");
					app.setMessenger( null ); 
					retVal = app.create( action );
					SystemClock.sleep(100);
					retVal = app.start( action );
				}
				break;
			case Action.ACTION_CLOSE://����Ӧ�ó���
				app = appTable.get( action.arg1 );
				retVal = app.close( action );
				break;
			case Action.ACTION_DATA://����Ӧ�ó�������
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
		
		activeActionSet.remove( action.flowNo );//�Ӽ����б����Ƴ�action���Ա�����ڴ�
		return retVal;
	}
	
	/**
	 * ����֪ͨ��Ϣ
	 * @param notice ����֪ͨ�ĸ�����Ϣ
	 */
	public void doNotice( Notice notice )
	{
		switch( notice.type )
		{
			case Notice.TYPE_ACTION_RESULT://֪ͨAction������״̬�仯
				Action action = activeActionSet.get( notice.flowNo );
				if( null != action )
				{
					action.setDoneState( notice.result );
					action.notityForDone();//֪ͨ��һ��Actionִ�н����ˡ�					
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


