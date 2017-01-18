package com.centerm.dispatch.common;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;

/**
 * @class: StartAppsClass
 * @function: 启动应用程序类
 * @author: zxx
 * 
 * @data:2013-12-26 16:21
 */
public final class StartAppsClass
{
	
	/**
	 * @name: startPQC
	 * @function: 启动PQC程序
	 * @param void
	 * @return void
	 */
	public static void startPQC()
	{
		String pkg = "com.centerm.pqctest";
		String name = "com.centerm.pqctest.MainActivity";
		
		try
		{
			ComponentName appComponent = new ComponentName(pkg, name);
			Intent intent = new Intent(Intent.ACTION_MAIN);
			intent.addCategory(Intent.CATEGORY_LAUNCHER);
			intent.setComponent(appComponent);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			DispatchApplication.getProcessContext().startActivity(intent);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			Log.e(MainActivity.TAG, "Can't not start pqctest!");
		}
	}
}
