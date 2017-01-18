package com.centerm.dispatch;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
public class MainActivity extends Activity {
	public static final String TAG = "Dispath";				//调度程序的log标签

	/*
	private static DispatchHandler dispatchHandler = null;	//分发线程消息处理
	private static HandlerThread dispatchThread = null;		//分发线程
	public static MsgListClass msgList = new MsgListClass();   //指令队列
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        DispatchApplication dispatchApplication = (DispatchApplication)getApplication();
        dispatchApplication.startService();
     //   runApp("com.example.testnew", "com.example.testnew.MainActivity");
  		finish();	//结束activity
    }
  //启动应用程序apk
  		private void runApp( String pkg, String name )
  		{
  			try
  			{
  				ComponentName appComponent = new ComponentName(pkg, name);
  				Intent intent = new Intent(Intent.ACTION_MAIN);
  				intent.addCategory(Intent.CATEGORY_LAUNCHER);
  				intent.setComponent(appComponent);
  				intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
  				startActivity(intent);
  			} 
  			catch (Exception e)
  			{
  				Log.e("", "Can't not start " + pkg);
  			}
  		}
  		
  		
  		@Override
  		public boolean onKeyUp(int keyCode, KeyEvent event) {
  			// TODO Auto-generated method stub
  			return super.onKeyUp(keyCode, event);
  		}
}
