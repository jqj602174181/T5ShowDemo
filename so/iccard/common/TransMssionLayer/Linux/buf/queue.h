#ifndef  _QUEUE_H_
#define  _QUEUE_H_

#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


#define QUEUE_SIZE  	2048
#define QUEUE_OK        0
#define QUEUE_FULL     -1
#define QUEUE_EMPTY    -2


typedef char qdata_t;   

struct QUEUE_T
{
	qdata_t data[ QUEUE_SIZE ];
	int head;
	int tail;
	int timeout;//单位毫秒
	pthread_mutex_t mutex;
    pthread_cond_t cond;
    int (*end_condition)(qdata_t*, int);
};

typedef struct QUEUE_T queue_t;

int queue_init( queue_t *queue );
inline int queue_empty( queue_t *queue );
inline int queue_full( queue_t *queue );
inline int queue_push( queue_t *queue, qdata_t ch );
inline int queue_pop( queue_t *queue, qdata_t *ch );
void queue_destroy( queue_t *queue );
void queue_clear( queue_t *queue );
int queue_write( queue_t *queue, qdata_t *data, int len );
int queue_read( queue_t *queue, qdata_t *buf, int len );



#ifdef __cplusplus
}
#endif

#endif
