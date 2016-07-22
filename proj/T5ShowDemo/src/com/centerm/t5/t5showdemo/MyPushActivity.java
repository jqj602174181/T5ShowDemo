package com.centerm.t5.t5showdemo;

import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class MyPushActivity extends Activity {

	private ImageView image;
	private TextView text;
	private Button close;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		this.setContentView(R.layout.mypush);

		this.setFinishOnTouchOutside(false);

		Intent intent = getIntent();
		String path = null;
		String content = null;
		if(intent != null){
			path = intent.getStringExtra("path");
			content = intent.getStringExtra("content");
		}

		image = (ImageView)findViewById(R.id.image);
		text = (TextView)findViewById(R.id.text);
		close = (Button)findViewById(R.id.close);
		close.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				MyPushActivity.this.finish();
			}
		});

		init(path, content);

		MyApp.getInstance().list.add(this);
	}

	private void init(String path, String content) {
		image.setImageDrawable(Drawable.createFromPath(path));
		text.setText(content);
	}
}
