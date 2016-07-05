#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "windows.h"
#include <io.h>
#include "GlobalDef.h"
#include "Debug.h"
#include "WindowsHid.h"
#include "setupapi.h"
#include "hidsdi.h"


#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

/***********************************/
#define HID_IN_REPORT_SIZE      64
#define HID_OUT_REPORT_SIZE     64
/***********************************/

static HANDLE g_hHid = NULL;


static S32 GetDataWithoutZero( S8* szInData, int nInData, S8* szOutData )
{
    S32 i = 0;
    S32 nCount = 0;
    
    for( i=0; i < nInData; i++ )
    {
        if( 0x0 != szInData[ i ] )
        {
            break;
        }
    }

    for( ; i < nInData; i++ )
    {
        if( 0x0 == szInData[i] )
        {
            break;
        }
        szOutData[ nCount++ ] = szInData[i];
    }

    return nCount;
}

static HANDLE  DeviceOpen(DWORD wVID,DWORD wPID)
{
    HANDLE MyHIDDevHandle = NULL;
    //����һ��GUID�Ľṹ��HidGuid������HID�豸�Ľӿ���GUID��
    GUID HidGuid;
    //����һ��DEVINFO�ľ��hDevInfoSet�������ȡ�����豸��Ϣ���Ͼ����
    HDEVINFO hDevInfoSet;
    //����MemberIndex����ʾ��ǰ�������ڼ����豸��0��ʾ��һ���豸��
    DWORD MemberIndex;
    //DevInterfaceData�����������豸�������ӿ���Ϣ
    SP_DEVICE_INTERFACE_DATA DevInterfaceData;
    //����һ��BOOL���������溯�������Ƿ񷵻سɹ�
    BOOL Result;
    //����һ��RequiredSize�ı���������������Ҫ������ϸ��Ϣ�Ļ��峤�ȡ�
    DWORD RequiredSize;
    //����һ��ָ���豸��ϸ��Ϣ�Ľṹ��ָ�롣
    PSP_DEVICE_INTERFACE_DETAIL_DATA    pDevDetailData = NULL;
    //����һ������������豸�ľ����
    HANDLE hDevHandle = NULL;
    //����һ��HIDD_ATTRIBUTES�Ľṹ������������豸�����ԡ�
    HIDD_ATTRIBUTES DevAttributes;

    //��DevInterfaceData�ṹ���cbSize��ʼ��Ϊ�ṹ���С
    DevInterfaceData.cbSize=sizeof(DevInterfaceData);
    //��DevAttributes�ṹ���Size��ʼ��Ϊ�ṹ���С
    DevAttributes.Size=sizeof(DevAttributes);

    //����HidD_GetHidGuid������ȡHID�豸��GUID����������HidGuid�С�
    HidD_GetHidGuid(&HidGuid);

    //����HidGuid����ȡ�豸��Ϣ���ϡ�����Flags��������Ϊ
    //DIGCF_DEVICEINTERFACE|DIGCF_PRESENT��ǰ�߱�ʾʹ�õ�GUIDΪ
    //�ӿ���GUID�����߱�ʾֻ�о�����ʹ�õ��豸����Ϊ��������ֻ
    //�����Ѿ������ϵ��豸�����صľ��������hDevinfo�С�ע���豸
    //��Ϣ������ʹ����Ϻ�Ҫʹ�ú���SetupDiDestroyDeviceInfoList
    //���٣���Ȼ������ڴ�й©��
    hDevInfoSet=SetupDiGetClassDevs(&HidGuid,
        NULL,
        NULL,
        DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
    if( !hDevInfoSet )
    {
        return NULL;
    }
    //Ȼ����豸������ÿ���豸�����о٣�����Ƿ�������Ҫ�ҵ��豸
    //���ҵ�����ָ�����豸�������豸�Ѿ��������ʱ�����˳����ҡ�
    //����ָ���һ���豸������MemberIndex��Ϊ0��
    MemberIndex=0;
    SetLastError(NO_ERROR);
    while(1)
    {
        //����SetupDiEnumDeviceInterfaces���豸��Ϣ�����л�ȡ���ΪMemberIndex���豸��Ϣ��
        Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
            NULL,
            &HidGuid,
            MemberIndex,
            &DevInterfaceData);
        //�����ȡ��Ϣʧ�ܣ���˵���豸�Ѿ�������ϣ��˳�ѭ����
        if(Result==FALSE)
        {
            MyHIDDevHandle = NULL;
            break;
        }
        //��MemberIndexָ����һ���豸
        MemberIndex++;

        //�����ȡ��Ϣ�ɹ����������ȡ���豸����ϸ��Ϣ���ڻ�ȡ�豸
        //��ϸ��Ϣʱ����Ҫ��֪��������ϸ��Ϣ��Ҫ���Ļ���������ͨ��
        //��һ�ε��ú���SetupDiGetDeviceInterfaceDetail����ȡ����ʱ
        //�ṩ�������ͳ��ȶ�ΪNULL�Ĳ��������ṩһ������������Ҫ���
        //�������ı���RequiredSize��
        Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
            &DevInterfaceData,
            NULL,
            0,
            &RequiredSize,
            NULL);

        //Ȼ�󣬷���һ����СΪRequiredSize�����������������豸��ϸ��Ϣ��
        if ( pDevDetailData != NULL )
        {
            free(pDevDetailData);
            pDevDetailData = NULL;
        }
        pDevDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
        if(pDevDetailData==NULL) //����ڴ治�㣬��ֱ�ӷ��ء�
        {

            MyHIDDevHandle = NULL;
            break;
        }

        //������pDevDetailData��cbSizeΪ�ṹ��Ĵ�С��ע��ֻ�ǽṹ���С�����������滺��������
        pDevDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        //Ȼ���ٴε���SetupDiGetDeviceInterfaceDetail��������ȡ�豸��
        //��ϸ��Ϣ����ε�������ʹ�õĻ������Լ���������С��
        Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
            &DevInterfaceData,
            pDevDetailData,
            RequiredSize,
            NULL,
            NULL);      

        //�������ʧ�ܣ��������һ���豸��
        if(Result==FALSE) 
        {
            continue;
        }

        //������óɹ�����ʹ�ò�����д���ʵ�CreateFile��������ȡ�豸�����ԣ�����VID��PID���汾�ŵȡ�
        //����һЩ��ռ�豸������USB���̣���ʹ�ö����ʷ�ʽ���޷��򿪵ģ�
        //��ʹ�ò�����д���ʵĸ�ʽ�ſ��Դ���Щ�豸���Ӷ���ȡ�豸�����ԡ�
        hDevHandle=CreateFile(pDevDetailData->DevicePath, 
            GENERIC_READ|GENERIC_WRITE,//NULL
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,//FILE_ATTRIBUTE_NORMAL,
            NULL);

        //����򿪳ɹ������ȡ�豸���ԡ�
        if(hDevHandle!=NULL)
        {
            //��ȡ�豸�����Բ�������DevAttributes�ṹ����
            Result=HidD_GetAttributes(hDevHandle, &DevAttributes);

            //�رոոմ򿪵��豸
            CloseHandle(hDevHandle);

            //��ȡʧ�ܣ�������һ��
            if(Result==FALSE) 
            {
                continue;
            }

            //debugPrintf("VID=%04X,PID=%04X \n",DevAttributes.VendorID,DevAttributes.ProductID);
            //�����ȡ�ɹ����������е�VID��PID�Լ��豸�汾����������Ҫ��
            //���бȽϣ������һ�µĻ�����˵������������Ҫ�ҵ��豸��
            if((DevAttributes.VendorID==wVID) && (DevAttributes.ProductID==wPID)) //���VID��Ȳ���PID���
            {
                //debugPrintf("find device now \n");
                //if(DevAttributes.VersionNumber==MyPvn) //����豸�汾�����
                {
                       //��ô��������Ҫ�ҵ��豸�����첽���ʵķ�ʽ�򿪡�
                      MyHIDDevHandle=CreateFile(pDevDetailData->DevicePath, 
                        GENERIC_READ|GENERIC_WRITE,
                        //�������Ϊ��ʱ��Ϊ��ռʽ modify by laiwenhui R0001
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                        NULL);

                    if(MyHIDDevHandle!=INVALID_HANDLE_VALUE)
                    {
                        //debugPrintf("���豸�ɹ� \n");
                    }
                    else
                    {
                       //debugPrintf("���豸ʧ�� \n");
                       MyHIDDevHandle = NULL;
                    }
                    break;
                }
            }
        }
        else//�����ʧ�ܣ��������һ���豸
        {
            continue;
        }
    }//end while(1)

    //����SetupDiDestroyDeviceInfoList���������豸��Ϣ����
    SetupDiDestroyDeviceInfoList(hDevInfoSet);
    if ( pDevDetailData )
    {
        free(pDevDetailData);
        pDevDetailData = NULL;
    }
    return MyHIDDevHandle;
}

