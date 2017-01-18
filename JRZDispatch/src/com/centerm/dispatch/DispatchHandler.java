package com.centerm.dispatch;

import com.centerm.dispatch.app.Action;
import com.centerm.dispatch.app.AppManager;
import com.centerm.dispatch.app.KeypadApp;
import com.centerm.dispatch.app.SignatureApp;
import com.centerm.dispatch.app.SystemtipApp;
import com.centerm.dispatch.communication.ComServiceThread;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class DispatchHandler extends Handler {
	//�ɴ������Ϣ�б�
	public final static int MSG_CREATE = 0;		//���ȳ��򴴽�����Ϣ
	public final static int MSG_DESTORY = 1;	//���ȳ��������˳�����Ϣ
	public final static int MSG_COMMAND = 2;	//����������������
	private int flowNo = 1;						//����Action����ˮ��
	private ComServiceThread comServiceThread = null;
	private DistributeCmd distributeCmd = null;

	private boolean isInit = false;

	public DispatchHandler(Looper looper) {
		super(looper);
	
	}

	public void stopThread()
	{
		if(comServiceThread!=null){
    		comServiceThread.stopThread();
    		comServiceThread = null;
    	}
		
		if(distributeCmd!=null){
    		distributeCmd.stopThread();
    		distributeCmd = null;
    	}
	}
	  /*
		 * 
		 */
	public  void initWait()//�߳��е��� ���ȴ���ʼ�����
	{
	
		int time = 50;
		while (!isInit) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			time--;
			if(time==0)break;
		}
	}
		
	public void initNotify()
	{
		isInit = true;
	}
	/*
	 * ��ȡΨһ����ˮ��
	 */
	public int getUniqueFlowNo() {
		return flowNo++;		
	}
	
	@Override
	public void handleMessage(Message msg) 
	{
		switch (msg.what) {
		case MSG_CREATE:			//���񴴽�
			initApplication();
	    	initService();
			break;
		case MSG_DESTORY:			//���ȳ������ٵ���Ϣ
			System.exit(msg.arg1);	//�˳����߳�
			break;
		case MSG_COMMAND:			//������������������
			Log.i(MainActivity.TAG, "Start processing MSG_COMMAND message!");
			AppManager.getManager().doAction( (Action)msg.obj );
			break;
		}
	}
	
	
	private void initService()
	{
		new Thread(){
			public void run(){
				
				comServiceThread = ComServiceThread.getInstance();
		    	comServiceThread.start();
		    	comServiceThread.initWait();
		    	distributeCmd = new DistributeCmd(DispatchHandler.this); //����ָ��ַ��߳�
				distributeCmd.start();//�����߳�
				initNotify();//��ʼ�����
			}
		}.start();
	}
	/*
	 * ��������ĸ����ʼ���񣬲���������Ӧ�ó���
	 */
	private void initApplication() {
		Message msg;
		Action action;

		//TODO:δ�����Կ��ǲ��÷��似��������Ĵ�����ö����õķ�ʽ����̬����
		//��������Ӧ�ó���������ע��app��Ȼ����ش����Ķ���	
		AppManager.getManager().registerApp(new KeypadApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), KeypadApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The KeypadApp program started successfully!");
	
		AppManager.getManager().registerApp(new SignatureApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), SignatureApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The SignatureApp program started successfully!");
	
		AppManager.getManager().registerApp(new SystemtipApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), SystemtipApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The SystemtipApp program started successfully!");
	
		/*AppManager.getManager().registerApp(new MediaplayerApp());
		action = new Action(Action.ACTION_CREATE, getUniqueFlowNo(), MediaplayerApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The MediaplayerApp program started successfully!");


		//������沥��
		action = new Action(Action.ACTION_START, getUniqueFlowNo(), MediaplayerApp.id, 0, null);
		msg = obtainMessage(MSG_COMMAND, action);
		sendMessage(msg);
		Log.i(MainActivity.TAG, "The MediaplayerApp program started successfully!..........................");
		 */
	}
}
