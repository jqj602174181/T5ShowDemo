package com.centerm.dispatch.app;

/**
 * 密码键盘程序控制对象
 */
public class SignatureApp extends Application 
{

	public static final int id = 2;		        //密码键盘的id
	public static final String name = "com.centerm.signature.MainActivity";
	//public static final String name = "com.centerm.signature.SignStartService";
	public static final String pkg = "com.centerm.signature";
	
	//TODO：权限
	public SignatureApp()
	{
		super( id, name, pkg );
		//startStyle= SERVICE;
		//设置程序优先级
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL3);
		
		//设置程序初始状态为PROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_UNOPEN);
	}
}