static BOOL DeviceWrite(HANDLE handle, BYTE *lpBuffer, DWORD dwSize)
{
    BYTE wBuffer[HID_OUT_REPORT_SIZE+1];
    OVERLAPPED WriteOverlapped;
    DWORD dwRet;
    BOOL bRetCode;
    DWORD i;

    if( !GetHandleInformation( handle, &dwRet) )
    {
        //debugPrintf("call GetHandleInformation here\n");
        return FALSE;
    }

    // ���
    HidD_FlushQueue(handle);

    WriteOverlapped.Offset=0;
    WriteOverlapped.OffsetHigh=0;
    WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

    for(i=0; i<dwSize; i=i+HID_OUT_REPORT_SIZE)
    {       
        memset(wBuffer, 0, sizeof(wBuffer));
        //��Ҫ���͵�����ǰ����ϱ���ID
        
        wBuffer[0] = 0x00;
        if( dwSize >= HID_OUT_REPORT_SIZE )
        {
             memcpy((wBuffer+1), lpBuffer+i, HID_OUT_REPORT_SIZE);
        }
        else
        {
            memcpy((wBuffer+1), lpBuffer+i, dwSize );
        }

        //д����
        bRetCode = WriteFile(handle, wBuffer, HID_OUT_REPORT_SIZE+1, &dwRet, &WriteOverlapped);
        //������ش��󣬲��Ҳ����̱߳�����
        if((bRetCode==FALSE) && (GetLastError()!=ERROR_IO_PENDING))
        {
            //debugPrintf("write %d report err! err no = %d ",i, GetLastError());
            return FALSE;
        }
        else
        {
            //�ȴ��¼��źţ�������ϻ�ʱ��
            WaitForSingleObject(WriteOverlapped.hEvent,1000);
            //�����¼�Ϊ���ź�
            ResetEvent(WriteOverlapped.hEvent);
            bRetCode = TRUE;
        }
    }

    if( dwSize > i )
    {
        memset(wBuffer, 0, sizeof(wBuffer));
        //��Ҫ���͵�����ǰ����ϱ���ID
        wBuffer[0] = 0x00;
        memcpy((wBuffer+1), lpBuffer+i, dwSize-i);
        //д����
        bRetCode = WriteFile(handle, wBuffer, HID_OUT_REPORT_SIZE+1, &dwRet, &WriteOverlapped);
        //������ش��󣬲��Ҳ����̱߳�����
        if((bRetCode==FALSE) && (GetLastError()!=ERROR_IO_PENDING))
        {
            return FALSE;
        }
        else
        {
            //�ȴ��¼��źţ�������ϻ�ʱ��
            WaitForSingleObject(WriteOverlapped.hEvent,1000);
            //�����¼�Ϊ���ź�
            ResetEvent(WriteOverlapped.hEvent);
            bRetCode = TRUE;
        }
    }

    return bRetCode;
}

