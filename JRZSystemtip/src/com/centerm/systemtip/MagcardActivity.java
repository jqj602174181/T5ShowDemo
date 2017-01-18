package com.centerm.systemtip;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.widget.VideoView;

import com.centerm.systemtip.common.PlaySoundClass;
import com.centerm.util.financial.DevManager;
import com.centerm.util.financial.MesDefUtil;
import com.centerm.util.financial.MsgCardService;
import com.centerm.util.financial.MsgCardService.OnReadCardListener;

public class MagcardActivity extends CommonActivity {

	private final static String TAG = "MagcardActivity";
	private VideoView videoView = null; // 视频视图
	private int mTimeout = 0; //1.超时时间

	private static final String showFile = "/mnt/internal_sd/media/tip/MagneticCard.mp4";
	private static final String voiceFile = "/mnt/internal_sd/media/sound/MagneticCard.mp3";


	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_playdemo);

		Intent intent = getIntent();
		if(intent != null){
			String timeout = intent.getStringExtra("timeout");
			mTimeout = Integer.parseInt(timeout);
			Log.e(TAG, "mTimeout:" + mTimeout);
		}

		videoView = (VideoView) findViewById(R.id.videoView);
		// 注册视频播放结束时的监听器
		videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
		{
			@Override
			public void onCompletion(MediaPlayer mp)
			{
				playVideo(showFile);
			}
		});

		// 注册视频播放执行错误的监听器
		videoView.setOnErrorListener(new MediaPlayer.OnErrorListener()
		{
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra)
			{
				playVideo(showFile);// 播放下一个,发生错误时，应该告诉Controller，尝试采用未加速的播放器进行播放
				return true;// true表示直接播放下一个
			}
		});
		videoView.setFocusable(false);
		videoView.setFocusableInTouchMode(false);

		// 加载提示音
		Intent mIntent = new Intent(MagcardActivity.this, TipSoundService.class);
		mIntent.putExtra("type", "file");
		mIntent.putExtra("FilePath", voiceFile);
		MagcardActivity.this.startService(mIntent);
		playVideo(showFile);

		DevManager.enableCKB();
	}
	
	@Override
	protected void onStart() {
		super.onStart();
		getBookAcct();
	}

	public void getBookAcct(){
		MsgCardService service = MsgCardService.getInstance();
		OnReadCardListener readListener = new OnReadCardListener() {
			public void onComplete(byte[] byTrack1, byte[] byTrack2, byte[] byTrack3){
				Map<String, String> msgMap = new HashMap<String, String>();
				String track2 = new String(byTrack2).trim();
				msgMap.put("data", track2);
				msgMap.put("track3", new String(byTrack3).trim());
				Log.i(TAG, track2);
				JSONObject msgJsonObject = new JSONObject(msgMap);
				byte[] bRetData = msgJsonObject.toString().getBytes();
				sendResultToDispatch(bRetData);

				MagcardActivity.this.finish();
			}

			public void onError(String nErrCode, String strMsg){
				Map<String, String> errorMsgMap = new HashMap<String, String>();
				errorMsgMap.put("errorcode", nErrCode);
				errorMsgMap.put("errormsg", strMsg);
				JSONObject errorMsgJsonObject = new JSONObject(errorMsgMap);
				byte[] bRetData = errorMsgJsonObject.toString().getBytes();
				sendResultToDispatch(bRetData);

				MagcardActivity.this.finish();
			}

			public void onCancel() {
			}
		};

		service.setComParams(MesDefUtil.MAGNETICCARDPORT, MesDefUtil.MAGNETICCARDBAUD);
		service.setTimeout(mTimeout);
		service.getBookAcct(readListener);
	}

	private void playVideo(String file) {
		videoView.stopPlayback();		// 结束上一次的播放
		videoView.setVideoPath(file);
		videoView.start();
	}
}
