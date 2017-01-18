package com.centerm.systemtip.ipc;

import java.util.List;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import com.centerm.systemtip.CommonActivity;
import com.centerm.systemtip.CommonRetActivity;
import com.centerm.systemtip.MainActivity;
import com.centerm.systemtip.common.SendMsgClass;
import com.centerm.systemtip.version.VersionHandler;

/**
 * �������Ե��ȵ�������Ϣ
 */
public class MessageHandler extends Handler
{
	public static final int MSG_CLOSE = 9999;
	public static final int MSG_START = 10000;
	int nFlowNo = -1;
	
	/**
	 * ���캯���� ��handler����looper
	 */
	public MessageHandler( Looper looper )
	{
		super( looper );
	}
	
	/**
	 * 
	 * @return �رյĽ������
	 */
	public static int closeCommonActivity() 
	{
		List<CommonActivity> commonActivitys = MainActivity.commonActivitys;
		int size = commonActivitys.size();
		if ( size <= 0) //�޽��棬��ֱ�ӷ���
		{
			return 0;
		} 
		for (int i = size - 1; i >= 0; i--) //��һ�عرո�������
		{
			if (commonActivitys.get(i) != null) 
			{
				commonActivitys.get(i).closeSystemtip();
			}
		}
		commonActivitys.clear();
		return size;
	}
	
	/**
	 * 
	 * @return �رյĽ������
	 */
	public static int closeRetCommonActivity() 
	{
		List<CommonRetActivity> commonRetActivitys = MainActivity.commonRetActivitys;
		int size = commonRetActivitys.size();
		if ( size <= 0) //�޽��棬��ֱ�ӷ���
		{
			return 0;
		} 
		for (int i = size - 1; i >= 0; i--) //��һ�عرո�������
		{
			if (commonRetActivitys.get(i) != null) 
			{
				commonRetActivitys.get(i).closeSystemtip();
			}
		}
		commonRetActivitys.clear();
		return size;
	}

	@Override
	public void handleMessage(Message msg) 
	{
		VersionHandler cmdHandler = null;
		switch( msg.what )
		{
			case MessageType.MSG_START://����Ӧ�ó���
				Log.i(MainActivity.TAG, "start message systemtip");
				nFlowNo = msg.arg1;
				cmdHandler = new VersionHandler();       	
				cmdHandler.executeCommand(msg.getData().getByteArray("data"), Integer.valueOf(msg.getData().getInt("linktype")), Integer.valueOf(nFlowNo));
	        	
				break;
			case MessageType.MSG_CLOSE://�ر�Ӧ�ó���
				Log.i(MainActivity.TAG,"close mesaage systemtip");
				nFlowNo = msg.arg1;
				int n = 0;
				n += closeCommonActivity();
				n += closeRetCommonActivity();
				SendMsgClass.SendStatusToDispatch(MessageType.MSG_NOTICE_STATE_CLOSE, MainActivity.ID, nFlowNo);
				break;
			case MessageType.MSG_DATA://����Ӧ�ó�������
				nFlowNo = msg.arg1;
				cmdHandler = new VersionHandler();       	
				cmdHandler.executeCommand(msg.getData().getByteArray("data"), Integer.valueOf(msg.getData().getInt("linktype")), Integer.valueOf(nFlowNo));
				break;
			case MSG_START:
				//SendMsgClass.SendStatusToDispatch(MessageType.MSG_NOTICE_STATE_RUN, MainActivity.ID, nFlowNo);
				SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_STATE_RUN, MainActivity.ID, nFlowNo, Integer.valueOf(msg.getData().getInt("linktype")), msg.getData().getByteArray("data"));
				break;
			case MSG_CLOSE:
				SendMsgClass.SendStatusToDispatch(MessageType.MSG_NOTICE_STATE_CLOSE, MainActivity.ID, nFlowNo);
				//SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_STATE_CLOSE, MainActivity.ID, nFlowNo, Integer.valueOf(msg.getData().getInt("linktype")), msg.getData().getByteArray("data"));
				break;
			default:
				super.handleMessage(msg);
				break;
		}
	}
}
