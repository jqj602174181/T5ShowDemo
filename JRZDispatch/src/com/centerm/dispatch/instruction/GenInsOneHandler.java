package com.centerm.dispatch.instruction;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.nio.charset.Charset;

import android.os.RecoverySystem;
import android.os.SystemClock;
import android.util.Log;
import com.centerm.common.ByteHandlerClass;
//import com.centerm.common.GlobalConfService;
import com.centerm.common.RWIniClass;
import com.centerm.common.FileDigest;
import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.MainActivity;
import com.centerm.dispatch.common.DealFileClass;
import com.centerm.dispatch.common.DownloaderClass;
import com.centerm.dispatch.common.PlaySoundClass;
import com.centerm.dispatch.communication.ComService;
import com.centerm.dispatch.communication.ResultAcceptThread;


public final class GenInsOneHandler implements Command
{	
	//文件路径名列表
	static private final String[] filePathList = new  String[]
	{
		"/mnt/internal_sd/media/picture/",
		"/mnt/internal_sd/media/video/",
		"/mnt/internal_sd/media/operator/",
		"/mnt/internal_sd/media/sound/",
		"/mnt/internal_sd/media/data/",
		"/mnt/internal_sd/media/",
	};
	DownloaderClass downLoad = new DownloaderClass();
		
	@Override
	public byte[] executeCommand(byte[] data, Integer nLinkType) 
	{
		byte[][] tempdata = new byte[2][];
		
		Log.i(MainActivity.TAG, "GenInsOneHandler dealwith config.....................");
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		
		if( tempdata[0] != null )//函数名必须不为空
		{
			String methodname = new String(tempdata[0]);
			try			
			{
				Method method = this.getClass().getMethod( methodname , byte[].class, Integer.class );
				return (byte[])method.invoke( this, tempdata[1], Integer.valueOf(nLinkType) );//执行命令
			}
			catch( NoSuchMethodException e )
			{
				Log.w(MainActivity.TAG, this.getClass().getName() + ":Not find method " + methodname + "!");
			}				
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		
		return null;
	}
	
	//获取健值
	public byte[] getKeyValue( String key, byte[] data )
	{
		if (key == null && data == null)
		{
			return null;
		}

		 byte[][] tempdata = new byte[2][];	
		 tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'=');
		 if (tempdata[0] != null && new String(tempdata[0]).equals(key))
		 {
			 return tempdata[1];
		 }
		 
		 return null;
	}
	
	
	//设置音量
	public byte[] setVolume( byte[] data, Integer linkType )
	{
        byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] retdata = {0x02, 0x55, 0x03};
		
