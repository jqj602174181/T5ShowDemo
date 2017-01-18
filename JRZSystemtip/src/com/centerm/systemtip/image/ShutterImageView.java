package com.centerm.systemtip.image;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.util.AttributeSet;
import android.view.animation.Animation.AnimationListener;
import android.widget.ImageView;

/**
 * ��Ҷ����
 */
public class ShutterImageView extends ImageView
{
	public static final int LEAF_COUNT = 16; // bitmap���ֵ�Ҷ�Ӹ���
	public static final int SUB_LEAF_COUNT = 12; // ��ʾ1��Ҷ�ӵ���Ҷ����
	public static final int INTERVAL_TIME = 800 / SUB_LEAF_COUNT; // �����ˢ����һ����ҳ

	public static final int VERTIAL = 0; // Ҷ����ˮƽ��
	public static final int HORIZENTAL = 1; // Ҷ���Ǵ�ֱ��

	int orientation = VERTIAL; // Ҷ�ӵķ���
	int mLeafCount = LEAF_COUNT;// Ҷ�ӵĸ���
	int mSubLeafCount = SUB_LEAF_COUNT;// ��Ҷ�ĸ���
	int step = 0; // ��Ҷ�Ĳ���
	
	private boolean bNoticeEnd = false; // ֪ͨ��������
	private AnimationListener mListener;
	
	public ShutterImageView(Context context)
	{
		super(context);
	}

	public ShutterImageView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}

	public ShutterImageView(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);
	}
	
	/**
	 * �ṩ�ӿڸ�container���������ö���������֪ͨ
	 * @param listener ��������������
	 */
	public void setAnimationListener(AnimationListener listener)
	{
		bNoticeEnd = true;
		mListener = listener;
	}
	
	/**
	 * ����Ҷ�ӵķ���
	 * 
	 * @param orientation
	 *            ����
	 */
	public void setOrientation(int orientation)
	{
		this.orientation = orientation;
		if (orientation == HORIZENTAL)
		{
			mLeafCount /= 2;
		}
	}

	/**
	 * ������һ����ҳ
	 */
	public void showNextLeaf()
	{
		step++;
		postInvalidateDelayed(INTERVAL_TIME);
	}

	@Override
	protected void onDraw(Canvas canvas)
	{
		// ��ȡ��ͼ����
		BitmapDrawable drawable = (BitmapDrawable) getDrawable();
		if (drawable == null)
		{
			System.out.println("Shutter Image view onDraw error!");
			return;
		}
		
		// ��ʾ��N����Ҷ
		Bitmap bitmap = drawable.getBitmap(); // ��ȡԭʼ��bitmap
		int width = drawable.getIntrinsicWidth(); // ͼ����
		int height = drawable.getIntrinsicHeight(); // ͼ��߶�
		int leafWidth = width / mLeafCount; // Ҷ�ӵĿ��
		int leafHeight = height / mLeafCount; // Ҷ�ӵĿ��
		int xOffset = (getWidth() - width) / 2; // ˮƽƫ�ƴ�С
		int yOffset = (getHeight() - height) / 2; // y��ƫ�ƴ�С

		// ������Ƶľ�������
		Rect srcRect = new Rect(0, 0, width, height);
		Rect desRect = new Rect(xOffset, yOffset, xOffset + width, yOffset
				+ height);

		// ͼ��̫С�ˣ���ֱ����ʾ
		if( (width < mLeafCount && orientation == VERTIAL) || (height < mLeafCount
				&& orientation == HORIZENTAL))
		{
			canvas.drawBitmap(bitmap, srcRect, desRect, null);
			return;
		}

		// ��Ȼ�߶�̫С���Ͳ���Ҫ��ô�ಽ������ʾ��
		if (width < mLeafCount * mSubLeafCount && orientation == VERTIAL)
		{
			mSubLeafCount = leafWidth;
		}
		if (height < mLeafCount * mSubLeafCount && orientation == HORIZENTAL)
		{
			mSubLeafCount = leafHeight;
		}
		if (mSubLeafCount == 0)
		{
			mSubLeafCount = 1;
		}

		// ���Ƹ���Ҷ
		for (int i = 0; i < mLeafCount; i++)
		{
			// ������Ҫˢ�µ���Ҷ�ľ���
			if (orientation == VERTIAL)// ��ֱ
			{
				srcRect.left = i * leafWidth;
				srcRect.right = srcRect.left + step * leafWidth / mSubLeafCount;

				desRect.left = xOffset + srcRect.left;
				desRect.right = xOffset + srcRect.right;
			} else
			// ˮƽ
			{
				srcRect.top = i * leafHeight;
				srcRect.bottom = srcRect.top + step * leafHeight
						/ mSubLeafCount;

				desRect.top = yOffset + srcRect.top;
				desRect.bottom = yOffset + srcRect.bottom;
			}
			canvas.drawBitmap(bitmap, srcRect, desRect, null);
		}// end of for
	
		// δ�������ǰ����׼����һ�λ���
		if (step < mSubLeafCount)
		{
			showNextLeaf();
		}
		
		if(step >= mSubLeafCount && bNoticeEnd)
		{
			mListener.onAnimationEnd(null);
			bNoticeEnd = false;
		}
	}// end of onDraw
}
