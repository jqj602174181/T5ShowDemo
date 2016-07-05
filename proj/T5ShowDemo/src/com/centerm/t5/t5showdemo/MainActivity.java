package com.centerm.t5.t5showdemo;



import java.util.HashMap;
import java.util.Set;

import android.R.integer;
import android.app.Activity;
import android.app.FragmentTransaction;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import com.centerm.util.RetUtil;

import com.centerm.t5.t5showdemo.R;
import com.centerm.t5.t5showdemo.common.OnMessageListener;
import com.centerm.t5.t5showdemo.ui.SureDialog;
import com.centerm.t5.t5showdemo.ui.WaitDialog;
import com.centerm.t5.util.bluetooth.BluetoothOperator;
import com.centerm.t5.util.common.CommonUtil;
import com.centerm.t5.util.dev.DeviceOperatorData;
public class MainActivity extends Activity implements View.OnClickListener,OnMessageListener {

	private final int[] layoutIds={R.id.layout_connect,R.id.layout_IcCard,R.id.layout_idCard,
							R.id.layout_finger,R.id.layout_MsgCard,R.id.layout_handwrite,R.id.layout_key};
	
	private View[] viewList;
	private View lastView;
	private int selectColor =R.color.white;
	
	//��ǩҳ����
	private FragmentBase fragPasswordKeyPad;
	private FragmentBase fragICCard;
	private FragmentBase fragIDCard;
	private FragmentBase fragHandwrite;
	private FragmentBase fragMsgCard;
	private FragmentBase fragFingerprint;
	private FragmentBlooth fragBlooth;
	
	private FragmentBase currentFragment;
	private BluetoothOperator bluetoothOperator;
	
	private SureDialog sureDialog;
	private WaitDialog waitDialog;
	private Handler mainHandler;
	
	private String bluetoothNoConnect="";
	
	private String readWait;
	private boolean isConnect = false;//�����ж��Ƿ�������
	private ReadWaitTime readWaitTime;

	private boolean isPressConnect = false;
	String macAddr;
	
	private HashMap<String, String> errorMap = new HashMap<String, String>();
	public final static String Unknown_Err = "DRV0000001";
	public final static String Timeout_Err = "DRV0000002";
	public final static String Open_Serial_Err = "DRV0000003";	
	public final static String Send_Mess_Err = "DRV0000004";
	
	public final static String Param_Err = "DRV0000005";
	public final static String Not_Find_So_Err = "DRV0000006";
	public final static String Load_So_Err = "DRV0000007";
	public final static String Device_Not_Connect = "DRV0000008";
	public final static String Recv_Error_Mess = "DRV0000009";
	public final static String Device_Connect_Broken = "DRV0000010";
	public final static String ARQC_ERROR = "DRV0000204";
	public final static String ShangDian_ERROR = "DRV0000101";
	public final static String ARQC_ERROR_Msg = "��ȡARQCʧ��";
	public final static String ShangDian_ERROR_Msg = "�ϵ�ʧ��";
	
	public final static String Unknown_Err_Msg = "δ֪����";
	public final static String Timeout_Err_Msg = "��ʱ����";
	public final static String Open_Serial_Err_Msg = "�򿪴���ʧ��";
	public final static String Send_Mess_Err_Msg = "���ͻ���ձ���ʧ��";
	
	public final static String Param_Err_Msg = "�ӿڲ�������";
	public final static String Not_Find_So_Err_Msg = "�Ҳ�����̬���ӿ�";
	public final static String Load_So_Err_Msg = "��̬���ӿ���ش���";
	public final static String Device_Not_Connect_Msg = "ͨѶ����û�н���";
	public final static String Recv_Error_Mess_Msg = "���յı��ĸ�ʽ����";
	public final static String Device_Connect_Broken_Msg = "ͨѶ�����ƻ�";

	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		FragmentBlooth.currentState = "δ���";
		super.onCreate(savedInstanceState);
		FragmentBlooth.isConnect = false;
		setContentView(R.layout.activity_main);
		readWait 		= getString(R.string.readWait);
		viewList 		= new View[layoutIds.length];
		readWaitTime 	= new ReadWaitTime();
		for(int i=0;i<layoutIds.length;i++){
			viewList[i] = findViewById(layoutIds[i]);
			
			viewList[i].setOnClickListener(this);
		}
		lastView = viewList[0];
		FragmentTransaction ft = getFragmentManager().beginTransaction();
		fragBlooth = new FragmentBlooth();
		ft.add(R.id.content, fragBlooth);
		currentFragment = fragBlooth;
		ft.commit();
		setHandler();
		bluetoothOperator = new BluetoothOperator(mainHandler,this);
		bluetoothNoConnect = getString(R.string.bluetoothNoConnect);
		
