#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/param.h> 
#include <sys/un.h>
#include <unistd.h>  
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>
#include <pthread.h>
#include "queue.h"
#include "StrOp.h"
#include "Debug.h"


int queue_write( queue_t *queue, qdata_t *data, int len );
int queue_read( queue_t *queue, qdata_t *buf, int len );


static queue_t g_read_queue;
static queue_t g_write_queue;

static int icc_read_end_condition( qdata_t*data, int len )
{
    //printf("data len-1 %02X\n", data[len-1] & 0xFF );
    return data[len-1] == 0x03;
}

//必须实现的jni接口，提供给apk调用
int write_buffer( unsigned char * data, int len )
{
    return queue_write( &g_write_queue, data, len );
}

//必须实现的jni接口，提供给apk调用
int read_buffer( unsigned char *buf, int len, int timeout )
{	
    g_read_queue.timeout = timeout*1000;
    return queue_read( &g_read_queue, buf, len );//返回读取到的数据
}

//底层通讯接口
int fifo_init(void)
{
	queue_init( &g_read_queue );
    queue_init( &g_write_queue );
    g_read_queue.end_condition = icc_read_end_condition;
    g_write_queue.end_condition = icc_read_end_condition;
    return 0;
}

int fifo_open( void )
{
    //打开时做清空缓存操作,避免受到上一个流程的影响;
    queue_clear(&g_write_queue ) ;
	return 0;
}	


int fifo_close( void )
{
	return 0;
}

int fifo_read( char *buf, int len, int timeout )
{
    int readlen = -1 ;
    g_write_queue.timeout = timeout*1000;
    readlen = queue_read( &g_write_queue, buf, len );
    queue_clear(&g_write_queue ) ;//读完清空

    return readlen;
	
}

int fifo_write( char *buf, int len )
{
    queue_clear( &g_read_queue );//写前先清空缓存
    
	return queue_write( &g_read_queue, buf, len );

}

int fifo_sendFinish(  ) 
{
    char cmdFinish[ 6 ] = { 0x02, 0x0F, 0x03 ,0x00, 0x00 ,0x00 } ;
    DbgPrintf( "Send Finish Target!" ) ;
    if( 3 == fifo_write( cmdFinish, 3 ) )
    {
        return 0 ;
    }

    return -1 ;
}






