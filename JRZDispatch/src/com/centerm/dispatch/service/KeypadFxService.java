package com.centerm.dispatch.service;

import android.app.Service;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.RelativeLayout;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.R;

public class KeypadFxService extends Service {
	private static final String TAG = "KeypadFxService";
	private static final String KEY_ACTION = "com.centerm.dispatch.KEY_EVENT";
	private static final int KEY_ACTION_DOWN = 0;
	private static final int KEY_ACTION_UP = 1;
	//定义浮动窗口布局
	private static RelativeLayout mFloatLayout;
	private static WindowManager.LayoutParams wmParams;
    //创建浮动窗口设置布局参数的对象
	private static WindowManager mWindowManager;
	
	@Override
	public void onCreate() {
		Log.i(TAG, "oncreat");
		createFloatView();
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
        //wmParams.flags = LayoutParams.FLAG_NOT_FOCUSABLE;
        wmParams.flags = LayoutParams.FLAG_NOT_TOUCHABLE;
        
        //调整悬浮窗显示的停靠位置为左侧置顶
        wmParams.gravity = Gravity.LEFT | Gravity.TOP; 
        
        // 以屏幕左上角为原点，设置x、y初始值
        wmParams.x = 0;
        wmParams.y = 0;

        // 设置悬浮窗口长宽数据
        wmParams.width = 1;
        wmParams.height = 1;
        
        LayoutInflater inflater = LayoutInflater.from(getApplication());
        //获取浮动窗口视图所在布局
        mFloatLayout = (RelativeLayout) inflater.inflate(R.layout.float_keypad, null);
        mFloatLayout.setFocusableInTouchMode(true);
        mFloatLayout.setOnKeyListener(mKeyListener);
        mWindowManager.addView(mFloatLayout, wmParams);
	}
	
	private static OnKeyListener mKeyListener = new OnKeyListener() {	
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			// 发送广播
			final String ACTION = KEY_ACTION;
			Intent intent = new Intent(); // 实例化Intent
			intent.setAction(ACTION);
			intent.putExtra("keycode", keyCode);
			
			if (event.getAction() == KeyEvent.ACTION_UP) {
				intent.putExtra("action", KEY_ACTION_UP);
			}
			else if (event.getAction() == KeyEvent.ACTION_DOWN) {
				intent.putExtra("action", KEY_ACTION_DOWN);
			}
			DispatchApplication.getProcessContext().sendBroadcast(intent);
			return true;
		}
	};
}
