package com.centerm.common;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

//ϵͳ����ļ��Ķ���д
public class SystemFile {
	
	/**
	 * ��ϵͳ�ļ���д������
	 */
	public static boolean write( String path, byte[] data ){
		//�ж��ļ��Ƿ���ڣ�����дȨ��
		File file = new File( path );
		if( file.exists() && file.canWrite() ){
			//д������
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
	 * ��֧�������ȡ��ϵͳ�ļ�ָ��ƫ����д������
	 * ע�⣺����������ϵͳ�ļ���ֻ֧�������ȡ����
	 * @param path �ļ�·��
	 * @param offset �ļ�ƫ����
	 * @param data ��д�������
	 * @return �ɹ���ʧ��
	 */
	public static boolean write( String path, long offset, byte[] data ){		
		RandomAccessFile file = null;//�ļ�
		try {
			file = new RandomAccessFile( path, "rw");
			file.seek( offset );
			file.write( data );
			file.close();
			return true;
		}catch( FileNotFoundException fe){
			//�����κδ���ֱ�ӷ���
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
	 * ��ϵͳ�ļ���д������
	 */
	public static boolean write( String path, String data ){
		return write( path, data.getBytes());
	}
	
	
	/**
	 * ��ϵͳ�ļ��ж�������
	 */
	public static byte[] readBin( String path ){
		//�ж��ļ��Ƿ���ڣ�����дȨ��
		File file = new File( path );
		if( file.exists() && file.canRead() ){
			//д������
			try {				
				FileInputStream is = new FileInputStream( file );
				
				//ѭ����ȡ����
				byte[] data = new byte[4096];
				int n = 0;	//���ζ����ĳ���
				int readLen = 0;//�ܹ������ĳ���
				do{
					//���治��ʱ����Ҫ���¹����洢������4096���ֽ�
					if( data.length - readLen == 0){
						byte[] newbuf = new byte[ data.length + 4096 ];
						System.arraycopy( data, 0, newbuf, 0, readLen );
						data = newbuf;
					}
					
					//��ȡ���ۼƶ�ȡ�����ֽ���
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
	 * ��֧�������ȡ��ϵͳ�ļ���ƫ����offset������ȡcount���ֽڣ�
	 * @param path ϵͳ�ļ� 
	 * @param offset ƫ����
	 * @param count �ֽ���
	 * @return ����������
	 */
	public static byte[] readBin( String path, long offset, int count ){
		RandomAccessFile file = null;//�ļ�
		byte[] data = new byte[count];
		int n = -1;//�������ֽ���
		
		//���ļ�
		try {			
			file = new RandomAccessFile( path, "r");			
			file.seek( offset );
			n = file.read( data );
			file.close();
		}catch( FileNotFoundException fe){
			//�����κδ���ֱ�ӷ���
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
		
		if( n == count )//������ɶ�����
			return data;
		else if( n < 1 )//��ȡʧ�ܻ����쳣 �����ؿ�
			return new byte[0];
		else{//�ֽ�������count�������¹���
			byte[] buf = new byte[n];
			System.arraycopy( data, 0, buf, 0, n );
			return buf;
		}
	}
	
	/**
	 * ��ϵͳ�ļ��ж������ݣ�����String����
	 */
	public static String read( String path ){
		return new String( readBin( path));
	}
}
