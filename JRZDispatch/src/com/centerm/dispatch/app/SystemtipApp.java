package com.centerm.dispatch.app;

/**
 * ϵͳ��ʾ������ƶ���
 */
public class SystemtipApp extends Application
{
	public static final int id = 8;		        //ϵͳ��ʾ��id
	public static final String name = "com.centerm.systemtip.MainActivity";
	public static final String pkg = "com.centerm.systemtip";
	//TODO��Ȩ��
	public SystemtipApp()
	{
		super( id, name, pkg );

		//���ó������ȼ�
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL3);
		
		//���ó����ʼ״̬ΪPROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_UNOPEN);
	}

}