static BOOL DeviceReadHid(HANDLE handle, BYTE *lpBuffer, DWORD *ulDataLeng, DWORD dwTimeOut )
{
    BYTE rBuffer[HID_IN_REPORT_SIZE+1] = {0};
    OVERLAPPED ReadOverlapped;
    DWORD dwRet;
    BOOL bRetCode;
    DWORD readLeng = 0;
    DWORD ulCont = 0;
    DWORD Result = 0;
    long nBegTime = 0;
    long nEndTime = 0;
    
    ReadOverlapped.Offset=0;
    ReadOverlapped.OffsetHigh=0;
    ReadOverlapped.hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);    
    memset(rBuffer, 0, sizeof(rBuffer));
    //������
    bRetCode = ReadFile(handle, rBuffer, HID_IN_REPORT_SIZE+1, &dwRet, &ReadOverlapped);
    
    nBegTime = GetTickCount();
    while(1)
    {
        nEndTime=GetTickCount();
        if((nEndTime-nBegTime)>dwTimeOut*1000)//R30001 modified by quexinsheng �޸�HID��ʱ��Чbug
        {
            CancelIo(handle);
            return FALSE;
        }
        
        Result=WaitForSingleObject(ReadOverlapped.hEvent,1000);
        if(Result==WAIT_OBJECT_0)//�糬ʱ������ȡ������
        {
            break;
        }         
    }
    //�����������ݸ��Ƶ�����buffer
    memcpy(lpBuffer, (rBuffer+1), HID_IN_REPORT_SIZE);
    *ulDataLeng = 64;
    return TRUE;
    
}
static BOOL DeviceClose( HANDLE handle )
{
    if( NULL != handle )
    {
        CloseHandle( handle );
    }
    return TRUE;
}

