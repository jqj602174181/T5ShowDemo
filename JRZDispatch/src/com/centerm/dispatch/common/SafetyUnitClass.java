package com.centerm.dispatch.common;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.KeySpec;
import java.util.Arrays;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.DESedeKeySpec;
import javax.crypto.spec.SecretKeySpec;

import android.util.Log;

import com.centerm.common.ChangeDataClass;


/**
 * @class: SafetyUnit
 * @function: �����㷨ģ��
 * @author: zxx
 * @data: 2013-02-19
 */
public final class SafetyUnitClass 
{
	final static String DES = "DES/ECB/NoPadding";
	final static String TriDes = "DESede/ECB/NoPadding";
	
	final static String AES = "AES/ECB/NoPadding";
	final static String AESKEY = "AES";

	/**
	 * @name: desEncrypt
	 * @function: DES����
	 * 
	 * @param byte[]
	 *            key ---��Կ
	 * @param byte[]
	 *            data ---���ܵ�����
	 * @return byte[] �ɹ����ؼ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] desEncrypt(byte[] key, byte[] data) 
	{
		if ( key == null || data == null )
		{
			Log.e("SafetyUnitClass", "desEncrypt param is null........");
			return null;
		}
		int len = ((data.length + 7) / 8) * 8;
		if ( len <= 0 )
		{
			Log.e("SafetyUnitClass", "desEncrypt data.length=0........");
			return null;
		}
		byte[] needData = new byte[len];
		Arrays.fill(needData, (byte) 0x00);
		System.arraycopy(data, 0, needData, 0, data.length);
		
		try 
		{
			KeySpec ks = new DESKeySpec(key);
			SecretKeyFactory kf = SecretKeyFactory.getInstance("DES");
			SecretKey ky = kf.generateSecret(ks);

			Cipher c = Cipher.getInstance(DES);
			c.init(Cipher.ENCRYPT_MODE, ky);
			return c.doFinal(needData);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * @name: desDecrypt
	 * @function: DES����
	 * 
	 * @param byte[]
	 *            key ---��Կ
	 * @param byte[]
	 *            data ---���ܵ�����
	 * @return byte[] �ɹ����ؽ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] desDecrypt(byte[] key, byte[] data) 
	{
		if ( key == null || data == null )
		{
			return null;
		}
		
		try 
		{
			KeySpec ks = new DESKeySpec(key);
			SecretKeyFactory kf = SecretKeyFactory.getInstance("DES");
			SecretKey ky = kf.generateSecret(ks);

			Cipher c = Cipher.getInstance(DES);
			c.init(Cipher.DECRYPT_MODE, ky);
			return c.doFinal(data);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * @name: triDesEncrypt
	 * @function: 3DES����
	 * 
	 * @param byte[]
	 *            key ---��Կ
	 * @param byte[]
	 *            data ---���ܵ�����
	 * @return �ɹ����ؼ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] triDesEncrypt(byte[] key, byte[] data) 
	{
		if ( key == null || data == null )
		{
			return null;
		}
		
		int len = (data.length + 7) / 8 * 8;
		if ( len <= 0 )
		{
			return null;
		}
		byte[] needData = new byte[len];
		System.arraycopy(data, 0, needData, 0, data.length);
		
		byte[] k = new byte[24];

		// ������Կ����24
		if (key.length == 16) 
		{
			System.arraycopy(key, 0, k, 0, key.length);
			System.arraycopy(key, 0, k, 16, 8);
		} 
		else if (key.length == 24)
		{
			System.arraycopy(key, 0, k, 0, 24);
		}
		else
		{
			return null;
		}
		
		try 
		{
			KeySpec ks = new DESedeKeySpec(k);
			SecretKeyFactory kf = SecretKeyFactory.getInstance("DESede");
			SecretKey ky = kf.generateSecret(ks);

			Cipher c = Cipher.getInstance(TriDes);
			c.init(Cipher.ENCRYPT_MODE, ky);
			return c.doFinal(needData);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return null;
		}

	}

	/**
	 * @name: triDesDecrypt
	 * @function: 3DES����
	 * 
	 * @param byte[]
	 *            key ---��Կ
	 * @param byte[]
	 *            data ---���ܵ�����
	 * @return byte[] �ɹ����ؽ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] triDesDecrypt(byte[] key, byte[] data) 
	{
		if ( key == null || data == null )
		{
			return null;
		}

		int len = data.length;
		if (data.length % 8 != 0) 
		{
			len = (data.length + 7) / 8 * 8;
		}
		
		if ( len <= 0 )
		{
			return null;
		}
		
		byte[] needData = new byte[len];
		System.arraycopy(data, 0, needData, 0, data.length);

		byte[] k = new byte[24];
		if (key.length == 16) 
		{
			System.arraycopy(key, 0, k, 0, key.length);
			System.arraycopy(key, 0, k, 16, 8);
		}
		else if (key.length == 24)
		{
			System.arraycopy(key, 0, k, 0, 24);
		}
		else
		{
			return null;
		}
		
		try 
		{
			KeySpec ks = new DESedeKeySpec(k);
			SecretKeyFactory kf = SecretKeyFactory.getInstance("DESede");
			SecretKey ky = kf.generateSecret(ks);

			Cipher c = Cipher.getInstance(TriDes);
			c.init(Cipher.DECRYPT_MODE, ky);
			return c.doFinal(needData);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * @name: desSelectEncrypt
	 * @function: DES��3DESѡ���Լ���
	 * 
	 * @param byte[]
	 *            key ---��Կ byte[] data ---���ܵ�����
	 * @return �ɹ����ؼ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] desSelectEncrypt(byte[] key, byte[] data) 
	{
		if (key.length == 16 || key.length == 24) 
		{
			return triDesEncrypt(key, data);
		} 
		else if (key.length == 8)
		{
			return desEncrypt(key, data);
		}
		
		return null;
	}

	/**
	 * @name: desSelectDecrypt
	 * @function: DES��3DESѡ���Խ���
	 * 
	 * @param byte[]
	 *            key ---��Կ
	 * @param byte[]
	 *            data ---���ܵ�����
	 * @return �ɹ����ؼ��ܺ�����ݣ�ʧ�ܷ���null
	 */
	public static byte[] desSelectDecrypt(byte[] key, byte[] data) 
	{
		if (key.length == 16 || key.length == 24) 
		{
			return triDesDecrypt(key, data);
		} 
		else if (key.length == 8) 
		{
			return desDecrypt(key, data);
		}
		
		return null;
	}
	