		byte[] type = getKeyValue("type", tempdata[0]);
		if (type != null)
		{
			byte[] size = getKeyValue("size", tempdata[1]);
			if (size != null)
			{
				int soundtype = Integer.parseInt(new String(type));
				int soundlevel = Integer.parseInt(new String(size));
				if ((soundtype == 1 && soundlevel >= 0 && soundlevel <= 5)
						|| (soundtype == 2 && soundlevel >= 0 && soundlevel <= 7)
						|| (soundtype == 3 && soundlevel >= 0 && soundlevel <= 15))
				{
					PlaySoundClass setsound = new PlaySoundClass();
					setsound.setSoundLevel(soundtype, soundlevel);
					retdata[1] = (byte)0xAA;
				}
			}	
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取音量
	public byte[] getVolume( byte[] data, Integer linkType )
	{
		byte[] retdata = null;
		byte[] type = getKeyValue("type", data);
		ComService comService = ComService.getInstance();
		if (type != null)                          
		{
			int soundtype = Integer.parseInt(new String(type));
			if (soundtype >= 1 && soundtype <= 3)
			{
				PlaySoundClass setsound = new PlaySoundClass();
				int soundlevel = setsound.getSoundLevel(soundtype);
				int len = String.valueOf(soundlevel).length();
				
				//成功返回数据： 0x02 音量等级 0x03
				retdata = new byte[ 2 + len ];
				retdata[0] = (byte)0x02;
				System.arraycopy( String.valueOf(soundlevel).getBytes(), 0, retdata, 1, len );
				retdata[len + 1] = (byte)0x03;
				
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				return null;
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	public byte[] uploadFile( byte[] data, Integer linkType )
	{
		//ComService comService = ComService.getInstance();
		byte[] retdata = null;
		
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] path = getKeyValue("path", tempdata[0]);
		if (path != null) 
		{
			String filePath = new String(path);
			if (DealFileClass.isFileExist(filePath))
			{
				tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(tempdata[1], (byte)'|');
				byte[] seeklen = getKeyValue("seeklen", tempdata[0]);
				if (seeklen != null)
				{
					byte[] filelen = getKeyValue("filelen", tempdata[1]);
					if (filelen != null) 
					{
						long lseeklen = Long.parseLong(new String(seeklen));
						long lfilelen = Long.parseLong(new String(filelen));
						byte[] fileData = null;
						try {
							RandomAccessFile rf = new RandomAccessFile(filePath, "rw");
							if (lseeklen + lfilelen <= rf.length())
							{
								fileData = new byte[Integer.parseInt(new String(filelen))];
								rf.seek(lseeklen);
								rf.read(fileData, 0, Integer.parseInt(new String(filelen)));
								rf.close();
								
								//retdata = new byte[fileData.length+2];
								//retdata[0] = (byte)0x02;
								//System.arraycopy(fileData, 0, retdata, 1, fileData.length);
								//retdata[fileData.length+1] = (byte)0x03;
								//comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
								ResultAcceptThread.getInstance().sendData(fileData, linkType);
								return null;
							}
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				}
			}
		}
		
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		ResultAcceptThread.getInstance().sendData(retdata, linkType);
		return null;
	}
	//文件下载
	public byte[] dwnloadFile( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] filetype = getKeyValue("filetype", tempdata[0]);
		if (filetype != null)
		{
			int fileindex = Integer.parseInt(new String(filetype));

			if ( fileindex >= 0 && fileindex <= 5 )
			{
				byte[] databuf = tempdata[1].clone();
				tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(databuf, (byte)'|');
				byte[] filename = getKeyValue("filename", tempdata[0]);
				if (filename != null)
				{
					byte[] datapackage = getKeyValue("datapackage", tempdata[1]);
					if (datapackage != null)
					{	
						try 
						{
							String pathname = filePathList[fileindex] + new String(filename, "GBK");
							if (downLoad.saveFile(pathname, new String(filename, "GBK"), datapackage))
							{
								retdata[1] = (byte)0xAA;
							}
						} 
						catch (UnsupportedEncodingException e) 
						{
							e.printStackTrace();
						}
					}
				}
			}
		}
		
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//文件删除
	public byte[] deleteFile( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] filetype = getKeyValue("filetype", tempdata[0]);
		if (filetype != null)
		{
			int fileindex = Integer.parseInt(new String(filetype));
			
			if ( fileindex >= 0 && fileindex <= 4 )
			{
				byte[] filename = getKeyValue("filename", tempdata[1]);
				if (filename != null)
				{
					if(new String(filename).equals("*"))//清空目录
					{
						DealFileClass.cleanDirectory( filePathList[fileindex] );
						retdata[1] = (byte)0xAA;
					}
					else
					{
						try 
						{
							String pathname = filePathList[fileindex] + new String(filename, "GBK");
							if (DealFileClass.isFileExist(pathname) == true)
							{
								DealFileClass.deleteFile(pathname);
							}
							retdata[1] = (byte)0xAA;
						} 
						catch (UnsupportedEncodingException e) 
						{
							e.printStackTrace();
						}
					}
				}
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//查询文件
	public byte[] queryFile( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] filetype = getKeyValue("filetype", tempdata[0]);
		if (filetype != null)
		{
			int fileindex = Integer.parseInt(new String(filetype));
			if ( fileindex >= 0 && fileindex <= 4 )
			{
				byte[] filename = getKeyValue("fileinfo", tempdata[1]);
				if (filename != null)
				{
					try 
					{
						String pathname = filePathList[fileindex] + new String(filename, "GBK");
						if (DealFileClass.isFileExist(pathname) == true)
						{
							String strLen = String.format("%d", DealFileClass.getFileSize(pathname));
							String strMD5 = FileDigest.getFileMD5(pathname);
							String strFileInfo = strLen + "|" + strMD5;
							retdata = new byte[strFileInfo.length() + 2];
							retdata[0] = (byte)0x02;
							System.arraycopy(strFileInfo.getBytes(), 0, retdata, 1, strFileInfo.length());
							retdata[strFileInfo.length() + 1] = (byte)0x03;
						}
				        }
					catch (UnsupportedEncodingException e) 
					{
						e.printStackTrace();
					}
					
				}
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//查询文件
	public byte[] queryFileCommon( byte[] data, Integer linkType )
	{
		byte[] retdata = "BERF".getBytes();
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] filetype = getKeyValue("filetype", tempdata[0]);
		if (filetype != null)
		{
			int fileindex = Integer.parseInt(new String(filetype));
			if ( fileindex >= 0 && fileindex <= 4 )
			{
				byte[] filename = getKeyValue("fileinfo", tempdata[1]);
				if (filename != null)
				{
					try 
					{
						String pathname = filePathList[fileindex] + new String(filename, "GBK");
						if (DealFileClass.isFileExist(pathname) == true)
						{
							retdata[1] = (byte)0x4F;
							retdata[2] = (byte)0x4B;
						}
					} 
					catch (UnsupportedEncodingException e) 
					{
						e.printStackTrace();
					}
					
				}
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取文件列表
	public byte[] getFileList( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;//返回给主机的数据
		byte[] filetype = getKeyValue("filetype", data);
		if (filetype != null)
		{
			int fileindex = Integer.parseInt(new String(filetype));
			if ( fileindex >= 0 && fileindex <= 4 )
			{
				//得到文件列表，然后生成文件列表格式
				String path = filePathList[fileindex];
				File file = new File( path );
				String[] fileList = file.list();					
				
				if( fileList != null )//不为空，说明文件是一个有效的文件夹
				{
					StringBuffer buffer = new StringBuffer();
					for( int i = 0; i < fileList.length; i++ )
					{
						if( i != 0) 
						{
							buffer.append('|');//分配分隔符
						}
						buffer.append( fileList[i] );
					}
					byte[] fileListData = buffer.toString().getBytes(Charset.forName("GBK"));
					
					//成功返回数据： 0x02 文件1|文件2|文件3 0x03
					if (fileListData != null)
					{
						retdata = new byte[ 2 + fileListData.length ];
						retdata[0] = (byte)0x02;
						System.arraycopy( fileListData, 0, retdata, 1, fileListData.length );
						retdata[fileListData.length + 1] = (byte)0x03;
						comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
						return null;
					}
				}
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//设置串口参数
	public byte[] setComAttr( byte[] data, Integer linkType )
	{
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] port = getKeyValue("port", tempdata[0]);
		if (port != null)
		{
			int portnum = Integer.parseInt(new String(port));
			
			//只支持串口号为1和2的属性修改
			if ( portnum >= 1 && portnum <= 2 )
			{
				String [] strBaud = { "1200", "2400", "4800", "9600", "19200", "38400", "115200" };
				byte[] baud = getKeyValue("baud", tempdata[1]);
				if (baud != null)
				{
					for (int i = 0; i < strBaud.length; i++)
					{
						//判断波特率有效性，保存波特率设置
						if (new String(baud).equals(strBaud[i]))
						{
							String section = "COM" + portnum;
							RWIniClass.WriteIniString("/mnt/internal_sd/config/system.ini", section, "baud", strBaud[i]);
							retdata[1] = (byte)0xAA;
							break;
						}
					}
				}
			}
		}
		ComService comService = ComService.getInstance();
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取串口参数
	public byte[] getComAttr( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		byte[] port = getKeyValue("port", data);
		if (port != null)
		{
			int portnum = Integer.parseInt(new String(port));
			
			//只支持串口号为1和2的属性修改
			if ( portnum >= 1 && portnum <= 2 )
			{
				String section = "COM" + portnum;
				String baud = RWIniClass.ReadIniString("/mnt/internal_sd/config/system.ini", section, "baud");
                if (baud != null)
                {
                	retdata = new byte[ 2 + baud.getBytes().length ];
					retdata[0] = (byte)0x02;
					System.arraycopy( baud.getBytes(), 0, retdata, 1, baud.getBytes().length );
					retdata[baud.getBytes().length + 1] = (byte)0x03;
					comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
					return null;
                }
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//设置全局配置
	public byte[] setGlobalConfig( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "setGlobalConfig");
		ComService comService = ComService.getInstance();
		byte[] retdata = {0x02, 0x55, 0x03};
		byte[][] tempdata = new byte[2][];	
		tempdata = ByteHandlerClass.getByteArrayBeforeSeperator(data, (byte)'|');
		byte[] type = getKeyValue("type", tempdata[0]);
		if (type != null)
		{
			int index = Integer.parseInt(new String(type));

            //取值范围：0x31-0x35；0x31表示系统模式；0x32表示自毁标识；0x33表示开关机标识；0x34表示自毁恢复标识；0x35表示启动参数；
			//0x36表示设备号;0x37表示生产日期;0x38表示设备名
			if ( index >= 1 && index <= 8 )
			{	
				byte[] value = getKeyValue("data", tempdata[1]);
				if (value != null)
				{
					switch(index)
					{
					    case 1:
					    	byte mode = (byte)(Integer.parseInt(new String(value)));
					    	if (mode == 0 || mode == 0x01)
					    	{
					    	/*	if (mode == 0x01)
						    	{
						    		DealFileClass.setSystemInfo("com.centerm.cashlisten", "enable");				    		
						    	}
						    	else if (mode == 0)
						    	{
						    		DealFileClass.setSystemInfo("com.centerm.cashlisten", "disable");
						    	}*/
					    		//GlobalConfService.comService.setSystemMode((byte)(mode+0x30));
						    	retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 2:
					    	byte destoryflag = (byte)(Integer.parseInt(new String(value)));
					    	if (destoryflag == 0x00 || destoryflag == 0x01)
					    	{
					    		//GlobalConfService.comService.setDestoryFlag(destoryflag);
					    		retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 3:
					    	byte autosc = (byte)(Integer.parseInt(new String(value)));
					    	if (autosc == 0x00 || autosc == 0x01)
					    	{
					    		//GlobalConfService.setCloseAutoSC(autosc);
					    		retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 4:
					    	byte recoveryflag = (byte)(Integer.parseInt(new String(value)));
					    	if (recoveryflag == 0x00 || recoveryflag == 0x01)
					    	{
					    		//GlobalConfService.comService.setRecoveryFlag(value[0]);
					    		retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 5:
					    	if (value.length <= 256)
					    	{
					    		//GlobalConfService.setCmdLine(value);
						    	retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 6:	// 设置序列号(不对外提供)
					    	if (value.length <= 32)
					    	{
					    		//GlobalConfService.comService.setDevSN(value, value.length);
						    	retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 7:	// 设置生产日期(不对外提供)
					    	if (value.length <= 8)
					    	{
					    		//GlobalConfService.comService.setDevDate(value);
						    	retdata[1] = (byte)0xAA;
					    	}
					    	break;
					    case 8:	// 设置设备名(不对外提供)
					    	if (value.length <= 16)
					    	{
					    		//GlobalConfService.comService.setDevName(value, value.length);
						    	retdata[1] = (byte)0xAA;
					    	}
					    	break;
					}
					
				}
			}
		}
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//获取全局配置
	@SuppressWarnings("unused")
	public byte[] getGlobalConfig( byte[] data, Integer linkType )
	{
		Log.i(MainActivity.TAG, "getGlobalConfig");
		ComService comService = ComService.getInstance();
		byte[] retdata = null;
		byte[] type = getKeyValue("type", data);
		if (type != null)
		{
			int index = Integer.parseInt(new String(type));
			
			//取值范围：0x31-0x35；0x31表示系统模式；0x32表示自毁标识；0x33表示开关机标识；0x34表示自毁恢复标识；0x35表示启动参数；
			//0x36表示设备号;0x37表示生产日期;0x38表示设备名
			if ( index >= 1 && index <= 8 )
			{
				switch(index)
				{
				    case 1:
				    	retdata = new byte[3];
				    	retdata[0] = (byte)0x02;
				    	retdata[2] = (byte)0x03;
				    	//retdata[1] = (byte)(GlobalConfService.comService.getSystemMode()-0x30);
				    	break;
				    case 2:
				    	retdata = new byte[3];
				    	retdata[0] = (byte)0x02;
				    	retdata[2] = (byte)0x03;
				    	//retdata[1] = GlobalConfService.comService.getDestoryFlag();
				    	break;
				    case 3:
				    	retdata = new byte[3];
				    	retdata[0] = (byte)0x02;
				    	retdata[2] = (byte)0x03;
				    	//retdata[1] = GlobalConfService.getCloseAutoSC();
				    	break;
				    case 4:
				    	retdata = new byte[3];
				    	retdata[0] = (byte)0x02;
				    	retdata[2] = (byte)0x03;
				    	//retdata[1] = GlobalConfService.comService.getRecoveryFlag();
				    	break;
				    case 5:				    	
				    {
				    	//byte[] value = GlobalConfService.getCmdLine();
				    	byte[] value = null;
				    	if (value != null)
				    	{
				    		retdata = new byte[ 2 + value.length ];
					    	retdata[0] = (byte)0x02;
					    	System.arraycopy( value, 0, retdata, 1, value.length );
							retdata[value.length + 1] = (byte)0x03;
				    	}
				    	else
				    	{
				    		retdata = new byte[2];
				    		retdata[0] = (byte)0x02;
				    		retdata[1] = (byte)0x03;
				    	}
				    	
				    	break;
				    }
				    case 6:		//获取序列号
				    {
				    	/*byte[] value = GlobalConfService.comService.getDevSN();
				    	Log.i(MainActivity.TAG, new String(value));
				    	if (value != null)
				    	{
				    		retdata = new byte[ 2 + value.length ];
					    	retdata[0] = (byte)0x02;
					    	System.arraycopy( value, 0, retdata, 1, value.length );
							retdata[value.length + 1] = (byte)0x03;
				    	}
				    	else
				    	{
				    		retdata = new byte[2];
				    		retdata[0] = (byte)0x02;
				    		retdata[1] = (byte)0x03;
				    	}*/
				    	
				    	break;
				    }
				    case 7:		//获取生产日期
				    {
				    	/*byte[] value = GlobalConfService.comService.getDevDate();
				    	Log.i(MainActivity.TAG, new String(value));
				    	if (value != null)
				    	{
				    		retdata = new byte[ 2 + value.length ];
					    	retdata[0] = (byte)0x02;
					    	System.arraycopy( value, 0, retdata, 1, value.length );
							retdata[value.length + 1] = (byte)0x03;
				    	}
				    	else
				    	{
				    		retdata = new byte[2];
				    		retdata[0] = (byte)0x02;
				    		retdata[1] = (byte)0x03;
				    	}*/
				    	
				    	break;
				    }
				    case 8:		//获取设备名
				    {
				    	/*byte[] value = GlobalConfService.comService.getDevName();
				    	Log.i(MainActivity.TAG, new String(value));
				    	if (value != null)
				    	{
				    		retdata = new byte[ 2 + value.length ];
					    	retdata[0] = (byte)0x02;
					    	System.arraycopy( value, 0, retdata, 1, value.length );
							retdata[value.length + 1] = (byte)0x03;
				    	}
				    	else
				    	{
				    		retdata = new byte[2];
				    		retdata[0] = (byte)0x02;
				    		retdata[1] = (byte)0x03;
				    	}*/
				    	
				    	break;
				    }
				}
				comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
				return null;
			}
		}
		retdata = new byte[3];
		retdata[0] = 0x02;
		retdata[1] = 0x55;
		retdata[2] = 0x03;
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}
	
	//远程升级
	public byte[] remoteUpdate( byte[] data, Integer linkType )
	{
		ComService comService = ComService.getInstance();
		byte[] retdata = {0x02, 0x55, 0x03};
		
		byte[] path = getKeyValue("path", data);
		if (path != null)
		{
			String filePath = new String(path);
			File updateFile = new File(filePath);
			if (updateFile.exists())
			{
				try 
				{
					retdata[1] = (byte)0xAA;
					comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
					SystemClock.sleep(500);
					RecoverySystem.installPackage(DispatchApplication.getProcessContext(), updateFile);
		            return null;
				} 
				catch (IOException e) 
				{
		            e.printStackTrace();
		        }
			}
		}
		comService.sendResultToJNI( linkType.intValue(), retdata, retdata.length );
		return null;
	}	
}
