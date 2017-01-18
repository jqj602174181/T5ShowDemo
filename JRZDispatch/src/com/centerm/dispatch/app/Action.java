package com.centerm.dispatch.app;

import android.util.Log;
import com.centerm.dispatch.MainActivity;

/**
 * 主机要求调度执行的操作
 *
 */
public class Action 
{
	//定义动作的类别
	public static final int ACTION_NONE = 0;		//无效的动作
	public static final int ACTION_CREATE = 1;		//创建应用程序的动作, arg1为应用程序id,arg2保留
	public static final int ACTION_START = 2;		//启动应用程序的动作, arg1为应用程序id,arg2保留
	public static final int ACTION_DATA = 3;        //处理应用程序的数据, arg1为应用程序id,arg2保留
	public static final int ACTION_CLOSE = 4;		//结束应用程序的动作, arg1为应用程序id,arg2保留
	public static final int ACTION_EXIT = 5;        //退出应用程序, arg1为应用程序id,arg2保留
	
	//定义动作的状态
	public static final int STATE_UNDO = 0;			//未被处理的状态
	public static final int STATE_DOING = 1;		//正在被处理的状态
	public static final int STATE_DONE = 2;			//执行结束的状态
	
	//执行结果
	public static final int RESULT_OK = 0;			//执行成功
	public static final int RESULT_ERR = 1;			//执行失败
	public static final int RESULT_TIMEOUT = 2;		//执行超时
	
	//其它
	public static final int DEFAULT_TIMEOUT = 2000;	//默认每个动作执行的超时时间
	public static final String FLOWNO = "FlowNO";	//流水号的键值对名称
	
	//动作的参数列表	
	public int flowNo;							//动作流水号
	public int type = ACTION_NONE;				//动作类型
	public int arg1;							//动作参数1
	public int arg2;							//动作参数2
	public Object obj;							//动作参数3
	
	//动作的当前执行状态
	private int state = STATE_UNDO;				//动作的状态
	private int result = RESULT_OK;				//动作的执行结果
	
	//构造函数
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
	 * 设置Action状态为正在执行
	 */
	public void setDoingState() 
	{
		this.state = STATE_DOING;
	}
	
	/**
	 * 设置Action的执行结果状态
	 * @param result 执行结果
	 */
	public void setDoneState( int result)
	{
		this.state = STATE_DONE;
		this.result = result;
	}

	/**
	 * 等待动作执行结束
	 * @return RESULT_TIMEOUT表示超时，RESULT_OK表示执行成功，RESULT_ERR表示执行失败。
	 */
	public synchronized int waitForDone()
	{
		Log.i( MainActivity.TAG, "test.........this.flowNo.................." + this.flowNo);
		//如果还正在执行，就等待事件结束
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
		
		//等待完后，再判断事件是否执行完成，如果完成就返回
		if( STATE_DONE == state )
		{
			return result;
		}
		Log.e( MainActivity.TAG, "Action: action " + flowNo + " is time out" );
		return RESULT_TIMEOUT;
	}
	
	/**
	 * 通知另一个线程，action执行结束
	 */
	public synchronized void notityForDone()
	{
		Log.v( MainActivity.TAG, "Action: flow " + this.flowNo + " is be done!!!!!!" );
		notify();
	}
}
