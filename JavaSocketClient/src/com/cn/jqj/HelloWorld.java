package com.cn.jqj;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.filechooser.FileFilter;

import com.cn.jqj.utils.DesUtil;
import com.cn.jqj.utils.ErrorUtil;
import com.cn.jqj.utils.MsgCreater;
import com.cn.jqj.utils.StringUtil;

public class HelloWorld{
	static Button send;
	static Button chooser;
	static JTextField jf;
	static JTextField jf2;
	static JTextField jf3;

	static String headPath = "C:/Users/Lenovo/Desktop/head.png";
	static FileInputStream is;

	public static void main(String[] args){
		JFrame frame = new JFrame("Socket Client");

		jf = new JTextField(25);
		jf.setText("测试推送数据");

		jf3 = new JTextField(15);
		jf3.setText("192.168.191.2");

		send = new Button("发送");
		send.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				if (e.getSource() == send) //发送
				{
					String content = jf.getText().toString();
					if(content.equals("")){
						return;
					}

					File file = new File(headPath);
					if(!file.exists()){
						return;
					}

					File portrai = new File("C:/Users/Lenovo/Desktop/head.png");

					try {
						String ip = jf3.getText().toString();
						if(ip.equals("")){
							ip = "192.168.191.2";
						}
						SocketAddress socketAddress = new InetSocketAddress(ip, 3535);
						Socket client = new Socket();
						client.connect(socketAddress, 10 * 1000);

						ClientThread thread = new ClientThread(client, content);
						thread.setFile(portrai, file);
						new Thread(thread).start();
					} catch (UnknownHostException e1) {
						e1.printStackTrace();
					} catch (IOException e2) {
						e2.printStackTrace();
					}
				}
			}
		});

		jf2 = new JTextField(20);
		jf2.setText("请选择图片路径");

		chooser = new Button("选择");
		chooser.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				if (e.getSource() == chooser) //选择
				{
					//文件选择器
					JFileChooser jfc=new JFileChooser();  
					jfc.setFileSelectionMode(JFileChooser.FILES_ONLY );  
					JpgFileFilter jpgFilter = new JpgFileFilter(); //jpg过滤器  
					GifFileFilter gifFilter = new GifFileFilter(); //gif过滤器  
					PngFileFilter pngFilter = new PngFileFilter(); //png过滤器
					jfc.addChoosableFileFilter(jpgFilter); //加载jpg文件过滤器  
					jfc.addChoosableFileFilter(gifFilter); //加载gif文件过滤器 
					jfc.addChoosableFileFilter(pngFilter); //加载png文件过滤器
					jfc.setFileFilter(pngFilter); 

					jfc.showDialog(new JLabel(), "选择");  
					File file=jfc.getSelectedFile();  
					if(file == null){
						return;
					}
					if(file.isDirectory()){  
						System.out.println("文件夹:"+file.getAbsolutePath());  
					}else if(file.isFile()){  
						jf2.setText(file.getAbsolutePath());
						headPath = file.getAbsolutePath();
						System.out.println("文件:"+file.getAbsolutePath());  
					}  
				}
			}
		});

		JPanel jp = new JPanel();
		jp.add(jf);

		JPanel jp2 = new JPanel();
		jp2.add(jf2);
		jp2.add(chooser);
		//		frame.add(jp2);

		JPanel jp3 = new JPanel();
		jp3.add(send);

		JPanel jp4 = new JPanel();
		jp4.add(jf3);

		frame.getContentPane().add(BorderLayout.NORTH,jp);
		frame.getContentPane().add(BorderLayout.CENTER,jp2);
		frame.getContentPane().add(BorderLayout.EAST,jp4);
		frame.getContentPane().add(BorderLayout.SOUTH,jp3);

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(600, 150);
		frame.setLocation(600, 300);
		frame.setVisible(true);
	}
}

class ClientThread implements Runnable{
	protected final String CODING = "utf-8";
	public byte work = 0x01;
	public byte type = 0x01;//信息的头两个字节

	Socket socket = null;
	String content = null;
	File portrai;
	File file;

	public ClientThread(Socket socket, String content){
		this.socket = socket;
		this.content = content;
	}

	public void setFile(File portrai, File file){
		this.portrai = portrai;
		this.file = file;
	}

