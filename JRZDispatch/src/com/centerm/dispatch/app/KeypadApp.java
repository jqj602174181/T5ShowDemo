package com.centerm.dispatch.app;

/**
 * ������̳�����ƶ���
 */
public class KeypadApp extends Application 
{

	public static final int id = 1;		        //������̵�id
	public static final String name = "com.centerm.keypad.MainActivity";
	public static final String pkg = "com.centerm.keypad";
	//TODO��Ȩ��
	public KeypadApp()
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
