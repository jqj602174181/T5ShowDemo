package com.centerm.dispatch.common;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.net.Uri;

import com.centerm.common.MediaPlayerClass;
import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;

public class PlaySoundClass
{
	public static int SYSTEM_VOLUME_MAX = 5;
	public static int BEEP_VOLUME_MAX = 7;
	public static int MEDIA_VOLUME_MAX = 15;
	
	private MediaPlayer startPlayer = null;
	private SoundPool sndPool = null;
	private AudioManager audioManager = null;
	private final Context mContext;
	private int sndid = -1;

	public PlaySoundClass()
	{
		mContext = DispatchApplication.getProcessContext().getApplicationContext();
		audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
		
		SYSTEM_VOLUME_MAX = getSoundMaxLevel(1);
		BEEP_VOLUME_MAX = getSoundMaxLevel(2);
		MEDIA_VOLUME_MAX = getSoundMaxLevel(3);
	}
	
	//音量设置
	public void setSoundLevel( int type, int level)
	{
		if (level < 0) {
			level = 0;
		}
		if (type == 1)
		{
			if (level > SYSTEM_VOLUME_MAX) {
				level = SYSTEM_VOLUME_MAX;
			}
			audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, level, AudioManager.FX_KEY_CLICK );
		}
		else if (type == 2)
		{
			if (level > BEEP_VOLUME_MAX) {
				level = BEEP_VOLUME_MAX;
			}
			audioManager.setStreamVolume(AudioManager.STREAM_ALARM, level, AudioManager.FX_KEY_CLICK );
		}
		else if (type == 3)
		{
			if (level > MEDIA_VOLUME_MAX) {
				level = MEDIA_VOLUME_MAX;
			}
			audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, level, AudioManager.FX_KEY_CLICK );
		}
	}
	
	//获取音量
	public int getSoundLevel(int type)
	{
		int soundlevel = 0;

		if (type == 1)
		{
			soundlevel = audioManager.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
		}
		else if (type == 2)
		{
			soundlevel = audioManager.getStreamVolume(AudioManager.STREAM_ALARM);
		}
		else if (type == 3)
		{
			soundlevel = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
		}
		
		return soundlevel;
	}
	
	//获取最大音量
	public int getSoundMaxLevel(int type)
	{
		int soundlevel = 0;

		if (type == 1)
		{
			soundlevel = audioManager.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL);
		}
		else if (type == 2)
		{
			soundlevel = audioManager.getStreamMaxVolume(AudioManager.STREAM_ALARM);
		}
		else if (type == 3)
		{
			soundlevel = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		}
		
		return soundlevel;
	}
	
	// 播放语音
	public void playAudio(String filename)
	{
		Uri uri = Uri.parse(filename);
		startPlayer = MediaPlayerClass.create(mContext, uri);
		if (startPlayer != null) {
			startPlayer.start();
		}
	}

	// 播放语音
	public void playAudio(int resid)
	{
		startPlayer = MediaPlayerClass.create(mContext, resid);
		if (startPlayer != null) {
			startPlayer.start();
		}
	}

	public void playKeyBeep()
	{
		
		int maxVol = audioManager.getStreamMaxVolume(AudioManager.STREAM_ALARM);
		int currentVol = audioManager.getStreamVolume(AudioManager.STREAM_ALARM);
		float fSoundVolume = (float) currentVol / (float) maxVol;
		
		if (sndPool != null)
		{
			sndPool.play(sndid, fSoundVolume, fSoundVolume, 1, 0, (float) 1.0);
		}
	}

	public void initKeyBeep(int resid)
	{
		sndPool = new SoundPool(10, AudioManager.STREAM_ALARM, 0);
		sndid = sndPool.load(mContext, resid, 1);
	}
	
	public void releaseKeyBeep()
	{
		if (sndPool != null)
		{
			sndPool.release();
			sndPool = null;
		}
	}

	// 释放多媒体资源
	public void releaseMediaPlayer()
	{
		MediaPlayerClass.releaseMediaPlayer(startPlayer);
		releaseKeyBeep();
	}

}