		errorMap.put(Unknown_Err, Unknown_Err_Msg);
		errorMap.put(Timeout_Err, Timeout_Err_Msg);
		errorMap.put(Open_Serial_Err, Open_Serial_Err_Msg);
		errorMap.put(Send_Mess_Err, Send_Mess_Err_Msg);
		
		errorMap.put(Param_Err, Param_Err_Msg);
		errorMap.put(Not_Find_So_Err, Not_Find_So_Err_Msg);
		errorMap.put(Load_So_Err, Load_So_Err_Msg);
		errorMap.put(Device_Not_Connect, Device_Not_Connect_Msg);
		
		errorMap.put(Open_Serial_Err_Msg, Recv_Error_Mess_Msg);
		errorMap.put(Device_Connect_Broken, Device_Connect_Broken_Msg);
		
		errorMap.put(Recv_Error_Mess, Recv_Error_Mess_Msg);
		errorMap.put(ARQC_ERROR, ARQC_ERROR_Msg);
		errorMap.put(ShangDian_ERROR, ShangDian_ERROR_Msg);
		
		/*
		BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
		Set<BluetoothDevice> devices = adapter.getBondedDevices();
		for(int i=0;i<devices.size();i++){
			 BluetoothDevice device = (BluetoothDevice) devices.iterator().next();
			//Log.e("dev"," devices name is "+device.getName()+" addr is "+device.getAddress());
		}*/

	}
	
	
	private void setHandler()
	{
		mainHandler = new Handler(){

			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				fragBlooth.setConnectState(msg.what);//�ж�����״̬
				Log.e("Dev","setHandler: msg.what = "+ msg.what );
				switch (msg.what) {
				case DeviceOperatorData.CONNECTFAIL://�豸����ʧ��
				//	waitDialog.closeDialog();
					closeWaitDialog();
					isConnect = false;
					break;
				case DeviceOperatorData.CONNECTSUCCESS://�豸���ӳɹ�
					isConnect= true;
					waitDialog.setText(readWait);
					closeWaitDialog();
					break;
				case DeviceOperatorData.NODEVICE://û���豸
					isConnect = false;
					closeWaitDialog();
					CommonUtil.showTip(MainActivity.this, MainActivity.this.getString(R.string.checkBluetooth));
					break;
				case DeviceOperatorData.CONNECTCANCEL://�豸����ʧ��
					isConnect = false;
					closeWaitDialog();
					break;
				case DeviceOperatorData.MESSAGE://��ȡ��Ϣ
					currentFragment.setData(msg.obj);//����ȡ����Ϣ��ʾ��UI����
					waitDialog.closeDialog();
					readWaitTime.stopRun();
					break;
				case DeviceOperatorData.READ_FAIL://��ȡʧ��
					waitDialog.closeDialog();
					readWaitTime.stopRun();
					break;
				default:
					break;
				}
			}
			
		};
	}
	
	
	private void replaceFragemtn(FragmentBase fragment)
	{
		currentFragment = fragment;
		FragmentTransaction ft = getFragmentManager().beginTransaction();
		ft.setCustomAnimations(R.anim.slide_in_left, R.anim.slide_out_right);
		ft.replace(R.id.content,fragment);
		
		ft.commit();
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		//super.onBackPressed();
		if(sureDialog==null){
			sureDialog = new SureDialog(this, this);
		}
		sureDialog.showDialog();
	}

	//��ǩҳ�л�
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stu
	//	bluetoothOperator.sendMessage(0);
		if(lastView!=v){
			lastView.setBackgroundResource(0);
			lastView = v;
			lastView.setBackgroundResource(selectColor);
		}else{
			//return;
		}
		switch (v.getId()) {
			case R.id.layout_key:
				//sendMessage(0);
				if(fragPasswordKeyPad==null){
					fragPasswordKeyPad = new FragmentPasswordKeyPad();
				}
				replaceFragemtn(fragPasswordKeyPad);
				break;
			case R.id.layout_IcCard:
			
				if(fragICCard==null){
					fragICCard = new FragmentICCard();
				}
				replaceFragemtn(fragICCard);
				break;
			case R.id.layout_idCard:
		
		
				if(fragIDCard==null){
					fragIDCard = new FragmentIDCard();
				}
				replaceFragemtn(fragIDCard);
				break;
			case R.id.layout_finger:
			
				if(fragFingerprint==null){
					fragFingerprint = new FragmentFingerprint();
				}
				replaceFragemtn(fragFingerprint);
				break;
			case R.id.layout_MsgCard:
			
				if(fragMsgCard==null){
					fragMsgCard = new FragmentMsgCard();
				}
				replaceFragemtn(fragMsgCard);
				break;
			case R.id.layout_connect:
				if(fragBlooth==null){
					fragBlooth = new FragmentBlooth();
				}
				
				
				replaceFragemtn(fragBlooth);
				if(bluetoothOperator!=null){
					fragBlooth.setConnect(bluetoothOperator.getIsConnect());
				}else{
					
				}
				break;
			case R.id.layout_handwrite:
			
				if(fragHandwrite==null){
					fragHandwrite = new FragmentSign();
				}
				replaceFragemtn(fragHandwrite);
//				int style =  DeviceOperatorData.SIGN;
//				sendMessage(style);
				break;
			default:
				break;
		}
	}


	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		bluetoothOperator.quitBluetooth();
