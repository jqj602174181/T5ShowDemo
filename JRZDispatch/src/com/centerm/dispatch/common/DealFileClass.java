package com.centerm.dispatch.common;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Properties;

public final class DealFileClass 
{
	public static boolean copyByNioTransferFrom(String szSrcFile, String szDestFile)
	{
		File file = new File(szSrcFile);
		if (!file.exists()) 
		{
			return false;
		}
		try 
		{
			RandomAccessFile fromFile = new RandomAccessFile(szSrcFile, "rw");
			FileChannel fromChannel = fromFile.getChannel();
			RandomAccessFile toFile = new RandomAccessFile(szDestFile, "rw");
			FileChannel toChannel = toFile.getChannel();
			long position = 0;
	        long count = fromChannel.size();
	        toChannel.transferFrom(fromChannel, position, count);
	        fromFile.close();
	        toFile.close();
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public static boolean copyFileByMbb(String szSrcFile, String szDestFile)
	{
		File file = new File(szSrcFile);
		if (!file.exists()) 
		{
			return false;
		}
		try 
		{
			FileInputStream fin = new FileInputStream(szSrcFile);
			RandomAccessFile fout = new RandomAccessFile(szDestFile, "rw");
			FileChannel fcin = fin.getChannel();
	        FileChannel fcout = fout.getChannel();
	        MappedByteBuffer mbbi = fcin.map(FileChannel.MapMode.READ_ONLY, 0,
	                fcin.size());
	        MappedByteBuffer mbbo = fcout.map(FileChannel.MapMode.READ_WRITE, 0,
	                fcin.size());
	        mbbo.put(mbbi);
	        mbbi.clear();
	        mbbo.clear();
	        fin.close();
	        fout.close();
		} 
		catch (FileNotFoundException e) 
		{
			e.printStackTrace();
			return false;
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	//单一文件拷贝
	public static boolean copyFile(String szSrcFile, String szDestFile)
	{
		File file = new File(szSrcFile);
		if (!file.exists()) 
		{
			return false;
		}
		try 
		{
			RandomAccessFile fSrcFile = new RandomAccessFile(szSrcFile, "rw");
			RandomAccessFile fDestFile = new RandomAccessFile(szDestFile, "rw"); 
			int nLen = (int)fSrcFile.length();
			byte[] bData = new byte[nLen];
			fSrcFile.readFully(bData);
			fDestFile.write(bData);
			fSrcFile.close();
			fDestFile.close();
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return false;
		}
		//AccessdService.doShell("sync");
		//AccessdService.doShell("chmod 0777 " + szDestFile);
		return true;
	}
	
	//判断文件是否存在
	public static boolean isFileExist(String path) 
	{
		if (path == null) 
		{
			return false;
		}
		try 
		{
			File f = new File(path);
			if (!f.exists()) 
			{
				return false;
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
    //删除单一文件
	public static void deleteFile(String filename)
	{
		if (filename == null || filename.equals(""))
		{
			return;
		}
		File file = new File(filename);
		if ( !file.exists() )
		{
		    return;
		}
		
		if ( file.isFile() )
		{
			file.delete();
		}
		//AccessdService.doShell("sync");
	}
	
	//查询文件大小
	public static long getFileSize(String filename)
	{
		if (filename == null || filename.equals(""))
		{
			return 0;
		}
		File file = new File(filename);
		if ( !file.exists() )
		{
		    return 0;
		}
		
		return file.length();
	}
	
	//清空文件夹内容
	public static void cleanDirectory( String filePath )
	{
		if ( filePath == null || filePath.equals(""))
		{
			return;
		}
		File f = new File(filePath);
		String[] list = f.list();
		if (list.length < 1)
		{
			return;
		}
		for ( int i = 0; i < list.length; i++ )
		{
			String name = list[i];
			File file = new File(filePath + "/" + name);
			if ( file.isFile() )	//若是文件则直接删除
			{
				file.delete();
			}
			else if ( file.isDirectory() )	//若是文件夹，则迭代本方法
			{
				deleteDirectory(filePath + "/" + name);
			}
		}
		try {
			Runtime.getRuntime().exec("sync");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	//递归删除文件夹与文件
	public static void deleteDirectory( String filePath )
	{
		if ( filePath == null || filePath.equals(""))
		{
			return;
		}
		File f = new File(filePath);
		String[] list = f.list();
		if (list.length < 1)
		{
			return;
		}
		for ( int i = 0; i < list.length; i++ )
		{
			String name = list[i];
			File file = new File(filePath + "/" + name);
			if ( file.isFile() )	//若是文件则直接删除
			{
				file.delete();
			}
			else if ( file.isDirectory() )	//若是文件夹，则迭代本方法
			{
				deleteDirectory(filePath + "/" + name);
			}
		}
		f.delete();	//最后将空文件夹删除
	}

	
	//获取系统属性信息
	public static String getSystemInfo( String key, String defValue )
	{
		File file = new File( "/mnt/internal_sd/config/sysconfig.properties" );
		
		if( file.exists() == false )
		{
			return "";
		}
		try
		{
			//加载属性文件，并返回
			FileInputStream fileStream = new  FileInputStream( file );			
			Properties systemInfo = new Properties();
			systemInfo.load(fileStream);
			fileStream.close();
			return systemInfo.getProperty( key );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
		
		return defValue;
	}
	
	
	//设置系统属性信息
	public static boolean setSystemInfo( String key, String value )
	{
		if ( (key == null || key.equals("")) || (value == null || value.equals("")) )
		{
			return false;
		}
		
		//信息相同，则不设置
		if ( getSystemInfo( key, "ERROR" ) != null )
		{
			if ( getSystemInfo( key, "ERROR" ).equals(value) == true )
			{
				return true;
			}
		}
		
		try
		{
			boolean hasFile = true;
			Properties systemInfo = new Properties();
			File file = new File( "/mnt/internal_sd/config/sysconfig.properties" );
			if( file.exists() == true )//若文件存在，则从文件中得到所有属性值
			{
				FileInputStream inStream = new  FileInputStream( file );
				systemInfo.load(inStream);
				inStream.close();
			}
			else
			{
				hasFile = false;
			}
			
			//开始保存新信息
			systemInfo.setProperty( key, value );
			FileOutputStream outStream = new FileOutputStream( file );
			systemInfo.store(outStream, null);
			
			//如果文件为新创建文件，则增加权限
			if (hasFile == false)
			{
				Runtime.getRuntime().exec("chmod 0777 " + "/mnt/internal_sd/config/sysconfig.properties");
			}
			
			Runtime.getRuntime().exec("sync");
		}
		catch( Exception e )
		{
			e.printStackTrace(); 
		}

		return true;
	}
	
	//截取指定长度数据
	public static byte[] cutChineseData(byte[] bData, int len)
	{
		boolean isChinese = false;
		byte[] ret = null;
		if (bData.length <= len)
		{
			return bData;
		}
		
		for (int i = 0; i < len; i++)
		{
			if ((bData[i] & 0xFF) > 0x80 && isChinese == false)
			{
				isChinese = true;
			}
			else if ((bData[i] & 0xFF) > 0x41 && isChinese == true)
			{
				isChinese = false;
			}
			else
			{
				isChinese = false;
			}
		}
		
		if (isChinese == true)
		{
			ret = new byte[len-1];
			System.arraycopy(bData, 0, ret, 0, len-1);
		}
		else
		{
			ret = new byte[len];
			System.arraycopy(bData, 0, ret, 0, len);
		}
		
		return ret;
	}
}
