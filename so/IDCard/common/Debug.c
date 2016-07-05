#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "GlobalDef.h"
#include "Debug.h"

#if defined( LINUX_EDITION )
#include <unistd.h> //c++
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifndef  DEBUG_FILE
#define  DEBUG_FILE    ("debug.log")
#endif

static S8 *getLogFilePath( void )
{
    static S8 szLogPath[ 512 ] = {0}; 
    
    memset( szLogPath, 0x0, sizeof(szLogPath) );
    
    #if defined( LINUX_EDITION )   
    
 		/*!<用当前路径来作为生成日志的保存路径*/
		getcwd(szLogPath,512);
		
		/*!<拼接字符串*/
		sprintf( szLogPath+strlen(szLogPath), "/%s", DEBUG_FILE ); 
    #else
		sprintf( szLogPath+strlen(szLogPath), "%s", DEBUG_FILE ); 
    #endif

    
    return szLogPath;
}

static S8* GetLogTime( void )
{
    struct  tm  *ptm;
    struct  timeb stTimeb;
    static  S8  szTime[128];
   
    memset( szTime, 0x0, sizeof(szTime) );
    ftime( &stTimeb );
    ptm = localtime( &stTimeb.time );
    sprintf( szTime, "%04d-%02d-%02d %02d:%02d:%02d(%03d)",ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm );
    
    return szTime;
}

void ICC_DPrintLogToFile( const S8 *fmt, ... )
{
    S8 cbBuf[4096];
    FILE *fp = NULL;
    va_list ap;
    
    fp = fopen( getLogFilePath(),"a" );
    if( NULL == fp  )
    {
        return;
    }
    
    fprintf( fp,"%s  ",GetLogTime() );
    memset( cbBuf, 0x0, sizeof(cbBuf) );
    
    va_start( ap, fmt);
    vsnprintf( cbBuf, sizeof(cbBuf), fmt, ap );
    va_end( ap );
    
    fprintf( fp,"%s", cbBuf );
    
    fclose( fp );
}

void ICC_DPrintDataHexToFile( S8 *titleName, U8 *data, S32 dataLen )
{

      FILE *  fp = NULL;
      S32 i = 0;
      
      fp = fopen( getLogFilePath(), "a" );
      if( !fp )
      {
          return; 
      }
      
      fprintf( fp,"%s  ",GetLogTime() );
      
      if( titleName )
      {
          fprintf( fp, "%s  LEN=%d   HEX: ", titleName,  dataLen );
      }
      else 
      {
            fprintf( fp, "LEN=%d   HEX: ",  dataLen );
      }
      
      for( i = 0 ; i < dataLen; i++ )
      {   
          fprintf( fp,"%02X", data[ i ]&0xFF );
      }
      
      fprintf( fp, "\r\n" );
      
      fclose(fp);
}