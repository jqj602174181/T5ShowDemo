package com.centerm.dispatch.app;

/**
 * Ӧ�ó�������״̬
 */
public class AppStatus 
{
	public static final int STATUS_PROGRAM_UNOPEN = -1;
	public static final int STATUS_PROGRAM_CLOSE = 0;
	public static final int STATUS_PROGRAM_RUN = 1;
	public static final int STATUS_PROGRAM_STOP = 2;
	
	protected int status;     //Ӧ�ó������ȼ�
	
	public synchronized int getStatus() 
	{
		return status;
	}

	public synchronized void setStatus(int status) 
	{
		this.status = status;
	}
}
