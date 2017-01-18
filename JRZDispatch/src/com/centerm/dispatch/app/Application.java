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
 * Ӧ�ó��������
 *
 */
public class Application implements IAppCtrl 
{

	public static final int ACTIVITY = 1;
	public static final int SERVICE = 2;
	protected int id;			//Ӧ�ó���id
	protected String name;		//Ӧ�ó�����ȫ�޶���
	protected String pkg;		//Ӧ�ó������
	protected Messenger msger;	//Ӧ�ó����messenger��ͨ�������Ա���ͨ��
	protected AppPriority applevel; //Ӧ�ó������ȼ�
	protected AppStatus appstatus; //Ӧ�ó������ȼ�
	public int startStyle= ACTIVITY;//������ʽ,1Activity,2service
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
	 * �Ƿ�����˴���������һֱ�����ں�̨
	 * @return true��ʾ����״̬��false��ʾδ����״̬
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
	 * ������Ϣ��Ӧ�ó���
	 * @param msg �����͵���Ϣ
	 * @return true��ʾ�ɹ���false��ʾʧ��
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
			//log�ʹ���Ӧ�ó����쳣�Ͽ�������
			return false;
		}
	}
	
	/**
	 * �ȴ�Ӧ�ó���ִ��������Action
	 * @param action ��ִ�еĶ���
	 * @return true��ʾִ�гɹ���false��ʾִ��ʧ��
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
		//���msger���ڣ��Ͳ�Ӧ���ٴδ���
		if( msger != null)
		{
			return true;
		}
		
		//��������������ָ���������޷������ɹ�
		if( pkg == null || name == null)
		{
			return false;
		}
        
		try 
		{
			//������Ӧ��Ӧ�ó���		
			ComponentName appComponent = new ComponentName( pkg, name );
			
			
			if(startStyle==ACTIVITY){
				Intent intent = new Intent( Intent.ACTION_MAIN );
				intent.addCategory( Intent.CATEGORY_LAUNCHER );
				intent.setComponent( appComponent );
				intent.putExtra( Action.FLOWNO, action.flowNo );//������ˮ�ţ�Ӧ�ó����ڻ����ɹ�����ʱ��Ӧ�÷�����ˮ��
				intent.addFlags( Intent.FLAG_ACTIVITY_NEW_TASK );
				DispatchApplication.getProcessContext().startActivity( intent );
			}else if(startStyle==SERVICE)
			{
				Intent intent = new Intent();
				intent.setComponent( appComponent );
				intent.putExtra( Action.FLOWNO, action.flowNo );//������ˮ�ţ�Ӧ�ó����ڻ����ɹ�����ʱ��Ӧ�÷�����ˮ��
				DispatchApplication.getProcessContext().startService( intent );
			}
				
			
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		
		
		return waitAction( action );//�ȴ�Ӧ�ó������������������ؽ��
	}

	@Override
	public boolean start(Action action) 
	{
		
		//����������ں�̨���У����ܷ�����Ϣ
		if( false == isCreated() )
		{
			Log.i(MainActivity.TAG, "start---The program has already exited!");
			//Ӧ���ȴ��������ٷ�����Ϣ
			if ( create(action) == false )
			{
				Log.e(MainActivity.TAG, "start---Start the program interface operation failed!");
				return false;
			}
		}

		Log.i(MainActivity.TAG, "start---Start sending start message!");
		//���´���ֻ�������ã�δ��һ��Ҫ��
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_START;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//������������������Ϣ
		
		return waitAction( action );//�ȴ�Ӧ�ó����������������ؽ��
	}

	@Override
	public boolean close(Action action) 
	{
		//����������ں�̨���У����ܷ�����Ϣ
		if( false == isCreated() )
		{
			Log.i(MainActivity.TAG, "close---The program has already exited!");
			//Ӧ���ȴ��������ٷ�����Ϣ
			if ( create(action) == false )
			{
				Log.e(MainActivity.TAG, "close---Start the program interface operation failed!");
				return false;
			}
		}
		
		Log.i(MainActivity.TAG, "close---Start sending start message!");
		//���´���ֻ�������ã�δ��һ��Ҫ��
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_CLOSE;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//���͹رճ���������Ϣ
		
		return waitAction( action );//�ȴ�Ӧ�ó����������������ؽ��
	}

	@Override
	public boolean dealdata(Action action) 
	{
		//����������ں�̨���У����ܷ�����Ϣ
		if( false == isCreated() )
		{
			//Ӧ���ȴ��������ٷ�����Ϣ
			return false;
		}
		
		//���´���ֻ�������ã�δ��һ��Ҫ��
		Message msg = Message.obtain();
		msg.what = MessageType.MSG_DATA;
		msg.arg1 = action.flowNo;
		msg.setData((Bundle)action.obj);
		sendMessage( msg );//����Ӧ�ó������ݴ������Ϣ
		
		return waitAction( action );//�ȴ�Ӧ�ó����������������ؽ��
	}

}
