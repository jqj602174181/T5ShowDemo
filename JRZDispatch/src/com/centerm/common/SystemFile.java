package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

//系统相关文件的读和写
public class SystemFile {
	
	/**
	 * 向系统文件中写入数据
	 */
	public static boolean write( String path, byte[] data ){
		//判断文件是否存在，且有写权限
		File file = new File( path );
		if( file.exists() && file.canWrite() ){
			//写入数据
			try {
				FileOutputStream os = new FileOutputStream( file );
				os.write( data );
				os.close();
				return true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	/**
	 * 向支持随机读取的系统文件指定偏移量写入数据
	 * 注意：并不是所有系统文件都只支持随机读取功能
	 * @param path 文件路径
	 * @param offset 文件偏移量
	 * @param data 待写入的数据
	 * @return 成功或失败
	 */
	public static boolean write( String path, long offset, byte[] data ){		
		RandomAccessFile file = null;//文件
		try {
			file = new RandomAccessFile( path, "rw");
			file.seek( offset );
			file.write( data );
			file.close();
			return true;
		}catch( FileNotFoundException fe){
			//不做任何处理，直接返回
		}catch( IOException e ) {
			e.printStackTrace();
		}
		finally
		{
			if( null != file )
			{
				try {
					file.close();
				} catch (IOException e) {					
					e.printStackTrace();
				}
			}
		}
		
		return false;
	}

	/**
	 * 向系统文件中写入数据
	 */
	public static boolean write( String path, String data ){
		return write( path, data.getBytes());
	}
	
	
	/**
	 * 从系统文件中读出数据
	 */
	public static byte[] readBin( String path ){
		//判断文件是否存在，且有写权限
		File file = new File( path );
		if( file.exists() && file.canRead() ){
			//写入数据
			try {				
				FileInputStream is = new FileInputStream( file );
				
				//循环读取数据
				byte[] data = new byte[4096];
				int n = 0;	//单次读到的长度
				int readLen = 0;//总共读到的长度
				do{
					//缓存不足时，需要重新构建存储，增加4096个字节
					if( data.length - readLen == 0){
						byte[] newbuf = new byte[ data.length + 4096 ];
						System.arraycopy( data, 0, newbuf, 0, readLen );
						data = newbuf;
					}
					
					//读取并累计读取的总字节数
					n = is.read( data, readLen, data.length - readLen );
					if( n > 0 )
						readLen += n;
				}while( n > 0 );				
				
				is.close();
				return data;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return new byte[0];
	}
	
	/**
	 * 从支持随机读取的系统文件的偏移量offset处，读取count个字节，
	 * @param path 系统文件 
	 * @param offset 偏移量
	 * @param count 字节数
	 * @return 读到的数据
	 */
	public static byte[] readBin( String path, long offset, int count ){
		RandomAccessFile file = null;//文件
		byte[] data = new byte[count];
		int n = -1;//读到的字节数
		
		//读文件
		try {			
			file = new RandomAccessFile( path, "r");			
			file.seek( offset );
			n = file.read( data );
			file.close();
		}catch( FileNotFoundException fe){
			//不做任何处理，直接返回
		}catch( IOException e ) {
			e.printStackTrace();
		}
		finally
		{
			if( null != file )
			{
				try {
					file.close();
				} catch (IOException e) {					
					e.printStackTrace();
				}
			}
		}
		
		if( n == count )//正常完成读操作
			return data;
		else if( n < 1 )//读取失败或发生异常 ，返回空
			return new byte[0];
		else{//字节数不足count，则重新构建
			byte[] buf = new byte[n];
			System.arraycopy( data, 0, buf, 0, n );
			return buf;
		}
	}
	
	/**
	 * 从系统文件中读出数据，返回String类型
	 */
	public static String read( String path ){
		return new String( readBin( path));
	}
}
