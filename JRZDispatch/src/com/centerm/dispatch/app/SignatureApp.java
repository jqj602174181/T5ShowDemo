package com.centerm.dispatch.app;

/**
 * ������̳�����ƶ���
 */
public class SignatureApp extends Application 
{

	public static final int id = 2;		        //������̵�id
	public static final String name = "com.centerm.signature.MainActivity";
	//public static final String name = "com.centerm.signature.SignStartService";
	public static final String pkg = "com.centerm.signature";
	
	//TODO��Ȩ��
	public SignatureApp()
	{
		super( id, name, pkg );
		//startStyle= SERVICE;
		//���ó������ȼ�
		applevel = new AppPriority();
		applevel.setPriority(AppPriority.PRIORITY_LEVEL3);
		
		//���ó����ʼ״̬ΪPROGRAM_UNOPEN
		appstatus = new AppStatus();
		appstatus.setStatus(AppStatus.STATUS_PROGRAM_UNOPEN);
	}
}
