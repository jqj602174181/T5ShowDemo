#include <stdlib.h>
#include <string.h>
#include "GlobalDef.h"
#include "ECash.h"
#include "TransTag.h"
#include "Debug.h"
#include "pboc.h"
#include "Des.h"
#include "stdio.h"


#define  DEVICE_KEY_LEN          16
#define  DEVICE_DATA_LEN         16


#ifndef DWORD
typedef unsigned long       DWORD;
#endif

/*<  读取记录时使用的标签结构体 */
typedef struct
{
    char name[4+1];  
    int flag;  
} TAGS; 

/*! AID 命令的标识 */
#define SELECT_FIRST 0x01
#define SELECT_NEXT  0x02

#define TERMINATE                 (-3)                 /*< 交易中止  */
#define GOTO                      (-4)                 /*< 返回交易  */
#define MAXLEN_ISSAPPDATA         (32)                 /*<  每条脚本最长32个字节 */

#define AID_LIST_LEN              1024                 /*<  IC 卡片中AID应用列表信息长度 */
static char g_IcAidList[AID_LIST_LEN];                 /*<  IC 卡片中AID应用列表信息 */
static int g_nAidListNum;                              /*< AID标签列表的个数  */

static unsigned char s_AidList[16][32+1];              /*< AID的长度最长32  个ASC字符  */
static unsigned char s_TcTag[30][3];                   /*< 标签最长2个字节  */
static unsigned char s_ArqcTag[30][3];


static unsigned char s_InfoTag[20][3];                /*< 获取IC卡信息标签  */
static ST_DEVICE_PARAM g_sDeviceParam;                /*< 设备结构体属性  */
static TERMPARAM g_sParam;                            /*< 串口结构体  */
static ICCARDPARAM g_ICCardParam;                     /*< 交易过程数据 全局结构体  */


/*<  静态数据标签 */
static unsigned char *sStaticTagList[]=
{
    EMV_TAG_9F79_IC_EC_BALANCE,     EMV_TAG_9F4F_IC_PBOCLOGFORMAT,         EMV_TAG_9F13_IC_LASTATC,
    EMV_TAG_9F78_IC_EC_STL,         EMV_TAG_9F6D_IC_EC_RESETTHRESHOLD,     EMV_TAG_9F17_IC_PINTRYCNTR,
    EMV_TAG_9F77_IC_EC_BALANCELIMIT,EMV_TAG_9F36_IC_ATC
};

/*<  基本标签列表 */
static unsigned char *s_BaseInfoTag[]=
{
    EMV_TAG_5A_IC_PAN,                 EMV_TAG_5F20_IC_HOLDERNAME,      EMV_TAG_5F20_IC_HOLDERNAME,      
    EMV_TAG_9F62_IC_CERTTYPE,          EMV_TAG_9F61_IC_CERTNO,          EMV_TAG_57_IC_TRACK2EQUDATA,     
    EMV_TAG_5F25_IC_APPEFFECTDATE,     EMV_TAG_5F24_IC_APPEXPIREDATE,   EMV_TAG_5F34_IC_PANSN,           
    EMV_TAG_8C_IC_CDOL1,               EMV_TAG_8D_IC_CDOL2,             EMV_TAG_9F08_IC_APPVERNO
    
};

/*<  TC默认报文 */
static unsigned char *s_TcTagDefault[]=
{
	EMV_TAG_9F33_TM_CAP,            EMV_TAG_95_TM_TVR,              EMV_TAG_9F37_TM_UNPNUM,         
	EMV_TAG_9F1E_TM_IFDSN,          EMV_TAG_9F10_IC_ISSAPPDATA,     EMV_TAG_9F26_IC_AC,          
	EMV_TAG_9F36_IC_ATC,            EMV_TAG_82_IC_AIP,              EMV_TAG_DF31_IC_IISSCRIRES,     
	EMV_TAG_9F1A_TM_CNTRYCODE,      EMV_TAG_9A_TM_TRANSDATE
};

/*<  ARQC默认报文 */
static unsigned char *s_ArqcTagDefault[]=
{
	EMV_TAG_9F26_IC_AC,             EMV_TAG_9F27_IC_CID,           EMV_TAG_9F10_IC_ISSAPPDATA,   
	EMV_TAG_9F37_TM_UNPNUM,         EMV_TAG_9F36_IC_ATC,           EMV_TAG_95_TM_TVR,  
	EMV_TAG_9A_TM_TRANSDATE,        EMV_TAG_9C_TM_TRANSTYPE,       EMV_TAG_9F02_TM_AUTHAMNTN ,    
	EMV_TAG_5F2A_TM_CURCODE,        EMV_TAG_82_IC_AIP,             EMV_TAG_9F1A_TM_CNTRYCODE,
	EMV_TAG_9F03_TM_OTHERAMNTN,     EMV_TAG_9F33_TM_CAP,           EMV_TAG_9F34_TM_CVMRESULT, 
	EMV_TAG_9F35_TM_TERMTYPE,       EMV_TAG_9F1E_TM_IFDSN ,        EMV_TAG_84_IC_DFNAME,           
	EMV_TAG_9F09_TM_APPVERNO,       EMV_TAG_9F41_TM_TRSEQCNTR
};


/*!
 * \brief        Pboc_SetAidNum      
 * \par 说明     设置AID列表的个数      
 * \param[in]    nNum  AID列表个数          
 * \retval       0 成功 -1  失败
 * \note                
 */
void Pboc_SetAidNum( int nNum )
{
    g_nAidListNum = nNum;
}

/*!
 * \brief        Pboc_GetAidNum       
 * \par 说明     获取AID列表的个数      
 * \param[out]   nNum    当前AID列表的个数
 * \retval       0 成功 -1  失败
 * \note                
 */
void Pboc_GetAidNum( int * nNum )
{
    *nNum = g_nAidListNum;
}

/*!
 * \brief        Pboc_Meth_AidList      
 * \par 说明     AID列表选择法          
 * \retval       0 成功 -1  失败
 * \note         暂时只选择一个      
 */
static int Pboc_Meth_AidList( int *nAIDNum )
{
    int nRecSum = 0;
    int nRet = 0;
    int i = 0;
    int nAidHexLen  = 0;
    int nFciLen  = 0;
    int nBufLen = 0;
    char cSelMode = SELECT_FIRST;
    char sAidHex[ 16+1 ] = { 0x00 };
    char c6D00Flag = 0;
    char sFci[ 256 ] = { 0x00 };
    char sBuf[ 1024 ] = { 0x00 };

    /**<获取终端支持的AID总条数*/
	DbgPrintf("【AID列表选择】\n");
    Pboc_GetAidNum( &nRecSum );
    DbgPrintf( "【AID列表选择】 获取终端AID的总条数为: %d\n",nRecSum );

    for( i = 0; i < nRecSum; i++ )
    {
        /**<已终端支持AID为DF名逐条选择*/
        memset( sAidHex, 0x00, sizeof(sAidHex) );
        nAidHexLen = strlen( s_AidList[i] )/2;
        AsciiToHex( s_AidList[i], strlen(s_AidList[i]), sAidHex );
        
        nRet = ICC_AppSelect( sAidHex, nAidHexLen, sFci, &nFciLen );
        DbgPrintf( "【AID列表选择】 选择AID %s 返回值:%2x\n", s_AidList[i], nRet );       
        
        if( nRet < 0 )
        {
            i++;
            cSelMode = SELECT_FIRST;
            continue;   
        }

        nRet = ICC_CardCmdGetErrCode();
        switch(nRet)
        {
            case 0x9000:
            {
					*nAIDNum = i+1;
                /*< 保存数据   6F 84  A5  50 87 9F38 5F2D 9F11 9F12 BF0C */
                memset(sBuf,0x00,sizeof(sBuf));
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_9F38_IC_PDOL, 2, sFci, nFciLen, sBuf,&nBufLen );
                if ( ( nRet == SUCC ) && ( nBufLen > 0 ) )
                {
                    tFunc_SaveTlvData( EMV_TAG_9F38_IC_PDOL, 2, sBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
                }
                
                /*<  保存 A5 */
                memset( sBuf, 0x00, sizeof(sBuf) );
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_A5_IC_FCIPROPTEMP, 1, sFci, nFciLen, sBuf, &nBufLen );
                if ( ( nRet == SUCC ) && (nBufLen > 0 ))
                {
                    tFunc_SaveTlvData(EMV_TAG_A5_IC_FCIPROPTEMP,1,sBuf,nBufLen,&g_ICCardParam.nIccAppDataLen,g_ICCardParam.szIccAppData);
                }

                /*<  保存 BF0C */
                memset( sBuf, 0x00, sizeof(sBuf) );
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_BF0C_IC_FCIDISCDATA, 2, sFci, nFciLen, sBuf, &nBufLen );
                if ( ( nRet == SUCC ) && ( nBufLen > 0 ))
                {
                    tFunc_SaveTlvData( EMV_TAG_BF0C_IC_FCIDISCDATA, 2, sBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
                }
                
                return SUCC;
            }
                
            case 0x6283:
            {
                /**<DF名与AID相同，终端使用其列表中的下一个AID向卡片发送SELECT命令*/
            
                /**<用同样的DF名选择下个应用*/
                continue;
            }
            
            case 0x6A81:
            {
                if(i == 0 && cSelMode == SELECT_FIRST)
                {
                    /**<第一次选择就返回卡片被锁定，
                    则终端结束卡片过程*/
                    return RET_AIDLIST_LOCK;
                }
                else
                {
                    i++;
                    cSelMode = SELECT_FIRST;
                    continue;
                }
            }
            
            case 0x9303: /*< 应用永远锁定 还需要验证  */
            {
                return  RET_CARD_LOCK_FOREVER;
            }

            case 0x6D00:
            {
                c6D00Flag++;
                i++;
                cSelMode = SELECT_FIRST;
                break;
            }
            
            default:
            {
                i++;
                cSelMode = SELECT_FIRST;
                continue;
            }
        }       
    }

    return SUCC;
}

/*!
 * \brief               Pboc_GetGACData
 * \par 说明            校验并保存GAC命令返回的数据
 * \param[in]           char *pszData:    GAC命令返回的数据 
 * \param[in]           int nLen:         数据长度 
 * \retval              0 成功 -1  失败
 * \note                
 */
