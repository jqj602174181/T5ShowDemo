#include <stdio.h>
#include <sys/types.h>
#include <libusb.h>
#include <errno.h>
#include <time.h>
#include "LinuxHid.h"
#include "Debug.h"

#define HID_WRITE_FLG   0x01
#define HID_READ_FLG    0x81


static libusb_device *m_targetDev = NULL;
//获取的usb设备列表
libusb_device **m_devs = NULL;
static libusb_device_handle *m_handle = NULL;




static S32 GetLinuxDataWithoutZero( S8* szInData, int nInData, S8* szOutData )
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

S32 ICC_OpenLinuxHid( const S32 nVid, const S32 nPid )
{
    libusb_device *dev = NULL;
    struct libusb_device_descriptor desc;
    int i = 0;
    int iReturn = 0;

    DbgPrintf("openHid:%d,%d\n", nVid, nPid);
    if(m_targetDev != NULL)
    {
        return RET_SUCCESS;
    }

    //初始化libusb库
    libusb_init(NULL);
 
    DbgPrintf("libusb_init\n");
    //libusb_set_debug( NULL, 1 );
    //获取设备列表
    iReturn = libusb_get_device_list(NULL, &m_devs);
    
    DbgPrintf("libusb_get_device_list:%d\n", iReturn);
    //查找目标设备
    while( NULL != (dev=m_devs[i++]) )
    {
        libusb_get_device_descriptor(dev, &desc);
        DbgPrintf("vid=%04X pid=%04X\n", desc.idVendor, desc.idProduct );    	
        if( (nVid == desc.idVendor) && (nPid == desc.idProduct) )
        {
            DbgPrintf("find target device\n");
            m_targetDev = dev;
            break;
        }
    }

    if(m_targetDev == NULL)
    {
        DbgPrintf("get hiddev err!\n");
        libusb_free_device_list(m_devs, 1);
        m_devs = NULL;
        return RET_DEVICE_UNOPEN;
    }

    if( libusb_open( m_targetDev, &m_handle) < 0 )
    {
        DbgPrintf("libusb open failed\n");
        libusb_free_device_list(m_devs, 1);
        m_devs = NULL;
        m_targetDev = NULL;
        return RET_FAIL_OPEN_FILE;
    }

    //find out if kernel driver is attached
    int Cnt = libusb_kernel_driver_active(m_handle,0);
    DbgPrintf("Cnt1=%d\n", Cnt);
    if( 1 == Cnt )
    {
        Cnt = libusb_detach_kernel_driver(m_handle,0);
        DbgPrintf("Cnt2=%d\n", Cnt);
    }
    //else
    //{
    //    libusb_close(m_handle);
    //    libusb_free_device_list(m_devs, 1);
    //    m_devs = NULL;
    //    m_targetDev = NULL;
    //    m_handle = NULL;
    //    return RET_FAIL_OPEN_FILE;
    //}
    //claim interface 0 (the first) of device
    iReturn = libusb_claim_interface(m_handle,0);
    if(iReturn < 0)
    {
        libusb_close(m_handle);
        libusb_free_device_list(m_devs, 1);
        m_devs = NULL;
        m_targetDev = NULL;
        m_handle = NULL;
        DbgPrintf("Cannot Claim Interface:%d\n", iReturn);  
        return RET_FAIL_OPEN_FILE;  
    }
    
    DbgPrintf("open hiddev success!\n");  

    return RET_SUCCESS;
}


S32 ICC_CloseLinuxHid( void )
{
    DbgPrintf("Close hid!\n");
    if(m_targetDev == NULL)
    {
        return RET_SUCCESS;
    }
    
    libusb_close(m_handle);
    libusb_free_device_list(m_devs, 1);
    m_devs = NULL;
    m_targetDev = NULL;
    m_handle = NULL;

    DbgPrintf("Close hid ok!\n");
    return RET_SUCCESS;
}


S32 ICC_WriteLinuxHid( U8* pIntData, S32 nDataSize  )
{
    int  nSendedLen = 0;
    int  nSendTol = 0;
    int  nTimes = 0;
    int  nReturn = 0;

    DbgPrintHex("hid write", pIntData, nDataSize);
    if(!m_handle)
    {
        return RET_DEVICE_UNOPEN;
    }
    
    //最多尝试发送5次，超过则返回错误
    for(nTimes = 0; nTimes < 5; nTimes++)
    {
        nReturn = libusb_bulk_transfer( m_handle, HID_WRITE_FLG, pIntData+nSendTol, nDataSize-nSendTol, &nSendedLen, 1000 );
        DbgPrintf("nTimes:%d sendlen%d ret:%d\n", nTimes, nSendedLen , nReturn);
        //if(nReturn < 0)
        //{
        //    return RET_WRITE_ERROR;
        //}
        nSendTol += nSendedLen;
        if(nSendTol == nDataSize)
        {
            break;
        }
    }

    if(nSendTol != nDataSize)
    {
        return RET_WRITE_ERROR;
    }
    
    DbgPrintf("write ok:%d\n", nSendTol);

    return nSendTol;
}


S32 ICC_ReadLinuxHid( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition  )
{
    time_t stStartTime,stEndTime; 
    S32 nReadLen = 0;   /*读取的字节数*/
    S32 nTempLen = 0;                  /*临时变量*/
    S32 nRetVal = RET_SUCCESS;  /*返回值*/
    S32 nReadOneTm = 0;
    U8  szReadOneTm[1024];
    /*获取起始时间*/
    time( &stStartTime );  

    DbgPrintf("hid read maxlen:%d\n", nMaxLen);
    while( 1 )
    {
        /*读取串口数据*/
        memset(szReadOneTm, 0, sizeof(szReadOneTm));
        nRetVal = libusb_bulk_transfer( m_handle, HID_READ_FLG, szReadOneTm, sizeof(szReadOneTm), &nReadOneTm, 200);	   

        DbgPrintHex("hid read:", szReadOneTm, nReadOneTm);
        /*if( nRetVal < 0 )
        {
            nRetVal = RET_READ_ERROR;
            break;
        }*/
        //如果读成功，最少返回64字节，后补0x00，需要将有效数据提取出来
        nReadOneTm = GetLinuxDataWithoutZero( szReadOneTm, nReadOneTm, szRecvBuf+nReadLen );
        nReadLen += nReadOneTm; 
        if( nMaxLen <= nReadLen )
        {
            nRetVal = nMaxLen;
            break;
        }  

        DbgPrintHex("read real data", szRecvBuf, nReadLen);

        if( ( lpEndCondition )&&( !lpEndCondition( szRecvBuf, nReadLen ) ) )
        {
            nRetVal = nReadLen;
            break;
        }

        /*!<time out */
        time( &stEndTime );
        if( stEndTime - stStartTime > nTimeOut )
        {
            nRetVal = RET_FAIL_TIMEOUT;
            break;
        }
    }

    DbgPrintf("read ret:%d\n", nRetVal);

    return nRetVal;
}
