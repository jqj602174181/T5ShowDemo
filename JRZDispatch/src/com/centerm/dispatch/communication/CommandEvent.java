package com.centerm.dispatch.communication;

public class CommandEvent 
{
	
	int comDeivce;			//事件来源于哪个通讯接口，就是C语言中的LinkType
	int programIndex;		//事件的目标程序索引
	int insType;            //事件类型
	byte[] cmdData;			//事件的命令数据
	
	public CommandEvent()
	{//do nothing		
	}
	
	public CommandEvent( int device, int program, int type, byte[] cmdData )
	{
		this.comDeivce = device;
		this.programIndex = program;
		this.insType = type;
		this.cmdData = cmdData;
	}

	public int getComDeivce() 
	{
		return comDeivce;
	}

	public void setComDeivce(int comDeivce) 
	{
		this.comDeivce = comDeivce;
	}

	public int getInsType() 
	{
		return insType;
	}

	public void setInsType(int insType) 
	{
		this.insType = insType;
	}
	
	public int getProgramIndex() 
	{
		return programIndex;
	}

	public void setProgramIndex(int programIndex) 
	{
		this.programIndex = programIndex;
	}

	public byte[] getCmdData() 
	{
		return cmdData;
	}

	public void setCmdData(byte[] cmdData) 
	{
		this.cmdData = cmdData;
	}
}
