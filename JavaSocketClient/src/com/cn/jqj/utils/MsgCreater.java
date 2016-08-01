package com.cn.jqj.utils;

/**
 * ����˵ı���Э��������
 */
public class MsgCreater {
	
	public static byte[] createMsg(int work,int type,String jsonStr, byte[] portraiFileData, byte[] fileData) throws Exception{
		byte[] tempContent 		= jsonStr.getBytes("utf-8");
		byte[] typeBytes 	= new byte[]{(byte) work,(byte) type};
		byte[] lenght 		= StringUtil.IntToByte(tempContent.length);
		
		byte[] content 		= StringUtil.bytes2And(typeBytes, lenght); //��Ϣ����2�ֽ�+��Ϣ����
		content 			= StringUtil.bytes2And(content, tempContent); 
		content 			= StringUtil.bytes2And(content, portraiFileData);
		content 			= StringUtil.bytes2And(content, fileData);
		byte checker		= RedundancyCheck(content);
		content 			= StringUtil.bytes2And(content, new byte[]{checker});
		byte[] len 			= StringUtil.IntToByte(content.length);
		return StringUtil.bytes2And(len, content);
	}
	
	/**
	 * ��������У��ֵ
	 * @param data	У������
	 * @return	У��ֵ
	 */
	public static byte RedundancyCheck(byte[] data)
	{
		byte checkCode = (byte)0x00;
		for ( int i = 0; i < data.length; i++ )
		{
			checkCode = (byte)(checkCode ^ data[i]);
		}
		
		return checkCode;
	}
	
	
	
}
