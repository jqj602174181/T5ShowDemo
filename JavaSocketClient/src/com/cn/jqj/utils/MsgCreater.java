package com.cn.jqj.utils;

/**
 * 服务端的报文协议生成器
 */
public class MsgCreater {
	
	public static byte[] createMsg(int work,int type,String jsonStr, byte[] portraiFileData, byte[] fileData) throws Exception{
		byte[] tempContent 		= jsonStr.getBytes("utf-8");
		byte[] typeBytes 	= new byte[]{(byte) work,(byte) type};
		byte[] lenght 		= StringUtil.IntToByte(tempContent.length);
		
		byte[] content 		= StringUtil.bytes2And(typeBytes, lenght); //信息类型2字节+信息长度
		content 			= StringUtil.bytes2And(content, tempContent); 
//		content 			= StringUtil.bytes2And(content, portraiFileData);
		content 			= StringUtil.bytes2And(content, fileData);
		byte checker		= RedundancyCheck(content);
		content 			= StringUtil.bytes2And(content, new byte[]{checker});
		byte[] len 			= StringUtil.IntToByte(content.length);
		return StringUtil.bytes2And(len, content);
	}
	
	public static byte[] createMsg(int work,int type,String jsonStr, byte[] fileData) throws Exception{
		byte[] tempContent 		= jsonStr.getBytes("utf-8");
		byte[] typeBytes 	= new byte[]{(byte) work,(byte) type};
		byte[] lenght 		= StringUtil.IntToByte(tempContent.length);
		
		byte[] content 		= StringUtil.bytes2And(typeBytes, lenght); //信息类型2字节+信息长度
		content 			= StringUtil.bytes2And(content, tempContent); 
		content 			= StringUtil.bytes2And(content, fileData);
		byte checker		= RedundancyCheck(content);
		content 			= StringUtil.bytes2And(content, new byte[]{checker});
		byte[] len 			= StringUtil.IntToByte(content.length);
		return StringUtil.bytes2And(len, content);
	}
	
	/**
	 * 计算冗余校验值
	 * @param data	校验数据
	 * @return	校验值
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
