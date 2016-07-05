#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <io.h>
#include "GlobalDef.h"
#include "Debug.h"
#include "WindowsCom.h"

#ifndef HDEVICE
typedef void* HDEVICE;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

#ifndef _countof
#define _countof(x)   sizeof(x)/sizeof(x[0])
#endif

#ifndef ALIGNUP
#define ALIGNUP(v,r)			(((v)+(r)-1)/(r)*(r))
#endif

#ifndef MAXPATH
#define MAXPATH (260)
#endif
#define DMAX_FILE_PATH      (1024)

#ifndef     RET_SUCCESS
#define     RET_SUCCESS     (0)
#endif

#ifndef     RET_ERROR
#define     RET_ERROR       (-1)
#endif

#ifndef     RET_INVALID_PARAM
#define     RET_INVALID_PARAM       (-2)
#endif

/*!
 * \brief 定义串口通信数据位为7位
 */
#define DCOM_DB7        (7)

/*!
 * \brief 定义串口通信数据位为8位
 */
#define DCOM_DB8        (8)

/*!
 * \brief 定义串口通信的停止位类型为1位
 */
#define DCOM_SB1       (0)


/*!
 * \brief 定义串口通信的停止位类型为1.5位
 */
#define DCOM_SB15       (1)

/*!
 * \brief 定义串口通信的停止位类型为2位
 */
#define DCOM_SB2       (2)

/*!
 * \brief 串口通信采用无校验
 */
#define DCOM_NOPARITY   (0)

/*!
 * \brief 串口通信采用奇校验
 */
#define DCOM_ODDPARITY   (1)

/*!
 * \brief 串口通信采用偶校验
 */
#define DCOM_EVENPARITY   (2)

/*!
 * \brief 串口通信采用MARK校验
 */
#define DCOM_MARKPARITY   (3)

/*!
 * \brief 串口通信采用SPACE校验
 */
#define DCOM_SPACEPARITY   (4)

/*!
 * \brief 串口通信开启CTS/RTS流控
 */
#define DCOM_CTS        (0x00000001)

/*!
 * \brief 串口通信开启DTR/DSR流控
 */
#define DCOM_DTR        (0x00000002)

/*!
 * \brief 串口通信开启xon/xoff流控
 */
#define DCOM_XONXOFF    (0x00000004)


/*!
 * \brief 定义串口设备信息结构体
 */
typedef struct _SERIALCOMDEVICE
{
    HANDLE hDev;        //设备句柄
    HANDLE hReadEvent;  //读的事件
    HANDLE hWriteEvent; //写的事件
    DCB dcbOld;         //打开设备前的配置
} SERIALCOMDEVICE;

static HDEVICE s_hDevice = NULL;

/*!
 * \brief 设置串口参数
 * \par 说明：
 *      详细设置串口通信参数。一般不需要调用。参数包括串口波特率、数据位、停止位和流控方式。
 * \param [in] hDev -设备信息指针, 必须提供由DOpenCom返回的指针
 * \param [in] nBaudrate -串口波特率，可以为1200,2400,4800, 9600,19200,38400,57600,115200
 * \param [in] nDatabit -数据位，可是7位或8位, 分别对应 DCOM_DB7, COM_DB8
 * \param [in] nStopbit -停止位，可是1位，1.5位和2位，分别对应 DCOM_SB1,DCOM_SB15,DCOM_SB2
 * \param [in] dwParity -校验方式，可以取值 DCOM_NOPARITY, DCOM_ODDPARITY, DCOM_EVENPARITY, DCOM_MARKPARITY, DCOM_SPACEPARITY
 * \param [in] dwFlowCtrl -流控标志，可取值 DCOM_CTS，DCOM_DTR, DCOM_XONXOFF
 * \return 若返回RET_SUCCESS表示设置成功；若返回RET_ERROR表示设置失败；若返回RET_INVALID_PARAM表示参数无效
 */
