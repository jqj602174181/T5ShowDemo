java -jar .\signapk.jar -w .\platform.x509.pem  .\platform.pk8 ..\bin\DispatchNormal.apk  ..\bin\Dispatch.apk

del ..\bin\DispatchNormal.apk

adb install -r ..\bin\Dispatch.apk
pause

adb reboot
