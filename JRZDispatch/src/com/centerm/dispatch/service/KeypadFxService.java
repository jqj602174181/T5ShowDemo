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
	//���帡�����ڲ���
	private static RelativeLayout mFloatLayout;
	private static WindowManager.LayoutParams wmParams;
    //���������������ò��ֲ����Ķ���
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
		//��ȡWindowManagerImpl.CompatModeWrapper
		mWindowManager = (WindowManager)getApplication().getSystemService(getApplication().WINDOW_SERVICE);
		//����window type
		wmParams.type = LayoutParams.TYPE_SYSTEM_ALERT; 
		//����ͼƬ��ʽ��Ч��Ϊ����͸��
        wmParams.format = PixelFormat.RGBA_8888; 
        //���ø������ڲ��ɾ۽���ʵ�ֲ���������������������ɼ����ڵĲ�����
        //wmParams.flags = LayoutParams.FLAG_NOT_FOCUSABLE;
        wmParams.flags = LayoutParams.FLAG_NOT_TOUCHABLE;
        
        //������������ʾ��ͣ��λ��Ϊ����ö�
        wmParams.gravity = Gravity.LEFT | Gravity.TOP; 
        
        // ����Ļ���Ͻ�Ϊԭ�㣬����x��y��ʼֵ
        wmParams.x = 0;
        wmParams.y = 0;

        // �����������ڳ�������
        wmParams.width = 1;
        wmParams.height = 1;
        
        LayoutInflater inflater = LayoutInflater.from(getApplication());
        //��ȡ����������ͼ���ڲ���
        mFloatLayout = (RelativeLayout) inflater.inflate(R.layout.float_keypad, null);
        mFloatLayout.setFocusableInTouchMode(true);
        mFloatLayout.setOnKeyListener(mKeyListener);
        mWindowManager.addView(mFloatLayout, wmParams);
	}
	
	private static OnKeyListener mKeyListener = new OnKeyListener() {	
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			// ���͹㲥
			final String ACTION = KEY_ACTION;
			Intent intent = new Intent(); // ʵ����Intent
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
