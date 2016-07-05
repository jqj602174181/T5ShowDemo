/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:  TlvOp.c    
*
*  Author:       laiwenhui 整理
*
*  Date:        2012-06-14 15:04
*
*  Description: TLV格式处理函数
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
Description: 在TLV格式的数据indata中取得tagname指定标签的起始位置
Input: tagname  -- 标签名
          taglen      -- 标签名长度
          indata      -- TLV格式数据
          inlen        -- TLV数据长度
Output: none
Return: NULL -- 没有找到标签 ，other -- 指定的标签的起始地址
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
           while( indata[iloop] != 0x00 )//当遇到'\0'表明字符串结束
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
               //未找到标签
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
        tmptag[0]=indata[iloop];//保存第一个tag字节
        tmptaglen=1;//复位为1，因为tag可以为1到多个，iso标准
        if((indata[iloop]&0x1F)==0x1F)//如果是多个字节    
        {    
            iloop++;
            //可能后续还有节点，ISO标准中允许tag多个字节
            while((indata[iloop]&0x80)&&(iloop<inlen)) 
            {     
                  //保存tag名字
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
           tmptag[ tmptaglen ] = indata[ iloop ];//保存最后一个tag字节
           tmptaglen++;     
        }
        
        iloop++;    
        pp=&indata[iloop-tmptaglen];
        //判断是否找到tag
        if((memcmp(tagname,tmptag,tmptaglen)==0)&&(taglen==tmptaglen))
        {     
              p=&indata[iloop-tmptaglen];
              return p;//找到了tagname指定的 tlv字符串,则返回TLV起始地址
        }
        
        if( indata[ iloop - tmptaglen ]&0x20) //如果是复合tlv结构        
        {
               //跳过tlv中的长度字节
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
        else//不是复合结构，意味着需要跳过length和value，到达下个tlv结构的开始
        {                     
              if(indata[iloop]==0x82)//length段由3个字节组成，具体长度有2、3字节决定
              {
                    iloop+=( indata[ iloop + 1 ]*256+indata[ iloop + 2 ] )&0x0FFFF;
                    iloop+=3;
              }
              else if( indata[ iloop ] == 0x81 )//length段由2个字节组成，具体长度由第二个字节决定
              {
                    iloop += indata[iloop+1];
                    iloop += 2;
              }
              else//length段由1个字节组成，具体长度由该字节的低7位决定
              {
                    iloop +=indata[iloop]+1;
              }
        }                       
      }
      
      return p;
}
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
S32 centerm_get_tag_value(U8 *tagname,S32 taglen,U8 * indata,S32 inlen, U8 * tagvalue,S32 * tagvaluelen)
{
      U8 * p;
      S32 ilen=0;
    
      p = centerm_find_tag( tagname, taglen, indata, inlen );
      if( p == NULL)
      {
            return -1;//没有找到tagname标签
      }
        
      p+=taglen;//跳过标签
      ilen=0;
      //得到TLV的长度
      if( p[0] == 0x82 )//length段由3个字节组成，具体长度有2、3字节决定
      {
            ilen+=(p[1]*256+p[2])&0x0FFFF;
            p+=3;
      }
      else if( p[0] == 0x81)//length段由2个字节组成，具体长度由第二个字节决定
      {
            ilen=indata[1];
            p+=2;
      }
      else//length段由1个字节组成，具体长度由该字节的低7位决定
      {
            ilen=p[0];
            p+=1;
      }
      //取得v的值及其长度
      memcpy( tagvalue, p, ilen );     
      (*tagvaluelen)=ilen;
      return 0;  
}
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
U8 *centerm_find_tag_app(unsigned  char *tagname, S32 taglen, U8 *indata,S32 inlen)
{
      U8 * p;
      S32 ilen=0;
      
      p = indata;
      //这里通过循环的方式来查找tagname指定的标签起始地址
      while( *p )
      {
            if(memcmp(p, tagname, taglen) == 0)
            {     
                return p;//找到标签，返回
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
Description: 取得tag的数据，ptagdata 为标签的起始地址
Input:     taglen  --        标签名的长度
              ptagdata --    标签起始地址 
Output:  OutData --   标签的数据
              OutDataLen -- 标签的数据长度
Return: 0 -- 成功 1 -- 失败
Others: 
*************************************************/
S32 centerm_get_tag_data(S32 taglen,U8 * ptagdata,U8 *OutData,U8 * OutDataLen)
{
      ptagdata+=taglen;
      if((*ptagdata)==0x82)//length段由3个字节组成，具体长度有2、3字节决定
      {
            (*OutDataLen)=((*(ptagdata+1))*256+(*(ptagdata+2)))&0x0FFFF;
            memcpy(OutData,ptagdata+3,(*OutDataLen));
      }
      else if((*ptagdata)==0x81)//length段由2个字节组成，具体长度由第二个字节决定
      {
            (*OutDataLen)=(*(ptagdata+1));
            memcpy(OutData,ptagdata+2,(*OutDataLen));
      }
      else//length段由1个字节组成，具体长度由该字节的低7位决定
      {
            (*OutDataLen)=(*ptagdata);
            memcpy( OutData, ptagdata+1, (*OutDataLen) );
      }
      return 0;
       
}
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
               return indata[iloop]; //返回标签类型
        }
        iloop++; 
      }
      return 0;
}
/*! \brief 只取字符串里满足指定标签名称和长度的值。
 *  \par 说明:
 *       该函数是操作TLV编码格式的字符串，但T范围只限'A'-'Z'和'P'-'W'，L是3位。
 *  \param [in]  tagname       -指定标签名称
 *  \param [in]  taglen        -指定标签长度
 *  \param [in]  indata        -待操作的字符串
 *  \param [in]  inlen         -待操作的字符串长度
 *  \param [out] tagvalue      -返回指定标签名称和长度的值
 *  \param [out] tagvaluelen   -返回指定标签名称和长度的值的长度
 *  \retval 0  表示操作成功
 *  \retval <0 表示操作失败
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
                return p;//找到标签，返回
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
            
        tmp+=taglen;//跳过tag，到达length段
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
Description:查找uszRecdata中是否含有uszAppTag中的标签，如果有将找到的标签从uszAppTag中去除
Input: uszRecdata - 需查找的数据串
       nRecDatalen - 数据串长度
       uszAppTag - 标签列表
       nAppTaglen - 标签列表长度
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
 * \par 说明     打包TLV数据格式    
 * \param[in]    nTagLen   标签的长度       
 * \param[in]    sTag      标签值     
 * \param[in]    nDatalen  数据的长度
 * \param[in]    sData     数据源
 * \param[out]   nOutLen   输出数据长度
 * \param[out]   sOutData  输出的数据   
 * \retval       0 成功 -1  失败
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
 * \par 说明     更新TLV数据格式中的已有的TAG标签数据       
 * \param[in]    sTag        需要更新的Tag 标签 ,如果原始数据中没有此TAG则增加      
 * \param[in]    nTagLen     标签长度      
 * \param[in]    sIndata     可能更能的数据源
 * \param[in]    nInDataLen  数据源长度
 * \param[out]   nOutDataLen [out] 更新后的数据长度 [in] 更新数据之前的长度
 * \param[out]   sOutDta      更新后数据    
 * \retval       0 成功 -1  失败
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
        /*<  没有找到数据标签则增加 */
        if( p == NULL)
        {
            centerm_packtTag( nTagLen, sTag, nTagValueLen, sTagValue, &nBuf1Len, sBuf1 );
            memcpy( sOutDta + nLen, sBuf1, nBuf1Len );
            *nOutDataLen = nLen+nBuf1Len;
        }
        /*<  找到标签 将数据分成前后两段 */
        else
        {
            memcpy( sBuf1, sOutDta,(p-sOutDta) );
            nBuf1Len = p-sOutDta;

            centerm_get_tag_value( sTag, nTagLen, sOutDta, nLen, sTmp, &nTmpLen );
            if (nTmpLen != 0)
            {
                nBuf3Len = nLen - nBuf1Len - nTagLen - 1 - nTmpLen;
                memcpy( sBuf3, p + nTagLen + 1 + nTmpLen, nBuf3Len );
                /*<  组包 */
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


























































































































