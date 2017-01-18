package com.centerm.common;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;

public class RWIniClass
{
	private static HashMap<String, IniEditorClass> iniEditorMap = 
			new HashMap<String,IniEditorClass>();//ini��д��ļ�¼map
	
	synchronized static IniEditorClass loadIniFile( String fileName ) 
	{
		File file = new File( fileName );
  		if (!file.exists() )
  		{
  			iniEditorMap.remove( fileName );
  			return null;
  		}
		IniEditorClass iniEditor = iniEditorMap.get( fileName );
		if( iniEditor == null )
		{
  			try
  	  		{
  				iniEditor = new IniEditorClass();
  	  			//���������ļ���Ϣ
  				iniEditor.load(fileName);
  				iniEditorMap.put( fileName, iniEditor );
  	  		} 
  	  		catch (IOException e)
  	  		{
  	  			e.printStackTrace();
  	  		}
		}
		return iniEditor;
	}
	
	//���ַ�����ʽ��ȡini�����ļ���Ϣ
  	public static String ReadIniString( String filename, String section, String key )
  	{
  	    //�����Ϸ����ж�
  		if (filename == null || section == null || key == null)
  		{
  			return null;
  		}

  		String value = null;
  		IniEditorClass iniEditor = loadIniFile(filename);//����ini
  		if( iniEditor == null )//˵���ļ�������
  		{
  			return null;
  		}
  		
  		//�ж��ֶ��Ƿ����
		if (iniEditor.hasOption(section, key) == false)
		{
			return null;
		}
		
		//��ȡ�ֶζ�Ӧ��ֵ
		value = iniEditor.get(section, key);
  		return value;
  	}
  	
  	//�ַ�����ʽдini�����ļ���Ϣ
  	public static void WriteIniString( String filename, String section, String key, String value )
  	{	
  	    //�����Ϸ����ж�
  		if (filename == null || section == null || key == null || value == null)
  		{
  			return;
  		}
  		
  		String readValue = null;
  		IniEditorClass iniEditor = loadIniFile(filename);//����ini  		
  		
  		if (iniEditor == null)//�ļ������ڣ��򴴽����ļ�
  		{	
  			try 
  		    {
	  		    //�ļ���һĿ¼�����ڣ�������Ŀ¼
	  			File file = new File(filename);
	  			if (file.getParentFile().exists() == false) 
	  			{
					file.getParentFile().mkdirs();
					Runtime.getRuntime().exec("chmod 0777 " + file.getParentFile().getAbsolutePath());
				}

  		    	file.createNewFile();
  		    	Runtime.getRuntime().exec("sync");
  		    	Runtime.getRuntime().exec("chmod 0777 " + filename);
  			} 
  		    catch (IOException e) 
  		    {
  				e.printStackTrace();
  			}
  		    iniEditor = loadIniFile(filename);//����ini
  		}
  		else//�ļ����ڣ���ֱ�Ӷ�ȡ���ж��Ƿ�ȡֵ��ͬ�������ͬ���򲻸���
  		{		
  			readValue = ReadIniString(filename, section, key);       
  			if ( readValue != null )
  			{
  				if ( readValue.equals(value) == true)
  	  			{
  	  				return;
  	  			}
  			}
  		}  		
  		
  	    //д����������
  		try
  		{
  			if( iniEditor == null )
  			{
  				System.out.println( "Error: iniEditor is null!" );
  				return;
  			}
  		    //�ж��ֶ��Ƿ����
  			if (iniEditor.hasSection(section) == false)
  			{
  				iniEditor.addSection(section);
  			}
  			
  			//���ò������ֶζ�Ӧ�Ľ�ֵ��Ϣ
  			iniEditor.set(section, key, value);
  			iniEditor.save(filename);
  			
  			Runtime.getRuntime().exec("sync");
  		} 
  		catch (IOException e)
  		{
  			e.printStackTrace();
  		}
  	}
  	
    //��������ʽ��ȡini�����ļ���Ϣ
  	public static int ReadIniInt( String filename, String section, String key, int defvalue )
  	{
  		String readValue = null;
  		readValue = ReadIniString(filename, section, key);
  		if (readValue == null)
  		{
  			return defvalue;
  		}
  		
  		//�ж��ַ���Ϊ'0'-'9'�ַ���
  		for ( int i = 0; i < readValue.length(); i++ )
  		{
  			if (i == 0)
  			{
  				if (readValue.getBytes()[i] == '-')
  				{
  					continue;
  				}
  			}
  			if ((readValue.getBytes()[i]&0xFF) < 0x30 || (readValue.getBytes()[i]&0xFF) > 0x39)
  	  		{
  	  			return defvalue;
  	  		}
  		}
  		
  		return Integer.parseInt(readValue);
  	}
  	
    //��������ʽдini�����ļ���Ϣ
  	public static void WriteIniInt( String filename, String section, String key, int value )
  	{
  		String strvalue = String.valueOf(value);
  		WriteIniString( filename, section, key, strvalue );
  	}
  	
  	public static void resetIniMap( String filename )
  	{
  		File file = new File( filename );
  		if (file.exists() )
  		{
  			iniEditorMap.remove( filename );
  		}
  	}
}