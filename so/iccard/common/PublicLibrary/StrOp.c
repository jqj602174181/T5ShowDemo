#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include "GlobalDef.h"
#include "StrOp.h" 


/**************************************************
Function: strs_Trim
Description:ȥ�������ַ�����ȫ���Ļ��з��Լ��ո���Ʊ����
Input:      s -���������ַ���
   
Output:none
Return: ����ȥ�������ַ�����ȫ���Ļ��з��Լ��ո���Ʊ����
Others: 
*************************************************/
char *strs_Trim(char *s)
{
  long    lLen;
  register  int  i, k;
  
  /*  ����Ϊ����ֱ�ӷ���*/
  if(!s) return NULL;
  
  lLen = (long)strlen(s);
  for(i = 0, k = 0; i < lLen; i++)
  {
    if(' ' != s[i] && 9 != s[i] && 10 != s[i] && 13 != s[i])
      s[k++] = s[i];
  }
  if(k != lLen)
    s[k] = 0;
  return s;
}
/**************************************************
Function: GetDataLRC
Description:ȡ�����ݵ�LRCУ��ֵ
Input: pData -- У�������
          iLen    -- У������ݳ���
Output:none
Return: LRCֵ
Others: 
*************************************************/
U8 GetDataLRC( U8 *pData, S32 iLen )
{
      S32 i = 0;
      U8 Temp = 0;
      
      for( i = 0; i < iLen; i++ )
      {
         Temp ^= pData[ i ];
      }
      return Temp;
}

/**************************************************
Function:SplitData
Description: �������
Input:   pInData   --  ���������
            iInLen     -- ��������ݳ���
Output: pOutData -- ��ֺ������
Return: none
Others: �������ݲ�ֳɸߵ�4λ��ֱ����0x30,eg:0xEF -> 0x3E 0x3F
*************************************************/
void SplitData(U8 *pInData, S32 iInLen, U8 * pOutData )
{
    S32 i = 0;
    S32 j = 0;
    
    if( ( NULL == pInData )||( iInLen <= 0 )||( NULL == pOutData ) )
    {
       return;    
    }
    
    for( i = 0; i < iInLen; i++, j+=2 )
    {
        pOutData[ j ] = ((pInData[i] >> 4)&0xF)+0x30;
        pOutData[ j + 1 ] = (pInData[i]&0xF)+0x30;
    }

}


/**************************************************
Function:MergeData
Description: �ϲ����ݣ�ΪSplitData�����ķ������
Input:  pInData  -- ��������
          iInLen     -- ���ݳ���
Output: pOutData -- �ϲ��������
Return: none
Others: �ϲ�����Ϊ��0x30
*************************************************/
void MergeData( U8 *pInData, S32 iInLen, U8 * pOutData )
{
    S32 i = 0;
    S32 j = 0;
    
    if( ( NULL == pInData )||( (iInLen % 2) != 0 )||( NULL == pOutData ) )
    {
       return;    
    }  
    for( i = 0; i < (iInLen/2); i++, j+=2 )
    {
         pOutData[i] = ( (pInData[j]<<4)&0xF0 ) | (  pInData[j+1]&0xF  );
    }
}

void GetTransDate( U8 *szBuff )
{
    time_t  now;
    struct tm * pstTime;
    
    time(&now);
    pstTime = localtime( &now );
    
    sprintf( (char *)szBuff, "%.2d%.2d%.2d", pstTime->tm_year-100, pstTime->tm_mon+1, pstTime->tm_mday);
}

