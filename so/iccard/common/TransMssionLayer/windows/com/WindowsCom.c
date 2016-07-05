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
 * \brief ���崮��ͨ������λΪ7λ
 */
#define DCOM_DB7        (7)

/*!
 * \brief ���崮��ͨ������λΪ8λ
 */
#define DCOM_DB8        (8)

/*!
 * \brief ���崮��ͨ�ŵ�ֹͣλ����Ϊ1λ
 */
#define DCOM_SB1       (0)


/*!
 * \brief ���崮��ͨ�ŵ�ֹͣλ����Ϊ1.5λ
 */
#define DCOM_SB15       (1)

/*!
 * \brief ���崮��ͨ�ŵ�ֹͣλ����Ϊ2λ
 */
#define DCOM_SB2       (2)

/*!
 * \brief ����ͨ�Ų�����У��
 */
#define DCOM_NOPARITY   (0)

/*!
 * \brief ����ͨ�Ų�����У��
 */
#define DCOM_ODDPARITY   (1)

/*!
 * \brief ����ͨ�Ų���żУ��
 */
#define DCOM_EVENPARITY   (2)

/*!
 * \brief ����ͨ�Ų���MARKУ��
 */
#define DCOM_MARKPARITY   (3)

/*!
 * \brief ����ͨ�Ų���SPACEУ��
 */
#define DCOM_SPACEPARITY   (4)

/*!
 * \brief ����ͨ�ſ���CTS/RTS����
 */
#define DCOM_CTS        (0x00000001)

/*!
 * \brief ����ͨ�ſ���DTR/DSR����
 */
#define DCOM_DTR        (0x00000002)

/*!
 * \brief ����ͨ�ſ���xon/xoff����
 */
#define DCOM_XONXOFF    (0x00000004)


/*!
 * \brief ���崮���豸��Ϣ�ṹ��
 */
typedef struct _SERIALCOMDEVICE
{
    HANDLE hDev;        //�豸���
    HANDLE hReadEvent;  //�����¼�
    HANDLE hWriteEvent; //д���¼�
    DCB dcbOld;         //���豸ǰ������
} SERIALCOMDEVICE;

static HDEVICE s_hDevice = NULL;

/*!
 * \brief ���ô��ڲ���
 * \par ˵����
 *      ��ϸ���ô���ͨ�Ų�����һ�㲻��Ҫ���á������������ڲ����ʡ�����λ��ֹͣλ�����ط�ʽ��
 * \param [in] hDev -�豸��Ϣָ��, �����ṩ��DOpenCom���ص�ָ��
 * \param [in] nBaudrate -���ڲ����ʣ�����Ϊ1200,2400,4800, 9600,19200,38400,57600,115200
 * \param [in] nDatabit -����λ������7λ��8λ, �ֱ��Ӧ DCOM_DB7, COM_DB8
 * \param [in] nStopbit -ֹͣλ������1λ��1.5λ��2λ���ֱ��Ӧ DCOM_SB1,DCOM_SB15,DCOM_SB2
 * \param [in] dwParity -У�鷽ʽ������ȡֵ DCOM_NOPARITY, DCOM_ODDPARITY, DCOM_EVENPARITY, DCOM_MARKPARITY, DCOM_SPACEPARITY
 * \param [in] dwFlowCtrl -���ر�־����ȡֵ DCOM_CTS��DCOM_DTR, DCOM_XONXOFF
 * \return ������RET_SUCCESS��ʾ���óɹ���������RET_ERROR��ʾ����ʧ�ܣ�������RET_INVALID_PARAM��ʾ������Ч
 */
