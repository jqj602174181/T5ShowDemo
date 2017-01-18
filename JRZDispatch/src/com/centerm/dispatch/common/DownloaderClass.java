package com.centerm.dispatch.common;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

import android.util.Log;

import com.centerm.common.ChangeDataClass;
import com.centerm.dispatch.MainActivity;

public class DownloaderClass 
{
	private static long        mRemain = 0;//���ڼ�ס�����ļ�ʱ��ʣ�������
	private static long        mSum = 0;//���ڼ�ס�����ļ�ʱ�ܰ�����
	private static String      mstrFilePath = null;//���ڼ�ס�����ļ�ʱ�ļ�·��
	
	//�ж����ļ���������
	private boolean isNewFile( long nSum, long nRemain )
	{
		if (nRemain == nSum - 1)
		{
			return true;
		}
		return false;
	}
	
	//�ж����һ�����ݰ�����
	private boolean isLastPackage( long nRemain )
	{
		if (nRemain == 0)
		{
			return true;
		}
		return false;
	}
	
	//У��ֵ�ж�
	private boolean checkSum( byte[] fileData )
	{
		byte bXor = 0;
		
		//����У��ֵ
		for( int i = 0; i < fileData.length-1; i++ )
		{
			bXor ^= fileData[i];
		}

		if (bXor == fileData[fileData.length-1])
		{
			return true;
		}

		Log.e(MainActivity.TAG, "checkSum error!");
		return false;
	}
	
	//�������ļ�����д�ļ�����
	private boolean createAndWtFile( String strFilePath, byte[] fileData )
	{
		File file = new File( strFilePath );
		try 
		{
			if ( !file.exists() )
			{
				file.createNewFile();
			}
			FileOutputStream out = new FileOutputStream( file );
			out.write( fileData );
			out.flush();
			out.close();
			
			Runtime.getRuntime().exec("chmod 0777 " + strFilePath);
		} 
		catch (Exception e) 
		{
			Log.e(MainActivity.TAG, "createAndWtFile error!");
			e.printStackTrace();
		}
		
		return true;
	}
	
	//׷���ļ����ݲ�����
	private boolean appendFileData(  String strFilePath, byte[] fileData )
	{
		try 
		{
			File file = new File( strFilePath );
			
			//�ļ�������ֱ�ӷ��ش���
			if ( !file.exists() )
			{
				return false;
			}
			RandomAccessFile rf = new RandomAccessFile(strFilePath, "rw");
			rf.seek(rf.length() );
			rf.write( fileData );
			rf.close();
		} 
		catch (IOException e) 
		{
			Log.e(MainActivity.TAG, "appendFileData error!");
			e.printStackTrace();
		}
			
		return true;
	}
	
	//�������ؼ�¼
	private void rememberInfo( long nSum, long  nRemain,  String strFilePath )
	{
		//���һ����
		if ( nRemain == 0 )
		{
			mRemain = 0;
			mstrFilePath = null;
			mSum = 0;
		}
		//�������һ����
		else
		{
			if ( mstrFilePath != null 
					&& !mstrFilePath.equals(strFilePath)
					&& DealFileClass.isFileExist(mstrFilePath) )
			{
				DealFileClass.deleteFile(mstrFilePath);
			}
			
			mSum = nSum;
			mRemain = nRemain;
			mstrFilePath = strFilePath;
		}
	}
	
	//���ݰ�У��
	private boolean checkPackage( long nSum, long nRemain, String strFilePath )
	{
		if (nSum == mSum && nRemain == mRemain -1 && strFilePath.equals(mstrFilePath))
		{
			return true;
		}
		Log.e(MainActivity.TAG, "checkPackage error!");
		return false;
	}
	
	//�������ؼ�¼
	private void resetRemember()
	{
		if ( mstrFilePath != null && DealFileClass.isFileExist(mstrFilePath) )
		{
			DealFileClass.deleteFile(mstrFilePath);
		}
		mRemain = 0;
		mstrFilePath = null;
		mSum = 0;
	}
	
	//�������ݽ���������
	private boolean parseAndWriteData2File( String fileName, byte[] fileData )
	{	
		long  nRemain;//ʣ�����
		long  nSum;//�ܿ���
		byte[] bdataBuf = null;
		boolean bRet = false;
		String tempPath = "/tmp/" + fileName + ".bak";
		
		if (fileData != null && fileData.length%2 != 0)
		{
			return false;
		}
		
		//�ϲ�����
		bdataBuf = ChangeDataClass.starCombination( fileData );
		
		//���У��ֵ
		if ( bdataBuf == null || !checkSum( bdataBuf ) )
		{
			return false;
		}

		//��ȡ�ܿ���
		nSum = ((bdataBuf[3]&0xFF)*256*256*256 + (bdataBuf[2]&0xFF)*256*256 + (bdataBuf[1]&0xFF)) * 256 + (bdataBuf[0]&0xFF);
		
		//��ȡʣ�����
		nRemain = ((bdataBuf[7]&0xFF)*256*256*256 + (bdataBuf[6]&0xFF)*256*256 + (bdataBuf[5]&0xFF)) * 256 + (bdataBuf[4]&0xFF);
		
		//���ݿ��쳣����
		if ( nSum <= 0 || nRemain < 0 || nSum <= nRemain )
		{
			return false;
		}
		
		//��ȡ�ļ�������Ϣ
		byte[] fileDataBuf = new byte[bdataBuf.length - 9];
		for( int i = 8; i < bdataBuf.length-1; i++ )
		{
			fileDataBuf[i-8] = bdataBuf[i];
		}
		
		//�µ��ļ���
		if ( isNewFile( nSum, nRemain ) )
		{
			bRet = createAndWtFile( tempPath, fileDataBuf );
		}
		else
		{
			bRet = checkPackage( nSum, nRemain, tempPath ) && appendFileData( tempPath, fileDataBuf );
		}

		if( bRet )
		{
			rememberInfo(  nSum, nRemain,  tempPath );
		}
		return bRet;
	}
	
	//���������������ļ�
	public boolean saveFile(String pathName, String fileName, byte[] data)
	{
		boolean bRet = true;
		
		//�������ݽ���������
		if ( !parseAndWriteData2File( fileName, data ) )
		{
			bRet = false;
			Log.e(MainActivity.TAG, "dwnloadFile parseAndWriteData2File( nFileType, strFileName, fileData ) == null..............");
		}
		
		//��������һ�������������ļ����Ƶ�����Ŀ¼
		if (bRet && isLastPackage(mRemain))
		{
			String strFilePath = "/tmp/" + fileName  + ".bak";
			DealFileClass.deleteFile(pathName);
			DealFileClass.copyFile(strFilePath, pathName);
			DealFileClass.deleteFile(strFilePath);
		}
		
		//���ü�ס��������Ϣ
		if ( !bRet )
		{
			resetRemember();
			return false;
		}
		return true;
	}
}
	
