package com.centerm.t5.socketserver;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;  
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;  
import java.net.Socket;  

import android.os.Handler;
import android.os.Message;

public class SocketServer   
{  
	private ServerSocket ss = null;  
	private static SocketServer instance = null;  

	private String dir = "/mnt/internal_sd/jrz";

	private Handler handler;
	int num = 0;

	private SocketServer(){
		//创建目录
		File file = new File(dir);
		if(!file.exists()){
			file.mkdir();
		}
	}  

	public void setHandler(Handler handler){
		this.handler = handler;
	}

	public static SocketServer getInstance(){
		if(instance == null){
			instance = new SocketServer();
		}
		return instance;
	}

	public void start(){
		new Thread(){
			public void run() {
				try   
				{  
					ss = new ServerSocket(3535);  
					System.out.println("The server is waiting your input...");  

					while(true)   
					{  
						num++;
						Socket socket = ss.accept();  
						new Thread(new ServerThread(socket),"Client "+num).start();
					}  
				} catch (IOException e) {  
					e.printStackTrace();  
				} finally{
					try{
						ss.close();
					}catch(IOException e){
						e.printStackTrace();
					}
				}
			};
		}.start();
	}

	/**
	 * @author JQJ
	 * 服务器处理客户端会话的线程
	 */
	class ServerThread implements Runnable {

		Socket socket = null;
		public ServerThread(Socket socket){
			System.out.println("Create a new ServerThread...");
			this.socket = socket;
		}

		@Override
		public void run() {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = socket.getInputStream();
				out  = socket.getOutputStream();
				//处理客户端发来的数据
				doRead(in);
				System.out.println("send Message to client.");
				//发送数据回客户端
				//				doWrite(out);
			} catch (IOException e) {
				e.printStackTrace();
			}
			finally{
				try {
					in.close();
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		/**
		 * 读取数据
		 * @param in
		 * @return
		 */
		public boolean doRead(InputStream in){
			byte[] inputByte = null;
			int length = 0;
			DataInputStream dis = null;
			FileOutputStream fos = null;
			try {
				try {
					dis = new DataInputStream(socket.getInputStream());
					String path = dir + File.separator + "jrz.png";
					fos = new FileOutputStream(new File(path));
					inputByte = new byte[1024];
					System.out.println("开始接收数据...");
					while ((length = dis.read(inputByte, 0, inputByte.length)) > 0) {
						fos.write(inputByte, 0, length);
						fos.flush();
					}
					System.out.println("完成接收");

					if(handler != null){
						//通知展示
						Message msg = new Message();
						msg.obj = path;
						handler.sendMessage(msg);
					}
				} finally {
					if (fos != null)
						fos.close();
					if (dis != null)
						dis.close();
					if (socket != null)
						socket.close();
				}
			} catch (Exception e) {
			}

			return true;
		}

		/**
		 * 写入数据
		 * @param out
		 * @return
		 */
		public boolean doWrite(OutputStream out){
			try {
				out.write("welcome you client.".getBytes());
				out.flush();		
			} catch (IOException e) {
				e.printStackTrace();
			}
			return true;
		}
	}

	public int bytesToIntByBigEndian(byte[] src, int offset) {  
		int value;    
		value = (int) ((src[offset+3] & 0xFF)   
				| ((src[offset+2] & 0xFF)<<8)   
				| ((src[offset+1] & 0xFF)<<16)   
				| ((src[offset] & 0xFF)<<24));  
		return value;  
	}
}  