package com.centerm.util.financial;

import java.io.File;


public class DevManager{
	
	private static final String PATH_DEVICE_PARENT = "/sys/devices/";//�豸�ļ����ڵ�Ŀ¼
	private static final String NAME_UART1_SWITCH = "uart1ctrl";	//ʵ��UART1�л���ϵͳ�ļ�����
	private static final String DRIVER_UART1_SWITCH = "centerm_uart1_control";//ʵ����ģ���Դ���Ƶ�ϵͳ�ļ���Ӧ������
	private static final String NAME_ENCRYPTMODULE_POWER_CTRL = "pwrctrl";	//ʵ����ģ���Դ���Ƶ�ϵͳ�ļ�����
	private static final String DRIVER_ENCRYPTMODULE_POWER_CTRL = "centerm_enchip_ctrl";//ʵ��UART1�л���ϵͳ�ļ���Ӧ������
	private static final String NAME_ENCRYPTMODULE_MODE_CTRL = "modectrl";	//ʵ�ּ���ģ��ģʽ�����л���ϵͳ�ļ�����
	private static final String DRIVER_ENCRYPTMODULE_MODE_CTRL = "centerm_enchip_ctrl";//ʵ�ּ���ģ��ģʽ���Ƶ�ϵͳ�ļ���Ӧ������
	private static final String NAME_FINGERPRINT_LIGHT_CTRL = "fgled";	//ʵ��ָ���ǵƿ��Ƶ�ϵͳ�ļ�����
	private static final String DRIVER_FINGERPRINT_LIGHT_CTRL = "centerm_led_control";//ʵ��ָ���ǵƿ��Ƶ�ϵͳ�ļ���Ӧ������
	private static final String NAME_BLUETOOTH_LIGHT_CTRL = "btled";	//ʵ�������ƿ��Ƶ�ϵͳ�ļ�����
	private static final String DRIVER_BLUETOOTH_LIGHT_CTRL = "centerm_led_control";//ʵ�������ƿ��Ƶ�ϵͳ�ļ���Ӧ������
	 
	//����ϵͳ�ļ�·�����̶�����ʱ.26����ʱ��.27����˸��ö�̬��ȡ
	private static boolean isDeviceUpdated = false;	//�豸·���Ƿ���¹�
	private static String PATH_UART1_SWITCH = "/sys/devices/centerm_uart1_control.26/uart1ctrl";//ʵ��UART1�л���ϵͳ�ļ�·��
	private static String PATH_ENCRYPTMODULE_POWER_CTRL = "/sys/devices/centerm_enchip_ctrl.27/pwrctrl";//����ģ���Դ���Ƶ�ϵͳ�ļ�·��
	private static String PATH_ENCRYPTMODULE_MODE_CTRL = "/sys/devices/centerm_enchip_ctrl.27/modectrl";//����ģ��ģʽ���Ƶ�ϵͳ�ļ�·��
	private static String PATH_FINGERPRINT_LIGHT_CTRL = "/sys/devices/centerm_led_control.27/fgled";//ָ���ǵƿ��Ƶ�ϵͳ�ļ�·��
	private static String PATH_BLUETOOTH_LIGHT_CTRL = "/sys/devices/centerm_led_control.27/btled";//�����ƿ��Ƶ�ϵͳ�ļ�·��

	public static final int UART1BUS_DISABLED = 0;	//��ǰuart1��ckb�ͼ���оƬ������ͨ��
	public static final int UART1BUS_CKB = 1;	//��ǰ��ͨ��uart1��ckb����ͨ��
	public static final int UART1BUS_ENCRYPTMODULE = 2;	//��ǰ��ͨ��uart1�����оƬ����ͨ��
	
