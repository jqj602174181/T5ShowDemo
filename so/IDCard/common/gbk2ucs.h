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
* Description: gbk��unicode��ת��ģ��
*
* History:     
*
*******************************************************************************************/

#ifndef _GBK2UCS_H
#define _GBK2UCS_H


#ifdef __cplusplus
extern "C" {
#endif

//��gbkת��Ϊucs2
int GbkToUcs2( unsigned short *wcstr, const char *gbkstr, unsigned int nCount );

//��ucs2ת��Ϊgbk
int Ucs2ToGbk( char *gbkstr, const unsigned short *wcstr, unsigned int nCount );

//����unicode�ĳ��ȣ��൱��wcslen
int Ucs2Len( const unsigned short *wcstr );

#ifdef __cplusplus
}
#endif

#endif
