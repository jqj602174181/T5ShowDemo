package com.centerm.systemtip;

import com.centerm.systemtip.common.PlaySoundClass;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.widget.VideoView;

public class PlayDemoActivity extends CommonRetActivity {
	
	private VideoView videoView = null; // 视频视图

	private PlayDemoOperator playDemoOperator;
	
	public static String playDemoOperatorName = "com.centerm.systemtip.PlayDemoOperator";
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_playdemo);
		videoView = (VideoView) findViewById(R.id.videoView);
		playDemoOperator = PlayDemoOperator.initPlayDemoOperator(playDemoOperatorName, this, videoView);
		Log.e("voice"," playDemoOperator is "+playDemoOperator);
	}
	
	
	

	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}





	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
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
		playDemoOperator.freeResource();
	}
}
