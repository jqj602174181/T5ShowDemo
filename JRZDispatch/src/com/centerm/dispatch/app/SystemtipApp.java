package com.centerm.dispatch.app;

/**
 * 系统提示程序控制对象
 */
public class SystemtipApp extends Application
{
	public static final int id = 8;		        //系统提示的id
	public static final String name = "com.centerm.systemtip.MainActivity";
	public static final String pkg = "com.centerm.systemtip";
	//TODO：权限
	public SystemtipApp()
	{
		super( id, name, pkg );

		//设置程序优先级
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL3);
		
		//设置程序初始状态为PROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_UNOPEN);
	}

}