static int  DSetComParam( HDEVICE hDev, const int nBaudrate, const int nDatabit,
                        const int nStopbit, const int dwParity, const int dwFlowCtrl )
{
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    DCB dcb;                                        //设备信息
    
    
    //获取原始的配置
    memset( &dcb, 0, sizeof( DCB ) );
    dcb.DCBlength = sizeof( DCB );
    
    if( GetCommState( pDev->hDev, &dcb ) )
    {
        dcb.BaudRate = nBaudrate;
        dcb.ByteSize = nDatabit;
        dcb.StopBits = nStopbit;
        dcb.Parity = dwParity;
        dcb.fOutxDsrFlow = dwFlowCtrl & DCOM_DTR ? 1 : 0; //流控不可开启，否则数据将接收异常
        dcb.fDtrControl = 0;
        dcb.fOutxCtsFlow = dwFlowCtrl & DCOM_CTS ? 1 : 0; //流控不可开启，否则数据将接收异常
        dcb.fRtsControl = 0;
        dcb.fInX = dcb.fOutX = dwFlowCtrl & DCOM_XONXOFF ? 1 : 0;
        dcb.XonChar = 0x11;
        dcb.XoffChar = 0x13;
        dcb.XonLim = 200 ;
        dcb.XoffLim = 100;
        dcb.fAbortOnError = TRUE;//当发生硬件错误时，读写操作被中断
        // other various settings
        dcb.fBinary = TRUE;//必须为TRUE，微软规定
        dcb.fParity = TRUE;
        
        //使新的配置生效
        if( SetCommState( pDev->hDev, &dcb ) )
        {
            return RET_SUCCESS;
        }
    }
    
    return RET_ERROR;
    
    
}
/*!
 * \brief 打开串口设备
 * \par 说明：
 *      打开串口nComIndex，并配置波特率为nBaudrate，若波特率无效，将采用9600。该函数内部默认的数据位为8位，停止位为1位，校验方式为无校验。
 * \param [in] nComIndex -串口索引，1表示串口1，2表示串口2，依此类推
 * \param [in] nBaudrate -串口波特率，可以为1200,2400,4800, 9600,19200,38400,57600,115200
 * \return 若返回RET_SUCCESS表示打开成功；若返回RET_ERROR表示打开失败；若返回RET_INVALID_PARAM表示参数无效
 */