	/**
	 * �����������ƺ��豸���ƣ������豸ȫ·����
	 * ���������ǲ���/sys/devices/�µ��ļ������ҳ�������������Ϊǰ׺���ļ���Ȼ����׷���豸����
	 * @param driver ��������
	 * @param name �豸����
	 * @return �豸��·����
	 */
	private static String getDevicePath( String driver, String name ){
		File file = new File( PATH_DEVICE_PARENT );
		String parentname = driver;//Ĭ��ȡ��������
		if( file.isDirectory()){
			//�����豸
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
	
	//�ж��Ƿ��Ѹ��¹�����û�У����������ɸ����豸·��
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
	 * �л�UART1��CKB���ڲ���CKBģ��ǰ��������øú���
	 */
	public static boolean enableCKB(){
		updateDevicePath();
		return writeSystemFile( PATH_UART1_SWITCH, "ckb" );
	}
	
	/**
	 * �л�UART1������оƬ���ڲ�������оƬǰ��������øú���
	 */
	public static boolean enableEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_UART1_SWITCH, "key" );
	}
	
	/**
	 * �õ�����1��ǰ��ָ���ĸ�оƬͨ��
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
	 * ����ģ���ϵ�
	 */
	public static boolean powerUpEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_ENCRYPTMODULE_POWER_CTRL, "high" ); 
	}
	
	/**
	 * ����ģ���µ�
	 */
	public static boolean powerOffEncryptModule(){
		updateDevicePath();
		return writeSystemFile( PATH_ENCRYPTMODULE_POWER_CTRL, "low" );
	}
	
	/**
	 * ����ģ���Ƿ����ϵ�
	 * @return true��ʾ�ϵ磬false��ʾ�µ�
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
	 * ����ģ���������ģʽ
	 */
	public static boolean enterEncryptUpdateModule(){
		updateDevicePath();
		//���µ磬������ģʽ�����ϵ�
		if( powerOffEncryptModule()){
			sleep(20);
			if( writeSystemFile( PATH_ENCRYPTMODULE_MODE_CTRL, "low" ) ){
				sleep(20);
				return powerUpEncryptModule();
			}
			powerUpEncryptModule();//�����ϵ�
		}
		return false;
	}
	
	/**
	 * ����ģ�����Ӧ��ģʽ
	 */
	public static boolean enterEncryptAppModule(){
		updateDevicePath();
		//���µ磬������ģʽ�����ϵ�
		if( powerOffEncryptModule()){
			sleep(20);
			if( writeSystemFile( PATH_ENCRYPTMODULE_MODE_CTRL, "high" ) ){
				sleep(20);
				return powerUpEncryptModule();
			}
			powerUpEncryptModule();//�����ϵ�
		}
		return false;
	}
	
	/**
	 * ����ģ���Ƿ����Ӧ��ģʽ
	 * @return true��ʾӦ�ã�false��ʾ����ģʽ
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
	 * ָ���ǵ���
	 */
	public static boolean fingerPrintLightOn(){
		updateDevicePath();
		return writeSystemFile( PATH_FINGERPRINT_LIGHT_CTRL, "on" );
	}
	
	/**
	 * ָ���ǵư�
	 */
	public static boolean fingerPrintLightOff(){
		updateDevicePath();
		return writeSystemFile( PATH_FINGERPRINT_LIGHT_CTRL, "off" );
	}
	
	/**
	 * ��������
	 */
	public static boolean blueToothLightOn(){
		updateDevicePath();
		return writeSystemFile( PATH_BLUETOOTH_LIGHT_CTRL, "on" );
	}
	
	/**
	 * �����ư�
	 */
	public static boolean blueToothLightOff(){
		updateDevicePath();
		return writeSystemFile( PATH_BLUETOOTH_LIGHT_CTRL, "off" );
	}
	
	/**
	 * ��ϵͳ�ļ���д������
	 */
	private static boolean writeSystemFile( String path, String data ){
		return SystemFile.write(path, data);
	}
	
	
	/**
	 * ��ϵͳ�ļ��ж������ݣ�����String����
	 */
	private static String readSystemFile( String path ){
		return SystemFile.read( path );
	}
	
	/**
	 * ˯��ָ��ʱ��
	 * @param millis ����
	 */
	private static void sleep( int millis ){
		try {
			Thread.sleep(millis);
		} catch (Exception e) {
			e.printStackTrace();
			
		}
	}
}
