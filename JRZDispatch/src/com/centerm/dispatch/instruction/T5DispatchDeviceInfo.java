package com.centerm.dispatch.instruction;

import org.json.JSONException;
import org.json.JSONStringer;

import com.centerm.common.DevInfo;
import com.centerm.common.SystemInfo;
import com.centerm.dispatch.DispatchApplication;
import com.centerm.dispatch.communication.BTAcceptThread;
//com.centerm.dispatch.instruction.DeviceInfo
public class T5DispatchDeviceInfo {

	public static  String getDeviceInfo() throws JSONException
	{
		JSONStringer jsonDevInfo = new JSONStringer();	//�豸��Ϣ
		jsonDevInfo.object();
		
		jsonDevInfo.key("Name");			//�豸����
		jsonDevInfo.value(SystemInfo.getDevName());
		jsonDevInfo.key("Model");			//�豸�ͺ�
		jsonDevInfo.value(SystemInfo.getDevModel());
		jsonDevInfo.key("Manufacture");		//��������
		jsonDevInfo.value(SystemInfo.getManufacture());
		jsonDevInfo.key("ProductionDate");	//��������
		jsonDevInfo.value(SystemInfo.getProductionDate());
		jsonDevInfo.key("SerialNumber");	//�豸���
		jsonDevInfo.value(DispatchApplication.serialNumber);
		jsonDevInfo.key("Hardware");		//Ӳ���汾��
		jsonDevInfo.value(SystemInfo.getHardware());
		jsonDevInfo.key("Version");			//����汾��
		jsonDevInfo.object();
		
		jsonDevInfo.key("firmware");		//�̼���Ϣ
		
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//�̼��汾
		jsonDevInfo.value(SystemInfo.getBIOSVersion());
		jsonDevInfo.key("build");			//�̼�bulid����
		jsonDevInfo.value(SystemInfo.getBIOSBuild());
		jsonDevInfo.endObject();
		
		jsonDevInfo.key("system");			//ϵͳ��Ϣ
		
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//ϵͳ�汾
		jsonDevInfo.value(SystemInfo.getSystemVersion());
		jsonDevInfo.key("build");			//ϵͳbulid����
		jsonDevInfo.value(SystemInfo.getSystemBuild());
		jsonDevInfo.endObject();
		
		jsonDevInfo.key("app");				//app��Ϣ
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//app�汾
		jsonDevInfo.value(SystemInfo.getAppsVersion());
		jsonDevInfo.key("build");			//appbulid����
		jsonDevInfo.value(SystemInfo.getAppsBuild());
		

		jsonDevInfo.endObject();
		jsonDevInfo.endObject();
		jsonDevInfo.endObject();
		
		return jsonDevInfo.toString();
	}
	
	
	public void setDeviceNo(String serialNo)
	{
		DevInfo.setSerialNo(serialNo);
	}
	public static T5DispatchDeviceInfo initDeviceInfo(String className)
	{
		Class<?> c = null;

		T5DispatchDeviceInfo deviceInfo = null;
		try {
			c = Class.forName(className);

			deviceInfo = (T5DispatchDeviceInfo) c.newInstance();
			
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}  catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return deviceInfo;
	
	}
}
