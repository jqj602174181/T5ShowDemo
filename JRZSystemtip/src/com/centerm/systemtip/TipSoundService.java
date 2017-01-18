package com.centerm.systemtip;

import com.centerm.systemtip.common.PlaySoundClass;

import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;

public class TipSoundService extends Service {

	private static final String TAG = TipSoundService.class.getSimpleName();
	private PlaySoundClass playSound = null;
	private MediaPlayer mediaPlayer = null;

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onCreate() {
		super.onCreate();
		playSound = new PlaySoundClass(TipSoundService.this);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		System.out.println("onStartCommandÉùÒô·þÎñ");
		if(mediaPlayer != null){
			mediaPlayer.stop();
			mediaPlayer.release();
			mediaPlayer = null;
		}
		if(intent != null){
			String type = intent.getStringExtra("type");
			if(type != null && type.equals("file")){
				String filePath = intent.getStringExtra("FilePath");
				if(filePath != null){
					mediaPlayer = playSound.playAudio(filePath);
				}
			}
		}
		return super.onStartCommand(intent, flags, startId);
	}

}
