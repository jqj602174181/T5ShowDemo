package com.centerm.common;

import android.util.Log;



/**
 * @class: DataHandlerUnit
 * @function: �����ϲ����ݹ���ģ��
 * @author: zxx
 * 
 * @data:2013-02-19
 */
public final class ChangeDataClass 
{
	/**
	 * @name: bytesToHexString
	 * @function: ��һ��byte���ݲ��ת�����ַ���,��byte[]{0x2B, 0��44, 0xEF}�C> "2B44EF"
	 * 
	 * @param byte[]
	 *            bData ---����������
	 * @return byte[] �ɹ����ز�ֺ�����ݣ�ʧ�ܷ���""
	 */

	public static byte[] bytesToHexString(byte[] bData) 
	{
		if (bData == null)
		{
			return null;
		}

        byte[] dest = new byte[2*bData.length];
        int temp = 0;
		for (int i = 0; i < bData.length; i++) 
		{
			temp = (bData[i] & 0xF0) >> 4;
	        if ( temp <= 9 )
	        {
	        	dest[2*i] = (byte)(0x30 + temp);
	        }
	        else
	        {
	        	dest[2*i] = (byte)(0x41 + temp -10);
	        }
	        
	        temp = (bData[i] & 0x0F);
	        if ( temp <= 9 )
	        {
	        	dest[2*i+1] = (byte)(0x30 + temp);
	        }
	        else
	        {
	        	dest[2*i+1] = (byte)(0x41 + temp -10);
	        }
		}
		return dest;
	}

	/**
	 * @name: hexStringToBytes
	 * @function: ��ָ��byte���ݣ���ÿ����byte�ϲ�ת��Ϊ16������ʽ �磺"2B44EF" �C> byte[]{0x2B, 0��44,
	 *            0xEF}
	 * 
	 * @param byte[]
	 *            bData ---���ָ���ַ���
	 * 
	 * @return byte[] �ϲ��������
	 */
	public static byte[] hexStringToBytes(byte[] bData) 
	{
		if (bData == null)
		{
			return null;
		}
		
		int nLen = bData.length;
		int temp = 0;
		int i = 0;
		byte[] bRet = new byte[ nLen / 2];
		byte dest;
		for (i = 0; i < bData.length / 2; i++) 
		{
			temp = (bData[i * 2] & 0xFF);

	        if ( (temp >= 0x30) && (temp <= 0x39) )
	        {
	        	dest = (byte)((bData[i * 2] << 4) & 0xF0);
	        }
	        else if ( (temp >= 0x61) && (temp <= 0x66) )
	        {
	        	dest = (byte)(( bData[i * 2] - 'a' + 10 ) << 4);
	        }
	        else if ( (temp >= 0x41) && (temp <= 0x46) )
	        {
	        	dest = (byte)(( bData[i * 2] - 'A' + 10 ) << 4);
	        }
	        else
	        {
	            break;
	        }
	        
			 Log.e("de"," dest index is 11 "+i*2+" dest is "+dest);
	        temp = (bData[2*i+1] & 0xFF);
			 Log.e("de"," temp index is 11 "+i*2+" temp is "+Integer.toHexString(temp));
	        if ( (temp >= 0x30) && (temp <= 0x39) )
	        {
	        	dest |= (temp & 0x0F);
	        }
	        else if ( (temp >= 0x61) && (temp <= 0x66) )
	        {
	        	dest |= ( temp - 'a' + 10 );
	        }
	        else if ( (temp >= 0x41) && (temp <= 0x46) )
	        {
	        	dest |= ( temp - 'A' + 10 );
	        }
	        else
	        {
	            break;
	        }
	    	Log.e("de"," dest index is 222 "+(i*2+1)+" dest 112 is "+dest);   
			bRet[i] = dest;
		}
		
		Log.e("de"," i iiiii is "+i);
		if (i != bData.length / 2)
		{
			return null;
		}
		return bRet;
	}
	
	/**
	 * @name: starCombination
	 * @function: ��ָ���ַ���src����ÿ�����ַ��ϲ�ת��Ϊ16������ʽ �磺byte[] {0x35, 0x3B, 0x34, 0x35 } 
	 * �C> byte[]{0x5B, 0��45}
	 *    	
	 * 
	 * @param byte
	 *            bData ---���ϲ���byte[]����
	 * 
	 * @return byte[] �ϲ��������
	 */
	public static byte[] starCombination(byte[] bData) 
	{
		if (bData == null)
		{
			return null;
		}
		int nLen = bData.length;
		int i = 0;
		int j = 0;
		byte[] bReturnData = new byte[nLen / 2];
		
		for (i = 0; i < nLen; i = i + 2) 
		{
			j = i / 2;
			bReturnData[j] = (byte) (((bData[i] << 4) & 0xF0) | (bData[i + 1] & 0x0F));
		}
		
		return bReturnData;
	}

	/**
	 * @name: starDivide
	 * @function: ��һ��byte��ֳ����� �磺byte[]{0x2B,0x45} �C> byte[]{0x32. 0x3B, 0x34,
	 *            0x45}
	 * 
	 * @param byte[]
	 *            bData ---���ָ���ַ�������
	 * 
	 * @return byte[] ��ֺ������
	 */
	public static byte[] starDivide(byte[] bData) 
	{
		if (bData == null)
		{
			return null;
		}
		
		int nLen = bData.length;
		byte[] bReturnData = new byte[2 * nLen];

		for (int i = 0; i < nLen; i++) 
		{
			bReturnData[2 * i] = (byte) (((bData[i] & 0xF0) >> 4) + 0x30);
			bReturnData[2 * i + 1] = (byte) ((bData[i] & 0x0F) + 0x30);
		}
		
		return bReturnData;
	}
}
