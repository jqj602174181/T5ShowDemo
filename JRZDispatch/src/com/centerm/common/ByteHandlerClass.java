package com.centerm.common;

public final class ByteHandlerClass 
{
	
	//两个字节数组内容比较，相同返回true, 不同返回false
	public static boolean cmpByteStartWith(byte[] bSrc, byte[] bCmpData)
	{
		if (bSrc == null || bCmpData == null)
		{
			return false;
		}
		
		//比较数据长度是否相同
		int nCmpDataLen = bCmpData.length;
		int nSrcLen = bSrc.length;
		
	    if ( nSrcLen != nCmpDataLen)
	    {
			return false;
		}
		
	    //比较数据内容
		for (int i = 0; i < nCmpDataLen; i++)
		{
			if (bSrc[i] != bCmpData[i])
			{
				return false;
			}
		}
		return true;
	}
	
	//两个字节数组内容头几个字节比较，相同返回true, 不同返回false
	public static boolean cmpByteHead(byte[] bSrc, byte[] bCmpData)
	{
		if (bSrc == null || bCmpData == null)
		{
			return false;
		}
		
		//比较数据长度是否相同
		int nCmpDataLen = bCmpData.length;
		int nSrcLen = bSrc.length;
		
	    if ( nSrcLen < nCmpDataLen)
	    {
			return false;
		}
		
	    //比较数据内容
		for (int i = 0; i < nCmpDataLen; i++)
		{
			if (bSrc[i] != bCmpData[i])
			{
				return false;
			}
		}
		return true;
	}
	
	//获取分隔符字符两边的数据
	public static byte[][] getByteArrayBeforeSeperator(byte[] bSrc, byte bSeperatorChar)
	{
		byte[] flag = new byte[1];
		flag[0] = bSeperatorChar;
		return getArrayBySeperatorArray(bSrc, flag);
	}
	
	
	//获取分隔符数组两边数据
	public static byte[][] getArrayBySeperatorArray(byte[] bSrc, byte[] bSeperatorChar)
	{
		byte[][] retdata = new byte[2][]; 
		retdata[0] = null;
		retdata[1] = null;
		
		//处理源数据为空情况
		if (bSrc == null)
		{
			return retdata;
		}
		//处理分隔数据为空境况
		if (bSeperatorChar == null)
		{
			retdata[0] = bSrc.clone();
			return retdata;
		}
		
		//处理源数据长度小于分隔数据长度的情况
		if (bSrc.length < bSeperatorChar.length)
		{
			retdata[0] = bSrc.clone();
			return retdata;
		}
		
		int i = 0;
		int j = 0;
		//遍历源数据并查找分隔数据
		for (i = 0; i < bSrc.length; ++i)
		{
			//如果剩余数据小于分隔数据则比较结束
			if (bSrc.length-i < bSeperatorChar.length)
			{
				retdata[0] = bSrc.clone();
				return retdata;
			}

			//比较数据是否相同
			for (j = 0; j < bSeperatorChar.length; ++j)
			{
				if ( bSrc[i+j] != bSeperatorChar[j] )
				{
					break;
				}
			}
			
			//找到指定的字符串
			if ( j == bSeperatorChar.length )
			{
				if (i > 0)
				{
					retdata[0] = new byte[i];
					System.arraycopy(bSrc, 0, retdata[0], 0, i);
				}
				
				int len = bSrc.length - bSeperatorChar.length - i;
				if (len > 0)
				{
					retdata[1] = new byte[len];
					System.arraycopy(bSrc, i+bSeperatorChar.length, retdata[1], 0, len);
				}
				return retdata;
			}
			
		}
		retdata[0] = bSrc.clone();
		return retdata;
	}

	/**
	 * 比较两个字符数组的指定区段字符是否完全相同
	 * @param arrayA 数组A
	 * @param posA  比较区段的起始索引号
	 * @param arrayB 数组B
	 * @param posB 比较区段的起始索引号
	 * @param length 比较区段的字节数
	 * @return 相同返回true, 否则返回false
	 */
	boolean arrayCompare(byte[] arrayA, int posA, byte[] arrayB, int posB, int length )
	{
		//判断参数是否有效
		if( null == arrayA || null == arrayB || arrayA.length <= posA
				|| arrayB.length <= posB || length <= 0 )
		{
			return false;
		}
		
		//如果超出了数组长度，则肯定是不相等的
		if( posA + length > arrayA.length || posB + length > arrayB.length )
		{
			return false;
		}
		 
		//逐个字节比较，如果完全相等，则返回true
		for( int i = 0; i < length; i++ )
		{
			if( arrayA[i+posA] != arrayB[i+posB])
			{
				return false;//有一个字符不相同，则返回false
			}
		}		
		return true;//完全相同，返回true
	}
}
