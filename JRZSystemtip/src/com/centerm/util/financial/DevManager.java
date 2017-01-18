package com.centerm.util.financial;

import java.io.File;


public class DevManager{
	
	private static final String PATH_DEVICE_PARENT = "/sys/devices/";//设备文件所在的目录
	private static final String NAME_UART1_SWITCH = "uart1ctrl";	//实现UART1切换的系统文件名称
	private static final String DRIVER_UART1_SWITCH = "centerm_uart1_control";//实现密模块电源控制的系统文件对应的驱动
	private static final String NAME_ENCRYPTMODULE_POWER_CTRL = "pwrctrl";	//实现密模块电源控制的系统文件名称
	private static final String DRIVER_ENCRYPTMODULE_POWER_CTRL = "centerm_enchip_ctrl";//实现UART1切换的系统文件对应的驱动
	private static final String NAME_ENCRYPTMODULE_MODE_CTRL = "modectrl";	//实现加密模块模式控制切换的系统文件名称
	private static final String DRIVER_ENCRYPTMODULE_MODE_CTRL = "centerm_enchip_ctrl";//实现加密模块模式控制的系统文件对应的驱动
	private static final String NAME_FINGERPRINT_LIGHT_CTRL = "fgled";	//实现指纹仪灯控制的系统文件名称
	private static final String DRIVER_FINGERPRINT_LIGHT_CTRL = "centerm_led_control";//实现指纹仪灯控制的系统文件对应的驱动
	private static final String NAME_BLUETOOTH_LIGHT_CTRL = "btled";	//实现蓝牙灯控制的系统文件名称
	private static final String DRIVER_BLUETOOTH_LIGHT_CTRL = "centerm_led_control";//实现蓝牙灯控制的系统文件对应的驱动
	 
	//由于系统文件路径不固定，有时.26，有时是.27，因此改用动态获取
	private static boolean isDeviceUpdated = false;	//设备路径是否更新过
	private static String PATH_UART1_SWITCH = "/sys/devices/centerm_uart1_control.26/uart1ctrl";//实现UART1切换的系统文件路径
	private static String PATH_ENCRYPTMODULE_POWER_CTRL = "/sys/devices/centerm_enchip_ctrl.27/pwrctrl";//加密模块电源控制的系统文件路径
	private static String PATH_ENCRYPTMODULE_MODE_CTRL = "/sys/devices/centerm_enchip_ctrl.27/modectrl";//加密模块模式控制的系统文件路径
	private static String PATH_FINGERPRINT_LIGHT_CTRL = "/sys/devices/centerm_led_control.27/fgled";//指纹仪灯控制的系统文件路径
	private static String PATH_BLUETOOTH_LIGHT_CTRL = "/sys/devices/centerm_led_control.27/btled";//蓝牙灯控制的系统文件路径

	public static final int UART1BUS_DISABLED = 0;	//当前uart1与ckb和加密芯片都不能通信
	public static final int UART1BUS_CKB = 1;	//当前可通过uart1与ckb进行通信
	public static final int UART1BUS_ENCRYPTMODULE = 2;	//当前可通过uart1与加密芯片进行通信
	
	/**
	 * 根据驱动名称和设备名称，生成设备全路径。
	 * 基本过程是查找/sys/devices/下的文件，并找出以驱动名称作为前缀的文件，然后再追加设备名称
	 * @param driver 驱动名称
	 * @param name 设备名称
	 * @return 设备合路径。
	 */
	private static String getDevicePath( String driver, String name ){
		File file = new File( PATH_DEVICE_PARENT );
		String parentname = driver;//默认取驱动名称
		if( file.isDirectory()){
			//查找设备
			String[] names = file.list();
			for( int i = 0; i < names.length; i++){
				String filename = names[i];
				if( filename.startsWith( driver )){
					parentname = filename;
					break;
				}
			}
		}
		return PATH_DEVICE_PARENT + parentname + "/" + name;				
	}
	
	//判断是否已更新过，若没有，就重新生成各个设备路径
	private static void updateDevicePath(){
		if( !isDeviceUpdated ){
			isDeviceUpdated = true;
			PATH_UART1_SWITCH = getDevicePath( DRIVER_UART1_SWITCH, NAME_UART1_SWITCH );
			PATH_ENCRYPTMODULE_POWER_CTRL = getDevicePath( DRIVER_ENCRYPTMODULE_POWER_CTRL, NAME_ENCRYPTMODULE_POWER_CTRL );
			PATH_ENCRYPTMODULE_MODE_CTRL = getDevicePath( DRIVER_ENCRYPTMODULE_MODE_CTRL, NAME_ENCRYPTMODULE_MODE_CTRL );
			PATH_FINGERPRINT_LIGHT_CTRL = getDevicePath( DRIVER_FINGERPRINT_LIGHT_CTRL, NAME_FINGERPRINT_LIGHT_CTRL );
			PATH_BLUETOOTH_LIGHT_CTRL = getDevicePath( DRIVER_BLUETOOTH_LIGHT_CTRL, NAME_BLUETOOTH_LIGHT_CTRL );
		}
	}
	

