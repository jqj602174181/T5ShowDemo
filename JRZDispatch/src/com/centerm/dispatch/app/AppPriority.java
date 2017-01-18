package com.centerm.dispatch.app;

/**
 * 应用程序优先级
 */
public class AppPriority 
{
	public static final int PRIORITY_LEVEL1 = 1;
	public static final int PRIORITY_LEVEL2 = 2;
	public static final int PRIORITY_LEVEL3 = 3;
	public static final int PRIORITY_LEVEL4 = 4;
	public static final int PRIORITY_LEVEL5 = 5;
	
	protected int priority;     //应用程序优先级
	
	public int getPriority() 
	{
		return priority;
	}

	public void setPriority(int priority) 
	{
		this.priority = priority;
	}
}
