package com.centerm.dispatch.app;

import android.util.Log;
import com.centerm.dispatch.MainActivity;

/**
 * ����Ҫ�����ִ�еĲ���
 *
 */
public class Action 
{
	//���嶯�������
	public static final int ACTION_NONE = 0;		//��Ч�Ķ���
	public static final int ACTION_CREATE = 1;		//����Ӧ�ó���Ķ���, arg1ΪӦ�ó���id,arg2����
	public static final int ACTION_START = 2;		//����Ӧ�ó���Ķ���, arg1ΪӦ�ó���id,arg2����
	public static final int ACTION_DATA = 3;        //����Ӧ�ó��������, arg1ΪӦ�ó���id,arg2����
	public static final int ACTION_CLOSE = 4;		//����Ӧ�ó���Ķ���, arg1ΪӦ�ó���id,arg2����
	public static final int ACTION_EXIT = 5;        //�˳�Ӧ�ó���, arg1ΪӦ�ó���id,arg2����
	
	//���嶯����״̬
	public static final int STATE_UNDO = 0;			//δ�������״̬
	public static final int STATE_DOING = 1;		//���ڱ������״̬
	public static final int STATE_DONE = 2;			//ִ�н�����״̬
	
	//ִ�н��
	public static final int RESULT_OK = 0;			//ִ�гɹ�
	public static final int RESULT_ERR = 1;			//ִ��ʧ��
	public static final int RESULT_TIMEOUT = 2;		//ִ�г�ʱ
	
	//����
	public static final int DEFAULT_TIMEOUT = 2000;	//Ĭ��ÿ������ִ�еĳ�ʱʱ��
	public static final String FLOWNO = "FlowNO";	//��ˮ�ŵļ�ֵ������
	
	//�����Ĳ����б�	
	public int flowNo;							//������ˮ��
	public int type = ACTION_NONE;				//��������
	public int arg1;							//��������1
	public int arg2;							//��������2
	public Object obj;							//��������3
	
	//�����ĵ�ǰִ��״̬
	private int state = STATE_UNDO;				//������״̬
	private int result = RESULT_OK;				//������ִ�н��
	
	//���캯��
	public Action( int type, int flowNo, int arg1, int arg2, Object obj )
	{
		this.type = type;
		this.flowNo = flowNo;
		this.arg1 = arg1;
		this.arg2 = arg2;
		this.obj = obj;
	}
	
	public Action( int type, int flowNo )
	{
		this.type = type;
		this.flowNo = flowNo;
	}
	
	public int getState() 
	{
		return state;
	}

	/**
	 * ����Action״̬Ϊ����ִ��
	 */
	public void setDoingState() 
	{
		this.state = STATE_DOING;
	}
	
	/**
	 * ����Action��ִ�н��״̬
	 * @param result ִ�н��
	 */
	public void setDoneState( int result)
	{
		this.state = STATE_DONE;
		this.result = result;
	}

	/**
	 * �ȴ�����ִ�н���
	 * @return RESULT_TIMEOUT��ʾ��ʱ��RESULT_OK��ʾִ�гɹ���RESULT_ERR��ʾִ��ʧ�ܡ�
	 */
	public synchronized int waitForDone()
	{
		Log.i( MainActivity.TAG, "test.........this.flowNo.................." + this.flowNo);
		//���������ִ�У��͵ȴ��¼�����
		if( STATE_DOING == state )
		{
			try
			{
				Log.i( MainActivity.TAG, "Action: wait flow " + this.flowNo + " to be done????????????" );
				wait( DEFAULT_TIMEOUT );
				Log.i( MainActivity.TAG, "Action: wait flow " + this.flowNo + " to be done............" );
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
		}
		
		//�ȴ�������ж��¼��Ƿ�ִ����ɣ������ɾͷ���
		if( STATE_DONE == state )
		{
			return result;
		}
		Log.e( MainActivity.TAG, "Action: action " + flowNo + " is time out" );
		return RESULT_TIMEOUT;
	}
	
	/**
	 * ֪ͨ��һ���̣߳�actionִ�н���
	 */
	public synchronized void notityForDone()
	{
		Log.v( MainActivity.TAG, "Action: flow " + this.flowNo + " is be done!!!!!!" );
		notify();
	}
}