	@Override
	public void run() {
		OutputStream out = null;
		InputStream in = null;
		try {
			out = socket.getOutputStream();
			in = socket.getInputStream();

			doWrite(out);
			//			doRead(in);

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
	 * 读取服务端数据
	 * @param in
	 * @return
	 */
	public boolean doRead(InputStream in){
		byte[] bytes = new byte[1024];
		try {
			in.read(bytes);
		} catch (IOException e) {
			e.printStackTrace();
		}

		return true;
	}

	public boolean sendData(OutputStream out, String jsonStr, File portrai, File file) throws Exception
	{
		byte[] portraiByte = getBytes(portrai);
		byte[] fileByte = getBytes(file);

		String tempJsonStr = "{'assets':'10万','customlevel':'普通','idcard':'350121199903040598','name':'名字啊','risklevel':'低风险','telnum':'15883659841','portraitsize':'"+portraiByte.length+"','portraitname':'portrai.png','filesize':'"+fileByte.length+"','filename':'file.png'}";

		byte[] data = tempJsonStr.getBytes(CODING);
		data = DesUtil.trides_crypt(DesUtil.KEYBYTES, data);
		String dataStr =  StringUtil.HexToStringA(data); 

		data = MsgCreater.createMsg(work,type,dataStr,portraiByte,fileByte);

		int success = sendMessage(out, data, data.length);
		if(success==ErrorUtil.ERR_OPEN){
			return false;
		}
		return true;
	}

	public byte[] getBytes(File file){  
		byte[] buffer = null;  
		try {  
			FileInputStream fis = new FileInputStream(file);  
			ByteArrayOutputStream bos = new ByteArrayOutputStream(1000);  
			byte[] b = new byte[1000];  
			int n;  
			while ((n = fis.read(b)) != -1) {  
				bos.write(b, 0, n);  
			}  
			fis.close();  
			bos.close();  
			buffer = bos.toByteArray();  
		} catch (FileNotFoundException e) {  
			e.printStackTrace();  
		} catch (IOException e) {  
			e.printStackTrace();  
		}  
		return buffer;  
	}

	private int sendMessage(OutputStream out, byte[] buf, int length) {
		if (out == null) {
			return ErrorUtil.ERR_OPEN;
		}

		try {
			out.write(buf, 0, length);
			//			System.out.println("send msg " + length + " byte");
		} catch (IOException e) {
			e.printStackTrace();
		}

		return ErrorUtil.ERR_SUCCESS;
	}

	/**
	 * 发送数据到服务端
	 * @param out
	 * @return
	 */
	public boolean doWrite(OutputStream out){

		String jsonStr = content;
		try{
			sendData(out, jsonStr, portrai, file);
		}catch(Exception e){
			e.printStackTrace();
		}

		//		try{
		//			DataOutputStream dos = new DataOutputStream(out);
		//			FileInputStream fis = new FileInputStream(file);
		//			int length = 0;
		//			byte[] sendBytes = new byte[1024];
		//			while ((length = fis.read(sendBytes, 0, sendBytes.length)) > 0) {
		//				dos.write(sendBytes, 0, length);
		//				dos.flush();
		//			}
		//			if (dos != null)
		//				dos.close();
		//			if (fis != null)
		//				fis.close();
		//		}catch (IOException e) {
		//			e.printStackTrace();
		//		} 
		return true;
	}

	public static byte[] IntToByte(int length)
	{
		byte[] byteArray = new byte[4];
		byteArray[0] = (byte)(length >>> 24);
		byteArray[1] = (byte)(length >>> 16);
		byteArray[2] = (byte)(length >>> 8);
		byteArray[3] = (byte)(length);
		return byteArray;
	}
}

class JpgFileFilter extends FileFilter {  
	public String getDescription() {  
		return "*.jpg";  
	}  

	public boolean accept(File file) {  
		String name = file.getName();  
		return name.toLowerCase().endsWith(".jpg");  
	}  
}  

class GifFileFilter extends FileFilter {  
	public String getDescription() {  
		return "*.gif";  
	}  

	public boolean accept(File file) {  
		String name = file.getName();  
		return name.toLowerCase().endsWith(".gif");  
	}  
} 

class PngFileFilter extends FileFilter {  
	public String getDescription() {  
		return "*.png";  
	}  

	public boolean accept(File file) {  
		String name = file.getName();  
		return name.toLowerCase().endsWith(".png");  
	}  
} 