int Pboc_GetGACData( char *pszData, int nLen )
{
    char szBuf[ 1024 ] = { 0 };
    int nBufLen = 0;
    int nRet    = FAIL;
    int nNum    = 0;
    int nIndex  = 0;
    char cCid   = 0;
    
	DbgPrintf("【解析GAC】\n");

    /*<  80模板 按照顺序解析数据 */
    if( !memcmp( pszData, EMV_TAG_80_IC_RMTF1, strlen(EMV_TAG_80_IC_RMTF1) ) )
    {
        centerm_get_tag_value( EMV_TAG_80_IC_RMTF1, strlen(EMV_TAG_80_IC_RMTF1), pszData, nLen, (BYTE *)szBuf, &nBufLen );
        if( nBufLen < 11 || nBufLen > (11 + MAXLEN_ISSAPPDATA) )
        {
            return FAIL;
        }
        
        tFunc_SaveTlvData( EMV_TAG_9F27_IC_CID, 2, szBuf,   1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F36_IC_ATC, 2, szBuf+1, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F26_IC_AC,  2, szBuf+3, 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
        if( nBufLen > 11 )
        {
            tFunc_SaveTlvData( EMV_TAG_9F10_IC_ISSAPPDATA, 2, szBuf+11, nBufLen-11, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
    }
    /*<  77模板  */
    else if( !memcmp( pszData, EMV_TAG_77_IC_RMTF2, 1 ) )
    {
        /**< CID必须存在 */
        nRet = centerm_get_tag_value( EMV_TAG_9F27_IC_CID, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        
        tFunc_SaveTlvData( EMV_TAG_9F27_IC_CID, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
        /**< ATC必须存在 */
        nRet = centerm_get_tag_value( EMV_TAG_9F36_IC_ATC, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F36_IC_ATC, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*<  保存应用密文数据 */
        memset( szBuf, 0, sizeof(szBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( EMV_TAG_9F26_IC_AC, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F26_IC_AC, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*<  保存发卡行自定义数据 */
        memset( szBuf, 0, sizeof(szBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( EMV_TAG_9F10_IC_ISSAPPDATA, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F10_IC_ISSAPPDATA, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
    }
    else
    {
        return FAIL;
    }
    DbgPrintf("【解析GAC】 成功\n");
    return SUCC;
}

 /*!
 * \brief           Pboc_FormatLog
 * \par 说明        交易格式
 * \param[in]       pszFmt   -- 交易格式
 * \param[in]       nLen     -- 交易格式的长度
 * \param[in]       pszData  -- 记录数据
 * \param[in]       nDataLen -- 记录数据的长度
 * \param[out]      ST_PBOC_DETAIL * stPbocDetail -- 转换后的交易结构体
 * \retval          0 -- 成功   -1 失败; 
 * \note                  
 */
int Pboc_FormatLog( char *pszFmt, int nLen, char *pszData, int nDataLen, ST_PBOC_DETAIL *stPbocDetail )
{
    int i = 3;
    int nOff = 0;

    DbgPrintf( "【读取日志格式】 \n" );
    DbgPrintHex( "【读取日志格式】 日志格式", pszFmt, nLen );
    DbgPrintHex( "【读取日志格式】 日志数据", pszData, nDataLen );

    memset( stPbocDetail, 0x00, sizeof(ST_PBOC_DETAIL) );
    
    /*< 数据内容为圈存数据  */
    if( !memcmp( pszFmt, EMV_TAG_DF4F_UPLOAD_FORMAT, 2 ) )
    {
        if( ( ( !memcmp( pszData, EMV_TAG_DF79_IC_DOUBLE_CUR_EC_BALANCE, 2 ) ) || ( !memcmp(pszData,EMV_TAG_9F79_IC_EC_BALANCE,2) ) ) && ( nDataLen >= 0x0e ) )
        {
            memcpy( stPbocDetail->sAmtType,   pszData,   2 );     /*< 币种类型  */
            memcpy( stPbocDetail->sBeforeAmt, pszData+2, 6 ); /*< 圈存前金额 */
            memcpy( stPbocDetail->sAfterAmt,  pszData+8, 6 );  /*< 圈存后金额 */
            nOff = 14;
        }
        else
        {
            return FAIL;
        }
    }

    if( pszFmt[2] == 0x00 )
    {
        return SUCC;
    }

    while ( i < nLen ) 
    {
        switch ( pszFmt[ i++ ] ) 
        {
            case '\x9A': /*< 交易日期;  */
            {
                if( pszFmt[i] != 0x03 ) 
                {
                    return FAIL;
                }
                
                memcpy( stPbocDetail->szDate, pszData+nOff, pszFmt[i] );
                DbgPrintHex( "【读取日志格式】 交易日期\n", stPbocDetail->szDate, 3 );
                
                break;
            }
            
            case '\x9C': /*< 交易类型; */
            {
                if ( pszFmt[i] != 0x01 ) 
                {
                    return FAIL;
                }
                stPbocDetail->cTranceType[ 0 ] = *(pszData+nOff);
                DbgPrintHex( "【读取日志格式】 交易类型", stPbocDetail->cTranceType, 1 );
                
                break;
            }
            
            case '\x9F':
            {
                switch ( pszFmt[i++] )
                {
                    case '\x21': /*< 交易时间  */
                    {
                        if ( pszFmt[i] != 0x03 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szTime,pszData+nOff,pszFmt[i] );
                        DbgPrintHex( " 【读取日志格式】 交易时间", stPbocDetail->szTime, 3 );
                        
                        break;
                    }
                    
                    case '\x02': /*< 授权金额  */
                    {
                        if ( pszFmt[i] != 0x06 )
                        {
                            return FAIL;
                        }
                        memcpy( stPbocDetail->szAmount, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( " 【读取日志格式】 授权金额", stPbocDetail->szAmount, 6 );
                        
                        break;
                    }
                    
                    case '\x03': /*< 其他金额  */
                    {
                        if ( pszFmt[i] != 0x06 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szOtherAmount, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( " 【读取日志格式】 其他金额", stPbocDetail->szAmount, 6 );
                        
                        break;
                    }
                    
                    case '\x1A': /*< 终端国家代码  */
                    {
                        if ( pszFmt[i] != 0x02 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szCountyCode, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( "【读取日志格式】 终端国家代码", stPbocDetail->szCountyCode, 2 );
                        break;
                    }
                    
                    case '\x4E': /*< 商户名称  */
                    {
                        if ( pszFmt[i] > 0x20 ) 
                        {
                            return FAIL;
                        }
                        
                        memset( stPbocDetail->szMerchen, 0x20, 20 );
                        memcpy( stPbocDetail->szMerchen, pszData+nOff, pszFmt[i] );
                        DbgPrintf( "【读取日志格式】 商户名称", stPbocDetail->szMerchen );
                        
                        break;
                    }
                    
                    case '\x36': /*< 交易计数器 */
                    {
                        if ( pszFmt[i] != 0x02 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szATC, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( "【读取日志格式】 交易计数器", stPbocDetail->szATC, 2 );
                        
                        break;
                    }
                    
                    default:
                    {
                        return FAIL;
                    }
                }
                break;
            }
            
            case '\x5F':
            {
                if ( pszFmt[i] == 0x2A ) /*< 交易货币代码  */
                {
                    i++;
                    if (pszFmt[i] != 0x02) 
                    {
                        return FAIL;
                    }
                    
                    memcpy( stPbocDetail->szCurCode, pszData+nOff, pszFmt[i] );
                    DbgPrintHex( "【读取日志格式】 交易货币代码", stPbocDetail->szCurCode, 2 );
                }
                break;
            }
            
            default:
            {
                return FAIL;
            }
            
        }// switch
        
        nOff += pszFmt[i];  
        i++;        
        
    }//while    
    return SUCC;
}

/*!
 * \brief       Pboc_IsAIDMatch       
 * \par 说明    查看AID是否在AID列表中      
 * \param[in]   pszICCAID      需要匹配的AID    
 * \param[in]   nICCAIDLen     AID的数据长度       
 * \param[out]  nIndex         匹配的索引       
 * \retval      YES 匹配  NO不匹配
 * \note        采用完全匹配的方式       
 */
YESORNO Pboc_IsAIDMatch( char *pszICCAID, int nICCAIDLen, int *nIndex )
{
    int i = 0;
    int nNum = 0;
    int nTermAIDLen = 0;
    int nAidHexLen = 0;
    char sAidHex[16+1] = {0x00};


    DbgPrintHex( "【AID匹配】\n", pszICCAID, nICCAIDLen );

    Pboc_GetAidNum( &nNum );
    DbgPrintf( "【AID匹配】 nNum:%d\n", nNum );

    for( i = 0; i< nNum; i++ )
    {
        AsciiToHex( s_AidList[i] , strlen(s_AidList[i]), sAidHex );

        if( !memcmp(sAidHex,pszICCAID,nICCAIDLen) )
        {
            *nIndex = i;
            DbgPrintf( "【AID匹配】 匹配成功！\n" );
            
            return YES;
        }
    }
    
    DbgPrintf( "【AID匹配】 匹配失败！\n" );   
    return NO;
}

/*!
 * \brief         Pboc_ReadDataByTags      
 * \par 说明      从卡片内部获取记录信息    
 * \param[in]     nDirect  0  从头到尾扫描  1  从尾到头扫描     
 * \param[in]     pTags       需要获取的TAG标签    
 * \param[in]     nTagCount   TAG的标签个数
 * \param[in]     szAfl       AFL入口
 * \param[in]     nAflLen     AFL的长度
 * \param[out]    szAppData   记录信息
 * \param[out]    pnAppDataLen记录信息的长度
 * \retval        0 成功 -1  失败
 * \note                
 */
int Pboc_ReadDataByTags( int nDirect, TAGS *pTags, S32 nTagCount, U8* szAfl, S32 nAflLen, U8 *szAppData, S32 *pnAppDataLen  )
{

    S32 i = 0;
    S32 j = 0;
    S32 n = 0;
    S32 count = 0;
    S32 use = 0;
    S32 nDataLen = 0;
    S32 nBufLen = 0;
    S32 start = 0;
    S32 all   = 0;
    S32 nRet = 0;
    S32 nValueLen = 0;
    U8  szData[ MAX_LEN ] = { 0 };
    U8 szValue[MAX_LEN] = {0x00};
    U8 szBuf[MAX_LEN] = {0x00};

    DbgPrintf( "【读取应用数据】\n");
    (*pnAppDataLen ) = 0;

    //从头扫到尾
    if( nDirect == 0 )
    {
        start = 0;
        all   = nAflLen/4;
    }
    //从尾扫到头
    else 
    {
        start = nAflLen/4-1;
        all   = 0;        
    }

    for( i = start; ( ( nDirect == 0 ) ? i < all : i >= 0 ); ( ( nDirect == 0 ) ?  i++ : i-- ) )
    {
        if( ( szAfl[4*i] < 1 ) || ( szAfl[4*i] > 100 ) )
        {
            continue;
        }
        j = nDirect ? szAfl[4*i+1] : szAfl[4*i+2];
        for( ;  (  nDirect ? j <= szAfl[4*i+2] : j >= szAfl[4*i+1] );  (nDirect ?  j++ : j-- ) )
        {

            memset( szData, 0x00, sizeof(szData) );
            nDataLen = 0;
            use = 0;

            if( RET_SUCCESS !=  ICC_CardCmdReadSFI(   szAfl[4*i]>>0x03, j,  szData,  &nDataLen ) )
            {
                DbgPrintf("【读取应用数据】 %s\n", ICC_CardCmdShowLastErrMsg() );
                continue;
            }
            else 
            {
                DbgPrintHex("【读取应用数据】", (U8 *)szData,  nDataLen );
                for( n=0; n < nTagCount; n++ )
                {

                    if( 1 != pTags[n].flag )
                    {
                        DbgPrintf("n=%d\n", n );
                        if( centerm_find_tag( (U8 *)pTags[n].name, strlen(pTags[n].name), szData,  nDataLen ) )
                        {
                            DbgPrintf("发现数据！\n");
                            use = 1;//找到有用数据
                            pTags[n].flag = 1;
                        }
                    }

                    /*<  保存记录中的数据 */
                    if( 1 == use )
                    {
                        DbgPrintf("【读取应用数据】 拷贝发现数据！\n");

                        memset( szValue, 0x00, sizeof(szValue));
                        nValueLen = 0;
                        nRet  = centerm_get_tag_value( EMV_TAG_70_TEMP, 1, szData, nDataLen, szValue, &nValueLen);

                        if ( ( nRet == RET_SUCCESS ) && ( nValueLen>0 ) )
                        {
                            /*<  保存数据到全局数组中 */
                            memset( szBuf, 0x00, sizeof(szBuf));
                            nBufLen = 0;
                            nRet = centerm_get_tag_value( pTags[n].name, strlen(pTags[n].name), szValue, nValueLen, szBuf, &nBufLen);
                            if ( ( nRet == RET_SUCCESS ) && ( nBufLen >0 ) )
                            {
                                tFunc_SaveTlvData( pTags[n].name, strlen(pTags[n].name), szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData);
                            }
                        }

                    }
                }
            }

            if ( use == 1 )
            {
                memcpy( szAppData+*pnAppDataLen, szValue, nValueLen);
                *pnAppDataLen += nValueLen;
            }
            //检查是否已经把所有tag获取到了
            count = 0;
            for( n=0; n<nTagCount; n++ )  
            {
                if( 1 == pTags[n].flag )
                {
                    count++;
                }
            }       
            DbgPrintf("count %d nTagCount %d\n", count, nTagCount );              
            if( count == nTagCount )
            {
                //需要读取的tag数据已经读取完毕
                return  RET_SUCCESS;   
            }

        }

    }

    DbgPrintHex("【读取应用数据】 读取的应用数据为", (U8 *)szAppData, *pnAppDataLen );  

    return RET_SUCCESS;
}

/*!
 * \brief       Pboc_SaveBasicTagData       
 * \par 说明    从输入数据中获取基本的交易TAG值，并进行保存     
 * \param[in]   sInData    数据源       
 * \param[in]   nInLen     数据源长度           
 * \retval      0 成功 -1  失败
 * \note                
 */
void Pboc_SaveBasicTagData(char * sInData,int nInLen)
{
    int i = 0;
    int nLen =0;
    int nRet = 0;
    char sData[1024] = {0x00};

    for ( i= 0; i<TABLESIZE(s_BaseInfoTag); i++)
    {
        nLen = 0;
        memset( sData, 0x00, sizeof(sData));
        nRet = centerm_get_tag_value( s_BaseInfoTag[i], strlen(s_BaseInfoTag[i]), sInData, nInLen, sData, &nLen);
        if ((nRet == RET_SUCCESS) && (nLen>0))
        {
            tFunc_SaveTlvData( s_BaseInfoTag[i], strlen(s_BaseInfoTag[i]), sData, nLen,  &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData);
            DbgPrintHex("Save Data\n", g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen);
        }
    }
}


/*!
 * \brief        Pboc_ReadRecordAll      
 * \par 说明     读取所有的记录文件，并保存数据     
 * \param[in]    szAfl        记录入口      
 * \param[in]    nAflLen      AFL长度      
 * \param[out]   szAppData    记录数据
 * \param[out]   pnAppDataLen 记录数据的长度    
 * \retval       0 成功 -1  失败
 * \note                
 */
S32 Pboc_ReadRecordAll( U8* szAfl, S32 nAflLen, U8 *szAppData, S32 *pnAppDataLen  )
{

    S32 i = 0;
    S32 j = 0;
    S32 nRet = 0;
    S32 nDataLen = 0;
    S32 nBufLen = 0;
    U8  szData[ MAX_LEN ] = { 0 };
    U8  szBuf[MAX_LEN] = {0x00};

    DbgPrintf( "读应用数据......\n");
    ( *pnAppDataLen ) = 0;    

    for( i = 0; i < ( nAflLen/4 ); i++ )
    {
        if( ( szAfl[4*i] < 1 ) || ( szAfl[4*i] > 100 ) )
        {
            continue;
        }

        for( j = szAfl[4*i+1];  j <= szAfl[4*i+2];  j++ )
        {

            memset( szData, 0x00, sizeof(szData) );
            nDataLen = 0;

            if( RET_SUCCESS !=  ICC_CardCmdReadSFI(   szAfl[4*i]>>0x03, j,  szData,  &nDataLen ) )
            {
                DbgPrintf("读应用数据错误 %s\n", ICC_CardCmdShowLastErrMsg() );
                continue;
            }
            else 
            {
                DbgPrintHex("应用数据", szData,  nDataLen );
            }
            
            /*<  从数据中解析 70 模板 */
            memset( szBuf, 0x00, sizeof(szBuf));
            nBufLen = 0;
            nRet  = centerm_get_tag_value( EMV_TAG_70_TEMP, 1,  szData, nDataLen, szBuf, &nBufLen);
            if (nRet == RET_SUCCESS)
            {
                 Pboc_SaveBasicTagData( szBuf, nBufLen);
            }
            
            memcpy( szAppData + *pnAppDataLen, szBuf, nBufLen);
            *pnAppDataLen += nBufLen;
        }
    }

    DbgPrintHex("读取的应用数据为", szAppData, *pnAppDataLen );  

    return RET_SUCCESS;
}

/*!
 * \brief           tFunc_ReadRecordData   
 * \par 说明        读取卡片中的记录文件    
 * \param[in]       nType      0 将卡片中的记录都读取出来   1 只是读取需要的TAG   2 只是读取 8C 8D
 * \param[in]       szAppTag   读取的标签列表   
 * \param[in]       nCount     读取的标签列表的个数
 * \param[out]      pnValueLen 读取出的记录数据长度
 * \param[out]      sTagValue  读取出的记录数据
 * \retval          0 成功 -1  失败
 * \note                
 */
int tFunc_ReadRecordData(int nType,char * szAppTag[],int nCount,int * pnValueLen,char * sTagValue)
{
    int nRet = 0;
    int nAflLen = 0;
    int i = 0;
    int j = 0;
    int nNum = 0;
    char sAfl[128] = {0x00};
    TAGS *pstTag;
    char sBuf[1024] = {0x00};
    int nBufLen =0;
    char sRecordData[4096] = {0x00};
    int nRecordLen =0;

    
    ASSERT_RET( centerm_get_tag_value( EMV_TAG_94_IC_AFL, 1, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sAfl, &nAflLen ) );

    if(nType == 0) /*< 全部读取  */
    {
        nRet =  Pboc_ReadRecordAll( sAfl, nAflLen, sRecordData, &nRecordLen );
        DbgPrintHex("App_tFunc_ReadRecordAll \n", sRecordData, nRecordLen );
    }
    else if (nType == 1) /*<  只读取需要的TAG数据  */
    {
        
        if ( nCount>=1 )
        {
            pstTag = ( TAGS  *)malloc( nCount *sizeof(TAGS) );
        }
        for ( i=0; i < nCount ; i++)
        {
            memset( &pstTag[i], 0x00, sizeof(TAGS));
            memcpy( pstTag[i].name, szAppTag[i], strlen(szAppTag[i]));
            pstTag[i].flag = 0;
            /*<  查看传输进来的标签是否为静态数据，如果为静态数据，则不进行获取 */
            for ( j = 0; j<TABLESIZE(sStaticTagList); j++ )
            {
                if ( memcmp( szAppTag[i], sStaticTagList[j], strlen( sStaticTagList[j] ) ) == 0 )
                {
                    pstTag[i].flag = 1;
                }
            }
        }
        DbgPrintf("Read Tag Count %d\n", nCount);
        nRet =  Pboc_ReadDataByTags( 0, pstTag,nCount, sAfl, nAflLen, sTagValue, pnValueLen );
        DbgPrintHex("Pboc_ReadDataByTags sTagValue\n", sTagValue, *pnValueLen);

    }
    else if(nType == 2) /*<  只读取CDOL1  CDOL2 */
    {
        nRet = ICC_TransReadCDOL( sAfl, nAflLen,sTagValue, pnValueLen );
    }
    else
    {
        DbgPrintf("tFunc_GetRecordData  param err\n");
        return RET_PARAM_ERR;
    }

    return nRet;
}

/*!
 * \brief           Pboc_GetIcStaticData   
 * \par 说明        获取静态数据    
 * \param[in]       sTag         需要获取静态数据的tag      
 * \param[in]       nTagLen      标签的长度
 * \param[out]      pRecordData  返回TLV格式的数据值
 * \param[out]      pRecordLen   数据的长度
 * \retval          0 成功 -1  失败
 * \note                
 */
int Pboc_GetIcStaticData( char *sTag,  int nTagLen, char * pRecordData,int  * pRecordLen )
{

    return ICC_CardCmdGetData( sTag[0], sTag[1], pRecordData,  pRecordLen );

}

/*!
 * \brief          Pboc_PackDomain        
 * \par 说明       打包55 域、TC报文    
 * \param[in]      nType   0 打包55域报文    1 打包TC报文
 * \param[out]     nOutLen    输出报文长度
 * \param[out]     sOutData   输出报文数据
 * \retval         0 成功 -1  失败
 * \note                
 */
int  Pboc_PackDomain(int nType,int * nOutLen ,char * sOutData)
{
    int i = 0;
    int nRet = 0;
    int nDataLen = 0;
    int nNum = 0;
    int nBufLen = 0;
    int nPointLen  = 0;
    int nTagLen = 0;
    char sBuf[1024] = {0x00};
    char sData[1024] = {0x00};
    char sTag[3+1] = {0x00};

    /*<  组织55域 、 TC 数据 有些数据元必须强制，可以增加限制 */ 
    if (nType == 0)
    {
        nNum = TABLESIZE( s_ArqcTag );
    }
    else
    {
        nNum = TABLESIZE( s_TcTag );
    }
    
    for ( i = 0; i<nNum; i++)
    {
        /*< 为空  */
        memset( sTag, 0x00, sizeof(sTag));
        if ( nType == 0 )
        {
            nTagLen = strlen( s_ArqcTag[i] );
            if ( nTagLen >0 )
            {
                memcpy( sTag, s_ArqcTag[i], nTagLen);
            }
        }
        else
        {
            nTagLen = strlen( s_TcTag[i] );
            if ( nTagLen >0 )
            {
                memcpy( sTag, s_TcTag[i], nTagLen);
            }
        }
        
        if ( sTag[0] == 0x00 )
        {
            break;
        }
        DbgPrintHex("Pboc_PackDomain\n", sTag, nTagLen);
        DbgPrintf("Len %d\n", nTagLen);

        memset( sBuf, 0x00, sizeof(sBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( sTag, nTagLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sBuf, &nBufLen );
        DbgPrintHex("data\n", sBuf, nBufLen);
        if ( nRet == RET_SUCCESS )
        {
            nDataLen = 0;
            memset( sData, 0x00, sizeof(sData) );
            nRet =  centerm_packtTag( nTagLen, sTag, nBufLen, sBuf, &nDataLen, sData );
            if (nRet == RET_SUCCESS)
            {
                memcpy( sOutData + nPointLen, sData, nDataLen );
                nPointLen += nDataLen;
            }
        }
        else
        {
            DbgPrintHex("get tag fail :\n ", sTag, nTagLen);
			if ( memcmp( sTag, EMV_TAG_9F26_IC_AC, 2) == 0 || \
					( memcmp( sTag, EMV_TAG_9F27_IC_CID, 2) == 0) || \
					( memcmp( sTag, EMV_TAG_9F10_IC_ISSAPPDATA, 2) == 0) )
			{
				*nOutLen = nPointLen;
				nRet = RET_ERROR;
			}
            
        }
    }

    *nOutLen = nPointLen;
    return SUCC;
}



/*!
 * \brief         tFunc_ConvertData     
 * \par 说明      转换数据  （将卡号和二磁等效数据进行转换）
 * \param[in]     szTagvalue [IN] 需要转换的数据   [OUT] 转换后的数据   
 * \param[in]     ValueLen   [IN] 转换前数据长度   [OUT] 转换后的数据长度       
 * \retval       
 * \note                
 */
void  tFunc_ConvertData( unsigned char* szTagvalue, int *ValueLen )
{
    int nRetVal = RET_SUCCESS;
	unsigned char szTmp[ MAX_LEN ] = {0x00};
	unsigned char uszBuf[ MAX_LEN ] = {0x00};
    int i = 0;
    int nLen = 0;
    int nCount = 0;
    
    nLen = *ValueLen;
    memset( szTmp, 0x0, sizeof(szTmp) );
    memcpy( uszBuf, szTagvalue, nLen);

    for( i=0; i<nLen; i++ )
    {
        if(uszBuf[i] == 'F')
        {
            break;
        }
        if(uszBuf[i] == 'D')
        {
            uszBuf[i]= '=';
        }
    }         
    *ValueLen = i;
    memcpy( szTagvalue, uszBuf, i );    
}



/*!
 * \brief         Pboc_OrgLogDetail     
 * \par 说明      按照通用格式组织数据      
 * \param[in]     stIndata   ST_PBOC_DETAIL结构体数据   
 * \param[in]     nNum       从卡片中读取出的日志的条数    
 * \param[out]    sOutData   组织后的数据格式 
 * \retval        0 成功 -1  失败
 * \note          记录条数2字节+每条记录长度3字节+ TLV(1字节标签+3字节长度+数据)格式记录
 例如：
 03106P012000000022200Q012000000000000R0040156S006141120T00260U006163101V0040156W020CHINA ZHENGZHOU BANKX0040007P012000000299700Q012000000000000R0040156S006141120T00260U006163025V0040156W020CHINA ZHENGZHOU BANKX0040005P012000000033300Q012000000000000R0040156S006141120T00260U006162958V0040156W020CHINA ZHENGZHOU BANKX0040002
 */
int Pboc_OrgLogDetail(ST_PBOC_DETAIL stIndata,int nNum, char * sOutData)
{
    int nLen = 0;
    unsigned char sBuf[40+1] = {0x00};


    /*<  记录数+ 每条记录长度（固定） */
	memset( sBuf, 0x00, sizeof(sBuf) );
    sprintf( sBuf, "%02d", nNum );
    sprintf( sBuf+2, "%03d", 106 );
	memcpy( sOutData, sBuf, 5);

    nLen = ( nNum - 1 )*106 + 5;

    /*< 授权金额P  */    
    sOutData[nLen++] = 'P';
    sprintf( sOutData + nLen, "%03d", 12 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szAmount, 6 ,sBuf );
	memcpy( sOutData + nLen, sBuf, 12 );
    nLen += 12;

    //  其它金额Q
    sOutData[nLen++] = 'Q';
    sprintf( sOutData+nLen, "%03d", 12 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szOtherAmount, 6 , sBuf );
	memcpy( sOutData + nLen, sBuf, 12 );
    nLen += 12;

    //  交易货币代码R
    sOutData[nLen++] = 'R';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szCurCode, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;

    // 交易日期S
    sOutData[nLen++] = 'S';
    sprintf( sOutData + nLen, "%03d", 6 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szDate, 3, sBuf );
	memcpy( sOutData + nLen, sBuf, 6 );
    nLen += 6;

    //  交易类型T
    sOutData[nLen++] = 'T';
    sprintf( sOutData + nLen, "%03d", 2 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.cTranceType, 1, sBuf );
	memcpy( sOutData + nLen, sBuf, 2 );
    nLen += 2;

    //  交易时间U
    sOutData[nLen++] = 'U';
    sprintf( sOutData + nLen, "%03d", 6 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szTime, 3, sBuf );
	memcpy( sOutData + nLen, sBuf, 6 );
    nLen += 6;

    //  终端国家代码V
    sOutData[nLen++] = 'V';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szCountyCode, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;

    //  商户名称W
    sOutData[nLen++] = 'W';
    sprintf( sOutData + nLen, "%03d", 20 );
    nLen += 3;
    memcpy( sOutData + nLen, stIndata.szMerchen, 20 );
    nLen += 20;

    //  应用交易计数器（ATC）X
    sOutData[nLen++] = 'X';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szATC, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4; 

    sOutData[nLen] = 0x00;

    DbgPrintf("App_LogFunc out Data %s\n", sOutData );
    return RET_SUCCESS;
}



/*!
 * \brief         Pboc_OrgCredLogDetail     
 * \par 说明      按照通用格式组织数据  （圈存交易格式）    
 * \param[in]     stIndata     ST_PBOC_DETAIL格式单条交易数据   
 * \param[in]     nNum         从卡片中读取的圈存交易记录的条数
 * \param[out]    sOutData     组织后的圈存数据    
 * \retval        0 成功 -1  失败
 * \note      
 例如：
9F79|000000033300|000000055500|141120|163101|0156|CHINA ZHENGZHOU BANK|0007|9F79|000000000000|000000033300|141120|162958|0156|CHINA ZHENGZHOU BANK|0002|
 */
int Pboc_OrgCredLogDetail(ST_PBOC_DETAIL stIndata,int nNum, char * sOutData)
{
    int nLen = 0;
    unsigned char sBuf[40+1] = {0x00};

    nLen = strlen(sOutData);

    /*< 金额类型P  */    
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sAmtType, 2,sBuf );
    memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;
    sOutData[nLen++] = '|';

    /*<  下发脚本前电子现金余额 */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sBeforeAmt, 6, sBuf );
    memcpy( sOutData + nLen, sBuf, 12 ); 
    nLen += 12;
    sOutData[nLen++] = '|';

    /*< 下发脚本后电子现金余额  */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sAfterAmt, 6, sBuf );
    memcpy( sOutData + nLen, sBuf, 12 ); 
    nLen += 12;
    sOutData[nLen++] = '|';

    /*<  交易日期 */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szDate, 3, sBuf );
    memcpy( sOutData + nLen, sBuf, 6);  
    nLen += 6;
    sOutData[nLen++] = '|';

    /*<  交易时间 */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szTime, 3, sBuf );
    memcpy( sOutData + nLen, sBuf, 6 );  
    nLen += 6;
    sOutData[nLen++] = '|';


    /*< 应用交易计数器  */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii(  stIndata.szATC, 2, sBuf );
    memcpy( sOutData + nLen, sBuf, 4 );  
    nLen += 4;
    sOutData[nLen++] = '|';

    sOutData[nLen] = 0x00;

    DbgPrintf("App_LogFunc out Data %s\n", sOutData );
    return RET_SUCCESS;
}



/*!
 * \brief             tFunc_SetDeviceParameter
 * \par 说明          设置设备的属性
 * \param[in]         pstDeviceParam  ST_DEVICE_PARAM设备属性设置
 * \retval            0 成功 -1  失败
 * \note                
 */
int tFunc_SetDeviceParameter(ST_DEVICE_PARAM  *pstDeviceParam)
{
    int nRet = RET_SUCCESS;

	DbgPrintf("【设置设备属性】\n");

	memset( &g_sDeviceParam, 0x00, sizeof(ST_DEVICE_PARAM) );
	if (pstDeviceParam != NULL)
	{
		memcpy( &g_sDeviceParam, pstDeviceParam, sizeof(ST_DEVICE_PARAM) );
	}
    return RET_SUCCESS;
}

/*!
 * \brief             tMidFunc_OpenPort 
 * \par 说明          打开串口
 * \param[in]         nPortComNo  端口号    
 * \param[in]         nBaund      波特率
 * \retval            0 成功 -1  失败
 * \note                
 */
int tFunc_OpenPort( int  nPortComNo, int  nBaund)
{
    int nReturn = 0;
    int m_Baund = 0;
    char szBP_Cmd[3] = {0};

	DbgPrintf("【打开串口】\n");
    if (nBaund == 9600)
    {
        m_Baund = BUAD_RATE;
    }
    m_Baund = nBaund;

    ICC_InitCardReaderParam( &g_sParam );
    if ( ( nPortComNo >= 1 ) && ( nPortComNo <= 20 ))
    {
        g_sParam.nCom = nPortComNo;
        g_sParam.nBaud = 9600; 
        g_sParam.nTransType = TYPE_COM;
        nReturn = ICC_OpenCardReader( &g_sParam );
    }
    else if( nPortComNo == 1001)
    {
        g_sParam.nTransType = TYPE_HID;
        g_sParam.nPID = HID_PID;
        g_sParam.nVID = HID_VID; 
        nReturn = ICC_OpenCardReader( &g_sParam );
        if( RET_SUCCESS != nReturn)
        {
            g_sParam.nTransType = TYPE_PCSC;
            g_sParam.nPID = HID_PID;
            g_sParam.nVID = HID_VID; 
            nReturn = ICC_OpenCardReader( &g_sParam );
        }
    }
    else
    {
        return RET_PARAM_ERR;
    }

    if( RET_SUCCESS != nReturn)
    {
        return RET_OPEN_PORT_FAIL;
    }
    
    //提升临时波特率指令
    if(g_sParam.nTransType != TYPE_PCSC)
    {
        ICC_CardReaderCmd( (unsigned char *)ICC_PORT_9600, 4 );
        if( ( m_Baund != 9600 ) && ( g_sParam.nTransType == TYPE_COM ) )
        {
            g_sParam.nBaud = m_Baund;
            nReturn = ICC_CardReaderSynBps( &g_sParam, 0 );
            if(RET_SUCCESS != nReturn)
            {
                return RET_OPEN_PORT_FAIL;
            }
        }
    }

    return RET_SUCCESS;
}

/*!
 * \brief        tFunc_ClosePort      
 * \par 说明     关闭串口           
 * \retval       0 成功 -1  失败
 * \note                
 */
int tFunc_ClosePort()
{
    int nRet = RET_SUCCESS;

	DbgPrintf("【关闭串口】\n");

    /*CCID设备不接模块切换命令*/
    if( g_sParam.nTransType != TYPE_PCSC )   
    {
        if( ( g_sParam.nBaud != 9600 ) && ( g_sParam.nTransType == TYPE_COM ) )
        {
            ICC_CardReaderResetBps(0);
        }
    //zxy   ICC_CardReaderCmd((unsigned char *) ICC_DEFAULT_PORT, 3 );
    }
    nRet =  ICC_CloseCardReader( ); 
    if ( nRet != RET_SUCCESS )
    {
        nRet = RET_CLOSE_PORT_FAIL;
    }
    return nRet;
}
/*!
 * \brief            tFunc_SetExPort 
 * \par 说明         设置转口   
 * \param[in]        cExPort  转口号
 * \retval           0 成功 -1  失败
 * \note                
 */
int tFunc_SetExPort ( char cExPort )
{
    int nRet = RET_SUCCESS;
    char szBP_Cmd[8+1] = {0x00};

	DbgPrintf("【设置转口】\n");

    //下发BP盒指令
    if( ( 'A' ==  cExPort ) ||  ( 'B' ==  cExPort ) || ( 'C' ==  cExPort  ) || ( 'K' ==  cExPort ) )
    {
        memset( szBP_Cmd, 0x00, sizeof(szBP_Cmd) );
        memcpy( szBP_Cmd, BP_CMD, 2 );
        szBP_Cmd[2] = cExPort;
        if( g_sParam.nTransType == TYPE_COM )
        {
            nRet =  ICC_CardReaderCmd( szBP_Cmd, 3);
            if ( nRet != RET_SUCCESS )
            {
                nRet = RET_EX_PORT_FAIL;
            }
        }
    }
    else
    {
        nRet = RET_PARAM_ERR;
    }
    return nRet;
}



/*!
 * \brief        tFunc_PowerOn      
 * \par 说明     对卡片进行上电     
 * \param[out]    pATRLen  IC上电数据的数据长度
 * \param[out]    pATR     IC卡上电返回数据 Hex码格式
 * \retval        0 成功 -1  失败
 * \note                
 */
int tFunc_PowerOn( int *pATRLen, unsigned char *pATR)
{
    int nRet = RET_PARAM_ERR;
    int nSlot = 0;
    int nAtrLen = 0;
    unsigned char uszAtr[256] = {0x00};

    *pATRLen = 0;

    /*CCID设备不接受上电命令*/
    if( g_sParam.nTransType == TYPE_PCSC  )
    {
        return 0;
    }

	ASSERT_RET ( tFunc_DeviceAuth() );

    tFunc_GetSlot( &nSlot );
    nRet = ICC_CardReaderPowerOn( nSlot , uszAtr, &nAtrLen, 0 );
    if( RET_SUCCESS == nRet )
    {
        if( pATRLen && pATR )
        {
            memcpy( pATR, uszAtr, nAtrLen );
            *pATRLen = nAtrLen;
            pATR[*pATRLen] = 0x00;
        }
        nRet = RET_SUCCESS;
    }
    else
    {
        nRet = ICC_CardReaderGetRecStatus( );
        if (nRet == -1002) /*< 接触式用户卡未插到位   */
        {
            nRet = RET_TOUCH_CARD_OUT;
        }
        else if (nRet == -1001) /*< 不支持接触式用户卡  */
        {
            nRet = RET_UNSUPT_TOUCH_CARD;
        }
        else if (nRet == -1003) /*< 接触式用户卡已经上电  */
        {
            nRet = RET_TOUCH_CARD_ON;
        }
        else if (nRet == -3004) /*<  非接触式卡未激活 */
        {
            nRet = RET_UNTOUCH_OFF;
        }
        else if (nRet == -3005 ) /*<  非接触式卡激活失败 */
        {
            nRet = RET_UNTOUCH_ON_FAIL;
        }
    }

    DbgPrintf("CT_PowerOn %d\r\n",nRet);

    return nRet;
}


/*!
 * \brief           tFunc_PowerOff    
 * \par 说明        对所设置的卡槽号进行下电操作        
 * \retval          0 成功 -1  失败
 * \note                
 */
int tFunc_PowerOff()
{
    int nSlot = 0;
    int nRet = 0;
    /*<  获取卡槽号 */
    memset( &g_sParam, 0x00, sizeof(g_sParam) );

    tFunc_GetSlot(&nSlot);

    if( g_sParam.nTransType == TYPE_PCSC )
    {
        return 0;
    }
    nRet =  ICC_CardReaderPowerOff( nSlot );
    if( nRet != RET_SUCCESS )
    {
        nRet = ICC_CardReaderGetRecStatus( );

        nRet = RET_POWER_OFF_FAIL;
    }
    return nRet;
}

/*!
 * \brief          tFunc_SetSlot    
 * \par 说明       设置卡槽号   
 * \param[in]      nSlot  需要设置的卡槽号      
 * \retval         0 成功 -1  失败
 * \note                
 */
void tFunc_SetSlot(int nSlot)
{
    ICC_CardReaderSetCardSlot( nSlot );
}

/*!
 * \brief          tFunc_GetSlot    
 * \par 说明       获取当前卡槽号   
 * \param[out]     nSlot  当前卡槽号
 * \retval         0 成功 -1  失败
 * \note                
 */
void tFunc_GetSlot(int * nSlot)
{
    *nSlot =  ICC_CardReaderGetCardSlot();
}
/*!
 * \brief        tFunc_CardPresent      
 * \par 说明     nSlot      
 * \param[in]    对输入的卡槽号进行卡在位判断       
 * \retval       0 成功 -1  失败
 * \note                
 */
int tFunc_CardPresent( int nSlot)
{
    int nReturn = RET_PARAM_ERR;
    
	DbgPrintf("【卡在位检测】 %d \n", nSlot);
    /*<  接触卡片 */
    if( ( nSlot >= 0x00 ) && ( nSlot <=0x0F ) )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
        if( RET_SUCCESS == nReturn )
        {
            ICC_CardReaderSetCardSlot( nSlot );
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn == -1003 )
            {
                nReturn = TOUCH_CARD_IN_UP;//接触在位已经上电
            }
            else if( nReturn == -1004 )
            {
                nReturn = TOUCH_CARD_IN;//接触在位未上电
            }
            else
            {
                nReturn = RET_OTHER_ERR;// 未知错误
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//超时
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    /*< 非接卡片  */
    else if( nSlot == 0xFF )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( PICC_CARD_SLOT );
        if( RET_SUCCESS == nReturn )
        {
            ICC_CardReaderSetCardSlot( PICC_CARD_SLOT );
            if ( ICC_CardReaderGetRecStatus( ) == 0 )
            {
                nReturn = NOTOUCH_CARD_IN_UP;
            }
            else
            {
                nReturn = RET_OTHER_ERR;
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//超时
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    /*< PSAM卡  */
    else if ( ( nSlot >= 0x10 ) && ( nSlot <= 0x1F ) )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
        if( SMART_CARD == nReturn )
        {
            ICC_CardReaderSetCardSlot( SMART_CARD );
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn == -1003 )
            {
                nReturn = TOUCH_CARD_IN_UP;//接触在位已经上电
            }
            else if( nReturn == -1004 )
            {
                nReturn = TOUCH_CARD_IN;//接触在位未上电
            }
            else
            {
                nReturn = RET_OTHER_ERR;// 未知错误
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//超时
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    return nReturn;
}
/*!
 * \brief         tFunc_SendCommand    
 * \par 说明      发送命令  
 * \param[in]     nCmdLen       需要发送的命令长度
 * \param[in]     pszCmd        需要发送的命令数据
 * \param[out]    pnRespLen     卡响应的命令数据长度
 * \param[out]    pszResp       卡响应的命令数据
 * \retval        0 成功 -1  失败
 * \note                
 */
int tFunc_SendCommand(int nCmdLen, unsigned char *pszCmd,int *pnRespLen, unsigned char *pszResp)
{
    int nRet = RET_PARAM_ERR;
    int nReadTime = 0;
    int nRecvLen = RET_SUCCESS;
    unsigned char  ucRecvData[ 512 ] = {0};
    //DWORD dwTick;
    
    //dwTick = GetTickCount();
    *pnRespLen = 0;

	DbgPrintf("【发送命令】\n");
    nRet = ICC_CardCmdVisitCOS( pszCmd, nCmdLen, ucRecvData ,&nRecvLen );
    if(nRet == RET_SUCCESS)
    {
        memcpy( pszResp, ucRecvData, nRecvLen );
        *pnRespLen = nRecvLen;
        pszResp[*pnRespLen] = 0x00;
        nRet = RET_SUCCESS;
    }
    else
    {
        if ( nRet != RET_SUCCESS )
        {
            nRet = ICC_CardReaderGetRecStatus( );
            if (nRet<0)
            {
                nRet = RET_SEND_APDU_FAIL;
            }
        }
        *pnRespLen = 0;
        pszResp[0] = 0x00;
    }
    //nReadTime = GetTickCount() - dwTick;
    DbgPrintf("CT_SendCommand Time:%d\r\n",nReadTime);

    return nRet;

}


/*!
 * \brief            tFunc_GetCmdErr  
 * \par 说明         输出当前的错误码   
 * \param[out]       pnErrCode  当前错误码
 * \retval           0 成功 -1  失败
 * \note                
 */
void tFunc_GetCmdErr(int * pnErrCode)
{
    *pnErrCode = ICC_CardCmdGetErrCode( );
}

/*!
 * \brief       tFunc_SaveTlvData       
 * \par 说明    存储TLV格式数据     
 * \param[in]   sTag         需要存储的TAG      
 * \param[in]   nTagLen      TAG的长度     
 * \param[in]   sData        存储的数据
 * \param[in]   nDataLen     存储的数据长度
 * \param[out]  nOutDataLen  存储后数据长度
 * \param[out]  sOutData     存储后数据    
 * \retval      0 成功 -1  失败
 * \note                
 */
int tFunc_SaveTlvData(char *sTag,int nTagLen,char *sData,int nDataLen,int *nOutDataLen,char *sOutData)
{
    int nLen =0;
    if ( (nDataLen == 0)  || (nTagLen == 0 ) )
    {
        return RET_ERROR;
    }

    nLen = *nOutDataLen;
    memcpy( sOutData + nLen, sTag, nTagLen );
    nLen += nTagLen;
    sOutData[nLen++] = (char)nDataLen;
    memcpy( sOutData + nLen, sData, nDataLen );
    nLen += nDataLen;

    *nOutDataLen = nLen;
    return RET_SUCCESS;
    
}

/*!
 * \brief           tFunc_SetTermParam   
 * \par 说明        设置交易中的交易性能    
 * \param[in]       stTermParam  交易性能结构体     
 * \retval          0 成功 -1  失败
 * \note                
 */
int tFunc_SetTermParam(ST_TERMPARA * stInTermParam)
{
    char szBuf[100] = {0};

	DbgPrintf("【设置交易性能】\n");

    /*<  清空全局结构体 */
    memset( &g_ICCardParam, 0x00, sizeof(g_ICCardParam) );

    /*< 使用默认的终端性能  */
    if ( stInTermParam == NULL )
    {
        tFunc_SaveTlvData( EMV_TAG_9F01_TM_ACQID, 2, "123456", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F15_TM_MCHCATCODE, 2, "01", 2, &g_ICCardParam.nIccAppDataLen,  g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F16_TM_MCHID, 2, "123456789012345", 15, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F39_TM_POSENTMODE, 2, "\x22", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F3C_TM_REFCURCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, "\xE0\xF0\xC8", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F40_TM_CAP_AD, 2,"\xFF\x00\xF0\xF0\x01", 5, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1C_TM_TERMID, 2, "1234", 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData  );
        tFunc_SaveTlvData( EMV_TAG_9F35_TM_TERMTYPE, 2, "\x22", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1E_TM_IFDSN, 2, "CENT_CKB", 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL, 2, "\x00\x00\x01\x00\x00\x00", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, "\x58\x00\x00\x00", 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F69_IC_CERTIFCATION_DATA, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_DF60_TM_CAPP, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

    }
    else
    {
		/*<  9F4E   9F42 95 */
		tFunc_SaveTlvData( EMV_TAG_9F01_TM_ACQID, 2, stInTermParam->szAcqID_n_9F01 , 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F15_TM_MCHCATCODE, 2, stInTermParam->szMerCateCode_n_9F15, 2, &g_ICCardParam.nIccAppDataLen,  g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F16_TM_MCHID, 2, stInTermParam->szMerID_ans_9F16, 15, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F39_TM_POSENTMODE, 2, (char *)stInTermParam->cEntryMode_n_9F39, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, stInTermParam->szCurCode_n_5F2A, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, stInTermParam->szTermCountryCode_b_9F1A, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F3C_TM_REFCURCODE, 2, stInTermParam->szRefCurrCode_n_9F3C, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, stInTermParam->szTermCap_b_9F33, 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F40_TM_CAP_AD, 2, stInTermParam->szAddTermCap_b_9F40, 5, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1C_TM_TERMID, 2, stInTermParam->szTermId_an_9F1C, 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData  );
		tFunc_SaveTlvData( EMV_TAG_9F35_TM_TERMTYPE, 2, (char *)stInTermParam->cTypeTerm_n_9F35, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1E_TM_IFDSN, 2, stInTermParam->szIFD_an_9F1E, 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, (char *)stInTermParam->cEcIndicator_9F7A, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL, 2, stInTermParam->cEcBalanceLimit_9F7B, 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, stInTermParam->szTransProp_9F66, 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F69_IC_CERTIFCATION_DATA, 2, (char *)stInTermParam->cSm, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_DF60_TM_CAPP, 2, (char *)stInTermParam->cAppExDF60, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

    }

    return RET_SUCCESS;
}

/*!
 * \brief        tFunc_SetAidList       
 * \par 说明     设置AID应用列表        
 * \param[in]    pszAidList  AID列表，AID应用以‘,’隔开    
 * \param[in]    nAidLen     AID列表的长度     
 * \retval       0 成功 -1  失败
 * \note                
 */
int tFunc_SetAidList ( char *pszAidList, int nAidLen )
{
    unsigned char sAid[ 16+1 ] = { 0x00 };
    char sTmpTag  = 0;
    int nAidPoint = 0;
    int nAidNum   = 0;
    
	DbgPrintf("【设置应用列表】\n");
    memset( s_AidList, 0x00, sizeof(s_AidList) );

    /*<  使用默认的AID */
    if ( pszAidList == NULL )
    {
        memset( &s_AidList[0], 0x00, 32 );
        memcpy( &s_AidList[0], "A000000333010101", 16 );

        memset( &s_AidList[1], 0x00,32);
        memcpy( &s_AidList[1], "A000000333010102", 16 );
        
        Pboc_SetAidNum(2);
    }
    
    while (pszAidList != 0x00)
    {
        sTmpTag = *(pszAidList++);
        nAidLen--;

        if (nAidLen <= 0)
        {
            memcpy( &s_AidList[nAidNum], sAid, nAidPoint );
            memset( sAid, 0x00, sizeof(sAid) );
            nAidPoint = 0;
            nAidNum++;
            Pboc_SetAidNum( nAidNum );
            return RET_SUCCESS;
        }
        if (sTmpTag != ',')
        {
            sAid[nAidPoint++] = sTmpTag;
        }
        else
        {
            memcpy( &s_AidList[nAidNum], sAid, nAidPoint );
            memset( sAid, 0x00, sizeof(sAid) );
            nAidPoint = 0;
            nAidNum++;
            Pboc_SetAidNum( nAidNum );
        }
    }
    return RET_SUCCESS;
}

/*!
 * \brief         tFunc_SetInfoTag     
 * \par 说明      设置读取卡片信息中的TAG   
 * \param[in]     pszTagList  需要标签列表 Hex码    
 * \param[in]     nTagListLen  标签的数据长度       
 * \retval        无
 * \note                
 */
void tFunc_SetInfoTag ( char *pszTagList[],int nNum )
{
    int i = 0;

	DbgPrintf("【设置信息标签】\n");
    for ( i = 0; i < nNum; i ++ )
    {
		memset( s_InfoTag[i], 0x00, 3 );
        memcpy( s_InfoTag[i], pszTagList[i], strlen(pszTagList[i]) );
    }
}

/*!
 * \brief          tFunc_SetMesgTag    
 * \par 说明       设置报文信息     
 * \param[in]      pszArqc   产生55域的报文格式     
 * \param[in]      pszTc     TC报文的数据格式       
 * \retval         无
 * \note                
 */
void tFunc_SetMesgTag( char *pszArqc[], int nNumArqc, char *pszTc[], int nNumTc )
{
    int i = 0;
	
	DbgPrintf("【设置报文信息】\n");

    memset( s_ArqcTag, 0x00, sizeof(s_ArqcTag) );
    memset( s_TcTag, 0x00, sizeof(s_TcTag) );

    /*<  设置ARQC报文 */
    if ( ( pszArqc != NULL )  && ( nNumArqc != 0 ) )
    {
        
        for( i = 0; i<nNumArqc; i++ )
        {
            memset( s_ArqcTag[i], 0x00, 3 );
            memcpy( s_ArqcTag[i], pszArqc[i], strlen(pszArqc[i]) );
        }
    }
    else
    {
        for( i = 0; i < TABLESIZE(s_ArqcTagDefault); i++)
        {
            memset( s_ArqcTag[i], 0x00, 3 );
            memcpy( s_ArqcTag[i], s_ArqcTagDefault[i], strlen(s_ArqcTagDefault[i]) );
        }
    }

    /*<  设置TC报文 */
    if ( ( pszTc != NULL ) && ( nNumTc != 0 ) )
    {
        
        for( i = 0; i < nNumTc; i++ )
        {
            memset( s_TcTag[i], 0x00, 3 );
            memcpy( s_TcTag[i], pszTc[i], strlen(pszTc[i]) );
        }
    }
    else
    {
		DbgPrintf("【设置报文信息】 设置默认报文\n");
        for( i = 0; i < TABLESIZE(s_TcTagDefault); i++ )
        {
            memset( s_TcTag[i], 0x00, 3 );
            memcpy( s_TcTag[i], s_TcTagDefault[i], strlen(s_TcTagDefault[i]) );
        }
    }
    return;

}


int Pboc_TransAppSelectByAid( unsigned char* szAid ,int nAidLen, unsigned char *pFci, int *pFciLen )
{
	//  应用被选择并确认交易：根据选择命令（SELECT）返回PDOL的TL信息并确认应用是否被锁定
	if( RET_SUCCESS !=  ICC_AppSelect(szAid, nAidLen, pFci, pFciLen ) )
	{
		if( 0x6283 == ICC_CardCmdGetErrCode() )
		{
			return RET_CARD_LOCK ;
		}
		else if ( 0x9303 == ICC_CardCmdGetErrCode())
		{
			return RET_CARD_LOCK_FOREVER ;
		}
		else
		{
			return RET_ERROR;
		}
	}
	return RET_SUCCESS;
}

/*!
 * \brief        tFunc_SetIcAidList        
 * \par 说明     在目录选择之后，设置IC卡AID应用列表   	
 * \param[in]    nNum         AID列表数 从0开始     	
 * \param[in]    szAid        卡片中AID   
 * \param[in]	 nAidLen      AID长度
 * \param[in]    szAppTag     AID应用名称
 * \param[in]    nAppTagLen   AID应用名称长度
 * \param[in]    szApn        AID优先名称
 * \param[in]    nApnLen      AID优先名称长度
 * \param[in]    cAPPpriority AID优先级	
 * \retval      
 * \note                
 */
void tFunc_SetIcAidList(int nNum,unsigned char *szAid, int nAidLen, unsigned char *szAppTag, int nAppTagLen, unsigned char *szApn, int nApnLen,  char cAPPpriority)
{
	if ( nNum == 0)
	{
		memset( g_IcAidList, 0x00, AID_LIST_LEN );
	}
	if ( nAidLen > 0 )
	{
		memcpy( g_IcAidList + nNum* 53, szAid ,nAidLen );
		g_IcAidList[ nNum* 53 + 16 ] = (char)nAidLen;
	}
	if ( nAppTagLen >0 )
	{
		memcpy( g_IcAidList + nNum* 53 +17, szAppTag ,nAppTagLen );
		g_IcAidList[ nNum* 53 + 16 + 17] = (char)nAidLen;
	}
	if ( nApnLen >0 )
	{
		memcpy( g_IcAidList + nNum* 53 + 34, szApn ,nApnLen );
		g_IcAidList[ nNum* 53 + 16 + 34 ] = (char)nApnLen;
	}

	g_IcAidList[ nNum*53 + 51 ] = cAPPpriority;

	g_IcAidList[ nNum*53 + 52 ] = '|';
}
/*!
 * \brief       tFunc_GetIcAidList       
 * \par 说明    以一定格式返回目录选择之后，AID列表   	
 * \param[out]	szOutAidList	卡片中AID应用列表
 * \retval      0 成功 -1  失败
 * \note                
 */
int  tFunc_GetIcAidList(char * szOutAidList)
{
	if ( g_nAidListNum == 0)
	{
		return FAIL;
	}
	
	memcpy(szOutAidList,g_IcAidList,AID_LIST_LEN);
	return RET_SUCCESS;
}

/*!
 * \brief        tFunc_App_SelectInit      
 * \par 说明     应用选择、初始化           
 * \retval       0 成功 -1  失败
 * \note                
 */
int tFunc_AppSelectInit()
{
    int nRet = 0;
    int nAFLlen = 0; // AFL 长度
    int nAIPlen = 0; // AIP 长度
	int nAtrLen = 0;
	int nAidNum = 0;
	int nAidLen = 0;
	int nFciLen = 0;
	int nErrCod = 0;
	int nIndex = 0;
	int nAIDNum = 0;
	int i = 0, nFlag = 1;
    unsigned char uszAFL[512] = {0x00};    // AFL 
    unsigned char uszAIP[4+1] = {0x00};    // AIP 
	unsigned char uszAtr[128] = {0x00};
	unsigned char uszAID[32] = {0x00};
	unsigned char uszFci[1024] = {0x00};
	AID_LIST *pAidList = NULL;

	DbgPrintf("【应用选择、初始化】\n");

	memset( uszAtr, 0x0, sizeof(uszAtr) );  
	nRet = ICC_CardCmdSelectFile( 0x04, 0x00, "1PAY.SYS.DDF01", strlen("1PAY.SYS.DDF01"), uszAtr,  &nAtrLen );
	DbgPrintHex("【应用选择、初始化】 文件选择结果", uszAtr, nAtrLen );
	if( RET_SUCCESS !=  nRet )
	{
		
		if( 0x6A82 ==  ICC_CardCmdGetErrCode()  )
		{
			DbgPrintf("【应用选择、初始化】 不支持目录选择\n");
			goto AidSel;
		}
		return nRet;
	}

	DbgPrintf("【应用选择、初始化】 应用列表\n");
	nRet =  ICC_GetAppList(  &pAidList,  &nAidNum  );
 	if( RET_SUCCESS != nRet)
 	{
 		ICC_FreeAidList( pAidList );
 		return RET_GET_APP_FAIL;
 	}

AidSel:
 	if( pAidList != NULL ) /*<  支持目录选择  */
 	{
		while( nFlag )
		{
			nAidLen = pAidList[i].AidLen;
			memcpy( uszAID, pAidList[i].AID, nAidLen );
			
			/*< 保存AID应用列表  */
			tFunc_SetIcAidList( i, pAidList[i].AID, pAidList[i].AidLen,  pAidList[i].AppTag,\
				pAidList[i].AppTagLen, pAidList[i].APN, pAidList[i].APNlen, pAidList[i].APPpriority );

			DbgPrintf("【应用选择、初始化】 AID匹配\n");
			if ( YES ==  Pboc_IsAIDMatch( uszAID, nAidLen, &nIndex ) )
			{
				DbgPrintf("【应用选择、初始化】 AID 应用选择\n");
				nRet  =  Pboc_TransAppSelectByAid( uszAID, nAidLen, uszFci, &nFciLen );
				if (nRet == RET_SUCCESS)
				{
					tFunc_SaveTlvData( EMV_TAG_6F_IC_FCITEMP, 1, uszFci, nFciLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
				}
			}
			if (pAidList[i].next != NULL)
			{
				i++;
			}
			else
			{
				nFlag = 0;
				if (nRet != RET_SUCCESS)
				{
					return nRet;
				}
			}	
		}
 	}
 	else /*<  不支持目录选择的时候  进行AID列表选择  */
 	{
		DbgPrintf("【应用选择、初始化】 应用列表选择\n");
		nRet = Pboc_Meth_AidList( &nAIDNum );
		if(nRet != SUCC)
		{
			return nRet;
		}
 	}
	/*<  释放AID列表指针 */
	DbgPrintf("【应用选择、初始化】 释放AID列表指针\n");
 	ICC_FreeAidList( pAidList );

	/*<  应用初始化 */
	DbgPrintf("【应用选择、初始化】 应用初始化\n");
	nRet =  ICC_AppInit( BANGK, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, uszAFL, &nAFLlen, uszAIP, &nAIPlen );
	if( nRet != 0 )
	{
		return RET_INIT_FAIL;
	}

	/*<  获取  */

	/*< AIP  */
	tFunc_SaveTlvData( EMV_TAG_82_IC_AIP, 1, uszAIP, nAIPlen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
	/*<  存储AFL  */
	tFunc_SaveTlvData( EMV_TAG_94_IC_AFL, 1, uszAFL, nAFLlen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
	DbgPrintf("【应用选择、初始化】 返回值 %d\n",nRet);
	return nRet;

}



/*!
 * \brief        tFunc_GetDataByTag      
 * \par 说明     从交易过程中通过TAG 获取标签值，如果为静态数据，则通过80 CA获取静态数据    
 * \param[in]    sTag      需要获取的TAG        
 * \param[in]    nTagLen   TAG标签的长度       
 * \param[out]   nValueLen 标签值的长度
 * \param[out]   sValue    标签值(Hex码格式)
 * \retval       0 成功 -1  失败
 * \note                
 */
int tFunc_GetDataByTag(char * sTag,int nTagLen,int * nValueLen,char * sValue)
{
    int nRet = 0;
    int nBufLen = 0;
    int i = 0;
    int nFlag = 0;
    int nDataLen =0;
    char sBuf[1024] = {0x00};
    char sData[128] = {0x00};

	DbgPrintf("【TAG获取】\n");
    *nValueLen = 0;

    /*< 查看TAG 是否为静态数据  */
    for( i =0; i<TABLESIZE(sStaticTagList); i++ )
    {
        if (memcmp ( sTag, sStaticTagList[i], nTagLen )   == 0 )
        {
            nFlag = 1;
            memset( sData, 0x00, sizeof(sData) );
            nDataLen = 0;
            nRet =  Pboc_GetIcStaticData(sTag, nTagLen, sData,&nDataLen);
            if (nRet == RET_SUCCESS)
            {
                centerm_get_tag_value( sTag, nTagLen, sData, nDataLen, sValue, nValueLen );
            }
        }
    }
    if (nFlag  == 0)
    {
        nRet = centerm_get_tag_value( sTag, nTagLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sBuf, &nBufLen );
        if (nRet == RET_SUCCESS)
        {
            memcpy( sValue, sBuf, nBufLen );
            *nValueLen = nBufLen;
        }
    }

    return nRet;
}
/*!
 * \brief          tFunc_GetIcCardInfo    
 * \par 说明       读取卡片信息 按照传入的Info Tag 列表 进行数据格式进行TLV组包
 * \param[out]     pnUsrInfoLen  数据长度
 * \param[out]     pszUserInfo   数据
 * \retval         0 成功 -1  失败
 * \note                
 */
int tFunc_GetIcCardInfo(int * pnUsrInfoLen,char *pszUserInfo)
{
    int i = 0;
    int nRet = 0;
    int nBufLen = 0;
    int nLen = 0;
    int nDataLen  =0;
    char sBuf[1024] = {0x00};
    char sData[1024] = {0x00};
    
	DbgPrintf("【获取卡片信息】\n");
    /*<  从交易过程数据中读取标签 */
    for ( i =0; i<TABLESIZE(s_InfoTag); i++ )
    {
        nBufLen = 0;
        memset( sBuf, 0x00, sizeof(sBuf) );
        nRet = tFunc_GetDataByTag( s_InfoTag[i], strlen(s_InfoTag[i]), &nBufLen, sBuf );
        DbgPrintHex("Get data s_InfoTag", s_InfoTag[i], strlen(s_InfoTag[i]) );

        if (nBufLen>0)
        {
            memset(sData,0x00,sizeof(sData));
            nRet = centerm_packtTag( strlen((char *)s_InfoTag), s_InfoTag[i], nBufLen, sBuf, &nDataLen, sData );
            
            memcmp( pszUserInfo + nLen, sData, nDataLen );
            nLen += nDataLen;
        }
    }
    *pnUsrInfoLen = nLen;

    return RET_SUCCESS;
}

/*!
 * \brief         tFunc_SetAppTransactionTag     
 * \par 说明      设置应用交易过程中的信息  
 * \param[in]     pszTranceInfo  ASC码格式，接口需要设置的交易信息      
 * \retval        0 成功 -1  失败
 * \note          格式为9F0206000010000000        
 */
int tFunc_SetAppTransactionTag ( char *pszTranceInfo)
{
    int nRet = 0;
    int nBufLen =0;
    char sBuf[1024] = {0x00};
    char sData[8+1] = {0x00};
    char sTagValue[256] = {0x00};
    int nTagValueLen  = 0;

	DbgPrintf("【设置交易信息】\n");
    /*<  应用没有下发交易数据 则使用默认的交易数据   在默认数据没有此TAG元素则增加 否则则覆盖*/
    /*<  由于当前的TLV格式限制 暂时存放于另一个全局变量中 */
    if (strlen(pszTranceInfo) != 0)
    {
        /*< 转换交易格式  */
        memset( &g_sParam.szTermData, 0x00, sizeof(g_sParam.szTermData) );
        AsciiToHex( pszTranceInfo, strlen(pszTranceInfo), sBuf );
        nBufLen = strlen(pszTranceInfo)/2;
        
        DbgPrintHex("【设置交易信息】 g_ICCardParam.szIccAppData before \n", g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen );

        /*!<授权金额*/
        nRet = centerm_get_tag_value( EMV_TAG_9F02_TM_AUTHAMNTN, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 6;
            memcpy( sTagValue, "\x00\x00\x00\x00\x00\x00", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 授权金额\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F02_TM_AUTHAMNTN, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<其它金额*/
        nRet = centerm_get_tag_value( EMV_TAG_9F03_TM_OTHERAMNTN, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
            nTagValueLen = 6;
            memcpy( sTagValue, "\x00\x00\x00\x00\x00\x00", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 其它金额\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F03_TM_OTHERAMNTN, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!<终端国家代码*/ 
        nRet = centerm_get_tag_value( EMV_TAG_9F1A_TM_CNTRYCODE, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 2;
            memcpy( sTagValue, "\x01\x56", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 终端国家代码\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!< Check 持卡人验证结果*/
        tFunc_SaveTlvData( EMV_TAG_9F34_TM_CVMRESULT, 2, "\x3f\x00\x00", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!< Check 交易状态信息*/
        tFunc_SaveTlvData( EMV_TAG_9B_TM_TSI, 1, "\x00\x00", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!<终端验证结果*/
        nRet = centerm_get_tag_value( EMV_TAG_95_TM_TVR, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 5;
            memcpy(  sTagValue, "\x00\x00\x00\x08\x00", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 终端验证结果\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_95_TM_TVR, 1, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<交易货币代码*/
        nRet = centerm_get_tag_value( EMV_TAG_5F2A_TM_CURCODE,  2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
            nTagValueLen = 2;
            memcpy( sTagValue, "\x01\x56", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 交易货币代码\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<交易类型*/
        nRet = centerm_get_tag_value( EMV_TAG_9C_TM_TRANSTYPE, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 1;
            memcpy( sTagValue, "\x00", nTagValueLen );
        }
		DbgPrintHex("【设置交易信息】 交易类型\n", sTagValue, nTagValueLen);
		tFunc_SaveTlvData( EMV_TAG_9C_TM_TRANSTYPE, 1, sTagValue, nTagValueLen,  &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		


        /*!<交易日期*/
        nRet = centerm_get_tag_value( EMV_TAG_9A_TM_TRANSDATE, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
			/*< 获取系统交易日期  */
            nTagValueLen = 3;
            memset( sData, 0, sizeof(sData) );
            GetTransDate( sData );
            AsciiToBcd( sData, 6,sTagValue, 3 );
        }
		DbgPrintHex("【设置交易信息】 交易日期\n", sTagValue, nTagValueLen);
		/*<  处理交易日期的格式 */
        if (nTagValueLen == 4)
        {
            tFunc_SaveTlvData( EMV_TAG_9A_TM_TRANSDATE, 1, sTagValue + 1, nTagValueLen - 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
        else
        {
            tFunc_SaveTlvData( EMV_TAG_9A_TM_TRANSDATE, 1, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
        


        /*!<交易时间*/
        nRet = centerm_get_tag_value( EMV_TAG_9F21_TM_TRANSTIME, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
			/*< 获取系统交易时间  */
            nTagValueLen = 3;
            memset( sData, 0, sizeof(sData) );
            GetTransTime( sData );
            AsciiToBcd( sData, 6,sTagValue, 3 );
        }
		DbgPrintHex("【设置交易信息】 交易时间\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F21_TM_TRANSTIME, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<随机数*/
        nRet = centerm_get_tag_value( EMV_TAG_9F37_TM_UNPNUM, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 4;
            GetRandom( 4, sTagValue );
        }
        tFunc_SaveTlvData( EMV_TAG_9F37_TM_UNPNUM, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );



        /*!<终端交易属性*/
        nRet = centerm_get_tag_value( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 4;
            memcpy( sTagValue, "\xF4\x00\x00\x00", 4 );
        }
		DbgPrintHex("【设置交易信息】 终端交易属性\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, sTagValue,nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<电子现金终端支持指示器*/
		nRet = centerm_get_tag_value( EMV_TAG_9F7A_TM_EC_TSI, 2,  sBuf, nBufLen, sTagValue, &nTagValueLen );
		if ( nRet != RET_SUCCESS )
		{
			nTagValueLen = 1;
			memcpy( sTagValue, "\x01", 1 );	
		}
		DbgPrintHex("【设置交易信息】 电子现金指示器\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<电子现金终端交易限额*/
        tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL,2,"\x00\x00\x01\x00\x00\x00", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!< 商户名称*/
        nRet = centerm_get_tag_value( EMV_TAG_9F4E_TM_NAMELOC, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 20;
            memset( sTagValue, 0x20, 21 );
            memcpy( sTagValue, "CKB 5150", 8 );
        }
		DbgPrintHex("【设置交易信息】 商户名称\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F4E_TM_NAMELOC, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*< 终端性能  */
		DbgPrintf("【设置交易信息】 终端性能\n");
        tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, "\xE0\xF0\xC8", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        DbgPrintHex("tFunc_SetAppTransactionTag g_ICCardParam.szIccAppData fater \n",g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen );
	}

    return nRet;
}


/*!
 * \brief       Pboc_ExecuteGacCmd       
 * \par 说明    执行GAC命令     
 * \param[in]   Cmd  表示GAC的命令类型          
 * \param[in]   szIccData     CDOL数据       
 * \param[in]   nIccDataLen   CDOL数据长度
 * \param[out]  szAc          GAC 响应数据
 * \param[out]  pnAcLen       GAC 响应数据长度  
 * \retval      0 成功 -1  失败
 * \note                
 */
int Pboc_ExecuteGacCmd( U8 Cmd, U8* szIccData, S32 nIccDataLen, U8* szAc, S32* pnAcLen )
{
    U8  P1 = 0x0;
    S32 nRetVal = RET_SUCCESS;
    U8  szOutData[ MAX_LEN ] = {0};
    S32 nOutDataLen = 0;

    switch( Cmd )
    {
    case CMD_GAC_ARQC:
        {
            P1 = 0x80;
            break;    
        }  
    case CMD_GAC_TC:
        {
            P1 = 0x40;
            break;    
        } 
    case CMD_GAC_AAC:
        {
            P1 = 0x00;
            break;    
        }
    default:
        {
            DbgPrintf("执行GAC命令码错误\n");
            return RET_CMD_ERR;    
        }                     
    }

    nRetVal = ICC_CardCmdGenerateAC( P1, szIccData, nIccDataLen, szOutData, &nOutDataLen );
    DbgPrintHex("GAC命令响应数据", szOutData, nOutDataLen );
    memcpy( szAc, szOutData, nOutDataLen );
    *pnAcLen = nOutDataLen;
    return nRetVal;
}
/*!
 * \brief            tFunc_GenARQC  
 * \par 说明         生成ARQC   
 * \param[out]       pnArqcLen  产生ARQC数据输出长度
 * \param[out]       pszArqc    ARQC数据(ASC 数据格式)
 * \retval           0 成功 -1  失败
 * \note                
 */
int tFunc_GenARQC (int * pnArqcLen, char *pszArqc)
{
    int nRet = RET_PARAM_ERR;
    int i = 0, nF55Len =0;
    int nCDOL1DataLen = 0;
    int nARQClen =0;
    int nBufLen =0;
    int nDOLLen =0;
    int nArqcHexLen  = 0;
    unsigned char uzCDOL1Data[256] = {0x00};
    unsigned char uszARQC[1024] = {0x00};
    unsigned char sBuf[1024] = {0x00};
    unsigned char  DOL[MAX_LEN];
    char szF55[512] = {0x00};
    char sArqcHex[512] = {0x00};
	char sData[1024] = {0x00};
	int nDataLen = 0;

    *pnArqcLen = 0;
    DbgPrintf("【ARQC】\n");
	
	/*<  倒叙读取 CDOL1 CDOL2  */
	nRet =  tFunc_ReadRecordData( 2,NULL, 2, &nDataLen , sData);
	if ( nRet == RET_SUCCESS )
	{
		/*< 打包CDOL1 数据  */
		nRet =  ICC_GetCDOL1( sData, nDataLen, DOL, &nDOLLen );
		DbgPrintHex("【ARQC】 获取CDOL1 ", DOL, nDOLLen );
		if ( nRet != RET_SUCCESS )
		{
			return RET_GET_CDOL1_FAIL;
		}
		nRet =  ICC_GetIccDataByDOL( DOL, nDOLLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, uzCDOL1Data, &nCDOL1DataLen );
		if(nRet != RET_SUCCESS)
		{
			DbgPrintf("【ARQC】 获取CDOL1 失败\n");
			return RET_GET_CDOL1_FAIL;
		}
		/*<  赋值数据 */
		if ( nDOLLen >0 )
		{
			memcpy( g_ICCardParam.szIccAppData + g_ICCardParam.nIccAppDataLen, sData, nDataLen);
			g_ICCardParam.nIccAppDataLen += nDataLen;
		}

	}


  
    /*< GAC 请求ARQC  */
	DbgPrintf("【ARQC】 GAC1 请求ARQC\n");
    memset( uszARQC, 0, sizeof(uszARQC) );
    nRet = Pboc_ExecuteGacCmd( CMD_GAC_ARQC, uzCDOL1Data, nCDOL1DataLen, uszARQC, &nARQClen );
    if( nRet != 0 )
    {
        if( ICC_CardReaderGetRecStatus( ) == -3 )//超时
        {
            return RET_TIMEOUT_ERR;
        }
        return RET_GEN_ARQC_FAIL;
    }

    DbgPrintHex("【ARQC】 卡片返回GAC数据", uszARQC, nARQClen );
    nRet =  Pboc_GetGACData( uszARQC, nARQClen);
    if ( nRet != RET_SUCCESS )
    {
		DbgPrintf("【ARQC】 解析GAC数据失败\n");
        return RET_PARASE_GAC_ERR;
    }
    
	/*<  打包数据 */
	DbgPrintf("【ARQC】 打包ARQC数据 \n");
    nRet = Pboc_PackDomain( 0, &nArqcHexLen , sArqcHex );
    if (nRet != RET_SUCCESS)
    {
		DbgPrintf("【ARQC】 打包ARQC数据 失败\n");
        nRet = RET_GEN_ARQC_FAIL;
    }
    
    /*<  转换数据 将HEX码转换成ＡＳＣＩＩ码  */
    HexToAscii( sArqcHex, nArqcHexLen, pszArqc );
    *pnArqcLen = 2*nArqcHexLen;
	DbgPrintf("【ARQC】ARQC数据 %s \n",pszArqc);
    return nRet;
}

/*!
 * \brief        tFunc_IssueAuth      
 * \par 说明     发卡行认证     
 * \param[in]    pszARPC  ASCII码格式       
 * \param[in]    nARPCLen   ARPC的数据长度    
 * \param[out]   szAuthCode  发卡行认证相应码
 * \param[out]      
 * \retval              0 成功 -1  失败
 * \note                
 */
int tFunc_IssueAuth (char *pszARPC, int nARPCLen,char * szAuthCode)
{
    char sArpcHex[256] = {0x00};  //最长为12
    int  nArpcHexLen = 0;
    int  nRet = RET_SUCCESS;

    DbgPrintf("【发卡行认证】 pszARPC %d  %s\r\n", nARPCLen, pszARPC ); 

    /*<  转换数据格式 */
    memset( sArpcHex, 0x00, sizeof(sArpcHex) );
    AsciiToHex( pszARPC, nARPCLen, (char *)sArpcHex );
    nArpcHexLen = nARPCLen/2;
    if( nArpcHexLen < 2 )
    {
        DbgPrintf("ARPC长度小于1\n"); 
        return RET_ERROR;
    }

    /*<  发卡行认证 */  
    memset(szAuthCode, 0x00, sizeof(szAuthCode));
    nRet = ICC_BankAuthentication( (unsigned char *)sArpcHex, nArpcHexLen, szAuthCode );
    if(nRet < 0)
    {
        DbgPrintf("【发卡行认证】 认证失败  ERR %s \n", ICC_CardCmdShowLastErrMsg() );
        return RET_FAIL_EXT_AUTH;
    }

    DbgPrintf("【发卡行认证】 授权响应码  %s\n", szAuthCode);

    return RET_SUCCESS;
}
/*!
 * \brief         tFunc_CompleteTransaction     
 * \par 说明      交易完成处理  
 * \param[in]     in_pszAuthCode    
 * \retval        0 成功 -1  失败
 * \note                
 */
int tFunc_CompleteTransaction(unsigned char * in_pszAuthCode)
{
    int nRet = RET_SUCCESS, nRespLen = 0, nCDOL2DataLen =0, nDOLLen =0; 
	unsigned char szRespData[256] = {0x00}; 
    unsigned char sCDOL2Data[256] = {0x00};
    unsigned char DOL[256] = {0x00};

	DbgPrintf("【生产GAC2】 \n");
    /*<  按格式组织CDOL2 数据  保存终端和发卡行的授权响应码 */
    tFunc_SaveTlvData( EMV_TAG_8A_TM_ARC, 1, in_pszAuthCode, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
    nRet =  ICC_GetCDOL2( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, DOL, &nDOLLen );
    if (nRet != RET_SUCCESS)
    {
        return RET_ERROR;
    }
	DbgPrintf("【生产GAC2】 获取CDOL2 数据成功 \n");

    /*<  打包CDOL2 数据 */
    nRet =  ICC_GetIccDataByDOL( DOL, nDOLLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sCDOL2Data, &nCDOL2DataLen );
    DbgPrintHex("ICC_GetCDOL2 DOL",sCDOL2Data, nCDOL2DataLen );
    if(nRet != RET_SUCCESS)
    {
        DbgPrintf("【生产GAC2】 打包CDOL2 数据失败 \n");
        return RET_ERROR;
    }
	DbgPrintf("【生产GAC2】 打包CDOL2 数据成功\n");

    /*<  第二次GAC数据  */
    DbgPrintf("【生产GAC2】 发送GAC2 命令 \n");
    memset( szRespData, 0, sizeof(szRespData) );
    nRet = ICC_ClosedTrade( in_pszAuthCode, sCDOL2Data, nCDOL2DataLen, szRespData, &nRespLen );
    if( RET_SUCCESS != nRet )
    {
		DbgPrintf("【生产GAC2】 GAC 失败 %d \n",ICC_CardCmdShowLastErrMsg());
        nRet = RET_CMD_ICGenAAC_FAIL;
    }
	DbgPrintf("【生产GAC2】 返回值 %d \n",nRet);

    return nRet;
}


/*!
 * \brief         tFunc_WriteScript     
 * \par 说明      处理脚本  
 * \param[in]     pszScript     下发的脚本数据  
 * \param[in]     nScriptLen    脚本数据的长度     
 * \param[out]    sResult       脚本结果    
 * \retval        0 成功 -1  失败
 * \note                
 */
int tFunc_WriteScript(char *pszScript, int nScriptLen,char *sResult)
{
    int nRet = RET_SUCCESS;

    DbgPrintf("【写脚本】\n");
    nRet = ICC_WriteScript( pszScript, nScriptLen, sResult);
    DbgPrintf("【写脚本】 返回值 %d 脚本结果 %s \n",nRet,sResult);
    return nRet;
}

/*!
 * \brief          tFunc_GetTC    
 * \par 说明       组织TC报文数据格式   
 * \param[out]     *pnTCLen   TC报文的数据长度
 * \param[out]     pszTC      TC报文数据 （ASC码数据格式）
 * \retval         0 成功 -1  失败
 * \note                
 */
int tFunc_GetTC (int *pnTCLen ,char *pszTC)
{
    int nRet = 0;
    int nTcHexLen  = 0;
    char sTcHex[1024] = {0x00};
    
	DbgPrintf("【生产TC】\n");
    nRet = Pboc_PackDomain( 1, &nTcHexLen, sTcHex);
    if (nRet != RET_SUCCESS)
    {
        nRet = RET_GEN_TC_FAIL;
    }

    /*<  转换数据 将HEX码转换成ＡＳＣＩＩ码  */
    HexToAscii( sTcHex, nTcHexLen, pszTC);
    *pnTCLen = 2*nTcHexLen;

	DbgPrintf("【生产TC】数据:%s \n",pszTC);
    return RET_SUCCESS;
}


/*!
 * \brief        tFunc_GetLog      
 * \par 说明     获取交易日志、圈存日志     
 * \param[in]    nType          0x01 交易日志  0x02 圈存日志        
 * \param[out]   nNum           日志条数     
 * \param[out]   pnTxDetailLen  日志信息总长度
 * \param[out]   TxDetail       日志信息数据
 * \param[int]   TranceDetail   读取日志回调函数
 * \retval       0 成功 -1  失败
 * \note         应用选择（tFunc_AppSelectInit） + 读取日志信息（tFunc_GetLog）          
 */
int tFunc_GetLog ( int nType,int* nNum,int *pnTxDetailLen, char *TxDetail,TranceDetail pDetail)
{
    int nRet = 0;
    int nBufLen = 0;
    int nLogSum =0;
    int i =0;
    unsigned char uszBuf[512] = {0};
    unsigned char ucRecordID = 0;
    unsigned char szLogFormat[32]={0x00};
    unsigned char bSfi = '0';
    unsigned char bNum = '0';

	DbgPrintf("【获取日志数据】\n");

    /*<  交易日志 */
    if (nType == 0x01 )
    {
        nRet = ICC_GetLogEntry( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &bSfi, &bNum );
		DbgPrintf("【获取日志数据】 交易日志格式 返回值 %d \n",nRet);
    }
    /*<  圈存日志 */
    else if (nType == 0x02)
    {
        nRet = ICC_GetCreditLoadLogEntry( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &bSfi, &bNum );
		DbgPrintf("【获取日志数据】 圈存日志格式 返回值 %d \n",nRet);
	}
    else
    {
        return RET_PARAM_ERR;
    }

    if(nType == 0x01) //交易格式
    {
        /*<  获取9F4D 交易格式 */
        memcpy( szLogFormat, "\x9F\x4F\x19\x9A\x03\x9F\x21\x03\x9F\x02\x06\x9F\x03\x06\x9F\x1A\x02\x5F\x2A\x02\x9F\x4E\x14\x9C\x01\x9F\x36\x02", 28);
        nLogSum = 28;
    }
    else  //圈存格式
    {
        /*< 获取DF4D交易格式  */
        memcpy( szLogFormat, "\xDF\x4F\x0E\x9A\x03\x9F\x21\x03\x9F\x1A\x02\x9F\x4E\x14\x9F\x36\x02", 17);
        nLogSum = 17;
    }

    if ( nRet != RET_SUCCESS )
    {
        return nRet;
    }
    
    /*< 读取日志数据  */
    for( ucRecordID = 1; ucRecordID < bNum; ucRecordID++ )
    {
        memset( uszBuf, 0x00, sizeof(uszBuf) );
        nRet = ICC_GetLog( bSfi,  ucRecordID, uszBuf, &nBufLen );
		DbgPrintf("【获取日志数据】 读取日志信息 返回值 %d \n",nRet);
        if ( nRet != RET_SUCCESS )
        {
            break;
        }
        Pboc_FormatLog( szLogFormat, nLogSum, uszBuf, nBufLen, &stPbocList[i] );

        /*<  组织日志格式  如果回调函数存在 则按照回调函数格式进行数据组织  */
        if ( pDetail != NULL)
        {
            pDetail( &stPbocList[i], i+1, TxDetail );
        }
        else
        {
            if ( nType == 0x01)
            {
                Pboc_OrgLogDetail( stPbocList[i], i+1, TxDetail );
            }
            else
            {
                Pboc_OrgCredLogDetail( stPbocList[i], i+1, TxDetail );
            }
        }
        i++;
    }

    DbgPrintf("【获取日志数据】 数据 :%s \n", TxDetail);
    return RET_SUCCESS;
}

/*!
 * \brief          tFunc_DeviceAuth    
 * \par 说明       设置安全认证 	    
 * \retval         0 安全认证成功 -1  安全认证失败
 * \note                
 */
int tFunc_DeviceAuth()
{
	int nRet = RET_SUCCESS, i = 0, nRandomAuthFlag = 0, nBufLen = 0;
	char sRandom[32+1] = {0x00};
	char sRandomHex[16+1] = {0x00};
	char szEncRandom[32] = {0x00};    /*<  加密后的随机数  */
	char szRecvEncData[32] = {0x00};  /*<  接收到的密文数据 */
	char szTemp[64] = {0x00};
	char szCmd[1024] = {0x00};
	char szBuf[256] = {0x00};
	char szDivEncData[64] = {0x00};
	char szNewRandom[32] = {0x00};
	char szCheckStr[16]= "CENTERM==ZNZDCPB" ;
	char szNormalKey[16] = {0x00};
	char szNormalKey1[16] = "\x7E\x1A\xD6\x40\x59\xAE\xC7\x1C\xF1\xAB\xCA\x33\x49\x94\x8C\x75";
	char szNormalKey2[16] = "\xE0\xD7\x3D\xB9\x6D\x2A\x5F\x95\xB2\xAF\x22\x3D\x57\xBD\x9A\x7A";
	char szRandomKey1[16] = "\x41\x15\x89\x07\x62\x8E\x21\xC0\xCB\xF2\x75\xE0\x40\x39\x26\xEB";
	char szRandomKey2[16] = "\x30\x4C\xC2\x19\x68\x35\xD6\xE8\x59\x6A\xFF\x15\x8A\xEE\x6F\x6F";
	char szXorData[32] = {0x00}; 
	char szRandomKey[16] = {0x00};


	/*< 不进行安全认证  */
#ifndef DEVICE_AUTH
		return RET_SUCCESS;
#endif
	DbgPrintf("【设备安全认证】\n");

	/*<  获取随机数 R1*/
	GetRandom( 16, sRandomHex );
	DbgPrintHex("【设备安全认证】 获取随机数\n",sRandomHex , 16 );

	/*< 正常秘钥加密随机数R1 产生 VerifyData1 */
	DbgPrintf("【设备安全认证】 正常秘钥加密随机数\n");
	for ( i = 0; i < DEVICE_KEY_LEN; i++ )
	{
		szNormalKey[i] = (BYTE)(szNormalKey1[i]^szNormalKey2[i]);
	}
	for ( i = 0; i < DEVICE_KEY_LEN; i++ )
	{
		szRandomKey[i] = (BYTE)(szRandomKey1[i]^szRandomKey2[i]);
	}
	ExEncryptDES_BYTE( szNormalKey, DEVICE_KEY_LEN, sRandomHex, 16, szEncRandom);

	/*< 拆分随机数密文  */
	memset( szBuf, 0x00, sizeof(szBuf) );
	SplitData( (char *)szEncRandom, DEVICE_KEY_LEN, szBuf );
	if (((sRandomHex[0]^sRandomHex[9]) == 0xFF)&&((sRandomHex[3]^sRandomHex[11]) == 0xFF))
	{
		nRandomAuthFlag = 1;
	}

	/*<  组合设置版本信息指令 */
	sprintf( szCmd, "\x1B[0D\x02%s\x03", szBuf);

	/*<  下发指令并接收结果 */
	nBufLen = DEVICE_KEY_LEN*2 + 6;
	if ( nBufLen != ICC_WriteDataToTransPort( szCmd, nBufLen ) )
	{
		DbgPrintf("【安全认证】 发送数据失败 ");
		nRet = RET_DEVICE_AUTH_FAIL;
	}
	else
	{
		/*<  获取返回密文 */
		memset( szBuf, 0x00, sizeof( szBuf) );
		nBufLen = sizeof(szBuf);
		nRet =  ICC_ReadDataFromTransPort( szBuf, nBufLen) ;
		if (nRet <= 0)
		{
			return RET_DEVICE_AUTH_FAIL;
		}
		/*< 从第二个字节开始算起  */
		memcpy(szDivEncData, &szBuf[2], strlen(szBuf)-3);
		MergeData( szDivEncData, strlen(szDivEncData), szRecvEncData );

		/*<  打印设备返回密文 */
		memset(szTemp, 0, sizeof(szTemp));
		HexToAscii((BYTE*)szRecvEncData, strlen(szDivEncData)/2, szTemp, sizeof(szTemp) );
		DbgPrintf( "【安全认证】设备返回密文：%s", szTemp);

		/*<  解密密文 */
		if (nRandomAuthFlag == 1)
		{	
			ExDecryptDES_BYTE( szRandomKey, DEVICE_KEY_LEN, (BYTE *)szRecvEncData,strlen(szDivEncData)/2 , szXorData);
		}
		else
		{
			ExDecryptDES_BYTE( szNormalKey, DEVICE_KEY_LEN, (BYTE*)szRecvEncData, strlen(szDivEncData)/2, szXorData);
		}

		/*<  打印随机数异或结果 */
		memset( szTemp, 0, sizeof(szTemp));
		HexToAscii( szXorData, DEVICE_KEY_LEN, szTemp, sizeof(szTemp) );
		DbgPrintf( "【安全认证】随机数异或结果为：%s\n", szTemp);

		/*<  获取随机数 */
		for ( i = 0; i < 16; i++)
		{
			szNewRandom[i] = (BYTE)(szXorData[i]^szCheckStr[i]);
		}

		/*<  打印随机数异或结果 */ 
		memset(szTemp, 0, sizeof(szTemp));
		HexToAscii( szNewRandom, DEVICE_KEY_LEN, szTemp);
		DbgPrintf( "【安全认证】随机数为：%s \n", szTemp);

		/*< 判断认证是否成功  */
		if (memcmp( sRandomHex, szNewRandom, DEVICE_KEY_LEN) == 0)
		{
			nRet = RET_SUCCESS;
			DbgPrintf("【安全认证】安全认证成功！\n");
		}
		else
		{
			nRet = RET_DEVICE_AUTH_FAIL;
			DbgPrintf("【安全认证】安全认证失败！\n");
		}
	}
	return nRet;
}

/*!
 * \brief        tFunc_GetPbocVersion        
 * \par 说明     获取PBOC的版本号  	
 * \param[out]   nPbocType   0 -- 2.0 卡片  1 -- 3.0卡片
 * \retval       
 * \note         通过判断卡片的9F08标签        
 */
void tFunc_GetPbocVersion(int  * pnPbocType)
{
	unsigned char sValue[1024] = {0x00};
	int nValueLen = 0, nRet = RET_SUCCESS;

	DbgPrintf("【获取卡片版本】");
	nRet = centerm_get_tag_value( EMV_TAG_9F08_IC_APPVERNO, 2, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &nValueLen, sValue );
	if( ( nRet == RET_SUCCESS ) && ( nValueLen > 0 ) )
	{
		if( memcmp( sValue, "\x00\x20", 2) == 0)	
		{
			*pnPbocType = 0;
		}
		else
		{
			*pnPbocType = 1;
		}
	}
	else
	{
		*pnPbocType = 0;
	}
}
