package com.centerm.dispatch.communication;

public class ResultEvent {
	private int nLinkType;
	private byte[] resultData;
	
	public ResultEvent() {
		
	}
	
	public ResultEvent(int linktype, byte[] result) {
		this.nLinkType = linktype;
		this.resultData = result;
	}
	
	public int getLinkType() {
		return nLinkType;
	}
	
	public byte[] getResultData() {
		return resultData;
	}
}
