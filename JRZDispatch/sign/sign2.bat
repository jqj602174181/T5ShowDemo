java -jar .\signapk.jar -w .\platform.x509.pem  .\platform.pk8 ..\bin\JRZDispatch.apk  ..\bin\JRZDispatch2.apk
move ..\bin\JRZDispatch.apk ..\bin\JRZDispatch_old.apk 
move ..\bin\JRZDispatch2.apk ..\bin\JRZDispatch.apk