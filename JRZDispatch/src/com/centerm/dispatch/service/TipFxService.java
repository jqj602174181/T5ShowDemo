package com.centerm.dispatch.service;

import com.centerm.common.BlueToothClsUtils;
import com.centerm.common.RWIniClass;
import com.centerm.dispatch.R;
import com.centerm.shutdown.ShutdownDialog;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.PixelFormat;
import android.os.BatteryManager;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class TipFxService extends Service {
	private static final String TAG = "TipFxService";
	//定义浮动窗口布局
	private LinearLayout mFloatLayout;
	private WindowManager.LayoutParams wmParams;
    //创建浮动窗口设置布局参数的对象
	private WindowManager mWindowManager;
	
	private TipReceiver mTipReceiver = null;
	private String actionBatteryInfo = Intent.ACTION_BATTERY_CHANGED;
	private String actionBtState = BluetoothAdapter.ACTION_STATE_CHANGED;
	private String actionBtConnectState = BluetoothAdapter.ACTION_CONNECTION_STATE_CHANGED;
	private String actionBtBondState = BluetoothDevice.ACTION_BOND_STATE_CHANGED;
	private String actionBtACLConnected = BluetoothDevice.ACTION_ACL_CONNECTED;
	private String actionBtACLDisconnected = BluetoothDevice.ACTION_ACL_DISCONNECTED;
	private String actionBtACLDisconnectRequested = BluetoothDevice.ACTION_ACL_DISCONNECT_REQUESTED;
	
	private String actionBtPairingRequest = BluetoothDevice.ACTION_PAIRING_REQUEST;
	private String FACTORY_PATH = "/mnt/sdcard/config/factory.ini";
	
	//通用版本标志
	private boolean isT5Normal = true;
	
	
	
	private ImageView mBluetoothState = null;
	private ImageView mBatteryLevel = null;
	private TextView mBatteryValue = null;
	private int mBatteryLevelList[] = {
			R.drawable.battery0,R.drawable.battery1,R.drawable.battery2,
			R.drawable.battery3,R.drawable.battery4,R.drawable.battery5,
			R.drawable.battery6,R.drawable.battery7,R.drawable.battery8,R.drawable.battery9
	};
	
	@Override
	public void onCreate() {
		super.onCreate();
		//读取产品型号
		String deviceName = RWIniClass.ReadIniString(FACTORY_PATH, "Factory", "deviceName");
		if (deviceName!= null && deviceName.trim().equals("T5-A")){
			isT5Normal = false;
		}else{
			isT5Normal = true;
		}
		
		
		createFloatView();
		//动态注册广播
		mTipReceiver = new TipReceiver();
		IntentFilter filter = new IntentFilter();
		filter.addAction(actionBatteryInfo);
		filter.addAction(actionBtState);
		filter.addAction(actionBtConnectState);
		filter.addAction(actionBtBondState);
		filter.addAction(actionBtACLConnected);
		filter.addAction(actionBtACLDisconnected);
		filter.addAction(actionBtACLDisconnectRequested);
		
		//以下为T5-A的广播过滤
		if (!isT5Normal){
			filter.addAction(actionBtPairingRequest);
			filter.addAction(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
		}
		
		
		this.registerReceiver(mTipReceiver, filter);
		updateBluetoothState(BluetoothAdapter.STATE_ON);
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
		if (mTipReceiver != null) {
			this.unregisterReceiver(mTipReceiver);
			mTipReceiver = null;
		}
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
	@SuppressWarnings("static-access")
	private void createFloatView() {
		wmParams = new WindowManager.LayoutParams();
		//获取WindowManagerImpl.CompatModeWrapper
		mWindowManager = (WindowManager)getApplication().getSystemService(getApplication().WINDOW_SERVICE);
		//设置window type
		wmParams.type = LayoutParams.TYPE_SYSTEM_ALERT; 
		//设置图片格式，效果为背景透明
	    wmParams.format = PixelFormat.RGBA_8888; 
	    //设置浮动窗口不可聚焦（实现操作除浮动窗口外的其他可见窗口的操作）
	    wmParams.flags = LayoutParams.FLAG_NOT_FOCUSABLE;
	    //wmParams.flags = LayoutParams.FLAG_NOT_TOUCHABLE;
	    
	    //调整悬浮窗显示的停靠位置为左侧置顶
	    wmParams.gravity = Gravity.RIGHT | Gravity.TOP; 
	    
	    // 以屏幕左上角为原点，设置x、y初始值
	    // wmParams.x = 0;
	    // wmParams.y = 0;

	    // 设置悬浮窗口长宽数据
	    wmParams.width = 110;
	    wmParams.height = 52;
	    
	    LayoutInflater inflater = LayoutInflater.from(getApplication());
	    //获取浮动窗口视图所在布局
	    mFloatLayout = (LinearLayout) inflater.inflate(R.layout.float_tip, null);
	    initView();
	    mWindowManager.addView(mFloatLayout, wmParams);
	}
	
	private void initView() {
		mBluetoothState = (ImageView)mFloatLayout.findViewById(R.id.bluetooth_icon);
		mBatteryLevel = (ImageView)mFloatLayout.findViewById(R.id.battery_icon);
		mBatteryValue = (TextView)mFloatLayout.findViewById(R.id.battery_value);
	}
	
	//更新电池电量
	private void updateBatteryInfo(int status, int level, int scale) {
		int batteryPercent = (level * 100 ) / scale;
		level = batteryPercent / 10;
		if (status != 2) {
			if (level == 10) {
				mBatteryLevel.setImageResource(mBatteryLevelList[9]);
			}
			else {
				mBatteryLevel.setImageResource(mBatteryLevelList[level]);
			}
		}
		else {
			mBatteryLevel.setImageResource(R.drawable.charging);
		}
		mBatteryValue.setText(String.valueOf(batteryPercent)+"%");
	}
	
	//更新蓝牙开关状态
	private void updateBluetoothState(int status) {
		if (status == BluetoothAdapter.STATE_OFF) {		//蓝牙关闭
			mBluetoothState.setImageResource(R.drawable.btclose);
		}
		else if (status == BluetoothAdapter.STATE_ON) {	//蓝牙打开
			mBluetoothState.setImageResource(R.drawable.btopen);
		}
	}
	
	//更新蓝牙配对状态
	/*private void updateBluetoothBondState(int status) {
		//取消
		if (status == BluetoothDevice.BOND_NONE) {
			mBluetoothState.setImageResource(R.drawable.btopen);
		}
		//配对成功
		else if (status == BluetoothDevice.BOND_BONDED) {
			mBluetoothState.setImageResource(R.drawable.btconnected);
		}
		//正在配对
		else if (status == BluetoothDevice.BOND_BONDING) {
			Log.i(TAG, "Bluetooth bonding");
			//mBluetoothState.setImageResource(R.drawable.btconnect1);
		}
	}*/
	
	private class TipReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			//电量广播
			if (intent.getAction().equals(actionBatteryInfo)) {
				int level = intent.getIntExtra( BatteryManager.EXTRA_LEVEL, 0 );	//电量水平
				int scale = intent.getIntExtra( BatteryManager.EXTRA_SCALE, 100 );	//满电量的大小
				int status = intent.getIntExtra( BatteryManager.EXTRA_STATUS, BatteryManager.BATTERY_STATUS_UNKNOWN );
				updateBatteryInfo(status, level, scale);
			}
			//蓝牙状态广播
			else if (intent.getAction().equals(actionBtState)) {
				int btState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.STATE_OFF);
				Log.i(TAG, "State:" + String.valueOf(btState));
				updateBluetoothState(btState);
			}
			else if (intent.getAction().equals(actionBtConnectState)) {
				int btState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.STATE_CONNECTED);
				Log.i(TAG, "ConnectState:" + String.valueOf(btState));
			}
			//蓝牙配对状态广播
			else if (intent.getAction().equals(actionBtBondState)) {
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				int btState = device.getBondState();
				Log.i(TAG, "BondState:" + String.valueOf(btState));
				//updateBluetoothBondState(btState);
			}
			else if (intent.getAction().equals(actionBtACLConnected)) {
				Log.i(TAG, "actionBtACLConnected");
				mBluetoothState.setImageResource(R.drawable.btconnected);
			}
			else if (intent.getAction().equals(actionBtACLDisconnected)) {
				Log.i(TAG, "actionBtACLDisconnected");
				mBluetoothState.setImageResource(R.drawable.btopen);
			}
			else if (intent.getAction().equals(actionBtACLDisconnectRequested)) {
				Log.i(TAG, "actionBtACLDisconnectRequested");
			}
			else if (intent.getAction().equals(actionBtPairingRequest)) {
				
				if (isT5Normal){
					return;
				}
				
				BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
	            try
	            {
	            	int mType = intent.getIntExtra(BluetoothDevice.EXTRA_PAIRING_VARIANT, BluetoothDevice.ERROR);
	            	if (mType == BluetoothDevice.PAIRING_VARIANT_PASSKEY_CONFIRMATION && 
	            		BlueToothClsUtils.setPairingConfirmation(btDevice.getClass(), btDevice) == true) {
	            		showTip(context, "本设备配对完毕，请确认远程设备配对");
	            	}
	            	else {
	            		showTip(context, "配对失败，请重新配对");
	            	}
	            	abortBroadcast();
	            	context.sendBroadcast(new Intent("android.bluetooth.device.action.PAIRING_CANCEL"));
	            	
	            }
	            catch (Exception e)
	            {
	                // TODO Auto-generated catch block
	                e.printStackTrace();
	            }
			}
			else if (intent.getAction().equals(Intent.ACTION_CLOSE_SYSTEM_DIALOGS)) {
				
				if (isT5Normal){
					return;
				}
				
				Log.i("ACTION_USER_PRESENT", "reboot");
				ShutdownDialog shutdownDialog = new ShutdownDialog(context);
				shutdownDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
				shutdownDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
				shutdownDialog.show();
			}
		}
	}
	
	private void showTip(Context context, String tip) {
		Toast toast = Toast.makeText(context, tip, Toast.LENGTH_LONG); 
    	toast.setGravity(Gravity.CENTER, 0, 0);
    	toast.show(); 
	}
	
	
}
