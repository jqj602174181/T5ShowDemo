package com.centerm.dispatch.instruction;

/**
 * ����ͳһ�������ӿ�
 */
public interface Command 
{
	/**
	 * ִ����ȷ������
	 * @param data �������
	 * @param nLinkType ����������·����
	 * @return ����Ľ�����ݣ��ϲ�ɸ��ݷ��ص����ݣ������ض����ٴ���
	 */
	public byte[] executeCommand(byte[] data, Integer nLinkType);
	
}