	/**
	 * @name: getPin
	 * @function: ��ȡPIN
	 * 
	 * @param String
	 *            pin ---PIN��
	 * 
	 * @return byte[] ����ת�����PIN��
	 */
	public static byte[] getPin(byte[] arrPin) 
	{
		if (arrPin == null)
		{
			return null;
		}
		int iLen = arrPin.length;
		if (iLen < 0 || iLen > 14)
		{
			return null;
		}

		byte[] bTmp = new byte[16];
		byte[] encode = null;
		
		Arrays.fill(bTmp, (byte) 0x46);
		bTmp[0] = 0x30;
		if ( iLen >= 10 && iLen <= 14)
		{
			bTmp[1] = (byte)((iLen & 0xFF) -10 + 0x41);
		}
		else
		{
			bTmp[1] = (byte)((iLen & 0xFF) + 0x30);
		}
		for (int i = 0; i < arrPin.length; ++i) 
		{
			bTmp[i+2] = arrPin[i];
		}
		encode = ChangeDataClass.hexStringToBytes(bTmp);
		
		return encode;
	}

	/**
	 * @name: getAccNo
	 * @function: ���˺�
	 * 
	 * @param String
	 *            accno ---�˺�
	 * 
	 * @return byte[] ����ת������˺�
	 */
	public static byte[] getAccNo(byte[] accno) 
	{
		if (accno == null)
		{
			return null;
		}

		byte encode[] = null;
		if ( accno.length == 12 )
		{
			byte arrAccno[] = new byte[16];		
			Arrays.fill(arrAccno, (byte) 0x30);
			for (int i = 0; i < 12; i++) 
			{
				arrAccno[i+4] = accno[i];
			}
			encode = ChangeDataClass.hexStringToBytes(arrAccno);
		}
		else
		{
			int len = accno.length;
			byte temp[] = new byte[12];
			Arrays.fill(temp, (byte) 0x30);
			if (len < 12)
			{
				System.arraycopy(accno, 0, temp, 12-len, len);
			}
			else if (len > 12)
			{
				System.arraycopy(accno, (len-1)-12, temp, 0, 12);
			}
			
			byte arrAccno[] = new byte[16];		
			Arrays.fill(arrAccno, (byte) 0x30);
			for (int i = 0; i < 12; i++) 
			{
				arrAccno[i+4] = temp[i];
			}
			encode = ChangeDataClass.hexStringToBytes(arrAccno);
		}
		return encode;
	}

