package com.centerm.dispatch.communication;

public class CommandEvent 
{
	
	int comDeivce;			//�¼���Դ���ĸ�ͨѶ�ӿڣ�����C�����е�LinkType
	int programIndex;		//�¼���Ŀ���������
	int insType;            //�¼�����
	byte[] cmdData;			//�¼�����������
	
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