	/**
	 * 切换UART1至CKB，在操作CKB模块前，必须调用该函数
	 */
	public static boolean enableCKB(){
		updateDevicePath();
		return writeSystemFile( PATH_UART1_SWITCH, "ckb" );
	}
	
	/**
	 * 切换UART1至加密芯片，在操作加密芯片前，必须调用该函数
	 */
	public static boolean enableEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_UART1_SWITCH, "key" );
	}
	
	/**
	 * 得到串口1当前被指向哪个芯片通信
	 * @return
	 */
	public static int getUart1BusMode(){
		updateDevicePath();
		String mode = readSystemFile( PATH_UART1_SWITCH );
		if( mode.startsWith( "ckb" ))
			return UART1BUS_CKB;
		else if( mode.startsWith( "key" ) )
			return UART1BUS_ENCRYPTMODULE;
		else
			return UART1BUS_DISABLED;
	}
	
	/**
	 * 加密模块上电
	 */
	public static boolean powerUpEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_ENCRYPTMODULE_POWER_CTRL, "high" ); 
	}
	
	/**
	 * 加密模块下电
	 */
	public static boolean powerOffEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_ENCRYPTMODULE_POWER_CTRL, "low" );
	}
	
	/**
	 * 加密模块是否已上电
	 * @return true表示上电，false表示下电
	 */
	public static boolean isEncryptModulePowerOn(){
		updateDevicePath();
		String mode = readSystemFile( PATH_ENCRYPTMODULE_POWER_CTRL );
		if( mode.startsWith( "high" ))
			return true;
		else//"low"
			return false;
	}
	
	/**
	 * 加密模块进入升级模式
	 */
	public static boolean enterEncryptUpdateModule(){
		updateDevicePath();
		//先下电，再设置模式，再上电
		if( powerOffEncryptModule()){
			sleep(20);
			if( writeSystemFile( PATH_ENCRYPTMODULE_MODE_CTRL, "low" ) ){
				sleep(20);
				return powerUpEncryptModule();
			}
			powerUpEncryptModule();//重新上电
		}
		return false;
	}
	
	/**
	 * 加密模块进入应用模式
	 */
	public static boolean enterEncryptAppModule(){
		updateDevicePath();
		//先下电，再设置模式，再上电
		if( powerOffEncryptModule()){
			sleep(20);
			if( writeSystemFile( PATH_ENCRYPTMODULE_MODE_CTRL, "high" ) ){
				sleep(20);
				return powerUpEncryptModule();
			}
			powerUpEncryptModule();//重新上电
		}
		return false;
	}
	
	/**
	 * 加密模块是否进入应用模式
	 * @return true表示应用，false表示升级模式
	 */
	public static boolean isEncryptModuleInApp(){
		updateDevicePath();
		String mode = readSystemFile( PATH_ENCRYPTMODULE_MODE_CTRL );
		if( mode.startsWith( "high" ))
			return true;
		else//"low"
			return false;
	}
	
	/**
	 * 指纹仪灯亮
	 */
	public static boolean fingerPrintLightOn(){
		updateDevicePath();
		return writeSystemFile( PATH_FINGERPRINT_LIGHT_CTRL, "on" );
	}
	
	/**
	 * 指纹仪灯暗
	 */
	public static boolean fingerPrintLightOff(){
		updateDevicePath();
		return writeSystemFile( PATH_FINGERPRINT_LIGHT_CTRL, "off" );
	}
	
	/**
	 * 蓝牙灯亮
	 */
	public static boolean blueToothLightOn(){
		updateDevicePath();
		return writeSystemFile( PATH_BLUETOOTH_LIGHT_CTRL, "on" );
	}
	
	/**
	 * 蓝牙灯暗
	 */
	public static boolean blueToothLightOff(){
		updateDevicePath();
		return writeSystemFile( PATH_BLUETOOTH_LIGHT_CTRL, "off" );
	}
	
	/**
	 * 向系统文件中写入数据
	 */
	private static boolean writeSystemFile( String path, String data ){
		return SystemFile.write(path, data);
	}
	
	
	/**
	 * 从系统文件中读出数据，返回String类型
	 */
	private static String readSystemFile( String path ){
		return SystemFile.read( path );
	}
	
	/**
	 * 睡眠指定时间
	 * @param millis 毫秒
	 */
	private static void sleep( int millis ){
		try {
			Thread.sleep(millis);
		} catch (Exception e) {
			e.printStackTrace();
			
		}
	}
}
