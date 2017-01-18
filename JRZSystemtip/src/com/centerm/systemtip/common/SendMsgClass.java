package com.centerm.systemtip.common;

import android.os.Bundle;
import android.os.Message;
import com.centerm.systemtip.ipc.MessageChannel;

//���ڷ�������
public final class SendMsgClass
{
	public static void SendResultToDispatch(int what,  int arg1, int arg2, int linktype, byte[] data) 
	{
		Message msg = Message.obtain();
		msg.what = what;
		msg.arg1 = arg1;
		msg.arg2 = arg2;
		Bundle bundle = new Bundle();// �������bundle����Ϊobj���Բ��ܿ����
		bundle.putInt("linktype", linktype);
		bundle.putByteArray("data", data);
		msg.setData(bundle);
		MessageChannel.getInstance().sendMessage(msg);
	}
	
	public static void SendStatusToDispatch(int what, int arg1,  int arg2) 
	{
		Message msg = Message.obtain();
		msg.what = what;
		msg.arg1 = arg1;
		msg.arg2 = arg2;
		MessageChannel.getInstance().sendMessage(msg);
	}
	
	public static void SendDataToDispatch(int what, int arg1, int linktype, byte[] data) 
	{
		Message msg = Message.obtain();
		msg.what = what;
		msg.arg1 = arg1;
		Bundle bundle = new Bundle();// �������bundle����Ϊobj���Բ��ܿ����
		bundle.putInt("linktype", linktype);
		bundle.putByteArray("data", data);
		msg.setData(bundle);
		MessageChannel.getInstance().sendMessage(msg);
	}
	
}