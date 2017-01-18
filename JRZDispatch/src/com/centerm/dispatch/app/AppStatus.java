package com.centerm.dispatch.app;

/**
 * 应用程序运行状态
 */
public class AppStatus 
{
	public static final int STATUS_PROGRAM_UNOPEN = -1;
	public static final int STATUS_PROGRAM_CLOSE = 0;
	public static final int STATUS_PROGRAM_RUN = 1;
	public static final int STATUS_PROGRAM_STOP = 2;
	
	protected int status;     //应用程序优先级
	
	public synchronized int getStatus() 
	{
		return status;
	}

	public synchronized void setStatus(int status) 
	{
		this.status = status;
	}
}