static HDEVICE  DOpenCom( const int nComIndex, const int nBaudrate )
{   
    wchar_t szDevice[ 32 ];     //设备名称
    DCB dcb;                    //设备信息
    DWORD dwErr;                //错误信息
	HANDLE hFile;
    
    //打开文件
#if !OS_VER_CE//CE没有异步方式
	if ( nComIndex < 10)
	{
		_snwprintf( szDevice, 32, L"COM%d", nComIndex );
	}
	else
	{
		 _snwprintf( szDevice, 32, L"\\\\.\\COM%d", nComIndex );
	}
 //   _snwprintf( szDevice, 32, L"COM%d", nComIndex );
    hFile = CreateFileW( szDevice, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
#else
    _snwprintf( szDevice, 32, L"COM%d:", nComIndex );//CE下串口必须加一个冒号
    hFile = CreateFile( szDevice, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL );
#endif
                                
    if( hFile != INVALID_HANDLE_VALUE )
    {
        //获取原始的配置
        memset( &dcb, 0, sizeof( DCB ) );
        dcb.DCBlength = sizeof( DCB );
        
        if( GetCommState( hFile, &dcb ) )
        {
            //将得到的变量和原始的配置进行备份
            SERIALCOMDEVICE* hDevice = ( SERIALCOMDEVICE* )malloc( sizeof( SERIALCOMDEVICE ) );
            
            if( hDevice )
            {
                memset( hDevice, 0, sizeof( SERIALCOMDEVICE ) );
                hDevice->hDev = hFile;
                memcpy( &hDevice->dcbOld, &dcb, sizeof( DCB ) );
                
                //使新的配置生效
                if( DSetComParam( ( HANDLE )hDevice, nBaudrate, DCOM_DB8, DCOM_SB1, DCOM_NOPARITY, 0 ) == RET_SUCCESS )
                {
                    SetupComm( hFile, 4096, 4096 );//设置输入和输出缓存大小
                    PurgeComm( hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );//清缓存，中断读写操作
                    ClearCommError( hFile, &dwErr, NULL );//清除已有的硬件错误  
                    return hDevice;
                }
                free( hDevice );
            }
        }
        
        CloseHandle( hFile );
    }
    
    return NULL;
}
/*!
 * \brief 关闭串口设备
 * \par 说明：
 *      关闭串口，包括关闭串口设备文件，关闭异步读写事件，释放分配的内存
 * \param [in] hDev -设备信息指针
 * \return 若返回RET_SUCCESS表示关闭成功；若返回RET_ERROR表示关闭失败；若返回RET_INVALID_PARAM表示参数无效
 */
static int  DCloseCom( HDEVICE hDev )
{
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    
    if( pDev )
    {
        if( pDev->hReadEvent )
        {
            CloseHandle( pDev->hReadEvent );
        }
        if( pDev->hWriteEvent )
        {
            CloseHandle( pDev->hWriteEvent );
        }
        
        //恢复原始的串口配置
        //SetCommState( pDev->hDev, &pDev->dcbOld );//不再恢复原始的配置，因为有可能缓存中的数据未发送完成
        
        if( pDev->hDev )
        {
            CloseHandle( pDev->hDev );
        }
        
        free( pDev );
        return RET_SUCCESS;
    }
    
    return RET_ERROR;
}
/*!
 * \brief 从串口读取数据 
 * \par 说明：
 *      从串口hDev读取数据，并存放到缓存pOutData中，最大可获取nDataSize个字节。读操作超时为nTimeOut秒，若超过，函数直接返回
 * \param [in] hDev -设备信息指针, 必须提供由DOpenCom返回的指针
 * \param [out] pOutData -读到的数据存放地址
 * \param [in] nDataSize -数据缓存大小，单位是字节
 * \param [in] nTimeOut  -读取的数据时，最大超时时间，单位是秒。可为0，表示一定要读完nDataSize才能返回函数。
 * \param [in] pfReadEnd -判断是否读完的函数地址，可为NULL，函数将在读完所有字节或超时超时时间nTimeOut时，函数返回
 * \return 若大于等于0，表示读取成功，且返回值表示读到的字节数;RET_ERROR表示关闭失败；若返回RET_INVALID_PARAM表示参数无效
 */
static int DReadCom( HDEVICE hDev, BYTE* pOutData, size_t nDataSize, const int nTimeOut, EndCondition pfReadEnd )
{
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    int nReadLen = 0;       //已读到的字节数
    int nReadTime = 0;      //读数据剩余的时间，单位是ms
	int nWantRead = 0;
    DWORD n = 0;
    DWORD dwTick;
    BOOL bRet;              //ReadFile函数返回值
    DWORD dwErr;            //错误信息
	OVERLAPPED stOverLapped;
	COMMTIMEOUTS stCommTimeout;
	COMSTAT comstat;
    
    if( nTimeOut == 0 )
    {
        nReadTime = 0x7fffffff;
    }
    else
    {
        nReadTime = nTimeOut * 1000;
    }
    
    
#if !OS_VER_CE
    //创建异步事件结构    
    memset( &stOverLapped, 0, sizeof( OVERLAPPED ) );
    if( !pDev->hReadEvent )//若读事件未创建，就需要先创建
    {
        pDev->hReadEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
        if( !pDev->hReadEvent )
        {
            //DDbgError( "Create Event for Com reading operation failed!\n" );
            return 0;
        }
    }
    stOverLapped.hEvent = pDev->hReadEvent;
#endif
    //设置读超时
    
    memset( &stCommTimeout, 0, sizeof( stCommTimeout ) );
    stCommTimeout.ReadTotalTimeoutConstant = nTimeOut * 1000;
    SetCommTimeouts( pDev->hDev, &stCommTimeout );
    
    while( nReadTime > 0 )
    {
        dwTick = GetTickCount();
        ClearCommError( pDev->hDev, &dwErr, &comstat );//清除之前的硬件错误
		if (0 == comstat.cbInQue)
		{
			goto nextloop;
		}
        
#if !OS_VER_CE//CE不支持异步读写

		//add by zff
		nWantRead = min( nDataSize-nReadLen, comstat.cbInQue );
		bRet = ReadFile( pDev->hDev, pOutData+nReadLen, nWantRead, &n, &stOverLapped );
        if( !bRet )//如果未得到数据
        {
            dwErr = GetLastError();                   
            if( dwErr == ERROR_IO_PENDING )//如果操作被挂起了，就等待
            {
                //若超时，直接返回读的结果
                if( WAIT_TIMEOUT == WaitForSingleObject( stOverLapped.hEvent, nReadTime ) )
                {
                    //如果操作未完成，就需要取消挂起的IO操作
                    if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, FALSE ) )
                    {
                        if( !CancelIo( pDev->hDev ))
                        {
                            //DDbgError( "Cancel Com reading operator failed!\n" );
                        }                        
                    }                    
                }
                ClearCommError( pDev->hDev, &dwErr, NULL );//清除之前的硬件错误，可能是CancelIo造成的，也可能是全信号事件造成的
                
                //获取实际读取的数据长度
                if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, TRUE ) )
                {          
                    ClearCommError( pDev->hDev, &dwErr, NULL );
                    break;
                }
            }
            else
            {
                ClearCommError( pDev->hDev, &dwErr, NULL );
                break;
            }
        }
        
#else//CE采用同步方式
        ReadFile( pDev->hDev, pOutData + nReadLen, 1, &n, NULL );
#endif
        nReadLen += n;
        
        //若有检测结束的函数，则调用该函数进行检测, 若已结束，就返回
		if( NULL != pfReadEnd 
			&&( RET_SUCCESS == pfReadEnd( pOutData, nReadLen ) ) )
		{
			break;
		}
        
        //若已达到最大长度，则返回
        if( ( size_t )nReadLen >= nDataSize )
        {
            break;
        }

