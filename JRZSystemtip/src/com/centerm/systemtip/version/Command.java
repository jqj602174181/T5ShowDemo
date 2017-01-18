package com.centerm.systemtip.version;

public interface Command 
{
	public abstract void executeCommand(byte[] bData, Integer nLinkType, Integer nFlowNo);
}
