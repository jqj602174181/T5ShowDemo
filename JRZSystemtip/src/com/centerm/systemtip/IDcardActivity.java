package com.centerm.systemtip;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import com.centerm.systemtip.common.PlaySoundClass;
import com.centerm.systemtip.common.SendMsgClass;
import com.centerm.systemtip.ipc.MessageType;
import com.centerm.util.StringUtil;
import com.centerm.util.financial.DevManager;
import com.centerm.util.financial.IDCardService;
import com.centerm.util.financial.MesDefUtil;
import com.centerm.util.financial.PersonInfo;
import com.centerm.util.financial.IDCardService.OnReadCardListener;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.widget.VideoView;

public class IDcardActivity extends CommonActivity {

	private final static String TAG = "IDcardActivity";
	private VideoView videoView = null; // 视频视图
	private int mTimeout = 0; //1.超时时间

	private static final String showFile = "/mnt/internal_sd/media/tip/IDCard.mp4";
	private static final String voiceFile = "/mnt/internal_sd/media/sound/IDCard.mp3";

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
		Intent mIntent = new Intent(IDcardActivity.this, TipSoundService.class);
		mIntent.putExtra("type", "file");
		mIntent.putExtra("FilePath", voiceFile);
		IDcardActivity.this.startService(mIntent);
		
		playVideo(showFile);

