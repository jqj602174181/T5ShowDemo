package com.centerm.dispatch.ipc;

/*
 * ������������Ӧ�ó�����ͨ����Ϣ����
 */
public class MessageType 
{ 
	/*
	 * 1 - 100 Ϊ������Ӧ�ó��������Ϣ
	 * 101 - 102ΪӦ�ó��������ȷ������Ϣ
	 */
	public final static int MSG_START = 3;				  //����Ӧ�ó���Ľ��棬arg1����ˮ�ţ�obj��ָ������
	public final static int MSG_CLOSE = 4;				  //�ر�Ӧ�ó���Ľ��棬arg1����ˮ�ţ�obj��ָ������
	public final static int MSG_EXIT = 5;                 //����Ӧ�ó���arg1����ˮ�ţ�obj��ָ������
	public final static int MSG_DATA = 6;                 //����Ӧ�ó�������ָ�arg1����ˮ�ţ� obj��ָ������
	public final static int MSG_CONTACTME = 101;		  //��ʾ"�Ժ󱣳���ϵ"����ʱ��Ӧ�ó����������ߵ��ȣ�Ӧ�ó����ͨ��Messenger, arg1��ʾӦ�ó���id��arg2��ʾAction����ˮ��
	public final static int MSG_FORGETME = 102;			  //Ӧ�ó��������Ͽ����ȵ�ͨ�ŷ���,ͨ��ֻ����Ӧ�ó����˳�ʱ���͸���Ϣ, arg1��ʾӦ�ó���id
	public final static int MSG_SEND_DATA = 103;		  //�������ݸ�������arg1��ʾӦ�ó���id��obj��ʾ�ϴ�����������
	public final static int MSG_NOTICE_OPERATOR_END = 111;//֪ͨ�������ִ�к����״̬,arg1��ʾӦ�ó���id��arg2Ϊִ��Ӧ�ó���״̬��obj��ʾ���͸�����������	
	public final static int MSG_NOTICE_STATE_DATA = 112;  //֪ͨ���ȣ�Ӧ�ó���ָ��������óɹ���arg1��ʾӦ�ó���id��arg2��ʾAction����ˮ�ţ�obj��ʾ�ϴ�����������
	public final static int MSG_NOTICE_STATE_CLOSE = 113; //֪ͨ���ȣ�Ӧ�ó����ں�̨����״̬��arg1��ʾӦ�ó���id��arg2��ʾAction����ˮ��
	public final static int MSG_NOTICE_STATE_RUN = 114;   //֪ͨ���ȣ�Ӧ�ó���������״̬��arg1��ʾӦ�ó���id��arg2��ʾAction����ˮ��
	public final static int MSG_SIGN_BYTE_DATA = 115;     //��дǩ��byte msg
	
	public final static int MSG_NOTICE_SEND_DATA = 116;   //�������ݸ����� ����չ ����֤
	
}