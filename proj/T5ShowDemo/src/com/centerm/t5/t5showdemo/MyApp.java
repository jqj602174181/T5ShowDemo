package com.centerm.t5.t5showdemo;

import java.util.ArrayList;
import java.util.List;

import com.centerm.t5.socketclient.PinYin;
import com.centerm.t5.socketserver.SocketServer;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;

public class MyApp extends Application {

	private static MyApp instance;

	public ArrayList<MyPushActivity> list = new ArrayList<MyPushActivity>();

	public static MyApp getInstance(){
		return instance;
	}

	private Handler handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);

			for(MyPushActivity activity : list){ //确保只有一个推送界面
				activity.finish();
			}

			String path = (String)msg.obj;
			Intent intent = new Intent(MyApp.this, MyPushActivity.class);
			intent.putExtra("path", path);
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			MyApp.this.startActivity(intent);
		}
	};

	@Override
	public void onCreate() {
		super.onCreate();

		instance = this;

		//拼音 初始化
		new Thread(){
			public void run() {
				PinYin.getPinYin( "中" );
			};
		}.start();

		//启动服务端
		SocketServer.getInstance().setHandler(handler);
		SocketServer.getInstance().start();
	}


	public ComponentName getComponentName(){
		ComponentName component = null;
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningTaskInfo> runningTasks = am.getRunningTasks(1);
		RunningTaskInfo rti = runningTasks.get(0);
		component = rti.topActivity;
		return component;
	}

}
