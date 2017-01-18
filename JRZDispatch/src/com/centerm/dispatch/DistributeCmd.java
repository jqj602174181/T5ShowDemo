package com.centerm.dispatch;


import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import com.centerm.dispatch.app.Action;
import com.centerm.dispatch.app.AppManager;
import com.centerm.dispatch.app.SystemtipApp;
import com.centerm.dispatch.communication.CommandEvent;
import com.centerm.dispatch.instruction.VersionHandler;

/* 主机通讯服务接口 */
public class DistributeCmd extends Thread
{
	//指令类型列表
	public final static int TYPE_INS_NONE = 0; //指令类型待定
	public final static int TYPE_INS_SHOW = 1; //有界面启动指令
	public final static int TYPE_INS_DATA = 2; //后台操作指令
	public final static int TYPE_INS_CLOSE = 3;//关闭程序界面指令
	public final static int TYPE_INS_EXIT = 4; //退出程序指令

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
		//从指令队列获取指令
		do
		{
			Log.w(MainActivity.TAG, "MainActivity.msgList.getMsg().....................");
			event = DispatchApplication.msgList.getMsg();
			if ( DispatchApplication.msgList.isEmptyMsg() == true )
			{
				DispatchApplication.msgList.setPriority(0);
			}
			//处理调度指令
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
	 * @param insMsg 指令消息对象
	 * 
	 * 将从指令队列获取的消息分发给处理线程处理
	 * 
	 */
	private void dealCommand(CommandEvent event) 
	{
		//TODO：指令分发
		Action action;
		Message msg;
		
		//将指令解析数据以键值对的方式存放
		Bundle transferData = new Bundle();
		transferData.putInt("linktype", event.getComDeivce());
		transferData.putByteArray("data", event.getCmdData());

		switch (event.getInsType()) 
		{
			//界面显示类指令处理
			case TYPE_INS_SHOW:
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_SHOW message!");
				//TODO:程序状态和优先级处理
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
			//数据处理类指令处理
			case TYPE_INS_DATA: 
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_DATA message!");
				action = new Action( Action.ACTION_DATA, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				break;
			//关闭程序界面类指令处理
			case TYPE_INS_CLOSE:
				Log.i(MainActivity.TAG, "Start processing TYPE_INS_CLOSE message!");
				action = new Action( Action.ACTION_CLOSE, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				Log.i(MainActivity.TAG, "End of close message processing!");
				break;
			//退出程序类指令处理
			case TYPE_INS_EXIT:
				action = new Action( Action.ACTION_EXIT, dh.getUniqueFlowNo(), event.getProgramIndex(), 0, transferData );
				msg = dh.obtainMessage( DispatchHandler.MSG_COMMAND, action);
				dh.sendMessage(msg);
				break;
		}
	}

    
	/**
	 * 
	 * @param insMsg 指令消息对象
	 * 
	 * 处理调度程序指令
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

