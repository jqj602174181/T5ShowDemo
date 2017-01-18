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
	private static long        mRemain = 0;//用于记住下载文件时还剩余包个数
	private static long        mSum = 0;//用于记住下载文件时总包个数
	private static String      mstrFilePath = null;//用于记住下载文件时文件路径
	
	//判断新文件下载条件
	private boolean isNewFile( long nSum, long nRemain )
	{
		if (nRemain == nSum - 1)
		{
			return true;
		}
		return false;
	}
	
	//判断最后一个数据包条件
	private boolean isLastPackage( long nRemain )
	{
		if (nRemain == 0)
		{
			return true;
		}
		return false;
	}
	
	//校验值判断
	private boolean checkSum( byte[] fileData )
	{
		byte bXor = 0;
		
		//计算校验值
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
	
	//创建新文件，并写文件数据
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
	
	//追加文件数据并保存
	private boolean appendFileData(  String strFilePath, byte[] fileData )
	{
		try 
		{
			File file = new File( strFilePath );
			
			//文件不存在直接返回错误
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
	
	//保存下载记录
	private void rememberInfo( long nSum, long  nRemain,  String strFilePath )
	{
		//最后一个包
		if ( nRemain == 0 )
		{
			mRemain = 0;
			mstrFilePath = null;
			mSum = 0;
		}
		//不是最后一个包
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
	
	//数据包校验
	private boolean checkPackage( long nSum, long nRemain, String strFilePath )
	{
		if (nSum == mSum && nRemain == mRemain -1 && strFilePath.equals(mstrFilePath))
		{
			return true;
		}
		Log.e(MainActivity.TAG, "checkPackage error!");
		return false;
	}
	
	//重置下载记录
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
	
	//下载数据解析并保存
	private boolean parseAndWriteData2File( String fileName, byte[] fileData )
	{	
		long  nRemain;//剩余块数
		long  nSum;//总块数
		byte[] bdataBuf = null;
		boolean bRet = false;
		String tempPath = "/tmp/" + fileName + ".bak";
		
		if (fileData != null && fileData.length%2 != 0)
		{
			return false;
		}
		
		//合并数据
		bdataBuf = ChangeDataClass.starCombination( fileData );
		
		//检测校验值
		if ( bdataBuf == null || !checkSum( bdataBuf ) )
		{
			return false;
		}

		//获取总块数
		nSum = ((bdataBuf[3]&0xFF)*256*256*256 + (bdataBuf[2]&0xFF)*256*256 + (bdataBuf[1]&0xFF)) * 256 + (bdataBuf[0]&0xFF);
		
		//获取剩余块数
		nRemain = ((bdataBuf[7]&0xFF)*256*256*256 + (bdataBuf[6]&0xFF)*256*256 + (bdataBuf[5]&0xFF)) * 256 + (bdataBuf[4]&0xFF);
		
		//数据块异常处理
		if ( nSum <= 0 || nRemain < 0 || nSum <= nRemain )
		{
			return false;
		}
		
		//获取文件数据信息
		byte[] fileDataBuf = new byte[bdataBuf.length - 9];
		for( int i = 8; i < bdataBuf.length-1; i++ )
		{
			fileDataBuf[i-8] = bdataBuf[i];
		}
		
		//新的文件包
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
	
	//保存下载数据至文件
	public boolean saveFile(String pathName, String fileName, byte[] data)
	{
		boolean bRet = true;
		
		//下载数据解析并保存
		if ( !parseAndWriteData2File( fileName, data ) )
		{
			bRet = false;
			Log.e(MainActivity.TAG, "dwnloadFile parseAndWriteData2File( nFileType, strFileName, fileData ) == null..............");
		}
		
		//如果是最后一个包，则将下载文件复制到下载目录
		if (bRet && isLastPackage(mRemain))
		{
			String strFilePath = "/tmp/" + fileName  + ".bak";
			DealFileClass.deleteFile(pathName);
			DealFileClass.copyFile(strFilePath, pathName);
			DealFileClass.deleteFile(strFilePath);
		}
		
		//重置记住的下载信息
		if ( !bRet )
		{
			resetRemember();
			return false;
		}
		return true;
	}
}
	
