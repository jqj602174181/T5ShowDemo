/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:  TlvOp.c    
*
*  Author:       laiwenhui ����
*
*  Date:        2012-06-14 15:04
*
*  Description: TLV��ʽ������
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "GlobalDef.h"
#include "TlvOp.h"
#include "Debug.h"
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
U8 *centerm_find_tag( unsigned  char *tagname, S32 taglen, U8 *indata, S32 inlen )
{
      U8 * p;  
      U8 *pp;
      U8 tmptag[256];
      unsigned tmptaglen = 0;
      S32 iloop = 0;
      p = NULL;
      iloop = 0;
      if( inlen == 0 )
      {
           while( indata[iloop] != 0x00 )//������'\0'�����ַ�������
           {
             if(indata[iloop]==0xFF)//
             {
                 return p;
             }
             tmptag[ 0 ] = indata[ iloop ];
             tmptaglen = 1;
             if( ( indata[iloop]&0x1F ) == 0x1F )   
             {
       
               iloop++;
       
               while((indata[iloop]&0x80)&&(iloop<inlen))
               {
                     if(indata[iloop]==0xFF)
                     {
                        return p;
                     }
                     tmptag[tmptaglen]=indata[iloop];
                     tmptaglen++;
                     iloop++;
               }
               
               tmptag[tmptaglen]=indata[iloop];
               tmptaglen++;      
             }
             
             iloop++;
             if((memcmp(tagname,tmptag,tmptaglen)==0)&&(taglen==tmptaglen))
             {     
                   p=&indata[iloop-tmptaglen];
                   return p;
             }
               //δ�ҵ���ǩ
             if(indata[iloop-tmptaglen]&0x20)        
             {
                   if(indata[iloop]==0x82) 
                   {             
                        iloop+=3;
                   }
                   else if(indata[iloop]==0x81)
                   {          
                        iloop+=2;
                   }
                   else
                   {
                        iloop++;
                   }                   
             }
             else
             {                            
                   if(indata[iloop]==0x82)
                   {
                         iloop+=(indata[iloop+1]*256+indata[iloop+2])&0x0FFFF;
                         iloop+=3;
                   }
                   else if(indata[iloop]==0x81)
                   {
                         iloop+=indata[iloop+1];
                         iloop+=2;
                   }
                   else
                   {
                        iloop+=indata[iloop]+1;
                   }
             }                        
           }
           return p;
      }
    
      while( iloop < inlen )
      {
        if(indata[iloop]==0xFF)
        {
             return p;
        }
        tmptag[0]=indata[iloop];//�����һ��tag�ֽ�
        tmptaglen=1;//��λΪ1����Ϊtag����Ϊ1�������iso��׼
        if((indata[iloop]&0x1F)==0x1F)//����Ƕ���ֽ�    
        {    
            iloop++;
            //���ܺ������нڵ㣬ISO��׼������tag����ֽ�
            while((indata[iloop]&0x80)&&(iloop<inlen)) 
            {     
                  //����tag����
                  if(indata[iloop]==0xFF)
                  {
                        return p;
                  }
                  tmptag[tmptaglen]=indata[iloop];
                  tmptaglen++;
                  iloop++;
            }
           if( iloop >= inlen )
           {
                return p;
           }
           tmptag[ tmptaglen ] = indata[ iloop ];//�������һ��tag�ֽ�
           tmptaglen++;     
        }
        
        iloop++;    
        pp=&indata[iloop-tmptaglen];
        //�ж��Ƿ��ҵ�tag
        if((memcmp(tagname,tmptag,tmptaglen)==0)&&(taglen==tmptaglen))
        {     
              p=&indata[iloop-tmptaglen];
              return p;//�ҵ���tagnameָ���� tlv�ַ���,�򷵻�TLV��ʼ��ַ
        }
        
        if( indata[ iloop - tmptaglen ]&0x20) //����Ǹ���tlv�ṹ        
        {
               //����tlv�еĳ����ֽ�
              if(indata[iloop]==0x82)
              {             
                    iloop+=3;
              }
              else if(indata[iloop]==0x81)
              {          
                  iloop+=2;
              }
              else
              {
                  iloop++;
              }                   
        }
        else//���Ǹ��Ͻṹ����ζ����Ҫ����length��value�������¸�tlv�ṹ�Ŀ�ʼ
        {                     
              if(indata[iloop]==0x82)//length����3���ֽ���ɣ����峤����2��3�ֽھ���
              {
                    iloop+=( indata[ iloop + 1 ]*256+indata[ iloop + 2 ] )&0x0FFFF;
                    iloop+=3;
              }
              else if( indata[ iloop ] == 0x81 )//length����2���ֽ���ɣ����峤���ɵڶ����ֽھ���
              {
                    iloop += indata[iloop+1];
                    iloop += 2;
              }
              else//length����1���ֽ���ɣ����峤���ɸ��ֽڵĵ�7λ����
              {
                    iloop +=indata[iloop]+1;
              }
        }                       
      }
      
      return p;
}
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
S32 centerm_get_tag_value(U8 *tagname,S32 taglen,U8 * indata,S32 inlen, U8 * tagvalue,S32 * tagvaluelen)
{
      U8 * p;
      S32 ilen=0;
    
      p = centerm_find_tag( tagname, taglen, indata, inlen );
      if( p == NULL)
      {
            return -1;//û���ҵ�tagname��ǩ
      }
        
      p+=taglen;//������ǩ
      ilen=0;
      //�õ�TLV�ĳ���
      if( p[0] == 0x82 )//length����3���ֽ���ɣ����峤����2��3�ֽھ���
      {
            ilen+=(p[1]*256+p[2])&0x0FFFF;
            p+=3;
      }
      else if( p[0] == 0x81)//length����2���ֽ���ɣ����峤���ɵڶ����ֽھ���
      {
            ilen=indata[1];
            p+=2;
      }
      else//length����1���ֽ���ɣ����峤���ɸ��ֽڵĵ�7λ����
      {
            ilen=p[0];
            p+=1;
      }
      //ȡ��v��ֵ���䳤��
      memcpy( tagvalue, p, ilen );     
      (*tagvaluelen)=ilen;
      return 0;  
}
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
U8 *centerm_find_tag_app(unsigned  char *tagname, S32 taglen, U8 *indata,S32 inlen)
{
      U8 * p;
      S32 ilen=0;
      
      p = indata;
      //����ͨ��ѭ���ķ�ʽ������tagnameָ���ı�ǩ��ʼ��ַ
      while( *p )
      {
            if(memcmp(p, tagname, taglen) == 0)
            {     
                return p;//�ҵ���ǩ������
            }
            else if(((*p >= 0x41) && (*p <= 0x4A)) || ((*p >= 0x50) && (*p <= 0x57)))
            {
                  p ++;
                  ilen=0;
                  ilen += p[0] - 0x30;
                  ilen *= 10;
                  ilen += p[1] - 0x30;
                  ilen *= 10;
                  ilen += p[2] - 0x30;
                  p += 3; 
                  p += ilen;
            }
            else
            {
                p ++;
            }
      }

      return NULL;
}
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
S32 centerm_get_tag_data(S32 taglen,U8 * ptagdata,U8 *OutData,U8 * OutDataLen)
{
      ptagdata+=taglen;
      if((*ptagdata)==0x82)//length����3���ֽ���ɣ����峤����2��3�ֽھ���
      {
            (*OutDataLen)=((*(ptagdata+1))*256+(*(ptagdata+2)))&0x0FFFF;
            memcpy(OutData,ptagdata+3,(*OutDataLen));
      }
      else if((*ptagdata)==0x81)//length����2���ֽ���ɣ����峤���ɵڶ����ֽھ���
      {
            (*OutDataLen)=(*(ptagdata+1));
            memcpy(OutData,ptagdata+2,(*OutDataLen));
      }
      else//length����1���ֽ���ɣ����峤���ɸ��ֽڵĵ�7λ����
      {
            (*OutDataLen)=(*ptagdata);
            memcpy( OutData, ptagdata+1, (*OutDataLen) );
      }
      return 0;
       
}
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
S32 centerm_get_tag_type(unsigned  char *tagname, S32 taglen, U8 *indata,S32 inlen)
{
    U8 * p;
    U8 tmptag[256],tmptaglen=0;
    S32 iloop=0;

    p=NULL;
    while(iloop<inlen)
   {
        if(indata[iloop] == 0xFF)
        {
            return 0;
        }
        tmptag[0]=indata[iloop];
        tmptaglen=1;
        if((indata[iloop]&0x1F)==0x1F)   
        {    
           iloop++;                    
           while((indata[iloop]&0x80)&&(iloop<inlen))  
           {
                 if(indata[iloop]==0xFF)
                 {
                      return 0;
                 }
                 tmptag[tmptaglen]=indata[iloop];
                 tmptaglen++;
                 iloop++;
           }
           if(iloop>=inlen)
           {
                return 0;
           }
           tmptag[tmptaglen]=indata[iloop];
           tmptaglen++;
          
        }
        iloop++; 
        if((memcmp(tagname,tmptag,tmptaglen)==0)&&(taglen==tmptaglen))
        {     
               return indata[iloop]; //���ر�ǩ����
        }
        iloop++; 
      }
      return 0;
}
/*! \brief ֻȡ�ַ���������ָ����ǩ���ƺͳ��ȵ�ֵ��
 *  \par ˵��:
 *       �ú����ǲ���TLV�����ʽ���ַ�������T��Χֻ��'A'-'Z'��'P'-'W'��L��3λ��
 *  \param [in]  tagname       -ָ����ǩ����
 *  \param [in]  taglen        -ָ����ǩ����
 *  \param [in]  indata        -���������ַ���
 *  \param [in]  inlen         -���������ַ�������
 *  \param [out] tagvalue      -����ָ����ǩ���ƺͳ��ȵ�ֵ
 *  \param [out] tagvaluelen   -����ָ����ǩ���ƺͳ��ȵ�ֵ�ĳ���
 *  \retval 0  ��ʾ�����ɹ�
 *  \retval <0 ��ʾ����ʧ��
 */
