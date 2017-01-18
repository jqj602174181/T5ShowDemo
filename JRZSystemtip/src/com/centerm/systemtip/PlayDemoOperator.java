package com.centerm.systemtip;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import android.app.Activity;
import android.media.MediaPlayer;
import android.util.Log;
import android.widget.VideoView;
//com.centerm.systemtip.PlayDemoOperator
import com.centerm.systemtip.common.PlaySoundClass;
public class PlayDemoOperator {
	protected VideoView videoView = null; // 视频视图
	protected PlaySoundClass playSound = null;
	protected int playType = 0;
	protected Activity activity;
	protected String[] showFile = {
			"/mnt/internal_sd/media/tip/IDCard.mp4",				//二代证
			"/mnt/internal_sd/media/tip/ContactICCard.mp4",			//接触式IC卡
			"/mnt/internal_sd/media/tip/ContactlessICCard.mp4",		//非接触式IC卡
			"/mnt/internal_sd/media/tip/MagneticCard.mp4",			//磁条卡
			"/mnt/internal_sd/media/tip/FingerPrint.mp4",			//指纹仪
	};
	
	protected String[] voiceFile = {
			"/mnt/internal_sd/media/sound/IDCard.mp3",				//二代证
			"/mnt/internal_sd/media/sound/ContactICCard.mp3",		//接触式IC卡
			"/mnt/internal_sd/media/sound/ContactlessICCard.mp3",	//非接触式IC卡
			"/mnt/internal_sd/media/sound/MagneticCard.mp3",		//磁条卡
			"/mnt/internal_sd/media/sound/FingerPrint.mp3",			//指纹仪
			
	};
	
	public PlayDemoOperator(Activity activity,VideoView videoView)
	{
		this.activity = activity;
		this.videoView = videoView;
		playType = activity.getIntent().getIntExtra("playtype", 0);
      // 注册视频播放结束时的监听器
		videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
		{
			@Override
			public void onCompletion(MediaPlayer mp)
			{
				playVideo(showFile[playType]);
			}
		});
		

		// 注册视频播放执行错误的监听器
		videoView.setOnErrorListener(new MediaPlayer.OnErrorListener()
		{
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra)
			{
				playVideo(showFile[playType]);// 播放下一个, TODO:发生错误时，应该告诉Controller，尝试采用未加速的播放器进行播放
				return true;// true表示直接播放下一个
			}
		});
	
		videoView.setFocusable(false);
	
		videoView.setFocusableInTouchMode(false);


		init();
	}
	
	
	
	protected void init()
	{
	

		 playSound = new PlaySoundClass(activity);

		 playSound.initKeyBeep(R.raw.beep);

		 playSound.playAudio(voiceFile[playType]);
		 


	    playVideo(showFile[playType]);

	}
	protected void playVideo(String file) {
		videoView.stopPlayback();		// 结束上一次的播放
		videoView.setVideoPath(file);
		videoView.start();
	}
	
	//释放相关资源
		public void freeResource()
		{
			//释放语音
			if (playSound != null)
			{
				playSound.releaseMediaPlayer();
			}
		}
		
		public static PlayDemoOperator initPlayDemoOperator(String className,Activity act,VideoView videoView)
		{
			Class<?> c = null;
			Constructor<?> constructor= null;
			PlayDemoOperator playDemoOperator = null;
			try {
				c = Class.forName(className);
				Log.e("voicd"," className is "+className);
				constructor = c.getConstructor(Activity.class,VideoView.class);
				playDemoOperator = (PlayDemoOperator) constructor.newInstance(act,videoView);
				
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				Log.e("voicd"," ClassNotFoundException is ");
				e.printStackTrace();
			}  catch (InstantiationException e) {
				Log.e("voicd"," InstantiationException is ");
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				Log.e("voicd"," IllegalAccessException is ");
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				
				// TODO Auto-generated catch block
				Log.e("voicd"," IllegalArgumentException is ");
			} catch (NoSuchMethodException e) {
				// TODO Auto-generated catch block
				Log.e("voicd"," NoSuchMethodException is ");
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				
				Log.e("voicd"," InvocationTargetException is "+e.getMessage());
				e.printStackTrace();
			}
			
			return playDemoOperator;
		
		}
}
