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
	protected VideoView videoView = null; // ��Ƶ��ͼ
	protected PlaySoundClass playSound = null;
	protected int playType = 0;
	protected Activity activity;
	protected String[] showFile = {
			"/mnt/internal_sd/media/tip/IDCard.mp4",				//����֤
			"/mnt/internal_sd/media/tip/ContactICCard.mp4",			//�Ӵ�ʽIC��
			"/mnt/internal_sd/media/tip/ContactlessICCard.mp4",		//�ǽӴ�ʽIC��
			"/mnt/internal_sd/media/tip/MagneticCard.mp4",			//������
			"/mnt/internal_sd/media/tip/FingerPrint.mp4",			//ָ����
	};
	
	protected String[] voiceFile = {
			"/mnt/internal_sd/media/sound/IDCard.mp3",				//����֤
			"/mnt/internal_sd/media/sound/ContactICCard.mp3",		//�Ӵ�ʽIC��
			"/mnt/internal_sd/media/sound/ContactlessICCard.mp3",	//�ǽӴ�ʽIC��
			"/mnt/internal_sd/media/sound/MagneticCard.mp3",		//������
			"/mnt/internal_sd/media/sound/FingerPrint.mp3",			//ָ����
			
	};
	
	public PlayDemoOperator(Activity activity,VideoView videoView)
	{
		this.activity = activity;
		this.videoView = videoView;
		playType = activity.getIntent().getIntExtra("playtype", 0);
      // ע����Ƶ���Ž���ʱ�ļ�����
		videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
		{
			@Override
			public void onCompletion(MediaPlayer mp)
			{
				playVideo(showFile[playType]);
			}
		});
		

		// ע����Ƶ����ִ�д���ļ�����
		videoView.setOnErrorListener(new MediaPlayer.OnErrorListener()
		{
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra)
			{
				playVideo(showFile[playType]);// ������һ��, TODO:��������ʱ��Ӧ�ø���Controller�����Բ���δ���ٵĲ��������в���
				return true;// true��ʾֱ�Ӳ�����һ��
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
		videoView.stopPlayback();		// ������һ�εĲ���
		videoView.setVideoPath(file);
		videoView.start();
	}
	
	//�ͷ������Դ
		public void freeResource()
		{
			//�ͷ�����
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
