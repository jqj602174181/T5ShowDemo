package com.centerm.common;

import java.io.IOException;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;

public final class MediaPlayerClass
{
	
	private final static int AUDIO_STYLE = AudioManager.STREAM_MUSIC;

	public static MediaPlayer create(Context context, Uri uri)
	{
		try
		{
			AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			int soundlevel = audioManager.getStreamVolume(AUDIO_STYLE);
			if (soundlevel == 0)
			{
				return null;
			}
			MediaPlayer mp = new MediaPlayer();
			mp.setAudioStreamType(AUDIO_STYLE);
			mp.setDataSource(context, uri);
			mp.prepare();
			return mp;
		} 
		catch (IOException ex)
		{
			ex.printStackTrace();
		} 
		catch (IllegalArgumentException ex)
		{
			ex.printStackTrace();
		} 
		catch (SecurityException ex)
		{
			ex.printStackTrace();
		}
		
		return null;
	}

	public static MediaPlayer create(Context context, int resid)
	{
		try
		{
			AssetFileDescriptor afd = context.getResources().openRawResourceFd(resid);
			if (afd == null)
			{
				return null;
			}
			AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			int soundlevel = audioManager.getStreamVolume(AUDIO_STYLE);
			if (soundlevel == 0)
			{
				return null;
			}
			MediaPlayer mp = new MediaPlayer();
			mp.setAudioStreamType(AUDIO_STYLE);
			mp.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
			afd.close();
			mp.prepare();
			return mp;
		} 
		catch (IOException ex)
		{
			ex.printStackTrace();
		} 
		catch (IllegalArgumentException ex)
		{
			ex.printStackTrace();
		} 
		catch (SecurityException ex)
		{
			ex.printStackTrace();
		}
		return null;
	}

	// 释放多媒体资源
	public static void releaseMediaPlayer(MediaPlayer mp)
	{
		if ( mp != null)
		{
			mp.reset();
			mp.release();
			mp = null;
		}
	}

}
