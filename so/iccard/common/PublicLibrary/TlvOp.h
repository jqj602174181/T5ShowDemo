/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:  TlvOp.h    
*
*  Author:       laiwenhui ����
*
*  Date:        2012-06-14 15:04
*
*  Description: TLV��ʽ������ͷ�ļ�
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/

#ifndef _TLV_OP_H_
#define _TLV_OP_H_
/**************************************************
Function:   centerm_find_tag
Description: ��TLV��ʽ������indata��ȡ��tagnameָ����ǩ����ʼλ��
Input: tagname  -- ��ǩ��
          taglen      -- ��ǩ������
          indata      -- TLV��ʽ����
          inlen        -- TLV���ݳ���
Output: none
Return: NULL -- û���ҵ���ǩ ��other -- ָ���ı�ǩ����ʼ��ַ
Others: 
*************************************************/
U8 *centerm_find_tag( U8 *tagname, S32 taglen, U8 *indata, S32 inlen );
/**************************************************
Function: centerm_get_tag_value
Description: ȡ��tagname��ǩ��ֵ�볤��
Input: tagname  -- ��ǩ��
          taglen      -- ��ǩ������
          indata      -- TLV��ʽ����
          inlen        -- TLV���ݳ���
Output:  tagvalue  -- tagname��ǩָ��������
              tagvaluelen -- tagvalue�ĳ���
Return:  -1 û��tagnameָ���ı�ǩ,   0 -- ȡ�óɹ�
Others: 
*************************************************/
S32 centerm_get_tag_value(U8 *tagname,S32 taglen,U8 * indata,S32 inlen,  U8 * tagvalue,S32 * tagvaluelen);

/**************************************************
Function:centerm_find_tag_app
Description: ��TLV�����ʽ���ַ�������ָ���ı�ǩ����ʼ��ַ��
Input: tagname  -- ��ǩ��
          taglen      -- ��ǩ������
          indata      -- TLV��ʽ����
          inlen        -- TLV���ݳ���
Output: none
Return: NULL -- û�в��ҵ���ǩ��other -- ָ���ı�ǩ��ַ
Others: ����TLV�����ʽ���ַ�������T��Χֻ��'A'-'Z'��'P'-'W'��L��3λ
*************************************************/
U8 *centerm_find_tag_app(U8*tagname, S32 taglen, U8 *indata,S32 inlen);
/**************************************************
Function:centerm_get_tag_data
Description: ȡ��tag�����ݣ�ptagdata Ϊ��ǩ����ʼ��ַ
Input:     taglen  --        ��ǩ���ĳ���
              ptagdata --    ��ǩ��ʼ��ַ 
Output:  OutData --   ��ǩ������
              OutDataLen -- ��ǩ�����ݳ���
Return: 0 -- �ɹ� 1 -- ʧ��
Others: 
*************************************************/
S32 centerm_get_tag_data(S32 taglen,U8 * ptagdata,U8 *OutData,U8 * OutDataLen);
/**************************************************
Function: centerm_get_tag_type
Description: ȡ�ñ�ǩ����
Input: tagname  -- ��ǩ��
          taglen      -- ��ǩ������
          indata      -- TLV��ʽ����
          inlen        -- TLV���ݳ���
Output: none
Return: 0 -- ʧ�� other -- ����
Others: 
*************************************************/
S32 centerm_get_tag_type(U8 *tagname, S32 taglen, U8 *indata,S32 inlen);

S32 centerm_get_tag_value_app(U8 *tagname,S32 taglen,U8 * indata,S32 inlen,U8 * tagvalue,S32 * tagvaluelen);
#endif
