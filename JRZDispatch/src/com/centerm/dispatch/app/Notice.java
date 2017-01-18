package com.centerm.dispatch.app;

/**
 * 通知对象，当应用程序执行完成一个操作后，会反馈一个执行的结果
 */
public class Notice 
{
	
	public static final int TYPE_ACTION_RESULT = 1;		//通知动作的执行结果，flowNo和result是有效的
	
	public int type;				//通知类型
	public int flowNo;				//流水号
	public int result;				//Action的执行结果
	
	public Notice( int type, int flowNo, int result )
	{
		this.type = type;
		this.flowNo = flowNo;
		this.result = result;
	}

}
