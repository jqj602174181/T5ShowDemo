package com.centerm.remoteserver.aidl;
interface RemoteInfo{	
	boolean getUpdateSuccess(int style);
	int onCheckUpdata(int style);
	void reboot();
	void setSystemTime(String time);
	String getUpdateVersion(int style);
	String getFinanicalVersion(int style);
	void startUpdateAct(int style);
	void killApp(String packageName);
	void startGetDeviceInfo(boolean isStart);
}