static int  DSetComParam( HDEVICE hDev, const int nBaudrate, const int nDatabit,
                        const int nStopbit, const int dwParity, const int dwFlowCtrl )
{
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    DCB dcb;                                        //�豸��Ϣ
    
    
    //��ȡԭʼ������
    memset( &dcb, 0, sizeof( DCB ) );
    dcb.DCBlength = sizeof( DCB );
    
    if( GetCommState( pDev->hDev, &dcb ) )
    {
        dcb.BaudRate = nBaudrate;
        dcb.ByteSize = nDatabit;
        dcb.StopBits = nStopbit;
        dcb.Parity = dwParity;
        dcb.fOutxDsrFlow = dwFlowCtrl & DCOM_DTR ? 1 : 0; //���ز��ɿ������������ݽ������쳣
        dcb.fDtrControl = 0;
        dcb.fOutxCtsFlow = dwFlowCtrl & DCOM_CTS ? 1 : 0; //���ز��ɿ������������ݽ������쳣
        dcb.fRtsControl = 0;
        dcb.fInX = dcb.fOutX = dwFlowCtrl & DCOM_XONXOFF ? 1 : 0;
        dcb.XonChar = 0x11;
        dcb.XoffChar = 0x13;
        dcb.XonLim = 200 ;
        dcb.XoffLim = 100;
        dcb.fAbortOnError = TRUE;//������Ӳ������ʱ����д�������ж�
        // other various settings
        dcb.fBinary = TRUE;//����ΪTRUE��΢��涨
        dcb.fParity = TRUE;
        
        //ʹ�µ�������Ч
        if( SetCommState( pDev->hDev, &dcb ) )
        {
            return RET_SUCCESS;
        }
    }
    
    return RET_ERROR;
    
    
}
/*!
 * \brief �򿪴����豸
 * \par ˵����
 *      �򿪴���nComIndex�������ò�����ΪnBaudrate������������Ч��������9600���ú����ڲ�Ĭ�ϵ�����λΪ8λ��ֹͣλΪ1λ��У�鷽ʽΪ��У�顣
 * \param [in] nComIndex -����������1��ʾ����1��2��ʾ����2����������
 * \param [in] nBaudrate -���ڲ����ʣ�����Ϊ1200,2400,4800, 9600,19200,38400,57600,115200
 * \return ������RET_SUCCESS��ʾ�򿪳ɹ���������RET_ERROR��ʾ��ʧ�ܣ�������RET_INVALID_PARAM��ʾ������Ч
 */
