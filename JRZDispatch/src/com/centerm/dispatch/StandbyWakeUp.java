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
 * �ڴ������ܻ��ѵ��ȣ���֤�䲻����˯��
 *
 */
public class StandbyWakeUp extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		AlarmManager alarm = null;
		PendingIntent sendor = PendingIntent.getBroadcast(context, 0, intent, 0);
		if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
			//�յ��ر���Ļ�Ĺ㲥��AlarmManagerִ�ж�ʱ����
			/* һ���������ʼ5s����ϵͳ����˯�� 
			 */
			alarm = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
			long trigger = Calendar.getInstance().getTimeInMillis() + 1000;
			alarm.setRepeating(AlarmManager.RTC_WAKEUP, trigger, 1000, sendor);
//			AlarmManager.ELAPSED_REALTIME_WAKEUP;
		}else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
			//ȡ����ʱ����
			Log.i("user", "ACTION_SCREEN_ON");
			if (alarm != null) {
				alarm.cancel(sendor);
				alarm = null;
			}
		}
		
	}
	
	
}
