#ifndef  _READ_FIFO_H_
#define  _READ_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif


int write_buffer( unsigned char * data, int len );
int read_buffer( unsigned char *buf, int len, int timeout );
int fifo_init(void);
int fifo_open( void );
int fifo_close( void );
int fifo_read( char *buf, int len, int timeout );
int fifo_write( char *buf, int len );
int fifo_sendFinish(  ) ;

#ifdef __cplusplus
}
#endif

#endif
