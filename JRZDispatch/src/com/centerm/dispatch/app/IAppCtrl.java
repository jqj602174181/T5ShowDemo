package com.centerm.dispatch.app;

/**
 * Ӧ�ó������ͳһ�ӿ�
 *
 */
public interface IAppCtrl 
{

	/**
	 * ����Ӧ�ó�����̣����������ں�̨
	 * @param action ��Ӧ�Ķ�����Ϊ
	 * @return true��ʾ�����ɹ���false��ʾ����ʧ��
	 */
	boolean create( Action action );
	
	/**
	 * ��ʾӦ�ó���
	 * @param action ��Ӧ�Ķ�����Ϊ
	 * @return true��ʾ�����ɹ���false��ʾ����ʧ��
	 */
	boolean start( Action action );
	
	
	/**
	 * �ر�Ӧ�ó������
	 * @param action ��Ӧ�Ķ�����Ϊ
	 * @return true��ʾ�����ɹ���false��ʾ����ʧ��
	 */
	boolean close( Action action );
	
	/**
	 * ����Ӧ�ó�������
	 * @param action ��Ӧ�Ķ�����Ϊ
	 * @return true��ʾ�����ɹ���false��ʾ����ʧ��
	 */
	boolean dealdata( Action action );
	
	//TODO:δ����Ҫ���������ӿ�
}
