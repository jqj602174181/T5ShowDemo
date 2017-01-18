package com.centerm.dispatch.app;

/**
 * 多媒体播放程序控制对象
 */
public class MediaplayerApp extends Application 
{

	public static final int id = 10;		//多媒体播放的id
	public static final String name = "com.centerm.mediaplayer.MainActivity";
	public static final String pkg = "com.centerm.mediaplayer";
	//TODO：权限
	public MediaplayerApp()
	{
		super( id, name, pkg );
		//设置程序优先级
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL1);
		
		//设置程序初始状态为PROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_RUN);
	}
}
