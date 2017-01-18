package com.centerm.common;

public final class ByteHandlerClass 
{
	
	//�����ֽ��������ݱȽϣ���ͬ����true, ��ͬ����false
	public static boolean cmpByteStartWith(byte[] bSrc, byte[] bCmpData)
	{
		if (bSrc == null || bCmpData == null)
		{
			return false;
		}
		
		//�Ƚ����ݳ����Ƿ���ͬ
		int nCmpDataLen = bCmpData.length;
		int nSrcLen = bSrc.length;
		
	    if ( nSrcLen != nCmpDataLen)
	    {
			return false;
		}
		
	    //�Ƚ���������
		for (int i = 0; i < nCmpDataLen; i++)
		{
			if (bSrc[i] != bCmpData[i])
			{
				return false;
			}
		}
		return true;
	}
	
	//�����ֽ���������ͷ�����ֽڱȽϣ���ͬ����true, ��ͬ����false
	public static boolean cmpByteHead(byte[] bSrc, byte[] bCmpData)
	{
		if (bSrc == null || bCmpData == null)
		{
			return false;
		}
		
		//�Ƚ����ݳ����Ƿ���ͬ
		int nCmpDataLen = bCmpData.length;
		int nSrcLen = bSrc.length;
		
	    if ( nSrcLen < nCmpDataLen)
	    {
			return false;
		}
		
	    //�Ƚ���������
		for (int i = 0; i < nCmpDataLen; i++)
		{
			if (bSrc[i] != bCmpData[i])
			{
				return false;
			}
		}
		return true;
	}
	
	//��ȡ�ָ����ַ����ߵ�����
	public static byte[][] getByteArrayBeforeSeperator(byte[] bSrc, byte bSeperatorChar)
	{
		byte[] flag = new byte[1];
		flag[0] = bSeperatorChar;
		return getArrayBySeperatorArray(bSrc, flag);
	}
	
	
	//��ȡ�ָ���������������
	public static byte[][] getArrayBySeperatorArray(byte[] bSrc, byte[] bSeperatorChar)
	{
		byte[][] retdata = new byte[2][]; 
		retdata[0] = null;
		retdata[1] = null;
		
		//����Դ����Ϊ�����
		if (bSrc == null)
		{
			return retdata;
		}
		//����ָ�����Ϊ�վ���
		if (bSeperatorChar == null)
		{
			retdata[0] = bSrc.clone();
			return retdata;
		}
		
		//����Դ���ݳ���С�ڷָ����ݳ��ȵ����
		if (bSrc.length < bSeperatorChar.length)
		{
			retdata[0] = bSrc.clone();
			return retdata;
		}
		
		int i = 0;
		int j = 0;
		//����Դ���ݲ����ҷָ�����
		for (i = 0; i < bSrc.length; ++i)
		{
			//���ʣ������С�ڷָ�������ȽϽ���
			if (bSrc.length-i < bSeperatorChar.length)
			{
				retdata[0] = bSrc.clone();
				return retdata;
			}

			//�Ƚ������Ƿ���ͬ
			for (j = 0; j < bSeperatorChar.length; ++j)
			{
				if ( bSrc[i+j] != bSeperatorChar[j] )
				{
					break;
				}
			}
			
			//�ҵ�ָ�����ַ���
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
	 * �Ƚ������ַ������ָ�������ַ��Ƿ���ȫ��ͬ
	 * @param arrayA ����A
	 * @param posA  �Ƚ����ε���ʼ������
	 * @param arrayB ����B
	 * @param posB �Ƚ����ε���ʼ������
	 * @param length �Ƚ����ε��ֽ���
	 * @return ��ͬ����true, ���򷵻�false
	 */
	boolean arrayCompare(byte[] arrayA, int posA, byte[] arrayB, int posB, int length )
	{
		//�жϲ����Ƿ���Ч
		if( null == arrayA || null == arrayB || arrayA.length <= posA
				|| arrayB.length <= posB || length <= 0 )
		{
			return false;
		}
		
		//������������鳤�ȣ���϶��ǲ���ȵ�
		if( posA + length > arrayA.length || posB + length > arrayB.length )
		{
			return false;
		}
		 
		//����ֽڱȽϣ������ȫ��ȣ��򷵻�true
		for( int i = 0; i < length; i++ )
		{
			if( arrayA[i+posA] != arrayB[i+posB])
			{
				return false;//��һ���ַ�����ͬ���򷵻�false
			}
		}		
		return true;//��ȫ��ͬ������true
	}
}
