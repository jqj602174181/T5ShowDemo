package com.centerm.systemtip;

import android.os.Bundle;


public class AdUpdateTipActivity extends CommonRetActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		//加载广告升级提示布局
		setContentView(R.layout.activity_adupdate);
	}
	
	/**
	 * 重载timeout方法，该方法应该被子类重载
	 */
	public void timeOut()
	{
	}
	
	@Override
	protected void onDestroy() 
	{
		freeResource();
		super.onDestroy();
	}

	//释放相关资源
	public void freeResource()
	{
		
	}
}