S32 AsciiToHex( U8 * szAscii, S32 nAsciiLen, U8 * szHex  )
{
      S32 i = 0;
      S32 j = 0;
      U8 ucTemp = 0;
      
      for( i = 0; i < nAsciiLen; i ++ )
      {
            if( ( szAscii[ i ] >= '0' )&&( szAscii[ i ] <= '9' ) )
            {
                 ucTemp = szAscii[ i ]-'0';
            }
            else if( ( szAscii[ i ] >= 'A' )&&( szAscii[ i ] <= 'F' ) )
            {
                 ucTemp = szAscii[ i ]-'A'+10;
            }
            else if( ( szAscii[ i ] >= 'a' )&&( szAscii[ i ] <= 'f' ) )
            {
                  ucTemp = szAscii[ i ] - 'a'+10;
            }
            else 
            {
                break;
            }
            
            /*����nAsciiLenΪ���������*/
            if( i+1 > nAsciiLen )
            {
                 break;
            }
            
            ucTemp = ucTemp*16;
            if( ( szAscii[ i+1 ] >= '0' )&&( szAscii[ i + 1 ] <= '9' ) )
            {
                  ucTemp += szAscii[ i + 1 ] - '0';
            } 
            else if( ( szAscii[ i + 1 ] >= 'A' )&&( szAscii[ i + 1 ] <= 'F' ) )
            {
                  ucTemp += szAscii[ i + 1 ] - 'A'+10;
            }
            else if( ( szAscii[ i + 1 ] >= 'a' )&&( szAscii[ i + 1 ] <= 'f' ) )
            {
                 ucTemp += szAscii[ i + 1 ] - 'a' + 10;
            }
            else 
            {
                break;
            }
          
            szHex[ j ] = ucTemp;
            j++;
            i++;
      }
      
      return j;    
}
S32 HexToAscii( U8 *szHex, S32 nHexLen, U8* szAscii )
{
    S32 i = 0;
    S32 j = 0;

	for( i = 0; i < nHexLen; i++, j+=2 )
    {
        szAscii[ j ] = ( ((szHex[i] >> 4)&0xF)  >=  0xA ) ? (((szHex[i] >> 4)&0xF) - 0xA+'A') : (((szHex[i] >> 4)&0xF)+0x30);
        szAscii[ j + 1 ] = ( ( szHex[ i ]&0xF)  >=  0xA ) ? ( (szHex[i]&0xF) - 0xA + 'A' ) : ((szHex[i]&0xF) + 0x30 );
    }
    szAscii[j] = '\0';
    return j;
}

S32 AsciiToBcd( U8 * szAscii, S32 nAsciiLen, U8 * szBcd , S32 nBcdLen )
{
   U8 szBuf[ MAX_LEN ] = {0};
   S32 nLen = 0;
   
   memset( szBuf, '0', sizeof(szBuf) );
   nLen = nAsciiLen > 2*nBcdLen ? 2*nBcdLen : nAsciiLen;
   memcpy( szBuf+(2*nBcdLen-nLen), szAscii, nLen );
   
   return AsciiToHex(  szBuf, 2*nBcdLen, szBcd );
}

S32 AsciiToCn( U8 * szAscii, S32 nAsciiLen, U8 * szCn , S32 nCnLen )
{
   U8 szBuf[ MAX_LEN ] = {0};
   S32 nLen = 0;
   
   memset( szBuf, 'F', sizeof(szBuf) );
   nLen = nAsciiLen > 2*nCnLen ? 2*nCnLen : nAsciiLen;
   memcpy( szBuf, szAscii, nLen );
   
   return AsciiToHex(  szBuf, 2*nCnLen, szCn );
}

S32 GetRandom( S32 nRndLen, U8*szRnd )
{
    S32 i = 0;
    
    srand( (U32)time(NULL) );
     
    for( i=0; i<nRndLen; i++ )
    {
        szRnd[ i ] = rand()%256;
        srand( (U32)time(NULL)+i+szRnd[i]  );
    }
    
    return RET_SUCCESS;
}

void GetTransTime( U8* szBuf )             
{
    time_t now;
    struct tm * pstTime;

    time( &now );
    pstTime = localtime( &now );
    sprintf( (S8 *)szBuf, "%.2d%.2d%.2d", pstTime->tm_hour, pstTime->tm_min, pstTime->tm_sec );
}