static HDEVICE  DOpenCom( const int nComIndex, const int nBaudrate )
{   
    wchar_t szDevice[ 32 ];     //�豸����
    DCB dcb;                    //�豸��Ϣ
    DWORD dwErr;                //������Ϣ
	HANDLE hFile;
    
    //���ļ�
#if !OS_VER_CE//CEû���첽��ʽ
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
    _snwprintf( szDevice, 32, L"COM%d:", nComIndex );//CE�´��ڱ����һ��ð��
    hFile = CreateFile( szDevice, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL );
#endif
                                
    if( hFile != INVALID_HANDLE_VALUE )
    {
        //��ȡԭʼ������
        memset( &dcb, 0, sizeof( DCB ) );
        dcb.DCBlength = sizeof( DCB );
        
        if( GetCommState( hFile, &dcb ) )
        {
            //���õ��ı�����ԭʼ�����ý��б���
            SERIALCOMDEVICE* hDevice = ( SERIALCOMDEVICE* )malloc( sizeof( SERIALCOMDEVICE ) );
            
            if( hDevice )
            {
                memset( hDevice, 0, sizeof( SERIALCOMDEVICE ) );
                hDevice->hDev = hFile;
                memcpy( &hDevice->dcbOld, &dcb, sizeof( DCB ) );
                
                //ʹ�µ�������Ч
                if( DSetComParam( ( HANDLE )hDevice, nBaudrate, DCOM_DB8, DCOM_SB1, DCOM_NOPARITY, 0 ) == RET_SUCCESS )
                {
                    SetupComm( hFile, 4096, 4096 );//�����������������С
                    PurgeComm( hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );//�建�棬�ж϶�д����
                    ClearCommError( hFile, &dwErr, NULL );//������е�Ӳ������  
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
 * \brief �رմ����豸
 * \par ˵����
 *      �رմ��ڣ������رմ����豸�ļ����ر��첽��д�¼����ͷŷ�����ڴ�
 * \param [in] hDev -�豸��Ϣָ��
 * \return ������RET_SUCCESS��ʾ�رճɹ���������RET_ERROR��ʾ�ر�ʧ�ܣ�������RET_INVALID_PARAM��ʾ������Ч
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
        
        //�ָ�ԭʼ�Ĵ�������
        //SetCommState( pDev->hDev, &pDev->dcbOld );//���ٻָ�ԭʼ�����ã���Ϊ�п��ܻ����е�����δ�������
        
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
 * \brief �Ӵ��ڶ�ȡ���� 
 * \par ˵����
 *      �Ӵ���hDev��ȡ���ݣ�����ŵ�����pOutData�У����ɻ�ȡnDataSize���ֽڡ���������ʱΪnTimeOut�룬������������ֱ�ӷ���
 * \param [in] hDev -�豸��Ϣָ��, �����ṩ��DOpenCom���ص�ָ��
 * \param [out] pOutData -���������ݴ�ŵ�ַ
 * \param [in] nDataSize -���ݻ����С����λ���ֽ�
 * \param [in] nTimeOut  -��ȡ������ʱ�����ʱʱ�䣬��λ���롣��Ϊ0����ʾһ��Ҫ����nDataSize���ܷ��غ�����
 * \param [in] pfReadEnd -�ж��Ƿ����ĺ�����ַ����ΪNULL���������ڶ��������ֽڻ�ʱ��ʱʱ��nTimeOutʱ����������
 * \return �����ڵ���0����ʾ��ȡ�ɹ����ҷ���ֵ��ʾ�������ֽ���;RET_ERROR��ʾ�ر�ʧ�ܣ�������RET_INVALID_PARAM��ʾ������Ч
 */
static int DReadCom( HDEVICE hDev, BYTE* pOutData, size_t nDataSize, const int nTimeOut, EndCondition pfReadEnd )
{
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    int nReadLen = 0;       //�Ѷ������ֽ���
    int nReadTime = 0;      //������ʣ���ʱ�䣬��λ��ms
	int nWantRead = 0;
    DWORD n = 0;
    DWORD dwTick;
    BOOL bRet;              //ReadFile��������ֵ
    DWORD dwErr;            //������Ϣ
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
    //�����첽�¼��ṹ    
    memset( &stOverLapped, 0, sizeof( OVERLAPPED ) );
    if( !pDev->hReadEvent )//�����¼�δ����������Ҫ�ȴ���
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
    //���ö���ʱ
    
    memset( &stCommTimeout, 0, sizeof( stCommTimeout ) );
    stCommTimeout.ReadTotalTimeoutConstant = nTimeOut * 1000;
    SetCommTimeouts( pDev->hDev, &stCommTimeout );
    
    while( nReadTime > 0 )
    {
        dwTick = GetTickCount();
        ClearCommError( pDev->hDev, &dwErr, &comstat );//���֮ǰ��Ӳ������
		if (0 == comstat.cbInQue)
		{
			goto nextloop;
		}
        
#if !OS_VER_CE//CE��֧���첽��д

		//add by zff
		nWantRead = min( nDataSize-nReadLen, comstat.cbInQue );
		bRet = ReadFile( pDev->hDev, pOutData+nReadLen, nWantRead, &n, &stOverLapped );
        if( !bRet )//���δ�õ�����
        {
            dwErr = GetLastError();                   
            if( dwErr == ERROR_IO_PENDING )//��������������ˣ��͵ȴ�
            {
                //����ʱ��ֱ�ӷ��ض��Ľ��
                if( WAIT_TIMEOUT == WaitForSingleObject( stOverLapped.hEvent, nReadTime ) )
                {
                    //�������δ��ɣ�����Ҫȡ�������IO����
                    if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, FALSE ) )
                    {
                        if( !CancelIo( pDev->hDev ))
                        {
                            //DDbgError( "Cancel Com reading operator failed!\n" );
                        }                        
                    }                    
                }
                ClearCommError( pDev->hDev, &dwErr, NULL );//���֮ǰ��Ӳ�����󣬿�����CancelIo��ɵģ�Ҳ������ȫ�ź��¼���ɵ�
                
                //��ȡʵ�ʶ�ȡ�����ݳ���
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
        
#else//CE����ͬ����ʽ
        ReadFile( pDev->hDev, pOutData + nReadLen, 1, &n, NULL );
#endif
        nReadLen += n;
        
        //���м������ĺ���������øú������м��, ���ѽ������ͷ���
		if( NULL != pfReadEnd 
			&&( RET_SUCCESS == pfReadEnd( pOutData, nReadLen ) ) )
		{
			break;
		}
        
        //���Ѵﵽ��󳤶ȣ��򷵻�
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
 * \brief ������д����
 * \par ˵����
 *      �Ӵ���hDevд���ݣ����ݳ���ΪnDataSize�����ݴ����pOutData, д��ʱʱ������ΪnTimeOut��
 * \param [in] hDev -�豸��Ϣָ��, �����ṩ��DOpenCom���ص�ָ��
 * \param [in] pOutData -��������ݴ�ŵ�ַ
 * \param [in] nDataSize -���ݳ��ȣ���λ���ֽ�
 * \param [in] nTimeOut  -�������ʱ�����ʱʱ�䣬��λ���롣��Ϊ0����ʾһ��Ҫд��nDataSize���ܷ��غ�����
 * \return �����ڵ���0����ʾд���ݳɹ����ҷ���ֵ��ʾд���ֽ���;RET_ERROR��ʾ�ر�ʧ�ܣ�������RET_INVALID_PARAM��ʾ������Ч
 */
static int DWriteCom( HDEVICE hDev, BYTE* pOutData, size_t nDataSize, const int nTimeOut )
{
    int nWriteLen = 0;      //��д�����ݳ���
    int nWriteTime = 0;     //д����ʣ���ʱ�䣬��λ��ms
    DWORD n;
    SERIALCOMDEVICE* pDev = ( SERIALCOMDEVICE* )hDev;
    DWORD dwTick;
    DWORD dwErr;            //������Ϣ
	OVERLAPPED stOverLapped;
    
    if( nTimeOut == 0 )
    {
        nWriteTime = 0x7fffffff;
    }
    else
    {
        nWriteTime = nTimeOut * 1000;
    }
    
#if !OS_VER_CE//CE��֧���첽��д
    //�����첽�¼��ṹ
    memset( &stOverLapped, 0, sizeof( OVERLAPPED ) );
    if( !pDev->hWriteEvent )//�����¼�δ����������Ҫ�ȴ���
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
        ClearCommError( pDev->hDev, &dwErr, NULL );//���֮ǰ���ܷ�����Ӳ������
        
#if !OS_VER_CE//CE��֧���첽��д
        WriteFile( pDev->hDev, pOutData + nWriteLen, nDataSize - nWriteLen, &n, &stOverLapped );
        
        //����ʱ��ֱ�ӷ��ض��Ľ��
        if( WAIT_TIMEOUT == WaitForSingleObject( stOverLapped.hEvent, nWriteTime ) )
        {
            //�������δ��ɣ�����Ҫȡ�������IO����
            if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, FALSE ) )
            {
                if( !CancelIo( pDev->hDev ))
                {
                    //DDbgError( "Cancel Com reading operator failed!\n" );
                }
            }
        }
        ClearCommError( pDev->hDev, &dwErr, NULL );//������ܷ�����Ӳ������
        
        //��ȡʵ��д������ݳ���
        if( !GetOverlappedResult( pDev->hDev, &stOverLapped, &n, TRUE ) )
        {
            ClearCommError( pDev->hDev, &dwErr, NULL );
            break;
        }
        
#else
        WriteFile( pDev->hDev, pOutData + nWriteLen, nDataSize - nWriteLen, &n, NULL );
#endif
        
        nWriteLen += n;
        
        //���Ѵﵽ��󳤶ȣ��򷵻�
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

