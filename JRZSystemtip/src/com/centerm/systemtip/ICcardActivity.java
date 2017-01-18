package com.centerm.systemtip;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.widget.VideoView;

import com.centerm.systemtip.common.PlaySoundClass;
import com.centerm.util.financial.DevManager;
import com.centerm.util.financial.ICCardService;
import com.centerm.util.financial.ICCardService.OnReadCardListener;

public class ICcardActivity extends CommonActivity {

	private final static String TAG = "ICcardActivity";
	private VideoView videoView = null; // 视频视图
	private int mTimeout = 0; //1.超时时间
	private int iIcFlag = 1; //1接触 2非接

	private static final String showFile1 = "/mnt/internal_sd/media/tip/ContactICCard.mp4";
	private static final String voiceFile1 = "/mnt/internal_sd/media/sound/ContactICCard.mp3";

	private static final String showFile2 = "/mnt/internal_sd/media/tip/ContactlessICCard.mp4";
	private static final String voiceFile2 = "/mnt/internal_sd/media/sound/ContactlessICCard.mp3";


	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_playdemo);

		Intent intent = getIntent();
		if(intent != null){
			String timeout = intent.getStringExtra("timeout");
			mTimeout = Integer.parseInt(timeout);
			String flag = intent.getStringExtra("flag");
			iIcFlag = Integer.parseInt(flag);
			Log.e(TAG, "mTimeout:" + mTimeout);
			Log.e(TAG, "iIcFlag:" + iIcFlag);
		}

		videoView = (VideoView) findViewById(R.id.videoView);
		// 注册视频播放结束时的监听器
		videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
		{
			@Override
			public void onCompletion(MediaPlayer mp)
			{
				if(iIcFlag == 1){
					playVideo(showFile1);
				}else if(iIcFlag == 2){
					playVideo(showFile2);
				}
			}
		});

		// 注册视频播放执行错误的监听器
		videoView.setOnErrorListener(new MediaPlayer.OnErrorListener()
		{
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra)
			{
				if(iIcFlag == 1){
					playVideo(showFile1);
				}else if(iIcFlag == 2){
					playVideo(showFile2);
				}
				return true;// true表示直接播放下一个
			}
		});
		videoView.setFocusable(false);
		videoView.setFocusableInTouchMode(false);

		// 加载提示音
		String voiceFile = null;
		if(iIcFlag == 1){
			voiceFile = voiceFile1;
			playVideo(showFile1);
		}else if(iIcFlag == 2){
			voiceFile = voiceFile2;
			playVideo(showFile2);
		}

		Intent mIntent = new Intent(ICcardActivity.this, TipSoundService.class);
		mIntent.putExtra("type", "file");
		mIntent.putExtra("FilePath", voiceFile);
		ICcardActivity.this.startService(mIntent);

		DevManager.enableCKB();
	}

	@Override
	protected void onStart() {
		super.onStart();
		getICCardInfo();
	}

	private void getICCardInfo()
	{
		ICCardService service = ICCardService.getInstance();
		OnReadCardListener readListener = new OnReadCardListener() {
			public void onComplete(){
				Map<String, String> msgMap = new HashMap<String, String>();
				msgMap.put("data", "");
				JSONObject msgJsonObject = new JSONObject(msgMap);
				byte[] bRetdata = msgJsonObject.toString().getBytes();
				sendResultToDispatch(bRetdata);

				ICcardActivity.this.finish();
			}

			public void onComplete(byte[] msg,int iIcFlag) {
				Log.i(TAG,"解析的IC卡数据内容 ： " + new String(msg));
				Map<String, String> msgMap = new HashMap<String, String>();
				String data = new String(msg);
				msgMap.put("type",String.valueOf(iIcFlag));
				if (msg != null && msg.length > 0) {
					msgMap.put("data", data.trim().substring(4));
				} else {
					msgMap.put("data", "");
				}
				JSONObject msgJsonObject = new JSONObject(msgMap);
				byte[] bRetdata = msgJsonObject.toString().getBytes();
				sendResultToDispatch(bRetdata);

				ICcardActivity.this.finish();
			}

			public void onError(String nErrCode, String strMsg){
				Map<String, String> errorMsgMap = new HashMap<String, String>();
				errorMsgMap.put("errorcode", nErrCode);
				errorMsgMap.put("errormsg", strMsg);
				JSONObject errorMsgJsonObject = new JSONObject(errorMsgMap);
				byte[] bRetdata = errorMsgJsonObject.toString().getBytes();
				sendResultToDispatch(bRetdata);

				ICcardActivity.this.finish();
			}

			public void onCancel() {
			}

			@Override
			public void onComplete(byte[] msgOne, byte[] msgTwo, int iIcFlag) {

			}
		};
		service.setiIcFlag(iIcFlag);
		service.setAryTagList("A");
		service.setStrAIDList("A000000333");
		service.setTimeout(mTimeout);
		service.getICCardInfo(readListener);
	}

	private void playVideo(String file) {
		videoView.stopPlayback();		// 结束上一次的播放
		videoView.setVideoPath(file);
		videoView.start();
	}
}