		DevManager.enableCKB();
	}

	@Override
	protected void onStart() {
		super.onStart();
		readFullIdCard();
	}

	//	public void readFullIdCard()
	//	{
	//		final long time1 = System.currentTimeMillis();
	//
	//		IDCardService service = IDCardService.getInstance();
	//		OnReadCardListener readListener = new OnReadCardListener() {
	//			public void onComplete(PersonInfo perInfo){
	//				long time2 = System.currentTimeMillis();
	//				Log.e("IdCard", "二代证读取时间：" + (time2-time1));
	//
	//				Log.e("IdCard", "数据处理开始...");
	//
	//				StringBuffer buffer = new StringBuffer();
	//				buffer.append(new String(perInfo.name).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.sex).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.nation).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.cardId).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.birthday).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.address).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.police).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.validStart).trim());
	//				buffer.append(",");
	//				buffer.append(new String(perInfo.validEnd).trim());
	//				buffer.append(",");
	//
	//				try {
	//					Bitmap bitmap = BitmapFactory.decodeByteArray(perInfo.photo, 0, perInfo.photo.length);
	//					ByteArrayOutputStream imageByteArray = new ByteArrayOutputStream();
	//					bitmap.compress(Bitmap.CompressFormat.JPEG, 50, imageByteArray);
	//					byte[] imageData = imageByteArray.toByteArray();
	//
	//					FileOutputStream fileOutputStream = new FileOutputStream("/mnt/internal_sd/shenfengzheng.jpeg");
	//					fileOutputStream.write(imageData);
	//					fileOutputStream.close();
	//					Log.e("IdCard", "jpg图片大小：" + imageData.length);
	//
	//					String value = StringUtil.bytesToHexString(imageData);
	//					buffer.append(value);
	//
	//					byte[] object = buffer.toString().getBytes();
	//					Log.e("IdCard", "要发送总的数据长度:" + object.length);
	//
	//					int outResLength = object.length;
	//					byte[] temp = IntToByte(outResLength);
	//					object = bytes2And(temp, object);
	//					long time5 = System.currentTimeMillis();
	//					Log.e("IdCard", "数据组装时间：" + (time5-time2));
	//					Log.e("IdCard", "数据处理结束...");
	//
	//					SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_SEND_DATA, MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, object);
	//				} catch (IOException e) {
	//					e.printStackTrace();
	//				}
	//
	//				IDcardActivity.this.finish();
	//			}
	//
	//			public void onError(String nErrCode, String strMsg){
	//				Map<String, String> errorMsgMap = new HashMap<String, String>();
	//				errorMsgMap.put("errorcode", nErrCode);
	//				errorMsgMap.put("errormsg", strMsg);
	//				JSONObject errorMsgJsonObject = new JSONObject(errorMsgMap);
	//				byte[] data = errorMsgJsonObject.toString().getBytes();
	//				SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_SEND_DATA, MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, data);
	//
	//				IDcardActivity.this.finish();
	//			}
	//
	//			public void onCancel() {
	//			}
	//		};
	//
	//		service.setTimeOut(mTimeout);
	//		service.setComParams(MesDefUtil.IDCARDPORT, 115200);
	//		service.readCard(readListener);
	//	}

	private byte[] IntToByte(int length)
	{
		byte[] byteArray = new byte[4];
		byteArray[0] = (byte)(length >>> 24);
		byteArray[1] = (byte)(length >>> 16);
		byteArray[2] = (byte)(length >>> 8);
		byteArray[3] = (byte)(length);
		return byteArray;
	}

	private byte[] bytes2And(byte[] left, byte[] right) {
		byte[] result = new byte[left.length + right.length];
		for (int i = 0; i < left.length; i++) {
			result[i] = left[i];
		}

		for (int i = 0; i < right.length; i++) {
			result[i + left.length] = right[i];
		}

		return result;
	}

	public void readFullIdCard()
	{
		final long time1 = System.currentTimeMillis();

		IDCardService service = IDCardService.getInstance();
		OnReadCardListener readListener = new OnReadCardListener() {
			public void onComplete(PersonInfo perInfo){
				long time2 = System.currentTimeMillis();
				Log.e("IdCard", "二代证读取时间：" + (time2-time1));

				Log.e("IdCard", "数据处理开始...");

				Map<String, String> personInfoMap = new HashMap<String, String>();
				personInfoMap.put("name", new String(perInfo.name).trim());
				personInfoMap.put("sex", new String(perInfo.sex).trim());
				personInfoMap.put("nation", new String(perInfo.nation).trim());
				personInfoMap.put("num", new String(perInfo.cardId).trim());
				personInfoMap.put("birthday", new String(perInfo.birthday).trim());
				personInfoMap.put("address", new String(perInfo.address).trim());
				personInfoMap.put("issue", new String(perInfo.police).trim());
				personInfoMap.put("validstart", new String(perInfo.validStart).trim());
				personInfoMap.put("validend", new String(perInfo.validEnd).trim());
				//				personInfoMap.put("photo", StringUtil.bytesToHexString(perInfo.photo));

				try {
					Bitmap bitmap = BitmapFactory.decodeByteArray(perInfo.photo, 0, perInfo.photo.length);
					ByteArrayOutputStream imageByteArray = new ByteArrayOutputStream();
					bitmap.compress(Bitmap.CompressFormat.JPEG, 50, imageByteArray);
					byte[] imageData = imageByteArray.toByteArray();

					FileOutputStream fileOutputStream = new FileOutputStream("/mnt/internal_sd/shenfengzheng.jpeg");
					fileOutputStream.write(imageData);
					fileOutputStream.close();
					Log.e("IdCard", "jpg图片大小：" + imageData.length);

					personInfoMap.put("photo", StringUtil.bytesToHexString(imageData));

					JSONObject personInfoJsonObject = new JSONObject(personInfoMap);
					byte[] object = personInfoJsonObject.toString().getBytes();
					int outResLength = object.length;
					byte[] temp = IntToByte(outResLength);
					object = bytes2And(temp, object);

					Log.e("IdCard", "要发送总的数据长度:" + object.length);
					long time5 = System.currentTimeMillis();
					Log.e("IdCard", "数据组装时间：" + (time5-time2));

					Log.e("IdCard", "数据处理结束...");

					SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_SEND_DATA, MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, object);
				} catch (IOException e) {
					e.printStackTrace();
				}

				IDcardActivity.this.finish();
			}

			public void onError(String nErrCode, String strMsg){
				Map<String, String> errorMsgMap = new HashMap<String, String>();
				errorMsgMap.put("errorcode", nErrCode);
				errorMsgMap.put("errormsg", strMsg);
				JSONObject errorMsgJsonObject = new JSONObject(errorMsgMap);
				byte[] data = errorMsgJsonObject.toString().getBytes();
				SendMsgClass.SendResultToDispatch(MessageType.MSG_NOTICE_SEND_DATA, MainActivity.ID, MainActivity.STATUS_PROGRAM_CLOSE, nLinkType, data);

				IDcardActivity.this.finish();
			}

			public void onCancel() {
			}
		};

		service.setTimeOut(mTimeout);
		service.setComParams(MesDefUtil.IDCARDPORT, 115200);
		service.readCard(readListener);
	}

	private void playVideo(String file) {
		videoView.stopPlayback();		// 结束上一次的播放
		videoView.setVideoPath(file);
		videoView.start();
	}
}