static BOOL DeviceRead(HANDLE handle, BYTE *lpBuffer,  DWORD dwBufferLen, int *ulDataLeng, DWORD dwTimeOut, EndCondition pfReadEnd )
{
       BYTE szBuf[ MAX_LEN ] = {0};
       int nBufLen = 0;
	   DWORD dwCount = dwTimeOut > 0 ? dwTimeOut : 5;
       int nLen  = 0;
       
       do 
       {
             memset( szBuf, 0x0, sizeof(szBuf) );          
             if( FALSE ==  DeviceReadHid( handle, szBuf, &nBufLen, 1 ) )
             {
                 dwCount --;
             }
             else
             {
                   if( nBufLen )
                   {
                       nBufLen = GetDataWithoutZero( szBuf, nBufLen, lpBuffer+nLen );       
                       if( nBufLen <= 0 )
                       {
                            dwCount --;
                            if( dwCount > 0 )
                            {
                                continue;
                            }
							else
							{
								break;
							}
                       }

                        nLen += nBufLen;
                 
                        if( pfReadEnd && !pfReadEnd( lpBuffer, nLen ) )
                        {                        
                            break;   
                        }

                   }
                  
             }
       } 
       while(  dwCount );

       if( 0 == nLen )
       {
            return FALSE;
       }
       
       *ulDataLeng  =  nLen;
       return TRUE;

}


S32 ICC_OpenWindowsHid( const S32 nVid, const S32 nPid )
{
    S32 nRetVal = RET_SUCCESS;

    if( !g_hHid )
    {

        g_hHid  =  DeviceOpen( nVid,  nPid ) ;
        if( !g_hHid )  
         {
             nRetVal = RET_DEVICE_UNOPEN;
         }
    }
    
    return nRetVal;
}

S32 ICC_CloseWindowsHid( void )
{
    S32 nRetVal = RET_SUCCESS;
    
    if( g_hHid  )   
    {
        DeviceClose( g_hHid );
        g_hHid = NULL;
     } 
     
     return nRetVal;
}


S32 ICC_ReadWindowsHid( U8* pOutData, S32 nDataSize, const S32 nTimeOut, EndCondition pfReadEnd  )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    S32 nDataLen = 0;

    if( g_hHid )
    {
        if(  TRUE ==  DeviceRead( g_hHid, pOutData, nDataSize, &nDataLen, nTimeOut, pfReadEnd ) ) 
        {
             nRetVal = nDataLen;
        }  
        else 
        {
            nRetVal = RET_READ_ERROR;
        }
    }

    return nRetVal;    
    
}


S32 ICC_WriteWindowsHid( U8* pIntData, S32 nDataSize  )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    if( g_hHid )
    {

        if(  TRUE == DeviceWrite( g_hHid,  pIntData,  nDataSize )  ) 
        {
             nRetVal = nDataSize;
        }  
        else 
        {
            nRetVal = RET_WRITE_ERROR;
        }
    }
    
    return nRetVal;
}


