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
		JSONStringer jsonDevInfo = new JSONStringer();	//设备信息
		jsonDevInfo.object();
		
		jsonDevInfo.key("Name");			//设备名称
		jsonDevInfo.value(SystemInfo.getDevName());
		jsonDevInfo.key("Model");			//设备型号
		jsonDevInfo.value(SystemInfo.getDevModel());
		jsonDevInfo.key("Manufacture");		//生产厂商
		jsonDevInfo.value(SystemInfo.getManufacture());
		jsonDevInfo.key("ProductionDate");	//生产日期
		jsonDevInfo.value(SystemInfo.getProductionDate());
		jsonDevInfo.key("SerialNumber");	//设备编号
		jsonDevInfo.value(DispatchApplication.serialNumber);
		jsonDevInfo.key("Hardware");		//硬件版本号
		jsonDevInfo.value(SystemInfo.getHardware());
		jsonDevInfo.key("Version");			//软件版本号
		jsonDevInfo.object();
		
		jsonDevInfo.key("firmware");		//固件信息
		
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//固件版本
		jsonDevInfo.value(SystemInfo.getBIOSVersion());
		jsonDevInfo.key("build");			//固件bulid日期
		jsonDevInfo.value(SystemInfo.getBIOSBuild());
		jsonDevInfo.endObject();
		
		jsonDevInfo.key("system");			//系统信息
		
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//系统版本
		jsonDevInfo.value(SystemInfo.getSystemVersion());
		jsonDevInfo.key("build");			//系统bulid日期
		jsonDevInfo.value(SystemInfo.getSystemBuild());
		jsonDevInfo.endObject();
		
		jsonDevInfo.key("app");				//app信息
		jsonDevInfo.object();
		jsonDevInfo.key("version");			//app版本
		jsonDevInfo.value(SystemInfo.getAppsVersion());
		jsonDevInfo.key("build");			//appbulid日期
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
