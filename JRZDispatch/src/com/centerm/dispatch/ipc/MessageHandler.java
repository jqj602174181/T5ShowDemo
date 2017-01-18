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
 * �������Ը���Ӧ�ó����ͨ������
 */
public class MessageHandler extends Handler 
{
	private ComService comService = null;
	/**
	 * ���캯���� ��handler����looper
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
			case MessageType.MSG_CONTACTME://Ӧ�ó�����������ȵ�ͨ����·
	
				AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_CLOSE);
				doContactMe( msg );//��¼Ӧ�ó����Messenger,��������Messenger���ͳɹ���Ϣ
				break;
			case MessageType.MSG_FORGETME:
				Log.i( MainActivity.TAG, "MessageType.MSG_FORGETME: get msg " + msg.what );
				restartApps(msg.arg1);
				break;
			case MessageType.MSG_SEND_DATA://�������ݸ����� ��Ŀǰ���ȷ���û��ʹ�ø���Ϣ
				Log.i( MainActivity.TAG, "............................ " + msg.what );
				if ( msg.getData().getByteArray("data") != null )
				{
					
					Log.i( MainActivity.TAG, "MessageType.MSG_SEND_DATA....." + msg.getData().getInt("linktype") );
					Log.i( MainActivity.TAG, "MessageType.MSG_SEND_DATA....." + new String(msg.getData().getByteArray("data")) );
					comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				break;
				
			case MessageType.MSG_NOTICE_OPERATOR_END://֪ͨ�������ִ�к����״̬,arg1��ʾӦ�ó���id��arg2Ϊִ��Ӧ�ó���״̬��obj��ʾ���͸�����������
				AppManager.getManager().setAppStatus(msg.arg1, msg.arg2);
				

				//TODO:����������ý����Ϣ��ָ����Ϣ��������
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
                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//���ý��״̬
                if ( msg.getData().getByteArray("data") != null )
				{
                	comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				noticeActionResult( msg.arg2, state );
				break;
			case MessageType.MSG_NOTICE_STATE_RUN:
				Log.i(MainActivity.TAG, "MSG_NOTICE_STATE_RUN arg1=" + msg.arg1 );
                ret = AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_RUN);
                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//���ý��״̬
                if ( msg.getData().getByteArray("data") != null )
				{
                	
                	comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				noticeActionResult( msg.arg2, state );
				break;
			case MessageType.MSG_SIGN_BYTE_DATA:
//				Log.i(MainActivity.TAG, "MSG_NOTICE_STATE_RUN arg1=" + msg.arg1 );
//                ret = AppManager.getManager().setAppStatus(msg.arg1, AppStatus.STATUS_PROGRAM_RUN);
//                state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//���ý��״̬
                if ( msg.getData().getByteArray("data") != null )
				{
                	//comService.sendResultToJNI( msg.getData().getInt("linktype"), msg.getData().getByteArray("data"), msg.getData().getByteArray("data").length );
				}
				break;
			case MessageType.MSG_NOTICE_SEND_DATA://����ũ���� ָ����
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
	 * ֪ͨӦ�ó������������һ��������ִ�н��
	 * @param flowNo ��ˮ��
	 * @param state ִ�еĽ��
	 */
	private void noticeActionResult( int flowNo, int state )
	{
		Notice notice = new Notice( Notice.TYPE_ACTION_RESULT, flowNo, state );//��ˮ�ű�����arg2��
		AppManager.getManager().doNotice( notice );
	}
	
	/**
	 * ��¼Ӧ�ó����Messenger,��������Messenger���ͳɹ���Ϣ
	 * @param msg Ӧ�ó����͵���Ϣ
	 */
	private void doContactMe( Message msg )
	{
		//�õ�Ӧ�ó���ͨ�Ž��ն���
		Messenger msger = msg.replyTo;
		if( msger == null )//Ӧ�ó���Ӧ�÷���һ������replyTo��MSG_CONNECT��Ϣ
		{
			//TODO:log
			return;
		}
		
		//ע��Ӧ�ó����ͨ�Ž��ն���		
		boolean ret = AppManager.getManager().setAppMessenger(msg.arg1, msger);		
				
		//����AppManager��Actionִ�н�����
		int state = ret ? Action.RESULT_OK : Action.RESULT_ERR;	//���ý��״̬
		noticeActionResult( msg.arg2, state );//��ˮ�ű�����arg2��
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


