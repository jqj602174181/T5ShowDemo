package com.centerm.dispatch.ipc;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.app.Action;
import com.centerm.dispatch.app.AppManager;
import com.centerm.dispatch.app.AppStatus;
import com.centerm.dispatch.app.Notice;
import com.centerm.dispatch.communication.BTAcceptThread;
import com.centerm.dispatch.communication.ComService;
import com.centerm.dispatch.communication.HIDAcceptThread;
import com.centerm.dispatch.communication.ResultAcceptThread;

/**
 * 接收来自各个应用程序的通信请求
 */
public class MessageHandler extends Handler 
{
	private ComService comService = null;
	/**
	 * 构造函数， 将handler绑定至looper
	 */
	public MessageHandler( Looper looper )
	{
		super( looper );
		comService = ComService.getInstance();
	}

	@Override
	public void handleMessage(Message msg) 
	{
		boolean ret;
		int state;
		Log.i( MainActivity.TAG, "Message Service: get msg " + msg.what );
		switch( msg.what )
		{
			case MessageType.MSG_CONTACTME://应用程序建立了与调度的通信链路
	
				AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_CLOSE);
				doContactMe( msg );//记录应用程序的Messenger,并尝试向Messenger发送成功消息
				break;
			case MessageType.MSG_FORGETME:
				Log.i( MainActivity.TAG, "MessageType.MSG_FORGETME: get msg " + msg.what );
				restartApps(msg.arg1);
				break;
			case MessageType.MSG_SEND_DATA://发送数据给主机 ，目前调度返回没有使用该消息
				Log.i( MainActivity.TAG, "............................ " + msg.what );
				if ( msg.getData().getByteArray("data") != null )
				{
					
					Log.i( MainActivity.TAG, "MessageType.MSG_SEND_DATA....." + msg.getData().getInt("linktype") );
					Log.i( MainActivity.TAG, "MessageType.MSG_SEND_DATA....." + new String(msg.getData().getByteArray("data")) );
					comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				break;
				
			case MessageType.MSG_NOTICE_OPERATOR_END://通知界面操作执行后程序状态,arg1表示应用程序id，arg2为执行应用程序状态，obj表示发送给主机的数据
				AppManager.getManager().setAppStatus(msg.arg1, msg.arg2);
				

				//TODO:发送启动多媒体消息到指令消息监听队列
				if ( msg.getData().getByteArray("data") != null )
				{
					Log.i( MainActivity.TAG, "MSG_NOTICE_OPERATOR_END.....datalen=" + msg.getData().getByteArray("data").length );
					Log.i( MainActivity.TAG, "MSG_NOTICE_OPERATOR_END.....data=" + new String(msg.getData().getByteArray("data")));
			
					comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				break;

			case MessageType.MSG_NOTICE_STATE_DATA:
				Log.i(MainActivity.TAG, "MessageType.MSG_NOTICE_STATE_DATA linktype=" + msg.getData().getInt("linktype"));	
				if ( msg.getData().getByteArray("data") != null )
				{
				
					comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				noticeActionResult( msg.arg2, Action.RESULT_OK );
				break;

			case MessageType.MSG_NOTICE_STATE_CLOSE:
                Log.i(MainActivity.TAG, "MSG_NOTICE_STATE_CLOSE arg1=" + msg.arg1);				
                ret = AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_CLOSE);
                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//设置结果状态
                if ( msg.getData().getByteArray("data") != null )
				{
                	comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				noticeActionResult( msg.arg2, state );
				break;
			case MessageType.MSG_NOTICE_STATE_RUN:
				Log.i(MainActivity.TAG, "MSG_NOTICE_STATE_RUN arg1=" + msg.arg1 );
                ret = AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_RUN);
                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//设置结果状态
                if ( msg.getData().getByteArray("data") != null )
				{
                	
                	comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				noticeActionResult( msg.arg2, state );
				break;
			case MessageType.MSG_SIGN_BYTE_DATA:
//				Log.i(MainActivity.TAG, "MSG_NOTICE_STATE_RUN arg1=" + msg.arg1 );
//                ret = AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_RUN);
//                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//设置结果状态
                if ( msg.getData().getByteArray("data") != null )
				{
                	//comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				break;
			case MessageType.MSG_NOTICE_SEND_DATA://北京农商行 指纹仪
				AppManager.getManager().setAppStatus(msg.arg1, msg.arg2);
				if ( msg.getData().getByteArray("data") != null )
				{
					Log.i( MainActivity.TAG, "MSG_NOTICE_SEND_DATA.....datalen=" + msg.getData().getByteArray("data").length );
					Log.i( MainActivity.TAG, "MSG_NOTICE_SEND_DATA.....data=" + new String(msg.getData().getByteArray("data")));

					int linktype = msg.getData().getInt("linktype");
					byte[] data = msg.getData().getByteArray("data");
					if (linktype == ComService.LINK_HID) {
						HIDAcceptThread.getInstance().writeHid(data);
					}
					else if (linktype == ComService.LINK_BLUETOOTH) {
						BTAcceptThread.getInstance().sendData(data);
					}
				}
				break;
			default:
			
				super.handleMessage(msg);
				//TODO:log
				break;
		}
	}
	
	/**
	 * 通知应用程序管理器，上一个动作的执行结果
	 * @param flowNo 流水号
	 * @param state 执行的结果
	 */
	private void noticeActionResult( int flowNo, int state )
	{
		Notice notice = new Notice( Notice.TYPE_ACTION_RESULT, flowNo, state );//流水号保存在arg2中
		AppManager.getManager().doNotice( notice );
	}
	
	/**
	 * 记录应用程序的Messenger,并尝试向Messenger发送成功消息
	 * @param msg 应用程序发送的消息
	 */
	private void doContactMe( Message msg )
	{
		//得到应用程序通信接收对象
		Messenger msger = msg.replyTo;
		if( msger == null )//应用程序不应该发送一个不带replyTo的MSG_CONNECT消息
		{
			//TODO:log
			return;
		}
		
		//注册应用程序的通信接收对象		
		boolean ret = AppManager.getManager().setAppMessenger(msg.arg1, msger);		
				
		//告诉AppManager，Action执行结束。
		int state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//设置结果状态
		noticeActionResult( msg.arg2, state );//流水号保存在arg2中
	}
	
	private void restartApps( int id )
	{
		Action action;
		
		if ( id > 0 && id < 8 )
		{
			AppManager.getManager().setAppMessenger( id, null);
			action = new Action( Action.ACTION_CREATE, 0, id, 0, null );
			AppManager.getManager().doAction( action );
		}
	}

}


