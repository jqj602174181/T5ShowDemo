package com.centerm.dispatch;

import java.util.Calendar;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemClock;
import android.util.Log;


/**
 * 在待机下能唤醒调度，保证其不进入睡眠
 *
 */
public class StandbyWakeUp extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		AlarmManager alarm = null;
		PendingIntent sendor = PendingIntent.getBroadcast(context, 0, intent, 0);
		if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
			//收到关闭屏幕的广播，AlarmManager执行定时任务
			/* 一般从灭屏开始5s会是系统进入睡眠 
			 */
			alarm = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
			long trigger = Calendar.getInstance().getTimeInMillis() + 1000;
			alarm.setRepeating(AlarmManager.RTC_WAKEUP, trigger, 1000, sendor);
//			AlarmManager.ELAPSED_REALTIME_WAKEUP;
		}else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
			//取消定时任务
			Log.i("user", "ACTION_SCREEN_ON");
			if (alarm != null) {
				alarm.cancel(sendor);
				alarm = null;
			}
		}
		
	}
	
	
}
