package com.centerm.dispatch.app;

/**
 * ֪ͨ���󣬵�Ӧ�ó���ִ�����һ�������󣬻ᷴ��һ��ִ�еĽ��
 */
public class Notice 
{
	
	public static final int TYPE_ACTION_RESULT = 1;		//֪ͨ������ִ�н����flowNo��result����Ч��
	
	public int type;				//֪ͨ����
	public int flowNo;				//��ˮ��
	public int result;				//Action��ִ�н��
	
	public Notice( int type, int flowNo, int result )
	{
		this.type = type;
		this.flowNo = flowNo;
		this.result = result;
	}

}