static U8 *centerm_find_app_tag( unsigned  char *tagname, S32 taglen, U8 *indata )
{
      U8 * p;
      S32 ilen=0;
      
      p = indata;
      while( *p )
      {
            if( memcmp( p, tagname, taglen ) == 0 )
            {     
                return p;//�ҵ���ǩ������
            }
            else
            {
                p ++;
            }
      }

      return NULL;
}

S32 centerm_get_tag_value_app( U8 *tagname, S32 taglen, U8 * indata,S32 inlen, U8 * tagvalue, S32 * tagvaluelen)
{
    U8 *p = indata;
    U8 *tmp = NULL;
    S32 ilen=0;
    
    while( p )
    {
        p = centerm_find_app_tag( tagname, taglen, p );
        if( (NULL == p)||( 0x0 == p[taglen] ) )
        {
            return -1;
        }    
        
        tmp = p;
            
        tmp+=taglen;//����tag������length��
        ilen=0;
        ilen += tmp[0] - 0x30;
        ilen *= 10;
        ilen += tmp[1] - 0x30;
        ilen *= 10;
        ilen += tmp[2] - 0x30;
        tmp += 3; 
        
        if( ilen > 40 )
        {
            p++;
        }
        else
        {
            p = tmp;
            break;
        }
    }
  
    memcpy( tagvalue, p, ilen );     
    (*tagvaluelen) = ilen;
    
    return 0;  
}

 //add szp 20130425
