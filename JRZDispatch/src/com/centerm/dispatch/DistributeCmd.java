package com.centerm.dispatch;


import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import com.centerm.dispatch.app.Action;
import com.centerm.dispatch.app.AppManager;
import com.centerm.dispatch.app.SystemtipApp;
import com.centerm.dispatch.communication.CommandEvent;
import com.centerm.dispatch.instruction.VersionHandler;

/* ����ͨѶ����ӿ� */
public class DistributeCmd extends Thread
{
	//ָ�������б�
	public final static int TYPE_INS_NONE = 0; //ָ�����ʹ���
	public final static int TYPE_INS_SHOW = 1; //�н�������ָ��
	public final static int TYPE_INS_DATA = 2; //��̨����ָ��
	public final static int TYPE_INS_CLOSE = 3;//�رճ������ָ��
	public final static int TYPE_INS_EXIT = 4; //�˳�����ָ��

	private DispatchHandler dh = null;
	private boolean isStop =false;
    
    public DistributeCmd( DispatchHandler dh) 
    {
    	this.dh = dh;
    }

    @Override
	public void run()
    {

    	CommandEvent event = null;
		//��ָ����л�ȡָ��
		do
		{
			Log.w(MainActivity.TAG, "MainActivity.msgList.getMsg().....................");
			event = DispatchApplication.msgList.getMsg();
			if ( DispatchApplication.msgList.isEmptyMsg() == true )
			{
				DispatchApplication.msgList.setPriority(0);
			}
			//�������ָ��
			if ( event.getProgramIndex() == 0 || event.getInsType() == TYPE_INS_NONE)
			{
				dispatchSetting(event);
				continue;
			}
			dealCommand(event);
			Log.w(MainActivity.TAG, "dealCommand(event).....................");
		} while(!isStop);
		
		Log.e("th"," stop thread is CommandEvent");
	}

    
    public void stopThread()
    {
    	isStop = true;
    }
	/**
	 * 
	 * @param insMsg ָ����Ϣ����
	 * 
	 * ����ָ����л�ȡ����Ϣ�ַ��������̴߳���
	 * 
	 */
	private void dealCommand(CommandEvent event) 
	{
		//TODO��ָ��ַ�
		Action action;
		Message msg;
		
		//��ָ����������Լ�ֵ�Եķ�ʽ���
		Bundle transferData = new Bundle();
		transferData.putInt("linktype", event.getComDeivce());
		transferData.putByteArray("data", event.getCmdData());

		switch (event.getInsType()) 
		{
			//������ʾ��ָ���
			case TYPE_INS_SHOW:
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_SHOW message!");
				//TODO:����״̬�����ȼ�����
				if ( (AppManager.getManager().getCurPriority() != -1) && 
					 (AppManager.getManager().getCurProgram() <= SystemtipApp.id) )
				{
					Log.i(MainActivity.TAG, "Starting the program state switching process!" + AppManager.getManager().getCurPriority());
					if ( AppManager.getManager().getAppPriority(event.getProgramIndex()) >= AppManager.getManager().getCurPriority() )
					{
						action = new Action( Action.ACTION_CLOSE, dh.getUniqueFlowNo(), AppManager.getManager().getCurProgram(), 0, transferData );
						msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
						dh.sendMessage(msg);
					}
					else
					{
						Log.i(MainActivity.TAG, "Throw away the instruction!");
						break;
					}
				}
				action = new Action( Action.ACTION_START, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				Log.i(MainActivity.TAG, "End of show message processing!");
				break;
			//���ݴ�����ָ���
			case TYPE_INS_DATA: 
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_DATA message!");
				action = new Action( Action.ACTION_DATA, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				break;
			//�رճ��������ָ���
			case TYPE_INS_CLOSE:
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_CLOSE message!");
				action = new Action( Action.ACTION_CLOSE, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				Log.i(MainActivity.TAG, "End of close message processing!");
				break;
			//�˳�������ָ���
			case TYPE_INS_EXIT:
				action = new Action( Action.ACTION_EXIT, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				break;
		}
	}

    
	/**
	 * 
	 * @param insMsg ָ����Ϣ����
	 * 
	 * ������ȳ���ָ��
	 * 
	 */
    private void dispatchSetting(CommandEvent event)
    {
    	if (event.getProgramIndex() == 0 || event.getInsType() == TYPE_INS_NONE)
    	{
    		VersionHandler insHandler = new VersionHandler();       	
        	insHandler.executeCommand(event.getCmdData(), event.getComDeivce());
    	}
	}
}

