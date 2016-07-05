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
#include <sys/time.h>
#include "queue.h"



int queue_init( queue_t *queue )
{
	memset( queue, 0x0, sizeof(queue_t) );
    pthread_mutex_init( &(queue->mutex), NULL );
    pthread_cond_init( &(queue->cond), NULL  );
    
	return 0;
}

int queue_empty( queue_t *queue )
{
    return queue->head == queue->tail;
}

int queue_full( queue_t *queue )
{
    return (queue->head+1)%QUEUE_SIZE == queue->tail;
}

int queue_push( queue_t *queue, qdata_t ch )
{
    if( queue_full( queue ) )
    {
        return  QUEUE_FULL;
    }

    queue->data[ queue->head++ ] = ch;

    if( queue->head  == QUEUE_SIZE  )
    {
        queue->head = 0;
    }

    return QUEUE_OK;
}

int queue_write( queue_t *queue, qdata_t *data, int len )
{
    int i = 0;

    if( queue_full( queue ) )
    {
        return -1;//队列满
    }

    pthread_mutex_lock( &(queue->mutex) );

    for( i=0; i< len; i++ )
    {

        if( QUEUE_FULL == queue_push( queue, data[i] ) )
        {
            //队列满，则退出
            break;
        }
    }

    pthread_mutex_unlock( &(queue->mutex) );
    pthread_cond_signal( &(queue->cond) );

    return i;//返回写入的数据

}

int queue_pop( queue_t *queue, qdata_t *ch )
{

	if( queue_empty( queue ) )
	{
		return QUEUE_EMPTY;
	}

    *ch = queue->data[ queue->tail++ ];
    if( queue->tail  == QUEUE_SIZE  )
    {
        queue->tail = 0;
    }

    return QUEUE_OK;
}

int queue_read( queue_t *queue, qdata_t *buf, int len )
{

    struct timeval tv;
    struct timespec ts;
    qdata_t ch = 0;
    int retval = 0;
    int readlen = 0;

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + queue->timeout/1000;//秒
    ts.tv_nsec = tv.tv_usec*1000 + (queue->timeout%1000)*1000;//纳秒

    pthread_mutex_lock( &(queue->mutex) );

    while(1)
    {
        retval = queue_pop( queue, &ch );
        if( QUEUE_OK == retval )
        {
            buf[ readlen++ ] = ch;

            if( readlen >= len )
            {
                break;
            }

            if( queue->end_condition && (queue->end_condition(buf, readlen)) )
            {
                break;
            }
        }
        else if( QUEUE_EMPTY == retval )
        {
            if( 0 != pthread_cond_timedwait( &(queue->cond), &(queue->mutex), &ts ) )
            {
                //超时或者出错
                break;
            } 
        }
    }

    pthread_mutex_unlock( &(queue->mutex) );

    return readlen;//返回读取到的数据

}

void queue_destroy( queue_t *queue )
{
    pthread_cond_destroy( &(queue->cond) );
    queue->head = 0;
    queue->tail = 0;
    pthread_mutex_destroy( &(queue->mutex) );
}

void queue_clear( queue_t *queue )
{
    pthread_mutex_lock( &(queue->mutex) );
    queue->head = 0;
    queue->tail = 0;
    pthread_mutex_unlock( &(queue->mutex) );

}


#if defined(FUNCTION_TEST)

static time_t s_start_time;

void get_start_time( void )
{
    time( &s_start_time );
}

double get_diff_time( void )
{
    time_t end_time;

    time( &end_time );

    return difftime( end_time, s_start_time );
}


void* thread_write( void* arg )
{
    queue_t *queue = (queue_t *)arg;
    int retval = 0;

    while(1)
    {
        sleep(1);
        retval = queue_write( queue, "1234567890\x03", 11 ); 
        printf("thread_write retval %d\n", retval); 
    }


    return NULL;
}

int end( qdata_t*data, int len )
{
    printf("data len-1 %02X\n", data[len-1] & 0xFF );
    return data[len-1] == 0x03;
}

int main(int argc, char const *argv[])
{
    int retval = 0;
    queue_t queue;
    pthread_t  th1;

 //    qdata_t ch1 = 'C';
	// qdata_t ch2 = 'B';
    qdata_t buf[256]={0};
    //int len = 0;

	/* code */
    printf("queue_init %d\n", queue_init( &queue ) );
    pthread_create(&th1,NULL,thread_write, &queue);  
	
	// printf("queue_empty %d\n", queue_empty( &queue ) );
	// printf("queue_full %d\n", queue_full( &queue ) );
	// printf("queue_push %d\n", queue_push( &queue, ch1 ) );
	// printf("queue_pop %d\n", queue_pop( &queue, &ch2 ) );	
	// printf("ch2 %c\n", ch2 );
    get_start_time( );
    queue.timeout = 4500;
    queue.end_condition = end;
    retval = queue_read( &queue, buf, sizeof(buf) );
    printf("retval %d(%f) %s\n", retval, get_diff_time(), buf );

    pthread_join(th1, NULL );
	return retval;
}
#endif






