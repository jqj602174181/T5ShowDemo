package com.centerm.common;

import android.util.Log;

public class Algorithm_H {
	
	//sm2 test
	public static boolean sm2Decode()
	{
		byte[] d = {(byte)0xce, (byte)0xe8, (byte)0x0c, (byte)0x64, 
				(byte)0x90, (byte)0x94, (byte)0x91, (byte)0xc8, 
				(byte)0xde, (byte)0xdc, (byte)0xf3, (byte)0xac, 
				(byte)0xff, (byte)0x25, (byte)0xa1, (byte)0x90, 
				(byte)0x2b, (byte)0x79, (byte)0xb0, (byte)0xc3, 
				(byte)0x62, (byte)0x40, (byte)0x09, (byte)0x23, 
				(byte)0x79, (byte)0xb0, (byte)0x59, (byte)0xda, 
				(byte)0x9a, (byte)0x41, (byte)0x4a, (byte)0x79};
		byte[] data = {(byte)0x79, (byte)0x37, (byte)0x67, (byte)0x8f, 
				(byte)0x12, (byte)0xd5, (byte)0xee, (byte)0x03, 
				(byte)0x5f, (byte)0x08, (byte)0x05, (byte)0xf7, 
				(byte)0xf6, (byte)0x01, (byte)0xb5, (byte)0x00, 
				(byte)0xe0, (byte)0x2e, (byte)0x79, (byte)0x04, 
				(byte)0x48, (byte)0xe6, (byte)0x6b, (byte)0x70, 
				(byte)0xd0, (byte)0x74, (byte)0x4e, (byte)0xf7, 
				(byte)0x29, (byte)0x49, (byte)0xa4, (byte)0xcb, 
				(byte)0x49, (byte)0xd9, (byte)0x52, (byte)0xa2, 
				(byte)0x20, (byte)0x16, (byte)0xe6, (byte)0xc4, 
				(byte)0xbb, (byte)0x70, (byte)0x41, (byte)0x4a, 
				(byte)0x35, (byte)0x5a, (byte)0x58, (byte)0x4f, 
				(byte)0x6c, (byte)0xac, (byte)0xe0, (byte)0xfc, 
				(byte)0xc9, (byte)0xec, (byte)0xb8, (byte)0xa6, 
				(byte)0x72, (byte)0x34, (byte)0x5f, (byte)0xcd, 
				(byte)0x96, (byte)0x94, (byte)0xc7, (byte)0x8f, 
				(byte)0x2c, (byte)0xd4, (byte)0x38, (byte)0x9e, 
				(byte)0x2e, (byte)0x8b, (byte)0xfe, (byte)0xa5, 
				(byte)0xe1, (byte)0x0a, (byte)0xd7, (byte)0x14, 
				(byte)0xb5, (byte)0xbc, (byte)0x53, (byte)0xf4, 
				(byte)0x35, (byte)0x3a, (byte)0xca, (byte)0x2a, 
				(byte)0xc0, (byte)0xbe, (byte)0xa5, (byte)0xd5, 
				(byte)0x80, (byte)0xc3, (byte)0xca, (byte)0x78, 
				(byte)0x93, (byte)0xc7, (byte)0x8f, (byte)0x47, 
				(byte)0xab, (byte)0xcc, (byte)0x11, (byte)0x9a, 
				(byte)0xac, (byte)0xe5, (byte)0x01, (byte)0xbd, 
				(byte)0x26, (byte)0xbc, (byte)0xce, (byte)0x38, 
				(byte)0x3d, (byte)0x64, (byte)0x12, (byte)0xa4, 
				(byte)0x54, (byte)0xd2, (byte)0x27, (byte)0x9c, 
				(byte)0x0c, (byte)0xc0, (byte)0x78, (byte)0x03, 
				(byte)0x35, (byte)0x47, (byte)0xac, (byte)0x33, (byte)0x1b};
		byte[] decode = new byte[128];
		int codeLen = sm2Decode(d, data, data.length, decode);
		/*String result = String.format("%02x ", decode[0]);
		for(int i=0; i< codeLen; i++)
		{
			result += String.format("%02x ", decode[i]);
		}
		Log.i("Encryption", String.format("%d", codeLen));
		Log.i("Encryption", result);*/
		if (0 == codeLen)
		{
			Log.i("test", String.format("decode failed.codeLen=%d", codeLen));
			return false;
		}
		
		byte[] result = new byte[codeLen];
		System.arraycopy(decode, 0, result, 0, codeLen);
		String str = new String(result);
	
		return str.equals("2323r3f3fffgfo34gj4hvi45gvi54");
	}
	
