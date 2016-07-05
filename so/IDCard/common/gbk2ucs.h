/******************************************************************************************
* 
* Copyright(C), 2002-2009, Centerm Information Co.,Ltd,All Rights Reserved
*
* FileName:    gbk2ucs.h
*
* Author:      Qiuzy
*
* Date:        2008-12-25
*
* Description: gbk与unicode互转的模块
*
* History:     
*
*******************************************************************************************/

#ifndef _GBK2UCS_H
#define _GBK2UCS_H


#ifdef __cplusplus
extern "C" {
#endif

//将gbk转化为ucs2
int GbkToUcs2( unsigned short *wcstr, const char *gbkstr, unsigned int nCount );

//将ucs2转化为gbk
int Ucs2ToGbk( char *gbkstr, const unsigned short *wcstr, unsigned int nCount );

//计算unicode的长度，相当于wcslen
int Ucs2Len( const unsigned short *wcstr );

#ifdef __cplusplus
}
#endif

#endif