//		CommService.destroyCommServer();
		readWaitTime.stopRun();
	
	}
	
	
	void sendMessage(int style)
	{
		if(!bluetoothOperator.getIsConnect()){
			CommonUtil.showTip(this, bluetoothNoConnect);
			return;
		}
			bluetoothOperator.sendMessage(0,style);
			waitDialog.showDialog();
			readWaitTime.startRun(20);
	}
	
	
	/**
	 * ���ڽ���ģ��Ĳ���
	 * @param style ----��ʾ������ģ�� 
	 * 
	 *
	 */
	void sendMessage(int style,Object object,int time)
	{
		if(time!=-1){
			readWaitTime.startRun(time+5);
			
		}
		final int style1 = style;
		final Object obj = object;
		
		if(waitDialog==null){
			waitDialog = new WaitDialog(this, false, this);
			waitDialog.setText(readWait);
		}
		waitDialog.showDialog();
		
		if(!bluetoothOperator.getIsConnect()){//���豸û�����ӳɹ���������������ִ��
			isPressConnect = false;
			new Thread(){
				
				public void run()
				{
					super.run();
					bluetoothOperator.setBluetoothMac(macAddr);
					bluetoothOperator.startBluetooth();
					connectAndSend(style1, obj);
					
				}
			}.start();
			
		}else{
			bluetoothOperator.sendMessage(0,style,object);// ģ������� style�����ж������ֽ���ģ�������
	
		}
		
	
	}
	
	public void sendMessage1(int style,Object object,int time)
	{
		if(time!=-1){
			//readWaitTime.startRun(time+5);
			
		}
		bluetoothOperator.sendMessage(0,style,object);// ģ������� style�����ж������ֽ���ģ�������
	}
	
	private void connectAndSend(int style,Object object)
	{
		int waitTime = 10;
		//�ж�����״̬
		while(true){
			if(waitTime==0)
				break;
		
			if(bluetoothOperator.getIsConnect()){
				break;
			}
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			waitTime--;
			
		}

		if(!bluetoothOperator.getIsConnect()){//�����ʱ��δ�����򷵻ش�����Ϣ
			String[] error= new String[2];
			error[0]="1";
			error[1]=RetUtil.Device_Not_Connect;
			Message message = mainHandler.obtainMessage(DeviceOperatorData.MESSAGE);
			message.obj = error;
			mainHandler.sendMessage(message);
			return;
		}
		bluetoothOperator.sendMessage(0,style,object);//����ģ�����
	}
	
	/**
	 * �豸����
	 * */
	void connectBluetooth(String mac)
	{
		isPressConnect = true; 
		if(waitDialog==null){
			waitDialog = new WaitDialog(this, false, this);
		}
		bluetoothOperator.setBluetoothMac(mac);//����������ַ
		bluetoothOperator.startBluetooth();//���������豸
		waitDialog.show();
	}
	
	
	
	
	@Override
	public void onMessageChange(int msg) {
		// TODO Auto-generated method stub
		switch (msg) {
			case DeviceOperatorData.CONNECTCANCEL://ȡ������
				if(isConnect){
					
				}else{
					bluetoothOperator.quitBluetooth();
					fragBlooth.setConnect(true);
					waitDialog.closeDialog();
				}
				
				break;
			case 1:
				finish();//�˳� 
				break;
			default:
				break;
		}
		
	}
	
	
	private class ReadWaitTime implements Runnable{
	private Handler handler;
	private int time = 30;
	private final static int delayTime = 1000;
	private boolean isStop = false;
	public ReadWaitTime()
	{
		time =60;
		handler = new Handler();
	}
	public void run() {
		if(isStop)return;
		handler.postDelayed(this, delayTime);
	
		time--;
		if(time==0){
			waitDialog.closeDialog();
			handler.removeCallbacks(this);
			isStop = false;
		}
	}
	
	public void startRun(int time)
	{
		isStop = false;
		this.time = time;
		handler.postDelayed(this, delayTime);
	}
	
	public void stopRun()
	{
		isStop = true;
		handler.removeCallbacks(this);
	}
	}


	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
	
	
	public void closeWaitDialog()
	{
		if(isPressConnect){
			waitDialog.closeDialog();
		}
	}

	public void setMacAddr(String macAddr)
	{
		this.macAddr =macAddr;
	}
	
	public String getErrorMsg(String key)
	{
		String error = errorMap.get(key);
		if(error!=null){
			return error;
		}else{
			return Unknown_Err_Msg;
		}
	}
}
