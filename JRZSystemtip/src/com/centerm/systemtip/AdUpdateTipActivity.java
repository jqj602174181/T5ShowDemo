package com.centerm.systemtip;

import android.os.Bundle;


public class AdUpdateTipActivity extends CommonRetActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		//���ع��������ʾ����
		setContentView(R.layout.activity_adupdate);
	}
	
	/**
	 * ����timeout�������÷���Ӧ�ñ���������
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

	//�ͷ������Դ
	public void freeResource()
	{
		
	}
}