/**************************************************
Function:centerm_findapptag
Description:����uszRecdata���Ƿ���uszAppTag�еı�ǩ������н��ҵ��ı�ǩ��uszAppTag��ȥ��
Input: uszRecdata - ����ҵ����ݴ�
       nRecDatalen - ���ݴ�����
       uszAppTag - ��ǩ�б�
       nAppTaglen - ��ǩ�б���
*************************************************/
S32 centerm_findapptag(U8 *uszRecdata, S32 nRecDatalen, U8 *uszAppTag, S32 nAppTaglen)   
{
    U8 uszTagname[8] = {0};
    U8 * pPos = NULL;
    S32 nLoop = 0,nTaglen = 0;

    while(nLoop < nAppTaglen)
    {
        uszTagname[0] = uszAppTag[nLoop];
        if((uszTagname[0]&0x1F) == 0x1F)
        {
            uszTagname[1] = uszAppTag[nLoop+1];
            uszTagname[2] = 0x00;
            nTaglen = 2;
        }
        else
        {       
            uszTagname[1] = 0x00;
            nTaglen = 1;
        }
        
        pPos = centerm_find_tag(uszTagname, nTaglen, uszRecdata, nRecDatalen);
        if(pPos == NULL)
        {
            nLoop+=nTaglen;
            continue;

        }
        
        memcpy( uszAppTag+nLoop, uszAppTag+nLoop+nTaglen, nAppTaglen-nLoop );
        nAppTaglen -= nTaglen;      
    }
    
    pPos = NULL;
    
    return nAppTaglen;
}


