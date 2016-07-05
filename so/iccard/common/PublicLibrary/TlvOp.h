/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:  TlvOp.h    
*
*  Author:       laiwenhui 整理
*
*  Date:        2012-06-14 15:04
*
*  Description: TLV格式处理函数头文件
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/

#ifndef _TLV_OP_H_
#define _TLV_OP_H_
/**************************************************
Function:   centerm_find_tag
Description: 在TLV格式的数据indata中取得tagname指定标签的起始位置
Input: tagname  -- 标签名
          taglen      -- 标签名长度
          indata      -- TLV格式数据
          inlen        -- TLV数据长度
Output: none
Return: NULL -- 没有找到标签 ，other -- 指定的标签的起始地址
Others: 
*************************************************/
U8 *centerm_find_tag( U8 *tagname, S32 taglen, U8 *indata, S32 inlen );
/**************************************************
Function: centerm_get_tag_value
Description: 取得tagname标签的值与长度
Input: tagname  -- 标签名
          taglen      -- 标签名长度
          indata      -- TLV格式数据
          inlen        -- TLV数据长度
Output:  tagvalue  -- tagname标签指定的数据
              tagvaluelen -- tagvalue的长度
Return:  -1 没有tagname指定的标签,   0 -- 取得成功
Others: 
*************************************************/
S32 centerm_get_tag_value(U8 *tagname,S32 taglen,U8 * indata,S32 inlen,  U8 * tagvalue,S32 * tagvaluelen);

/**************************************************
Function:centerm_find_tag_app
Description: 在TLV编码格式的字符串查找指定的标签的起始地址。
Input: tagname  -- 标签名
          taglen      -- 标签名长度
          indata      -- TLV格式数据
          inlen        -- TLV数据长度
Output: none
Return: NULL -- 没有查找到标签，other -- 指定的标签地址
Others: 操作TLV编码格式的字符串，但T范围只限'A'-'Z'和'P'-'W'，L是3位
*************************************************/
U8 *centerm_find_tag_app(U8*tagname, S32 taglen, U8 *indata,S32 inlen);
/**************************************************
Function:centerm_get_tag_data
Description: 取得tag的数据，ptagdata 为标签的起始地址
Input:     taglen  --        标签名的长度
              ptagdata --    标签起始地址 
Output:  OutData --   标签的数据
              OutDataLen -- 标签的数据长度
Return: 0 -- 成功 1 -- 失败
Others: 
*************************************************/
S32 centerm_get_tag_data(S32 taglen,U8 * ptagdata,U8 *OutData,U8 * OutDataLen);
/**************************************************
Function: centerm_get_tag_type
Description: 取得标签类型
Input: tagname  -- 标签名
          taglen      -- 标签名长度
          indata      -- TLV格式数据
          inlen        -- TLV数据长度
Output: none
Return: 0 -- 失败 other -- 类型
Others: 
*************************************************/
S32 centerm_get_tag_type(U8 *tagname, S32 taglen, U8 *indata,S32 inlen);

S32 centerm_get_tag_value_app(U8 *tagname,S32 taglen,U8 * indata,S32 inlen,U8 * tagvalue,S32 * tagvaluelen);
#endif
