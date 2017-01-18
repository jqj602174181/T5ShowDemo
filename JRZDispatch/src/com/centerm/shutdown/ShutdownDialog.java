package com.centerm.shutdown;

import java.io.IOException;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

import com.centerm.dispatch.R;

public class ShutdownDialog extends Dialog {

	private TextView tvShutdown;
	private Handler mhandler = null;
	private DelayRunable delayRunable;
	private int delayMillis = 1000;
	private int allTime = 3;
	
	public ShutdownDialog(Context context) {
		super(context);	
	}

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.shutdown_dialog);
		tvShutdown = (TextView) findViewById(R.id.tv_shutdown);
		tvShutdown.setText(allTime + "秒后自动关机！");
		mhandler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				tvShutdown.setText(allTime + "秒后自动关机！");
				super.handleMessage(msg);
			}
		};
		delayRunable = new DelayRunable();
		setCancelable(false);
	}

	public void show() {
		super.show();
		allTime = 3;
		mhandler.postDelayed(delayRunable, delayMillis);
	}

	public void setText(String text) {
		tvShutdown.setText(text);
	}

	private class DelayRunable implements Runnable {
		

		@Override
		public void run() {

			allTime--;
			if (allTime == 0) {
				mhandler.removeCallbacks(this);
				try {
					Runtime.getRuntime().exec("reboot -p");
				} catch (IOException e) {
					e.printStackTrace();
				}
			} else {
				mhandler.sendEmptyMessage(0);
				mhandler.postDelayed(this, delayMillis);// 倒计时
				
			}

		}
	}
}
