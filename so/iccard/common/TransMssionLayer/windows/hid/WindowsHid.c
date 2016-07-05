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
    //定义一个GUID的结构体HidGuid来保存HID设备的接口类GUID。
    GUID HidGuid;
    //定义一个DEVINFO的句柄hDevInfoSet来保存获取到的设备信息集合句柄。
    HDEVINFO hDevInfoSet;
    //定义MemberIndex，表示当前搜索到第几个设备，0表示第一个设备。
    DWORD MemberIndex;
    //DevInterfaceData，用来保存设备的驱动接口信息
    SP_DEVICE_INTERFACE_DATA DevInterfaceData;
    //定义一个BOOL变量，保存函数调用是否返回成功
    BOOL Result;
    //定义一个RequiredSize的变量，用来接收需要保存详细信息的缓冲长度。
    DWORD RequiredSize;
    //定义一个指向设备详细信息的结构体指针。
    PSP_DEVICE_INTERFACE_DETAIL_DATA    pDevDetailData = NULL;
    //定义一个用来保存打开设备的句柄。
    HANDLE hDevHandle = NULL;
    //定义一个HIDD_ATTRIBUTES的结构体变量，保存设备的属性。
    HIDD_ATTRIBUTES DevAttributes;

    //对DevInterfaceData结构体的cbSize初始化为结构体大小
    DevInterfaceData.cbSize=sizeof(DevInterfaceData);
    //对DevAttributes结构体的Size初始化为结构体大小
    DevAttributes.Size=sizeof(DevAttributes);

    //调用HidD_GetHidGuid函数获取HID设备的GUID，并保存在HidGuid中。
    HidD_GetHidGuid(&HidGuid);

    //根据HidGuid来获取设备信息集合。其中Flags参数设置为
    //DIGCF_DEVICEINTERFACE|DIGCF_PRESENT，前者表示使用的GUID为
    //接口类GUID，后者表示只列举正在使用的设备，因为我们这里只
    //查找已经连接上的设备。返回的句柄保存在hDevinfo中。注意设备
    //信息集合在使用完毕后，要使用函数SetupDiDestroyDeviceInfoList
    //销毁，不然会造成内存泄漏。
    hDevInfoSet=SetupDiGetClassDevs(&HidGuid,
        NULL,
        NULL,
        DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
    if( !hDevInfoSet )
    {
        return NULL;
    }
    //然后对设备集合中每个设备进行列举，检查是否是我们要找的设备
    //当找到我们指定的设备，或者设备已经查找完毕时，就退出查找。
    //首先指向第一个设备，即将MemberIndex置为0。
    MemberIndex=0;
    SetLastError(NO_ERROR);
    while(1)
    {
        //调用SetupDiEnumDeviceInterfaces在设备信息集合中获取编号为MemberIndex的设备信息。
        Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
            NULL,
            &HidGuid,
            MemberIndex,
            &DevInterfaceData);
        //如果获取信息失败，则说明设备已经查找完毕，退出循环。
        if(Result==FALSE)
        {
            MyHIDDevHandle = NULL;
            break;
        }
        //将MemberIndex指向下一个设备
        MemberIndex++;

        //如果获取信息成功，则继续获取该设备的详细信息。在获取设备
        //详细信息时，需要先知道保存详细信息需要多大的缓冲区，这通过
        //第一次调用函数SetupDiGetDeviceInterfaceDetail来获取。这时
        //提供缓冲区和长度都为NULL的参数，并提供一个用来保存需要多大
        //缓冲区的变量RequiredSize。
        Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
            &DevInterfaceData,
            NULL,
            0,
            &RequiredSize,
            NULL);

        //然后，分配一个大小为RequiredSize缓冲区，用来保存设备详细信息。
        if ( pDevDetailData != NULL )
        {
            free(pDevDetailData);
            pDevDetailData = NULL;
        }
        pDevDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
        if(pDevDetailData==NULL) //如果内存不足，则直接返回。
        {

            MyHIDDevHandle = NULL;
            break;
        }

        //并设置pDevDetailData的cbSize为结构体的大小（注意只是结构体大小，不包括后面缓冲区）。
        pDevDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        //然后再次调用SetupDiGetDeviceInterfaceDetail函数来获取设备的
        //详细信息。这次调用设置使用的缓冲区以及缓冲区大小。
        Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
            &DevInterfaceData,
            pDevDetailData,
            RequiredSize,
            NULL,
            NULL);      

        //如果调用失败，则查找下一个设备。
        if(Result==FALSE) 
        {
            continue;
        }

        //如果调用成功，则使用不带读写访问的CreateFile函数来获取设备的属性，包括VID、PID、版本号等。
        //对于一些独占设备（例如USB键盘），使用读访问方式是无法打开的，
        //而使用不带读写访问的格式才可以打开这些设备，从而获取设备的属性。
        hDevHandle=CreateFile(pDevDetailData->DevicePath, 
            GENERIC_READ|GENERIC_WRITE,//NULL
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,//FILE_ATTRIBUTE_NORMAL,
            NULL);

        //如果打开成功，则获取设备属性。
        if(hDevHandle!=NULL)
        {
            //获取设备的属性并保存在DevAttributes结构体中
            Result=HidD_GetAttributes(hDevHandle, &DevAttributes);

            //关闭刚刚打开的设备
            CloseHandle(hDevHandle);

            //获取失败，查找下一个
            if(Result==FALSE) 
            {
                continue;
            }

            //debugPrintf("VID=%04X,PID=%04X \n",DevAttributes.VendorID,DevAttributes.ProductID);
            //如果获取成功，则将属性中的VID、PID以及设备版本号与我们需要的
            //进行比较，如果都一致的话，则说明它就是我们要找的设备。
            if((DevAttributes.VendorID==wVID) && (DevAttributes.ProductID==wPID)) //如果VID相等并且PID相等
            {
                //debugPrintf("find device now \n");
                //if(DevAttributes.VersionNumber==MyPvn) //如果设备版本号相等
                {
                       //那么就是我们要找的设备，以异步访问的方式打开。
                      MyHIDDevHandle=CreateFile(pDevDetailData->DevicePath, 
                        GENERIC_READ|GENERIC_WRITE,
                        //这个参数为零时，为独占式 modify by laiwenhui R0001
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                        NULL);

                    if(MyHIDDevHandle!=INVALID_HANDLE_VALUE)
                    {
                        //debugPrintf("打开设备成功 \n");
                    }
                    else
                    {
                       //debugPrintf("打开设备失败 \n");
                       MyHIDDevHandle = NULL;
                    }
                    break;
                }
            }
        }
        else//如果打开失败，则查找下一个设备
        {
            continue;
        }
    }//end while(1)

    //调用SetupDiDestroyDeviceInfoList函数销毁设备信息集合
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

    // 清掉
    HidD_FlushQueue(handle);

    WriteOverlapped.Offset=0;
    WriteOverlapped.OffsetHigh=0;
    WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

    for(i=0; i<dwSize; i=i+HID_OUT_REPORT_SIZE)
    {       
        memset(wBuffer, 0, sizeof(wBuffer));
        //在要发送的数据前面加上报告ID
        
        wBuffer[0] = 0x00;
        if( dwSize >= HID_OUT_REPORT_SIZE )
        {
             memcpy((wBuffer+1), lpBuffer+i, HID_OUT_REPORT_SIZE);
        }
        else
        {
            memcpy((wBuffer+1), lpBuffer+i, dwSize );
        }

        //写报告
        bRetCode = WriteFile(handle, wBuffer, HID_OUT_REPORT_SIZE+1, &dwRet, &WriteOverlapped);
        //如果返回错误，并且不是线程被挂起
        if((bRetCode==FALSE) && (GetLastError()!=ERROR_IO_PENDING))
        {
            //debugPrintf("write %d report err! err no = %d ",i, GetLastError());
            return FALSE;
        }
        else
        {
            //等待事件信号（发送完毕或超时）
            WaitForSingleObject(WriteOverlapped.hEvent,1000);
            //重置事件为无信号
            ResetEvent(WriteOverlapped.hEvent);
            bRetCode = TRUE;
        }
    }

    if( dwSize > i )
    {
        memset(wBuffer, 0, sizeof(wBuffer));
        //在要发送的数据前面加上报告ID
        wBuffer[0] = 0x00;
        memcpy((wBuffer+1), lpBuffer+i, dwSize-i);
        //写报告
        bRetCode = WriteFile(handle, wBuffer, HID_OUT_REPORT_SIZE+1, &dwRet, &WriteOverlapped);
        //如果返回错误，并且不是线程被挂起
        if((bRetCode==FALSE) && (GetLastError()!=ERROR_IO_PENDING))
        {
            return FALSE;
        }
        else
        {
            //等待事件信号（发送完毕或超时）
            WaitForSingleObject(WriteOverlapped.hEvent,1000);
            //重置事件为无信号
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
    //读报告
    bRetCode = ReadFile(handle, rBuffer, HID_IN_REPORT_SIZE+1, &dwRet, &ReadOverlapped);
    
    nBegTime = GetTickCount();
    while(1)
    {
        nEndTime=GetTickCount();
        if((nEndTime-nBegTime)>dwTimeOut*1000)//R30001 modified by quexinsheng 修改HID超时无效bug
        {
            CancelIo(handle);
            return FALSE;
        }
        
        Result=WaitForSingleObject(ReadOverlapped.hEvent,1000);
        if(Result==WAIT_OBJECT_0)//如超时返回则取消操作
        {
            break;
        }         
    }
    //将读出的数据复制到出口buffer
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


