package com.centerm.dispatch.app;

/**
 * 密码键盘程序控制对象
 */
public class KeypadApp extends Application 
{

	public static final int id = 1;		        //密码键盘的id
	public static final String name = "com.centerm.keypad.MainActivity";
	public static final String pkg = "com.centerm.keypad";
	//TODO：权限
	public KeypadApp()
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