	//sm2
	public static byte[] sm2_GetKey()
	{
		byte[] bKeyData = new byte[128];
		int nRet = sm2GetKey(bKeyData);
		if (nRet > 0)
		{
			byte[] bKey = new byte[nRet];
			System.arraycopy(bKeyData, 0, bKey, 0, nRet);
			return bKey;
		}
		return null;
	}
	public static byte[] sm2_Encode(byte[] x, byte[] y, byte[] data, int dataLen)
	{
		byte[] bEncode = new byte[256];
		int nRet = sm2Encode(x, y, data, dataLen, bEncode);
		if (nRet >= 0)
		{
			byte[] bEText = new byte[nRet];
			System.arraycopy(bEncode, 0, bEText, 0, nRet);
			return bEText;
		}
		return null;
	}
	public static byte[] sm2_Decode(byte[] prikey, byte[] data, int dataLen)
	{
		byte[] bDecode = new byte[128];
		int nRet = sm2Decode(prikey, data, dataLen, bDecode);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bDecode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	public static byte[] sm4_Encode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen)
	{
		byte[] bEncode = new byte[128];
		int nRet = sm4Encode(key, keyLen, mode, data, dataLen, iv, ivLen, bEncode);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bEncode, 0, bText, 0, nRet);
			return bText;
		}
		return null;

	}
	public static byte[] sm4_Decode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen)
	{
		byte[] bDecode = new byte[128];
		int nRet = sm4Decode(key, keyLen, mode, data, dataLen, iv, ivLen, bDecode);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bDecode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	public static byte[] sm3_Abt(byte[] data, int dataLen)
	{
		byte[] bAbt = new byte[128];
		int nRet = sm3Abt(data, dataLen, bAbt);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bAbt, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	public static byte[] sm3_AbtID(byte[] x, byte[] y, byte[] id, int idLen, byte[] data, int dataLen)
	{
		byte[] bAbt = new byte[128];
		int nRet = sm3AbtID(x, y, id, idLen, data, dataLen, bAbt);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bAbt, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	//rsa
	public static byte[] rsa_GetKey(int eBitlen, int nBitlen)
	{
		byte[] bKeyData = new byte[1024];
		int nRet = rsaNewKey(eBitlen, nBitlen, bKeyData);
		Log.i("rsa_GetKey", "nRet" + nRet);
		if (nRet > 0)
		{
			byte[] bKey = new byte[nRet];
			System.arraycopy(bKeyData, 0, bKey, 0, nRet);
			return bKey;
		}
		return null;
	}
	public static byte[] rsa_Public(byte[] n, int nBitlen, byte[] e, int eBitlen, byte[] data, int dataBitlen)
	{
		byte[] bEncode = new byte[512];
		int nRet = rsaPubEncrypt(n, nBitlen, e, eBitlen, data, dataBitlen, bEncode);
		if (nRet >= 0)
		{
			Log.i("rsa_Public", "nRet=" + nRet);
			byte[] bText = new byte[nRet];
			System.arraycopy(bEncode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	
	public static byte[] rsa_Private(byte[] n, int nBitlen, byte[] d, int dBitlen, byte[] data, int dataBitlen)
	{
		byte[] bEncode = new byte[512];
		int nRet = rsaPriEncrypt(n, nBitlen, d, dBitlen, data, dataBitlen, bEncode);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bEncode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	public static byte[] sm1_Encode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen)
	{
		byte[] bEncode = new byte[128];
		int nRet = sm1Encode(key, keyLen, mode, data, dataLen, iv, ivLen, bEncode);
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bEncode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	public static byte[] sm1_Decode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen)
	{
		byte[] bDecode = new byte[128];
		int nRet = sm1Encode(key, keyLen, mode, data, dataLen, iv, ivLen, bDecode);;
		if (nRet >= 0)
		{
			byte[] bText = new byte[nRet];
			System.arraycopy(bDecode, 0, bText, 0, nRet);
			return bText;
		}
		return null;
	}
	
	//获取加密新品版本号
	public static String getEncryptVersion()
	{
		byte[] bTmp = new byte[128];
		int nRet = versionEncrypt(bTmp);
		if (nRet > 0)
		{
			byte[] bVersion = new byte[nRet];
			System.arraycopy(bTmp, 0, bVersion, 0, nRet);
			return new String(bVersion);
		}
		else
		{
			return null;
		}
	}
	
	public static int updateEncryption(byte[] filePath)
	{
		return updateEncrypt(filePath);
	}
	
	//重置模块
	private static native int resetEncrypt();
	//烧写模块
	private static native int flashEncrypt();
	private static native int versionEncrypt(byte[] version);
	private static native int updateEncrypt(byte[] filePath);
	private static native int sm1Encode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen, byte[] encode);
	private static native int sm1Decode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen, byte[] decode);
	//sm2获取密钥
	private static native int sm2GetKey(byte[] key);
	private static native int sm2Encode(byte[] x, byte[] y, byte[] data, int dataLen, byte[] encode);
	private static native int sm2Decode(byte[] prikey, byte[] data, int dataLen, byte[] decode);
	private static native int sm2Sign(byte[] prikey, byte[] e, byte[] sign);
	private static native int sm2SignCheck(byte[] x, byte[] y, byte[] r, byte[] s, byte[] e);
	private static native int sm2Agt(byte[] ownX, byte[] ownY, byte[] ownPrikey, byte[] ownTempX, byte[] ownTempY, byte[] ownTempPrikey, 
			byte[] otherX, byte[] otherY, byte[] otherTempX, byte[] otherTempY, 
			byte[] ownID, int ownIDLen, byte[] otherID, int otherIDLen, byte role, int kLen, byte[] key);
	private static native int sm3Abt(byte[] data, int dataLen, byte[] abt);
	private static native int sm3AbtID(byte[] x, byte[] y, byte[] id, int idLen, byte[] data, int dataLen, byte[] abt);
	private static native int sm4Encode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen, byte[] encode);
	private static native int sm4Decode(byte[] key, int keyLen, byte mode, byte[] data, int dataLen,
			byte[] iv, int ivLen, byte[] decode);
	private static native int rsaNewKey(int eBitlen, int nBitlen, byte[] key);
	private static native int rsaPubEncrypt(byte[] n, int nBitlen, byte[] e, int eBitlen, byte[] data, int dataBitlen, byte[] encrypt);
	private static native int rsaPriEncrypt(byte[] n, int nBitlen, byte[] d, int dBitlen, byte[] data, int dataBitlen, byte[] encrypt);
	static
	{
		System.loadLibrary( "encryption" );
	}
	
	
	
}
