package com.centerm.systemtip;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import com.centerm.systemtip.common.SendMsgClass;
import com.centerm.systemtip.ipc.MessageChannel;
import com.centerm.systemtip.ipc.MessageHandler;
import com.centerm.systemtip.ipc.MessageType;

/**
 * 所有Activity的父类，封装界面的共同元素和操作
 *
 */
public abstract class CommonActivity extends Activity
{
	private final static int MSG_CLOSE_SYSTEMTIP = 1109;//结束系统提示	

	//可供子类直接修改的参数
	public int nLinkType = 0;               //主机的连接接口

	public boolean enableKeyEvent = false;   //是否允许按键响应

	//以下子类不可重新修改
	private  boolean bCloseFlag = false;	    //是否正在关闭界面

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		MainActivity.commonActivitys.add(this);

		nLinkType = getIntent().getIntExtra("linktype", -1);//获取主机连接类型

		sendMsgToMsgHandler(MessageHandler.MSG_START);//发送消息给调度，通知其状态发生了变化
	}

	//该函数用于判断应用程序所占内存是否到达一定程度，如到了一定程度则关闭该进程。
	private void checkMemoryOutAndClose()
	{
		int pids[] = { android.os.Process.myPid() };
		System.out.println( "Keypad pids="+ pids[0] );
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);  
		android.os.Debug.MemoryInfo[] memoryInfoArray = am.getProcessMemoryInfo( pids );
		android.os.Debug.MemoryInfo pidMemoryInfo = memoryInfoArray[0];

		System.out.println( "Keypad pidMemoryInfo.getTotalPss()" + pidMemoryInfo.getTotalPss() );
		if (pidMemoryInfo.getTotalPss() > 50000)
		{
			System.out.println( "killed!");
			android.os.Process.killProcess( android.os.Process.myPid() );
		}
	}

	@Override
	protected void onDestroy()
	{
		enableKeyEvent = false;//不允许再响应按键了

		//关闭密码键盘
		if( bCloseFlag )
		{
			sendMsgToMsgHandler(MessageHandler.MSG_CLOSE);
		}
		super.onDestroy();
		checkMemoryOutAndClose();
	}


	/**
	 * 发送消息给调度，通知其状态发生了变化
	 * @param msgWhat
	 */
	private void sendMsgToMsgHandler(int msgWhat)
	{
		if(MessageChannel.handler != null){
			Message msgTmp = MessageChannel.handler.obtainMessage(msgWhat);
			msgTmp.sendToTarget();
		}
	}

	//用于处理关闭界面的消息处理
	@SuppressLint("HandlerLeak")
	private Handler mHandler = new Handler() 
	{
		@Override
		public void handleMessage(Message msg) 
		{
			if (msg.what == MSG_CLOSE_SYSTEMTIP )
			{
				bCloseFlag = true;
				finish();
			}
			super.handleMessage(msg);
		}
	};

	/**
	 * 结束信息交互界面
	 */
	public void closeSystemtip()
	{
		Message closeMsg = mHandler.obtainMessage(MSG_CLOSE_SYSTEMTIP);
		closeMsg.sendToTarget();
	}

	/**
	 * 发送结果给主机和调度
	 * @param retData
	 */
	public void sendResultToDispatch( byte[] retData )
	{
		SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_OPERATOR_END, 
				MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, retData );
	}
}