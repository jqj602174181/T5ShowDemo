package com.centerm.util.financial;

public class APDUData {

	public APDUData() {
		// TODO Auto-generated constructor stub
	}
	
	public byte[] Command = new byte[4];
	public byte Lc;
	public byte[] DataIn = new byte[256];
	public byte  Le;
	public int LengthOut;
	public byte[] DataOut = new byte[1024];
	public byte SW1;
	public byte SW2;
}