nextloop:
        nReadTime -= GetTickCount() - dwTick;
    }

    return nReadLen;
    
}

/*!
 * \brief 往串口写数据
 * \par 说明：
 *      从串口hDev写数据，数据长度为nDataSize，数据存放在pOutData, 写超时时间设置为nTimeOut秒
 * \param [in] hDev -设备信息指针, 必须提供由DOpenCom返回的指针
 * \param [in] pOutData -输出的数据存放地址
 * \param [in] nDataSize -数据长度，单位是字节
 * \param [in] nTimeOut  -输出数据时，最大超时时间，单位是秒。可为0，表示一定要写完nDataSize才能返回函数。
 * \return 若大于等于0，表示写数据成功，且返回值表示写的字节数;RET_ERROR表示关闭失败；若返回RET_INVALID_PARAM表示参数无效
 */
static int DWriteCom( HDEVICE hDev, BYTE* pOutData, size_t nDataSize, const int nTimeOut )
{
    int nWriteLen = 0;      //已写的数据长度
    int nWriteTime = 0;     //写数据剩余的时间，单位是ms
    DWORD n;
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    DWORD dwTick;
    DWORD dwErr;            //错误信息
	OVERLAPPED stOverLapped;
    
    if( nTimeOut == 0 )
    {
        nWriteTime = 0x7fffffff;
    }
    else
    {
        nWriteTime = nTimeOut * 1000;
    }
    
#if !OS_VER_CE//CE不支持异步读写
    //创建异步事件结构
    memset( &stOverLapped, 0, sizeof( OVERLAPPED ) );
    if( !pDev->hWriteEvent )//若读事件未创建，就需要先创建
    {
        pDev->hWriteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
        if( !pDev->hWriteEvent )
        {
            //DDbgError( "Create Event for Com writing operation failed!\n" );
            return 0;
        }
    }
    stOverLapped.hEvent = pDev->hWriteEvent;
#endif
    
    while( nWriteTime > 0 )
    {
        dwTick = GetTickCount();
        ClearCommError( pDev->hDev, &dwErr, NULL );//清除之前可能发生的硬件错误
        
#if !OS_VER_CE//CE不支持异步读写
        WriteFile( pDev->hDev, pOutData + nWriteLen, nDataSize - nWriteLen, &n, &stOverLapped );
        
        //若超时，直接返回读的结果
        if( WAIT_TIMEOUT == WaitForSingleObject( stOverLapped.hEvent, nWriteTime ) )
        {
            //如果操作未完成，就需要取消挂起的IO操作
            if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, FALSE ) )
            {
                if( !CancelIo( pDev->hDev ))
                {
                    //DDbgError( "Cancel Com reading operator failed!\n" );
                }
            }
        }
        ClearCommError( pDev->hDev, &dwErr, NULL );//清除可能发生的硬件错误
        
        //获取实际写入的数据长度
        if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, TRUE ) )
        {
            ClearCommError( pDev->hDev, &dwErr, NULL );
            break;
        }
        
#else
        WriteFile( pDev->hDev, pOutData + nWriteLen, nDataSize - nWriteLen, &n, NULL );
#endif
        
        nWriteLen += n;
        
        //若已达到最大长度，则返回
        if( ( size_t )nWriteLen >= nDataSize )
        {
            break;
        }
        
        nWriteTime -= GetTickCount() - dwTick;
    }
    
    return nWriteLen;
}


S32 ICC_OpenWindowsCom( const S32 nComIndex, const S32 nBaudrate )
{
    S32 nRetVal = RET_SUCCESS;
    
    if( !s_hDevice )
    {
        s_hDevice = DOpenCom( nComIndex, nBaudrate );
        if( s_hDevice )
        {
            nRetVal = RET_SUCCESS;
        }
		else
		{
			nRetVal = RET_ERROR;
		}
        
    }
    
    return nRetVal;
}


S32 ICC_CloseWindowsCom( void )
{
    S32 nRetVal = RET_SUCCESS;
    
    if( s_hDevice )
    {
        DCloseCom( s_hDevice );
        s_hDevice = NULL;
    }
    
    return nRetVal;
}


S32 ICC_ReadWindowsCom( U8* pOutData, S32 nDataSize, const S32 nTimeOut, EndCondition pfReadEnd  )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    if( s_hDevice )
    {
        nRetVal = DReadCom( s_hDevice, pOutData, nDataSize, nTimeOut,  pfReadEnd );
    }
    
    return nRetVal;
    
}

S32 ICC_WriteWindowsCom( U8* pIntData, S32 nDataSize  )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    if( s_hDevice )
    {
        nRetVal = DWriteCom( s_hDevice, pIntData, nDataSize, DEFAULT_TIME_OUT );
    }
    
    return nRetVal;
    
}

