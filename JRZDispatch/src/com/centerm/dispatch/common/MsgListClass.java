package com.centerm.dispatch.common;

import java.util.LinkedList;
import java.util.List;

import android.util.Log;

import com.centerm.dispatch.communication.CommandEvent;

public class MsgListClass 
{
	private int listPriority = 0;

	List<CommandEvent> msgList = new LinkedList<CommandEvent>();
	
	public synchronized CommandEvent getMsg() 
	{
		Log.w("MsgList", "....................remove msgList.size()=" + msgList.size());
		if ( msgList.size() <= 0 ) 
		{
			try
			{
				wait();
			}
			catch ( InterruptedException ie )
			{
				ie.printStackTrace();
			}
		}
		Log.w("MsgList", "remove msgList.size()=" + msgList.size());
		CommandEvent msg = msgList.get(0);
		msgList.remove(0);
		
		return msg;
	}
	
	public synchronized void emptyMsg()
	{
		if (msgList !=null && msgList.size() > 0)
		{
			msgList.clear();
			Log.w("MsgList", "emptyMsg msgList.size()=" + msgList.size());
		}
	}
	
	public boolean isEmptyMsg()
	{
		return msgList.isEmpty();
	}
	
	/**
	 * ����Ϣ�ҵ�����β��
	 * @param msg ��Ϣ
	 */
	public synchronized void addMsg( CommandEvent msg ) 
	{
		msgList.add(msg);
		Log.w("MsgList", "add msgList.size()=" + msgList.size());
		notifyAll();
	}
	
	/**
	 * ����Ϣ�ҵ������ײ������������������ִ�и�����
	 * @param msg ��Ϣ
	 */
	public synchronized void insertMsg( CommandEvent msg ) 
	{
		msgList.add(0, msg);
		notifyAll();
	}
	
	public synchronized int getPriority()
	{
		return listPriority;
	}
	
	public synchronized void setPriority( int listPriority ) 
	{
        this.listPriority = listPriority;
	}
}


