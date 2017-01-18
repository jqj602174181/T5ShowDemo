package com.centerm.common;

/*
 * specialversion.ini
 * [System]
 * version=NTFY
 * ...
 * */
public class SpecialVer {
	static private String Version = null;
	static private final String EtcFilePath = "/mnt/internal_sd/config/etc/specialversion.ini"; 
	static private final String EtcSelSection = "System";
	static private final String EtcSelKey = "version";

	private SpecialVer(){}
	
	public static String getVersion()
	{
		if (Version == null)
		{
			Version = RWIniClass.ReadIniString(EtcFilePath, EtcSelSection, EtcSelKey);
			if (Version == null || Version.length() == 0)
				Version = "GEN";
		}
		return Version;			
	}
	
	public static boolean isVersion(String ver)
	{
		return getVersion().equals(ver);			
	}
	
}
