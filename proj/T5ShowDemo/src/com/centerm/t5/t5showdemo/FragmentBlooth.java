package com.centerm.t5.t5showdemo;

import java.util.ArrayList;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.nfc.Tag;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.TextView;
import com.centerm.device.CommService;
import com.centerm.t5.t5showdemo.R;
import com.centerm.t5.util.dev.DeviceOperatorData;
import com.example.commenlibary.Interface.CommenLibaryMessageOperator;
import com.example.commenlibary.ui.ListDialog.SelectListDialog;

public class FragmentBlooth extends FragmentBase implements View.OnClickListener,OnCheckedChangeListener,CommenLibaryMessageOperator{

	private Button btnConnect;
	private TextView tvState;
	private EditText etMac;

	private RadioGroup rgConn;
	private String connectSuccess = "连接成功";
	private String connectFail = "连接失败";

	private String connectCancel = "连接断开";
	private String noCheck = "未检测";
    static String currentState = "未检测";
    
    private final static  String macAddr ="macAddr"; 
    private  ArrayList<String> bluetoothList =null;
    private ArrayList<String> macList = null;
    private SelectListDialog selectListDialog;
	BluetoothReceiver bluetoothReceiver;
	private int selectIndex = 0;
	
	private Button btnMore;
	public View onCreateView(LayoutInflater inflater,ViewGroup container,Bundle savedInstanceState)
	{
		View view = super.onCreateView(inflater, container, savedInstanceState, R.layout.frag_communication_conn);
		tvState 	= (TextView)view.findViewById(R.id.tvBTStat);
		btnConnect 	= (Button)view.findViewById(R.id.bt_conn_open);
		etMac		= (EditText)view.findViewById(R.id.etMac);
		rgConn		= (RadioGroup)view.findViewById(R.id.rg_conn);
		btnMore 	= (Button)view.findViewById(R.id.btnMore);
		btnMore.setOnClickListener(this);
		rgConn.setOnCheckedChangeListener(this);
		btnConnect.setOnClickListener(this);
		bluetoothList = new ArrayList<String>();
		macList		  = new ArrayList<String>();
		selectListDialog = new SelectListDialog(mainActivity, 300, bluetoothList);
		selectListDialog.setCommenLibaryMessageOperator(this);
		CommService.type = 1;
		
		tvState.setText(currentState);
		btnConnect.setEnabled(!isConnect);
		
		SharedPreferences Pre = mainActivity.getSharedPreferences(macAddr, Context.MODE_PRIVATE);
		String name = Pre.getString("name", getString(R.string.macAddr));
		String address = Pre.getString("mac",  getString(R.string.macAddr));
		etMac.setText(name);
		mainActivity.setMacAddr(name);
		macList.add(address);
		selectListDialog.addSelectObject(name);
		
		bluetoothReceiver = new BluetoothReceiver(mainActivity);
		bluetoothReceiver.registBroadcast();
		return view;
	}
	

	/**
	 * 对于设备连接状态的处理
	 * */
	public void setConnectState(int state)
	{
		switch (state) {
		case DeviceOperatorData.CONNECTSUCCESS://连接成功
			Editor editor = mainActivity.getSharedPreferences(macAddr, Context.MODE_PRIVATE).edit();
			editor.putString("mac", macList.get(selectIndex));
			editor.putString("name", etMac.getText().toString());
			editor.commit();
			tvState.setText(connectSuccess);
			btnConnect.setEnabled(false);
			isConnect = true;
			currentState = connectSuccess;
			break;
		case DeviceOperatorData.CONNECTFAIL://连接失败
			tvState.setText(connectFail);
			btnConnect.setEnabled(true);
			isConnect = false;
			currentState = connectFail;
			break;
		case DeviceOperatorData.CONNECTCANCEL://取消连接
			tvState.setText(connectCancel);
			btnConnect.setEnabled(true);
			isConnect = false;
			currentState = connectCancel; 
			break;
		case DeviceOperatorData.MESSAGE:
			break;
		case DeviceOperatorData.NODEVICE:
			break;
		default:
			tvState.setText(noCheck);
			btnConnect.setEnabled(true);
			isConnect = false;
			currentState = noCheck; 
			break;
		}
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.bt_conn_open:
			btnConnect.setEnabled(false);
			mainActivity.connectBluetooth( null );
			//mainActivity.connectBluetooth(etMac.getText().toString());//连接蓝牙 
			break;

		case R.id.btnMore:
			
			selectListDialog.setWidgetWidth(etMac.getWidth());
			selectListDialog.ShowDialog(etMac);
			break;
		default:
			break;
		}
	}
	
	/**
	 * 激活连接按钮 
	 */
	public void setConnect(boolean is)
	{
		if(btnConnect!=null){
			btnConnect.setEnabled(is);
		}
	}

	@Override
	public void setData(Object object) {
		// TODO Auto-generated method stub
		
	}
	/**
	 *连接方式选择按钮
	 */
	@Override
	public void onCheckedChanged(RadioGroup group, int checkedId) {
		// TODO Auto-generated method stub
		switch (checkedId) {
		case R.id.bt_conn:
			btnConnect.setEnabled(true);

			Log.e("Dev","bt type ");
			Log.e("Dev", "CommService.type =" + CommService.type );
			CommService.type = 1;//蓝牙连接方式
			
			break;
		case R.id.hid_conn:
			btnConnect.setEnabled(true);
			Log.e("Dev","hid type ");
			Log.e("Dev", "CommService.type =" + CommService.type );
			CommService.type = 2;//HID连接连接方式
			
			break;
		default:
			break;
		}
	}
	
	
	public class BluetoothReceiver extends BroadcastReceiver{
		private Context context;
		
		public BluetoothReceiver(Context context)
		{
			this.context = context;
		}
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			 String action = intent.getAction();        
			 //Log.e("tag"," action is "+action);
			 if (BluetoothDevice.ACTION_FOUND.equals(action)) {   
				 BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);  
				 // If it's already paired, skip it, because it's been listed already           
				 if (device.getBondState() != BluetoothDevice.BOND_BONDED) {                
				//	 mNewDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());  
					 //Log.e("tag","name is "+device.getName()+" addresss is "+device.getAddress());
					 if(!macList.contains(device.getAddress())){
						 macList.add(device.getAddress());
						 selectListDialog.addSelectObject(device.getName());
					 }
					
				 }else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {  
					
				 }       
		}
		
		
		
		}
		
		public void registBroadcast()
		{
			   IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
			   context.registerReceiver(this, filter);
			   // Register for broadcasts when discovery has finished
			   filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED); 
			   context.registerReceiver(this, filter); 
			   filter.setPriority(Integer.MAX_VALUE); 




		}
	 
		
		public void unregistBroadcast()
		{
			context.unregisterReceiver(this);
		}
	}


	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		bluetoothReceiver.unregistBroadcast();
	}


	@Override
	public void operatorMessage(int msg) {
		// TODO Auto-generated method stub
		selectIndex = msg;
		etMac.setText(bluetoothList.get(msg));
		selectListDialog.CloseDialog();
	}

	
}
