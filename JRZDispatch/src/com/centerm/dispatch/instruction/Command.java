package com.centerm.dispatch.instruction;

/**
 * 定义统一的命令处理接口
 */
public interface Command 
{
	/**
	 * 执行明确的命令
	 * @param data 命令参数
	 * @param nLinkType 主机连接链路类型
	 * @return 处理的结果数据，上层可根据返回的数据，进行特定的再处理
	 */
	public byte[] executeCommand(byte[] data, Integer nLinkType);
	
}
