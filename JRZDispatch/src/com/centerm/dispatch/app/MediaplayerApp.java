package com.centerm.dispatch.app;

/**
 * ��ý�岥�ų�����ƶ���
 */
public class MediaplayerApp extends Application 
{

	public static final int id = 10;		//��ý�岥�ŵ�id
	public static final String name = "com.centerm.mediaplayer.MainActivity";
	public static final String pkg = "com.centerm.mediaplayer";
	//TODO��Ȩ��
	public MediaplayerApp()
	{
		super( id, name, pkg );
		//���ó������ȼ�
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL1);
		
		//���ó����ʼ״̬ΪPROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_RUN);
	}
}
