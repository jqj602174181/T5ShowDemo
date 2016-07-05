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
    
    // #if defined( LINUX_EDITION )   
    
    // /*!<生成日志保存路径*/
    // sprintf( szLogPath, "%s/icclog/", getenv("HOME") );
    // /*!<创建目录*/
    // mkdir( szLogPath, 0755 );
    
    // #endif
    
    sprintf( szLogPath+strlen(szLogPath), "%s", DEBUG_FILE ); 
    
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
    sprintf( szTime, "%02d%02d%02d%02d%02d%03d", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm );
    
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
    
    fflush(fp);

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
      
      fflush(fp);

      fclose(fp);
}