/*!
 * \brief        centerm_packtTag      
 * \par ˵��     ���TLV���ݸ�ʽ    
 * \param[in]    nTagLen   ��ǩ�ĳ���       
 * \param[in]    sTag      ��ǩֵ     
 * \param[in]    nDatalen  ���ݵĳ���
 * \param[in]    sData     ����Դ
 * \param[out]   nOutLen   ������ݳ���
 * \param[out]   sOutData  ���������   
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int centerm_packtTag(int nTagLen,unsigned char *sTag,int nDatalen,unsigned char *sData,int * nOutLen ,unsigned char *sOutData)
{
    if ( ( nTagLen == 0 ) || ( sTag == NULL ) )
    {
        *nOutLen = 0;
        return RET_ERROR;
    }

    memcpy( sOutData, sTag, nTagLen );
    *(sOutData+nTagLen) = (char)nDatalen;
    memcpy( sOutData + nTagLen + 1, sData, nDatalen );
    *nOutLen = nTagLen + 1 + nDatalen;
    return RET_SUCCESS;

}

/*!
 * \brief        centerm_updateTag        
 * \par ˵��     ����TLV���ݸ�ʽ�е����е�TAG��ǩ����       
 * \param[in]    sTag        ��Ҫ���µ�Tag ��ǩ ,���ԭʼ������û�д�TAG������      
 * \param[in]    nTagLen     ��ǩ����      
 * \param[in]    sIndata     ���ܸ��ܵ�����Դ
 * \param[in]    nInDataLen  ����Դ����
 * \param[out]   nOutDataLen [out] ���º�����ݳ��� [in] ��������֮ǰ�ĳ���
 * \param[out]   sOutDta      ���º�����    
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int centerm_updateTag(char *sTag,int nTagLen,char * sIndata,int nInDataLen,int *nOutDataLen,char *sOutDta)
{
    int nLen = 0;
    int nTmpLen =0;
    int nRet = 0;
    int nTagValueLen =0;
    int nBuf1Len = 0;
    int nBuf2Len = 0;
    int nBuf3Len = 0;
    char sTagValue[1024] = {0x00};
    char sBuf1[1024] = {0x00};
    char sBuf2[1024] = {0x00};
    char sBuf3[1024] = {0x00};
    char sTmp[4096] = {0x00};
    char  * p;


    if ( ( sTag == NULL ) || ( nTagLen == 0 ) || ( sIndata == NULL ) || ( nInDataLen == 0 ) ) 
    {
        return RET_ERROR;
    }

    nLen = *nOutDataLen;
    nRet = centerm_get_tag_value( sTag, nTagLen, sIndata, nInDataLen, sTagValue, &nTagValueLen );
    if ( nRet == RET_SUCCESS )
    {
        memset( sTmp, 0x000, sizeof(sTmp) );
        nTmpLen =0;

        p = centerm_find_tag( sTag, nTagLen, sOutDta, nLen );
        /*<  û���ҵ����ݱ�ǩ������ */
        if( p == NULL)
        {
            centerm_packtTag( nTagLen, sTag, nTagValueLen, sTagValue, &nBuf1Len, sBuf1 );
            memcpy( sOutDta + nLen, sBuf1, nBuf1Len );
            *nOutDataLen = nLen+nBuf1Len;
        }
        /*<  �ҵ���ǩ �����ݷֳ�ǰ������ */
        else
        {
            memcpy( sBuf1, sOutDta,(p-sOutDta) );
            nBuf1Len = p-sOutDta;

            centerm_get_tag_value( sTag, nTagLen, sOutDta, nLen, sTmp, &nTmpLen );
            if (nTmpLen != 0)
            {
                nBuf3Len = nLen - nBuf1Len - nTagLen - 1 - nTmpLen;
                memcpy( sBuf3, p + nTagLen + 1 + nTmpLen, nBuf3Len );
                /*<  ��� */
                centerm_packtTag( nTagLen, sTag, nLen, sOutDta, &nBuf2Len, sBuf2 );
                memset( sOutDta, 0x00, nLen );
                memcpy( sOutDta, sBuf1,nBuf1Len );
                memcpy( sOutDta + nBuf1Len, sBuf2, nBuf2Len );
                memcpy( sOutDta + nBuf1Len + nBuf2Len, sBuf3, nBuf3Len );
                *nOutDataLen = nBuf1Len + nBuf2Len + nBuf3Len;
            }
            return RET_SUCCESS;  
        }
    }
    return RET_SUCCESS;
}


























































































































