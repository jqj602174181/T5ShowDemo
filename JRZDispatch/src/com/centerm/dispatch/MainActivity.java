package com.centerm.dispatch;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
public class MainActivity extends Activity {
	public static final String TAG = "Dispath";				//���ȳ����log��ǩ

	/*
	private static DispatchHandler dispatchHandler = null;	//�ַ��߳���Ϣ����
	private static HandlerThread dispatchThread = null;		//�ַ��߳�
	public static MsgListClass msgList = new MsgListClass();   //ָ�����
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        DispatchApplication dispatchApplication = (DispatchApplication)getApplication();
        dispatchApplication.startService();
     //   runApp("com.example.testnew", "com.example.testnew.MainActivity");
  		finish();	//����activity
    }
  //����Ӧ�ó���apk
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