	/**
	 * @name: getPinBlock
	 * @function: ��PinBlock
	 * 
	 * @param String
	 *            pin ---PIN��
	 * @param String
	 *            accno ---�˺�
	 * @return byte[] ����PinBlock
	 */
	public static byte[] getPinBlock(byte[] pin, byte[] accno) 
	{
		byte arrPin[] = getPin(pin);
		byte arrAccno[] = null;
		if ( accno.length == 8 )
		{
			arrAccno = new byte[8];
			System.arraycopy(accno, 0, arrAccno, 0, accno.length);
		}
		else
		{
			arrAccno = getAccNo(accno);
		}
		if ( arrPin == null || arrAccno == null )
		{
			return null;
		}
		
		byte arrRet[] = new byte[8];
		// PIN BLOCK ��ʽ���� PIN ��λ��� ���ʺ�;
		for (int i = 0; i < 8; i++) 
		{
			arrRet[i] = (byte) (arrPin[i] ^ arrAccno[i]);
		}
		return arrRet;
	}

	/**
	 * @name: encryptAnsi9_8
	 * @function: Ansi9.8��׼������DESѡ���Խ���DES��3DES���м���
	 * 
	 * @param String
	 *            pin ---PIN��
	 * @param String
	 *            accno ---�˺�
	 * @return byte[] ����PinBlock
	 */
	public static byte[] encryptAnsi9_8(byte[] key, byte[] pin, byte[] accno) 
	{
		return desSelectEncrypt(key, getPinBlock(pin, accno));
	}
	

	/**
	 * AES����
	 * 
	 * @param data
	 *            ��Ҫ���ܵ�����
	 * @param key
	 *            ��������
	 * @return
	 */
	public static byte[] encryptAES(byte[] key, byte[] data) 
	{
		
		if (key == null || data == null ) 
		{
			return null;
		} 
		if (key.length != 16 && key.length != 24 && key.length != 32) 
		{
			return null;
		} 
		
		try 
		{
			int nLen = (data.length + 15) / 16 * 16;
			byte[] bNeedData = new byte[nLen];
			System.arraycopy(data, 0, bNeedData, 0, data.length);
			SecretKeySpec genkey = new SecretKeySpec(key, AESKEY);  
			Cipher cipher = Cipher.getInstance(AES);// ����������
			cipher.init(Cipher.ENCRYPT_MODE, genkey);// ��ʼ��
			byte[] result = cipher.doFinal(bNeedData);
			return result; // ����
		} 
		catch (NoSuchAlgorithmException e) 
		{
			e.printStackTrace();
		} 
		catch (NoSuchPaddingException e) 
		{
			e.printStackTrace();
		} 
		catch (InvalidKeyException e) 
		{
			e.printStackTrace();
		} 
		catch (IllegalBlockSizeException e) 
		{
			e.printStackTrace();
		} 
		catch (BadPaddingException e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	/**
	 * AES����
	 * 
	 * @param data
	 *            ����������
	 * @param key
	 *            ������Կ
	 * @return
	 */
	public static byte[] decryptAES(byte[] key, byte[] data) 
	{
		if (data == null || key == null) 
		{
			return null;
		}
		if (key.length != 16 && key.length != 24 && key.length != 32) 
		{
			return null;
		} 

		try 
		{
			SecretKeySpec genkey = new SecretKeySpec(key, AESKEY);
			Cipher cipher = Cipher.getInstance(AES);// ����������
			cipher.init(Cipher.DECRYPT_MODE, genkey);// ��ʼ��
			byte[] result = cipher.doFinal(data);
			return result; // ����
		} 
		catch (NoSuchAlgorithmException e) 
		{
			e.printStackTrace();
		} 
		catch (NoSuchPaddingException e) 
		{
			e.printStackTrace();
		} 
		catch (InvalidKeyException e) 
		{
			e.printStackTrace();
		} 
		catch (IllegalBlockSizeException e) 
		{
			e.printStackTrace();
		} 
		catch (BadPaddingException e) 
		{
			e.printStackTrace();
		}
		return null;
	}
}

